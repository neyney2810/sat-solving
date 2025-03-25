#include "ipasir.h"

#include <stdio.h>
#include <stdlib.h>

//  Lösung des Graphfärbungsproblems per inkrementellem SAT-Solving unter Verwendung des IPASIR-Interfaces
//  auf der Kommandozeile ein Argument entgegennehmen, den Dateinamen einer DIMACS-Graph-Datei
//  soll dann die kleinste Zahl gefunden werden
//  für die eine Färbung der Knoten des Graphen existiert
//  so dass benachbarte Knoten unterschiedliche Farben annehmen
//  Ausgabe des Programms soll die Anzahl der benötigen Farben
//  sowie für jeden Knoten dessen Farbe.

// Read dimacs file for number of edges
// Divide edges into smaller groups -> how many groups? first 1 group
// Each loop will generate new clauses: (k for max 1 color, 1 for min 1 color + new assumption var) -> n times

// clauses for edges coloring:
// -color(i,k) v -color(j,k) for each edge (i,j) for k = 1 to k
// number of new clauses: m * k

// Drop the assumption min 1 color in 1 -> k-1: as(k) = 1 for k = 1 to k - 1
// Assume the assumption min 1 color in 1 -> k: as(k) = 0 for k = k
// Solve -> If SAT -> k min, if UNSAT -> k = k + 1

void generateClauses(const int n, const int k, const int sVar)
{
    // Should generate new clauses for new k = k + 1
    int vertexCount;
    int edgeCount;
    for (size_t i = 1; i < n; i++)
    {
        /* code */
        // k = k + 1, each node has max 1 color
        // -color(i,k) v -color(i,1)
        // -color(i,k) v -color(i,2)
        // ...
        // -color(i,k) v -color(i,k-1)

        // each node has min 1 color
        // color(i,1) v color(i,2) v ... v color(i,k) v as(k)
    }
}

int main(int argc, char *argv[])
{
    void *solver;
    int n = 0, m = 0, k = 0; // n = number nodes, m = number edges, k = number of colors

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    solver = ipasir_init();

    // map color
    int n = 0; // Known number of rows
    int k;     // k incremental

    // Allocate memory for the 2D array
    int **array = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        array[i] = (int *)malloc(k * sizeof(int));
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
        if (line[0] == '\n' || line[0] == 'c')
        {
            continue;
        }
    }
    fclose(file);
    return EXIT_SUCCESS;
}