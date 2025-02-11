#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_YEAR_DURATION	10	// 기간
#define LINEAR_SEARCH 0
#define BINARY_SEARCH 1

// 구조체 선언
typedef struct {
	char	name[20];		// 이름
	char	sex;			// 성별 'M' or 'F'
	int		freq[MAX_YEAR_DURATION]; // 연도별 빈도
} tName;

typedef struct {
	int		len;		// 배열에 저장된 이름의 수
	int		capacity;	// 배열의 용량 (배열에 저장 가능한 이름의 수)
	tName	*data;		// 이름 배열의 포인터
} tNames;

////////////////////////////////////////////////////////////////////////////////
// 함수 원형 선언(declaration)

// 연도별 입력 파일을 읽어 이름 정보(이름, 성별, 빈도)를 이름 구조체에 저장
// 이미 구조체에 존재하는(저장된) 이름은 해당 연도의 빈도만 저장
// 새로 등장한 이름은 구조체에 추가
// 주의사항: 동일 이름이 남/여 각각 사용될 수 있으므로, 이름과 성별을 구별해야 함
// names->capacity는 2배씩 증가
// 선형탐색(linear search) 버전
void load_names_lsearch( FILE *fp, int year_index, tNames *names);

// 이진탐색(binary search) 버전 (bsearch 함수 이용)
void load_names_bsearch( FILE *fp, int year_index, tNames *names);

// 구조체 배열을 화면에 출력
void print_names( tNames *names, int num_year);

// qsort, bsearch를 위한 비교 함수
// 정렬 기준 : 이름(1순위), 성별(2순위)
int compare( const void *n1, const void *n2);

////////////////////////////////////////////////////////////////////////////////
// 함수 정의 (definition)

// 이름 구조체를 초기화
// len를 0으로, capacity를 1로 초기화
// return : 구조체 포인터
tNames *create_names(void)
{
	tNames *pnames = (tNames *)malloc( sizeof(tNames));
	
	pnames->len = 0;
	pnames->capacity = 1;
	pnames->data = (tName *)malloc(pnames->capacity * sizeof(tName));

	return pnames;
}

// 이름 구조체에 할당된 메모리를 해제
void destroy_names(tNames *pnames)
{
	free(pnames->data);
	pnames->len = 0;
	pnames->capacity = 0;

	free(pnames);
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	tNames *names;
	int option;
	
	FILE *fp;
	int num_year = 0;
	
	if (argc <= 2)
	{
		fprintf( stderr, "Usage: %s option FILE...\n\n", argv[0]);
		fprintf( stderr, "option\n\t-l\n\t\twith linear search\n\t-b\n\t\twith binary search\n");
		return 1;
	}
	
	if (strcmp( argv[1], "-l") == 0) option = LINEAR_SEARCH;
	else if (strcmp( argv[1], "-b") == 0) option = BINARY_SEARCH;
	else {
		fprintf( stderr, "unknown option : %s\n", argv[1]);
		return 1;
	}
	
	// 이름 구조체 초기화
	names = create_names();

	// 첫 연도 알아내기 "yob2009.txt" -> 2009
	int start_year = atoi( &argv[2][strlen(argv[2])-8]);
	
	for (int i = 2; i < argc; i++)
	{
		num_year++;
		fp = fopen( argv[i], "r");
		if( !fp) {
			fprintf( stderr, "cannot open file : %s\n", argv[i]);
			return 1;
		}

		int year = atoi( &argv[i][strlen(argv[i])-8]); // ex) "yob2009.txt" -> 2009
		
		fprintf( stderr, "Processing [%s]..\n", argv[i]);
		
		if (option == LINEAR_SEARCH)
		{
			// 연도별 입력 파일(이름 정보)을 구조체에 저장
			// 선형탐색 모드
			load_names_lsearch( fp, year-start_year, names);
		
		}
		else //(option == BINARY_SEARCH)
		{
			// 이진탐색 모드
			load_names_bsearch( fp, year-start_year, names);
			
			// 정렬 (이름순 (이름이 같은 경우 성별순))
			qsort( names->data, names->len, sizeof(tName), compare);
		}
		fclose( fp);

	}
	
	// 정렬 (이름순 (이름이 같은 경우 성별순))
	qsort(names->data, names->len, sizeof(tName), compare);
	
	// 이름 구조체를 화면에 출력
	print_names(names, num_year);

	// 이름 구조체 해제
	destroy_names( names);
	
	return 0;
}

void load_names_lsearch( FILE *fp, int year_index, tNames *names){

	if(year_index == 0){
		int zero_index = 0;
		char *buffer = (char *)calloc(30, sizeof(char));

		while(EOF != fscanf(fp, "%s", buffer)){
			names->len++;
			if(names->len > names->capacity){
				names->capacity = names->capacity*2;
				names->data = (tName *)realloc(names->data, names->capacity*sizeof(tName));
				memset(&names->data[(names->capacity)/2], 0, names->capacity/2 * sizeof(tName));
			}
			
			for(int i=0; i<strlen(buffer); i++){
				if(buffer[i] == ','){
					buffer[i] = ' ';
				}
			}
			memset(&names->data[zero_index].freq[year_index], 0, sizeof(names->data->freq)); //초기화
			sscanf(buffer, "%s %c %d", names->data[zero_index].name, &names->data[zero_index].sex, &names->data[zero_index].freq[year_index]);
			zero_index++;
		}
		free(buffer);
	}
	
	else{

		int first_count = 0; first_count = names->len;
      	int checking = 0; checking = names->len;
      	char *buffer = (char*)calloc(30, sizeof(char));
		char *buffer2 = (char*)calloc(20, sizeof(char));
		char buffersex = 0;
		int bufferfreq = 0;
      	int something = 0;

		while(EOF != fscanf(fp, "%s", buffer)){ 
			something = 0;

			for(int i=0; i<strlen(buffer); i++){
				if(buffer[i] == ','){
					buffer[i] = ' ';
				}
			}

			sscanf(buffer, "%s %c %d", buffer2, &buffersex, &bufferfreq);
			
			for(int search_index = 0; search_index < first_count; search_index++){
				
				if(strcmp(buffer2, names->data[search_index].name) == 0){
					
					if( (buffersex - names->data[search_index].sex) == 0 ){
						names->data[search_index].freq[year_index] = bufferfreq;
						something = -1;
						break;
					}
				}
			}

			if(something != -1){
				names->len++;
				if(names->len > names->capacity){
					names->capacity = names->capacity*2;
					names->data = (tName *)realloc(names->data, names->capacity*sizeof(tName));
					memset(&names->data[(names->capacity)/2], 0, names->capacity/2 * sizeof(tName));
				}
				strcpy(names->data[checking].name, buffer2);
				strcpy(&names->data[checking].sex, &buffersex);
				memset(&names->data[checking].freq[year_index], 0, sizeof(names->data->freq));
				names->data[checking].freq[year_index] = bufferfreq;
				checking++;
			}
		}
		free(buffer);
		free(buffer2);
   	}
	
};

void load_names_bsearch( FILE *fp, int year_index, tNames *names){

	if(year_index == 0){
		int zero_index = 0;
		char *buffer = (char *)calloc(30, sizeof(char));

		while(EOF != fscanf(fp, "%s", buffer)){
			names->len++;
			if(names->len > names->capacity){
				names->capacity = names->capacity*2;
				names->data = (tName *)realloc(names->data, names->capacity*sizeof(tName));
				memset(&names->data[(names->capacity)/2], 0, names->capacity/2 * sizeof(tName));
			}

			for(int i=0; i<strlen(buffer); i++){
				if(buffer[i] == ','){
					buffer[i] = ' ';
				}
			}
			memset(&names->data[zero_index].freq[year_index], 0, sizeof(names->data->freq)); //초기화
			sscanf(buffer, "%s %c %d", names->data[zero_index].name, &names->data[zero_index].sex, &names->data[zero_index].freq[year_index]);
			zero_index++;
		}
		free(buffer);
	}

	else{
		int first_cnt = names->len;
		int checking = names->len;	
      	char *buffer = (char*)calloc(30, sizeof(char));
		int bufferfreq = 0;

		tName *semibuffer = (tName *)calloc(1,sizeof(tName));

		int something = 0;

		while(EOF != fscanf(fp, "%s", buffer)){ 
			something = 0;

			for(int i=0; i<strlen(buffer); i++){
				if(buffer[i] == ','){
					buffer[i] = ' ';
				}
			}

			sscanf(buffer, "%s %c %d", semibuffer->name, &semibuffer->sex, &bufferfreq);

			tName *ptr = NULL;
			ptr = (tName *)bsearch(semibuffer, names->data, first_cnt, sizeof(tName), compare);
			
			if(ptr != NULL){
				ptr->freq[year_index] = bufferfreq;
			}
			else something = -1;

			if (something == -1){
				names->len++;
				if(names->len > names->capacity){
					names->capacity = names->capacity*2;
					names->data = (tName *)realloc(names->data, names->capacity*sizeof(tName));
					memset(names->data, 0, sizeof(names->capacity));
				}
				strcpy(names->data[checking].name, semibuffer->name);
				strcpy(&names->data[checking].sex, &semibuffer->sex);
				memset(&names->data[checking].freq[year_index], 0, sizeof(names->data->freq)); //초기화
				names->data[checking].freq[year_index] = bufferfreq;
				checking++;
			}

		}
		free(semibuffer);
		free(buffer);
	}
};

void print_names( tNames *names, int num_year){
	tNames *ans = names;

	for(int i=0; i<ans->len; i++){
    	printf("%s\t%c", ans->data[i].name, ans->data[i].sex);

    	for(int j=0; j<num_year; j++){
         	printf("\t%d", ans->data[i].freq[j]);
      	}
		printf("\n");
   }
};

int compare( const void *n1, const void *n2){
	
	tName *str1 = (tName *)n1;
	tName *str2 = (tName *)n2;

   	if (strcmp(str1->name, str2->name) == 0){
      	return (str1->sex - str2->sex);
  	}
   	else return strcmp(str1->name, str2->name);
};