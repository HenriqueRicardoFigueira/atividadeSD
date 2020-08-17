###
#!/usr/bin/python 
# author: henrique Ricardo Figueira
# disciplina: Sistemas Distribuídos
# data: 16/08/2020
# descrição: Código referente a atividade Programação com sockets TCP
###

import socket
from datetime import datetime
import binascii

def recive_message(clientsock):
    data = clientsock.recv(20)
    return data

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind(('127.0.0.1', 5000))
sock.listen(10)
(client, addr) = sock.accept()

while True:
    dataRecive = recive_message(client)
    if not dataRecive:
        sock.close()
        break
    else:
        print(dataRecive)
        dataHora = str(datetime.now())
        hora = dataHora[11:]
        data = dataHora[0:10]
        if dataRecive.decode() == "time" or dataRecive.decode() == "TIME":
            hora = bytes(hora, "utf-8")
            client.send(hora)
        elif dataRecive.decode() == "date" or dataRecive.decode() == "DATE":
            data = bytes(data , "utf-8")
            client.send(data)
        elif dataRecive.decode() == "exit" or dataRecive.decode() == "EXIT":
            sock.close()
            break
