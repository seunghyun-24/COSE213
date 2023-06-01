# COSE213

# 자료구조

---
## Assignment 1 : Dynamic array 구현 (동적배열)
```
주어진 소스코드(name.c)를 완성한다.
- 소스파일에는 함수의 원형(prototype)이 선언되어 있으며, 함수를 직접 정의해야 함
- main 함수에 있는 주석을 제거하여 실행 파일(name)과 같이 실행되도록 수정해야 함
- valgrind를 통해 메모리 누수(memory leak) 현상을 제거해야 함
- 과제 설명에 대한 강의는 "강의자료 -> 2주차"에 있음
- 실행방법: 
$ ./name -l *.txt > 출력파일명
$ ./name -b *.txt > 출력파일명
- 제출파일: name.c (파일명을 수정하지 말 것!)
```
--- 
## Assignment 2 : General list 구현 (동적배열+정렬된(ordered) 선형리스트)
```
주어진 소스파일(name2.c)을 완성한다.
- 소스파일에는 함수의 원형(prototype)이 선언되어 있으며, 함수를 직접 정의해야 함
- main 함수에 있는 주석을 제거하여 실행 파일(name2)과 같이 실행되도록 수정해야 함 (프로그램의 결과는 과제 1과 동일함)
- valgrind를 통해 메모리 누수(memory leak) 현상을 제거해야 함
- C언어에서 제공하는 qsort 함수를 사용하지 말 것
- name2.txt를 변형하지 말 것
- 과제 설명에 대한 강의는 "강의자료 -> 5주차"에 있음
- 실행방법: $ ./name2 name2.txt > 출력파일명
- 제출파일: name2.c (파일명을 수정하지 말 것!)
```
---
## Assignment 3 : General list 구현 (Doubly linked list + 정렬된(ordered) 선형리스트)
```
주어진 소스파일(name_dlist.c)을 완성한다.
- 소스파일에는 함수의 원형(prototype)만 선언되어 있으며, 함수를 직접 정의해야 함
- main 함수에 있는 주석을 제거하여 실행 파일(name_dlist)과 같이 실행되도록 수정해야 함
- valgrind를 통해 메모리 누수(memory leak) 현상을 제거해야 함
- 과제 설명에 대한 강의는 "강의자료 -> 6주차"에 있음
- 실행 방법: $ ./name_dlist 파일명
  예) $ ./name_dlist name3.txt
- 실행 파일은 주어진 입력 파일(name3.txt)에 포함된 이름(name)들을 doubly linked list에 순서를 유지하여 삽입한다. (이름순, 성별순(F, M))
  만약 이미 리스트에 저장된 이름인 경우 빈도(freq)를 증가시킨다.
  
  파일 입력이 완료되면, 사용자로부터 메뉴를 선택받는다.
  
  Q : 실행 종료 (프로그램 종료 전에 리스트를 위해 할당한 메모리를 모두 해제해야 함)
  P : 화면에 (순방향으로) list를 출력한다. (이름(name), 성별(sex), 빈도(freq)를 탭(tab)문자로 구분하여 출력)
  B : 화면에 역방향으로 list를 출력한다. 
  S : 사용자가 입력한 문자열(이름/성별)을 리스트에서 찾는다.
  D : 사용자가 입력한 문자열(이름/성별)을 리스트에서 삭제한다.
  C : 리스트에 포함된 이름(노드)의 수를 출력한다.
- 제출파일: name_dlist.c (파일명을 수정하지 말 것!)
```
---
## Assignment 4 : ADT General list 구현 (Doubly linked list + 정렬된(ordered) 선형리스트)
```
주어진 소스파일(name.c와 adt_dlist.c)을 완성한다.
- 소스파일에는 함수의 원형(prototype)만 선언되어 있으며, 함수를 직접 정의해야 함
- main 함수에 있는 주석을 제거하여 실행 파일(name_dlist)과 같이 실행되도록 수정해야 함
- valgrind를 통해 메모리 누수(memory leak) 현상을 제거해야 함
- 과제 설명에 대한 강의는 "강의자료 -> 8주차"에 있음
- 실행 방법: $ ./name_dlist 파일명
  예) $ ./name_dlist name3.txt
- 실행 파일은 주어진 입력 파일(name3.txt)에 포함된 이름(name)들을 doubly linked list에 순서를 유지하여 삽입한다. (이름순, 성별순(F, M))
  만약 이미 리스트에 저장된 이름인 경우 빈도(freq)를 증가시킨다.
  
  파일 입력이 완료되면, 사용자로부터 메뉴를 선택받는다.
  
  Q : 실행 종료 (프로그램 종료 전에 리스트를 위해 할당한 메모리를 모두 해제해야 함)
  P : 화면에 (순방향으로) list를 출력한다. (이름(name), 성별(sex), 빈도(freq)를 탭(tab)문자로 구분하여 출력)
  B : 화면에 역방향으로 list를 출력한다. 
  S : 사용자가 입력한 문자열(이름/성별)을 리스트에서 찾는다.
  D : 사용자가 입력한 문자열(이름/성별)을 리스트에서 삭제한다.
  C : 리스트에 포함된 이름(노드)의 수를 출력한다.
- 제출파일: adt_dlist.c, adt_dlist.h, name.c, Makefile (파일명을 수정하지 말 것!)
```
---
## Assignment 5 : binary search tree 구현 (정수자료형)
```
주어진 소스파일(intbst.c)을 완성한다.
- 소스파일에는 함수의 원형(prototype)만 선언되어 있으며, 함수를 직접 정의해야 함
- main 함수에 있는 주석을 제거하여 실행 파일(intbst)과 같이 실행되도록 수정해야 함
- 단, 왼쪽과 오른쪽 서브트리를 모두 가지고 있는 노드를 삭제하는 경우는 오른쪽 서브트리의 가장 작은 값을 가진 노드가 삭제될 노드의 자리로 위치하도록 해야 함
- valgrind를 통해 메모리 누수(memory leak) 현상을 제거해야 함
- 과제 설명에 대한 강의는 "강의자료 -> 10주차"에 있음
- 실행 방법: ./intbst 파일명 | ./intbst number
  예) $ ./intbst numbers.txt
      $ ./intbst 10
- 입력 파일은 숫자들의 리스트로 이루어진 텍스트 파일이며, 순서대로 BST에 삽입되어야 함
- 제출파일: intbst.c (파일명을 수정하지 말 것!)
```
---
## Assignment 6 : Heap 구현 (추상자료형)
```
주어진 소스파일(adt_heap.c)를 완성한다.
- max-heap으로 구현해야 함
- adt_heap.h에 정의해야 할 함수의 원형(prototype)이 선언되어 있으며, adt_heap.c에 함수를 직접 정의해야 함
- main 함수에 있는 주석을 제거하여 두 개의 실행 파일(run_int_heap, run_str_heap)과 같이 실행되도록 수정해야 함
- valgrind를 통해 메모리 누수(memory leak) 현상을 제거해야 함
- 과제 설명에 대한 강의는 "강의자료 -> 12주차"에 있음
- 실행 방법: ./run_int_heap
- 실행 방법: ./run_str_heap 파일명
          예) ./run_str_heap names.txt
- 제출 파일: 모든 소스코드(adt_heap.c, adt_heap.h, run_int_heap.c, run_str_heap.c)와 Makefile
```
---
## Assignment 7 : AVL Tree 구현 (문자열 자료형)
```
주어진 소스파일(stravlt.c)을 완성한다.
- 소스파일에는 함수의 원형(prototype)만 선언되어 있으며, 함수를 직접 정의해야 함
- main 함수에 있는 주석을 제거하여 실행 파일(stravlt)과 같이 실행되도록 수정해야 함
- valgrind를 통해 메모리 누수(memory leak) 현상을 제거해야 함
- 과제 설명에 대한 강의는 "강의자료 -> 13주차"에 있음
- 실행 방법: ./stravlt 입력파일
 (실행 예: ./stravlt words_ordered.txt)
소스코드에서 아래와 같이 설정한 후 제출해야 함
#define SHOW_STEP 0
#define BALANCING 1
- 제출파일: stravlt.c (파일명을 수정하지 말 것!)
```
---
## Assignment 8 : Trie 구현
```
주어진 소스파일(trie.c 또는 permuterm_trie.c) 중 하나를 완성한다.
- 소스파일에는 함수의 원형(prototype)만 선언되어 있으며, 함수를 직접 정의해야 함
- main 함수에 있는 주석을 제거하여 실행 파일(trie 또는 permuterm_trie)과 같이 실행되도록 수정해야 함
- valgrind를 통해 메모리 누수(memory leak) 현상을 제거해야 함
- 와일드카드 검색을 위한 함수 구현시 가산점 부여함 (50%)
- 과제 설명에 대한 강의는 "강의자료 -> 14주차"에 있음
- 실행 방법
$ ./trie words_ordered.txt
$ ./permuterm_trie words_ordered.txt
- 제출파일: trie.c 또는 permuterm_trie.c (둘 중 하나만 제출하면 됨)
```
