#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Helper function to generate combinations
void combinations(int n, int k, int*** result, int* result_size) {
    bool* v = (bool*)malloc(n * sizeof(bool));
    for (int i = 0; i < n; i++) {
        v[i] = (i >= n - k);
    }
    
    *result_size = 0;
    *result = (int**)malloc(1000 * sizeof(int*));  // Initial size, will grow if necessary
    
    do {
        int* combination = (int*)malloc(k * sizeof(int));
        int idx = 0;
        for (int i = 0; i < n; ++i) {
            if (v[i]) {
                combination[idx++] = i + 1;
            }
        }
        
        (*result)[*result_size] = combination;
        (*result_size)++;
        
        int i = n - 1;
        while (i >= 0 && v[i]) {
            v[i] = false;
            --i;
        }
        if (i >= 0) {
            v[i] = true;
        }
    } while (v[0]);
    
    free(v);
}

// Helper function to generate edge combinations from vertices
void get_edges(int* vertices, int vertices_size, int*** edges, int* edges_size) {
    *edges_size = 0;
    *edges = (int**)malloc(1000 * sizeof(int*));  // Initial size, will grow if necessary
    
    for (int i = 0; i < vertices_size; ++i) {
        for (int j = i + 1; j < vertices_size; ++j) {
            int* edge = (int*)malloc(2 * sizeof(int));
            edge[0] = (vertices[i] < vertices[j]) ? vertices[i] : vertices[j];
            edge[1] = (vertices[i] < vertices[j]) ? vertices[j] : vertices[i];
            (*edges)[*edges_size] = edge;
            (*edges_size)++;
        }
    }
}

// Helper function to map edges to variables
void map_edges_to_vars(int** edges, int edges_size, int* edge_to_var) {
    for (int i = 0; i < edges_size; ++i) {
        edge_to_var[i] = i + 1;
    }
}

void generate_ramsey_cnf(int N, int P, int Q) {
    // Generate all edges in the complete graph K_N
    int* vertices = (int*)malloc(N * sizeof(int));
    for (int i = 0; i < N; ++i) {
        vertices[i] = i + 1;
    }
    
    int edges_size;
    int** edges;
    get_edges(vertices, N, &edges, &edges_size);
    
    // Create mapping from edges to variables
    int* edge_to_var = (int*)malloc(edges_size * sizeof(int));
    map_edges_to_vars(edges, edges_size, edge_to_var);
    
    // Generate clauses for K_P (red edges)
    int** red_combinations;
    int red_combinations_size;
    combinations(N, P, &red_combinations, &red_combinations_size);
    
    // Generate clauses for K_Q (blue edges)
    int** blue_combinations;
    int blue_combinations_size;
    combinations(N, Q, &blue_combinations, &blue_combinations_size);
    
    // Prepare DIMACS format output
    printf("p cnf %d %d\n", edges_size, red_combinations_size + blue_combinations_size);
    
    // Red clauses
    for (int i = 0; i < red_combinations_size; ++i) {
        int* red_clique = red_combinations[i];
        int red_clique_size = P;
        
        int* clause = (int*)malloc(red_clique_size * sizeof(int));
        int clause_size = 0;
        
        for (int j = 0; j < red_clique_size; ++j) {
            int* clique_edges;
            int clique_edges_size;
            get_edges(&red_clique[j], red_clique_size, &clique_edges, &clique_edges_size);
            
            for (int k = 0; k < clique_edges_size; ++k) {
                clause[clause_size++] = edge_to_var[k];
            }
        }
        
        for (int j = 0; j < clause_size; ++j) {
            printf("%d ", clause[j]);
        }
        printf("0\n");
    }
    
    // Blue clauses
    for (int i = 0; i < blue_combinations_size; ++i) {
        int* blue_clique = blue_combinations[i];
        int blue_clique_size = Q;
        
        int* clause = (int*)malloc(blue_clique_size * sizeof(int));
        int clause_size = 0;
        
        for (int j = 0; j < blue_clique_size; ++j) {
            int* clique_edges;
            int clique_edges_size;
            get_edges(&blue_clique[j], blue_clique_size, &clique_edges, &clique_edges_size);
            
            for (int k = 0; k < clique_edges_size; ++k) {
                clause[clause_size++] = -edge_to_var[k];
            }
        }
        
        for (int j = 0; j < clause_size; ++j) {
            printf("%d ", clause[j]);
        }
        printf("0\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: %s N P Q\n", argv[0]);
        return 1;
    }
    
    int N = atoi(argv[1]);
    int P = atoi(argv[2]);
    int Q = atoi(argv[3]);
    
    generate_ramsey_cnf(N, P, Q);
    
    return 0;
}
