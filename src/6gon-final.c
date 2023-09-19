#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SIZE	17

int orient (int a, int b, int c) {
  assert (a < b);
  assert (b < c);

  return a + (b-2)*(b-1)/2 + (c-3)*(c-2)*(c-1)/6;
}

int main (int argc, char** argv) {
  int n = SIZE;

  if (argc > 1) n = atoi (argv[1]);

  int nVar = n * (n-1) * (n-2) / 6;                            // base variables

  int nCls = 4 * (n-3) * (n-2) * (n-1) * (n-4) / 24;           // sign clauses
  nCls += (n-1) * (n-2) / 2;                                   // unit clauses, redundant in this encoding
  nCls += (n-1) * (n-2) * (n-3) * (n-4) * (n-5) / 120;

  // 6gon: count the number of clauses and aux variables
  for (int c = 4; c <= n; c++)
    for (int d = c+1; d+2 <= n; d++) {
      if (c == 4 || d+2 == n) {
        for (int a = 2; a < c; a++)
          for (int b = a+1; b < c; b++)
            for (int e = d+1; e <= n; e++)
              for (int f = e+1; f <= n; f++)
                nCls++;
        continue; }
      for (int a = 2; a < c; a++)
        for (int b = a+1; b < c; b++)
          nCls++;
      for (int e = d+1; e <= n; e++)
        for (int f = e+1; f <= n; f++)
          nCls++;
      nVar++; }

  for (int a = 2; a <= n; a++)
    for (int f = a+5; f <= n; f++) {
      nVar += 2;
      for (int b = a+1; b < f; b++) {
        nCls++;
        for (int c = b+1; c < f; c++) {
          nCls += 2;
          for (int d = c+1; d < f; d++)
            nCls++; } } }

  printf ("p cnf %i %i\n", nVar, nCls);

  // single sign swap clauses
  for (int a = 2; a <= n; a++)
    for (int b = a + 1; b <= n; b++)
      for (int c = b + 1; c <= n; c++)
        for (int d = c + 1; d <= n; d++) {
          printf ("%i %i %i 0\n",  orient (a, b, c), -orient (a, b, d),  orient (a, c, d)); // P1
          printf ("%i %i %i 0\n", -orient (a, b, c),  orient (a, b, d), -orient (a, c, d)); // P2
          printf ("%i %i %i 0\n",  orient (a, b, c), -orient (a, c, d),  orient (b, c, d)); // P3
          printf ("%i %i %i 0\n", -orient (a, b, c),  orient (a, c, d), -orient (b, c, d)); // P4
        }

  // symmetry breaking predicate
  for (int c = 2; c <= n; c++)
    for (int b = 2; b < c; b++)
      printf ("%i 0\n", orient (1, b, c));

  int aux = 1 + n * (n-1) * (n-2) / 6;

  // print constraints for a == 1
  for (int b = 2; b <= n; b++)
    for (int c = b+1; c <= n; c++)
      for (int d = c + 1; d <= n; d++)
        for (int e = d + 1; e <= n; e++)
          for (int f = e + 1; f <= n; f++)
            printf ("%i %i %i 0\n", -orient (b, c, d), -orient (c, d, e), -orient (d, e, f));

  // only positive orient literals, start with a == 2
  for (int c = 4; c <= n; c++)
    for (int d = c+1; d+2 <= n; d++) {
      // special cases c == 4 or d+2 == n -> no new variable
      if (c == 4) { // a == 2 and b == 3
        for (int e = d+1; e <= n; e++)
          for (int f = e+1; f <= n; f++)
            printf ("%i %i %i %i 0\n", orient (2,3,c), orient (3,c,d), orient (c,d,e), orient (d,e,f));
        continue; }
      if (d+2 == n) { // e == n-1 and f == n
        for (int a = 2; a < c; a++)
          for (int b = a+1; b < c; b++)
            printf ("%i %i %i %i 0\n", orient (a,b,c), orient (b,c,d), orient (c,d,n-1), orient (d,n-1,n));
        continue; }
      for (int a = 2; a < c; a++)
        for (int b = a+1; b < c; b++)
          printf ("%i %i %i 0\n",  aux, orient (a,b,c), orient (b,c,d));
      for (int e = d+1; e <= n; e++)
        for (int f = e+1; f <= n; f++)
          printf ("%i %i %i 0\n", -aux, orient (c,d,e), orient (d,e,f));
      aux++; }

  // exactly one negative orient literal
  for (int a = 2; a <= n; a++)
    for (int f = a+5; f <= n; f++) {

      // three points above, one point below
      for (int b = a+1; b < f; b++) {
        printf ("%i %i 0\n", aux, -orient (a,b,f));
        for (int c = b+1; c < f; c++)
          for (int d = c+1; d < f; d++)
            printf ("%i %i %i %i 0\n", -aux, orient (a,b,c), orient (b,c,d), orient (c,d,f)); }
      aux++;

      // two points above, two points below
      for (int b = a+1; b < f; b++)
        for (int c = b+1; c < f; c++) {
          printf ("%i %i %i 0\n", -aux,  orient (a,b,c),  orient (b,c,f));
          printf ("%i %i %i 0\n",  aux, -orient (a,b,c), -orient (b,c,f)); }
      aux++;
    }
}
