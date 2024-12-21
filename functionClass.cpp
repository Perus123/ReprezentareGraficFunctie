#include "header.hpp"

functie::functie()
{
    input = "";
}

   double executareFunctie(double variabila, const functie &a){
   stack<double> stiva;
   int i = 0;
   for (int i = 0; i < a.sirPostfix.size(); i++)
   {
      // incepem prin a separa variabilele si le punem pe stiva
      string element = a.sirPostfix[i];
      if (element == "x")
         stiva.push(variabila);
      else if (isdigit(element[0]))
         stiva.push(stof(element));
      else if (element.size() >= 2) // daca avem o functie, o inlocuim cu valoarea ei
      {
         double var = stiva.top();
         stiva.pop();
         if (element == "ln")
         {  
            if(var<=0)
               throw invalid_argument("Se calculeaza ln dintr-un numar negativ");
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
      }
      else // avem 2 variabile despartite printr-un operator
      {
         double var1 = stiva.top();
         stiva.pop();
         
         double var2 =0;
         if(stiva.empty()==0){
            var2=stiva.top();
            stiva.pop();
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

void functie::calcularePuncte(double start, double end, double delta)
{   
    valori.clear();
    double valoare = start;
    while (valoare <= end)
    {
        double res = executareFunctie(valoare, *this);
        valori.emplace_back(punct({valoare, res}));
        valoare += delta;
    }
    cout<<valori.size()<<'\n';
    
}

void functie::calculareOrdinePostfix()
{
    int i = 0, lungime = input.size();
    std::string numar = "";            /// string auxiliar pentru adaugat elemente in stiva
    std::stack<std::string> operatori; /// stiva de operatori

    // 2*3 + (8-3)/2
    for (; i < lungime; i++)
    {
        char c = input[i]; /// patru cazuri 1. Functii sin, cos, ln, tan / 2. numere sau variabila x, 3. operatori / 4. paranteze
                           /// functia de prelucrare input se asigura ca nu vor exista probleme
        if (c >= 'a' && c <= 'z' && c != 'x')
        { // cazul 1, sin, cos , ln, tan, verificam daca este un cuvant care descrie o functie
            while (c >= 'a' && c <= 'z')
            {
                numar += c;
                i++;
                c = input[i];
            }
            i--;
            operatori.push(numar); // adaugam la final "numele" functiei in stiva de operatori
            numar = "";            /// creeaza functie si o pune in stack
        }
        else if (isdigit(c) || c == 'x')
        { // daca avem x sau un numar
            while (isdigit(c) || c == 'x')
            {
                numar += c;
                i++;
                c = input[i];
            }
            i--;
            sirPostfix.emplace_back(numar); // folosita pentru ca este mai rapida decat push_back
            numar = "";                     /// creeaza numar si il pune in sir
        }
        else if (isoperator(c))
        {
            while (!operatori.empty() &&
                   (prioritateOperator(operatori.top()) > prioritateOperator(c) ||
                    (prioritateOperator(operatori.top()) == prioritateOperator(c) && c != '^')))
            { // verificam prioritatea operatorilor
                sirPostfix.emplace_back(operatori.top());
                operatori.pop();
            }
            numar += c;
            operatori.push(numar);
            numar = ""; /// creeaza operator si il pune in sir
        }
        else if (c == '(')
        {
            operatori.push("(");
        }
        else if (c == ')')
        { // daca avem paranteza inchisa, dam pop la toti operatorii intalniti pana la '('
            while (!operatori.empty() && operatori.top() != "(")
            {
                sirPostfix.emplace_back(operatori.top());
                operatori.pop();
            }
            if (!operatori.empty()) // elimina si paranteza '('
                operatori.pop();
            if (!operatori.empty() && operatori.top()[0] >= 'a' && operatori.top()[0] <= 'z')
            { // daca aveai in fata o functie de tipul cos, ln etc.
                sirPostfix.emplace_back(operatori.top());
                operatori.pop();
            } /// rezolva parantezele
        }
    }
    while (!operatori.empty())
    {
        sirPostfix.emplace_back(operatori.top());
        operatori.pop();
    }
}

void functie::prelucrareInput()
{
    return;
}
/// cos(x)^2+x^3/5*2+sin(4*x/5)
/// 2*3 + (8-3)/2
