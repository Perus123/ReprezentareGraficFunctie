#include<header.hpp>
class grafic
{
   public:
   punct centru;
   double capatStanga, capatDreapta;
   const int numarPuncte=2000;
   double delta,diviziune;
   double latimeEcran, inaltimeEcran;
   double capatSus, capatJos;
   void calculareDeltasiDivizune(){//functie pentru  calcula delta constant, in functie de capetele intervalului etc 
        delta=(capatDreapta-capatStanga)/numarPuncte;
        diviziune=latimeEcran/(capatDreapta-capatStanga);
   }
   grafic(double screenWidth, double screenHeight){
        centru.x=screenWidth/2;   
        centru.y=screenHeight/2; ///coordonate centru
        capatStanga=-10;
        capatDreapta=10;///coordonate stanga si dreapta
        calculareDeltasiDivizune();
        capatSus=floor(screenHeight/(diviziune*2));
        capatJos=-capatSus; ///calculare sus si jos, in functie de diviziuna initiala
        
   }
   void initializareGrafic(){

   }
};