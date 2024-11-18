#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <utility> // for std::pair

// Helper function to calculate combinations (n choose k)
long long comb(int n, int k) {
    if (k > n) return 0;
    if (k * 2 > n) k = n - k;
    if (k == 0) return 1;

    long long result = n;
    for(int i = 2; i <= k; ++i) {
        result *= (n - i + 1);
        result /= i;
    }
    return result;
}

// Helper function to generate vertex combinations
void vertex_combinations(std::vector<std::vector<int> >& result, int n, int k) {
    std::vector<bool> v(n);
    std::fill(v.end() - k, v.end(), true);
    
    do {
        std::vector<int> combination;
        for (int i = 0; i < n; ++i) {
            if (v[i]) {
                combination.push_back(i + 1);  // +1 because vertices start from 1
            }
        }
        result.push_back(combination);
    } while (std::next_permutation(v.begin(), v.end()));
}

// Helper function to generate edge combinations from vertices
std::vector<std::pair<int, int> > edge_combinations(const std::vector<int>& vertices) {
    std::vector<std::pair<int, int> > edges;
    for (size_t i = 0; i < vertices.size(); ++i) {
        for (size_t j = i + 1; j < vertices.size(); ++j) {
            edges.push_back(std::make_pair(vertices[i], vertices[j]));
        }
    }
    return edges;
}

void generate(int n, int p, int q) {
    // Create edge dictionary
    std::map<std::pair<int,int>, int> edge_dict;
    int edge_counter = 0;
    
    // Generate edge variables
    for (int j = 1; j <= n; j++) {
        for (int i = 1; i < j; i++) {
            edge_counter++;
            edge_dict[std::make_pair(i,j)] = edge_counter;
        }
    }

    // Generate temporary filename
    std::string temp_file = "constraints_temp_" + std::to_string(n) + "_" + 
                           std::to_string(p) + "_" + std::to_string(q);
    std::string final_file = "constraints_" + std::to_string(n) + "_" + 
                            std::to_string(p) + "_" + std::to_string(q);

    // Open temporary file for writing
    std::ofstream temp_out(temp_file);
    
    // Generate p-clique constraints
    std::vector<std::vector<int> > p_combinations;
    vertex_combinations(p_combinations, n, p);
    
    for (const auto& vertices : p_combinations) {
        std::string constraint;
        auto edges = edge_combinations(vertices);
        for (const auto& edge : edges) {
            constraint += std::to_string(-edge_dict[edge]) + " ";
        }
        temp_out << constraint << "0\n";
    }

    // Generate q-clique constraints
    std::vector<std::vector<int> > q_combinations;
    vertex_combinations(q_combinations, n, q);
    
    for (const auto& vertices : q_combinations) {
        std::string constraint;
        auto edges = edge_combinations(vertices);
        for (const auto& edge : edges) {
            constraint += std::to_string(edge_dict[edge]) + " ";
        }
        temp_out << constraint << "0\n";
    }
    
    temp_out.close();

    // Calculate totals
    int count = comb(n, 2);
    int clause_count = comb(n, p) + comb(n, q);

    // Write final CNF file
    std::ofstream final_out(final_file);
    final_out << "p cnf " << count << " " << clause_count << "\n";
    
    std::ifstream temp_in(temp_file);
    std::string line;
    while (std::getline(temp_in, line)) {
        final_out << line << "\n";
    }
    
    temp_in.close();
    final_out.close();

    // Clean up temporary file
    std::remove(temp_file.c_str());
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " n p q\n";
        return 1;
    }
    
    int n = std::stoi(argv[1]);
    int p = std::stoi(argv[2]);
    int q = std::stoi(argv[3]);
    
    generate(n, p, q);
    return 0;
}
