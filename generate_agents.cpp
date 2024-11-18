#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <utility>

// Helper function to generate combinations
std::vector<std::vector<int> > combinations(int n, int k) {
    std::vector<std::vector<int> > result;
    std::vector<bool> v(n);
    std::fill(v.end() - k, v.end(), true);
    
    do {
        std::vector<int> combination;
        for (int i = 0; i < n; ++i) {
            if (v[i]) {
                combination.push_back(i + 1);
            }
        }
        result.push_back(combination);
    } while (std::next_permutation(v.begin(), v.end()));
    
    return result;
}

// Helper function to generate edge combinations from vertices
std::vector<std::pair<int, int> > get_edges(const std::vector<int>& vertices) {
    std::vector<std::pair<int, int> > edges;
    for (size_t i = 0; i < vertices.size(); ++i) {
        for (size_t j = i + 1; j < vertices.size(); ++j) {
            edges.push_back(std::make_pair(
                std::min(vertices[i], vertices[j]),
                std::max(vertices[i], vertices[j])
            ));
        }
    }
    return edges;
}

std::string generate_ramsey_cnf(int N, int P, int Q) {
    // Generate all edges in the complete graph K_N
    std::vector<std::pair<int, int> > edges;
    for (int i = 1; i <= N; ++i) {
        for (int j = i + 1; j <= N; ++j) {
            edges.push_back(std::make_pair(i, j));
        }
    }
    int num_edges = edges.size();
    
    // Create mapping from edges to variables
    std::map<std::pair<int, int>, int> edge_to_var;
    for (size_t i = 0; i < edges.size(); ++i) {
        edge_to_var[edges[i]] = i + 1;
    }
    
    std::vector<std::vector<int> > clauses;
    
    // Generate clauses for K_P (red edges)
    auto red_combinations = combinations(N, P);
    for (const auto& red_clique : red_combinations) {
        std::vector<int> clause;
        auto clique_edges = get_edges(red_clique);
        for (const auto& edge : clique_edges) {
            clause.push_back(edge_to_var[edge]);
        }
        clauses.push_back(clause);
    }
    
    // Generate clauses for K_Q (blue edges)
    auto blue_combinations = combinations(N, Q);
    for (const auto& blue_clique : blue_combinations) {
        std::vector<int> clause;
        auto clique_edges = get_edges(blue_clique);
        for (const auto& edge : clique_edges) {
            clause.push_back(-edge_to_var[edge]);
        }
        clauses.push_back(clause);
    }
    
    // Prepare DIMACS format output
    std::stringstream output;
    output << "p cnf " << num_edges << " " << clauses.size() << "\n";
    
    for (const auto& clause : clauses) {
        for (int literal : clause) {
            output << literal << " ";
        }
        output << "0\n";
    }
    
    return output.str();
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " N P Q\n";
        return 1;
    }
    
    int N = std::stoi(argv[1]);
    int P = std::stoi(argv[2]);
    int Q = std::stoi(argv[3]);
    
    std::string cnf_output = generate_ramsey_cnf(N, P, Q);
    std::cout << cnf_output;
    
    return 0;
}
