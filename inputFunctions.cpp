#include "header.hpp"
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
bool estefunctie(const string& s)
{
   return (s == "cos" || s == "sin" || s == "tan" || s == "ln");
}
bool isoperand(char c)
{
   return c=='*' || c=='-' ||c=='+' ||c=='^';
}
void curatareInput(string& s)
{
   string cuvant;
   string expresie;
   int inparanteza = 0;
   string auxFaraSpatii="";
   for(int i=0;i<s.size();i++)
      if(s[i]!=' ')
         auxFaraSpatii+=s[i];
   s=auxFaraSpatii;
   for (int i = 0; i < s.size(); i++)
   {
      char c = s[i];
      cuvant = "";
      if (c >= 'a' && c <= 'z' && c != 'x')
      {
         bool ok=1;
         while (i < s.size() && c >= 'a' && c <= 'z' && c != 'x' && cuvant.size()<3 && ok) // daca ar trebui sa fie numele unei functii
         {
            cuvant += c;
            
            i++;
            c = s[i];
            if(cuvant=="ln")
            ok=0;
         }
         i--;

         if (estefunctie(cuvant)) // daca avem numele unei functii, o adaugam si deschidem o paranteza;
         {
            expresie += cuvant;
            expresie += '(';
            inparanteza++;
         }
         else
         {
            cout << "nu a fost introdus numele unei functii";
            s="";
            return;
         }
      }

      else if (isdigit(c)) // daca e numar
      {
         cuvant += c;
         c = s[i + 1];
         i++;
         while (isdigit(c)) // daca este numar format din mai multe cifre
         {
            cuvant += c;
            i++;
            c = s[i];
         }
         if (c == 'x') // daca dupa numar avem x, o sa traducem in nr*x
            expresie += cuvant + "*" + 'x',i++;
         else
            expresie += cuvant;
            i--;
      }
      else if (c == 'x')
      {
         if (isdigit(s[i + 1])) // daca dupa x avem un numar
         {
            i++;
            c = s[i];
            while (isdigit(c)) // daca este numar
            {
               cuvant += c;
               i++;
               c = s[i];
            }
            i--;
            expresie += (cuvant + '*');
         }
         expresie += 'x';
         expresie += ')';
         inparanteza-- ;
      }
      else if(isoperand(c))
         expresie += c;
   }
   while (inparanteza !=0)
     { expresie += ')';
       inparanteza--;
     }

   s=expresie;
}