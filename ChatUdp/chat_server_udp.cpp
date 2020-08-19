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

struct in_addr localInterface;
struct sockaddr_in groupSock;

int main(int argc, const char** argv) {
    
    int sock, ret;
    char loopch = 0;
    memset((char *) &groupSock, 0 , sizeof(groupSock));
    
    //cria socket udp
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        cout << ">> ERRO ao criar o socket.\n";
		printf("%i \n", errno);
		printf("%s \n", strerror(errno));
		exit(1);
    }

    //inicializa a strutura sockaddr 
    groupSock.sin_family = AF_INET;
    groupSock.sin_addr.s_addr = inet_addr("225.1.1.1");
    groupSock.sin_port = htons(5000);

    //disabilita loopback
    ret = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&loopch, sizeof(loopch));
    if (ret < 0)
    {
        cout << ">> ERRO ao desabilitar loopback.\n";
		printf("%i \n", errno);
		printf("%s \n", strerror(errno));
		exit(1);
    }
   
    localInterface.s_addr = inet_addr("127.0.0.1");
    
    //seta socket como multicast
    ret = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface));
    if (ret < 0)
    {
        cout << ">> ERRO ao criar multicast.\n";
		printf("%i \n", errno);
		printf("%s \n", strerror(errno));
		exit(1);
    }
    message msg = message();
    msg.set_type(0);
    msg.set_nick("server");
    msg.set_sms("ack");
    
    vector<char> sms = msg.cast_msg();
    char databuf[sms.size()];
    for (size_t i = 0; i < sms.size(); i++)
    {
        printf("%d ", sms[i]);
        databuf[i] = sms[i];
    }

    ret = sendto(sock, &databuf, sizeof(databuf), 0, (struct sockaddr*)&groupSock, sizeof(groupSock));
    if (ret < 0)
    {
        cout << ">> erro send msg.\n";
		printf("%i \n", errno);
		printf("%s \n", strerror(errno));
		exit(1);
    }
    else
    {
        printf("\n %d  ", ret);
        printf("\n msg enviada \n");
    }
    
    return 0;
}