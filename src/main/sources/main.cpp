#include <iostream>
#include <string>
#include <thread>
#include "util/Settings.h"
#include "util/Dbg.h"

#define ROOM_INIT_COUNT 1

using namespace std;

void responder() {
    while (true) {
        string a;
        cin >> a;
        if (a.compare("yespls")==0) {
    		DBG::sig("Terminating");
            //terminate();
            return;
        }else{
    		DBG::sig(a);
		}
    }
}

int main() {
    Settings::setDebug(true);
    DBG::sig("Program started.");
    thread console(responder);
    console.join();	
    return 0;
}
