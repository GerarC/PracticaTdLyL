#include <iostream>
#include <ostream>
#include <string>

#include "include/automata.hpp"

using std::string;

int main(int argc, char** argv){
	/* char reg[] = "(0|10*1)*10*"; */
	/* char reg[] = "(aa|b*a|(abc|bc|~)bac|~)+ab*"; */
	/* char reg[] = "(a|bcbac)(aa|b*a|(abc|bc|~)bac|~)*ab*|b*"; */
	/* char reg[] = "(aa|b*a|(abc|bc|a*)bac|~)+ab*"; */
	/* char reg[] = "0*1(0|10*1)*10*|0*"; */
	/* char reg[] = "0*1(0|10*1)*10*"; */
	char reg[] = "1*10*1";
	Automata af = Automata(reg);	

    std::cout << std::endl;
    std::cout << "Regex: ";
	std::cout << af.getRegex() << std::endl;
    std::cout << "Symbols of the Re: ";
	std::cout << af.getSymbols() << std::endl;
    std::cout << std::endl;


    /* bool isvalid = af.validate("aaaaaaaaaaaabbbbbbbabacabcbacab"); */
    bool isvalid = af.validate("11111111101");
    if(isvalid) std::cout << "Es Valido" << std::endl;
    else std::cout << "nada de nada" << std::endl;

    /* string derived = reg; */
    /* char symbol = 'a'; */
    /* for (int i = 0; i < 1; i++){ */
    /*     std::cout << std::endl; */
    /*     derived = derive(derived, symbol);; */
    /*     std::cout << "The Re " << reg << " Derive respect to " << symbol << "\n-> "; */
	    /* std::cout << derived << std::endl; */
    /* } */

	return 0;
}
