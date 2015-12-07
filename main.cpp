#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#include "SocketConnector.h"

using namespace std;

void sigchld_handler(int s){
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}

void sigaction_handler(struct sigaction &sa){
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1){
        cerr << "sigaction setup failure" << endl;
        exit(-1);
    }
}

int main() {

    SocketConnector *sc = new SocketConnector("8989");
    sc->makeListen();
    cout << "Awaiting connections..." << endl;

    struct sigaction sa;
    sigaction_handler(sa);

    Packet *packet;
    Packet sendPacket;
    sendPacket.putData("OK");

    while (true){ //Kill me to stop me
        sc->makeAccept();

        if (!fork()){
            while (true) {
                cout << "**Connection from " <<
                    sc->getReadableClientAddr() << ": ";

                sc->getResponse(packet);
                cout << "\t\nPacket nr " << packet->getId();
                cout << "\t\nSent at " << packet->getTime();
                cout << "\t\nData: " << packet->getData() << std::endl;

                sc->sendMessage(&sendPacket);

                if (packet->getId() == 2) { //just for example
                    delete packet;
                    break;
                }

                delete packet;
            }

            sc->closeClientSocket();
            break;
        }

    }

    delete sc;

    return 0;
}