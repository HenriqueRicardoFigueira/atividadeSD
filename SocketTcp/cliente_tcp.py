###
#!/usr/bin/python 
# author: henrique Ricardo Figueira
# disciplina: Sistemas Distribuídos
# data: 16/08/2020
# descrição: Código referente a atividade Programação com sockets TCP
###

import socket
import pickle
import os
import tqdm

SEPARATOR = "<SEPARATOR>"
BUFFER_SIZE = 4096 
host = '127.0.0.1'
port = 5000
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print(f"[+] Connecting to {host}:{port}")
sock.connect((host, port))
print("[+] Connected.")

while True:
    messag = input("Digite um comando: ")
    dow = messag.split(" ")
    message = bytes(messag, "utf-8")
    sock.send(message)
    if messag == "exit" or messag == "EXIT":
        sock.close()
        break

    elif messag == "files" or messag == "FILES":
        data = sock.recv(BUFFER_SIZE)
        print(pickle.loads(data))

    elif dow[0] == "down" or dow[0] == "DOWN":
        data = sock.recv(BUFFER_SIZE)
        filename, filesize = data.decode().split(SEPARATOR)
        filename = os.path.basename(filename)
        filesize = int(filesize)
        progress = tqdm.tqdm(range(filesize), f"Receiving {filename}", unit="B", unit_scale=True, unit_divisor=1024)
        with open("./recive/"+filename, "wb") as f:
            for _ in progress:
                bytes_read = sock.recv(BUFFER_SIZE)
                if not bytes_read:    
                    break
                f.write(bytes_read)
                progress.update(len(bytes_read))
    else:
        data = sock.recv(BUFFER_SIZE)
        if data != b"":
            print(str(repr(data)))
    
#sock.close()

