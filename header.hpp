#ifndef HEADER_H
#define HEADER_H
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <cmath>
using namespace std;
bool isoperator(char c)
{
   return (c == '+') || (c == '-') || (c == '*') || (c == '/') || (c == '^');
}
int prioritateOperator(char c)
{
   if (c == '+' || c == '-')
      return 1;
   if (c == '*' || c == '/')
      return 2;
   if (c == '^')
      return 3;
   return 0;
}
int prioritateOperator(const string &c)
{
   if (c[0] == '+' || c[0] == '-')
      return 1;
   if (c[0] == '*' || c[0] == '/')
      return 2;
   if (c[0] == '^')
      return 3;
   if (c[0] >= 'a' && c[0] <= 'z')
      return 4;
   return 0;
}
void stackdump(vector<string> st)
{

   for (int i = 0; i < st.size(); i++)
      cout << st[i] << " ";
}
struct punct
{
   float x=0,y=0;
};
class functie
{
public:
   string input;
   vector<string> sirPostfix;
   /// inputul trebuie prelucrat in REVERSE POLISH
   float delta;
   vector<punct> valori;
   functie()
   {
      input = "";
      delta = 0.01;
   }
   friend float executareFunctie(float valoare, const functie& a);
   void calcularePuncte(float start, float end){
      float  valoare=start;
      while(valoare<=end)
      {  
         float res=executareFunctie(valoare, *this);
         valori.emplace_back(punct{valoare, res});
         valoare+=delta;
      }
   }

   void calculareOrdinePostfix()
   {
      int i = 0, lungime = input.size();
      string numar = "";       /// string auxiliar pentru adaugat elemente in stiva
      stack<string> operatori; /// stiva de operatori
      // 2*3 + (8-3)/2
      for (i; i < lungime; i++)
      {
         char c = input[i]; /// patru cazuri 1. Functii sin, cos, ln, tan / 2. numere sau variabila x, 3. operatori / 4. paranteze
                            /// functia de prelucrare input se asigura ca nu vor exista probleme
         if (c >= 'a' && c <= 'z' && c != 'x')// cazul 1, sin, cos , ln, tan, verificam daca este un cuvant care descrie o functie
         {
            while (c >= 'a' && c <= 'z')
            {
               numar += c;
               i++;
               c = input[i];
            }
            i--;
            operatori.push(numar);// adaugam la final "numele" functiei in stiva de operatori
            numar = ""; /// creeaza functie si o pune in stack
         }
         else if (isdigit(c) || c == 'x')// daca avem x sau un numar 
         {
            while (isdigit(c) || c == 'x')
            {
               numar += c;
               i++;
               c = input[i];
            }
            i--;
            sirPostfix.emplace_back(numar);// folosita pentru ca este mai rapida decat push_back
            numar = ""; /// creeaza numar si il pune in sir
         }
         else if (isoperator(c))
         {

            while (operatori.empty() == 0 &&
                   (prioritateOperator(operatori.top()) > prioritateOperator(c) ||
                    (prioritateOperator(operatori.top()) == prioritateOperator(c) && c != '^')))// verificam prioritatea operatorilor
            {
               sirPostfix.emplace_back(operatori.top());
               operatori.pop();
            }
            numar += c;
            operatori.push(numar);
            numar = ""; /// creeaza operator si il pune in sir
         }
         else if (c == '(')
            operatori.push("(");
         else if (c == ')')// daca avem paranteza inchisa, dam pop la toti operatorii intalniti pana la '('
         {
            while (operatori.empty() == 0 && operatori.top() != "(")
            {

               sirPostfix.emplace_back(operatori.top());
               operatori.pop();
            }
            if (operatori.empty() == 0)// elimina si paranteza '('
               operatori.pop();
            if (operatori.empty() == 0 && operatori.top()[0] >= 'a' && operatori.top()[0] <= 'z')// daca aveai in fata o functie de tipul cos, ln etc.
            {
               sirPostfix.emplace_back(operatori.top());
               operatori.pop();
            } /// rezolva parantezele
         }
      }
      while (operatori.empty() == 0)
      {
         sirPostfix.emplace_back(operatori.top());
         operatori.pop();
      }
   }

   void prelucrareInput();
   /// cos(x)^2+x^3/5*2+sin(4*x/5)
   /// 2*3 + (8-3)/2
};
float executareFunctie(float variabila, const functie &a)
{
   stack<float> stiva;
   int i = 0;
   for (int i = 0; i < a.sirPostfix.size(); i++)
   {
      // incepem prin a separa variabilele si le punem pe stiva
      string element = a.sirPostfix[i];
      if (element == "x")
         stiva.push(variabila);
      else if (isdigit(element[0]))
         stiva.push(stof(element));
      else if (element.size() >= 2)// daca avem o functie, o inlocuim cu valoarea ei
      {
         float var = stiva.top();
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
            var = tan(var);
            stiva.push(var);
         }
         else if (element == "tan")
         {
            var = tan(var);
            stiva.push(var);
         }
      }
      else// avem 2 variabile despartite printr-un operator
      {
         float var1=stiva.top();
         stiva.pop();
         float var2=stiva.top();
         stiva.pop();
         if(element=="+")
            stiva.push(var2+var1);
         else if(element=="-")
            stiva.push(var2-var1);
         else if(element=="*")
            stiva.push(var2*var1);
         else if(element=="/")
            stiva.push(var2/var1);
         else 
            stiva.push(pow(var2, var1));

      }
   }
   return stiva.top();
}

string curatareInput (string s)
{
   stack<string> operanzi;
   
}


#endif