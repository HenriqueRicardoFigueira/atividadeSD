###
#!/usr/bin/python 
# author: henrique Ricardo Figueira
# disciplina: Sistemas Distribuídos
# data: 16/08/2020
# descrição: Código referente a atividade Programação com sockets TCP
###

import socket
from datetime import datetime
import pickle
import struct
import os

def recive_message(clientsock):
    data = clientsock.recv(1024)
    return data

def parse_date(dt):
    dataHora = str(datetime.now())
    hora = dataHora[11:]
    data = dataHora[0:10]
    if dt == "hora":
        return hora
    else:
        return data

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind(('127.0.0.1', 5000))
sock.listen(10)


while True:
    client, addr = sock.accept()
    with client:
        dataRecive = recive_message(client)
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

            elif dataDecode == "exit" or dataDecode == "EXIT":
                sock.close()
                break
