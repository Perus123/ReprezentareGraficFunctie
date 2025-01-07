#include "header.hpp"

double zoomLevel = 1;
class TextofBox
{
public:
    sf::RectangleShape box, deleteButton;
    sf::Text textBox, textButon;

    TextofBox(sf::Font &f)
    {

        box.setSize(sf::Vector2f(400, 40));
        box.setFillColor(sf::Color::White);
        box.setOutlineThickness(2);
        box.setOutlineColor(sf::Color::Black);
        deleteButton.setSize(sf::Vector2f(100, 40));
        deleteButton.setFillColor(sf::Color::Green);
        deleteButton.setOutlineThickness(2);
        deleteButton.setOutlineColor(sf::Color::Black);
        textBox.setFont(f);
        textBox.setString("");
        textBox.setCharacterSize(15);
        textBox.setFillColor(sf::Color::Black);
        textButon.setFont(f);
        textButon.setString("Sterge");
        textButon.setCharacterSize(15);
        textButon.setFillColor(sf::Color::Black);
    }
};
void grafic::calculateDeltaDivision()
{
    delta = (RightEnd - LeftEnd) / pointsNumber;
}

grafic::grafic(double screenWidthParameter, double screenHeightParameter)
{
    center.x = screenWidthParameter / 2;
    center.y = screenHeightParameter / 2;
    LeftEnd = -10;
    RightEnd = 10; /// coordonate stanga si dreapta
    screenWidth = screenWidthParameter;
    screenHeight = screenHeightParameter;
    division = screenWidth / 20;
    calculateDeltaDivision();
}

bool isMouseOverButton(const sf::RectangleShape &button, const sf::RenderWindow &window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f buttonPos = button.getPosition();
    sf::Vector2f buttonSize = button.getSize();
    return mousePos.x >= buttonPos.x && mousePos.x <= buttonPos.x + buttonSize.x &&
           mousePos.y >= buttonPos.y && mousePos.y <= buttonPos.y + buttonSize.y;
}

void grafic::initialiseGraphic(vector<function> &functions)
{
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Function Plotter");

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
    enterButton.setOutlineThickness(2);
    enterButton.setOutlineColor(sf::Color::Black);

    // Enter button text
    sf::Text enterButtonText;
    enterButtonText.setFont(font);
    enterButtonText.setString("Plot");
    enterButtonText.setCharacterSize(15);
    enterButtonText.setFillColor(sf::Color::Black);
    enterButtonText.setPosition(425, 20);

    // Error message setup
    sf::Text errorText;
    errorText.setFont(font);
    errorText.setCharacterSize(16);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(10, 60);

    sf::Vector2f coordonateAfisareFunctie = {10.0, 55.0}; /// 10 spatiu + 40 input + 5 bordura
    vector<TextofBox> chenareFunctii;
    // Setup for coordinate system
    sf::VertexArray lines(sf::Lines, 4);
    settingLines(lines);

    string currentInput;
    bool isInputActive = false;
    bool needsRecalculation = false;
    functions.clear();
    unordered_map<sf::Keyboard::Key, bool> keyStates = {
        {sf::Keyboard::W, false}, {sf::Keyboard::A, false}, {sf::Keyboard::S, false}, {sf::Keyboard::D, false}, {sf::Keyboard::Left, false}, {sf::Keyboard::Right, false}, {sf::Keyboard::Up, false}, {sf::Keyboard::Down, false}};

    window.setFramerateLimit(60);

    while (window.isOpen())
    {

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
                    for(vector<TextofBox>::iterator i = chenareFunctii.begin(); i!=chenareFunctii.end();)
                    {   
                        TextofBox auxButton=*i;
                        if(auxButton.deleteButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)){
                            chenareFunctii.erase(i);
                            functions.erase(functions.begin()+(i-chenareFunctii.begin()));
                        }
                        else
                            i++;
                    }
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

                            function newFunc;
                            TextofBox newChenar(font);
                            newFunc.input = currentInput;
                            /// curatareInput(newFunc.input);
                            newFunc.postfixOrderCalculation();
                            newFunc.calculatePoints(LeftEnd, RightEnd, delta);
                            functions.push_back(newFunc);
                            newChenar.textBox.setString(newFunc.input);
                            chenareFunctii.push_back(newChenar);
                            needsRecalculation = false;
                            isInputActive = false;
                            errorText.setString("");
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

                        function newFunc;
                        TextofBox newChenar(font);
                        newFunc.input = currentInput;
                        /// curatareInput(newFunc.input);
                        newFunc.postfixOrderCalculation();
                        newFunc.calculatePoints(LeftEnd, RightEnd, delta);
                        functions.push_back(newFunc);
                        newChenar.textBox.setString(newFunc.input);
                        chenareFunctii.push_back(newChenar);
                        needsRecalculation = false;
                        errorText.setString("");
                        isInputActive = false;
                        inputBox.setOutlineColor(sf::Color::Black);
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
                    zoomChange(0.5);
                    needsRecalculation = true;
                }
                if (event.key.code == sf::Keyboard::Subtract)
                {
                    zoomChange(2.0);
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
            screenMovement(keyStates, needsRecalculation);
            settingLines(lines);
        }

        // Recalculate points if needed

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

        // Draw coordinate system
        window.clear(sf::Color::White);
        window.draw(lines);
        drawNumbers(window);
        if (needsRecalculation == true)
        {
            for (int i = 0; i < functions.size(); i++)
            {
                functions[i].calculatePoints(LeftEnd, RightEnd, delta);
            }
            needsRecalculation = false;
        }

        // Draw function only if we have a valid one
        for (int i = 0; i < functions.size(); i++)
        {
            drawFunctionLines(window, functions[i]);
        }

        // Draw UI elements
        window.draw(inputBox);
        window.draw(inputText);
        window.draw(enterButton);
        window.draw(enterButtonText);
        window.draw(errorText);
        cout << LeftEnd << " " << RightEnd << '\n';
        for (int i = 0; i < chenareFunctii.size(); i++)
        {
            chenareFunctii[i].box.setPosition(sf::Vector2f(coordonateAfisareFunctie.x, coordonateAfisareFunctie.y + i * 40));
            chenareFunctii[i].textBox.setPosition(sf::Vector2f(coordonateAfisareFunctie.x + 5, coordonateAfisareFunctie.y + i * 40 + 5));
            chenareFunctii[i].deleteButton.setPosition(sf::Vector2f(coordonateAfisareFunctie.x + 410, coordonateAfisareFunctie.y + i * 40));
            chenareFunctii[i].textButon.setPosition(sf::Vector2f(coordonateAfisareFunctie.x + 415, coordonateAfisareFunctie.y + i * 40 + 5));
            window.draw(chenareFunctii[i].deleteButton);
            window.draw(chenareFunctii[i].box);
            window.draw(chenareFunctii[i].textBox);
            window.draw(chenareFunctii[i].textButon);
        }

        window.display();
    }
}
void grafic::drawNumbers(sf::RenderWindow &window)
{

    sf::Font fontTimesNewRoman;
    if (!fontTimesNewRoman.loadFromFile("TIMES.TTF"))
    {
        cerr << "Eroare: Fontul nu a putut fi incarcat\n";
        return;
    }

    double abscissa = center.x, ordinate = center.y;
    double index = 0;
    double step = zoomLevel;
    while (ordinate > 0)
    {
        sf::Text text;
        setText(text, fontTimesNewRoman, index, abscissa, ordinate);
        window.draw(text);
        sf::Vertex point1(sf::Vector2f(abscissa - 5, ordinate), sf::Color::Black);
        sf::Vertex point2(sf::Vector2f(abscissa + 5, ordinate), sf::Color::Black);
        sf::Vertex linie[] = {point1, point2};
        window.draw(linie, 2, sf::Lines);
        ordinate -= division;
        index += step;
    }

    ordinate = center.y + division;
    index = -step;
    while (ordinate <= screenHeight)
    {
        sf::Text text;
        setText(text, fontTimesNewRoman, index, abscissa, ordinate);
        window.draw(text);
        sf::Vertex point1(sf::Vector2f(abscissa - 5, ordinate), sf::Color::Black);
        sf::Vertex point2(sf::Vector2f(abscissa + 5, ordinate), sf::Color::Black);
        sf::Vertex linie[] = {point1, point2};
        window.draw(linie, 2, sf::Lines);
        ordinate += division;
        index -= step;
    }
    abscissa = center.x - division, ordinate = center.y;
    index = -step;
    while (abscissa > 0)
    {
        sf::Text text;
        setText(text, fontTimesNewRoman, index, abscissa, ordinate);
        window.draw(text);
        sf::Vertex point1(sf::Vector2f(abscissa, ordinate - 5), sf::Color::Black);
        sf::Vertex point2(sf::Vector2f(abscissa, ordinate + 5), sf::Color::Black);
        sf::Vertex linie[] = {point1, point2};
        window.draw(linie, 2, sf::Lines);
        abscissa -= division;
        index -= step;
    }
    LeftEnd = index;
    abscissa = center.x + division;
    index = step;
    while (abscissa < screenWidth)
    {
        sf::Text text;
        setText(text, fontTimesNewRoman, index, abscissa, ordinate);
        window.draw(text);
        sf::Vertex point1(sf::Vector2f(abscissa, ordinate - 5), sf::Color::Black);
        sf::Vertex point2(sf::Vector2f(abscissa, ordinate + 5), sf::Color::Black);
        sf::Vertex linie[] = {point1, point2};
        window.draw(linie, 2, sf::Lines);
        abscissa += division;
        index += step;
    }
    RightEnd = index;
}
void grafic::drawFunctionLines(sf::RenderWindow &window, const function &currentFunction)
{
    sf::VertexArray curvedLine(sf::LineStrip);
    for (int i = 0; i < currentFunction.values.size(); i++)
    {

        double xPunct = currentFunction.values[i].x, yPunct = currentFunction.values[i].y;
        if (yPunct == NAN)
            continue;
        sf::Vector2f point(
            center.x + xPunct * (division * (1.0 / zoomLevel)), // Mapam x la coordonate
            center.y - yPunct * (division * (1.0 / zoomLevel))  // Mapam y la coordonate
        );
        curvedLine.append(sf::Vertex(point, sf::Color::Black));
    }
    window.draw(curvedLine);
    for (int i = 1; i < currentFunction.values.size() - 1; i++) // Sărim peste primul și ultimul punct
    {
        double prevY = currentFunction.values[i - 1].y;
        double currY = currentFunction.values[i].y;
        double nextY = currentFunction.values[i + 1].y;
        if (prevY == NAN || currY == NAN || nextY == NAN)
            continue;
        double diff = abs(currentFunction.values[i - 1].x - currentFunction.values[i].x);

        // Convertim punctul curent la coordonate pe grafic
        double xPunct = currentFunction.values[i].x, yPunct = currentFunction.values[i].y;
        sf::Vector2f point(
            center.x + xPunct * (division * (1.0 / zoomLevel)), // Mapam x la coordonate
            center.y - yPunct * (division * (1.0 / zoomLevel)));
        // Verificăm dacă este minim sau maxim
        if (currY < prevY && currY < nextY && diff < 1) // Minimul local
        {
            sf::CircleShape minimumPoint(5.0f);                 // Raza cercului
            minimumPoint.setFillColor(sf::Color::Green);        // Verde pentru minim
            minimumPoint.setPosition(point.x - 5, point.y - 5); // Centrăm cercul
            window.draw(minimumPoint);
        }
        else if (currY > prevY && currY > nextY && diff < 1) // Maximul local
        {
            sf::CircleShape maximumPoint(5.0f);                 // Raza cercului
            maximumPoint.setFillColor(sf::Color::Red);          // Roșu pentru maxim
            maximumPoint.setPosition(point.x - 5, point.y - 5); // Centrăm cercul
            window.draw(maximumPoint);
        }
    }
}
void grafic::screenMovement(const unordered_map<sf::Keyboard::Key, bool> keyStates, bool &pointsRecalculation)
{

    if (keyStates.at(sf::Keyboard::W) || keyStates.at(sf::Keyboard::Up))
    {
        center.y += division;
    }
    if (keyStates.at(sf::Keyboard::S) || keyStates.at(sf::Keyboard::Down))
    {
        center.y -= division;
    }
    if (keyStates.at(sf::Keyboard::A) || keyStates.at(sf::Keyboard::Left))
    {
        center.x += division;
        pointsRecalculation = true;
    }
    if (keyStates.at(sf::Keyboard::D) || keyStates.at(sf::Keyboard::Right))
    {
        center.x -= division;
        pointsRecalculation = true;
    }
}
void grafic::settingLines(sf::VertexArray &lines)
{
    lines[0].position = sf::Vector2f(0, center.y);
    lines[0].color = sf::Color::Black;
    lines[1].position = sf::Vector2f(screenWidth, center.y);
    lines[1].color = sf::Color::Black;
    lines[2].position = sf::Vector2f(center.x, 0);
    lines[2].color = sf::Color::Black;
    lines[3].position = sf::Vector2f(center.x, screenHeight);
    lines[3].color = sf::Color::Black;
}
void grafic::zoomChange(const double zoomConstant)
{
    if (zoomConstant > 1)
    {
        center.x -= (center.x - screenWidth / 2) / 2;
        center.y -= (center.y - screenHeight / 2) / 2;
        if (abs(center.x - screenWidth / 2) < 1)
            center.x = screenWidth / 2;
        if (abs(center.y - screenHeight / 2) < 1)
            center.y = screenHeight / 2;
    }
    else
    {
        center.x += (center.x - screenWidth / 2);
        center.y += (center.y - screenHeight / 2);
    }

    LeftEnd *= zoomConstant;
    RightEnd *= zoomConstant;
    zoomLevel *= zoomConstant;

    calculateDeltaDivision();
}
void setText(sf::Text &text, sf::Font &font, double value, double abscissa, double ordinate)
{
    text.setFont(font);
    std::string result = to_string(value);
    result.erase(result.find_last_not_of('0') + 1);
    if (!result.empty() && result.back() == '.')
    {
        result.pop_back();
    }
    text.setString(result);
    text.setCharacterSize(15);
    text.setFillColor(sf::Color::Black);
    text.setPosition(abscissa, ordinate);
}