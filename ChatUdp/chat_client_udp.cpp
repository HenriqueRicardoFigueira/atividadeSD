/*
# author: henrique Ricardo Figueira
# disciplina: Sistemas Distribuídos
# data: 19/08/2020
# descrição: Implementação de serviço de chat  P2P usando Sockets Multicast e UDP
###
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "errno.h"
#include "net/if.h" 
using namespace std;
#include "message.cpp"

struct sockaddr_in localSock;
struct ip_mreq group;

string cast_string(char * a, int size){
    string s;
    for (size_t i = 0; i < size; i++)
    {
      s += a[i]; 
    } 
    return s;
}

int main(int argc, const char** argv) {
    int sock, ret;
    int reuse = 1;

    //cria socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        cout << ">> ERRO ao criar o socket.\n";
		printf("%i \n", errno);
		printf("%s \n", strerror(errno));
		exit(1);
    }
    
    //habilita multiplas instancias
    ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
    if (ret < 0)
    {
        cout << ">> ERRO reuse.\n";
		printf("%i \n", errno);
		printf("%s \n", strerror(errno));
		exit(1);
    } 

    memset((char *) &localSock, 0, sizeof(localSock));
    localSock.sin_family = AF_INET;
    localSock.sin_port = htons(6789);;
    localSock.sin_addr.s_addr  = INADDR_ANY;

    ret = bind(sock, (struct sockaddr*)&localSock, sizeof(localSock));
    if (ret < 0)
    {
        cout << ">> ERRO bind.\n";
		printf("%i \n", errno);
		printf("%s \n", strerror(errno));
		exit(1);
    } 

    //adiciona no grupo de msgs
    group.imr_multiaddr.s_addr = inet_addr("225.1.1.1");
    group.imr_interface.s_addr = inet_addr("127.0.0.1");
    ret = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group));
    if (ret < 0)
    {
        cout << ">> ERRO entrar no grupo.\n";
		printf("%i \n", errno);
		printf("%s \n", strerror(errno));
		exit(1);
    }

    //prepara para receber ack
    message msg = message();
    array<char,32> sms;
    vector<char> msgs;
    char nick[25];
    char comando[25];
    string comandos;
    void *databuf;
    databuf = &sms;
    int i = 0; 
    string dg;

    cout << "/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////" << endl;
    cout << "//                                                                                      ________                              //" << endl;
    cout << "//                                                                                     /_______/                             //" << endl;
    cout << "//     _________     __       __    __________    ______________    ___________        __________     ___________           //" << endl;
    cout << "//    |  _______|   |  |     |  |  |   ____   |  |_____    _____|  |________   |      |   ____   |   |   _____   |         //" << endl;
    cout << "//    |  |          |  |     |  |  |  |    |  |        |  |                /  /       |  |    |  |   |  |     |  |        //" << endl;
    cout << "//    |  |          |  |_____|  |  |  |____|  |        |  |               /  /        |  |____|  |   |  |     |  |       //" << endl;
    cout << "//    |  |          |   _____   |  |   ____   |        |  |              /  /         |   ____   |   |  |     |  |      //" << endl;
    cout << "//    |  |          |  |     |  |  |  |    |  |        |  |             /  /          |  |    |  |   |  |     |  |     //___________" << endl;
    cout << "//    |  |_______   |  |     |  |  |  |    |  |        |  |            /  /_______    |  |    |  |   |  |_____|  |                //" << endl;
    cout << "//    |_________|   |__|     |__|  |__|    |__|        |__|           |___________|   |__|    |__|   |___________|               //" << endl;
    cout << "//                                                                                                                              //" << endl;
    cout << "//                               Chat multicast para a disciplina de Sistemas distribuídos         by henrislip     ___________// "  << endl;
    cout << "////////////////////////////////////////////////////////////////////////////////////////////////////////////////////" << endl;
    cout << endl;
    cout << "Comandos: JOIN - para entrar no grupo." << endl;
    cout << "          MSG 'mensagem' - pra enviar uma msg. " << endl;
    cout << "          MSGIDV TO [nick] - para enviar uma msg para uma pessoa especifica." << endl;
    cout << "          LEAVE - para sair do grupo." << endl;
    cout << "          HELP  - para exibir a lista de comandos novamente. " << endl;
    cout << endl;
    cout << " Digite seu nick: "; 
    cin.getline(nick , 25);
    string nicks = cast_string(nick, strlen(nick));
    cout << " Olá " << nicks << " estamos esperando uma conexão !!" << endl;
   
    //laço para recebimento e envio de msgs
    while (true)
    {
        cout << " Digite um comando: ";
        scanf("%s", comando);
        cout << comando << endl;
        comandos = cast_string(comando, strlen(comando));
        memset(&comando, 0, sizeof(comando));
        cout << comandos << endl;
        if (comandos == "HELP" || comandos == "help" )
        {
            comandos = "\0";
            cout << "Comandos: JOIN - para entrar no grupo." << endl;
            cout << "          MSG 'mensagem' - pra enviar uma msg. " << endl;
            cout << "          MSGIDV TO [nick] - para enviar uma msg para uma pessoa especifica." << endl;
            cout << "          LEAVE - para sair do grupo." << endl;
            cout << "          HELP  - para exibir a lista de comandos novamente. " << endl;
            continue;
        }
        else
        {
            ;
        }
        //caso não consiga receber msg sai
        if (read(sock, databuf, sizeof(sms)) < 0)
        {
            cout << ">> ERRO receber msg.\n";
		    printf("%i \n", errno);
		    printf("%s \n", strerror(errno));
		    exit(1);
        }
        else
        {   //recebe msg e converte de array para vector ****arrumar tamanho do array******
            while (sms[i] != -1)
            {
                //pega tamanho do nick
                if (i == 1)
                {
                    msg.set_nicksize(size_t(sms[i]));
                }
                //pega tamanho da msg
                else if (i == (3+int(msg.get_nicksize())))
                {
                    msg.set_msgsize(size_t(sms[i]));
                }
                msgs.push_back(sms[i]);
                i++;
            }
            //traduz vetor para strings
            msg.parse_message(msgs);
            cout << "MSG: " << msg.get_nick() << " - " << msg.get_msg() << "\n";
        }
        
    }
    
    return 0;
}