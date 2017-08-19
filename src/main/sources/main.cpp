#include <iostream>
#include <string>
#include <thread>
#include "util/Settings.h"
#include "util/Dbg.h"
#include "Connection.h"

#define ROOM_INIT_COUNT 1

using namespace std;

Connection* connection;

void responder() {
    while (true) {
        string a;
        cin >> a;
        if (a.compare("yespls")==0) {
    		DBG::sig("Terminating");
            //terminate();
            return;
        }else{
            //DBG::sig(a);
			connection->sendMsg(a);
		}
    }
}

int main() {
    Settings::setDebug(true);
    DBG::sig("Program started.");
	connection = new Connection();
    thread console(responder);
    console.join();	
    return 0;
}
