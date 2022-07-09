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
		string regex; // ER de la que está basada el AFD
		string symbols; // Conjunto de símbolos que utiliza el AFD
		State actualState; // Estado actual del AF
		list<State> states; // Lista de estados del AF
		list<string> rightStates; // Lista de estados de Aceptación

	public:
		Automata(); // Constructor Vacio por si ajá
		Automata(string regex); // Constructor en base a una ER
		string getRegex();
		string getSymbols();
		list<State> getStates();
		list<string> getRightStates();
        list<string> getTransitions();
		string getActualState();
        string getTable(); // Retorna un string con la tabla que representa el AFD
        bool validate(string line); // Valida un string caracter a caracter una vez que el AFD está construido
		void nextState(char sym); // Busca el estado siguiente
		void buildAutomata(); // Construye el AFD si ya la clase tiene definido el regex 
        void shortNames(); // Sustituye los nombres temporales de los estados por unos más cortos
};

string derive(string lang, char symbol);
bool hasLambda(string regex);
bool stateExist(list<State> states, string name);

#endif
