#include <stdio.h>
#include <stdlib.h>	// malloc
#include <string.h>	// strdup
#include <ctype.h>	// isupper, tolower

#define MAX_DEGREE	27 // 'a' ~ 'z' and EOW
#define EOW			'$' // end of word

// used in the following functions: trieInsert, trieSearch, triePrefixList
#define getIndex(x)		(((x) == EOW) ? MAX_DEGREE-1 : ((x) - 'a'))

// TRIE type definition
typedef struct trieNode {
	int 			index; // -1 (non-word), 0, 1, 2, ...
	struct trieNode	*subtrees[MAX_DEGREE];
} TRIE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a trie node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
TRIE *trieCreateNode(void);

/* Deletes all data in trie and recycles memory
*/
void trieDestroy( TRIE *root);

/* Inserts new entry into the trie
	return	1 success
			0 failure
*/
// 주의! 엔트리를 중복 삽입하지 않도록 체크해야 함
// 대소문자를 소문자로 통일하여 삽입
// 영문자와 EOW 외 문자를 포함하는 문자열은 삽입하지 않음
int trieInsert( TRIE *root, char *str, int dic_index);

/* Retrieve trie for the requested key
	return	index in dictionary (trie) if key found
			-1 key not found
*/
int trieSearch( TRIE *root, char *str);

/* prints all entries in trie using preorder traversal
*/
void trieList( TRIE *root, char *dic[]);

/* prints all entries starting with str (as prefix) in trie
	ex) "abb" -> "abbas", "abbasid", "abbess", ...
	this function uses trieList function
*/
void triePrefixList( TRIE *root, char *str, char *dic[]);

/* makes permuterms for given str
	ex) "abc" -> "abc$", "bc$a", "c$ab", "$abc"
	return	number of permuterms
*/
int make_permuterms( char *str, char *permuterms[]);

/* recycles memory for permuterms
*/
void clear_permuterms( char *permuterms[], int size);

/* wildcard search
	ex) "ab*", "*ab", "a*b", "*ab*"
	this function uses triePrefixList function
*/
void trieSearchWildcard( TRIE *root, char *str, char *dic[]);

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	TRIE *trie = NULL;
	TRIE *permute_trie = NULL;
	char *dic[100000] = {0, };

	int ret = 0;
	char str[100] = {0, };
	FILE *fp;
	char *permuterms[100] = { 0, };
	int num_p = 0;
	int index = 0;
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "File open error: %s\n", argv[1]);
		return 1;
	}
	
	trie = trieCreateNode(); // original trie
	permute_trie = trieCreateNode(); // trie for permuterm index
	
	while (fscanf( fp, "%s", str) != EOF)
	{	
		ret = 0;
		ret = trieInsert( trie, str, index);
		
		if (ret)
		{	
			num_p = 0;
			num_p = make_permuterms( str, permuterms);
			
			for (int i = 0; i < num_p; i++){
				trieInsert( permute_trie, permuterms[i], index);
			}
			
			//for (int i = 0; i < num_p; i++){
			//	printf("%s ", permuterms[i]);
			//}

			clear_permuterms( permuterms, num_p);
			
			dic[index++] = strdup( str);
		}
	}
	
	fclose( fp);
	
	printf( "\nQuery: ");
	while (fscanf( stdin, "%s", str) != EOF)
	{
		// wildcard search term
		if (strchr( str, '*')) 
		{
			trieSearchWildcard( permute_trie, str, dic);
		}
		// keyword search
		else 
		{
			ret = trieSearch( trie, str);
			if (ret == -1) printf( "[%s] not found!\n", str);
			else printf( "[%s] found!\n", dic[ret]);
		}
		printf( "\nQuery: ");
	}

	for (int i = 0; i < index; i++)
		free( dic[i]);
	
	trieDestroy( trie);
	trieDestroy( permute_trie);
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
/* Allocates dynamic memory for a trie node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
TRIE *trieCreateNode(void){
	TRIE *trie = (TRIE *)malloc(sizeof(TRIE));
	memset(trie, 0, sizeof(TRIE));
	if(trie == NULL) return NULL;

	trie->index = -1;
	memset(trie->subtrees, 0, sizeof(trie->subtrees));
	return trie;
};

/* Deletes all data in trie and recycles memory
*/
void trieDestroy( TRIE *root){
	
	if(root){
		for(int i=0; i<27; i++){
			if(root->subtrees[i]) {
				trieDestroy(root->subtrees[i]);
				//free(root->subtrees[i]);
			}
		}
		free(root);
	}

};

/* Inserts new entry into the trie
	return	1 success
			0 failure
*/
// 주의! 엔트리를 중복 삽입하지 않도록 체크해야 함
// 대소문자를 소문자로 통일하여 삽입
// 영문자와 EOW 외 문자를 포함하는 문자열은 삽입하지 않음
int trieInsert( TRIE *root, char *str, int dic_index){
	
	int level = 0;
	int length = 0;
	length = strlen(str);
	int index = 0;
	
	TRIE *node = root;

	for(level = 0; level<length; level++){
		
		if(isupper(str[level])) str[level] = tolower(str[level]);

		index = getIndex(str[level]);

		if(index<0 || index>26 ) { 
			return 0;
		}
		
		if(!node->subtrees[index]) {
			node->subtrees[index] = trieCreateNode();
		}

		node = node->subtrees[index];
	}

	if(node->index != -1) return 0;
	
	node->index = dic_index;

	return 1;

};

/* Retrieve trie for the requested key
	return	index in dictionary (trie) if key found
			-1 key not found
*/
int trieSearch( TRIE *root, char *str){
	
	int level = 0;
    int length = strlen(str);
    int index;

    TRIE *node = root;
 
    for (level = 0; level < length; level++)
    {
        index = getIndex(str[level]);
 
        if (!node->subtrees[index]) return -1;

       	node = node->subtrees[index];
    }

    return (node->index);
};

/* prints all entries in trie using preorder traversal
*/
void trieList( TRIE *root, char *dic[]){
	
	if(root){
		if(root->index != -1) printf("%s\n", dic[root->index]);
		for(int i=0; i<27; i++){
			if(root->subtrees[i]) trieList(root->subtrees[i], dic);

		}
	}

};

/* prints all entries starting with str (as prefix) in trie
	ex) "abb" -> "abbas", "abbasid", "abbess", ...
	this function uses trieList function
*/
void triePrefixList( TRIE *root, char *str, char *dic[]){

	int level = 0;
    int index = 0;

    TRIE *node = root;

    for (level = 0; level < strlen(str); level++)
    {
        index = getIndex(str[level]);

		if (!node->subtrees[index]) return;

       	node = (node->subtrees[index]);
    }

	trieList(node, dic);

};

/* makes permuterms for given str
	ex) "abc" -> "abc$", "bc$a", "c$ab", "$abc"
	return	number of permuterms
*/
int make_permuterms( char *str, char *permuterms[]){

	int len = 0;
	len = strlen(str);

	char *new = (char *)calloc((len+2), sizeof(char));
	strcpy(new, str);

	new[len]= '$';
	new[len+1] = '\0';

	for(int j=0; j<= len; j++){

		permuterms[j] = strdup(new);

		char save = new[0];

		for(int k=0; k<len; k++){
			new[k] = new[k+1];
		}
		new[len] = save;
	}

	free(new);

	return (len+1);
};

/* recycles memory for permuterms
*/
void clear_permuterms( char *permuterms[], int size){

	for(int i=0; i<size; i++){
		free(permuterms[i]);
	}
	
};

/* wildcard search
	ex) "ab*", "*ab", "a*b", "*ab*"
	this function uses triePrefixList function
*/
void trieSearchWildcard( TRIE *root, char *str, char *dic[]){

	char *lazenca = (char*)calloc((strlen(str))+1, sizeof(char));
	strcpy(lazenca, str);
	
	int a = 0;
	char *p = NULL;
	char *ss = NULL;
	for(p=str; ss=strtok(p,"*"); p=NULL){ a++; }

	if(a==1){
		if(str[0] == '*'){
			if(lazenca[strlen(lazenca)-1] == '*'){ // *ab*

				for(int i=0; i<strlen(str); i++){
					str[i] = str[i+1];
				}
				triePrefixList(root, str, dic);
			}
			
			else{ //*ab

				for(int i=0; i<strlen(lazenca)+1; i++){
					lazenca[i] = lazenca[i+1];
				}
				lazenca[strlen(lazenca)] = '$';

				triePrefixList(root, lazenca, dic);
				
			}
		}
		else{ //ab*

			char *lazenca2 = (char *)calloc((strlen(lazenca))+1, sizeof(char));
			strcpy(lazenca2, lazenca);

			for(int i=0; i<strlen(lazenca)-1; i++){
				lazenca2[i+1] = lazenca[i];
			}
			lazenca2[0] = '$';

			triePrefixList(root, lazenca2, dic);
			free(lazenca2);
		}
	}
	else { //a*b

		for(int i=0; i<strlen(lazenca); i++){
			if(lazenca[i] == '*') lazenca[i] = '$';
		}

		for(int j=0; j<strlen(lazenca)-strlen(str)-1; j++){
			lazenca[j] = lazenca[j+strlen(str)+1];
		}
		lazenca[strlen(lazenca)-strlen(str)-1] = '$';
		for(int k=0; k<strlen(str); k++){
			lazenca[strlen(lazenca)-strlen(str)+k] = str[k]; 
		}

		triePrefixList(root, lazenca, dic);
	}

	free(lazenca);
};