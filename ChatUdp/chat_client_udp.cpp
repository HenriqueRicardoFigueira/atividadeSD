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
    localSock.sin_port = htons(5000);;
    localSock.sin_addr.s_addr  = INADDR_ANY;

    ret = bind(sock, (struct sockaddr*)&localSock, sizeof(localSock));
    if (ret < 0)
    {
        cout << ">> ERRO bind.\n";
		printf("%i \n", errno);
		printf("%s \n", strerror(errno));
		exit(1);
    } 

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
    message msg = message();
    array<char,32> sms;
    vector<char> msgs;
    void *databuf;
    databuf = &sms;
    int i = 0; 
    string dg;

    printf("Digite seu nick: ");
    //scanf("%s", dg);

    while (true)
    {
        if (read(sock, databuf, sizeof(sms)) < 0)
        {
            cout << ">> ERRO receber msg.\n";
		    printf("%i \n", errno);
		    printf("%s \n", strerror(errno));
		    exit(1);
        }
        else
        {   
            while (sms[i] != -1)
            {
                if (i == 1)
                {
                    msg.set_nicksize(size_t(sms[i]));
                }
                else if (i == (3+int(msg.get_nicksize())))
                {
                    msg.set_msgsize(size_t(sms[i]));
                }
                msgs.push_back(sms[i]);
                i++;
            }
            msg.parse_message(msgs);
            cout << "MSG: " << msg.get_nick() << " - " << msg.get_msg() << "\n";
        }
        
    }
    
    return 0;
}