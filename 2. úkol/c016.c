	/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, 2014-2018
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

void htInit ( tHTable* ptrht ) {

	int n = 0; 
	while(n < HTSIZE){
 		(*ptrht)[n]=NULL; //Na vsechny mista da null
 	 	n++;
 	}
}

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {

	tHTItem *ukazatel = (*ptrht)[hashCode(key)];
	while(ukazatel != NULL){ //Dokud nejsme na konci prohledavej
 		if(ukazatel->key == key){ //Pokud se budou klice rovnat, tak vratim ten hledany prvek
 			return ukazatel;
 		} //Posunu se dal v hledani, na dalsi misto
 		ukazatel=ukazatel->ptrnext;
 	}
	return NULL;
}

void htInsert ( tHTable* ptrht, tKey key, tData data ) {

 	tHTItem *zaloha_dat= (*ptrht)[hashCode(key)];
 	tHTItem *ukazatel = htSearch(ptrht, key); 	
 	if(ukazatel != NULL) { //Pokud nalezneme prvek, ktery jsme chteli vlozi, tudiz uz existuje, upravime jen data
 		ukazatel->data= data;
 	}
 	else { //Jinak vytvorime novy
 		(*ptrht)[hashCode(key)] = malloc(sizeof(tHTItem)); //Alokujeme misto
		(*ptrht)[hashCode(key)]->key = key; //Vlozime klic
		(*ptrht)[hashCode(key)]->data = data; //Vlozime data
		(*ptrht)[hashCode(key)]->ptrnext = zaloha_dat; //A posuneme se dal do tabulky
 	}	
}

tData* htRead ( tHTable* ptrht, tKey key ) {

	tHTItem *ukazatel= (htSearch(ptrht, key)); 
 	return (ukazatel==NULL)? NULL : &ukazatel->data; //Pokud neni polozka nalezena vracime NULL, jinak vracime data
}

void htDelete ( tHTable* ptrht, tKey key ) {

	tHTItem *ukazatel = (*ptrht)[hashCode(key)];
	tHTItem *ukazatel_dalsi = NULL ;
	tHTItem *ukazatel_predchozi = NULL;
	while(ukazatel != NULL &&  ukazatel->key != key){ //Dokud nejsme na konci nebo nenalezneme hledany mazany vyraz
		if(ukazatel != NULL){ //Tady si kontorluji jestli mam jit dal, nebo jestli jsem nasel mazany vyraz
			ukazatel_predchozi = ukazatel;
			ukazatel = ukazatel->ptrnext;

		}
	}
	if(ukazatel != NULL){ 
		if(ukazatel_predchozi == NULL){ //Kontroluji jestli nahodou mazany prvek neni prvni
			    ukazatel_dalsi = ukazatel->ptrnext;	//Zalohuji data dalsich prvku
				free(ukazatel);  //Uvolnim data
				(*ptrht)[hashCode(key)] = ukazatel_dalsi; //Posunu se
		}
		else { //Pokud neni prvni, tak si ukazu predchozim za mazany a smazu jej
			ukazatel_predchozi->ptrnext = ukazatel->ptrnext;
			free(ukazatel);
		}
	}
}

void htClearAll ( tHTable* ptrht ) {

	tHTItem *ukazatel_na_dalsi = NULL;

	int i = 0;
	while(i<HTSIZE){	//Dokud nejsem na konci
		while((*ptrht)[i] != NULL){ //Pokud tam neco je, mazu jej
			ukazatel_na_dalsi= (*ptrht)[i]->ptrnext; //Zalohuji si ukazatel na dalsi
			free((*ptrht)[i]); //Uvolnim data
			(*ptrht)[i]= ukazatel_na_dalsi; //Posunu se dal

		}
		i++;
	}
}
