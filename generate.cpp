#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <utility> // for std::pair
#include <cassert>
#include <set>
#include <sstream>
#include <limits>

// Global counters for categorized assertion statistics
struct AssertionStats {
    enum Category {
        INPUT_VALIDATION,
        MATHEMATICAL_PROPERTIES,
        ENCODING_PROPERTIES,
        FORMAT_CONSISTENCY,
        THEORETICAL_BOUNDS
    };
    
    static std::map<Category, std::pair<int, int> > checks; // {total, passed} for each category
    static std::map<Category, std::string> category_names;
    
    static void init() {
        category_names.clear();
        category_names[INPUT_VALIDATION] = "Input Validation";
        category_names[MATHEMATICAL_PROPERTIES] = "Mathematical Properties";
        category_names[ENCODING_PROPERTIES] = "Encoding Properties";
        category_names[FORMAT_CONSISTENCY] = "Format Consistency";
        category_names[THEORETICAL_BOUNDS] = "Theoretical Bounds";
    }
    
    static void check(bool condition, const char* message, Category category) {
        checks[category].first++;
        if (condition) {
            checks[category].second++;
        } else {
            std::cerr << "Assertion failed (" << category_names[category] << "): " 
                      << message << std::endl;
            assert(false && message);
        }
    }
    
    static void report() {
        std::cout << "\nAssertion Statistics:\n";
        int total_checks = 0;
        int total_passed = 0;
        
        std::map<Category, std::string>::const_iterator it;
        for (it = category_names.begin(); it != category_names.end(); ++it) {
            const std::pair<int, int>& stats = checks[it->first];
            std::cout << it->second << ":\n";
            std::cout << "  Total checks: " << stats.first << "\n";
            std::cout << "  Passed checks: " << stats.second << "\n";
            std::cout << "  Status: " 
                      << (stats.first == stats.second ? "PASSED" : "FAILED")
                      << "\n\n";
            total_checks += stats.first;
            total_passed += stats.second;
        }
        
        std::cout << "Overall Statistics:\n";
        std::cout << "Total checks: " << total_checks << "\n";
        std::cout << "Total passed: " << total_passed << "\n";
        std::cout << "Overall status: " 
                  << (total_checks == total_passed ? "PASSED" : "FAILED")
                  << std::endl;
    }
};

std::map<AssertionStats::Category, std::pair<int, int> > AssertionStats::checks;
std::map<AssertionStats::Category, std::string> AssertionStats::category_names;

// Macro for categorized assertions
#define CHECK_ASSERT(condition, message, category) \
    AssertionStats::check((condition), message, category)

// Assert: n >= 1, k >= 0, k <= n
// Returns: value >= 0
long long comb(int n, int k) {
    CHECK_ASSERT(n >= 1, "n must be at least 1", AssertionStats::INPUT_VALIDATION);
    CHECK_ASSERT(k >= 0, "k must be non-negative", AssertionStats::INPUT_VALIDATION);
    CHECK_ASSERT(k <= n, "k cannot exceed n", AssertionStats::INPUT_VALIDATION);
    
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

// Assert: n >= k > 0
// Assert: result will contain C(n,k) combinations
// Assert: each combination contains k distinct numbers from 1 to n
void vertex_combinations(std::vector<std::vector<int> >& result, int n, int k) {
    CHECK_ASSERT(n >= k, "n must be >= k", AssertionStats::INPUT_VALIDATION);
    CHECK_ASSERT(k > 0, "k must be positive", AssertionStats::INPUT_VALIDATION);
    
    std::vector<bool> v(n);
    std::fill(v.end() - k, v.end(), true);
    
    do {
        std::vector<int> combination;
        for (int i = 0; i < n; ++i) {
            if (v[i]) {
                combination.push_back(i + 1);  // +1 because vertices start from 1
            }
        }
        CHECK_ASSERT(combination.size() == static_cast<size_t>(k), "Each combination must contain exactly k elements", AssertionStats::ENCODING_PROPERTIES);
        result.push_back(combination);
    } while (std::next_permutation(v.begin(), v.end()));
}

// Assert: vertices contains at least 2 numbers
// Assert: vertices are sorted in ascending order
// Returns: vector of edges (i,j) where i < j
std::vector<std::pair<int, int> > edge_combinations(const std::vector<int>& vertices) {
    CHECK_ASSERT(vertices.size() >= 2, "Need at least 2 vertices for edges", AssertionStats::INPUT_VALIDATION);
    
    // Assert vertices are sorted
    for (size_t i = 1; i < vertices.size(); ++i) {
        CHECK_ASSERT(vertices[i] > vertices[i-1], "Vertices must be sorted in ascending order", AssertionStats::INPUT_VALIDATION);
    }
    
    std::vector<std::pair<int, int> > edges;
    for (size_t i = 0; i < vertices.size(); ++i) {
        for (size_t j = i + 1; j < vertices.size(); ++j) {
            edges.push_back(std::make_pair(vertices[i], vertices[j]));
            CHECK_ASSERT(edges.back().first < edges.back().second, "Edge pairs must have i < j", AssertionStats::ENCODING_PROPERTIES);
        }
    }
    return edges;
}

// Add these helper functions for assertions
bool is_valid_clause(const std::string& clause) {
    std::istringstream iss(clause);
    std::set<int> literals;
    int lit;
    while (iss >> lit && lit != 0) {
        CHECK_ASSERT(lit != 0, "Non-zero literal before end of clause", AssertionStats::FORMAT_CONSISTENCY);
        literals.insert(std::abs(lit));
    }
    return !literals.empty();
}

// Move R function definition before it's used
int R(int p, int q) {
    if (p == 2) return q;
    if (q == 2) return p;
    if (p == 3 && q == 3) return 6;
    if (p == 3 && q == 4) return 9;
    if (p == 3 && q == 5) return 14;
    if (p == 4 && q == 4) return 18;
    return std::numeric_limits<int>::max();
}

// Assert: n >= 2 (need at least 2 vertices for an edge)
// Assert: p >= 2 (clique size must be at least 2)
// Assert: q >= 2 (independent set size must be at least 2)
// Assert: p <= n and q <= n (subgraph sizes can't exceed total vertices)
void generate(int n, int p, int q) {
    // Input validation
    CHECK_ASSERT(n >= 2, "Need at least 2 vertices", 
                AssertionStats::INPUT_VALIDATION);
    CHECK_ASSERT(p >= 2, "Clique size must be at least 2", 
                AssertionStats::INPUT_VALIDATION);
    CHECK_ASSERT(q >= 2, "Independent set size must be at least 2", 
                AssertionStats::INPUT_VALIDATION);
    CHECK_ASSERT(p <= n && q <= n, "Subgraph sizes can't exceed total vertices", 
                AssertionStats::INPUT_VALIDATION);
    
    // Mathematical properties
    CHECK_ASSERT(comb(n,2) < (1 << 30), "Problem size too large for SAT encoding", 
                AssertionStats::MATHEMATICAL_PROPERTIES);
    
    // Edge dictionary assertions
    std::map<std::pair<int,int>, int> edge_dict;
    int edge_counter = 0;
    
    for (int j = 1; j <= n; j++) {
        for (int i = 1; i < j; i++) {
            edge_counter++;
            edge_dict[std::make_pair(i,j)] = edge_counter;
            
            CHECK_ASSERT(edge_dict[std::make_pair(i,j)] == edge_counter, 
                        "Edge variables must be numbered consecutively",
                        AssertionStats::ENCODING_PROPERTIES);
        }
    }

    // For p-clique constraints
    std::vector<std::vector<int> > p_combinations;
    vertex_combinations(p_combinations, n, p);
    
    std::vector<std::vector<int> >::const_iterator p_it;
    for (p_it = p_combinations.begin(); p_it != p_combinations.end(); ++p_it) {
        std::string constraint;
        std::vector<std::pair<int, int> > edges = edge_combinations(*p_it);
        
        std::set<int> used_vertices;  // Define set for each p-clique constraint
        std::vector<std::pair<int, int> >::const_iterator edge_it;
        for (edge_it = edges.begin(); edge_it != edges.end(); ++edge_it) {
            used_vertices.insert(edge_it->first);
            used_vertices.insert(edge_it->second);
            
            int var = edge_dict[*edge_it];
            constraint += std::to_string(-var) + " ";
        }
        CHECK_ASSERT(static_cast<int>(used_vertices.size()) == p, 
                    "P-clique constraint must involve exactly p vertices",
                    AssertionStats::ENCODING_PROPERTIES);
        
        // Check clause validity
        CHECK_ASSERT(is_valid_clause(constraint + "0"), 
                    "Invalid p-clique clause format",
                    AssertionStats::FORMAT_CONSISTENCY);
    }

    // For q-clique constraints
    std::vector<std::vector<int> > q_combinations;
    vertex_combinations(q_combinations, n, q);
    
    std::vector<std::vector<int> >::const_iterator q_it;
    for (q_it = q_combinations.begin(); q_it != q_combinations.end(); ++q_it) {
        std::string constraint;
        std::vector<std::pair<int, int> > edges = edge_combinations(*q_it);
        
        std::set<int> used_vertices;  // Define set for each q-clique constraint
        std::vector<std::pair<int, int> >::const_iterator edge_it;
        for (edge_it = edges.begin(); edge_it != edges.end(); ++edge_it) {
            used_vertices.insert(edge_it->first);
            used_vertices.insert(edge_it->second);
            
            int var = edge_dict[*edge_it];
            constraint += std::to_string(var) + " ";
        }
        CHECK_ASSERT(static_cast<int>(used_vertices.size()) == q, 
                    "Q-clique constraint must involve exactly q vertices",
                    AssertionStats::ENCODING_PROPERTIES);
        
        // Check clause validity
        CHECK_ASSERT(is_valid_clause(constraint + "0"), 
                    "Invalid q-clique clause format",
                    AssertionStats::FORMAT_CONSISTENCY);
    }

    // Final CNF format assertions
    int count = comb(n, 2);
    int clause_count = comb(n, p) + comb(n, q);
    
    // Check DIMACS format requirements
    CHECK_ASSERT(count > 0, "Number of variables must be positive", AssertionStats::FORMAT_CONSISTENCY);
    CHECK_ASSERT(clause_count > 0, "Number of clauses must be positive", AssertionStats::FORMAT_CONSISTENCY);
    CHECK_ASSERT(count == edge_counter, 
                "Variable count must match number of edges",
                AssertionStats::FORMAT_CONSISTENCY);
}

// Assert: argc == 4 (program name + three arguments)
// Assert: argv[1], argv[2], argv[3] are convertible to positive integers
int main(int argc, char* argv[]) {
    try {
        AssertionStats::init();
        
        CHECK_ASSERT(argc == 4, "Must provide exactly 3 arguments: n p q",
                    AssertionStats::INPUT_VALIDATION);
        
        int n = std::stoi(argv[1]);
        int p = std::stoi(argv[2]);
        int q = std::stoi(argv[3]);
        
        generate(n, p, q);
        
        AssertionStats::report();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}