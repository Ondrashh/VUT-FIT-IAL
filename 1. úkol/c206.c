
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2019
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

/* Vypracoval: Ondrej Pavlacky (xpavla15)
** VUT FIT, 2.BIT, 3.semestr, 2019/20 */

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {

   L->First=NULL; //---)
   L->Act=NULL;	  //---}-->Vsechny ukazatele nastavime na NULL
   L->Last=NULL;  //---)
}

void DLDisposeList (tDLList *L) {

	L->Act=L->First;	//Nastavim aktivni na prvni, protoze chci prochazet a rusit seznam pres aktivni
	while(L->Act!=L->Last){	//Pokud aktivni, ktery je ted nataven na prvni neni zaroven posledni
		tDLElemPtr p=L->Act->rptr;
		free(L->Act); 		//Postupne uvolnuji vsechny prvky seznamu
		L->Act=p;
	}
	free(L->Act); //Nakonec uvolnim i posledni
	L->Act=NULL;   //---)
	L->First=NULL; //---}-->Vsechny ukazatele nastavim na NULL
	L->Last=NULL;  //---)
}

void DLInsertFirst (tDLList *L, int val) {

   tDLElemPtr p=malloc(sizeof(struct tDLElem));	//Alokace pameti pro novy prvni prvek
	if(p == NULL){	//Pokud nastala chyba v alokaci, volej DLError
		DLError();
	}

	p->data=val;	//Priradim hodnotu novemu prvku
	p->lptr=NULL;	//Bude jej pridavat na zacatek, tedy levy ukazatel nikam neukazuje protoze je prvni
	p->rptr=L->First;	//Pravy ukazatel bude ukazovat na puvodni prvni prvek

	if(L->First != NULL){	//Pokud neni prvni NULL, tak nastav levy ukazatel prvniho na novy prvni prvek
		L->First->lptr = p;
	}
	else {	//Pokud neni zadny prvni, tak novy bude zaroven i posledni
		L->Last=p;
	}
	L->First = p;	//Prirad novy prvek na prvni misto
}

void DLInsertLast(tDLList *L, int val) {

	tDLElemPtr p=malloc(sizeof(struct tDLElem)); //Alokace pameti pro novy posledni prvek
	if(p == NULL){	//Pokud nastala chyba v alokaci, volej DLError
		DLError();
	}

	p->data=val;	//Priradim hodnotu novemu prvku
	p->rptr=NULL;	//Budu jej prirazovat na konec, tedy pravy ukazatel nikam neukazuje protoze je posledni
	p->lptr=L->Last;	//Levy ukazatel bude ukazovat na puvodni posledni prvek

	if(L->Last != NULL){	//Pokud neni posledni NULL, tak nastav pravy ukazarel posledniho na novy posledni prvek
		L->Last->rptr = p;
	}
	else {	//Pokud neni zadny posledni, tak novy bude zaroven i prvni
		L->First=p;
	}
	L->Last = p;	//Prirad novy prvek na posledni misto
}

void DLFirst (tDLList *L) {

	L->Act = L->First;	//Nastavim aktivni prvek na hodnotu prvniho prvku (nemusime nic testovat)
}

void DLLast (tDLList *L) {

	L->Act = L->Last; //Nastavim aktivni prvek na hodnotu posledniho prvku (nemusim nic testovat)
}

void DLCopyFirst (tDLList *L, int *val) {

	if(L->First !=NULL){ //Nahraj do parametru val hodnotu prviho, pokud nejaky prvni je, jinak volej DLError
		*val=L->First->data;
	}
	else DLError();
}

void DLCopyLast (tDLList *L, int *val) {

	if(L->Last != NULL){ //Nahraj do parametru val hodnotu posledniho, pokud nejaky posledni je, jinak volej DLError 
		*val=L->Last->data;
	}
	else DLError();
}

void DLDeleteFirst (tDLList *L) {

	if(L->First !=NULL){ //Kontroluji jestli prvni prvek vubec je
		tDLElemPtr p=L->First;	
		if(L->Act == L->First){	//Pokud je prvni prvek aktivni, aktivitu ztraci
			L->Act = NULL;
		}
		if(L->Last == L->First){ //Pokud je prvni prvek shodny s poslednim (v seznamu je jeden prvek), zrus ukazatel i na Last
			L->Last = NULL;
			L->First = NULL;
		}	
		else {	
			L->First = L->First->rptr; //Novy prvni prvek bude ten nasledujici z prava
			L->First->lptr = NULL;	//Levy ukazatel noveho prvniho nebude nic, protoze je prvni
		}
		free(p); //Uvolneni prvniho prvku
	}
}	

void DLDeleteLast (tDLList *L) {

	if(L->Last !=NULL){	//Kontroluji jestli posledni prvek vubec je
		tDLElemPtr p=L->Last;
		if(L->Act == L->Last){	//Pokud je posledni prvek aktivni, aktivitu ztraci
			L->Act = NULL;
		}
		if(L->First == L->Last){ //Pokud je posledni prvek shodny s prvnim (v seznamu je jeden prvek), zrus ukazatel i na First
			L->First = NULL;
			L->Last = NULL;
		}	
		else {
			L->Last = L->Last->lptr; //Novy posledni prvek bude ten predchozi z leva
			L->Last->rptr = NULL;	//Pravy ukazatel noveho posledniho nebude nic, protoze je posledni
		}
		free(p); //Uvolneni poslednih prvku
	}
}	

void DLPostDelete (tDLList *L) {
	
	if(L->Act != NULL){	//Pokud neni zadny prvek aktivni, nic se nedeje
		if(L->Act->rptr != NULL){	//Pokud za aktualnim prvkem nic neni (je posledni), nic se nedeje 
			tDLElemPtr p=L->Act->rptr; //Do p ulozime ukazatel na dalsi prvek za aktivnim
			L->Act->rptr = p->rptr; //Pravy ukazatel aktivniho bude pravy ukazatel nasledujiciho
			if(p == L->Last){	//Pokud je p poslednim prvkem, tak bude posledni aktivni
				L->Last = L->Act;
			}
			else p->rptr->lptr = L->Act;	//Levy ukazatel nasledujiciho prvku bude aktivni
			free(p); //Uvolneni p
		}
	}	
}

void DLPreDelete (tDLList *L) {
	
	if(L->Act != NULL){ //Pokud neni zadny prvek aktivni, nic se nedeje
		if(L->Act->lptr != NULL){	//Pokud pred aktualnim prvkem nic neni (je prvni), nic se nedeje
			tDLElemPtr p=L->Act->lptr; //Do p ulozime ukazatel na predchozi prvek pred aktivnim
			L->Act->lptr = p->lptr; //Levy ukazatel aktivniho bude levy ukazatel nasledujiciho
			if(p == L->First){	//Pokud je p prvnim prvkem, tak nude prvni aktivni 
				L->First = L->Act;
			}
			else p->lptr->rptr = L->Act;	//Pravy ukazatel predchoziho prvku bude aktivni
			free(p); //Uvolneni p
		}
	}
}

void DLPostInsert (tDLList *L, int val) {

		if(L->Act!=NULL){	//Pokud neni seznam aktivni, nic se nedeje
			if(L->Act==L->Last){	//Pokud je aktivni stejny s poslednim, muzu pouzit funkci DLInsertLast
				DLInsertLast(L,val);
				L->Act=L->Last->lptr;	//Pak jen nastavim aktivni na predchozi prvek, coz je ten puvodni aktivni
			}
			else {	//Pokud aktivni neni posledni
				tDLElemPtr p=malloc(sizeof(struct tDLElem));	//Alokuji pamet pro novy prvek
				if(p!=NULL){	//Pokud nenastala chyba pri alokaci pokracuji
					p->data=val;	//Priradim hodnotu novemu prvku
					p->rptr=L->Act->rptr;	//Pravy ukazatel bude stejny jak pravy ukazatel aktivniho
					p->lptr=L->Act->rptr->lptr;	//Levy ukazatel bude aktivni prvek
					L->Act->rptr->lptr=p;	//Levy ukazatel nasledujiciho prvku bude novy prvek
					L->Act->rptr=p;	//Pravy ukazatel aktivniho je novy prvek
				}
				else DLError();	
			}
		}
}

void DLPreInsert (tDLList *L, int val) {

	if(L->Act!=NULL){	//Pokud neni seznam aktivni, nic se nedeje
		if(L->Act==L->First){	//Pokud je aktivni stejny s prvnim, muzu pouzit funkci DLInsertFirst
			DLInsertFirst(L,val);
			L->Act=L->First->rptr;	//Pak jen nastavim aktivni na nasledujici, coz je puvodni aktivni
		}
		else{ //Pokud aktivni prvek neni prvni
			tDLElemPtr p=malloc(sizeof(struct tDLElem));	//Alokuji pamet pro novy prvek
				if(p!=NULL){	//Pokud nenastala chyba pri alokaci pokracuji
					p->data=val;	//Priradim hodnotu novemu prvku
					p->lptr=L->Act->lptr;	//Levy ukazatel bude stejny jak levy uakzatel aktivniho
					p->rptr=L->Act->lptr->rptr;	//Pravy ukazatel bude aktivni prvek
					L->Act->lptr->rptr=p; //Pravy ukazatel predchoziho prvku bude novy prvek
					L->Act->lptr=p;	//Levy ukazatel aktivniho je novy prvek
				}
				else DLError();
		}
	}
}

void DLCopy (tDLList *L, int *val) {

	if(L->Act != NULL){	//Pokud neni seznam aktivni volam DLError
	*val=L->Act->data;	//Do parametru val nahraji hodnotu aktivniho prvku
	}
	else DLError();
}

void DLActualize (tDLList *L, int val) {

	if(L->Act != NULL){		//Pokud neni seznam aktivni, nic se nedeje
		L->Act->data=val;	//Data aktivniho se zmeni na hodnotu val
	}
}

void DLSucc (tDLList *L) {

	if(L->Act != NULL && L->Last != NULL){	//Pokud je aktivni NULL a zaroven je posledni NULL nic se nedeje
		L->Act= L->Act->rptr;				//Posunu aktivitu na nasledujici prvek
	} 
}


void DLPred (tDLList *L) {

	if(L->Act != NULL && L->First != NULL){ //Pokud je aktivni NULL a zaroven je prvni NULL nic se nedeje
		L->Act= L->Act->lptr;				//Posunu aktivitu na predchozi prvek
	}	
}

int DLActive (tDLList *L) {

 return (L->Act != NULL)? 42 : 0;	//Pokud je seznam aktivni tak vracim nenulovou hodnotu, pokud neni vracim nulu
}
/* Konec c206.c*/