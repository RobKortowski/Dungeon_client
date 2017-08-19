#ifndef CONNECTION_H
#define CONNECTION_H

#define BLEN 128

#include <string>
#include <mutex>
#include <thread>
#include <atomic>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

class Connection {
public:
    Connection(int, std::string);
    Connection();
    ~Connection();
   
    /**
	 * sends string through socked specified by socketfd
	 */
    int sendMsg(std::string);

    /*returns "" when there is no new msg from socket, string msg otherwise*/
    std::string getMsg();
   
    /*determines whether Connection is in closed state*/
    bool isClosed();

    /*sets inGame bool which determines thread behavior*/
    void setState(bool);

    
private:
    /*inGame state determines thread behavior*/
    std::atomic<bool> inGame;
    
    /*protects msg string*/
    std::mutex mutex;

    /* NOTE: change msg to atomic? */ 
    std::string msg;

    /*determines whether msg has changed since last getMsg() call*/
    std::atomic<bool> newMsg;

    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    int sin_size;
    int yes = 1;
    
    int sockfd;
    std::string ip;
    std::thread* receiver;
    char buffer[1024*128];

    std::atomic<bool> closed; 


    void setClosed(bool);
    void tfunc();

    void interpret(std::string&);    

};

#endif
