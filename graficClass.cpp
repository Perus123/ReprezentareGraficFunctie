#include "header.hpp"
void grafic::calculareDeltasiDivizune()
{ // functie pentru  calcula delta constant, in functie de capetele intervalului etc
    delta = (capatDreapta - capatStanga) / numarPuncte;
    diviziune = latimeEcran / (capatDreapta - capatStanga);
}
grafic::grafic(double screenWidth, double screenHeight)
{
    centru.x = screenWidth / 2;
    centru.y = screenHeight / 2; /// coordonate centru
    capatStanga = -10;
    capatDreapta = 10; /// coordonate stanga si dreapta
    latimeEcran = screenWidth;
    inaltimeEcran = screenHeight;
    calculareDeltasiDivizune();
    capatSus = floor(screenHeight / (diviziune * 2));
    capatJos = -capatSus; /// calculare sus si jos, in functie de diviziuna initiala
}
void grafic::initializareGrafic(const vector<functie>& functii)
{
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
        /// aici se face desenul in sine

        window.draw(lines);
        deseneazaNumere(window);
        deseneazaLiniaFunctiei(window,functii[0]);
        window.display();
    }
}
void grafic::deseneazaNumere(sf::RenderWindow& window){
    
    sf::Font fontTimesNewRoman;
    if (!fontTimesNewRoman.loadFromFile("TIMES.TTF"))
    {
        cerr << "Eroare: Fontul nu a putut fi incarcat\n";
        return;
    }
    ///generare stanga - 0
    double abscisa=centru.x, ordonata=centru.y;
    int index=0;
    while(ordonata>0)
    {
        sf::Text text;
        text.setFont(fontTimesNewRoman);
        text.setString(to_string(index));
        text.setCharacterSize(15);
        text.setFillColor(sf::Color::Black);
        text.setPosition(abscisa, ordonata);
        window.draw(text);
        sf::Vertex punct1(sf::Vector2f(abscisa-5, ordonata), sf::Color::Black);
        sf::Vertex punct2(sf::Vector2f(abscisa+5, ordonata), sf::Color::Black);
        sf::Vertex linie[] ={punct1,punct2};
        window.draw(linie,2,sf::Lines);
        ordonata-=diviziune;
        index++;
    }  
    ///generare 0 - dreapta
    ordonata=centru.y+diviziune;
    index=-1;
    while(ordonata<=inaltimeEcran){
        sf::Text text;
        text.setFont(fontTimesNewRoman);
        text.setString(to_string(index));
        text.setCharacterSize(15);
        text.setFillColor(sf::Color::Black);
        text.setPosition(abscisa, ordonata);
        window.draw(text);
        sf::Vertex punct1(sf::Vector2f(abscisa-5, ordonata), sf::Color::Black);
        sf::Vertex punct2(sf::Vector2f(abscisa+5, ordonata), sf::Color::Black);
        sf::Vertex linie[] ={punct1,punct2};
        window.draw(linie,2,sf::Lines);
        ordonata+=diviziune;
        index--;
    }
    ///generare 0 - sus
    abscisa=centru.x-diviziune, ordonata=centru.y;
    index=-1;
    while(abscisa>0)
    {
        sf::Text text;
        text.setFont(fontTimesNewRoman);
        text.setString(to_string(index));
        text.setCharacterSize(15);
        text.setFillColor(sf::Color::Black);
        text.setPosition(abscisa, ordonata);
        window.draw(text);
        sf::Vertex punct1(sf::Vector2f(abscisa, ordonata-5), sf::Color::Black);
        sf::Vertex punct2(sf::Vector2f(abscisa, ordonata+5), sf::Color::Black);
        sf::Vertex linie[] ={punct1,punct2};
        window.draw(linie,2,sf::Lines);
        abscisa-=diviziune;
        index--;
    }
    ///generare 0 - jos
    abscisa=centru.x+diviziune;
    index=1;
    while(abscisa<=latimeEcran){
        sf::Text text;
        text.setFont(fontTimesNewRoman);
        text.setString(to_string(index));
        text.setCharacterSize(15);
        text.setFillColor(sf::Color::Black);
        text.setPosition(abscisa, ordonata);
        window.draw(text);
        sf::Vertex punct1(sf::Vector2f(abscisa, ordonata-5), sf::Color::Black);
        sf::Vertex punct2(sf::Vector2f(abscisa, ordonata+5), sf::Color::Black);
        sf::Vertex linie[] ={punct1,punct2};
        window.draw(linie,2,sf::Lines);
        abscisa+=diviziune;
        index++;
    }

}
void grafic::deseneazaLiniaFunctiei(sf::RenderWindow& window, const functie& functiaCurenta){
     sf::VertexArray linieCurbata(sf::LineStrip);
     for(int i=0;i<functiaCurenta.valori.size();i++){
        double xPunct=functiaCurenta.valori[i].x, yPunct=functiaCurenta.valori[i].y;
        sf::Vector2f punct(
            centru.x+ xPunct* diviziune,       // Map x to pixel coordinates
            centru.y - yPunct * diviziune         // Map y to pixel coordinates
        );
        linieCurbata.append(sf::Vertex(punct, sf::Color::Black));
     }
     window.draw(linieCurbata);
}