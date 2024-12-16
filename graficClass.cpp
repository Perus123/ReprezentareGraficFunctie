#include "header.hpp"

    void grafic::calculareDeltasiDivizune(){//functie pentru  calcula delta constant, in functie de capetele intervalului etc 
        delta=(capatDreapta-capatStanga)/numarPuncte;
        diviziune=latimeEcran/(capatDreapta-capatStanga);
   }
   grafic::grafic(double screenWidth, double screenHeight){
        centru.x=screenWidth/2;   
        centru.y=screenHeight/2; ///coordonate centru
        capatStanga=-10;
        capatDreapta=10;///coordonate stanga si dreapta
        calculareDeltasiDivizune();
        capatSus=floor(screenHeight/(diviziune*2));
        capatJos=-capatSus; ///calculare sus si jos, in functie de diviziuna initiala
        
   }
   void grafic::initializareGrafic(){
        sf::Window window(sf::VideoMode(800, 600), "My window");
        while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
   }
