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

int main(int argc, char *argv[])
{
    void *solver;
    int n = 0, m = 0; // n = number nodes, m = number edges, k = number of colors

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Read the DIMACS file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    char line[256];
    int edgeIndex = 0;  // Keep track of the current edge index
    int **edges = NULL; // Declare edges array here
    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '\n' || line[0] == 'c')
        {
            continue;
        }
        if (line[0] == 'p')
        {
            printf("%s", line);
            char type[10];
            sscanf(line, "p %s %d %d", type, &n, &m);
            printf("Number of nodes: %d, Number of edges: %d\n", n, m);
            // Allocate memory for edges array
            edges = (int **)malloc(m * sizeof(int *));
            for (int i = 0; i < m; i++)
            {
                edges[i] = (int *)malloc(2 * sizeof(int));
            }
            continue;
        }
        if (line[0] == 'e')
        {
            int u, v;
            sscanf(line, "e %d %d", &u, &v);
            edges[edgeIndex][0] = u;
            edges[edgeIndex][1] = v;
            edgeIndex++;
        }
    }
    // // Print edges to verify correctness
    // printf("Edges:\n");
    // for (int i = 0; i < m; i++)
    // {
    //     printf("Edge %d: %d - %d\n", i + 1, edges[i][0], edges[i][1]);
    // }

    // Generate clauses for the graph coloring problem
    solver = ipasir_init();
    printf("Using the incremental SAT solver %s.\n", ipasir_signature());
    long inc = 0; // Incremental solving

    // Allocate memory for the color variable
    int **color = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        color[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
        {
            color[i][j] = ++inc;
        }
    }

    for (int k = 1; k <= n; k++)
    {
        // Generate clauses for the current k
        // Should generate new clauses for new k = k + 1

        // k = k + 1, each node has max 1 color
        // -color(i,k) v -color(i,1)
        // -color(i,k) v -color(i,2)
        // ...
        // -color(i,k) v -color(i,k-1)
        for (int i = 0; i < n; i++)
        {
            for (int j = 1; j <= k; j++)
            {
                ipasir_add(solver, -color[i][k]);
                ipasir_add(solver, color[i][j]);
                ipasir_add(solver, 0);
            }
        }

        // clauses for edges coloring:
        // -color(i,k) v -color(j,k) for each edge (i,j) for k = 1 to k
        // number of new clauses: m * k
        for (int i = 0; i < m; i++)
        {
            for (int j = 1; j <= k; j++)
            {
                ipasir_add(solver, -color[edges[i][0] - 1][j]);
                ipasir_add(solver, -color[edges[i][1] - 1][j]);
                ipasir_add(solver, 0);
            }
        }

        // each node has min 1 color
        // color(i,1) v color(i,2) v ... v color(i,k) v as(k)
        for (int i = 0; i < k; i++)
        {
            for (int j = 1; j <= k; j++)
            {
                ipasir_add(solver, color[i][j]);
            }
            // Add the assumption variable for the current k
            ipasir_add(solver, 0);
        }

        // Each loop will generate new clauses: (k for max 1 color, 1 for min 1 color + new assumption var) -> n times

        // Drop the assumption min 1 color in 1 -> k-1: as(k) = 1 for k = 1 to k - 1
        // Assume the assumption min 1 color in 1 -> k: as(k) = 0 for k = k
        // Solve -> If SAT -> k min, if UNSAT -> k = k + 1

        // Solve the SAT problem
        int res = ipasir_solve(solver);
        if (res == 10)
        {
            printf("SAT\n");
            break;
        }
        else
        {
            printf("UNSAT\n");
            k++;
        }
    }
    // // Print the result
    // if (res == 10)
    // {
    //     printf("SAT\n");
    //     for (int i = 1; i <= n; i++)
    //     {
    //         int color = ipasir_val(solver, i);
    //         if (color != 0)
    //         {
    //             printf("Node %d: Color %d\n", i, color);
    //         }
    //     }
    // }
    // else
    // {
    //     printf("UNSAT\n");
    // }
    for (int i = 0; i < m; i++)
    {
        free(edges[i]);
    }
    free(edges);
    fclose(file);
    return EXIT_SUCCESS;
}