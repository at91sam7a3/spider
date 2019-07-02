#include "Server.h"
#include <iostream>
#include <unistd.h>
//#include "Gyroscope.h"

using namespace std;

int main(int argc, char *argv[])
{
    bool need2sleep=false;
    if(argc > 1) need2sleep = true;
    cout << "hexapod control v1.0" << endl;
    spider::Server serv;
    serv.startServer(need2sleep);
    while(1)
    {
        sleep(1);
    }
    return 0;
}
