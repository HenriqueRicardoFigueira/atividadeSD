###
#!/usr/bin/python 
# author: henrique Ricardo Figueira
# disciplina: Sistemas Distribuídos
# data: 16/08/2020
# descrição: Código referente a atividade Programação com sockets TCP
###

SEPARATOR = "<SEPARATOR>"
BUFFER_SIZE = 4096 

import socket
from datetime import datetime
import pickle
import struct
import os
import tqdm

def recive_message(clientsock):
    data = clientsock.recv(1024)
    return data

def parse_date(dt):
    dataHora = str(datetime.now())
    hora = dataHora[11:]
    data = dataHora[0:10]
    data = data.replace("-","/")
    data = data.split("/")
    data = data[2]+"/"+data[1]+"/"+data[0]
    if dt == "hora":
        return hora
    else:
        return data

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind(('127.0.0.1', 5000))
sock.listen(10)
client, addr = sock.accept()

while True:
    if client:
        dataRecive = recive_message(client)
        dow = dataRecive.decode().split(" ")
        print(dataRecive)
        if dataRecive != b'':
            dataDecode = dataRecive.decode()            
            if dataDecode == "time" or dataDecode == "TIME":
                hora = parse_date("hora")
                client.send(bytes(hora, "utf-8"))
            
            elif dataDecode == "date" or dataDecode == "DATE":
                data = parse_date("data")
                client.send(bytes(data , "utf-8"))
           
            elif dataDecode == "files" or dataDecode == "FILES":
                listDir = os.listdir("./shared")
                nov = (len(listDir), listDir)
                dados = pickle.dumps(nov)
                client.send(dados)
            
            elif dow[0] == "down" or dow[1] == "DOWN":
                n = dataDecode.split(" ")
                filename = str(n[1])
                filesize = os.path.getsize("./shared/"+filename)
                data_up = f"{filename}{SEPARATOR}{filesize}".encode()
                client.send(data_up)
                progress = tqdm.tqdm(range(filesize), f"Sending {filename}", unit="B", unit_scale=True, unit_divisor=1024)
                with open("./shared/"+filename, "rb") as f:
                   for _ in progress:
                        bytes_read = f.read(BUFFER_SIZE)
                        if not bytes_read:
                            break
                        client.sendall(bytes_read)
                        progress.update(len(bytes_read))
            elif dataDecode == "exit" or dataDecode == "EXIT":
                client.close()
                break
                
