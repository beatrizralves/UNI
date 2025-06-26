import socket
import struct
import time
import json
from PIL import Image
from io import BytesIO
import configparser

def save_image(img_bytes, filename):
    image = Image.open(BytesIO(img_bytes))
    image.save(filename)

def client(ip, port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((ip, port))
    
    buffer = []

    resync_needed = False  # flag para indicar se é necessário ressincronizar

    while True:
        pdu, addr = sock.recvfrom(65535)
        
        # campos de controle do PDU recebidos
        F, A = struct.unpack('!BH', pdu[:3])
        
        max_buffer_size = calculate_buffer_size(F, A)
        resync_threshold = max_buffer_size  # buffer ficará cheio quando tiver max_buffer_size + 1

        # extrai a imagem do PDU corretamente
        img_bytes = pdu[3:]
        
        # verifica se img_bytes são bytes
        if isinstance(img_bytes, bytes):
            # adiciona ao buffer com o tempo atual
            buffer.append((img_bytes, time.time()))
            
            # verifica se já é hora de reproduzir uma frame
            current_time = time.time()
            if buffer:
                frame_time = buffer[0][1] + A
                if current_time >= frame_time:
                    frame_bytes, frame_timestamp = buffer.pop(0)
                    save_image(frame_bytes, f"client_received.png")
                    timestamp = time.strftime("%H:%M:%S", time.localtime(frame_timestamp))
                    print(f"Cliente:[{timestamp}] Recebido frame. ")
            
            # ressincronização 
            if len(buffer) > resync_threshold and not resync_needed:
                print("Cliente: Buffer cheio - ressincronização necessária")
                print("Cliente: Ressincronizando...")
                resync_needed = True
        
        else:
            print("Cliente: Dados de imagem recebidos não são bytes.")
        
        # se precisar ressincronizar, faz isso uma vez
        if resync_needed:
            time.sleep(A)
            resync_needed = False
            buffer.clear()  # Limpa o buffer após a ressincronização
        
        time.sleep(10 ** (-F))

def calculate_buffer_size(F, A):
    return 10 ** F * A

# configs

with open('config.json', 'r') as f:
    config = json.load(f)


client_config = config['client']

ip = client_config['ip']
port = client_config['port']


print(f"Client connecting to {ip}:{port}")


# ip = 'localhost'
# port = 12346


client(ip, port)
