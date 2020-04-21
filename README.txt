To build/compile the program:
make clean
make testMinesweeper.exe

To run the program:
./testMinesweeper.exe

Dependencies:
Minesweeper uses all standard libraries, excepting the graphics library SFML. This should already be installed on the testing machine per the Piazza post regarding external libraries. Specifically, the code is compiled with the following frameworks/flags:
-framework sfml-graphics -framework sfml-window -framework sfml-system

I wrote this project primarily on my mac, but it was tested and optimized in a linux virtual box (Ubuntu) with no issues. However, I couldn't test on ix-dev as I couldn't install the software. In case you run into problems, I did write a second version in the sfml_local_installation branch on bitbucket that has sfml locally insalled the correct Makefile to execute this. I couldn't test it (no display on ix-dev, not sure how to sim one) but it compiles properly and the implementation itself has been tested on other machines.