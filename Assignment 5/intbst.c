#include <stdlib.h> // malloc, atoi, rand
#include <stdio.h>
#include <assert.h>
#include <time.h> // time

#define RANDOM_INPUT	1
#define FILE_INPUT		2

////////////////////////////////////////////////////////////////////////////////
// TREE type definition
typedef struct node
{
	int			data;
	struct node	*left;
	struct node	*right;
} NODE;

typedef struct
{
	NODE	*root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE *BST_Create( void);

/* Deletes all data in tree and recycles memory
*/
void BST_Destroy( TREE *pTree);

/* internal function (not mandatory)
*/
static void _destroy( NODE *root);

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int BST_Insert( TREE *pTree, int data);

/* internal function (not mandatory)
*/
static void _insert( NODE *root, NODE *newPtr);

NODE *_makeNode( int data);

/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/
int BST_Delete( TREE *pTree, int dltKey);

/* internal function
	success is 1 if deleted; 0 if not
	return	pointer to root
*/
static NODE *_delete( NODE *root, int dltKey, int *success);

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
int *BST_Retrieve( TREE *pTree, int key);

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve( NODE *root, int key);

/* prints tree using inorder traversal
*/
void BST_Traverse( TREE *pTree);
static void _traverse( NODE *root);

/* Print tree using inorder right-to-left traversal
*/
void printTree( TREE *pTree);
/* internal traversal function
*/
static void _inorder_print( NODE *root, int level);

/* 
	return 1 if the tree is empty; 0 if not
*/
int BST_Empty( TREE *pTree);

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	int mode; // input mode
	TREE *tree;
	int data;
	
	if (argc != 2)
	{
		fprintf( stderr, "usage: %s FILE or %s number\n", argv[0], argv[0]);
		return 1;
	}
	
	FILE *fp;
	
	if ((fp = fopen(argv[1], "rt")) == NULL)
	{
		mode = RANDOM_INPUT;
	}
	else mode = FILE_INPUT;
	
	// creates a null tree
	tree = BST_Create();
	
	if (!tree)
	{
		printf( "Cannot create a tree!\n");
		return 100;
	}
	
	if (mode == RANDOM_INPUT)
	{
		int numbers;
		numbers = atoi(argv[1]);
		assert( numbers > 0);

		fprintf( stdout, "Inserting: ");
		
		srand( time(NULL));
		for (int i = 0; i < numbers; i++)
		{
			data = rand() % (numbers*3) + 1; // random number (1 ~ numbers * 3)
			
			fprintf( stdout, "%d ", data);
			
			// insert function call
			int ret = BST_Insert( tree, data);
			if (!ret) break;
		}
	}
	else if (mode == FILE_INPUT)
	{
		fprintf( stdout, "Inserting: ");
		
		while (fscanf( fp, "%d", &data) != EOF)
		{
			fprintf( stdout, "%d ", data);
			
			// insert function call
			int ret = BST_Insert( tree, data);
			if (!ret) break;
		}
		fclose( fp);
	}
	
	fprintf( stdout, "\n");

	if (BST_Empty( tree))
	{
		fprintf( stdout, "Empty tree!\n");
		BST_Destroy( tree);
		return 0;
	}	

	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	BST_Traverse( tree);
	fprintf( stdout, "\n");
	
	// print tree with right-to-left inorder traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);

	while (1)
	{
		fprintf( stdout, "Input a number to delete: "); 
		int num;
		if (scanf( "%d", &num) == EOF) break;
		
		int ret = BST_Delete( tree, num);
		if (!ret)
		{
			fprintf( stdout, "%d not found\n", num);
			continue;
		}
		
		// print tree with right-to-left inorder traversal
		fprintf( stdout, "Tree representation:\n");
		printTree(tree);
		
		if (BST_Empty( tree))
		{
			fprintf( stdout, "Empty tree!\n");
			break;
		}
	}
	
	BST_Destroy( tree);

	return 0;
}

TREE *BST_Create( void){

	TREE *root = (TREE *)malloc( sizeof(TREE));
	root->root = NULL;
	return root;
};

void BST_Destroy( TREE *pTree){

	_destroy(pTree->root);
	free(pTree);
	//뭔가 많이 구현해야하나 ㅠ
	//메모리 누수 발생 시 traverse 등을 사용하며 메모리 재귀적으로 destroy 해줄 것.
};

static void _destroy( NODE *root){
	if(root != NULL){
		_destroy(root->left);
		_destroy(root->right);
		free(root);
	}
};

int BST_Insert( TREE *pTree, int data){

	NODE *tep = _makeNode(data);
	
	if(BST_Empty(pTree) == 1){
		pTree->root = tep;
		return 1;
	}

	if(pTree->root->data > data) {
		
		if(pTree->root->left == NULL){
			pTree->root->left = tep;
			return 1;
		}
		else _insert(pTree->root->left, tep); 
		return 1;
	}
	else if(pTree->root->data <= data){
		if(pTree->root->right == NULL){
			pTree->root->right = tep;
			return 1;
		}
		else _insert(pTree->root->right, tep);
		return 1;
	}
};

static void _insert( NODE *root, NODE *newPtr){
	
	if(root == NULL) {
		root = newPtr;
		root->data = newPtr->data;
		return;
	}

	if(root->data > newPtr->data) {
		if(root->left == NULL){
			root->left = newPtr;
			return;
		}
		else _insert(root->left, newPtr);
	}
	else if(root->data <= newPtr->data){
		if(root->right == NULL){
			root->right = newPtr;
			return;
		}
		else _insert(root->right, newPtr);
	}
	//같은 값이 나오면 어떡하지? 일단 왼쪽에 넣어두자
};

NODE *_makeNode( int data){
	NODE *make = (NODE *)malloc( sizeof(NODE));
	make->data = data;
	make->left = NULL;
	make->right = NULL;

	return make;
};

int BST_Delete( TREE *pTree, int dltKey){

   	if(BST_Empty(pTree) == 1) {
      return 0;
   	}

   int success = 0;
   pTree -> root = _delete( pTree->root, dltKey, &success);

   return success;
};

static NODE *_delete( NODE *root, int dltKey, int *success){
	
	NODE* d = NULL;    // 삭제할 노드     
    NODE* parent = NULL;    // 삭제할 노드의 부모 노드 
    NODE* successor = NULL;    // 삭제할 노드의 왼쪽 서브트리에서 가장 큰 노드 
	NODE* pS = NULL;
    NODE* child = NULL;        // 삭제할 노드의 자식 노드 
    
    d= root;
   	(*success)=0;
    while(d != NULL){    // 삭제할 노드 탐색 
        if(dltKey == d->data){
         	(*success)=1;
            break;
        }
        parent = d;
        if(dltKey < d->data){
            d = d->left;
        }else{
            d = d->right;
        }
    }
    
    if(d == NULL){
      	(*success)=0;
        return root;
    }
    
    if(d->left == NULL && d->right == NULL){

        if(parent != NULL){    

            if(parent->left == d){    
                (*success)=1;
            	parent->left = NULL;
            }
			else{
  
				(*success)=1;
				parent->right = NULL;
			}
        }
      	else{
			root=NULL;
			free(d);
			return root;
        } 
    }
   	else if(d->left != NULL && d->right != NULL){    

		pS = d;
        successor = d->right;
        
        while(successor->left != NULL){
			pS = successor;  
            successor = successor->left;
        }
        (*success)=1;
		d->data = successor->data;
		
		if(successor->right == NULL){
			if(d != pS) pS->left = NULL;
			else pS->right = NULL;
			free(successor);
			return root;
		}
		else{
			if(d != pS) pS->left = successor->right;
			else pS->right = successor->right;
			free(successor);
			return root;
		}
    }
   	else{  
		(*success)=1;
      	if(d->left != NULL){   
            child = d->left;
        }
      	else{  
            child = d->right;
        }
        
        if(parent != NULL){ 
            if(parent->left == d){   
                parent->left = child;
            }
        	else{  
                parent->right = child;
            }
        }
      	else{
			root = child;
			root->left = child->left;
			root->right = child->right;
			root->data = child->data;
			free(d);
			return child;
        }
    }
    free(d);
    return root; 
};


int BST_Empty( TREE *pTree){
	return(pTree->root == NULL);
};

void printTree( TREE *pTree){
	int level = 0;
	_inorder_print(pTree->root, level); 
};

//오->루트->왼
static void _inorder_print( NODE *root, int level){
	if(root != NULL){
		_inorder_print(root->right, level+1);
	
		for(int j = 0; j < level; j++) { printf("\t"); }
		
		printf("%d\n", root->data);

		_inorder_print(root->left, level+1);
	}
};


int *BST_Retrieve( TREE *pTree, int key){

	if(pTree->root->data == key) {
		return &(pTree->root->data);
	}

	if(pTree->root->data > key) {
		if(_retrieve(pTree->root->left, key)==NULL) return NULL;
		else return &((_retrieve(pTree->root->left, key))->data);
	}
		
	else if (pTree->root->data < key) {
		if(_retrieve(pTree->root->right, key) == NULL) return NULL;
		else return &((_retrieve(pTree->root->right, key))->data);
	}

};

static NODE *_retrieve( NODE *root, int key){
	if(root == NULL) return NULL;

	if(root != NULL){
		if(root->data == key) return root;
		
		else if(root->data > key){
			if(root->left == NULL) return NULL;
			else _retrieve(root->left, key);
		}

		else if(root->data < key){
			if(root->right == NULL) return NULL;
			else _retrieve(root->right, key);
		}
	}
};


void BST_Traverse( TREE *pTree){
	_traverse(pTree->root);
};

static void _traverse( NODE *root){
	if(root){
		if(root->left != NULL) _traverse(root->left);
		printf("%d ", root->data);
		if(root->right != NULL) _traverse(root->right);
	}
};