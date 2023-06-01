#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_YEAR_DURATION	10	// 기간

// 구조체 선언
typedef struct {
	char	name[20];		// 이름
	char	sex;			// 성별 M or F
	int		freq[MAX_YEAR_DURATION]; // 연도별 빈도
} tName;

typedef struct {
	int		len;		// 배열에 저장된 이름의 수
	int		capacity;	// 배열의 용량 (배열에 저장 가능한 이름의 수)
	tName	*data;		// 이름 배열의 포인터
} tNames;

// 함수 원형 선언

// 연도별 입력 파일을 읽어 이름 정보(연도, 이름, 성별, 빈도)를 이름 구조체에 저장
// 이미 구조체에 존재하는(저장된) 이름은 해당 연도의 빈도만 저장
// 새로 등장한 이름은 구조체에 추가
// 주의사항: 동일 이름이 남/여 각각 사용될 수 있으므로, 이름과 성별을 구별해야 함
// 주의사항: 정렬 리스트(ordered list)를 유지해야 함 (qsort 함수 사용하지 않음)
// 이미 등장한 이름인지 검사하기 위해 bsearch 함수를 사용
// 새로운 이름을 저장할 메모리 공간을 확보하기 위해 memmove 함수를 이용하여 메모리에 저장된 내용을 복사
// names->capacity는 2배씩 증가

void load_names( FILE *fp, int start_year, tNames *names);

// 구조체 배열을 화면에 출력
void print_names( tNames *names, int num_year);

// bsearch를 위한 비교 함수
int compare( const void *n1, const void *n2);

// 이진탐색 함수
// return value: key가 발견되는 경우, 배열의 인덱스
//				 key가 발견되지 않는 경우, key가 삽입되어야 할 배열의 인덱스
int binary_search( const void *key, const void *base, size_t nmemb, size_t size, int (*compare)(const void *, const void *));

// 함수 정의

// 이름 구조체 초기화
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
	FILE *fp;
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n\n", argv[0]);
		return 1;
	}

	// 이름 구조체 초기화
	names = create_names();
	
	fp = fopen( argv[1], "r");
	if (!fp)
	{
		fprintf( stderr, "cannot open file : %s\n", argv[1]);
		return 1;
	}

	fprintf( stderr, "Processing [%s]..\n", argv[1]);
		
	// 연도별 입력 파일(이름 정보)을 구조체에 저장
	load_names( fp, 2009, names);
	
	fclose( fp);
	
	// 이름 구조체를 화면에 출력
	print_names( names, MAX_YEAR_DURATION);

	// 이름 구조체 해제
	destroy_names( names);
	
	return 0;
}

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

int binary_search( const void *key, const void *base, size_t nmemb, size_t size, int (*compare)(const void *, const void *)){
	int index_left = 0;
	int index_right = nmemb; //인덱스닌까,,
	int index_mid = 0; // 초기화를 위함
	int cpare = 0;

	while(index_left < index_right){
		index_mid = (index_left + index_right)/2;

		cpare = compare(base+index_mid*sizeof(tName), key);
		
		if(cpare == 0) return index_mid;

		if(cpare<0) index_left = index_mid+1;
		else index_right = index_mid;
	}

	if(cpare>=0) return index_mid;
	else return index_mid+1;
};

void load_names( FILE *fp, int start_year, tNames *names){
	int cnt = 0; // 얼마나 저장되어있는가?
	char *buffer = (char*)calloc(40, sizeof(char)); //임시로 받기
	int bufferyear = 0; //임시로 받기
	int bufferfreq = 0; //임시로 받기
	tName *ptr = NULL; //bsearch에 이용할 포인터
	tName *semibuffer = (tName *)calloc(1,sizeof(tName)); //임시로 받기
	int bindex = 0;

	while( NULL != fgets(buffer, 40, fp) ){

		sscanf(buffer,"%d   %s   %c   %d", &bufferyear, semibuffer->name, &semibuffer->sex, &bufferfreq);

		ptr = (tName *)bsearch(semibuffer, names->data, names->len, sizeof(tName), compare);

		if(ptr != NULL){
			bindex = binary_search(semibuffer, names->data, names->len, sizeof(tName), compare);
			names->data[bindex].freq[bufferyear-start_year] = bufferfreq;
		}
		else {
			names->len++;
			if(names->len > names->capacity){ //capacity 늘리고 초기화하기
				names->capacity = names->capacity*2;
				names->data = (tName *)realloc(names->data, names->capacity*sizeof(tName));
				memset(&names->data[(names->capacity)/2], 0, names->capacity/2 * sizeof(tName));
			}
			
			bindex = binary_search(semibuffer, names->data, names->len-1, sizeof(tName), compare);
			//bindex는 딱 저장할 바로 '그' 위치여야 한다.
			
			if(bindex != names->len -1 ){ //마지막에 저장하는 게 아니면 이동시키기 (memmove)
				memmove(&names->data[bindex+1], &names->data[bindex], ((names->len-1)-bindex)*sizeof(tName));
				memset(&names->data[bindex], 0, sizeof(tName));
			}
			
			strcpy(names->data[bindex].name, semibuffer->name);
			strcpy(&names->data[bindex].sex, &semibuffer->sex);
			memset(&names->data[bindex].freq, 0, sizeof(names->data->freq)); //초기화
			names->data[bindex].freq[bufferyear-start_year] = bufferfreq;
			//buffer랑 semibuffer을 비워줘야할까?
		}
	}

	free(semibuffer);
	free(buffer);
};