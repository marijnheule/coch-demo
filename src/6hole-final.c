#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SIZE		30

#define BASE
#define SBP

#define ACDBELOW	0
#define ADEBELOW	1
#define ACDABOVE	2
#define ADEABOVE	3

#define AFLINE		0
#define AFBELOW		1
#define AFABOVE		2

int n;

int orient (int a, int b, int c) {
  assert (a < b);
  assert (b < c);

  return a + (b-2)*(b-1)/2 + (c-3)*(c-2)*(c-1)/6;
}

int line (int a, int b, int d, int aux) {
  assert (a < b);

  int rel = a + (b-2)*(b-1)/2;

  return aux + 3*rel + d;
}

int def (int a, int b, int c, int d, int aux) {
  assert (a < b);
  assert (b < c);

  int rel = a + (b-2)*(b-1)/2 + (c-3)*(c-2)*(c-1)/6;
  return aux + 4*rel + d;
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
#ifdef IMPLIED
  nCls += (n-3) * (n-2) * (n-1) * n / 12;              // implied hole relation
//  nCls += (n-3) * (n-2) * (n-1) * n / 12;              // implied hole relation
#endif

  nVar += 3 * n * (n-1) / 2;
  nVar += 2 * n * (n-1) * (n-2) / 3;

  for (int a = 2; a <= n; a++)
    for (int f = a+5; f <= n; f++) {
      nCls += (f-a-1)*(f-a-2);
      nCls += 4 * (f-a-1); }

  // three or four points below, a == 1 clauses are satisfied by SBP
  for (int a = 2; a <= n; a++) {
    for (int d = a+3; d+1 <= n; d++)
      for (int c = a+2; c < d; c++)
        nCls += c - a - 1; // def odd

    for (int e = a+4; e <= n; e++)
      for (int d = a+3; d < e; d++)
        nCls += d - a - 2;

    for (int e = a+4; e < n; e++)
      for (int d = a+3; d < e; d++)
        nCls += n - e; // no extend f
  }

  // three or four points above
  for (int a = 1; a <= n; a++) {
    for (int d = a+3; d+1 <= n; d++)
      for (int c = a+2; c < d; c++)
        nCls += c - a - 1; // def odd

    for (int e = a+4; e <= n; e++)
      for (int d = a+3; d < e; d++)
        nCls += d - a - 2;

    for (int e = a+4; e < n; e++)
      for (int d = a+3; d < e; d++)
        nCls += n - e; // no extend f
  }

  printf ("p cnf %i %i\n", nVar, nCls);
#ifdef SBP
  // symmetry breaking predicate
  for (int c = 2; c <= n; c++)
    for (int b = 2; b < c; b++)
      printf ("%i 0\n", orient (1, b, c));
#endif
#ifdef BASE
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
#endif
  int aux = 1 + n * (n-1) * (n-2) / 6;
  aux += n * (n-1) * (n-2) * (n-3) / 12; // inside variables
  aux += n * (n-1) * (n-2) / 6;          // hole variables

  int xxx = aux + 3*n*(n-1)/2;

  // two points below, two points above, a == 1 clauses are satisfied by SBP
  for (int a = 2; a <= n; a++)
    for (int f = a+5; f <= n; f++) { // f = a+4 would be only redundant
      for (int b = a+1; b < f; b++)
        for (int c = b+1; c < f; c++) {
          printf ("%i %i %i %i %i 0\n", -line(a,f,AFLINE,aux),  orient (a,b,c),  orient (b,c,f), -hole (a,b,c), -hole (a,c,f));
          printf ("%i %i %i %i %i 0\n",  line(a,f,AFLINE,aux), -orient (a,b,c), -orient (b,c,f), -hole (a,b,c), -hole (a,c,f)); }
    }

  for (int a = 2; a <= n; a++)
    for (int f = a+5; f <= n; f++) {
      for (int b = a+1; b < f; b++) {
        printf ("%i %i 0\n",    -line(a,f,AFBELOW,aux), -def (a,b,f,ADEBELOW,xxx));
        printf ("%i %i %i 0\n",  line(a,f,AFBELOW,aux), -orient (a,b,f), -hole (a,b,f)); }
   }

  for (int a = 2; a <= n; a++)
    for (int f = a+5; f <= n; f++) {
      for (int b = a+1; b < f; b++) {
        printf ("%i %i 0\n",    -line(a,f,AFABOVE,aux), -def (a,b,f,ADEABOVE,xxx));
        printf ("%i %i %i 0\n",  line(a,f,AFABOVE,aux), orient (a,b,f), -hole (a,b,f)); }
    }

  aux = xxx;
  /*** three or four points below, a == 1 clauses are satisfied by SBP ***/

  // four points with 4-hole {a,b,c,d}
  for (int a = 2; a <= n; a++)
    for (int d = a+3; d+1 <= n; d++)
      for (int c = a+2; c < d; c++)
        for (int b = a+1; b < c; b++)
          printf ("%i %i %i %i %i 0\n", def (a,c,d,ACDBELOW,aux), orient (a,b,c), orient (b,c,d), -hole (a,b,c), -hole (a,c,d));

  // five points with 5-hole {a,*,c,d,e}
  for (int a = 2; a <= n; a++)
    for (int e = a+4; e <= n; e++) // e could be n
      for (int d = a+3; d < e; d++)
        for (int c = a+2; c < d; c++)
          printf ("%i %i %i %i 0\n", def (a,d,e,ADEBELOW,aux), orient (c,d,e), -hole (a,d,e), -def (a,c,d,ACDBELOW,aux));

  // cannot extend 5-hole with a point f > e
  for (int a = 2; a <= n; a++)
    for (int e = a+4; e < n; e++) // e could not be n
      for (int d = a+3; d < e; d++)
        for (int f = e+1; f <= n; f++)
          printf ("%i %i %i 0\n", -def (a,d,e,ADEBELOW,aux),  orient (d,e,f), -hole (a,e,f));

  /*** three or four points below ***/

  // four points with 4-hole {a,b,c,d}
  for (int a = 1; a <= n; a++)
    for (int d = a+3; d+1 <= n; d++)
      for (int c = a+2; c < d; c++)
        for (int b = a+1; b < c; b++)
          printf ("%i %i %i %i %i 0\n", def (a,c,d,ACDABOVE,aux), -orient (a,b,c), -orient (b,c,d), -hole (a,b,c), -hole (a,c,d));

  // five points with 5-hole {a,*,c,d,e}
  for (int a = 1; a <= n; a++)
    for (int e = a+4; e <= n; e++) // e could be n
      for (int d = a+3; d < e; d++)
        for (int c = a+2; c < d; c++)
          printf ("%i %i %i %i 0\n", def (a,d,e,ADEABOVE,aux), -orient (c,d,e), -hole (a,d,e), -def (a,c,d,ACDABOVE,aux));

  // cannot extend 5-hole with a point f > e
  for (int a = 1; a <= n; a++)
    for (int e = a+4; e < n; e++) // e could not be n
      for (int d = a+3; d < e; d++)
        for (int f = e+1; f <= n; f++)
          printf ("%i %i %i 0\n", -def (a,d,e,ADEABOVE,aux), -orient (d,e,f), -hole (a,e,f));
}


