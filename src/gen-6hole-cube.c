#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//#define VALIDATE
#define SBP

#define BITS		21
#define OFFSET		3

#define MASKA		21
#define MASKB		31

unsigned int orient (int a, int b, int c) {
  assert (a < b);
  assert (b < c);

  return a + (b-2)*(b-1)/2 + (c-3)*(c-2)*(c-1)/6;
}

unsigned int invalid (unsigned int bv, unsigned int bits) {
  int full = (1 << bits) - 1;

  int pos = 7;
  for (int j = 0; j < bits; j++)
    if (((pos << j) & bv) == (pos << j))
      return pos << j;

  int neg = 15;
  for (int j = 0; j < bits; j++)
    if (((neg << j) & (bv^full)) == (neg << j))
      return neg << j;

  return 0;
}

unsigned int mirror (unsigned int bv, unsigned int bits) {
  int result = 0;
  for (int i = 0; i < bits; i++)
    if (bv & (1 << i))
      result |= 1 << (bits - i - 1);

  return result;
}

// for debugging
void printMask (int bv, int bits) {
  for (int i = 0; i < bits; i++) {
    if ((1 << i) & bv) printf ("1");
    else               printf ("0"); }
}

void printCube (int *array, int size) {
#ifdef VALIDATE
  for (int i = 0; i < size; i++)
    printf ("%i ", -array[i]);
#else
  printf ("a ");
  for (int i = 0; i < size; i++)
    printf ("%i ", array[i]);
#endif
  printf ("0\n");
}

int main (int argc, char** argv) {
  int match = 0;
  if (argc > 1)
    match = atoi (argv[1]);
  int count = 1;
  int bits = BITS;
//  int bits = atoi (argv[1]);

  int offset = OFFSET;
//  if (argc > 2) offset = atoi (argv[2]);

  int *dudud, *cube, size;
  int allocSize = bits*2;
  dudud = (int *) malloc (sizeof (int) * allocSize);
  cube  = (int *) malloc (sizeof (int) * allocSize);

  for (int i = 0; i < allocSize; i++) dudud[i] = cube[i] = 0;

  int max = (1 << bits) - 1;

#ifdef VALIDATE
  printf ("p cnf %i %i\n", orient (bits+1, bits+2, bits+3), 0);
#endif

  int f = 1;
  int palin = 0;
  for (int i = 0; i <= max; i++) {
    size = 0;

    if (invalid (i, bits)) continue;

    if (i == mirror (i,bits)) palin++;

#ifdef SBP
    if (i > mirror (i, bits)) continue;
#endif
    unsigned int mask = max;

    int extra = 0;

    int j = i << 1;

    int c = 0;
    while (j) {
      if (((j ^ MASKA) & MASKB) == MASKB)
        dudud[extra++] = orient (c + offset + 1, c + offset + 3, c + offset + 5);
      j = j >> 1;
      c++;
    }

    for (int b = 0; b < bits; b++) {
      if (((1 << b) & mask) == 0) continue;
      if (((1 << b) & i   ) == 0)
        cube[size++] = -orient (b + offset + 2, b + offset + 3, b + offset + 4);
      else
        cube[size++] =  orient (b + offset + 2, b + offset + 3, b + offset + 4); }

      int m = (1 << extra) - 1;
      for (int k = 0; k <= m; k++) {
        size = bits;
        for (int e = 0; e < extra; e++) {
          if (((1 << e) & k   ) == 0)
           cube[size++] = -dudud[e];
          else
           cube[size++] =  dudud[e]; }

        if (match == 0 || count == match)
          printCube (cube, size);
        count++;
      }
  }

#ifdef VALIDATE
  int o = offset;
  for (int i = 2; i+1 <= bits; i++)
    printf ("%i %i %i 0\n", -orient (i+o, i+o+1, i+o+2), -orient (i+o+1, i+o+2, i+o+3), -orient (i+o+2, i+o+3, i+o+4));

  for (int i = 2; i+2 <= bits; i++)
    printf ("%i %i %i %i 0\n", orient (i+o, i+o+1, i+o+2), orient (i+o+1, i+o+2, i+o+3), orient (i+o+2, i+o+3, i+o+4), orient (i+o+3, i+o+4, i+o+5));
#endif

//  printf ("c palindromes: %i\n", palin);

}

