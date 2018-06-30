#include "Server.h"
#include <iostream>
//#include "Gyroscope.h"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "hexapod control v1.0" << endl;
    spider::Server serv;
    serv.startServer();
    while(1)
    {
        sleep(1);
    }
    return 0;
}
