/*
                        DSA_IDEATHON TITLE : Traveling Salesman Problem (TSP) Solver

This program solves the Traveling Salesman Problem (TSP), which is a classic optimization problem in computer science and operations research. 
In the TSP, a salesman must visit each of a given set of cities exactly once and return to the starting city, minimizing the total distance traveled.

The program implements two algorithms to solve the TSP:
1. Nearest Neighbor heuristic: Generates an initial solution by starting from a specified city and repeatedly visiting the nearest unvisited city until all cities have been visited.
2. Dynamic Programming: Refines the initial solution obtained from the Nearest Neighbor heuristic by exhaustively exploring all possible tours using dynamic programming techniques.

The input to the program is an adjacency matrix representing the distances between pairs of cities. 
The output is the optimal tour  and  its cost,  along with the execution time of the algorithms.

*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define MAX_VERTICES 20

int ary[MAX_VERTICES][MAX_VERTICES], n;
clock_t start, end;


void takeInput(FILE *inputFile);
void nearestNeighbor(int initialCity, int *tour);
int tspDynamicProgramming(int mask, int pos, int **dp);
void writeOutput(FILE *outputFile, int *tour, int optimalCost);


void takeInput(FILE *inputFile) {
    fscanf(inputFile, "%d", &n); 

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            fscanf(inputFile, "%d", &ary[i][j]);
    }
}


/* 
Nearest Neighbor heuristic to generate an initial solution:
- Initialize all vertices as not visited.
- Start from the initial city.
- Mark the initial city as visited.
- Add the initial city to the tour.
- Generate the tour using the nearest neighbor heuristic:
  - Find the nearest unvisited neighbor.
  - Update the current city, mark the nearest neighbor as visited, and update the total cost.
  - Add the nearest neighbor to the tour.
- Add the cost to return to the initial city.
*/
void nearestNeighbor(int initialCity, int *tour) {
    int visited[MAX_VERTICES] = {0}; 
    int current = initialCity; 
    visited[current] = 1; 
    tour[0] = current; 
    int totalCost = 0;
    for (int i = 1; i < n; i++) {
        int minDistance = INT_MAX;
        int nearestNeighbor = -1;
        for (int j = 0; j < n; j++) {
            if (!visited[j] && ary[current][j] < minDistance) {
                minDistance = ary[current][j];
                nearestNeighbor = j;
            }
        }
        current = nearestNeighbor;
        visited[current] = 1;
        tour[i] = current; 
        totalCost += minDistance;
    }
    totalCost += ary[current][initialCity];
}


/*
 Dynamic Programming to find the optimal tour
 If all vertices are visited, return to the initial city.
 Return the memoized value if available.
 Try all unvisited cities as next steps.
 If city i is not visited,
 recursively compute the cost of visiting city i and memoize the result.
*/
int tspDynamicProgramming(int mask, int pos, int **dp) {
    if (mask == (1 << n) - 1) 
        return ary[pos][0]; 

    if (dp[mask][pos] != -1)
        return dp[mask][pos];

    int minCost = INT_MAX;
    for (int i = 0; i < n; i++) {
        if (!(mask & (1 << i))) {
            int cost = ary[pos][i] + tspDynamicProgramming(mask | (1 << i), i, dp);
            if (cost < minCost)
                minCost = cost;
        }
    }
    return dp[mask][pos] = minCost; 
}


void writeOutput(FILE *outputFile, int *tour, int optimalCost) {
    fprintf(outputFile, "Optimal Tour: ");
    for (int i = 0; i < n; i++)
        fprintf(outputFile, "%d ", tour[i] + 1); // Convert to 1-based indexing
    fprintf(outputFile, "1\n");
    fprintf(outputFile, "Optimal Tour Cost: %d\n", optimalCost);
}

int main() {
    FILE *inputFile = fopen("input.txt", "r");
    if (inputFile == NULL) {
        printf("Error: Unable to open input file\n");
        return 1;
    }

    FILE *outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        printf("Error: Unable to open output file\n");
        fclose(inputFile);
        return 1;
    }

    takeInput(inputFile);
    fclose(inputFile);

    start = clock();

    // Step 1: Generate initial solution using Nearest Neighbor heuristic
    int *initialTour = (int *)malloc(n * sizeof(int));
    if (initialTour == NULL) {
        printf("Error: Memory allocation failed\n");
        fclose(outputFile);
        return 1;
    }
    nearestNeighbor(0, initialTour);

    // Step 2: Refine the initial solution using Dynamic Programming
    int **dp = (int **)malloc((1 << n) * sizeof(int *));
    if (dp == NULL) {
        printf("Error: Memory allocation failed\n");
        free(initialTour);
        fclose(outputFile);
        return 1;
    }
    for (int i = 0; i < (1 << n); i++) {
        dp[i] = (int *)malloc(n * sizeof(int));
        if (dp[i] == NULL) {
            printf("Error: Memory allocation failed\n");
            for (int j = 0; j < i; j++)
                free(dp[j]);
            free(dp);
            free(initialTour);
            fclose(outputFile);
            return 1;
        }
        for (int j = 0; j < n; j++)
            dp[i][j] = -1; // Initialize DP table
    }
    int optimalCost = tspDynamicProgramming(1, 0, dp);

    end = clock();
    double executionTime = ((double)(end - start)) / CLOCKS_PER_SEC;

    writeOutput(outputFile, initialTour, optimalCost);
    fprintf(outputFile, "Execution Time: %.6f seconds\n", executionTime);

    for (int i = 0; i < (1 << n); i++)
        free(dp[i]);
    free(dp);
    free(initialTour);

    fclose(outputFile);

    system("python graph.py");

    return 0;
}
