#ifndef AUTOMATA_H
#define AUTOMATA_H

#include <string>
#include <set>
#include <list>
#include <map>

#include "state.hpp"

using std::string;
using std::list;

class Automata{
	private:
		string regex;
		string symbols;
		State actualState;
		list<State> states;
		list<string> rightStates;

	public:
		Automata();
		Automata(string regex);
		string getRegex();
		string getSymbols();
		list<State> getStates();
		list<string> getRightStates();
        list<string> getTransitions();
		string getActualState();
        string getTable();
        bool validate(string line);
		void nextState(char sym);
		void buildAutomata();
        void shortNames();
};

string derive(string lang, char symbol);
bool hasLambda(string regex);
bool stateExist(list<State> states, string name);

#endif
