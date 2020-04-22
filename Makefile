CXX = g++
CXXFLAGS = -g -std=c++14
FRAMEWORKS = -lsfml-graphics -lsfml-window -lsfml-system

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $<

testMinesweeper.exe: testMinesweeper.o minesweeper.o
	$(CXX) -o $@ $(FRAMEWORKS) $^

clean: 
	$(RM) *.o *.exe
