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
        latimeEcran=screenWidth;
        inaltimeEcran=screenHeight;
        calculareDeltasiDivizune();
        capatSus=floor(screenHeight/(diviziune*2));
        capatJos=-capatSus; ///calculare sus si jos, in functie de diviziuna initiala
        
   }
   void grafic::initializareGrafic(const vector<functie>& functii){
        sf::RenderWindow window(sf::VideoMode(latimeEcran, inaltimeEcran), "My window");
        sf::VertexArray lines(sf::Lines, 4);

    // Setăm punctele pentru două linii
        lines[0].position = sf::Vector2f(0, centru.y);
        lines[0].color = sf::Color::Black;
        lines[1].position = sf::Vector2f(latimeEcran, centru.y);
        lines[1].color = sf::Color::Black;
        lines[2].position = sf::Vector2f(centru.x, 0);
        lines[2].color = sf::Color::Black;
        lines[3].position = sf::Vector2f(centru.x, inaltimeEcran);
        lines[3].color = sf::Color::Black;
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

        window.clear(sf::Color::White);
        ///aici se face desenul in sine
        window.draw(lines); 
        window.display();
    }
   }
