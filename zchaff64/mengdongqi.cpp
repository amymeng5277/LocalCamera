//
// Created by mengdongqi on 15-11-12.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "mengdongqi.h"
#include "SAT.h"

int main() {
    USER_INPUT user_input;
    read_and_handle_input(&user_input);
    return 0;
}

void read_and_handle_input(USER_INPUT *user_input) {
    size_t line_buffer_max = LINE_BUFFER_SIZE;
    char *line = (char *) malloc(line_buffer_max);
    bool getV = false, getE = false;
    while (getline(&line, &line_buffer_max, stdin) != EOF) {
        int nodeNum = 0;
        if (line[0] == 'V') {
            clear_user_input(user_input);
            sscanf(line + 2, "%d", &nodeNum);
            if (DEBUG)
                printf("catch input node Num: %d\n", nodeNum);
            if (nodeNum > 0) {
                user_input->input_node_num = nodeNum;
                getV = true;
            } else {
                printf("Error: node Num %d should be greater than 0.\n", nodeNum);
                clear_user_input(user_input);
                continue;
            }
        }
        //printf("V:%d", user_input->input_node_num);
        if (line[0] == 'E') {
            bool input_edge_legal = true;
            int a, b = 0;
            char s[32]; // max num of 32 digit
            char *line_cursor = (line + 3);
            sscanf(line_cursor, "<%d,%d>", &a, &b);
            line_cursor += num_str_length(s, a) + num_str_length(s, b) + 3;
            EDGE e = create_input_edge(user_input, a, b);
            if (check_input_edge(e, user_input->input_node_num) == false) {
                input_edge_legal = false;
            }
            if (DEBUG)
                printf("catch input edge between a=%d and b=%d\n", a, b);
            while (*line_cursor != '}') {
                sscanf(line_cursor, ",<%d,%d>", &a, &b);
                line_cursor += num_str_length(s, a) + num_str_length(s, b) + 4;
                if (DEBUG)
                    printf("catch input edge between a=%d and b=%d\n", a, b);
                EDGE e = create_input_edge(user_input, a, b);
                if (check_input_edge(e, user_input->input_node_num) == false) {
                    input_edge_legal = false;
                    break;
                }
            }
            if (input_edge_legal == false) {
                clear_user_input(user_input);
                continue;
            } else {
                getE = true;
            }
        }
        // printf("E num:%d", user_input->input_edge_num);

        if (!(getV && getE)) continue;
        if (getV && getE) {
            int min_vertexs = 1, node_num = 0;
            node_num = user_input->input_node_num;
            int answer[node_num];
            memset(answer, 0, node_num * sizeof(int));

            while (min_vertexs <= node_num) {
                int p[2], fd_bak;
                if (pipe(p) < 0) {
                    fprintf(stderr, "Error: pipe failed\n");
                    exit(-1);
                }
                fd_bak = dup(STDOUT_FILENO);
                dup2(p[1], STDOUT_FILENO);

                int min_vertex_list_count, node_count, temp_index;
                int initial_value = 1;// value of variable
                int matrix[node_num][min_vertexs];// a node table with n * k

                // initialize atoms'value and the number of atoms is n * k
                for (min_vertex_list_count = 0; min_vertex_list_count < min_vertexs; min_vertex_list_count++) {
                    for (node_count = 0; node_count < node_num; node_count++) {
                        matrix[node_count][min_vertex_list_count] = initial_value;
                        initial_value++;
                    }
                }

                SAT_Manager mgr = SAT_InitManager();

                SAT_SetNumVariables(mgr, node_num * min_vertexs);//atomic formulas(number of n*k)

                int clause_list[node_num * min_vertexs];

                //Clause 1
                for (min_vertex_list_count = 0; min_vertex_list_count < min_vertexs; min_vertex_list_count++) {
                    for (node_count = 0; node_count < node_num; node_count++) {
                        clause_list[node_count] = (matrix[node_count][min_vertex_list_count] << 1);
                    }
                    SAT_AddClause(mgr, clause_list, node_num);
                }

                //Clause 2
                node_count = 0;
                while (min_vertexs > 1 && node_count < node_num) {
                    for (min_vertex_list_count = 0; min_vertex_list_count < min_vertexs - 1; min_vertex_list_count++) {
                        for (temp_index = 1; min_vertex_list_count + temp_index < min_vertexs; temp_index++) {
                            clause_list[0] = (matrix[node_count][min_vertex_list_count] << 1) + 1;
                            clause_list[1] = (matrix[node_count][min_vertex_list_count + temp_index] << 1) + 1;
                            SAT_AddClause(mgr, clause_list, 2);
                        }
                    }
                    node_count++;
                }

                //Clause 3
                for (min_vertex_list_count = 0; min_vertex_list_count < min_vertexs; min_vertex_list_count++) {
                    for (node_count = 0; node_count < node_num - 1; node_count++) {
                        for (temp_index = 1; node_count + temp_index < node_num; temp_index++) {
                            clause_list[0] = (matrix[node_count][min_vertex_list_count] << 1) + 1;
                            clause_list[1] = (matrix[node_count + temp_index][min_vertex_list_count] << 1) + 1;
                            SAT_AddClause(mgr, clause_list, 2);
                        }
                    }
                }


                //Clause 4
                node_count = 0;
                while (node_count < user_input->input_edge_num) {
                    min_vertex_list_count = 0;
                    while (min_vertex_list_count < min_vertexs) {
                        clause_list[min_vertex_list_count] = (matrix[user_input->input_edges[node_count].A][min_vertex_list_count] << 1);
                        min_vertex_list_count++;
                    }
                    temp_index = min_vertexs;
                    min_vertex_list_count = 0;
                    while (min_vertex_list_count < min_vertexs) {
                        clause_list[temp_index] = (matrix[user_input->input_edges[node_count].B][min_vertex_list_count] << 1);
                        temp_index++;
                        min_vertex_list_count++;
                    }
                    SAT_AddClause(mgr, clause_list, 2 * min_vertexs);
                    node_count++;
                }

                int result = SAT_Solve(mgr);
                dup2(fd_bak, STDOUT_FILENO);

                if (result == SATISFIABLE) {
                    int answer_index = 0;
                    int anwser_node_num = 0;

                    int variables_count = SAT_NumVariables(mgr);
                    int i;
                    for (i = 1; i <= variables_count; i++) {
                        int assignment = SAT_GetVarAsgnment(mgr, i);
                        if (assignment == 1) {
                            if (i > node_num) {
                                if ((i % node_num) == 0)
                                    answer[answer_index] = node_num - 1;
                                else
                                    answer[answer_index] = (i % node_num) - 1;
                            }
                            else
                                answer[answer_index] = i - 1;
                            answer_index++;
                            anwser_node_num++;

                        }
                        else if (assignment == 0) {
                            //printf("test %d ", -s); fflush(stdout);
                        }
                        else {
                            printf("Error: unexpect assignment!");
                            fflush(stdout);
                        }
                    }

                    //sort the result
                    qsort(answer, anwser_node_num, sizeof(answer[0]), compare_num);

                    for (temp_index = 0; temp_index < anwser_node_num - 1; temp_index++) {
                        printf("%d ", answer[temp_index]);
                        fflush(stdout);
                    }
                    printf("%d\n", answer[anwser_node_num - 1]);
                    fflush(stdout);
                    break;
                }
                else {
                    //printf("UNSATISFIABLE.\n"); fflush(stdout);
                }

                min_vertexs++;
                SAT_ReleaseManager(mgr);

            }
        }
        getE = false;
        getV = false;
    }
    free(line);
}

void clear_user_input(USER_INPUT *user_input) {
    user_input->input_node_num = 0;
    user_input->input_edge_num = 0;
    for (int i = 0; i < MAX_INPUT_EDGE_NUM; i++) {
        EDGE e = {0, 0};
        user_input->input_edges[i] = e;
    }

}

bool check_input_edge(EDGE e, int input_node_num) {
    if ((e.A >= 0 && e.A <= input_node_num - 1) &&
        (e.B >= 0 && e.B <= input_node_num - 1))
        return true;
    printf("Error: the edge of %d -> %d is not in the range of [0,%d)\n", e.A, e.B, input_node_num);
    return false;
}

EDGE create_input_edge(USER_INPUT *user_input, int A, int B) {
    EDGE edge = {A, B};
    user_input->input_edge_num++;
    user_input->input_edges[user_input->input_edge_num - 1] = edge;
    return edge;
}

int num_str_length(char *s, int num) {
    sprintf(s, "%d", num);
    int num_length = strlen(s);
    return num_length;
}

int compare_num(const void *num_A, const void *num_B) {
    return *(int *) num_A - *(int *) num_B;

}



