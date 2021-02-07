/* c401.c: **********************************************************}
{* Téma: Rekurzivní implementace operací nad BVS
**                                         Vytvořil: Petr Přikryl, listopad 1994
**                                         Úpravy: Andrea Němcová, prosinec 1995
**                                                      Petr Přikryl, duben 1996
**                                                   Petr Přikryl, listopad 1997
**                                  Převod do jazyka C: Martin Tuček, říjen 2005
**                                         Úpravy: Bohuslav Křena, listopad 2009
**                                                 Karel Masařík, říjen 2013
**                                                 Radek Hranický 2014-2018
**
** Implementujte rekurzivním způsobem operace nad binárním vyhledávacím
** stromem (BVS; v angličtině BST - Binary Search Tree).
**
** Klíčem uzlu stromu je jeden znak (obecně jím může být cokoliv, podle
** čeho se vyhledává). Užitečným (vyhledávaným) obsahem je zde integer.
** Uzly s menším klíčem leží vlevo, uzly s větším klíčem leží ve stromu
** vpravo. Využijte dynamického přidělování paměti.
** Rekurzivním způsobem implementujte následující funkce:
**
**   BSTInit ...... inicializace vyhledávacího stromu
**   BSTSearch .... vyhledávání hodnoty uzlu zadaného klíčem
**   BSTInsert .... vkládání nové hodnoty
**   BSTDelete .... zrušení uzlu se zadaným klíčem
**   BSTDispose ... zrušení celého stromu
**
** ADT BVS je reprezentován kořenovým ukazatelem stromu (typ tBSTNodePtr).
** Uzel stromu (struktura typu tBSTNode) obsahuje klíč (typu char), podle
** kterého se ve stromu vyhledává, vlastní obsah uzlu (pro jednoduchost
** typu int) a ukazatel na levý a pravý podstrom (LPtr a RPtr). Přesnou definici typů
** naleznete v souboru c401.h.
**
** Pozor! Je třeba správně rozlišovat, kdy použít dereferenční operátor *
** (typicky při modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem
** (např. při vyhledávání). V tomto příkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, použijeme dereferenci.
**/

#include "c401.h"
int solved;

void BSTInit (tBSTNodePtr *RootPtr) {

	*RootPtr=NULL; //Inicializace stromu
}

int BSTSearch (tBSTNodePtr RootPtr, char K, int *Content)	{

	if(RootPtr != NULL){ //Kdyz koren neni NULL, hledam key ve stromu
		if(RootPtr->Key == K){
			*Content = RootPtr->BSTNodeCont;
			return TRUE;
		} //Ternarnim operatorem zjistim, jestli jdu doprava, nebo doleva
		else return BSTSearch(((RootPtr->Key > K)? RootPtr->LPtr : RootPtr->RPtr), K, Content);
	}
	else return FALSE;
}


void BSTInsert (tBSTNodePtr* RootPtr, char K, int Content)	{

	if(*RootPtr == NULL){ //Pokud uzel neni, tak jej vytvorim a priradim hodnoty
   		*RootPtr = malloc(sizeof(struct tBSTNode));
			if(!*RootPtr) return;
			(*RootPtr)->RPtr        = NULL;
			(*RootPtr)->LPtr		= NULL;
			(*RootPtr)->Key  	    = K;
			(*RootPtr)->BSTNodeCont = Content;
   } 
   else if((*RootPtr)->Key == K){ //Kdyz existuje, obnovim hodnotu
   		(*RootPtr)->BSTNodeCont = Content;
   }
   else { //Ternarnim operatorem zjistim, jestli jdu doprava, nebo doleva
   		BSTInsert((((*RootPtr)->Key > K)? &((*RootPtr)->LPtr) : &((*RootPtr)->RPtr)), K, Content);
   }
}

void ReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr) {

	tBSTNodePtr ukazatel= NULL;
	if((*RootPtr)->RPtr != NULL) ReplaceByRightmost(PtrReplaced, &((*RootPtr)->RPtr)); //Posouvani se k nejpravejsimu uzlu
	else{ //Az ho najdu, uvolnim ten, ktery chci smazat a smazu
		
		PtrReplaced->Key= (*RootPtr)->Key;
		PtrReplaced->BSTNodeCont= (*RootPtr)->BSTNodeCont;
		ukazatel = (*RootPtr);
		(*RootPtr) = (*RootPtr)->LPtr;
		free(ukazatel);
		}
}

void BSTDelete (tBSTNodePtr *RootPtr, char K){

	tBSTNodePtr ukazatel;
	if(*RootPtr != NULL){ //Kontroluji za neni prazdny, nez zacnu neno hledat/mazat
		if((*RootPtr)->Key == K){ //Kdyz jsem ho nasel musit zaridit fungovani stromu
			if((*RootPtr)->LPtr == NULL && (*RootPtr)->RPtr == NULL){ //Kdyz nema zadne syny, staci uvolnit
				free(*RootPtr);
				*RootPtr= NULL;
			}
			else if((*RootPtr)->LPtr != NULL && (*RootPtr)->RPtr != NULL){ //Kdyz ma oba syny musim projit strom
				ReplaceByRightmost(*RootPtr, ((*RootPtr)->LPtr->RPtr != NULL)? &(*RootPtr)->LPtr->RPtr : &(*RootPtr)->LPtr);

			}
			else { //Tady se rozhoduji jestli ma leveho, nebo praveho syna a tim padem jdu doleva, nebo do prava
				ukazatel=(((*RootPtr)->LPtr != NULL && (*RootPtr)->RPtr == NULL)? (((*RootPtr)->LPtr)) : (((*RootPtr)->RPtr)));
				free(*RootPtr);
				*RootPtr= ukazatel;
			}

		} //Tady se rozhoduji jakou cestou jit
		else BSTDelete(((*RootPtr)->Key > K)? &((*RootPtr)->LPtr) : &((*RootPtr)->RPtr), K);
	}
}

void BSTDispose (tBSTNodePtr *RootPtr) {

	if(*RootPtr != NULL){ //Kontrola zda je strom NULL
		for(int i = 0; i < 2; i++) { //Cyklus aby se mazani provedlo 2x, prostě to funguje
		BSTDispose((((*RootPtr)->LPtr != NULL)? (&(*RootPtr)->LPtr) : (&(*RootPtr)->RPtr)));
		}
		free(*RootPtr);
		(*RootPtr) = NULL;
	}
}

/* konec c401.c */