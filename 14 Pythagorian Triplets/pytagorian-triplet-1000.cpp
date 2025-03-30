// C++ program to generate pythagorean
// triplets smaller than a given limit
#include <cstdio>
#include <iostream>

extern "C"
{
#include "ipasir.h"
}

// Driver Code
int main()
{
  int limit = 1000;
  void *solver = ipasir_init();
  printf("Using the incremental SAT solver %s.\n", ipasir_signature());

  // Function to generate pythagorean
  // triplets smaller than limit

  // triplet: a^2 + b^2 = c^2
  int a, b, c = 0;

  // loop from 2 to max_limit
  int m = 2;

  // Limiting c would limit
  // all a, b and c
  while (c < limit)
  {

    // now loop on j from 1 to i-1
    for (int n = 1; n < m; ++n)
    {

      // Evaluate and print triplets using
      // the relation between a, b and c
      a = m * m - n * n;
      b = 2 * m * n;
      c = m * m + n * n;

      if (c > limit)
        break;

      printf("( %d %d %d ),", a, b, c);

      ipasir_add(solver, a);
      ipasir_add(solver, b);
      ipasir_add(solver, c);
      ipasir_add(solver, 0);

      ipasir_add(solver, -a);
      ipasir_add(solver, -b);
      ipasir_add(solver, -c);
      ipasir_add(solver, 0);
    }
    m++;
  }

  int res = ipasir_solve(solver);
  if (res == 10)
  {
    printf("SAT\n");
    int *model = (int *)malloc(sizeof(int) * limit);
    for (int i = 1; i <= limit; i++)
    {
      model[i] = ipasir_val(solver, i);
      if (model[i] == i)
      {
        printf("%d ", i);
      }
    }
    printf("\n");
  }
  else
  {
    printf("UNSAT\n");
  }

  return 0;
}
