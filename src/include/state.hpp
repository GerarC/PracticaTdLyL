#ifndef STATE_H
#define STATE_H

#include <map>
#include <string>
#include <utility>

using std::map;
using std::string;

class State {
    private:
        string name;
        map <char, string> transitions;
        
    public:
        State();
        State(string name);
        void setName(string nName);
        string getName();
        void addTransition(char sym, string state);
        void editTransition(string name, string nName);
        map<char, string> getTransitions();
        string next(char sym);
};

#endif
