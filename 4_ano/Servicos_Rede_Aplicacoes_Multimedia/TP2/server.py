import socket
import struct
import time
import json
from PIL import Image
from io import BytesIO

# dicionário que associa os números e o separador a cada imagem
digit_images = {
    '0': 'digitos/zero.png',
    '1': 'digitos/um.png',
    '2': 'digitos/dois.png',
    '3': 'digitos/tres.png',
    '4': 'digitos/quatro.png',
    '5': 'digitos/cinco.png',
    '6': 'digitos/seis.png',
    '7': 'digitos/sete.png',
    '8': 'digitos/oito.png',
    '9': 'digitos/nove.png',
    ':': 'digitos/separador.png',
}

def load_digit_images():
    #carrega as imagens para a memoria
    images = {}
    for char, path in digit_images.items():
        images[char] = Image.open(path) #associão da imagem  à chave correspondente no dicionário images

    return images

def get_current_time(F):
    now = time.time()
    time_struct = time.localtime(now)

    time_str = time.strftime("%H:%M:%S", time_struct)

    
    if F >= 1:
        frac = str(now).split('.')[1][:F]  # obtem os F dígitos (milisegundos)

        # se frac for vazio, preencher com zeros
        if not frac:
            frac = '0' * F

        # concatenar a parte inteira com a parte fracionária(milisegundos)
        time_str += ":" + frac

    return time_str

def save_image(img_bytes, filename):
    image = Image.open(BytesIO(img_bytes))
    image.save(filename)

#main
def server(ip, port, F, A):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    images = load_digit_images()
    
    while True:
        current_time = get_current_time(F)
        frames = [images.get(char, images[':']) for char in current_time]
        
        total_width = sum(img.width for img in frames)
        max_height = max(img.height for img in frames)
        combined_image = Image.new('RGB', (total_width, max_height))
        x_offset = 0
        for img in frames:
            combined_image.paste(img, (x_offset, 0))
            x_offset += img.width
        
        img_byte_arr = BytesIO()
        combined_image.save(img_byte_arr, format='PNG')
        img_bytes = img_byte_arr.getvalue()
        
        pdu = struct.pack('!B H', F, A) + img_bytes
        sock.sendto(pdu, (ip, port))
        
        save_image(img_bytes, "server_send.png")  # salvamos a imagem que enviamos
        
        print(f"Servidor: [{current_time}] Enviado frame")
        
        time.sleep(10**(-F))

# configs


# Load the config file
with open('config.json', 'r') as f:
    config = json.load(f)

# Get server-specific configurations
server_config = config['server']

ip = server_config['ip']
port = server_config['port']
F = server_config['F']
A = server_config['A']

# Now you can use these variables in your server code
print(f"Server running on {ip}:{port}, F={F}, A={A}")


# ip = 'localhost' # ip do servidor
# port = 12345 #porta do servidor UDP 
# F = 0  # precisão do relógio (número de dígitos de milissegundos)
# A = 10  # atraso máximo de reprodução (em segundos)


server(ip, port, F, A)