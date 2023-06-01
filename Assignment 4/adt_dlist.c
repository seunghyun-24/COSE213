#include <stdlib.h> // malloc

#include "adt_dlist.h"

/* internal insert function
	inserts data into a new node
	return	1 if successful
			0 if memory overflow
*/
static int _insert( LIST *pList, NODE *pPre, void *dataInPtr);

/* internal delete function
	deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, void **dataOutPtr);

/* internal search function
	searches list and passes back address of node
	containing target and its logical predecessor
	return	1 found
			0 not found
*/
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, void *pArgu);


/////////////////////////////////////////////////////////////////////////////
LIST *createList( int (*compare)(const void *, const void *)){
	LIST *listed = (LIST *)malloc(sizeof(LIST));
	listed->head = NULL;
	listed->rear = NULL;
	listed->count = 0;
	listed->compare = (*compare);
	//((*listed->compare)( listed->head, listed->rear));
	if(!listed) return NULL;
	
	return listed;
};

void destroyList( LIST *pList, void (*callback)(void *)){
	NODE* cur = pList->head;
   	NODE* tmp;
   	while(cur != NULL){
      	callback(cur->dataPtr); //destroyName
      	tmp = cur;
      	cur=cur->rlink;
	  	free(tmp);
   	}
   	free(cur);
   	free(pList);
};

int addNode( LIST *pList, void *dataInPtr, void (*callback)(const void *, const void *)){
	NODE *ling, *ding;
   	
	if(_search(pList, &ling, &ding, dataInPtr) == 1){
      	callback(ding->dataPtr, dataInPtr); //increase_freq
      	return 2;
   	}
   	else {
      	if(_insert(pList, ling, dataInPtr)==1) {  
        	return 1;
      	}
      	else return 0;
   	}
};

int removeNode( LIST *pList, void *keyPtr, void **dataOutPtr){
	NODE *ling, *ding;
   
	if(_search(pList, &ling, &ding, keyPtr) == 1){
      	_delete(pList, ling, ding, dataOutPtr);
      	pList->count--;
	  	return 1;
  	}
   	else return 0;
};

int searchList( LIST *pList, void *pArgu, void **dataOutPtr){
	NODE *ling, *ding;
   	if(_search(pList, &ling, &ding, pArgu) == 1){
    	*dataOutPtr = ding->dataPtr;
    	return 1;
   	}
   	else return 0;
};

int countList( LIST *pList){
	return pList->count;
};

int emptyList( LIST *pList){
	if (countList(pList) == 0) return 1;
   	else return 0;
};

void traverseList( LIST *pList, void (*callback)(const void *)){
	NODE *travel;
   	travel = pList->head;
   	while(travel != NULL){
      	callback(travel->dataPtr);
      	travel = travel->rlink;
   	}
   free(travel);
};

void traverseListR( LIST *pList, void (*callback)(const void *)){
	NODE *travel;
   	travel = pList->rear;
   	while(travel != NULL){
    	callback(travel->dataPtr);
    	travel = travel->llink;
   	}
   	free(travel);
};

static int _insert( LIST *pList, NODE *pPre, void *dataInPtr){
   	NODE *ss = (NODE *)malloc(sizeof(NODE));
   	if(!ss) return 0;

   	ss->dataPtr = dataInPtr;
   
	if (pList->count==0){
		pList->count++;
		ss->llink = NULL;
		ss->rlink = NULL;
		pList->head = ss;
		pList->rear = ss;
		return 1;
	}
   
   	if(pPre == NULL){
		pList->count++;
		ss->rlink = pList->head;
		ss->llink = NULL;
		pList->head->llink = ss;
		pList->head = ss;
		return 1;
   	}
   	else if(pPre->rlink==NULL){
		pList->count++;
		ss->rlink = NULL;
		pPre->rlink = ss;
		ss->llink = pPre;
		pList->rear = ss;
		return 1;
   	}
   	else if(pPre->rlink != NULL && pPre != NULL){
		pList->count++;
		ss->rlink = pPre->rlink;
		pPre->rlink = ss;
		ss->llink = pPre;
		if(ss->rlink != NULL){
			ss->rlink->llink = ss;
		}
		return 1;
	}
   	else return 0;
};

static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, void **dataOutPtr){
   	*dataOutPtr = pLoc->dataPtr;
	
	if(pList->head == NULL || pLoc == NULL) return;
	
	if(pList->head == pLoc) {
		pList->head = pLoc->rlink;
	}
	if(pLoc->rlink != NULL){
		pLoc->rlink->llink = pLoc->llink;
	}
	if(pLoc->llink != NULL){
		pPre->rlink = pLoc->rlink;
	}
	free(pLoc);
};

static int _search( LIST *pList, NODE **pPre, NODE **pLoc, void *pArgu){
   int check = 0;
   
    if(pList->count == 0) return 0;

   	(*pPre)=NULL;
    (*pLoc)=pList->head;
   
    while(*pLoc != NULL){

		//((*pList->compare)( ss->dataPtr, dataInPtr ));
        check = (*pList->compare)((*pLoc)->dataPtr, pArgu); //cmpName
        if(check==0) return 1;
        else if(check>0) return 0;
        else {
			*pPre = *pLoc;
			*pLoc = (*pLoc)->rlink;
        }
    }
    return 0;
};
