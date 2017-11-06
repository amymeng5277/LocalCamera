#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "SAT.h"

#define SIZE 50000

int inputCom;
char readStr[SIZE];
int n = 0; // The number of vertices
int v_for_edges[SIZE]; // store the vertex no. specified by E command
int count_v = 0;

void read_edge();

int compar(const void*a, const void*b){

   return  *(int*)a-*(int*)b;

}


int main(void){

	memset(v_for_edges, 0, SIZE*sizeof(int));

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

		}//end if "V"

		else if(readStr[0] == 'E'){

			int answer[n];

			memset(answer, 0, n*sizeof(int));

			read_edge();
			
			int k = 1;// minimum vertex cover			

			while(k <= n){
				int p[2], bak;
				if(pipe(p) < 0){
					fprintf(stderr, "Error: pipe failed\n");
					exit(-1);
				}
				bak = dup(1);
    				dup2(p[1], 1);

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

				dup2(bak, 1);

				if(result == SATISFIABLE) {
					int index = 0;
					int counter = 0;

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
							//printf("%d ", -1*s); fflush(stdout);
						}
						else {
							printf("Error!"); fflush(stdout);
						}
					}

					//sort the integer array answer
					qsort(answer, counter, sizeof(answer[0]), compar);

					for(d = 0; d < counter - 1; d++){
						printf("%d ", answer[d]);fflush(stdout);
					}
					printf("%d\n", answer[counter-1]);fflush(stdout);
					break;
				}
				else {
					//printf("unsat.\n"); fflush(stdout);
				}

				k++;
				SAT_ReleaseManager(mgr);

			}// end while			

		}//end "E"

	}// end outer while

	return 0;
}

void read_edge(){
	int j = 4; //The first number in E
	int l = 0; 
	int src = 0; 
	int dst = 0;
	count_v = 0;
	//int return_value;
	char token1[SIZE];
				
	while(readStr[j] != '>'){	
		l = 0;
					
		while(readStr[j] != ',' && readStr[j] != '>'){
			token1[l] = readStr[j];
			j++;
			l++;
		}
		token1[l] = '\0';
								
		v_for_edges[count_v] = atoi(token1);

		count_v++;
			
		if(readStr[j] == '>' && readStr[j+1] != '}')
			j += 3;
		else if(readStr[j] == '>' && readStr[j+1] == '}')
			break;
		else if(readStr[j] == ',' && readStr[j+1] == '<')
			j += 2;
		else if(readStr[j] == ',' && readStr[j+1] != '>')
			j++;
	}

	v_for_edges[count_v] = '\0';

	int b = 0;		
			
	while(b < count_v){
		src = v_for_edges[b];
		dst = v_for_edges[b+1];	
		if (src > n - 1){
			printf("Error: The vertex %d in the edge <%d,%d> does not exsit.\n",src, src, dst);
			break;
		}
		if (dst > n - 1){
			printf("Error: The vertex %d in the edge <%d,%d> does not exsit.\n",dst, src, dst);
			break;
		}
		//return_value = add_edge(src, dst);
		//if(return_value != 0)
			//break;
		b +=2;
			
	}
}

