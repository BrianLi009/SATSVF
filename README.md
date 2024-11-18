# SATSVF:

We want to use the SVF tool to check if the LLM agent generated ramsey encoder works correct.

# Prompt: 

https://compute.hyper.space

```
produce a pipeline that generate an encoding in conjunctive normal form for the ramsey problem in DIMACS format. The code should take in 3 inputs: n, p, q, representing ramsey graph R(p,q) on graph of order n. The code should be in c++.

Each boolean variable should represent an edge in the graph.
```

# SVF

https://websvf.vercel.app