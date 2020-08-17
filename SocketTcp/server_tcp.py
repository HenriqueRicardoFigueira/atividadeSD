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


sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind(('127.0.0.1', 5000))
sock.listen(10)

while True:
    (client, addr) = sock.accept()
    if client:
        print(addr)
        data = client.recv(20)
        if not data:
            sock.close()
            break
        else:
            print(data)
            hora = bytes(str(datetime.now()), "utf-8")
            client.send(hora)
            #sock.close()
