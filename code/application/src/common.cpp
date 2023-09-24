#include "stdio.h"
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>


void printEnergetic() {
    std::cout << "██████╗░░░░░░██████╗░███████╗██████╗░" << std::endl;
    std::cout << "██╔══██╗░░░░░██╔══██╗██╔════╝██╔══██╗" << std::endl;
    std::cout << "██████╔╝█████╗██████╔╝█████╗░░██████╔╝" << std::endl;
    std::cout << "██╔═══╝░╚════╝██╔══██╗██╔══╝░░██╔══██╗" << std::endl;
    std::cout << "██║░░░░░░░░░░██║░░██║███████╗██║░░██║" << std::endl;
    std::cout << "╚═╝░░░░░░░░░░╚═╝░░╚═╝╚══════╝╚═╝░░╚═╝" << std::endl;
}

void hello(void)
{
    std::cout << "Welcome to the Energetic Program!" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << std::endl;
    
    printEnergetic();
}

void delay(double seconds)
{
    struct timespec req;
    req.tv_sec = (time_t)seconds;
    req.tv_nsec = (long)((seconds - (double)req.tv_sec) * 1e9);

    nanosleep(&req, NULL);
}

