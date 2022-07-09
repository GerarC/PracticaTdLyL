#include "include/automata.hpp"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <set>
#include <sstream>
#include <string>

using std::set;
using std::map;

/*
    Voy a usar el método de las derivadas para construir
    un Autómata Finito Determinístico a partir de una Expresión Regular.

    Las derivadas tienen las siguientes propiedades:

        E -> Un Alfabeto
        L -> Un lenguaje construido con E
        a, b, c -> Simbolos del leguaje
        Ψ -> Símbolo que representa el error
        λ -> Símbolo que representa la Secuencia nula
        u, v -> Expresiones Regulares

        Dλ(L) = L
        Da(Ψ) = Ψ
        Da(λ) = Ψ
        Da(a) = λ
        Da(b) = Ψ
        Da(a*) = a*
        Da(a.c) = c
        Da(a+) = a*
        Da(a|b) = λ -> Da(a)|Da(b) -> (λ + Ψ) = λ
        Da((a|b).c) = c

        Da(u|v) = Da(u)|Da(v)
        Da(u.v) = Da(u).v|Da(v) if λϵu
                = Da(u).v if λ∉ u
        Da(u*) = Da(u).u*

        Y la última, esta no la explicó el profesor en clase,
        pero con intuición la puedes inferir
        Da(u+) = Da(u).u*

    Siempre se parte la ER en el primer punto de concatenación si esta empieza
    con una expresión de paréntesis o un operador de asterisco
    
    Los simbolos con los que se representan el Error y La secuencia nula en el código
    son los siguientes:

        Ψ -> #
        λ -> ~

*/

#define DOT '.'
#define PLUS '+'
#define ASTERISK '*'
#define OR '|'
#define LPAR '('
#define RPAR ')'
#define LAMBDA '~' // 03BB
#define ERROR '#'  // 03A8

Automata::Automata() {}

Automata::Automata(string regex) {
    this->regex = regex;
    set<char> setSym;
    int len = regex.size();
    for (int i = 0; i < len; i++) {
        switch (regex[i]) {
            case DOT:
            case PLUS:
            case ASTERISK:
            case OR:
            case LPAR:
            case RPAR:
            case LAMBDA:
                continue;
                break;
            default:
                setSym.insert(regex[i]);
                break;
        }
    }
    this->symbols = "";
    int a = 0;
    for (auto i = setSym.begin(); i != setSym.end(); i++) {
        symbols.push_back(*i);
        a++;
    }
    this->buildAutomata();
    this->shortNames();
}

string Automata::getRegex(){
    return this->regex;
}

string Automata::getSymbols(){
    return this->symbols;
}

list<State> Automata::getStates(){
    return this->states;
}

list<string> Automata::getRightStates(){
    return this->rightStates;
}

list<string> Automata::getTransitions(){
    list<string> transitions;

    return transitions;
}

string Automata::getActualState(){
    return this->actualState.getName();
}

bool Automata::validate(string line){
    bool isValid = false;
    int lineSize = line.size();

        std::cout << "Actual State: " << this->actualState.getName() << std::endl;
    for (int i = 0; i < lineSize; i++){
        std::cout << "Actual Symbol: " << line[i] << std::endl;
        this->nextState(line[i]);
        std::cout << "Actual State: " << this->actualState.getName() << std::endl;
    }
    auto rsTemp = std::find(this->rightStates.begin(), this->rightStates.end(), this->actualState.getName());
    if (rsTemp != std::end(this->rightStates)) isValid = true;

    return isValid;
}

void Automata::nextState(char sym){
    string next;
    next = this->actualState.next(sym);
    
    for (auto s = this->states.begin(); s != this->states.end(); s++){
        if(s->getName() == next) this->actualState = *s;
    }
}

void Automata::buildAutomata(){
    string state = "";
    string newL = "";
    State tempS;
    int SymNum = this->symbols.size();
    char tempC;

    this->states.push_back(State(regex));

    this->actualState = *this->states.begin();
    for (auto s = this->states.begin(); s != this->states.end(); s++){
        for (int i = 0; i < SymNum; i++){
            tempC = this->symbols[i];
            newL = derive(s->getName(), tempC);
            
            tempS = State(newL);
            if (!stateExist(this->states, newL))
                this->states.push_back(State(newL));
            s->addTransition(tempC, newL);
            if (hasLambda(newL)){
                auto rsTemp = std::find(
                        this->rightStates.begin(),
                        this->rightStates.end(),
                        newL
                );
                if (rsTemp == std::end(this->rightStates)) this->rightStates.push_back(newL);
            }
        }
    }

    for (auto s = this->states.begin(); s != this->states.end(); s++){
        std::cout << "Estado: " << s->getName() << std::endl;
        std::cout << "\tTransiciones:\n";
        map<char,string> transitions = s->getTransitions();
        for(auto t = transitions.begin(); t != transitions.end(); t++){
            std::cout << "\t\tpor " << t->first << " a " << t->second << std::endl;
        }
    }

    std::cout << "Estados finales:\n";
    for (auto rs = this->rightStates.begin(); rs != this->rightStates.end(); rs++)
        std::cout << "\t-> " << *rs << std::endl;

    std::cout << "Estado Actual: " << this->actualState.getName() << std::endl;
}

void Automata::shortNames(){ // Solo sirve para poner nombres cortos a los estados
    string genState = "C";
    string state;
    int stateNum = 0;
    string temp;
    
    for (auto s = this->states.begin(); s != this->states.end(); s++){
        temp = s->getName();
        state = genState + std::to_string(stateNum);
        if (temp == "#") state = "Err";
        else stateNum++;
        s->setName(state);
        for (auto st = this->states.begin(); st != this->states.end(); st++){
            st->editTransition(temp, state);
        }
        for (auto r = this->rightStates.begin(); r != this->rightStates.end(); r++) if ((*r) == temp) (*r) = state;
    }
    actualState = *this->states.begin();
}

string derive(string regex, char symbol) {
    /*
        
       Como se dijo al principio, separaremos el ER al primer punto,
       la parte izquierda será llamada U y la Parte derecha V.
       Pero para esto necesitamos verificar si la expresión solo tiene concatenaciones
       en el primer nivel, es decir, si no tiene operandos de or '|' fuera de paréntesis.
       Luego de partir la ER verificaremos caso a caso cómo derivar

    */

    int pS = 0;     // PairStack -> en qué nivel de paréntesis está
    int orPos = -1; // Aquí se guarda la posición del primer Or encontrado en caso de que haya
    int splitPos = 0; // Esta será la posición desde la que se separará el Re en caso de no haber Or Principal
    int mainPairs = 0; // Cuantos paréntesis hay en el nivel principal
    int reSize = regex.size(); // Tamaño del Regex
    string result = "~"; // La derivada de la RE
    string u; // Parte izquierda de la RE
    string v; // Parte derecha de la RE
    string dU; // Derivada de la parte izquierda
    string dV; // Derivada de la parte derecha

    if (reSize == 0) return "~";
    else if (regex == "#") return "#";
    else if ((regex == "#") && (symbol != '~')) return "#";
    if (symbol == '~') return regex;

    if (reSize == 2) {
        if (regex[0] == symbol && (regex[1] == '*' || regex[1] == '+')) {
            /*
               Si la ER tiene solo dos simbolos, El primero de estos dos coincide
               con el símbolo con el cual se está derivando y el segundo con '*' o '+'
               retorna <símbolo>*
            */
        result = symbol;
        result.push_back('*');
        }
        else if (regex[0] != symbol) {
            // Si el tamaño de la ER es 2 y el primero no coincide con el simbolo entonces retorna error
            result = "#";
        }
        else result = regex[1];
        /* std::cout << lang <<" Result: " << result << std::endl; */
        return result;
    }
    else if (reSize == 1){
        if (regex[0] == symbol) return "~";
        else return "#";
    }

    for (int i = 0; i < reSize; i++) {
        switch (regex[i]) {
        case '(': // sP++ Y verifica si el caracter anterior es el final de un paréntesis principal
            if (i > 1 && regex[i - 1] == ')' && mainPairs == 1 && pS == 0) splitPos = i;
            pS++;
            break;
        case ')': // sP-- 
            pS--;
            if (pS == 0) mainPairs++; // Cuando stackPairs es 0 otra vez, entonces aumenta el número de parentesis principales
            break;
        case '*':
        case '+': // Define el punto de separación despues de un '*' o un '+'
            if ((pS == 0 && mainPairs <= 1 && splitPos == 0) || (splitPos == 1 && i == 1))
                splitPos = i + 1;
            break;
        case '|': // Verifica si hay un Or principal
            if (pS == 0) {
                orPos = i;
                i = reSize;
            }
            break;
        default:
            if (pS == 0) { // Si no está dentro de un paréntesis
                if (splitPos == 0) splitPos = i + 1; // El punto de separación se pone en el primer caracter
                if (i > 1 && regex[i - 1] == ')' && mainPairs == 1) splitPos = i;
            }
        }
    }
    if (orPos != -1) { // Si hay un Or principal entonces divide el RE en U y V
        u = regex.substr(0, orPos);
        v = regex.substr(orPos + 1);
        dU = derive(u, symbol);
        dV = derive(v, symbol);
        if ((dU != "~") && (dU != "#")) {
            result = dU;
            if (((dV != "~") && (dV != "#")) && result != "~") result = result + "|" + dV;
        }
        else if (dV != "#") result = dV;
        else if (dU == "#") result = "#";
        return result;
    }
    switch (regex[0]) { // Verifica si el primer caracter es un paréntesis u otra cosa
    case '(':
        if (mainPairs == 1 && regex[reSize - 1] == '*' && regex[reSize - 2] == ')') {
            // Solo Hay una expresión del tipo (u)*
            bool mo = false; // main Or
            u = regex.substr(1, reSize - 3);
            v = regex;
            dU = derive(u, symbol);
            for (int i = 0; i < dU.size(); i++) {
                // Aquí se verífica si la derivada tiene un Or principal
                switch (dU[i]) {
                    case '(':
                        pS++;
                        break;
                    case ')':
                        pS--;
                        break;
                    case '|':
                        if (pS == 0) mo = true;
                        break;
                }
            }
            if (mo) { // Si la ER tiene un Or principal la pone entre paréntesis
                dU = '(' + dU + ')';
            }
            result = dU;
            if (result != "~") result += v;
            else result = v;
            if (dU == "#") result = "#";
        }
        else if (mainPairs == 1 && regex[reSize - 1] == '+' && regex[reSize - 2] == ')') {
            // Solo Hay una expresión del tipo (u)+
            bool mo = false; // Or principal
            u = regex.substr(1, reSize - 3);
            v = "(" + u + ")*";
            dU = derive(u, symbol);
            for (int i = 0; i < dU.size(); i++) {
                switch (dU[i]) {
                    case '(':
                        pS++;
                        break;
                    case ')':
                        pS--;
                        break;
                    case '|':
                        if (pS == 0) mo = true;
                        break;
                }
            }
            if (mo) dU = '(' + dU + ')';
            result = dU;
            if (result != "~") result += v;
            else result = v;
            if (dU == "#") result = "#";
        }
        else if (mainPairs == 1 && regex[reSize - 1] == ')') {
            // Solo una expresión del tipo (u)
            u = regex.substr(1, reSize - 2);
            dU = derive(u, symbol);
            if (dU != "~") result = dU;
        }
        else { // La expresión el del tipo (u)*<algo>, (u)+<algo> o (u)<algo> 
            bool hl;
            u = regex.substr(0, splitPos);
            v = regex.substr(splitPos);
            hl = hasLambda(u);
            dU = derive(u, symbol);
            if ((dU != "~") && (dU != "#")) result = dU;
            if (result != "~") result += v;
            else result = v;
            if (dU == "#") result = "#";
            if (hl) { // Si u contiene a lambda, entonces deriva a v
                dV = derive(v, symbol);
                if ((dV != "~") && (dV != "#")) {
                if ((result != "~")&&(result != "#")) result = result + "|" + dV;
                else result = dV;
            }
        }
    }
        break;
    default:
        if (splitPos == 1) { // Si la ER no comienza con '(u)' o 'u*' y similares, entonces la deriva
            if (regex[0] == symbol) result = regex.substr(splitPos);
            else return "#";
        }
        else { // Si comienza con 'u*' o 'u+' entonces lo separa en u y v, entonces los deriva
            u = regex.substr(0, splitPos);
            v = regex.substr(splitPos);
            if (regex[splitPos - 1] == '*') {
                dU = derive(u, symbol);
                if (dU != "#") result = dU + v;
                dV = derive(v, symbol);
                if ((dV != "~") && (dV != "#")) {
                    if (result != "~") result = result + "|" + dV;
                    else result = dV;
                }
                if((dU == "#")&&( dV == "#")) return "#";
            }
        }
        break;
    }
    return result;
}

bool hasLambda(string regex) {
    int pS = 0;     // PairStack -> En que nivel de parentesis está el análisis
    int orPos = -1; // Aquí se guarda la posición del primer Or encontrado en caso de que haya
    int splitPos = 0; // Esta será la posición desde la que se separará el Re en caso de no haber Or Principal
    int mainPairs = 0; // Cuando Parentesis hay en el nivel principal
    int reSize = regex.size(); // Tamaño del Regex
    bool hasL = false;
    string u; // Parte izquierda de la RE
    string v; // Parte derecha de la RE

    if (regex == "~") return true;
    if ((reSize == 2)&&(regex[1] == '*')) return true ;

    for (int i = 0; i < reSize; i++) { // Básicamente hace lo mismo que el switch de derive
        switch (regex[i]) {
        case '(': 
            if (i > 1 && regex[i - 1] == ')' && mainPairs == 1 && pS == 0)
                splitPos = i;
            pS++;
            break;
        case ')':
            pS--;
            if (pS == 0) mainPairs++;
            break;
        case '*':
        case '+':
            if ((pS == 0 && mainPairs <= 1 && splitPos == 0) || (splitPos == 1 && i == 1))
            splitPos = i + 1;
            break;
        case '|':
            if (pS == 0) {
                orPos = i;
                i = reSize;
            }
            break;
        default:
            if (pS == 0) {
                if (splitPos == 0) splitPos = i + 1;
                if (i > 1 && regex[i - 1] == ')' && mainPairs == 1) splitPos = i;
            }
            break;
        }
    }
    if (orPos != -1) {
        u = regex.substr(0, orPos);
        v = regex.substr(orPos + 1);
        hasL = hasLambda(u) || hasLambda(v);
        return hasL;
    }
    switch (regex[0]) {
        case '(':
            if (mainPairs == 1 && regex[reSize - 1] == '*' && regex[reSize - 2] == ')') {
                hasL = true;
            }
            else if (mainPairs == 1 && regex[reSize - 1] == '+' && regex[reSize - 2] == ')') {
                u = regex.substr(1, reSize - 3);
                hasL = hasLambda(u);
            }
            else if (mainPairs == 1 && regex[reSize - 1] == ')') {
                u = regex.substr(1, reSize - 2);
                hasL = hasLambda(u);
            }
            else { 
                u = regex.substr(0, splitPos);
                v = regex.substr(splitPos);
                hasL = hasLambda(u) && hasLambda(v);
            }
            break;
        default:
            hasL = false;
            break;
    }
    return hasL;
}

bool stateExist(list<State> states, string name){
    bool exits = false;
    for(auto it = states.begin(); it != states.end(); it++){
        if(it->getName() == name) exits = true;
    }
    return exits;
}

string Automata::getTable(){
    using std::left;
    using std::setw;
    using std::endl;
    using std::stringstream;

    stringstream table;

    table << "Autómata para " << this->regex;
    table << endl << endl; 

    table << left << setw(6) << "Sts.";
    for (int i = 0; i < this->symbols.size(); i++){
        table << left << setw(5) << this->symbols[i];
    }
    table << endl; 
    for (auto s = this->states.begin(); s != this->states.end(); s++){
        table << left << setw(6) << s->getName();
        map<char,string> transitions = s->getTransitions();
        for(auto t = transitions.begin(); t != transitions.end(); t++)
            table << left << setw(5) << t->second;
        auto rsTemp = std::find(
            this->rightStates.begin(),
            this->rightStates.end(),
            s->getName()
        );
        if (rsTemp != std::end(this->rightStates)) table << left << setw(1) << 1;
        else table << left << setw(1) << 0;
        table << endl; 
    }
    return table.str();
}

/*
    Cosas para imprimir el automata 

	std::cout << "Símbolos: " << this->getSymbols() << std::endl;
    std::cout << std::endl;
    for (auto s = this->states.begin(); s != this->states.end(); s++){
        std::cout << "Estado: " << s->getName() << std::endl;
        std::cout << "\tTransiciones:\n";
        map<char,string> transitions = s->getTransitions();
        for(auto t = transitions.begin(); t != transitions.end(); t++){
            std::cout << "\t\tpor " << t->first << " a " << t->second << std::endl;
        }
    }

    std::cout << "Estados finales:\n";
    for (auto rs = this->rightStates.begin(); rs != this->rightStates.end(); rs++)
        std::cout << "\t-> " << *rs << std::endl;

    std::cout << "Estado Actual: " << this->actualState.getName() << std::endl;
*/
