flags= -lncurses -lfltk
obj=g++ -c 
final=g++ -o 
project=./build/automatas
incl=src/include/

objs=src/main.o src/automata.o src/state.o src/app.o

all: autof clean

autof: $(objs)
	$(final)$(project) $(objs) $(flags)

main.o: src/main.cpp $(incl)automata.hpp
	$(obj)src/main.cpp

automata.o: src/automata.cpp $(incl)automata.hpp $(incl)state.hpp
	$(obj)src/automata.cpp
	
state.o: src/state.cpp $(incl)state.hpp
	$(obj)src/state.cpp

app.o: src/app.cpp $(incl)app.hpp
	$(obj)src/app.cpp
	
clean:
	rm $(objs)
