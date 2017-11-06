#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "SAT.h"
#include <pthread.h>

#define SIZE 500

int inputCom;
char readStr[SIZE];


void read_edge(char[], int);
int compar(const void*, const void*);

void * thread_cnf (void *);
void * thread_a1 (void *);
void * thread_re_a1 (void *);
void * thread_a2 (void *);
void * thread_re_a2 (void *);


int v_for_edges[SIZE]; // store the vertex no. specified by E command
int count_v = 0;
int n = 0; // The number of vertices

int answer[SIZE];// cnf
int counter = 0;

int answer1[SIZE];// approx-a1
int counter1 = 0;

int answer2[SIZE];// a1-refine
int t_answer2[SIZE];// temp answer for a1-refine
int counter2 = 0;
int counter21 = 0;

int answer3[SIZE];// approx-a2
int counter3 = 0;

int answer4[SIZE];// a2-refine
int t_answer4[SIZE];// temp answer for a2-refine
int counter4 = 0;
int counter41 = 0;

int main(void){
	int j = 0;

	while((inputCom = getchar()) != EOF){

		
		
		int strNum = 0;
		while(inputCom != '\n' && strNum < SIZE){
			readStr[strNum] = inputCom;
			strNum++;
			inputCom = getchar();
		}
		readStr[strNum] = '\0';

		if(readStr[0] == 'V'){

			char *token;
			token = strtok(readStr, " ");
			token = strtok(NULL, " ");
			n = atoi(token);// The number of nodes
			
			int i = 0;
			
			if(n == 0){
				count_v = 0;
			}
		}//end if "V"

		else if(readStr[0] == 'E'){

			read_edge(readStr, n);

			int p[2], bak;
			if(pipe(p) < 0){
				fprintf(stderr, "Error: pipe failed\n");
				exit(-1);
			}
			bak = dup(1);
    			dup2(p[1], 1);	
			
			int rn1;
			int rn2;
			int rn3;
			int rn4;
			int rn5;
			pthread_t threadCnf;
			pthread_t threadA1;
			pthread_t threadReA1;
			pthread_t threadA2;
			pthread_t threadReA2;

			rn1 = pthread_create(&threadCnf, NULL, thread_cnf, NULL);
			if(rn1 != 0){
				fprintf(stderr, "The thread creation failed\n");
				exit(-1);
			}

			rn2 = pthread_create(&threadA1, NULL, thread_a1, NULL);
			if(rn2 != 0){
				fprintf(stderr, "The thread creation failed\n");
				exit(-1);
			}

			rn3 = pthread_create(&threadReA1, NULL, thread_re_a1, NULL);
			if(rn3 != 0){
				fprintf(stderr, "The thread creation failed\n");
				exit(-1);
			}

			
			rn4 = pthread_create(&threadA2, NULL, thread_a2, NULL);
			if(rn4 != 0){
				fprintf(stderr, "The thread creation failed\n");
				exit(-1);
			}

			rn5 = pthread_create(&threadReA2, NULL, thread_re_a2, NULL);
			if(rn5 != 0){
				fprintf(stderr, "The thread creation failed\n");
				exit(-1);
			}

			pthread_join(threadCnf, NULL);
			pthread_join(threadA1, NULL);
			pthread_join(threadReA1, NULL);
			pthread_join(threadA2, NULL);
			pthread_join(threadReA2, NULL);

			dup2(bak, 1);	

			printf("CNF-SAT-VC: ");
			for(j = 0; j < counter - 1; j++){
				printf("%d ", answer[j]);
			}
			printf("%d\n", answer[counter-1]);fflush(stdout);

			printf("APPROX-VC-1: ");
			for(j = 0; j < counter1-1; j++){
				printf("%d,", answer1[j]);
			}
			printf("%d\n", answer1[counter1-1]);fflush(stdout);

			
			printf("REFINED-APPROX-VC-1: ");
			for(j = 0; j < counter2-1; j++){
				printf("%d,", answer2[j]);
			}
			printf("%d\n", answer2[counter2-1]);fflush(stdout);

			printf("APPROX-VC-2: ");
			for(j = 0; j < counter3-1; j++){
				printf("%d,", answer3[j]);
			}
			printf("%d\n", answer3[counter3-1]);fflush(stdout);

			printf("REFINED-APPROX-VC-2: ");
			for(j = 0; j < counter4-1; j++){
				printf("%d,", answer4[j]);
			}
			printf("%d\n", answer4[counter4-1]);fflush(stdout);
			

		}//end "E"

		counter = 0;
		counter1 = 0;
		counter2 = 0;
		counter21 = 0;
		counter3 = 0;
		counter4 = 0;
		counter41 = 0;

		
	}// end outer while

	return 0;
}


int compar(const void*a, const void*b){

   return  *(int*)a-*(int*)b;

}

void read_edge(char readStr1[], int n1){
	int j = 4; //The first number in E
	int l = 0; 
	int src = 0; 
	int dst = 0;
	count_v = 0;
	int return_value;
	char token1[SIZE];
	int b1 = 0;

	if(readStr1[3] == '}'){
		printf(" ");
		printf("\n");
		//continue;
	}
				
	while(readStr1[j] != '>'){	
		l = 0;
					
		while(readStr1[j] != ',' && readStr1[j] != '>'){
			token1[l] = readStr1[j];
			j++;
			l++;
		}
		token1[l] = '\0';
								
		v_for_edges[count_v] = atoi(token1);

		count_v++;
			
		if(readStr1[j] == '>' && readStr1[j+1] != '}')
			j += 3;
		else if(readStr1[j] == '>' && readStr1[j+1] == '}')
			break;
		else if(readStr1[j] == ',' && readStr1[j+1] == '<')
			j += 2;
		else if(readStr1[j] == ',' && readStr1[j+1] != '>')
			j++;
	}
				
			
	while(b1 < count_v){
		src = v_for_edges[b1];
		dst = v_for_edges[b1+1];	
		if (src > n1 - 1){
			printf("Error: The vertex %d in the edge <%d,%d> does not exsit.\n",src, src, dst);
			b1 = 0;
			break;
		}
		if (dst > n1 - 1){
			printf("Error: The vertex %d in the edge <%d,%d> does not exsit.\n",dst, src, dst);
			b1 = 0;
			break;
		}
		
		b1 +=2;
			
	}

	
}



void * thread_cnf (void *msg){
	int k = 1;// minimum vertex cover			
	while(k <= n){
		/*
		int p[2], bak;
		if(pipe(p) < 0){
			fprintf(stderr, "Error: pipe failed\n");
			exit(-1);
		}
		bak = dup(1);
    		dup2(p[1], 1);*/		
		int a,b,d; // a for count k, b for count n, d for extra count
		int g = 1;// value of variable
		int m[n][k];// matrix
		// initialize the value of every atom, in all number of n * k atoms.
		for(a = 0; a < k; a++){
			for(b = 0; b < n; b++){
				m[b][a] = g;
				g++;
			}
		}		
		SAT_Manager mgr = SAT_InitManager();
		SAT_SetNumVariables(mgr, n * k);//atomic formulas(number of n*k)
		int c[n*k];		
		//case 1
		for(a = 0; a < k; a++){
			for(b = 0; b < n; b++){
				c[b] = (m[b][a] << 1);
			}
			SAT_AddClause(mgr, c, n);
		}
		//case 2
		if(k > 1){
			for(b = 0; b < n; b++){
				for(a = 0; a < k-1; a++){
					for(d = 1; a+d < k; d++){
						c[0] = (m[b][a] << 1) + 1;
						c[1] = (m[b][a+d] << 1) + 1;
						SAT_AddClause(mgr, c, 2);
					}
				}
			}
		}
		//case 3
		for(a = 0; a < k; a++){
			for(b = 0; b < n-1; b++){
				for(d = 1; b+d < n; d++){
					c[0] = (m[b][a] << 1) +1;
					c[1] = (m[b+d][a] << 1) +1;
					SAT_AddClause(mgr, c, 2);
				}
			}
		}	
		//case 4
		for(b = 0; b < count_v; b+=2){
			for(a = 0; a < k; a++){
				c[a] = (m[v_for_edges[b]][a] << 1);
			}
			d = k;
			for(a = 0; a < k; a++){
				c[d] = (m[v_for_edges[b+1]][a] << 1);
				d++;
			}
			SAT_AddClause(mgr, c, 2*k);
		}
		int result = SAT_Solve(mgr);
		//dup2(bak, 1);	
		if(result == SATISFIABLE) {
			int index = 0;		
			int z = SAT_NumVariables(mgr);
			int s;
			for(s = 1; s <= z; s++) {
				int a1 = SAT_GetVarAsgnment(mgr, s);
				if(a1 == 1) {
							
					if(s <= n){
						answer[index] = s - 1;
					}
					else{
						if((s % n) == 0){
							answer[index] = n - 1;
						}
						else{
							answer[index] = (s % n) - 1;
						}
					}
					index++;
					counter++;		
				}
				else if(a1 == 0) {		
				}
				else {
					printf("Error!"); fflush(stdout);
				}
			}
			//sort the integer array answer
			qsort(answer, counter, sizeof(answer[0]), compar);
			break;
		}
		else {
		}
		k++;
		SAT_ReleaseManager(mgr);
	}// end while
	return NULL;
}


void * thread_a1 (void *msg1){
	int ct[n];// count each vertex's degree
	int i,j;
	int max, index, sum = 0;

	for(i = 0; i < n; i++){
		ct[i] = 0;
	}

	for(i = 0; i < n; i++){
		for(j = 0; j < count_v; j++){
			if(i == v_for_edges[j]){
				ct[i]+=1;
			}
		}
	}
	for(i = 0; i < n; i++){
		sum +=ct[i];// total degree
	}
	//printf("sum before is %d\n", sum);
	while(sum != 0){
		max = ct[0];
		index = 0;
		// find the node with highest degree
		for(i = 1; i < n; i++){
			if(max < ct[i]){
				max = ct[i];
				index = i;
			}
		}
		//printf("the node with highest degree is %d\n", index);
	
		ct[index] = 0;
	
		//add index to the vertex cover
		answer1[counter1] = index;
		counter1++;

		for(j = 0; j < count_v; j+=2){
			if(index == v_for_edges[j]){
				if(ct[v_for_edges[j+1]] != 0)
					ct[v_for_edges[j+1]]--;
			}
			else if(index == v_for_edges[j+1]){
				if(ct[v_for_edges[j]] != 0)
					ct[v_for_edges[j]]--;
			}
		}
		
		sum = 0;
		for(i = 0; i < n; i++){
			sum +=ct[i];
		}

	}//end while

	//sort the integer array answer
	qsort(answer1, counter1, sizeof(answer1[0]), compar);
	
	return NULL;
}


void * thread_re_a1(void *msg2){
	int ct[n];// count each vertex's degree
	int i,j;
	int max, index, sum = 0;

	for(i = 0; i < n; i++){
		ct[i] = 0;
	}

	for(i = 0; i < n; i++){
		for(j = 0; j < count_v; j++){
			if(i == v_for_edges[j]){
				ct[i]+=1;
			}
		}
	}
	for(i = 0; i < n; i++){
		sum +=ct[i];// total degree
	}
	//printf("sum before is %d\n", sum);
	while(sum != 0){
		max = ct[0];
		index = 0;
		// find the node with highest degree
		for(i = 1; i < n; i++){
			if(max < ct[i]){
				max = ct[i];
				index = i;
			}
		}
		//printf("the node with highest degree is %d\n", index);
	
		ct[index] = 0;
	
		//add index to the vertex cover
		t_answer2[counter21] = index;
		counter21++;

		for(j = 0; j < count_v; j+=2){
			if(index == v_for_edges[j]){
				if(ct[v_for_edges[j+1]] != 0)
					ct[v_for_edges[j+1]]--;
			}
			else if(index == v_for_edges[j+1]){
				if(ct[v_for_edges[j]] != 0)
					ct[v_for_edges[j]]--;
			}
		}
		
		sum = 0;
		for(i = 0; i < n; i++){
			sum +=ct[i];
		}

	}//end while

	//return a vertex cover, next, refine it
	//simply check each edge <u,v>, u or v must in vertex cover
	int check[count_v/2];
	int check1[counter21];
	int k,h,l;
	for(i = 0; i < (count_v/2); i++){
		check[i] = 0;
	}

	for(i = 0; i < counter21; i++){
	        check1[i] = 0;
        }
		
	for(i = 0; i < counter21; i++){
		for(j = 0; j < count_v; j+=2){
			if(i == 0){
				for(k = 1; k < counter21; k++){
					if(v_for_edges[j] == t_answer2[k])
						check[j/2]++;
					if(v_for_edges[j+1] == t_answer2[k])
						check[j/2]++;
				}
			}
			else if(i > 0 && i < counter21 - 1){
				for(k = 0; k < i; k++){
					if(v_for_edges[j] == t_answer2[k])
						check[j/2]++;
					if(v_for_edges[j+1] == t_answer2[k])
						check[j/2]++;
				}
				for(k = i+1; k < counter21; k++){
					if(v_for_edges[j] == t_answer2[k])
						check[j/2]++;
					if(v_for_edges[j+1] == t_answer2[k])
						check[j/2]++;
				}
			}
			else{
				for(k = 0; k < counter21 -1; k++){
					if(v_for_edges[j] == t_answer2[k])
						check[j/2]++;
					if(v_for_edges[j+1] == t_answer2[k])
						check[j/2]++;
				}

			}
			if(check[j/2] == 0){
				answer2[counter2] = t_answer2[i];
				counter2++;
				break;
			}
			else{
				check1[i]++;
			}
			
		}//end for loop for v_for_edges

		if(check1[i] == count_v/2){
			for(l = i; l < counter21 - 1; l++){
				t_answer2[l] = t_answer2[l+1];
			}
			counter21--;
			check1[i] = 0;
			i--;
			
		}
		
		for(h = 0; h < (count_v/2); h++){
			check[h] = 0;
		}
		
	}//end for loop for t_answer2

	//sort the integer array answer
	qsort(answer2, counter2, sizeof(answer2[0]), compar);
	
	
	return NULL;
}


void * thread_a2 (void *msg1){
	int cmp[2];
	int number = count_v;// if number decrease to 0, then no edges exist
	int v_f_e[count_v];
	int count = count_v;// count how many nodes left in v_f_e after throwing away edges
	int count1 = 0;
	memcpy(v_f_e, v_for_edges, sizeof(int)*count_v);

	answer3[counter3] = v_f_e[0];
	counter3++;
	answer3[counter3] = v_f_e[1];
	counter3++;

	cmp[0] = v_f_e[0];
	cmp[1] = v_f_e[1];
	
	int i;
	while(number != 0){
		number -=2;
		for(i = 2; i < count; i+=2){
			if((cmp[0] == v_f_e[i] || cmp[1] == v_f_e[i]) || (cmp[0] == v_f_e[i+1] || cmp[1] == v_f_e[i+1])){
				number -=2;
				continue;
			}
			else{
				v_f_e[count1] = v_f_e[i]; count1++;
				v_f_e[count1] = v_f_e[i+1]; count1++;
				
			}
		}
		count = count1;
		count1 = 0;
		
		cmp[0] = v_f_e[0];
		cmp[1] = v_f_e[1];

		if(number != 0){
		
			answer3[counter3] = v_f_e[0];
			counter3++;
			answer3[counter3] = v_f_e[1];
			counter3++;
		}
	}	

	//sort the integer array answer
	qsort(answer3, counter3, sizeof(answer3[0]), compar);
	
	return NULL;
}

void * thread_re_a2 (void *){
	int cmp[2];
	int number = count_v;// if number decrease to 0, then no edges exist
	int v_f_e[count_v];
	int count = count_v;// count how many nodes left in v_f_e after throwing away edges
	int count1 = 0;
	memcpy(v_f_e, v_for_edges, sizeof(int)*count_v);

	t_answer4[counter41] = v_f_e[0];
	counter41++;
	t_answer4[counter41] = v_f_e[1];
	counter41++;

	cmp[0] = v_f_e[0];
	cmp[1] = v_f_e[1];
	
	int i;
	while(number != 0){
		number -=2;
		for(i = 2; i < count; i+=2){
			if((cmp[0] == v_f_e[i] || cmp[1] == v_f_e[i]) || (cmp[0] == v_f_e[i+1] || cmp[1] == v_f_e[i+1])){
				number -=2;
				continue;
			}
			else{
				v_f_e[count1] = v_f_e[i]; count1++;
				v_f_e[count1] = v_f_e[i+1]; count1++;
				
			}
		}
		count = count1;
		count1 = 0;
		
		cmp[0] = v_f_e[0];
		cmp[1] = v_f_e[1];

		if(number != 0){
		
			t_answer4[counter41] = v_f_e[0];
			counter41++;
			t_answer4[counter41] = v_f_e[1];
			counter41++;
		}
	}


	//return a vertex cover, next, refine it
	//simply check each edge <u,v>, u or v must in vertex cover
	int check[count_v/2];
	int k,h;
	int j,l;

	int check1[counter41];	
	
	for(i = 0; i < (count_v/2); i++){
		check[i] = 0;
	}

	for(i = 0; i < counter41; i++){
		check1[i] = 0;
	}
		
	for(i = 0; i < counter41; i++){
		for(j = 0; j < count_v; j+=2){
			if(i == 0){
				for(k = 1; k < counter41; k++){
					if(v_for_edges[j] == t_answer4[k])
						check[j/2]++;
					if(v_for_edges[j+1] == t_answer4[k])
						check[j/2]++;
				}
			}
			else if(i > 0 && i < counter41 - 1){
				for(k = 0; k < i; k++){
					if(v_for_edges[j] == t_answer4[k])
						check[j/2]++;
					if(v_for_edges[j+1] == t_answer4[k])
						check[j/2]++;
				}
				for(k = i+1; k < counter41; k++){
					if(v_for_edges[j] == t_answer4[k])
						check[j/2]++;
					if(v_for_edges[j+1] == t_answer4[k])
						check[j/2]++;
				}
			}
			else{
				for(k = 0; k < counter41 -1; k++){
					if(v_for_edges[j] == t_answer4[k])
						check[j/2]++;
					if(v_for_edges[j+1] == t_answer4[k])
						check[j/2]++;
				}

			}
			
			if(check[j/2] == 0){
				// the j/2-th node in t_answer4 must be in vertex cover
				answer4[counter4] = t_answer4[i];
				counter4++;
				break;
			}
			else{
				check1[i]++;
			}
			
			
		}//end for loop for v_for_edges
		
		// node i is useless
		if(check1[i] == count_v/2){
			for(l = i; l < counter41-1; l++){
				t_answer4[l] = t_answer4[l+1];
			}
			counter41--;
			check1[i] = 0;	
			i--;
			
		}
		
		
		for(h = 0; h < (count_v/2); h++){
			check[h] = 0;
		}
		
	}//end for loop for t_answer4

	//sort the integer array answer
	qsort(answer4, counter4, sizeof(answer4[0]), compar);
	
	
	return NULL;	
}

