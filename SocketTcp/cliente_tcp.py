###
#!/usr/bin/python 
# author: henrique Ricardo Figueira
# disciplina: Sistemas Distribuídos
# data: 16/08/2020
# descrição: Código referente a atividade Programação com sockets TCP
###

import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(('127.0.0.1', 5000))

while True:
    message = input("Digite um comando: ")
    message = bytes(message, "utf-8")
    sock.send(message)
    if message == "exit" or message == "EXIT":
        sock.close()
    data = sock.recv(1024)
    print(repr(data))
    
#sock.close()

