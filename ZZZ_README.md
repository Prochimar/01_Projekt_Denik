# Projekt
Elektronický deník pro vkládání úkolů


tu bude brainstorming aka mozková bouře

1. Zobrazení úkolů pro aktuální den, navigace mezi úkoly
2. Nastavitelnost priority
3. nastavitelnost splněnosti úkolu
4. kopírování úkolů/ přesouvání úkolů mezi daty

5. přepínání vypisování úkolů v týdenním režimu a denním režimu
----------------------------------------


použitelné knihovy
#include<stdlib.h>
    - příkaz: system("cls") //vymaže konzoli (to chceme dělat třeba po přidání úkolu aby to bylo clean)


Věci co taky použijeme
 %6d //doplní číslo mezerami zleva do celkové šířky 6 znaků



Interface

Vyjede legenda zmáčkni "a" pro přidání nového úkolu
Vyjede legenda zmáčkni "b" pro vybírání mezi úkoly => přes které pak budeš oznčovat ty hotové
Vyjede legenda zmáčkni "c" pro upravení úkolu stiskni [c]
Vyjede legenda zmáčkni "d" pro přepnutí mezi denním zobrazením a týdenním zobrazením 

možná by bylo hezčí kdyby to písmeno bylo v hranatých závorkách [a]
----------------------------------------

zmáčkneme "a" 
Program se zeptá na který den a čas (to by mělo jít s pomocí nějaké knihovny pro čas)
            - po zadání data a času se zeptá na daný úkol (obsah daného úkoliu)
            - poté se zeptá na prioritu pro daný úkol 1-5 (1 nejvyšší priorita, 5 nejnižší)
program by měl přiřadit číslo k danému úkolu (toto číslování by se mělo měnit podle toho jaký úkol má vyšší prioritu)

po uložení odpovědi vypíše všechny úkoly na daný den v týdnu/ pro celý týden (to by mělo jít s pomocí nějaké knihovny pro čas) (pomocí stavové proměnné, kterou budeme měnit "d" budeme přepínat mezi vypisováním daného dne a týdenního vypisování)
-----------------------------------------

zmáčkneme "b" 
Program se zeptá, který úkol chceme editovat 
    - zeptá se na den a poté na číslo úkolu
    - vyjede legenda
        zmáčkni "e" pro označení úkoly jako splněný
        zmáčkni "f" pro změnu priority
        zmáčkni "g" pro změnu obsahu úkolu

zadané úkoly by se měly číslovat - po splněný úkol by měl mít automaticky prioritu 5
-----------------------------------------

Když budeme v meny a zmáčkneme "d" program nám přepne vypisování mezi denním a týdenním zobrazením
2 Stavové proměnné [1,2] 
stiskem "d" se tato proměnná změní na tu druhou

Stav 1 
Vypisuje úkoly pro daný den

Stav 2
vypisuje úkoly pro daný týden 
-----------------------------------------

vždy nohoře bude legenda a ta bude oddělena ---------------------------------
