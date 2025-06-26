import socket
import struct
import time
import json
from collections import deque

# Configurações

# Load the config file
with open('config.json', 'r') as f:
    config = json.load(f)

# Get forwarder-specific configurations
forwarder_config = config['forwarder']

SERVER_IP = forwarder_config['SERVER_IP']
SERVER_PORT = forwarder_config['SERVER_PORT']
CLIENT_IP = forwarder_config['CLIENT_IP']
CLIENT_PORT = forwarder_config['CLIENT_PORT']
N = forwarder_config['N']
P = forwarder_config['P']
M = forwarder_config['M']


print(f"Forwarder connecting to {SERVER_IP}:{SERVER_PORT} and {CLIENT_IP}:{CLIENT_PORT}")


# SERVER_IP = 'localhost'
# SERVER_PORT = 12345
# CLIENT_IP = 'localhost'
# CLIENT_PORT = 12346
# N = 10  # A cada N frames, pausa
# P = 10   # Duração da pausa (segundos)
# M = 5   # A cada M segundos, ignora um PDU

def reencaminhador(server_ip, server_port, client_ip, client_port, N, P, M):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((server_ip, server_port))
    client_addr = (client_ip, client_port)
    
    buffer = deque()
    frame_count = 0
    last_drop_time = time.time()
    in_pause = False
    
    while True:
        pdu, addr = sock.recvfrom(65535)
        frame_count += 1
        current_time = time.time()
        # TODO: adicionar ao buffer
        # Simulação de perda de PDU a cada M segundos
        if current_time - last_drop_time >= M:
            print(f"Reencaminhador:[{time.strftime('%H:%M:%S')}] Frame descartado")
            last_drop_time = current_time
            # TODO: remover do buffer
            continue  # Ignora o frame sem adicioná-lo ao buffer
        
        buffer.append(pdu) 
        if not in_pause:
            # Verifica se o buffer está cheio
            while buffer:
                frame = buffer.popleft()
                sock.sendto(frame, client_addr)
                print(f"Reencaminhador:[{time.strftime('%H:%M:%S')}] Frame do buffer reencaminhado para o cliente.")
            
        frame_count += 1
        
        # Verifica se é momento de pausar
        if frame_count % N == 0:
            print(f"Reencaminhador:[{time.strftime('%H:%M:%S')}] Pausando por {P} segundos")
            in_pause = True
            time.sleep(P)
            in_pause = False
            print("Reencaminhador: Fim da pausa")

if __name__ == "__main__":
    reencaminhador(SERVER_IP, SERVER_PORT, CLIENT_IP, CLIENT_PORT, N, P, M)
