#include "header.hpp"

void grafic::initializeThemes() // initializam temele
{
    // tema default
    lightTheme = {
        sf::Color::White,     // backgroundColor
        sf::Color::Black,     // textColor
        sf::Color::Black,     // axisColor
        sf::Color::Black,     // functionColor
        sf::Color::White,     // inputBoxFillColor
        sf::Color::Black,     // inputBoxOutlineColor
        sf::Color::Green,     // buttonFillColor
        sf::Color(0, 180, 0), // buttonHoverColor
        sf::Color::Green,     // minimumPointColor
        sf::Color::Red        // maximumPointColor
    };

    // tema intunecata
    darkTheme = {
        sf::Color(30, 30, 30),  // backgroundColor
        sf::Color::White,       // textColor
        sf::Color::White,       // axisColor
        sf::Color(0, 255, 0),   // functionColor
        sf::Color(50, 50, 50),  // inputBoxFillColor
        sf::Color::White,       // inputBoxOutlineColor
        sf::Color(0, 100, 0),   // buttonFillColor
        sf::Color(0, 130, 0),   // buttonHoverColor
        sf::Color(0, 255, 100), // minimumPointColor
        sf::Color(255, 50, 50)  // maximumPointColor
    };

    isDarkTheme = false; // Start with light theme
}

void grafic::setupThemeButton(sf::Font &font)
{
    themeButton.setSize(sf::Vector2f(100, 40));
    themeButton.setPosition(screenWidth - 110, 10);
    themeButton.setFillColor(getCurrentTheme().buttonFillColor);
    themeButton.setOutlineThickness(2);
    themeButton.setOutlineColor(getCurrentTheme().inputBoxOutlineColor);

    themeButtonText.setFont(font);
    themeButtonText.setString("Theme");
    themeButtonText.setCharacterSize(15);
    themeButtonText.setFillColor(getCurrentTheme().textColor);
    themeButtonText.setPosition(screenWidth - 105, 20);
}

void grafic::toggleTheme()
{
    isDarkTheme = !isDarkTheme;
    Theme &currentTheme = getCurrentTheme();

    // Update theme button colors
    themeButton.setFillColor(currentTheme.buttonFillColor);
    themeButton.setOutlineColor(currentTheme.inputBoxOutlineColor);
    themeButtonText.setFillColor(currentTheme.textColor);
}

double zoomLevel = 1;
class TextofBox
{
public:
    sf::RectangleShape box, deleteButton; // box reprezinta fundalul
    sf::Text textBox, textButon;          // textBox este textul functiei afisate iar textButton textul butonului

    TextofBox(sf::Font &f)
    {
        // setam fontul default pentru program
        box.setSize(sf::Vector2f(400, 40));
        box.setFillColor(sf::Color::White);
        box.setOutlineThickness(2);
        box.setOutlineColor(sf::Color::Black);
        // setam carcateristicile la butonul de delete
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
    delta = (rightEnd - leftEnd) / pointsNumber;
}

grafic::grafic(double screenWidthParameter, double screenHeightParameter) // constructorul clasei grafic
{
    center.x = screenWidthParameter / 2;
    center.y = screenHeightParameter / 2;
    leftEnd = -10;
    rightEnd = 10; /// coordonate stanga si dreapta
    screenWidth = screenWidthParameter;
    screenHeight = screenHeightParameter;
    division = screenWidth / 20;
    calculateDeltaDivision();
}

bool isMouseOverButton(const sf::RectangleShape &button, const sf::RenderWindow &window) // functia pentru a vedea daca coordonatele mouseului se afla in interiorul unui buton
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

    initializeThemes();
    setupThemeButton(font);

    // setup-ul pentru caseta de input
    sf::RectangleShape inputBox(sf::Vector2f(400, 40));
    inputBox.setPosition(10, 10);
    inputBox.setFillColor(getCurrentTheme().inputBoxFillColor);
    inputBox.setOutlineThickness(2);
    inputBox.setOutlineColor(getCurrentTheme().inputBoxOutlineColor);

    // setup-ul pentru textul din caseta de input
    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(20);
    inputText.setFillColor(getCurrentTheme().textColor);
    inputText.setPosition(15, 15);

    // setup pentru butonul de enter
    sf::RectangleShape enterButton(sf::Vector2f(100, 40));
    enterButton.setPosition(420, 10);
    enterButton.setFillColor(getCurrentTheme().buttonFillColor);
    enterButton.setOutlineThickness(2);
    enterButton.setOutlineColor(getCurrentTheme().inputBoxOutlineColor);

    // setup pentru textul din caseta de enter
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

    sf::Vector2f coordonatesandFunctionDisplay = {10.0, 55.0}; /// 10 spatiu + 40 input + 5 bordura , locul de unde incepe afisarea functiei
    vector<TextofBox> borderFunctions;                         // chenareFunctii, vector care stocheaza elemente de tipul TextofBox (asta inseamna gen, chenare and all ??)

    sf::VertexArray lines(sf::Lines, 4);
    settingLines(lines);

    string currentInput;
    bool isInputActive = false;
    bool needsRecalculation = false;
    functions.clear();
    // map pentru initializare de setup, inputurile sunt marcate ca si false
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
            // handling pt click-urile de la mouse
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (themeButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) // daca mouse-ul este peste butonul de schimbare a temelor
                    {
                        toggleTheme();

                        // Update all UI elements with new theme colors
                        inputBox.setFillColor(getCurrentTheme().inputBoxFillColor);
                        inputBox.setOutlineColor(getCurrentTheme().inputBoxOutlineColor);
                        inputText.setFillColor(getCurrentTheme().textColor);
                        enterButton.setFillColor(getCurrentTheme().buttonFillColor);
                        enterButtonText.setFillColor(getCurrentTheme().textColor);
                        errorText.setFillColor(sf::Color::Red); // Keep error text red in both themes

                        // Update existing function boxes
                        for (auto &box : borderFunctions)
                        {
                            box.box.setFillColor(getCurrentTheme().inputBoxFillColor);
                            box.box.setOutlineColor(getCurrentTheme().inputBoxOutlineColor);
                            box.textBox.setFillColor(getCurrentTheme().textColor);
                            box.deleteButton.setFillColor(getCurrentTheme().buttonFillColor);
                            box.textButon.setFillColor(getCurrentTheme().textColor);
                        }
                    }
                    for (vector<TextofBox>::iterator i = borderFunctions.begin(); i != borderFunctions.end();) // nu-s sigur ca am inteles aici complet
                    {
                        TextofBox auxButton = *i;
                        if (auxButton.deleteButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                        {
                            borderFunctions.erase(i);
                            functions.erase(functions.begin() + (i - borderFunctions.begin()));
                        }
                        else
                            i++;
                    }
                    if (inputBox.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) // daca introducem text, facem caseta albastra
                    {
                        isInputActive = true;
                        inputBox.setOutlineColor(sf::Color::Blue);
                    }
                    // Daca apasam pe butonul enter
                    else if (enterButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {                              // trebuie adaugat daca chestia aia nu e functie ig
                        if (!currentInput.empty()) // daca avem o functie scrisa inauntru
                        {
                            function newFunc;
                            TextofBox newChenar(font);
                            newFunc.input = currentInput;
                            /// curatareInput(newFunc.input);
                            newFunc.postfixOrderCalculation();
                            newFunc.calculatePoints(leftEnd, rightEnd, delta);
                            functions.push_back(newFunc);
                            newChenar.textBox.setString(newFunc.input);
                            borderFunctions.push_back(newChenar);
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

            // verificam input-ul
            if (isInputActive && event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode == '\b')
                { // daca apasam pe backspace, stergem un element
                    if (!currentInput.empty())
                    {
                        currentInput.pop_back();
                    }
                }
                else if (event.text.unicode == 13)
                { // daca apas pe enter
                    if (!currentInput.empty())
                    {
                        function newFunc;
                        TextofBox newChenar(font);
                        newFunc.input = currentInput;
                        /// curatareInput(newFunc.input);
                        newFunc.postfixOrderCalculation();
                        newFunc.calculatePoints(leftEnd, rightEnd, delta);
                        functions.push_back(newFunc);
                        newChenar.textBox.setString(newFunc.input);
                        borderFunctions.push_back(newChenar);
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

            // cum procesam input-ul de la tastatura
            if (event.type == sf::Event::KeyPressed)
            {
                if (keyStates.find(event.key.code) != keyStates.end())
                {
                    keyStates[event.key.code] = true;
                }
                // procesare zoom
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

        // daca ne miscam incontinuu
        if (!isInputActive)
        { // ne putem misca doar daca avem o functie (sa ramana feature saaau ?)
            screenMovement(keyStates, needsRecalculation);
            settingLines(lines);
        }

        // updatam efectul de hover
        if (isMouseOverButton(themeButton, window))
        {
            themeButton.setFillColor(getCurrentTheme().buttonHoverColor);
        }
        else
        {
            themeButton.setFillColor(getCurrentTheme().buttonFillColor);
        }

        // Desenare
        window.clear(getCurrentTheme().backgroundColor);

        // Update the color of coordinate system lines
        lines[0].color = getCurrentTheme().axisColor;
        lines[1].color = getCurrentTheme().axisColor;
        lines[2].color = getCurrentTheme().axisColor;
        lines[3].color = getCurrentTheme().axisColor;

        window.draw(lines);
        drawNumbers(window);
        if (needsRecalculation == true)
        {
            for (int i = 0; i < functions.size(); i++)
            {
                functions[i].calculatePoints(leftEnd, rightEnd, delta);
            }
            needsRecalculation = false;
        }

        // Draw functions with theme colors
        for (const auto &func : functions)
        {
            drawFunctionLines(window, func);
        }

        // Draw UI elements
        window.draw(inputBox);
        window.draw(inputText);
        window.draw(enterButton);
        window.draw(enterButtonText);
        window.draw(errorText);
        window.draw(themeButton);
        window.draw(themeButtonText);

        cout << leftEnd << " " << rightEnd << '\n';      // afisam in consola capetele stanga si dreapta
        for (int i = 0; i < borderFunctions.size(); i++) // aici desenam propriu zis toate elementele de tip, butoane , setand mai intai de unde le punem
        {
            borderFunctions[i].box.setPosition(sf::Vector2f(coordonatesandFunctionDisplay.x, coordonatesandFunctionDisplay.y + i * 40));
            borderFunctions[i].textBox.setPosition(sf::Vector2f(coordonatesandFunctionDisplay.x + 5, coordonatesandFunctionDisplay.y + i * 40 + 5)); //+5 pentru ca este in interiorul casetei
            borderFunctions[i].deleteButton.setPosition(sf::Vector2f(coordonatesandFunctionDisplay.x + 410, coordonatesandFunctionDisplay.y + i * 40));
            borderFunctions[i].textButon.setPosition(sf::Vector2f(coordonatesandFunctionDisplay.x + 415, coordonatesandFunctionDisplay.y + i * 40 + 5)); // +5 pentru ca este in interiorul casetei
            window.draw(borderFunctions[i].deleteButton);
            window.draw(borderFunctions[i].box);
            window.draw(borderFunctions[i].textBox);
            window.draw(borderFunctions[i].textButon);
        }
        window.display();
    }
}
// desenam numerele de pe grafic
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
    
    // Draw small lines for axis markers
    while (ordinate > 0)
    {
        sf::Text text;
        setText(text, fontTimesNewRoman, index, abscissa, ordinate);
        text.setFillColor(getCurrentTheme().textColor); // Use theme color for text
        window.draw(text);
        
        // Draw axis markers using theme colors
        sf::Vertex point1(sf::Vector2f(abscissa - 5, ordinate), getCurrentTheme().axisColor);
        sf::Vertex point2(sf::Vector2f(abscissa + 5, ordinate), getCurrentTheme().axisColor);
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
        text.setFillColor(getCurrentTheme().textColor); // Use theme color for text
        window.draw(text);
        
        // Draw axis markers using theme colors
        sf::Vertex point1(sf::Vector2f(abscissa - 5, ordinate), getCurrentTheme().axisColor);
        sf::Vertex point2(sf::Vector2f(abscissa + 5, ordinate), getCurrentTheme().axisColor);
        sf::Vertex linie[] = {point1, point2};
        window.draw(linie, 2, sf::Lines);
        
        ordinate += division;
        index -= step;
    }

    abscissa = center.x - division;
    ordinate = center.y;
    index = -step;
    while (abscissa > 0)
    {
        sf::Text text;
        setText(text, fontTimesNewRoman, index, abscissa, ordinate);
        text.setFillColor(getCurrentTheme().textColor); // Use theme color for text
        window.draw(text);
        
        // Draw axis markers using theme colors
        sf::Vertex point1(sf::Vector2f(abscissa, ordinate - 5), getCurrentTheme().axisColor);
        sf::Vertex point2(sf::Vector2f(abscissa, ordinate + 5), getCurrentTheme().axisColor);
        sf::Vertex linie[] = {point1, point2};
        window.draw(linie, 2, sf::Lines);
        
        abscissa -= division;
        index -= step;
    }
    leftEnd = index;
    abscissa = center.x + division;
    index = step;
    while (abscissa < screenWidth)
    {
        sf::Text text;
        setText(text, fontTimesNewRoman, index, abscissa, ordinate);
        text.setFillColor(getCurrentTheme().textColor); // Use theme color for text
        window.draw(text);
        
        // Draw axis markers using theme colors
        sf::Vertex point1(sf::Vector2f(abscissa, ordinate - 5), getCurrentTheme().axisColor);
        sf::Vertex point2(sf::Vector2f(abscissa, ordinate + 5), getCurrentTheme().axisColor);
        sf::Vertex linie[] = {point1, point2};
        window.draw(linie, 2, sf::Lines);
        
        abscissa += division;
        index += step;
    }
    rightEnd = index;
}

// Also modify the setText function to use theme colors
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
    // Note: The color is now set in drawNumbers function using the current theme
    text.setPosition(abscissa, ordinate);
}

void grafic::drawFunctionLines(sf::RenderWindow &window, const function &currentFunction) // unim punctele din grafic pentru functia curenta
{
    sf::VertexArray curvedLine(sf::LineStrip);
    for (const auto &value : currentFunction.values) // parcurgerea punctelor functiei
    {
        if (std::isnan(value.y)) // daca valoarea lui y nu este numar, acesta este ignorat
            continue;

        sf::Vector2f point(
            center.x + value.x * (division * (1.0 / zoomLevel)), // Mapam x la coordonate dupa formula
            center.y - value.y * (division * (1.0 / zoomLevel))  // Mapam y la coordonate
        );
        curvedLine.append(sf::Vertex(point, getCurrentTheme().functionColor)); // adaugam punctul
    }
    window.draw(curvedLine); // aici desenam linia care uneste punctele de pe ecran

    for (int i = 1; i < currentFunction.values.size() - 1; i++) // Sărim peste primul și ultimul punct si determinam minimele si maximele locale
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
            minimumPoint.setFillColor(getCurrentTheme().minimumPointColor);// Verde pentru minim
            minimumPoint.setPosition(point.x - 5, point.y - 5); // Centrăm cercul
            window.draw(minimumPoint);
        }
        else if (currY > prevY && currY > nextY && diff < 1) // Maximul local
        {
            sf::CircleShape maximumPoint(5.0f);                 // Raza cercului
            maximumPoint.setFillColor(getCurrentTheme().maximumPointColor);// Roșu pentru maxim
            maximumPoint.setPosition(point.x - 5, point.y - 5); // Centrăm cercul
            window.draw(maximumPoint);
        }
    }
}
// functia propriu zisa care creeaza miscarea pe ecran, incrementand sau decrementand coorodnatele cu diviziunea aleasa
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
    // trebuie recalculate punctele doar daca ne miscam stanga sau dreapta deoarece modificam domeniul;
}
void grafic::settingLines(sf::VertexArray &lines) // setam pentru cele doua axe, coordonatele capetelor si culoarea
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

    leftEnd *= zoomConstant;
    rightEnd *= zoomConstant;
    zoomLevel *= zoomConstant;

    calculateDeltaDivision();
}