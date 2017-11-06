#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "SAT.h"
#include <pthread.h>
#include <math.h>

#define SIZE 500

//int inputCom;
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

struct timespec ts;
struct timespec ts0;
struct timespec ts1;
struct timespec ts11;
struct timespec ts2;
struct timespec ts22;
struct timespec ts3;
struct timespec ts33;
struct timespec ts4;
struct timespec ts44;

float cnfsum = 0.0;
float a1sum = 0.0;
float rea1sum = 0.0;
float a2sum = 0.0;
float rea2sum = 0.0;

float totalcnf= 0.0;
float totala1 = 0.0;
float totalrea1 = 0.0;
float totala2 = 0.0;
float totalrea2 = 0.0;

float cnf_ave;
float a1_ave;
float rea1_ave;
float a2_ave;
float rea2_ave;

float cnf_dev;
float a1_dev;
float rea1_dev;
float a2_dev;
float rea2_dev;
	
float cnf[100]; 
float a1[100];
float re_a1[100];
float a2[100]; 
float re_a2[100];


float ratio1[100];
float ratio2[100];
float ratio3[100];
float ratio4[100];

float r1_ave;
float r2_ave; 
float r3_ave; 
float r4_ave; 

float r1sum = 0.0;
float r2sum = 0.0;
float r3sum = 0.0;
float r4sum = 0.0;

float totalr1= 0.0;
float totalr2 = 0.0;
float totalr3 = 0.0;
float totalr4 = 0.0;

float r1_dev;
float r2_dev;
float r3_dev;
float r4_dev;

int times = 0;


int main(void){
	int j = 0;

	int p[2], bak;

	if(pipe(p) < 0){
			fprintf(stderr, "Error: pipe failed\n");
			exit(-1);
	}
	bak = dup(1);
    	dup2(p[1], 1);

	//while((inputCom = getchar()) != EOF){
	while(fgets(readStr,SIZE,stdin) != NULL){
		/*
		int strNum = 0;
		while(inputCom != '\n' && strNum < SIZE){
			readStr[strNum] = inputCom;
			strNum++;
			inputCom = getchar();
		}*/
		//readStr[strNum] = '\0';

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


			//float ratio1, ratio2, ratio3, ratio4;


			read_edge(readStr, n);

			int y = 0;
			for(y = 0; y < 10; y++){
			
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

			//printf("n is %d\n", n);fflush(stdout);	

			cnf[times] = 1.0*ts.tv_nsec / 1000 - 1.0*ts0.tv_nsec / 1000;
			a1[times] = 1.0*ts1.tv_nsec / 1000 - 1.0*ts11.tv_nsec / 1000;
			re_a1[times] = 1.0*ts2.tv_nsec / 1000 - 1.0*ts22.tv_nsec / 1000;
			a2[times] = 1.0*ts3.tv_nsec / 1000 - 1.0*ts33.tv_nsec / 1000;
			re_a2[times] = 1.0*ts4.tv_nsec / 1000 - 1.0*ts44.tv_nsec / 1000; 

			cnfsum += cnf[times];
			a1sum += a1[times];
			rea1sum += re_a1[times];
			a2sum += a2[times];
			rea2sum += re_a2[times];

			//printf("testtesttesttest: %.2f\n", 1.0*ts0.tv_nsec/1000);

			
		
			/*
			printf("cnf: %ld, cnf: %ld\n", ts0.tv_nsec / 1000, ts.tv_nsec / 1000);fflush(stdout);	
			printf("a1: %ld, a1: %ld\n", ts11.tv_nsec / 1000, ts1.tv_nsec / 1000);fflush(stdout);	
			printf("re-a1: %ld, re-a1: %ld\n", ts22.tv_nsec / 1000, ts2.tv_nsec / 1000);fflush(stdout);
			printf("a2: %ld, a2: %ld\n", ts33.tv_nsec / 1000, ts3.tv_nsec / 1000);fflush(stdout);
			printf("re-a2: %ld, re-a2: %ld\n", ts44.tv_nsec / 1000, ts4.tv_nsec / 1000); fflush(stdout);
			*/
			/*
			printf("%.2f\n", cnf[times]); fflush(stdout);
			printf("%.2f\n", a1[times]); fflush(stdout);
			printf("%.2f\n", re_a1[times]); fflush(stdout);
			printf("%.2f\n", a2[times]); fflush(stdout);
			printf("%.2f\n", re_a2[times]); fflush(stdout);
			*/
			ratio1[times] = 1.0*counter1/counter;
			ratio2[times] = 1.0*counter2/counter;
			ratio3[times] = 1.0*counter3/counter;
			ratio4[times] = 1.0*counter4/counter;

			r1sum += ratio1[times];
			r2sum += ratio2[times];
			r3sum += ratio3[times];
			r4sum += ratio4[times];

			//printf("ratio1: %.2f, ratio2: %.2f, ratio3: %.2f, ratio4: %.2f\n", ratio1, ratio2, ratio3, ratio4);
			//fflush(stdout);

			//printf("\n\n"); fflush(stdout);

			times++;

			counter = 0;
			counter1 = 0;
			counter2 = 0;
			counter21 = 0;
			counter3 = 0;
			counter4 = 0;
			counter41 = 0;			

			}
			

		}//end "E"

		//counter = 0;
		//counter1 = 0;
		//counter2 = 0;
		//counter21 = 0;
		//counter3 = 0;
		//counter4 = 0;
		//counter41 = 0;		

		
	}// end outer while


	
	r1_ave = r1sum / times;
	r2_ave = r2sum / times;
	r3_ave = r3sum / times;
	r4_ave = r4sum / times;

	cnf_ave = cnfsum / times;
	a1_ave = a1sum / times;
	rea1_ave = rea1sum / times;
	a2_ave = a2sum / times;
	rea2_ave = rea2sum / times;

	//cal the std deviation
	int x = 0;
	for(x = 0; x < times; x++){
		cnf[x] -= cnf_ave;
		cnf[x] = cnf[x]*cnf[x];	
		a1[x] -= a1_ave;
		a1[x] = a1[x]*a1[x];
		re_a1[x] -= rea1_ave;
		re_a1[x] = re_a1[x]*re_a1[x];
		a2[x] -= a2_ave;
		a2[x] = a2[x]*a2[x];
		re_a2[x] -= rea2_ave;
		re_a2[x] = re_a2[x]*re_a2[x];
		
		totalcnf += cnf[x];
		totala1 += a1[x];
		totalrea1 += re_a1[x];
		totala2 += a2[x];
		totalrea2 += re_a2[x];
	}

	for(x = 0; x < times; x++){
		ratio1[x] -= r1_ave;
		ratio1[x] = ratio1[x]*ratio1[x];	
		ratio2[x] -= r2_ave;
		ratio2[x] = ratio2[x]*ratio2[x];
		ratio3[x] -= r3_ave;
		ratio3[x] = ratio3[x]*ratio3[x];
		ratio4[x] -= r4_ave;
		ratio4[x] = ratio4[x]*ratio4[x];
		
		totalr1 += ratio1[x];
		totalr2 += ratio2[x];
		totalr3 += ratio3[x];
		totalr4 += ratio4[x];
	}
	
	cnf_dev = sqrt(totalcnf/times);
	a1_dev = sqrt(totala1/times);
	rea1_dev = sqrt(totalrea1/times);
	a2_dev = sqrt(totala2/times);
	rea2_dev = sqrt(totalrea2/times);

	r1_dev = sqrt(totalr1/times);
	r2_dev = sqrt(totalr2/times);
	r3_dev = sqrt(totalr3/times);
	r4_dev = sqrt(totalr4/times);

	dup2(bak, 1);	

	printf("cnf average: %.2f, std_dev: %.2f\n", cnf_ave, cnf_dev);fflush(stdout);
	printf("a1 average: %.2f, std_dev: %.2f\n", a1_ave, a1_dev);fflush(stdout);
	printf("rea1 average: %.2f, std_dev: %.2f\n", rea1_ave, rea1_dev);fflush(stdout);
	printf("a2 average: %.2f, std_dev: %.2f\n", a2_ave, a2_dev);fflush(stdout);
	printf("rea2 average: %.2f, std_dev: %.2f\n", rea2_ave, rea2_dev);fflush(stdout);
	printf("\n\n"); fflush(stdout);
	printf("ratio1 average: %.2f, std_dev: %.2f\n", r1_ave, r1_dev);fflush(stdout);
	printf("ratio2 average: %.2f, std_dev: %.2f\n", r2_ave, r2_dev);fflush(stdout);
	printf("ratio3 average: %.2f, std_dev: %.2f\n", r3_ave, r3_dev);fflush(stdout);
	printf("ratio4 average: %.2f, std_dev: %.2f\n", r4_ave, r4_dev);fflush(stdout);
	printf("\n\n"); fflush(stdout);

	return 0;
}


int compar(const void*a, const void*b){

   return  *(int*)a-*(int*)b;

}


void * thread_cnf (void *msg){
	
	clockid_t cid;

	pthread_getcpuclockid(pthread_self(), &cid);

	clock_gettime(cid, &ts0);
	

	int k = 1;// minimum vertex cover			
	while(k <= n){
		//int p[2], bak;
		/*
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

	pthread_getcpuclockid(pthread_self(), &cid);

	clock_gettime(cid, &ts);
	
	//printf("%4ld.%03ld\n", ts.tv_sec, ts.tv_nsec / 1000000);
	

	return NULL;
}


void * thread_a1 (void *msg1){

	clockid_t cid;

	pthread_getcpuclockid(pthread_self(), &cid);

	clock_gettime(cid, &ts11);

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


	pthread_getcpuclockid(pthread_self(), &cid);

	clock_gettime(cid, &ts1);
	//printf("%4ld.%03ld\n", ts.tv_sec, ts.tv_nsec / 1000000);
	
	
	
	return NULL;
}


void * thread_re_a1(void *msg2){

	clockid_t cid;

	pthread_getcpuclockid(pthread_self(), &cid);

	clock_gettime(cid, &ts22);

			

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


	pthread_getcpuclockid(pthread_self(), &cid);

	clock_gettime(cid, &ts2);
	//printf("%4ld.%03ld\n", ts.tv_sec, ts.tv_nsec / 1000000);
		
	
	
	return NULL;
}


void * thread_a2 (void *msg1){


	clockid_t cid;

	pthread_getcpuclockid(pthread_self(), &cid);

	clock_gettime(cid, &ts33);


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

	pthread_getcpuclockid(pthread_self(), &cid);

	clock_gettime(cid, &ts3);
	//printf("%4ld.%03ld\n", ts.tv_sec, ts.tv_nsec / 1000000);
	
	
	return NULL;
}

void * thread_re_a2 (void *){


	clockid_t cid;

	pthread_getcpuclockid(pthread_self(), &cid);

	clock_gettime(cid, &ts44);


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


	pthread_getcpuclockid(pthread_self(), &cid);

	clock_gettime(cid, &ts4);
	//printf("%4ld.%03ld\n", ts.tv_sec, ts.tv_nsec / 1000000);
		
	
	
	return NULL;	
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

