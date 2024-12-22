#include "header.hpp"
void grafic::calculareDeltasiDivizune()
{ // functie pentru  calcula delta constant, in functie de capetele intervalului etc
    delta = (capatDreapta - capatStanga) / numarPuncte;
}
/*bool isMouseOverButton(const sf::RectangleShape &button, const sf::RenderWindow &window)
{
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::FloatRect buttonBounds = button.getGlobalBounds();
    return buttonBounds.contains(static_cast<sf::Vector2f>(mousePosition));
}*/
grafic::grafic(double screenWidth, double screenHeight)
{
    centru.x = screenWidth / 2;
    centru.y = screenHeight / 2; /// coordonate centru
    capatStanga = -10;
    capatDreapta = 10; /// coordonate stanga si dreapta
    latimeEcran = screenWidth;
    inaltimeEcran = screenHeight;
    diviziune=latimeEcran/20;
    calculareDeltasiDivizune();
    /// capatSus = floor(screenHeight / (diviziune * 2));
    ////capatJos = -capatSus; /// calculare sus si jos, in functie de diviziuna initiala
}

bool isMouseOverButton(const sf::RectangleShape &button, const sf::RenderWindow &window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f buttonPos = button.getPosition();
    sf::Vector2f buttonSize = button.getSize();
    return mousePos.x >= buttonPos.x && mousePos.x <= buttonPos.x + buttonSize.x &&
           mousePos.y >= buttonPos.y && mousePos.y <= buttonPos.y + buttonSize.y;
}

void grafic::initializareGrafic(vector<functie> &functii)
{
    sf::RenderWindow window(sf::VideoMode(latimeEcran, inaltimeEcran), "Function Plotter");

    sf::Font font;
    if (!font.loadFromFile("TIMES.TTF"))
    {
        std::cerr << "Error loading font!" << std::endl;
        return;
    }

    // Input box setup
    sf::RectangleShape inputBox(sf::Vector2f(400, 40));
    inputBox.setPosition(10, 10);
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineThickness(2);
    inputBox.setOutlineColor(sf::Color::Black);

    // Input text setup
    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(20);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(15, 15);

    // Enter button setup
    sf::RectangleShape enterButton(sf::Vector2f(100, 40));
    enterButton.setPosition(420, 10);
    enterButton.setFillColor(sf::Color::Green);

    // Enter button text
    sf::Text enterButtonText;
    enterButtonText.setFont(font);
    enterButtonText.setString("Plot");
    enterButtonText.setCharacterSize(20);
    enterButtonText.setFillColor(sf::Color::White);
    enterButtonText.setPosition(445, 20);

    // Error message setup
    sf::Text errorText;
    errorText.setFont(font);
    errorText.setCharacterSize(16);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(10, 60);

    // Setup for coordinate system
    sf::VertexArray lines(sf::Lines, 4);
    setareLinii(lines);

    string currentInput;
    bool isInputActive = false;
    bool needsRecalculation = false;

    // Clear any existing functions
    functii.clear();

    unordered_map<sf::Keyboard::Key, bool> keyStates = {
        {sf::Keyboard::W, false}, {sf::Keyboard::A, false}, {sf::Keyboard::S, false}, {sf::Keyboard::D, false}, {sf::Keyboard::Left, false}, {sf::Keyboard::Right, false}, {sf::Keyboard::Up, false}, {sf::Keyboard::Down, false}};

    window.setFramerateLimit(60);

    while (window.isOpen())
    {   
        cout<<diviziune<<'\n';
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // Handle mouse clicks
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    // Check if clicked on input box
                    if (inputBox.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        isInputActive = true;
                        inputBox.setOutlineColor(sf::Color::Blue);
                    }
                    // Check if clicked on enter button
                    else if (enterButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        if (!currentInput.empty())
                        {
                            try
                            {
                                functie newFunc;
                                newFunc.input = currentInput;
                                /// curatareInput(newFunc.input);
                                if (newFunc.input.empty())
                                {
                                    errorText.setString("Invalid function format!");
                                    continue;
                                }
                                newFunc.calculareOrdinePostfix();
                                newFunc.calcularePuncte(capatStanga, capatDreapta, delta);
                                functii.push_back(newFunc);
                                needsRecalculation = false;
                                isInputActive=false;
                                errorText.setString("");
                            }
                            catch (const std::exception &e)
                            {
                                errorText.setString(e.what());
                            }
                        }
                    }
                    else
                    {
                        isInputActive = false;
                        inputBox.setOutlineColor(sf::Color::Black);
                    }
                }
            }

            // Handle text input
            if (isInputActive && event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode == '\b')
                { // Backspace
                    if (!currentInput.empty())
                    {
                        currentInput.pop_back();
                    }
                }
                else if (event.text.unicode == 13)
                { // Enter key
                    if (!currentInput.empty())
                    {
                        try
                        {
                            functie newFunc;
                            newFunc.input = currentInput;
                            /// curatareInput(newFunc.input);
                            if (newFunc.input.empty())
                            {
                                errorText.setString("Invalid function format!");
                                continue;
                            }
                            newFunc.calculareOrdinePostfix();
                            newFunc.calcularePuncte(capatStanga, capatDreapta, delta);
                            functii.push_back(newFunc);
                            needsRecalculation = false;
                            errorText.setString("");
                            isInputActive = false;
                            inputBox.setOutlineColor(sf::Color::Black);
                        }
                        catch (const std::exception &e)
                        {
                            errorText.setString(e.what());
                        }
                    }
                }
                else if (event.text.unicode < 128)
                { // Regular character
                    currentInput += static_cast<char>(event.text.unicode);
                }
                inputText.setString(currentInput);
            }

            // Handle keyboard movement
            if (event.type == sf::Event::KeyPressed)
            {
                if (keyStates.find(event.key.code) != keyStates.end())
                {
                    keyStates[event.key.code] = true;
                }
                // Handle zoom
                if (event.key.code == sf::Keyboard::Add)
                {
                    schimbareZoom(0.5);
                    needsRecalculation = true;
                }
                if (event.key.code == sf::Keyboard::Subtract)
                {
                    schimbareZoom(2.0);
                    needsRecalculation = true;
                }
            }
            if (event.type == sf::Event::KeyReleased)
            {
                if (keyStates.find(event.key.code) != keyStates.end())
                {
                    keyStates[event.key.code] = false;
                }
            }
        }

        // Handle continuous movement
        if (!isInputActive)
        { // Only allow movement if we have a function
            miscareEcran(keyStates, needsRecalculation);
            setareLinii(lines);
        }

        // Recalculate points if needed
        if (needsRecalculation == true)
            for (int i = 0; i < functii.size(); i++)
            {
                functii[i].calcularePuncte(capatStanga, capatDreapta, delta);
                needsRecalculation = false;
            }

        // Update hover effects
        if (enterButton.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
        {
            enterButton.setFillColor(sf::Color(0, 180, 0)); // Lighter green on hover
        }
        else
        {
            enterButton.setFillColor(sf::Color::Green);
        }

        // Drawing
        window.clear(sf::Color::White);

        // Draw coordinate system
        window.draw(lines);
        deseneazaNumere(window);

        // Draw function only if we have a valid one
        for (int i = 0; i < functii.size(); i++)
        {
            deseneazaLiniaFunctiei(window, functii[i]);
        }

        // Draw UI elements
        window.draw(inputBox);
        window.draw(inputText);
        window.draw(enterButton);
        window.draw(enterButtonText);
        window.draw(errorText);

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
            centru.x + xPunct * diviziune, // Mapam x la coordonate
            centru.y - yPunct * diviziune  // Mapam y la coordonate
        );
        linieCurbata.append(sf::Vertex(punct, sf::Color::Black));
    }
    window.draw(linieCurbata);
    for (int i = 1; i < functiaCurenta.valori.size() - 1; i++) // Sărim peste primul și ultimul punct
    {
        double prevY = functiaCurenta.valori[i - 1].y;
        double currY = functiaCurenta.valori[i].y;
        double nextY = functiaCurenta.valori[i + 1].y;

        // Convertim punctul curent la coordonate pe grafic
        double xPunct = functiaCurenta.valori[i].x, yPunct = functiaCurenta.valori[i].y;
        sf::Vector2f punct(
            centru.x + xPunct * diviziune,
            centru.y - yPunct * diviziune
        );

        // Verificăm dacă este minim sau maxim
        if (currY < prevY && currY < nextY) // Minimul local
        {
            sf::CircleShape punctMinim(5.0f); // Raza cercului
            punctMinim.setFillColor(sf::Color::Green); // Verde pentru minim
            punctMinim.setPosition(punct.x - 5, punct.y - 5); // Centrăm cercul
            window.draw(punctMinim);
        }
        else if (currY > prevY && currY > nextY) // Maximul local
        {
            sf::CircleShape punctMaxim(5.0f); // Raza cercului
            punctMaxim.setFillColor(sf::Color::Red); // Roșu pentru maxim
            punctMaxim.setPosition(punct.x - 5, punct.y - 5); // Centrăm cercul
            window.draw(punctMaxim);
        }
    }
}
void grafic::miscareEcran(const unordered_map<sf::Keyboard::Key, bool> keyStates, bool &recalculPuncte)
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
        capatStanga -= 1;
        capatDreapta -= 1;
        recalculPuncte = true;
    }
    if (keyStates.at(sf::Keyboard::D) || keyStates.at(sf::Keyboard::Right))
    {
        centru.x -= diviziune;
        capatStanga += 1;
        capatDreapta += 1;
        recalculPuncte = true;
    }
}
void grafic::setareLinii(sf::VertexArray &lines)
{
    lines[0].position = sf::Vector2f(0, centru.y);
    lines[0].color = sf::Color::Black;
    lines[1].position = sf::Vector2f(latimeEcran, centru.y);
    lines[1].color = sf::Color::Black;
    lines[2].position = sf::Vector2f(centru.x, 0);
    lines[2].color = sf::Color::Black;
    lines[3].position = sf::Vector2f(centru.x, inaltimeEcran);
    lines[3].color = sf::Color::Black;
}
void grafic::schimbareZoom(const double ConstantaDeZoom)
{

    capatStanga *= ConstantaDeZoom;
    capatDreapta *= ConstantaDeZoom;
    
    calculareDeltasiDivizune();
    diviziune*=(1/ConstantaDeZoom);
}

