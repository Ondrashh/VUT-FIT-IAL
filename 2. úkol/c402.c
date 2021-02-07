/* c402.c: ********************************************************************}
{* Téma: Nerekurzivní implementace operací nad BVS
**                                     Implementace: Petr Přikryl, prosinec 1994
**                                           Úpravy: Petr Přikryl, listopad 1997
**                                                     Petr Přikryl, květen 1998
**			  	                        Převod do jazyka C: Martin Tuček, srpen 2005
**                                         Úpravy: Bohuslav Křena, listopad 2009
**                                                 Karel Masařík, říjen 2013
**                                                 Radek Hranický 2014-2018
**
** S využitím dynamického přidělování paměti, implementujte NEREKURZIVNĚ
** následující operace nad binárním vyhledávacím stromem (předpona BT znamená
** Binary Tree a je u identifikátorů uvedena kvůli možné kolizi s ostatními
** příklady):
**
**     BTInit .......... inicializace stromu
**     BTInsert ........ nerekurzivní vložení nového uzlu do stromu
**     BTPreorder ...... nerekurzivní průchod typu pre-order
**     BTInorder ....... nerekurzivní průchod typu in-order
**     BTPostorder ..... nerekurzivní průchod typu post-order
**     BTDisposeTree ... zruš všechny uzly stromu
**
** U všech funkcí, které využívají některý z průchodů stromem, implementujte
** pomocnou funkci pro nalezení nejlevějšího uzlu v podstromu.
**
** Přesné definice typů naleznete v souboru c402.h. Uzel stromu je typu tBTNode,
** RootPtr na něj je typu tBTNodePtr. Jeden uzel obsahuje položku int Cont,
** která současně slouží jako užitečný obsah i jako vyhledávací klíč
** a RootPtre na levý a pravý podstrom (LPtr a RPtr).
**
** Příklad slouží zejména k procvičení nerekurzivních zápisů algoritmů
** nad stromy. Než začnete tento příklad řešit, prostudujte si důkladně
** principy převodu rekurzivních algoritmů na nerekurzivní. Programování
** je především inženýrská disciplína, kde opětné objevování Ameriky nemá
** místo. Pokud se Vám zdá, že by něco šlo zapsat optimálněji, promyslete
** si všechny detaily Vašeho řešení. Povšimněte si typického umístění akcí
** pro různé typy průchodů. Zamyslete se nad modifikací řešených algoritmů
** například pro výpočet počtu uzlů stromu, počtu listů stromu, výšky stromu
** nebo pro vytvoření zrcadlového obrazu stromu (pouze popřehazování RootPtrů
** bez vytváření nových uzlů a rušení starých).
**
** Při průchodech stromem použijte ke zpracování uzlu funkci BTWorkOut().
** Pro zjednodušení práce máte předem připraveny zásobníky pro hodnoty typu
** bool a tBTNodePtr. Pomocnou funkci BTWorkOut ani funkce pro práci
** s pomocnými zásobníky neupravujte
** Pozor! Je třeba správně rozlišovat, kdy použít dereferenční operátor *
** (typicky při modifikaci) a kdy budeme pracovat pouze se samotným RootPtrem
** (např. při vyhledávání). V tomto příkladu vám napoví prototypy funkcí.
** Pokud pracujeme s RootPtrem na RootPtr, použijeme dereferenci.
**/

#include "c402.h"
int solved;

void BTWorkOut (tBTNodePtr Ptr)		{
/*   ---------
** Pomocná funkce, kterou budete volat při průchodech stromem pro zpracování
** uzlu určeného RootPtrem Ptr. Tuto funkci neupravujte.
**/

	if (Ptr==NULL)
    printf("Chyba: Funkce BTWorkOut byla volána s NULL argumentem!\n");
  else
    printf("Výpis hodnoty daného uzlu> %d\n",Ptr->Cont);
}

/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu tBTNodePtr. Tyto funkce neupravujte.
**/

void SInitP (tStackP *S)
/*   ------
** Inicializace zásobníku.
**/
{
	S->top = 0;
}

void SPushP (tStackP *S, tBTNodePtr ptr)
/*   ------
** Vloží hodnotu na vrchol zásobníku.
**/
{
                 /* Při implementaci v poli může dojít k přetečení zásobníku. */
  if (S->top==MAXSTACK)
    printf("Chyba: Došlo k přetečení zásobníku s RootPtri!\n");
  else {
		S->top++;
		S->a[S->top]=ptr;
	}
}

tBTNodePtr STopPopP (tStackP *S)
/*         --------
** Odstraní prvek z vrcholu zásobníku a současně vrátí jeho hodnotu.
**/
{
                            /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (S->top==0)  {
		printf("Chyba: Došlo k podtečení zásobníku s RootPtri!\n");
		return(NULL);
	}
	else {
		return (S->a[S->top--]);
	}
}

bool SEmptyP (tStackP *S)
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
{
  return(S->top==0);
}

/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu bool. Tyto funkce neupravujte.
*/

void SInitB (tStackB *S) {
/*   ------
** Inicializace zásobníku.
**/

	S->top = 0;
}

void SPushB (tStackB *S,bool val) {
/*   ------
** Vloží hodnotu na vrchol zásobníku.
**/
                 /* Při implementaci v poli může dojít k přetečení zásobníku. */
	if (S->top==MAXSTACK)
		printf("Chyba: Došlo k přetečení zásobníku pro boolean!\n");
	else {
		S->top++;
		S->a[S->top]=val;
	}
}

bool STopPopB (tStackB *S) {
/*   --------
** Odstraní prvek z vrcholu zásobníku a současně vrátí jeho hodnotu.
**/
                            /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (S->top==0) {
		printf("Chyba: Došlo k podtečení zásobníku pro boolean!\n");
		return(NULL);
	}
	else {
		return(S->a[S->top--]);
	}
}

bool SEmptyB (tStackB *S) {
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
  return(S->top==0);
}

/* -------------------------------------------------------------------------- */
/*
** Následuje jádro domácí úlohy - funkce, které máte implementovat.
*/

void BTInit (tBTNodePtr *RootPtr)	{

	*RootPtr=NULL; //nastavime ukazatel na NULL
}

void BTInsert (tBTNodePtr *RootPtr, int Content) {
 
	while(42){ //Udelam si nekonecny cyklus ktery se pak zastavi jakmile vlozi novy uzel, nebo jestli uz existuje stejny
		if ((*RootPtr) == NULL){ //Kdyz bude na urcitem miste RootPtr NULL, vim ze tam muzu vlozit uzel
			(*RootPtr) = malloc(sizeof(struct tBTNode)); //Alokuji pamet pro novy uzel
			(*RootPtr)->Cont = Content; //Vlozim hodnotu (Content)
			(*RootPtr)->LPtr = NULL; // - } Vlevo ani v pravo nic nebude, protoze je novy, tudiz tam nic neni
			(*RootPtr)->RPtr = NULL; // - } 
			return; //Tady se mi cyklus ukonci, protoze jsem vlozil hodnotu a uz nic delat nechci
		}
		else if(Content == (*RootPtr)->Cont) return; //Kdyz uzel eistuje, tak jen ukoncim cyklus a nic se nedeje
		else RootPtr=(Content < (*RootPtr)->Cont)? &(*RootPtr)->LPtr : &(*RootPtr)->RPtr; 
		//Ternarnim operatorem si zjistim jestli je hodnota vetsi, nebo mensi a tim padem jdu doleva, nebo doprava
	}
}

/*                                  PREORDER                                  */

void Leftmost_Preorder (tBTNodePtr ptr, tStackP *Stack)	{

	/*Prevzato ze slidu z prednasky (prednaska 4) a ze zadani
	---------------------------------------------------------
	s1 -globální zásobník ukazatelů 
	while(ptr!= NULL)
	{
		Push(&s1,ptr);
		DInsertLast(l,ptr->data);
 		ptr=ptr->lPtr;
	}
	*/

	while(ptr != NULL){ //Prochazim leve uzly uplne dolu
		SPushP(Stack, ptr); //Ulozeni uzlu na zasobnik
		BTWorkOut(ptr); //Funkce na zpracovani uzlu, ze zadani
		ptr=ptr->LPtr; //Posouvma se doleva
	}
}

void BTPreorder (tBTNodePtr RootPtr)	{

	/*Prevzato ze slidu z prednasky (prednaska 4)
	---------------------------------------------
	DListInit(l);
	SInit(&s1);
	NejlevPre(ptr,l);
	while(!SEmpty(&s1))
	{
		ptr=Top(&s1);
		Pop(&s1);
		NejlevPre(ptr->rPtr,l);
	}
	*/

	tStackP zasobnik;
	SInitP(&zasobnik); //Inicializace zasobniku
	Leftmost_Preorder(RootPtr, &zasobnik);
	while(!SEmptyP(&zasobnik)){ //Dokud zasobnik neni prazdny, zpracovavame uzly a jejich prave podstromy
		RootPtr = STopPopP(&zasobnik); //V prednasce jsou 2 funkce, my mame jednu predefinovanou zastupujici obe
		Leftmost_Preorder(RootPtr->RPtr, &zasobnik);
	}
}

/*                                  INORDER                                   */

void Leftmost_Inorder(tBTNodePtr ptr, tStackP *Stack)		{

	/*Prevzato ze slidu z prednasky (prednaska 4)
	---------------------------------------------
	s1 -globální zásobník ukazatelů
	while(ptr!= NULL)
	{
		Push(&s1,ptr);
		ptr= ptr->lPtr;
	}
	*/

	while(ptr != NULL){ //Dokud levy uzel neni NULL, prochazim jej az dolu
		SPushP(Stack, ptr); //Ukladam ho na zasobnik
		ptr= ptr->LPtr; //Posouvam se uzly doleva
	}
}

void BTInorder (tBTNodePtr RootPtr)	{

	/*Prevzato ze slidu z prednasky (prednaska 4)
	---------------------------------------------
	DListInit(l);
	SInit(&s1);
	NejlevIn(ptr);
	while(!SEmpty(&s1))
	{
		ptr=Top(&s1);
		Pop(&s1);
		DInsertLast(l,ptr->data);  // změna od PreOrder
		NejlevIn(ptr->rPtr);
	}
	*/

	tStackP zasobnik;
	SInitP(&zasobnik); //Inicializace zasobniku
	Leftmost_Inorder(RootPtr, &zasobnik); 
	while(!SEmptyP(&zasobnik)){ //Dokud zasobnik neni prazdny, zpracovavame uzly a jejich prave podstromy
		RootPtr = STopPopP(&zasobnik);
		BTWorkOut(RootPtr); //Zmena od preOrder  (zpracovani uzlu)
		Leftmost_Inorder(RootPtr->RPtr, &zasobnik);
	}
}

/*                                 POSTORDER                                  */

void Leftmost_Postorder (tBTNodePtr ptr, tStackP *StackP, tStackB *StackB) {

	/* Prevzato ze slidu z prednasky (prednaska 4)
	----------------------------------------------
	s1 -globální zásobník ukazatelů
	sb1 –globální zásobník booleovských hodnot 
	while(ptr!= NULL)
	{
		Push(&s1,ptr);
		BPush(&sb1,true);
		ptr= ptr->lPtr;
	}
	*/

	while(ptr != NULL){ //Dokud ptr neni NULL, tak jdi doleva dolu
		SPushP(StackP, ptr); //Zpracovany uzel je ulozen na zasobnik
		SPushB(StackB, true); //Dale je tam ulozeno true, coz znamena ze uzel byl navstiven poprve
		ptr= ptr->LPtr; //Posun na dalsi levy podstrom
	}
}

void BTPostorder (tBTNodePtr RootPtr)	{

	/*Prevzato ze slidu z prednasky (prednaska 4)
	---------------------------------------------
	bool zleva;
	DListInit(l);
	SInit(&s1);
	SBInit(&sb1);
	NejlevPost(ptr);
	while(!SEmpty(&s1)) 
	{
		ptr= Top(&s1);
		zleva= BTop(&sb1);
		BPop(&sb1);
		if(zleva)
		{ //přichází zleva, půjde doprava
			BPush(&sb1,false);
			NejlevPost(ptr->rPtr);
			}
		else {  //zprava, odstraní a zpracuje uzel
			Pop(&s1);
			DInsertLast(l, ptr->data);
		} //if
	}  //while
	*/

	bool zleva;
	tStackP zasobnik;      
	SInitP(&zasobnik); //Inicializace zasobniku 
	tStackB bzasobnik;
	SInitB(&bzasobnik); //Inicializace bzasobniku 
	Leftmost_Postorder(RootPtr, &zasobnik, &bzasobnik);

	while(!SEmptyP(&zasobnik)){ //Dokud v zasobniku neco je tak pracujeme s uzlem ze zasobniku
			RootPtr = STopPopP(&zasobnik);
			zleva = STopPopB(&bzasobnik);
			if(zleva){ //projde jej zprava
				SPushB(&bzasobnik, false);
				SPushP(&zasobnik, RootPtr);
				Leftmost_Postorder(RootPtr->RPtr, &zasobnik, &bzasobnik);
			}
			else { //Kdyz uzel byl navstiven podruhe tak ho zpracujeme
				BTWorkOut(RootPtr);
			}
	}
}


void BTDisposeTree (tBTNodePtr *RootPtr)	{
/*   -------------
** Zruší všechny uzly stromu a korektně uvolní jimi zabranou paměť.
**
** Funkci implementujte nerekurzivně s využitím zásobníku RootPtrů.
**/

	/*Prevzato ze slidu z prednasky (prednaska 4)
	---------------------------------------------
	SInit(&s1);  //s1 -zásobník ukazatelů 
	do {
		if(ptr== NULL)
		{ //vezmu uzel ze zásobníku
			if(!SEmpty(&s1)) 
			{
				ptr= Top(&s1);
				Pop(&s1);
			}
		} else {
			if(ptr->rPtr!= NULL)
			{ // pravého dám do zásobníku
				Push(&s1,ptr->rPtr);
			}
			TNode*auxPtr= ptr;
			ptr=ptr->lPtr; //jdu doleva
			free(auxPtr);   //zrušímaktuální uzel
		} //else
	}while((ptr!= NULL)||(!SEmpty(&s1)));
	*/

	tStackP zasobnik;
	SInitP(&zasobnik);
	do {
		if(*RootPtr == NULL){
			if(!SEmptyP(&zasobnik))	*RootPtr = STopPopP(&zasobnik);
		}
		else {
			if((*RootPtr)->RPtr != NULL){
				SPushP(&zasobnik, (*RootPtr)->RPtr);
			}
			tBTNodePtr auxPtr = *RootPtr;
			*RootPtr=(*RootPtr)->LPtr;
			free(auxPtr);
		}
	}while((*RootPtr != NULL) || (!SEmptyP(&zasobnik)));
}

/* konec c402.c */