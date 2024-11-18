import itertools

def generate_ramsey_cnf(N, P, Q):
    # Number of edges in the complete graph K_N
    edges = list(itertools.combinations(range(1, N + 1), 2))
    num_edges = len(edges)
    
    # Create a mapping from edges to boolean variables
    edge_to_var = {edge: i + 1 for i, edge in enumerate(edges)}
    
    clauses = []
    
    # Generate clauses for K_P (red edges)
    for red_clique in itertools.combinations(range(1, N + 1), P):
        clause = [edge_to_var[(min(u, v), max(u, v))] for u, v in itertools.combinations(red_clique, 2)]
        clauses.append(clause)
    
    # Generate clauses for K_Q (blue edges)
    for blue_clique in itertools.combinations(range(1, N + 1), Q):
        clause = [-edge_to_var[(min(u, v), max(u, v))] for u, v in itertools.combinations(blue_clique, 2)]
        clauses.append(clause)
    
    # Prepare the DIMACS format output
    dimacs_output = []
    dimacs_output.append(f"p cnf {num_edges} {len(clauses)}")
    
    for clause in clauses:
        dimacs_output.append(" ".join(map(str, clause)) + " 0")
    
    return "\n".join(dimacs_output)

# Example usage
N = 4  # Number of vertices
P = 3  # Size of the first complete subgraph
Q = 4  # Size of the second complete subgraph
cnf_output = generate_ramsey_cnf(N, P, Q)
print(cnf_output)
