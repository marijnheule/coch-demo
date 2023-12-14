#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SIZE		30

#define BASE
#define HOLE
#define SBP			// redundant symmetry-breaking clauses
#define ALTAXIOM
#define REFLECT

//#define EXPLICIT

#define CAP		0
#define CUP		1
#define CAPF		2

int n;

int orient (int a, int b, int c) {
  assert (a < b);
  assert (b < c);

  return a + (b-2)*(b-1)/2 + (c-3)*(c-2)*(c-1)/6;
}

int def (int a, int b, int c, int d, int aux) {
  assert (a < b);
  assert (b < c);

  int size = 3;

  int rel = a + (b-2)*(b-1)/2 + (c-3)*(c-2)*(c-1)/6;
  return aux + size*rel + d;
}

int line (int a, int b, int d, int aux) {
  assert (a < b);

  int rel = a + (b-2)*(b-1)/2;

  return aux + 3 * n * (n-1) * (n-2) / 6 + 2*rel + d;
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

void printInside (int d, int t, int i, int j) {
  printf ("%i %i %i 0\n", -d,  t, -i);
  printf ("%i %i %i 0\n", -d,  t, -j);
  printf ("%i %i %i 0\n", -d, -t,  i);
  printf ("%i %i %i 0\n", -d, -t,  j);
}

void defineInsideA (int x, int a, int b, int c) {
  assert (a < x);
  assert (x < b);
  assert (b < c);

  int d = inside (x, a, b, c);
  int t = orient (a, b, c);
  int axb = -orient (a, x, b);
  int axc =  orient (a, x, c);

  printInside (d, t, axb, axc);
}

void defineInsideB (int x, int a, int b, int c) {
  assert (a < b);
  assert (b < x);
  assert (x < c);

  int d = inside (x, a, b, c);
  int t = orient (a, b, c);
  int axc =  orient (a, x, c);
  int bxc = -orient (b, x, c);

  printInside (d, t, axc, bxc);
}

void printExplicitSix (int holeFlag) {
  for (int a = 2; a <= n; a++)
    for (int b = a + 1; b <= n; b++)
      for (int c = b + 1; c <= n; c++)
        for (int d = c + 1; d <= n; d++)
          for (int e = d + 1; e <= n; e++) {
            if (holeFlag) printf ("%i ",   -hole   (a,c,e));
            printf ("%i %i %i 0\n",        -orient (a,b,c), -orient (b,c,d), -orient (c,d,e));
            for (int f = e + 1; f <= n; f++) {
              if (holeFlag) printf ("%i ", -hole   (a,c,e));
              printf ("%i %i %i %i 0\n",    orient (a,b,c),  orient (b,c,d),  orient (c,d,e),  orient (d,e,f));
              if (holeFlag) printf ("%i ", -hole   (a,c,f));
              printf ("%i %i %i %i 0\n",    orient (a,b,c),  orient (b,c,d),  orient (c,d,f), -orient (a,e,f));
              if (holeFlag) printf ("%i ", -hole   (a,c,f));
              printf ("%i %i %i %i 0\n",    orient (a,b,c),  orient (b,c,e),  orient (c,e,f), -orient (a,d,f));
              if (holeFlag) printf ("%i ", -hole   (a,d,f));
              printf ("%i %i %i %i 0\n",    orient (a,b,d),  orient (b,d,e),  orient (d,e,f), -orient (a,c,f));
              if (holeFlag) printf ("%i ", -hole   (a,d,f));
              printf ("%i %i %i %i 0\n",    orient (a,c,d),  orient (c,d,e),  orient (d,e,f), -orient (a,b,f));
              if (holeFlag) printf ("%i ", -hole   (a,c,e));
              printf ("%i %i %i %i 0\n",    orient (a,b,c),  orient (b,c,f), -orient (a,d,e), -orient (d,e,f));
              if (holeFlag) printf ("%i ", -hole   (a,c,e));
              printf ("%i %i %i %i 0\n",   -orient (a,b,c), -orient (b,c,f),  orient (a,d,e),  orient (d,e,f));
              if (holeFlag) printf ("%i ", -hole   (a,d,e));
              printf ("%i %i %i %i 0\n",    orient (a,b,d),  orient (b,d,f), -orient (a,c,e), -orient (c,e,f));
              if (holeFlag) printf ("%i ", -hole   (a,d,e));
              printf ("%i %i %i %i 0\n",   -orient (a,b,d), -orient (b,d,f),  orient (a,c,e),  orient (c,e,f));
              if (holeFlag) printf ("%i ", -hole   (a,d,e));
              printf ("%i %i %i %i 0\n",    orient (a,c,d),  orient (c,d,f), -orient (a,b,e), -orient (b,e,f));
              if (holeFlag) printf ("%i ", -hole   (a,d,e));
              printf ("%i %i %i %i 0\n",   -orient (a,c,d), -orient (c,d,f),  orient (a,b,e),  orient (b,e,f)); } }
}

int main (int argc, char** argv) {
  n = SIZE;

  int holeFlag = 1;
#ifndef HOLE
  holeFlag = 0;
#endif

  if (argc > 1)
    n = atoi (argv[1]);

  int nVar = n * (n-1) * (n-2) / 6;              // base variables
#ifdef HOLE
  nVar += n * (n-1) * (n-2) * (n-3) / 12;        // inside variables
  nVar += n * (n-1) * (n-2) / 6;                 // hole variables
#endif
  int nCls = (n-1) * (n-2) * (n-3) * (n-4) / 12; // axiom clauses
#ifndef ALTAXIOM
  nCls += (n-1) * (n-2) * (n-3) * (n-4) / 12;    // axiom clauses
#else
  for (int a = 2; a <= n; a++)
    for (int c = a+2; c <= n; c++)
      for (int d = c+1; d <= n; d++) nCls += 2;  // axiom clauses
#endif
#ifdef HOLE
  nCls += (n-1) * (n-2) * (n-3) * (n-4) / 3;     // inside clauses wo blocked
  nCls += (n-1) * (n-2) * (n-3) / 6;             // hole clauses wo blocked
#endif
#ifdef SBP
  nCls += (n-1) * (n-2) / 2;                     // unit clauses
#endif
#ifdef REFLECT
  nCls += 1 << ((n-3)/2);
  nCls--;
#endif

#ifndef EXPLICIT
  int size = 3;
  nVar += size * n * (n-1) * (n-2) / 6;
#endif
#ifdef EXPLICIT
  for (int a = 2; a <= n; a++)
    for (int b = a + 1; b <= n; b++)
      for (int c = b + 1; c <= n; c++)
        for (int d = c + 1; d <= n; d++)
          for (int e = d + 1; e <= n; e++) {
            nCls++;
            for (int f = e + 1; f <= n; f++) nCls += 11; }
#else
  for (int a = 2; a <= n; a++)
    for (int b = a+1; b <= n; b++)
      for (int c = b+1; c <= n; c++)
        for (int d = c+1; d <= n; d++) {
          nCls += 2;
          if (b < a+2) continue;
          nCls += 2;
          if (b < a+3) continue;
          nCls += 1;
        }

  for (int a = 2; a <= n; a++)
    for (int c = a+2; c <= n; c++)
      for (int b = a+1; b < c; b++)
        for (int p = a+1; p < c; p++) {
          if (b == p) continue; // prevents BVA to do the optimization
          if ((b >= a+2) && (p >= a+2)) nCls++;
          if (b < a+3) continue;
          nCls += 1;
        }
#endif

  printf ("p cnf %i %i\n", nVar, nCls);

#ifdef SBP
  // symmetry breaking predicate
  for (int a = 2; a <= n; a++)
    for (int b = a+1; b <= n; b++)
      printf ("%i 0\n", orient (1, a, b));
#endif
#ifdef REFLECT
  // could be implemented without the arrays
  int reflect = (n-3)/2;
  int* midl = (int *) malloc (sizeof(int) * reflect);
  int* midr = (int *) malloc (sizeof(int) * reflect);
  for (int i = 0; i < reflect; i++) {
    midl[reflect-i-1] =  orient (  2+i,  3+i,4+i);
    midr[reflect-i-1] = -orient (n-2-i,n-1-i,n-i); }

  for (int i = 0; i < reflect; i++)
    for (int j = 0; j < (1 << i); j++) {
      for (int k = 0; k < i; k++)
        if ((1<<k) & j) printf ("%i ", midl[k]);
        else            printf ("%i ", midr[k]);
      printf ("%i %i 0\n", midl[i], midr[i]); }

  free (midl);
  free (midr);
#endif
#ifdef BASE
  // single sign swap clauses
  for (int a = 2; a <= n; a++)
    for (int b = a + 1; b <= n; b++)
      for (int c = b + 1; c <= n; c++)
        for (int d = c + 1; d <= n; d++) {
          printf ("%i %i %i 0\n",  orient (a, b, c), -orient (a, b, d),  orient (a, c, d)); // P1
          printf ("%i %i %i 0\n", -orient (a, b, c),  orient (a, b, d), -orient (a, c, d)); // P2
#ifndef ALTAXIOM
          printf ("%i %i %i 0\n",  orient (a, b, c), -orient (a, c, d),  orient (b, c, d)); // P3
          printf ("%i %i %i 0\n", -orient (a, b, c),  orient (a, c, d), -orient (b, c, d)); // P4
#endif
        }
 #ifdef HOLE
  // define inside variables
  for (int a = 2; a <= n; a++)
    for (int b = a+1; b <= n; b++)
      for (int c = b+1; c <= n; c++)
       for (int d = c+1; d <= n; d++) {
          defineInsideA (b, a, c, d);
          defineInsideB (c, a, b, d); }

  // define hole variables
  for (int a = 2; a <= n; a++)
    for (int b = a+1; b <= n; b++)
      for (int c = b+1; c <= n; c++) {
        printf ("%i ", hole (a, b, c));
        for (int x = a+1; x < c; x++)
          if (x != b) printf ("%i ", inside (x, a, b, c));
        printf ("0\n"); }
 #endif
#endif
#ifdef EXPLICIT
  printExplicitSix (holeFlag);
#else

  int aux = 1 + n * (n-1) * (n-2) / 6;
#ifdef HOLE
  aux += n * (n-1) * (n-2) * (n-3) / 12; // inside variables
  aux += n * (n-1) * (n-2) / 6;          // hole variables
#endif

  for (int a = 2; a <= n; a++)
    for (int b = a+1; b <= n; b++)
      for (int c = b+1; c <= n; c++)
        for (int d = c+1; d <= n; d++) {
          printf ("%i %i %i 0\n",       def (a,c,d,CAP ,aux),  orient (a,b,c),  orient (b,c,d));
          printf ("%i %i %i 0\n",       def (a,c,d,CUP ,aux), -orient (a,b,c), -orient (b,c,d));
          if (b < a+2) continue;
          if (holeFlag) printf ("%i ", -hole (a,b,d));
          printf ("%i %i %i 0\n",       def (a,c,d,CAPF,aux),  orient (b,c,d), -def (a,b,c,CAP ,aux));
          if (holeFlag) printf ("%i ", -hole (a,b,d));
          printf ("%i %i 0\n",         -def (a,b,c,CUP ,aux), -orient (b,c,d));  // three below
          if (b < a+3) continue;
          printf ("%i %i 0\n",         -def (a,b,c,CAPF,aux),  orient (b,c,d));  // four above
        }

#ifdef ALTAXIOM
  // efficiently express axioms
  for (int a = 2; a <= n; a++)
    for (int c = a+2; c <= n; c++)
      for (int d = c+1; d <= n; d++) {
        printf ("%i %i 0\n", -def (a,c,d,CAP,aux), -orient (a,c,d));
        printf ("%i %i 0\n", -def (a,c,d,CUP,aux),  orient (a,c,d)); }
#endif

  for (int a = 2; a <= n; a++)
    for (int c = a+2; c <= n; c++)
      for (int b = a+1; b < c; b++)
        for (int p = a+1; p < c; p++) {
          if (b == p) continue;
          if ((b >= a+2) && (p >= a+2)) {
            if (b < p) { if (holeFlag) printf ("%i ", -hole (a,b,p)); }
            else       { if (holeFlag) printf ("%i ", -hole (a,p,b)); }
            printf ("%i %i 0\n", -def (a,b,c,CUP ,aux), -def (a,p,c,CAP ,aux));   // two above, two below
          }
          if (b < a+3) continue;
          printf ("%i %i 0\n",   -def (a,b,c,CAPF,aux), -orient (a,p,c));         // three above, one below
       }
#endif
}
