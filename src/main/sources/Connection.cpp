#include "Connection.h"
#include "util/Dbg.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define MYPORT 3450

using namespace std;

Connection::Connection(){
    my_addr.sin_family = AF_INET; // filling up addres struct 
    my_addr.sin_port = htons(MYPORT);
    my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(&(my_addr.sin_zero), '\0', sizeof my_addr.sin_zero);
    sin_size = sizeof (struct sockaddr_in);



    int new_socket;
 	new_socket = socket(PF_INET, SOCK_DGRAM, 0);
     //setsockopt(new_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (int));
    //bind(new_socket, (struct sockaddr *) &my_addr, sizeof (struct sockaddr));
	connect(new_socket, (struct sockaddr *) &my_addr, sin_size);
    DBG::sig("Connection opened");    
    this->sockfd = new_socket;
	this->ip = "test";
    closed = false;
    receiver = new thread(&Connection::tfunc, this);
}

Connection::Connection(int sockfd, string ip) : inGame(false) {
    this->sockfd = sockfd;
    this->ip = ip;
    closed = false;
    receiver = new thread(&Connection::tfunc, this);
}


Connection::~Connection() {
    setClosed(true);
    shutdown(sockfd, SHUT_RDWR);
    close(sockfd);
    receiver->join();
    delete receiver;
    //DBG::sig("DELETED CONNECTION");
}

int Connection::sendMsg(string msg) {
    int len, left, total, n;
    len = msg.length();
    const char* buf = msg.c_str();
    left = len;
    total = 0;

    while (total < len) {
        n = send(sockfd, buf + total, left, 0);
        if (n == -1) return -1;
        total += n;
        left -= n;
    }

    return 0;
}

bool Connection::isClosed() {
    return closed;
}

void Connection::setClosed(bool status) {
    closed = status;
}

void Connection::tfunc() {
    int len;
    while (!isClosed()) {
        len = recv(sockfd, buffer, 1024, 0);
        if (len == 0) { //shutdown of the socket
            if(isClosed()){
		break;
	    }else{
		//TODO warn connection for bad protocol
			continue;
	    }
        } else if (len == -1) {
            continue;
        }
        buffer[len] = '\0';
        string received(buffer);
        DBG::sig(received);
	}
}

void Connection::setState(bool s) {
    inGame = s;
}








