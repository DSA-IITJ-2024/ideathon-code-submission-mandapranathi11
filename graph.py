"""
1. The code reads in the cost matrix and optimal tour from files, making it easy to test different scenarios without manual input.
2. Utilizing NetworkX and Matplotlib.pyplot, the code visually represents cities and their connections, providing a clear depiction of the problem.
3. Utilised powerful libraries like NetworkX, Matplotlib.pyplot, and NumPy streamlines the implementation, simplifying graph creation, plotting, and numerical operations.
4. Color mapping adds a personal touch, enhancing the visual appeal of the graph and making it easier to interpret.
5. The code attempts to display the optimal tour on the graph, aiding in understanding the solution's path.
"""

import networkx as nx
import matplotlib.pyplot as plt
import numpy as np

def read_cost_matrix(file_path):
    with open(file_path, 'r') as file:
        n = int(file.readline())
        cost_matrix = []
        for _ in range(n):
            row = list(map(int, file.readline().split()))
            cost_matrix.append(row)
    return np.array(cost_matrix)

def read_optimal_tour(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()
        tour_line = [int(x) for x in lines[0].split()[2:]] 
    return tour_line


def visualize_graph(cost_matrix, tour=None):
    n = len(cost_matrix)
    G = nx.DiGraph()

    G.add_nodes_from(range(1, n + 1))

    for i in range(n):
        for j in range(n):
            if cost_matrix[i][j] > 0:
                G.add_edge(i + 1, j + 1, weight=cost_matrix[i][j])

    pos = nx.kamada_kawai_layout(G)

    node_colors = [G.degree(node) for node in G.nodes()]
    cmap = plt.get_cmap('RdYlBu')
    norm = plt.Normalize(min(node_colors), max(node_colors))
    node_colors = [cmap(norm(color)) for color in node_colors]

    edge_colors = [G[i][j]['weight'] for i, j in G.edges()]
    cmap = plt.get_cmap('RdYlBu')
    norm = plt.Normalize(min(edge_colors), max(edge_colors))
    edge_colors = [cmap(norm(color)) for color in edge_colors]

    nx.draw_networkx_nodes(G, pos, node_color=node_colors)
    nx.draw_networkx_edges(G, pos, edge_color=edge_colors)
    nx.draw_networkx_edge_labels(G, pos, edge_labels={(i, j): G[i][j]['weight'] for i, j in G.edges()})
    
    nx.draw_networkx_labels(G, pos, labels={node: node for node in G.nodes()})

    if tour:
        tour_edges = [(tour[i], tour[i + 1]) for i in range(len(tour) - 1)]
        nx.draw_networkx_edges(G, pos, edgelist=tour_edges, edge_color='red', width=2)

    plt.show()

input_file = "input.txt"
cost_matrix = read_cost_matrix(input_file)

output_file = "output.txt"
tour = read_optimal_tour(output_file)

# Visualize the graph without the optimal tour
visualize_graph(cost_matrix)

# Visualize the graph with the optimal tour
visualize_graph(cost_matrix, tour)
