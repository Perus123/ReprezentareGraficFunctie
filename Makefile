all:
	g++ main.cpp functionClass.cpp inputFunctions.cpp graficClass.cpp -o main.exe -Isrc/include -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system
	