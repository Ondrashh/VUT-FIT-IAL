
/* ******************************* c202.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c202 - Zásobník znaků v poli                                        */
/*  Referenční implementace: Petr Přikryl, 1994                               */
/*  Vytvořil: Václav Topinka, září 2005                                       */
/*  Úpravy: Kamil Jeřábek, září 2019                                          */
/* ************************************************************************** */
/*
** Implementujte datový typ zásobník znaků ve statickém poli. ADT zásobník je
** reprezentován záznamem typu tStack. Statické pole 'arr' je indexováno
** do maximální hodnoty STACK_SIZE. Položka záznamu 'top' ukazuje na prvek
** na vrcholu zásobníku. Prázdnému zásobníku odpovídá hodnota top == -1,
** zásobníku s jedním prvkem hodnota 0, atd. Přesnou definici typů neleznete
** v hlavičkovém souboru c202.h.
**
** Implementujte následující funkce:
**
**    stackInit .... inicializace zásobníku
**    stackEmpty ... test na prázdnost zásobníku
**    stackFull .... test na zaplněnost zásobníku
**    stackTop ..... přečte hodnotu z vrcholu zásobníku
**    stackPop ..... odstraní prvek z vrcholu zásobníku
**    stackPush .... vloží prvku do zásobníku
**
** Své řešení účelně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

/* Vypracoval: Ondrje Pavlacky (xpavla15)
** VUT FIT, 2.BIT, 3.semestr, 2019/20 */

#include "c202.h"

int solved;
int STACK_SIZE = MAX_STACK;

void stackError ( int error_code ){
/*   ----------
** Vytiskne upozornění, že došlo k chybě při práci se zásobníkem.
**
** TUTO FUNKCI, PROSÍME, NEUPRAVUJTE!
*/
	static const char* SERR_STRINGS[MAX_SERR+1] = {"Unknown error","Stack error: INIT","Stack error: PUSH","Stack error: TOP"};
	if ( error_code <= 0 || error_code > MAX_SERR )
		error_code = 0;
	printf ( "%s\n", SERR_STRINGS[error_code] );
	err_flag = 1;
}

void stackInit ( tStack* s ) {

	if(s == NULL){	//Pokus je adresa zasobniku NULL, vyhodim chybu z procedury stackError
		stackError(SERR_INIT);
	}

	else s->top = -1;	//Vrchol zasobniku je nastaven na hodnotu -1, jak je definovano v zadani	
}

int stackEmpty ( const tStack* s ) {

	return (s->top != -1)? 0 : 42;	//Pokud zasobnik neni prazdny, vratim 0, jinak vracim nenulove cislo, v mem pripade cislo 42
}

int stackFull ( const tStack* s ) {

	return (s->top < (STACK_SIZE-1))? 0 : 42;	//Pokud je v zasobniku jeste misto vracim 0, jinak nenulove cislo, v mem pripade 42
}

void stackTop ( const tStack* s, char* c ) {

	if(stackEmpty(s)){	//Pouziji drive definovanou funkci stackEmpty pro overeni jestli je zasobmik prazdny
	 	stackError(SERR_TOP);	//Pokud je vyhazuji chybovou hlasku z procedury stackError
	}

	else *c=s->arr[s->top];	//do parametru c nahraji znak z vrcholu zasobniku
}


void stackPop ( tStack* s ) {

	if(stackEmpty(s)){}	//Pouziji drive definovanou funkci stackEmpty pro overeni jestli je zasobnik prazdny. Pokud je, nic se nedeje, protoze tam nic neni, neosetruji viz popis funkce
	else s->top--;	//Odstranim prvek z vrcholu zasobniku
}


void stackPush ( tStack* s, char c ) {

	if(stackFull(s)){	//Pouziji drive definovanou funkci stackFull pro overeni jestli je zasobnik plny. Pokud je, volam proceduru stackError
		stackError(SERR_PUSH);
	}
	else { s->top++;	//Prictu k indexu vrcholu zasobniku
		   s->arr[s->top] = c;	//Pridam znak na vrchol zasobniku
	}
}
/* Konec c202.c */
