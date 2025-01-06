#include "header.hpp"
bool isoperator(char c)
{
   return (c == '+') || (c == '-') || (c == '*') || (c == '/') || (c == '^');
}
int operatorsPriority(char c)
{
   if (c == '+' || c == '-')
      return 1;
   if (c == '*' || c == '/')
      return 2;
   if (c == '^')
      return 3;
   return 0;
}
int operatorsPriority(const string &c)
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
bool isFunction(const string& s)
{
   return (s == "cos" || s == "sin" || s == "tan" || s == "ln");
}
bool isoperand(char c)
{
   return c=='*' || c=='-' ||c=='+' ||c=='^';
}
void curatareInput(string& s)
{
   string word;
   string expression;
   int inBrackets = 0;
   string noSpaceAux="";
   for(int i=0;i<s.size();i++)
      if(s[i]!=' ')
         noSpaceAux+=s[i];
   s=noSpaceAux;
   for (int i = 0; i < s.size(); i++)
   {
      char c = s[i];
      word = "";
      if (c >= 'a' && c <= 'z' && c != 'x')
      {
         bool ok=1;
         while (i < s.size() && c >= 'a' && c <= 'z' && c != 'x' && word.size()<3 && ok) // daca ar trebui sa fie numele unei functii
         {
            word += c;
            
            i++;
            c = s[i];
            if(word=="ln")
            ok=0;
         }
         i--;

         if (isFunction(word)) // daca avem numele unei functii, o adaugam si deschidem o paranteza;
         {
            expression += word;
            expression += '(';
            inBrackets++;
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
         word += c;
         c = s[i + 1];
         i++;
         while (isdigit(c)) // daca este numar format din mai multe cifre
         {
            word += c;
            i++;
            c = s[i];
         }
         if (c == 'x') // daca dupa numar avem x, o sa traducem in nr*x
            expression += word + "*" + 'x',i++;
         else
            expression += word;
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
               word += c;
               i++;
               c = s[i];
            }
            i--;
            expression += (word + '*');
         }
         expression += 'x';
         expression += ')';
         inBrackets-- ;
      }
      else if(isoperand(c))
         expression += c;
   }
   while (inBrackets !=0)
     { expression += ')';
       inBrackets--;
     }

   s=expression;
}