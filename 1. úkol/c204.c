
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického
** výrazu do postfixového tvaru. Pro převod využijte zásobník (tStack),
** který byl implementován v rámci příkladu c202. Bez správného vyřešení
** příkladu c202 se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix .... konverzní funkce pro převod infixového výrazu 
**                       na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar .... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

/* Vypracoval: Ondrej Pavlacky (xpavla15)
** VUT FIT, 2.BIT, 3.semestr, 2019/20 */

#include "c204.h"

int solved;

void untilLeftPar ( tStack* s, char* postExpr, unsigned* postLen ) {

	while(s->arr[s->top] != '(' && s->top > -1){	//Pokud na vrcholu zasobniku neni leva zavorka, nebo neni prazdny
		postExpr[*postLen] =  s->arr[s->top];		//Vloz operatory do postExpr, vrchol zasobniku se zmensi
		s->top--;
		(*postLen)++;
	}		
	
	if(s->arr[s->top] == '('){	//Pokud je na vrcholu zasobniku leva zavorka, preskoc ji, my ji nevypisujeme

		s->top--;
	}
}

void doOperation ( tStack* s, char c, char* postExpr, unsigned* postLen) {

	if(c == '('){	//Pokud je c leva zavorka poslu jej na zasobnik
		stackPush(s,c);
	}
	else if(c == '*' || c == '/' || c == '+' || c == '-'){ 	//Pokud je c jeden z operatoru, rozhoduji se co s nim
		for(;;){	//Nekonecny cyklus na protrideni operatoru
			if(stackEmpty(s) || s->arr[s->top] == '(') { //Pokud je zasobnik prazdny, nebo je na vrcholu leva zavorka posli znak na vrchol zasobniku
				stackPush(s,c);
				break;
			}
			else if((c == '*' || c == '/') && ((s->arr[s->top] == '+') || (s->arr[s->top] == '-'))){ //Je-li znak c * nebo / a zaroven na vrcholu
				stackPush(s,c);																		 //zasobniku je + nebo -, dej znak na vrchol zasobniku
				break;
			}
			else {	//Tady vkladam znak do postExpr, znak odebiram ze zasobniku a posouvam se dal
				postExpr[*postLen] = s->arr[s->top];
				stackPop(s);
				(*postLen)++;
			}
		}
	}

	else if(c == ')'){	//Pokud je c prava zavorka, prechazim do funkce untilLeftPar, ktera vyprazdnuje zasobnik
		untilLeftPar(s,postExpr,postLen);
	} 
	else if(c == '='){	//Pokud je c rovna se, prechazim do funkce untilLeftPar, ktera vyprazdnuje zasobnik
		untilLeftPar(s,postExpr,postLen);
		postExpr[*postLen]= '=';	//Doplneni rovna se na konec postExpr
	}

    else {	//Tady davam do postExpr vsechny identifikatory a cislice
    	postExpr[*postLen]=c;
    	(*postLen)++;	
    }
}

char* infix2postfix (const char* infExpr) {

	unsigned post_index=0;
	unsigned index=0;
	tStack zasobnik;
	stackInit(&zasobnik);
	char *postExpr = malloc(sizeof(char) * MAX_LEN);	//Alokace pameti pro vystupni retezec

	//infExpr  <---- tady mam vstupni infixovy vyraz
	//postExpr <---- sem davam zpracovany vyraz
	//zasobnik <---- zasobnik na operatory
	
	while(infExpr[index] != '\0'){ //Cyklus na pruchod celeho vstuhniho vyrazu
		doOperation(&zasobnik,infExpr[index],postExpr,&post_index); 
		index++;
	}
	postExpr[(++post_index)] = '\0'; //Cokoli co je za postfixovym retezcem uz cist nechci
	return postExpr;
}
/* Konec c204.c */
