all:
	g++ main.cpp functionClass.cpp .\inputFunctions.cpp -o main.exe -Isrc/include -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system
	