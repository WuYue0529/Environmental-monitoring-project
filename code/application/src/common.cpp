#include "stdio.h"
#include <iostream>

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