###
#!/usr/bin/python 
# author: henrique Ricardo Figueira
# disciplina: Sistemas Distribuídos
# data: 16/08/2020
# descrição: Código referente a atividade Programação com sockets TCP
###

import socket
def send_message(sock, message):
    ret = sock.send(message)
    print(ret)

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(('127.0.0.1', 5000))

while True:
    messag = input("Digite um comando: ")
    message = bytes(messag, "utf-8")
    send_message(sock, message)
    if messag == "exit" or messag == "EXIT":
        sock.close()
        break
    data = sock.recv(1024)
    print(repr(data))
    
#sock.close()

