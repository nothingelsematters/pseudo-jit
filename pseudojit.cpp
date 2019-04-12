#include <iostream>
#include <ios>
#include <exception>

#include "pseudocompiler.hpp"
#include "fnvhash.hpp"


int main() {
    std::cout << "input a string to compute FNV-hash: ";
    std::string str;
    std::cin >> str;

    try {
        pcompiler pc(str.c_str());
        pc.compile();

        std::cout << std::hex
            << "run-time function: " << fnvhash(str.c_str()) << '\n'
            << "compiled output:   " << pc.execute() << '\n';

    } catch (const std::exception& e) {
        return -1;
    }
    return 0;
}
