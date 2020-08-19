/*
#!/usr/bin/python 
# author: henrique Ricardo Figueira
# disciplina: Sistemas Distribuídos
# data: 19/08/2020
# descrição: Implementação de serviço de chat  P2P usando Sockets Multicast e UDP
###
*/

#include <iostream>
#include <bits/stdc++.h>
#include <unistd.h>
#include "sys/socket.h"
#include "errno.h"
#include "net/if.h"
#include "unistd.h"
#include <cstdlib>
#include <string>
#include <ctime>
#include <sys/time.h>
#include <list>
#include <vector> 
using namespace std;

/*
    Cria messagem padrão para envio e recebimento , baseado no código do professor.
*/

class message
{
private:
    char type;
    string nick;
    string msg;
    size_t nicksize;
    size_t msgsize;
    char checksum = 255;

public:
    message();
    char get_type();
    string get_nick();
    string get_msg();
    size_t get_nicksize();
    size_t get_msgsize();
    void set_type(char t);
    void set_nick(string n);
    void set_sms(string m);
    void set_nicksize(size_t n);
    void set_msgsize(size_t m);
    void print();
    void parse_message(vector<char> msg);
    vector<char> cast_msg();
    ~message();
};

message::message(){}

size_t message::get_nicksize(){
    return nicksize;
}

size_t message::get_msgsize(){
    return msgsize;
}

void message::set_nicksize(size_t n)
{
    nicksize = n;
}

void message::set_msgsize(size_t m)
{
    msgsize = m;
}

void message::set_type(char t)
{
    type = t;
}

void message::set_nick(string n)
{
    nick = n;
}

void message::set_sms(string m)
{
    msg = m;
}

void message::print()
{
    printf("%d \n", type);
    cout << nick << "\n";
    cout << msg << "\n";
}


//converte o vetor criado para strings
void message::parse_message(vector<char> msgg)
{
    type = msgg[0];
    int y = 2;
    string auxnick, auxmsg;
    for (int i = 0 ; i < nicksize; i++)
    {
        auxnick.insert(auxnick.end(),char(msgg[y]));
        y++;
    }
    y = nicksize+4;
    for (int i = 0; i < msgsize; i++)
    {
        auxmsg.insert(auxmsg.end(), char(msgg[y]));
        y++;
    }
    nick = auxnick;
    msg = auxmsg;
}

//cria vetor com as msgs de acordo com o protocolo
vector<char> message::cast_msg()
{
    vector <char> msgB;
    msgB.push_back(type);
    msgB.push_back(nick.length());
    for (int i = 0; i <= nick.length(); i++)
    {
        msgB.push_back(nick[i]);
    }
    msgB.push_back(msg.length());
    for (int i = 0; i <= msg.length(); i++)
    {
        msgB.push_back(msg[i]);
    }
    msgB.push_back(checksum);
    return msgB;
}

char message::get_type()
{
    return type;
}

string message::get_msg()
{
    return msg;
}

string message::get_nick()
{
    return nick;
}

message::~message(){}
