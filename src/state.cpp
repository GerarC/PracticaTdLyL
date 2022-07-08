#include "include/state.hpp"

State::State(){}

State::State(string name):
    name(name)
{}
        
void State::setName(string nName){
    this->name = nName;
}

string State::getName(){
    return this->name;
}

void State::addTransition(char sym, string state){
    this->transitions.insert(std::make_pair(sym, state));
}

void State::editTransition(string name, string nName){
    for (auto it = this->transitions.begin(); it != this->transitions.end(); it++){
        if (it->second == name) it->second = nName;
    }
}

map<char, string> State::getTransitions(){
    return this->transitions;
}

string State::next(char sym){
    string next = "#";
    map<char, string>::iterator it = this->transitions.find(sym);
    if (it != this->transitions.end()) next = it->second;
    return next;
}
