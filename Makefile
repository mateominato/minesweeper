CXX = g++
CXXFLAGS = -g -std=c++14
FRAMEWORKS = -framework sfml-graphics -framework sfml-window -framework sfml-system

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $<

testMinesweeper.exe: testMinesweeper.o minesweeper.o
	$(CXX) -o $@ $(FRAMEWORKS) $^

clean: 
	$(RM) *.o *.exe
