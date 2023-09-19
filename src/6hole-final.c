#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SIZE		30

int n;

int orient (int a, int b, int c) {
  assert (a < b);
  assert (b < c);

  return a + (b-2)*(b-1)/2 + (c-3)*(c-2)*(c-1)/6;
}

int inside (int x, int a, int b, int c) {
  assert (a <  b);
  assert (b <  c);
  assert (a <  x);
  assert (x <  c);
  assert (x != b);

  int l = x, r = b;
  if (l > r) { l = b; r = x; }

  int base = n * (n-1) * (n-2) / 6;
  int more = 2 * (a + (l-2)*(l-1)/2 + (r-3)*(r-2)*(r-1)/6 + (c-4)*(c-3)*(c-2)*(c-1)/24) + -1 * (x < b);

  return base + more;
}

int hole (int a, int b, int c) {
  assert (a < b);
  assert (b < c);

  int base = n * (n-1) * (n-2) / 6 + n * (n-1) * (n-2) * (n-3) / 12;

  return base + a + (b-2)*(b-1)/2 + (c-3)*(c-2)*(c-1)/6;
}

void printInside (int d, int t, int abx, int bcx, int acx) {
  printf ("%i %i %i 0\n", -d,  t, -abx);
  printf ("%i %i %i 0\n", -d,  t, -bcx);
  printf ("%i %i %i 0\n", -d,  t, -acx);

  printf ("%i %i %i 0\n", -d, -t,  abx);
  printf ("%i %i %i 0\n", -d, -t,  bcx);
  printf ("%i %i %i 0\n", -d, -t,  acx);
}

void defineInsideA (int x, int a, int b, int c) {
  assert (a < x);
  assert (x < b);
  assert (b < c);

  int d = inside (x, a, b, c);
  int t = orient (a, b, c);
  int axb = -orient (a, x, b);
  int xbc =  orient (x, b, c);
  int axc =  orient (a, x, c);

  printInside (d, t, axb, xbc, axc);
}

void defineInsideB (int x, int a, int b, int c) {
  assert (a < b);
  assert (b < x);
  assert (x < c);

  int d = inside (x, a, b, c);
  int t = orient (a, b, c);
  int abx =  orient (a, b, x);
  int bxc = -orient (b, x, c);
  int axc =  orient (a, x, c);

  printInside (d, t, abx, bxc, axc);
}

int main (int argc, char** argv) {
  n = SIZE;

  if (argc > 1)
    n = atoi (argv[1]);

  int nVar = n * (n-1) * (n-2) / 6;                    // base variables
  nVar += n * (n-1) * (n-2) * (n-3) / 12;              // inside variables
  nVar += n * (n-1) * (n-2) / 6;                       // hole variables

  int nCls = (n-1) * (n-2) / 2;                        // unit clauses
  nCls += (n-3) * (n-2) * (n-1) * (n-4) / 6;           // sign clauses
  nCls += n * (n-1) * (n-2) * (n-3) / 2;               // inside clauses wo blocked
  nCls += n * (n-1) * (n-2) / 6;                       // hole clauses wo blocked

  // all four points above
  for (int a = 2; a <= n; a++)
    for (int c = a+2; c <= n; c++)
      for (int d = c+1; d+2 <= n; d++) {
        nVar++;
        for (int b = a+1; b < c; b++)
          nCls++;
        for (int e = d+1; e <= n; e++)
          for (int f = e+1; f <= n; f++)
            nCls++; }

  // all four points below
  for (int a = 1; a <= n; a++)
    for (int c = a+2; c <= n; c++)
      for (int d = c+1; d+2 <= n; d++) {
        nVar++;
        for (int b = a+1; b < c; b++)
          nCls++;
        for (int e = d+1; e <= n; e++)
          for (int f = e+1; f <= n; f++)
            nCls++; }

  for (int a = 2; a <= n; a++)
    for (int f = a+5; f <= n; f++) {
      nVar += 3;

      // three points below/above, one point above/below
      for (int b = a+1; b < f; b++)
        nCls += 2;
      for (int b = a+1; b < f; b++)
        for (int c = b+1; c < f; c++)
          for (int d = c+1; d < f; d++)
            nCls += 2;

      // two points below, two points above
      for (int b = a+1; b < f; b++)
        for (int c = b+1; c < f; c++) {
          nCls += 2; } }

  printf ("p cnf %i %i\n", nVar, nCls);

  // symmetry breaking predicate
  for (int c = 2; c <= n; c++)
    for (int b = 2; b < c; b++)
      printf ("%i 0\n", orient (1, b, c));

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

  // define inside variables
  for (int d = 1; d <= n; d++)
    for (int c = 1; c < d; c++)
      for (int b = 1; b < c; b++)
        for (int a = 1; a < b; a++) {
          defineInsideA (b, a, c, d);
          defineInsideB (c, a, b, d); }

  // define hole variables
  for (int c = 1; c <= n; c++)
    for (int b = 1; b < c; b++)
      for (int a = 1; a < b; a++) {
        printf ("%i ", hole (a, b, c));
        for (int x = a+1; x < c; x++)
          if (x != b) printf ("%i ", inside (x, a, b, c));
        printf ("0\n");
      }

  int aux = 1 + n * (n-1) * (n-2) / 6;
  aux += n * (n-1) * (n-2) * (n-3) / 12; // inside variables
  aux += n * (n-1) * (n-2) / 6;          // hole variables

  // all four points above, a == 1 clauses satisfied
  for (int a = 2; a <= n; a++)
    for (int c = a+2; c <= n; c++)
      for (int d = c+1; d+2 <= n; d++) {
        for (int b = a+1; b < c; b++)
            printf ("%i %i %i %i %i 0\n",  aux, orient (a,b,c), orient (b,c,d), -hole (a,b,c), -hole (a,c,d));
        for (int e = d+1; e <= n; e++)
          for (int f = e+1; f <= n; f++)
            printf ("%i %i %i %i %i 0\n", -aux, orient (c,d,e), orient (d,e,f), -hole (a,d,e), -hole (a,e,f));
        aux++; }

  // all four points below
  for (int a = 1; a <= n; a++)
    for (int c = a+2; c <= n; c++)
      for (int d = c+1; d+2 <= n; d++) {
        for (int b = a+1; b < c; b++)
            printf ("%i %i %i %i %i 0\n",  aux, -orient (a,b,c), -orient (b,c,d), -hole (a,b,c), -hole (a,c,d));
        for (int e = d+1; e <= n; e++)
          for (int f = e+1; f <= n; f++)
            printf ("%i %i %i %i %i 0\n", -aux, -orient (c,d,e), -orient (d,e,f), -hole (a,d,e), -hole (a,e,f));
        aux++; }

  for (int a = 2; a <= n; a++)
    for (int f = a+5; f <= n; f++) { // f = a+4 would be only redundant

      // three points below, one point above
      for (int b = a+1; b < f; b++)
        printf ("%i %i %i 0\n", aux, -orient (a,b,f), -hole (a,b,f));
      for (int b = a+1; b < f; b++)
        for (int c = b+1; c < f; c++)
          for (int d = c+1; d < f; d++)
            printf ("%i %i %i %i %i %i %i 0\n", -aux, orient (a,b,c), orient (b,c,d), orient (c,d,f),
                                                      -hole (a,b,c), -hole (a,c,d), -hole (a,d,f));
      aux++;

      // three points above, one point below
      for (int b = a+1; b < f; b++)
        printf ("%i %i %i 0\n", aux, orient (a,b,f), -hole (a,b,f));
      for (int b = a+1; b < f; b++)
        for (int c = b+1; c < f; c++)
          for (int d = c+1; d < f; d++)
            printf ("%i %i %i %i %i %i %i 0\n", -aux, -orient (a,b,c), -orient (b,c,d), -orient (c,d,f),
                                                      -hole (a,b,c), -hole (a,c,d), -hole (a,d,f));
      aux++;

      // two points below, two points above
      for (int b = a+1; b < f; b++)
        for (int c = b+1; c < f; c++) {
          printf ("%i %i %i %i %i 0\n", -aux,  orient (a,b,c),  orient (b,c,f), -hole (a,b,c), -hole (a,c,f));
          printf ("%i %i %i %i %i 0\n",  aux, -orient (a,b,c), -orient (b,c,f), -hole (a,b,c), -hole (a,c,f)); }
      aux++;
    }
}


