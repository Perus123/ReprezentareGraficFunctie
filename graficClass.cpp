#include "header.hpp"
void grafic::calculareDeltasiDivizune()
{ // functie pentru  calcula delta constant, in functie de capetele intervalului etc
    delta = (capatDreapta - capatStanga) / numarPuncte;
    diviziune = latimeEcran / (capatDreapta - capatStanga);
}
bool isMouseOverButton(const sf::RectangleShape& button, const sf::RenderWindow& window) {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::FloatRect buttonBounds = button.getGlobalBounds();
    return buttonBounds.contains(static_cast<sf::Vector2f>(mousePosition));
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
    ///capatSus = floor(screenHeight / (diviziune * 2));
    ////capatJos = -capatSus; /// calculare sus si jos, in functie de diviziuna initiala
}
void grafic::initializareGrafic(vector<functie> &functii)
{
    sf::RenderWindow window(sf::VideoMode(latimeEcran, inaltimeEcran), "My window");
    sf::Font f;
    if (!f.loadFromFile("TIMES.ttf")) 
        return;
    sf::RectangleShape button(sf::Vector2f(200, 50));
    button.setFillColor(sf::Color::Blue);
    button.setPosition(0, 0);
    // Creează textul pentru buton
    sf::Text buttonText;
    buttonText.setFont(f);
    buttonText.setString("Introduce funcția");
    buttonText.setCharacterSize(20);
    buttonText.setFillColor(sf::Color::Black);
    buttonText.setPosition(button.getPosition().x + 20, button.getPosition().y + 10);
    // Setăm punctele pentru două linii 
    sf::VertexArray lines(sf::Lines, 4);
    setareLinii(lines);
    functii[0].calcularePuncte(capatStanga, capatDreapta, delta);
    unordered_map<sf::Keyboard::Key, bool> tastaApasata = {
        {sf::Keyboard::W, false},
        {sf::Keyboard::A, false},
        {sf::Keyboard::S, false},
        {sf::Keyboard::D, false},
        {sf::Keyboard::Left, false},
        {sf::Keyboard::Right, false},
        {sf::Keyboard::Up, false},
        {sf::Keyboard::Down, false}
    };
    string userInput;
    window.setFramerateLimit(30);
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        bool nevoieDeRecalculare=false;
        bool isInputActive = false;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (tastaApasata.find(event.key.code) != tastaApasata.end())
                    tastaApasata[event.key.code] = true;
            }
            if (event.type == sf::Event::KeyReleased)
            {
                if (tastaApasata.find(event.key.code) != tastaApasata.end())
                    tastaApasata[event.key.code] = false;
            }
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    if (event.mouseWheelScroll.delta > 0) {
                            schimbareZoom(0.9);
                            nevoieDeRecalculare=true;
                    } else if (event.mouseWheelScroll.delta < 0) {
                            schimbareZoom(1.1);
                            nevoieDeRecalculare=true;
                    }
                }
                //recalculare pentru toate functiile
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (isMouseOverButton(button, window)) {
                    isInputActive = true;
                }
            }
            if (isInputActive && event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b') { // Backspace
                    if (!userInput.empty()) userInput.pop_back();
                } else if (event.text.unicode == '\r') { // Enter
                    isInputActive = false; // Dezactivează introducerea textului
                    std::cout << "Text introdus: " << userInput << "\n";
                } else if (event.text.unicode < 128) { // Adaugă caractere ASCII
                    userInput += static_cast<char>(event.text.unicode);
                }
                 // Actualizează textul afișat
            }
        }
        
        if (isMouseOverButton(button, window)) {
            button.setFillColor(sf::Color::White);
        } else {
            button.setFillColor(sf::Color::Blue);
        }
        setareLinii(lines);
        miscareEcran(tastaApasata, nevoieDeRecalculare);
        if(nevoieDeRecalculare){
            functii[0].calcularePuncte(capatStanga, capatDreapta, delta);
            cout<<delta<<'\n';
        }
        /// aici se face desenul in sine 
        window.clear(sf::Color::White);
        window.draw(lines);
        window.draw(button);
        window.draw(buttonText);
        deseneazaNumere(window);
        deseneazaLiniaFunctiei(window, functii[0]);
        window.display();
    }
}
void grafic::deseneazaNumere(sf::RenderWindow &window)
{

    sf::Font fontTimesNewRoman;
    if (!fontTimesNewRoman.loadFromFile("TIMES.TTF"))
    {
        cerr << "Eroare: Fontul nu a putut fi incarcat\n";
        return;
    }
    /// generare stanga - 0
    double abscisa = centru.x, ordonata = centru.y;
    int index = 0;
    while (ordonata > 0)
    {
        sf::Text text;
        text.setFont(fontTimesNewRoman);
        text.setString(to_string(index));
        text.setCharacterSize(15);
        text.setFillColor(sf::Color::Black);
        text.setPosition(abscisa, ordonata);
        window.draw(text);
        sf::Vertex punct1(sf::Vector2f(abscisa - 5, ordonata), sf::Color::Black);
        sf::Vertex punct2(sf::Vector2f(abscisa + 5, ordonata), sf::Color::Black);
        sf::Vertex linie[] = {punct1, punct2};
        window.draw(linie, 2, sf::Lines);
        ordonata -= diviziune;
        index++;
    }
    /// generare 0 - dreapta
    ordonata = centru.y + diviziune;
    index = -1;
    while (ordonata <= inaltimeEcran)
    {
        sf::Text text;
        text.setFont(fontTimesNewRoman);
        text.setString(to_string(index));
        text.setCharacterSize(15);
        text.setFillColor(sf::Color::Black);
        text.setPosition(abscisa, ordonata);
        window.draw(text);
        sf::Vertex punct1(sf::Vector2f(abscisa - 5, ordonata), sf::Color::Black);
        sf::Vertex punct2(sf::Vector2f(abscisa + 5, ordonata), sf::Color::Black);
        sf::Vertex linie[] = {punct1, punct2};
        window.draw(linie, 2, sf::Lines);
        ordonata += diviziune;
        index--;
    }
    /// generare 0 - sus
    abscisa = centru.x - diviziune, ordonata = centru.y;
    index = -1;
    while (abscisa > 0)
    {
        sf::Text text;
        text.setFont(fontTimesNewRoman);
        text.setString(to_string(index));
        text.setCharacterSize(15);
        text.setFillColor(sf::Color::Black);
        text.setPosition(abscisa, ordonata);
        window.draw(text);
        sf::Vertex punct1(sf::Vector2f(abscisa, ordonata - 5), sf::Color::Black);
        sf::Vertex punct2(sf::Vector2f(abscisa, ordonata + 5), sf::Color::Black);
        sf::Vertex linie[] = {punct1, punct2};
        window.draw(linie, 2, sf::Lines);
        abscisa -= diviziune;
        index--;
    }
    /// generare 0 - jos
    abscisa = centru.x + diviziune;
    index = 1;
    while (abscisa <= latimeEcran)
    {
        sf::Text text;
        text.setFont(fontTimesNewRoman);
        text.setString(to_string(index));
        text.setCharacterSize(15);
        text.setFillColor(sf::Color::Black);
        text.setPosition(abscisa, ordonata);
        window.draw(text);
        sf::Vertex punct1(sf::Vector2f(abscisa, ordonata - 5), sf::Color::Black);
        sf::Vertex punct2(sf::Vector2f(abscisa, ordonata + 5), sf::Color::Black);
        sf::Vertex linie[] = {punct1, punct2};
        window.draw(linie, 2, sf::Lines);
        abscisa += diviziune;
        index++;
    }
}
void grafic::deseneazaLiniaFunctiei(sf::RenderWindow &window, const functie &functiaCurenta)
{
    sf::VertexArray linieCurbata(sf::LineStrip);
    for (int i = 0; i < functiaCurenta.valori.size(); i++)
    {
        double xPunct = functiaCurenta.valori[i].x, yPunct = functiaCurenta.valori[i].y;
        sf::Vector2f punct(
            centru.x + xPunct * diviziune, // Map x to pixel coordinates
            centru.y - yPunct * diviziune  // Map y to pixel coordinates
        );
        linieCurbata.append(sf::Vertex(punct, sf::Color::Black));
    }
    window.draw(linieCurbata);
}
void grafic::miscareEcran(const unordered_map<sf::Keyboard::Key, bool> keyStates, bool& recalculPuncte)
{   
   
    if (keyStates.at(sf::Keyboard::W) || keyStates.at(sf::Keyboard::Up))
    {
        centru.y += diviziune;
    }
    if (keyStates.at(sf::Keyboard::S) || keyStates.at(sf::Keyboard::Down))
    {
        centru.y -= diviziune;
    }
    if (keyStates.at(sf::Keyboard::A) || keyStates.at(sf::Keyboard::Left))
    {
        centru.x += diviziune;
        capatStanga-=1;
        capatDreapta-=1;
        recalculPuncte=true;
    }
    if (keyStates.at(sf::Keyboard::D) || keyStates.at(sf::Keyboard::Right))
    {
        centru.x -= diviziune;
        capatStanga+=1;
        capatDreapta+=1;
        recalculPuncte=true;
    }

}
void grafic::setareLinii(sf::VertexArray& lines){
    lines[0].position = sf::Vector2f(0, centru.y);
    lines[0].color = sf::Color::Black;
    lines[1].position = sf::Vector2f(latimeEcran, centru.y);
    lines[1].color = sf::Color::Black;
    lines[2].position = sf::Vector2f(centru.x, 0);
    lines[2].color = sf::Color::Black;
    lines[3].position = sf::Vector2f(centru.x, inaltimeEcran);
    lines[3].color = sf::Color::Black;
}
void grafic::schimbareZoom(const double ConstantaDeZoom){

    capatStanga*=ConstantaDeZoom;
    capatDreapta*=ConstantaDeZoom;
    calculareDeltasiDivizune();
}