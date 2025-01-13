#include "header.hpp"

function::function()
{
    input = "";
    extremePoints = 0;
    sum = 0;
}

double executeFunction(double variable, const function &a, bool &validFunction)
{
    stack<double> stiva;
    int i = 0;

    for (int i = 0; i < a.postfixRow.size(); i++)
    {

        // incepem prin a separa variabilele si le punem pe stiva
        string element = a.postfixRow[i];

        if (element == "x")
            stiva.push(variable);
        else if (isdigit(element[0]))
            stiva.push(stof(element));

        else if (element.size() >= 2) // daca avem o functie, o inlocuim cu valoarea ei
        {

            double var;
            if (stiva.empty() == 0)
            {
                var = stiva.top();
                stiva.pop();
            }
            if (element == "ln")
            {
                if (var <= 0)
                    return NAN;
                var = log(var);
                stiva.push(var);
            }
            else if (element == "cos")
            {
                var = cos(var);
                stiva.push(var);
            }
            else if (element == "sin")
            {
                var = sin(var);
                stiva.push(var);
            }
            else if (element == "tan")
            {
                var = tan(var);
                stiva.push(var);
            }
            else if (element == "arctan")
            {
                var = atan(var);
                stiva.push(var);
            }
            else if (element == "arcsin")
            {
                var = asin(var);
                stiva.push(var);
            }
            else if (element == "arccos")
            {
                var = acos(var);
                stiva.push(var);
            }
            else
            {
                validFunction = false;

                return 0;
            }
        }
        else // avem 2 variabile despartite printr-un operator
        {
            double var1;
            bool check = false;
            if (stiva.empty() == 0)
            {
                var1 = stiva.top();
                check = true;
                stiva.pop();
            }

            double var2 = 0;
            if (stiva.empty() == 0)
            {
                var2 = stiva.top();
                stiva.pop();
            }
            if (check == false)
            {
                validFunction = false;
                return 0;
            }

            if (element == "+")
                stiva.push(var2 + var1);
            else if (element == "-")
                stiva.push(var2 - var1);
            else if (element == "*")
                stiva.push(var2 * var1);
            else if (element == "/")
                stiva.push(var2 / var1);
            else
                stiva.push(pow(var2, var1));
        }
    }
    return stiva.top();
}

bool function::calculatePoints(double start, double end, double delta)
{
    values.clear();
    sum = 0;
    double value = start;
    bool validFunction = true;

    double res = executeFunction(value, *this, validFunction);
    while (value <= end && validFunction)
    {
        sum+=res*delta;
        
        values.emplace_back(point({value, res}));
        res = executeFunction(value, *this, validFunction);
        value += delta;
    }
    sum+=res*delta;

    return validFunction;
}

void function::postfixOrderCalculation()
{
    int i = 0, length = input.size();
    std::string number = "";           /// string auxiliar pentru adaugat elemente in stiva
    std::stack<std::string> operators; /// stiva de operatori

    // 2*3 + (8-3)/2
    for (; i < length; i++)
    {
        char c = input[i]; /// patru cazuri 1. Functii sin, cos, ln, tan / 2. numere sau variabila x, 3. operatori / 4. paranteze
                           /// functia de prelucrare input se asigura ca nu vor exista probleme
        if (c >= 'a' && c <= 'z' && c != 'x')
        { // cazul 1, sin, cos , ln, tan, verificam daca este un cuvant care descrie o functie
            while (c >= 'a' && c <= 'z')
            {
                number += c;
                i++;
                c = input[i];
            }
            i--;

            operators.push(number); // adaugam la final "numele" functiei in stiva de operatori
            number = "";            /// creeaza functie si o pune in stack
        }
        else if (isdigit(c) || c == 'x')
        { // daca avem x sau un number
            while (isdigit(c) || c == 'x')
            {
                number += c;
                i++;
                c = input[i];
            }
            i--;
            postfixRow.emplace_back(number); // folosita pentru ca este mai rapida decat push_back
            number = "";                     /// creeaza numar si il pune in sir
        }
        else if (isoperator(c))
        {
            while (!operators.empty() &&
                   (operatorsPriority(operators.top()) > operatorsPriority(c) ||
                    (operatorsPriority(operators.top()) == operatorsPriority(c) && c != '^')))
            { // verificam prioritatea operatorilor
                postfixRow.emplace_back(operators.top());
                operators.pop();
            }
            number += c;
            operators.push(number);
            number = ""; /// creeaza operator si il pune in sir
        }
        else if (c == '(')
        {
            operators.push("(");
        }
        else if (c == ')')
        { // daca avem paranteza inchisa, dam pop la toti operatorii intalniti pana la '('
            while (!operators.empty() && operators.top() != "(")
            {
                postfixRow.emplace_back(operators.top());
                operators.pop();
            }
            if (!operators.empty()) // elimina si paranteza '('
                operators.pop();
            if (!operators.empty() && operators.top()[0] >= 'a' && operators.top()[0] <= 'z')
            { // daca aveai in fata o functie de tipul cos, ln etc.
                postfixRow.emplace_back(operators.top());
                operators.pop();
            } /// rezolva parantezele
        }
    }
    while (!operators.empty())
    {
        postfixRow.emplace_back(operators.top());
        operators.pop();
    }
}

void function::prelucrareInput()
{
    return;
}
/// cos(x)^2+x^3/5*2+sin(4*x/5)
/// 2*3 + (8-3)/2
