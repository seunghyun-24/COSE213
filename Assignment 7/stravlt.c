#define SHOW_STEP 0 // 제출시 0
#define BALANCING 1 // 제출시 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h> //strcmp, strdup

#define max(x, y)   (((x) > (y)) ? (x) : (y))

////////////////////////////////////////////////////////////////////////////////
// AVL_TREE type definition
typedef struct node
{
   char      *data;
   struct node   *left;
   struct node   *right;
   int         height;
} NODE;

typedef struct
{
   NODE   *root;
   int      count;  // number of nodes
} AVL_TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
   	return  head node pointer
    		NULL if overflow
*/
AVL_TREE *AVL_Create( void);

/* Deletes all data in tree and recycles memory
*/
void AVL_Destroy( AVL_TREE *pTree);
static void _destroy( NODE *root);

/* Inserts new data into the tree
   	return   1 success
    0 overflow
*/
int AVL_Insert( AVL_TREE *pTree, char *data);

/* internal function
   This function uses recursion to insert the new data into a leaf node
   return   pointer to new root
*/
static NODE *_insert( NODE *root, NODE *newPtr);

static NODE *_makeNode( char *data);

/* Retrieve tree for the node containing the requested key
   	return  address of data of the node containing the key
        	NULL not found
*/
char *AVL_Retrieve( AVL_TREE *pTree, char *key);

/* internal function
   	Retrieve node containing the requested key
   	return  address of the node containing the key
        	NULL not found
*/
static NODE *_retrieve( NODE *root, char *key);

/* Prints tree using inorder traversal
*/
void AVL_Traverse( AVL_TREE *pTree);
static void _traverse( NODE *root);

/* Prints tree using inorder right-to-left traversal
*/
void printTree( AVL_TREE *pTree);
/* internal traversal function
*/
static void _infix_print( NODE *root, int level);

/* internal function
   	return   height of the (sub)tree from the node (root)
*/
static int getHeight( NODE *root);

/* internal function
   Exchanges pointers to rotate the tree to the right
   updates heights of the nodes
   return   new root
*/
static NODE *rotateRight( NODE *root);

/* internal function
   Exchanges pointers to rotate the tree to the left
   updates heights of the nodes
   return   new root
*/
static NODE *rotateLeft( NODE *root);

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	AVL_TREE *tree;
	char str[1024];
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 0;
	}
   
   	// creates a null tree
   	tree = AVL_Create();
   
	if (!tree)
	{
		fprintf( stderr, "Cannot create tree!\n");
		return 100;
	} 

	FILE *fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "Cannot open file! [%s]\n", argv[1]);
		return 200;
	}

	while(fscanf( fp, "%s", str) != EOF)
	{

#if SHOW_STEP
    fprintf( stdout, "Insert %s>\n", str);
#endif      
    // insert function call
    AVL_Insert( tree, str);

#if SHOW_STEP
    fprintf( stdout, "Tree representation:\n");
    printTree( tree);
#endif
   }
   
   	fclose( fp);
   
#if SHOW_STEP
   	fprintf( stdout, "\n");

	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	AVL_Traverse( tree);
	fprintf( stdout, "\n");

	// print tree with right-to-left infix traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
#endif
	fprintf( stdout, "Height of tree: %d\n", tree->root->height);
	fprintf( stdout, "# of nodes: %d\n", tree->count);
	
	// retrieval
	char *key;
	fprintf( stdout, "Query: ");
	while( fscanf( stdin, "%s", str) != EOF)
	{
		key = AVL_Retrieve( tree, str);
		
		if (key) fprintf( stdout, "%s found!\n", key);
		else fprintf( stdout, "%s NOT found!\n", str);
		
		fprintf( stdout, "Query: ");
	}
   
	// destroy tree
	AVL_Destroy( tree);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
   return   head node pointer
         NULL if overflow
*/
AVL_TREE *AVL_Create( void){
	AVL_TREE *AVLT = (AVL_TREE *)malloc(sizeof(AVL_TREE));

	AVLT->count = 0;
	AVLT->root = NULL;

	return AVLT;
};

/* Deletes all data in tree and recycles memory
*/
void AVL_Destroy( AVL_TREE *pTree){

	_destroy(pTree->root);
	free(pTree);

};

static void _destroy( NODE *root){
   
    if(root == NULL) return;

	free(root->data);
	_destroy(root->left);
	_destroy(root->right);
	free(root);

};

/* Inserts new data into the tree
   return   1 success
         0 overflow
*/
int AVL_Insert( AVL_TREE *pTree, char *data){

   	NODE *new = _makeNode(data);

	if(pTree->count == 0){
		pTree->count++;
		pTree->root = new;

		return 1;
	}

	if( strcmp(pTree->root->data, data) > 0 ) {
		pTree->count++;
		if(pTree->root->left == NULL){
			pTree->root->left = new;

		}
		else pTree->root->left = _insert(pTree->root->left, new); 
	}
	else if( strcmp(pTree->root->data, data) <= 0){
		pTree->count++;
		if(pTree->root->right == NULL){
			pTree->root->right = new;

		}
		else pTree->root->right = _insert(pTree->root->right, new);
	}
	else return 0;

   pTree->root->height = getHeight(pTree->root);
	
	if(BALANCING==1){

		int lbal = 0;
		int rbal = 0;

		int reb = 0;
		int b = 0;
		int c = 0;

		if(pTree->root->left != NULL || pTree->root->right != NULL){
			
			if(pTree->root->left != NULL){
				b = getHeight(pTree->root->left);
				if(pTree->root->left->left != NULL && pTree->root->left->right != NULL) lbal = getHeight(pTree->root->left->left) - getHeight(pTree->root->left->right);
				else if(pTree->root->left->left != NULL) lbal = getHeight(pTree->root->left->left);
				else lbal = getHeight(pTree->root->left->right) * (-1);
			}

			if(pTree->root->right != NULL){
				c = getHeight(pTree->root->right);
				if(pTree->root->right->left != NULL && pTree->root->right->right != NULL) rbal = getHeight(pTree->root->right->left) - getHeight(pTree->root->right->right);
				else if(pTree->root->right->left != NULL) rbal = getHeight(pTree->root->right->left);
				else rbal = getHeight(pTree->root->right->right) * (-1);
			}

		}
		reb = b-c;

		//LL
		if(reb>1 && lbal >= 0){
			pTree->root = rotateRight(pTree->root);
		}
		//RR
		if(reb<-1 && rbal <= 0){
			pTree->root = rotateLeft(pTree->root);
	
		} 
		//LR
		if(reb>1 && lbal < 0){
			pTree->root->left = rotateLeft(pTree->root->left);
			pTree->root = rotateRight(pTree->root);
		}
		//RL
		if(reb<-1 && rbal > 0){
			pTree->root->right = rotateRight(pTree->root->right);
			pTree->root = rotateLeft(pTree->root);
		} 

		pTree->root->height = getHeight(pTree->root);
	}
	
   	return 1;
};

/* internal function
   This function uses recursion to insert the new data into a leaf node
   return   pointer to new root
*/
static NODE *_insert( NODE *root, NODE *newPtr){

	if(root == NULL) {
		root = newPtr;
		root->data = newPtr->data;
		//root->height = getHeight(root);
		//return root;
	}

	if( strcmp(root->data, newPtr->data) > 0) {
		if(root->left == NULL){
			root->left = newPtr;
			newPtr->height = getHeight(newPtr);
			root->height = getHeight(root);
			return root;
		}
		else root->left = _insert(root->left, newPtr);
	}
	else if( strcmp(root->data, newPtr->data) <= 0){
		if(root->right == NULL){
			root->right = newPtr;
			newPtr->height = getHeight(newPtr);
			root->height = getHeight(root);
			return root;
		}
		else root->right = _insert(root->right, newPtr);
	}
	else return root;

	newPtr->height = getHeight(newPtr);
	root->height = getHeight(root);

	if(BALANCING==1){

		int lbal = 0;
		int rbal = 0;

		int reb = 0;
		int b = 0;
		int c = 0;

		if(root->left != NULL || root->right != NULL){
			
			if(root->left != NULL){
				b = getHeight(root->left);
				if(root->left->left != NULL && root->left->right != NULL) lbal = getHeight(root->left->left) - getHeight(root->left->right);
				else if(root->left->left != NULL) lbal = getHeight(root->left->left);
				else lbal = getHeight(root->left->right) * (-1);
			}

			if(root->right != NULL){
				c = getHeight(root->right);
				if(root->right->left != NULL && root->right->right != NULL) rbal = getHeight(root->right->left) - getHeight(root->right->right);
				else if(root->right->left != NULL) rbal = getHeight(root->right->left);
				else rbal = getHeight(root->right->right) * (-1);
			}

		}

		reb = b-c;
		//LL
		if(reb>1 && lbal >= 0){
		//if(reb>1 && strcmp(data, pTree->root->left->data)<0) {
			root = rotateRight(root);
		}
		//RR
		if(reb<-1 && rbal <= 0){
		//if(reb<-1 && strcmp(data, pTree->root->right->data)>0){
			root = rotateLeft(root);
		} 
		//LR
		if(reb>1 && lbal < 0){
		//if(reb>1 && strcmp(data, pTree->root->left->data)>0){
			root->left = rotateLeft(root->left);
			root = rotateRight(root);
		}
		//RL
		if(reb<-1 && rbal > 0){
		//if(reb<-1 && strcmp(data, pTree->root->right->data)<0){
			root->right = rotateRight(root->right);
			root = rotateLeft(root);
		}
	}

	newPtr->height = getHeight(newPtr);
	root->height = getHeight(root);
	return root;
};

static NODE *_makeNode( char *data){

	NODE *make = (NODE *)malloc(sizeof(NODE));
	char *Newdata = strdup(data);
	//strcpy(Newdata, data);
	make->data = Newdata;
	make->left = NULL;
	make->right = NULL;
	make->height = 1;

	return make;
};

/* Retrieve tree for the node containing the requested key
   return   address of data of the node containing the key
         NULL not found
*/
char *AVL_Retrieve( AVL_TREE *pTree, char *key){

	if(strcmp(pTree->root->data, key)==0) {
		return (pTree->root->data);
	}

	if(strcmp(pTree->root->data, key) > 0) {
		if(_retrieve(pTree->root->left, key)==NULL) return NULL;
		else return ((_retrieve(pTree->root->left, key))->data);
	}
		
	else if (strcmp(pTree->root->data, key) < 0) {
		if(_retrieve(pTree->root->right, key) == NULL) return NULL;
		else return ((_retrieve(pTree->root->right, key))->data);
	}

};

/* internal function
   Retrieve node containing the requested key
   return   address of the node containing the key
         NULL not found
*/
static NODE *_retrieve( NODE *root, char *key){
   
	if(root == NULL) return NULL;

	if(root != NULL){
		if(strcmp(root->data, key)==0) return root;
		
		else if(strcmp(root->data, key) > 0){
			_retrieve(root->left, key);
		}

		else if(strcmp(root->data, key) < 0){
			_retrieve(root->right, key);
		}
	}
};

/* Prints tree using inorder traversal
*/
void AVL_Traverse( AVL_TREE *pTree){
   	_traverse(pTree->root);
};

static void _traverse( NODE *root){
	if(root){
		if(root->left != NULL) _traverse(root->left);
		printf("%s ", root->data);
		if(root->right != NULL) _traverse(root->right);
	}
};

/* Prints tree using inorder right-to-left traversal
*/
void printTree( AVL_TREE *pTree){
   
	int level = 0;
	_infix_print(pTree->root, level); 
};

/* internal traversal function
*/
static void _infix_print( NODE *root, int level){
	if(root != NULL){
		_infix_print(root->right, level+1);
	
		for(int j = 0; j < level; j++) { printf("\t"); }
		
		printf("%s\n", root->data);

		_infix_print(root->left, level+1);
	}
};

/* internal function
   return   height of the (sub)tree from the node (root)
*/
static int getHeight( NODE *root){
   
	if(root==NULL) return 0;
	
	if(root->left==NULL && root->right==NULL) return 1;
	else if(root->right==NULL) return 1+getHeight(root->left);//->height;
	else if (root->left==NULL) return 1+getHeight(root->right); //->height+1;
	
	return (1+max(root->left->height, root->right->height));
};

/* internal function
   Exchanges pointers to rotate the tree to the right
   updates heights of the nodes
   return   new root
*/
static NODE *rotateRight( NODE *root){

	NODE *a = root->left;

	NODE *b = a->right;

	a->right = root;

	root->left = b;

	root->height = getHeight(root);
	a->height = getHeight(a);
	
	return a;
}

/* internal function
   Exchanges pointers to rotate the tree to the left
   updates heights of the nodes
   return   new root
*/
static NODE *rotateLeft( NODE *root){

	NODE *a = root->right;
	NODE *b = a->left;

	a->left = root;
	root->right = b;

	root->height = getHeight(root);
	a->height = getHeight(a);

	return a;

};