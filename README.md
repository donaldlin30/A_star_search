# A* 8-Puzzle Solver with Novel Heuristic

This repository contains an implementation of the A* search algorithm to solve the 8-puzzle problem. We've introduced a new heuristic in our approach to enhance the search efficiency.

## Contents

- `a_star_search.cpp`: Main code containing the A* search algorithm and the logic for generating all possible simulations.
- `h_1_summary.txt`, `h_2_summary.txt`, `h_3_summary.txt`: Text files containing data from our experiments for each heuristic.
- `paper.pdf`: Our research paper detailing the methodology, heuristics used, and results.
- `parser.cpp`: Supplementary code for parsing and data handling.

## Heuristics

1. **h1**: Number of misplaced tiles.
2. **h2**: Manhattan distance (sum of the distances of the tiles from their goal positions).
3. **h3**: Our novel heuristic that builds upon the Manhattan distance by considering specific tile arrangements.

## Results

Our experiments yielded significant insights into the efficiency and effectiveness of the new heuristic in solving the 8-puzzle problem. Detailed results, including comparisons with traditional heuristics, can be found in `paper.pdf`.

For a quick summary:
- `h_1_summary.txt`: Results using the h1 heuristic.
- `h_2_summary.txt`: Results using the h2 heuristic.
- `h_3_summary.txt`: Results using our novel h3 heuristic.
