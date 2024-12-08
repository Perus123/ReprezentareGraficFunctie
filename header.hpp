#ifndef HEADER_H
#define HEADER_H
#include <iostream> 
#include <string>
#include <queue>
#include <stack>
#include <vector>
using namespace std;
bool isoperator(char c){
   return (c=='+')||(c=='-')||(c=='*')||(c=='/')||(c=='^');
}
int prioritateOperator(char c){
   if(c=='+'||c=='-') return 1;
   if(c=='*'||c=='/') return 2;
   if(c=='^') return 3;
   return 4;
}
int prioritateOperator(const string& c){
   if(c[0]=='+'||c[0]=='-') return 1;
   if(c[0]=='*'||c[0]=='/') return 2;
   if(c[0]=='^') return 3;
   return 0;
}
void stackdump(queue<string> st)
{ 
  
   while(st.empty()==0)
      {
         cout<<st.front();
         st.pop();

      }
}
class functie
{  
   public:
   string input;
   queue<string> sirPostfix;
   ///inputul trebuie prelucrat in REVERSE POLISH 
   float delta;
   vector<float> valori;
   functie(){
      input="";
      delta=0.001;
   }
   void calcularePuncte(float start, float end);
   void calculareOrdinePostfix(){
      int i=0, lungime=input.size();
      string numar="";
      stack<string>operatori;
      //2*3 + (8-3)/2
      for(i;i<lungime;i++){
         char c=input[i];
         
         if(isdigit(c)){
            while(isdigit(c))
               {
                  numar+=c;
                  i++;
                  c=input[i];
               }
            i--;
            sirPostfix.push(numar);
            numar="";
         }
         else if(isoperator(c)){
            
            while(operatori.empty()==0 && prioritateOperator(operatori.top())>=prioritateOperator(c))
            {
               sirPostfix.push(operatori.top());
               operatori.pop();
            }
            numar+=c;
            operatori.push(numar);
            numar="";
         }
         else if(c=='(')
            operatori.push("(");  
         else if(c==')')
            {  cout<<c<< " ";
               while(operatori.empty()==0 && operatori.top()!="(")
                  {  
                     
                     sirPostfix.push(operatori.top());
                     operatori.pop();

                  }
               if(operatori.empty()==0)
               operatori.pop();
               
            }
      }
      while(operatori.empty()==0)
         {
            sirPostfix.push(operatori.top());
            operatori.pop();
         }
   }

   void prelucrareInput();
   ///cos(x)^2+x^3/5*2+sin(4*x/5)
   ///2*3 + (8-3)/2
};

#endif 