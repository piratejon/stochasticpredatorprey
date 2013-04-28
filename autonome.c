#include <sys/stat.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "autonome.h"

double random_interval() {
  return (double)rand()/(double)RAND_MAX;
}

void clear_board (Object ** g) {
  int x, y;
  for (y = 0; y < HEIGHT; y += 1) {
    for (x = 0; x < WIDTH; x += 1) {
      g[x][y].c = BARE;
      g[x][y].age = 0;
    }
  }
}

void sprinkle(Object ** g, double init_rabbit, double init_fox) {
  int i, squares, x, y;

  squares = WIDTH*HEIGHT;

  for (i = squares*init_rabbit; i > 0; -- i) {
    do {
      x = (double)WIDTH*random_interval();
      y = (double)HEIGHT*random_interval();
      if (g[x][y].c == BARE) {
        g[x][y].c = RABBIT;
        g[x][y].age = 0;
        break;
      }
    } while (1);
  }

  for (i = squares*init_fox; i > 0; -- i) {
    do {
      x = (double)WIDTH*random_interval();
      y = (double)HEIGHT*random_interval();
      if (g[x][y].c == BARE) {
        g[x][y].c = FOX;
        g[x][y].age = 0;
        break;
      }
    } while (1);
  }
}

void usage ( char * me ) {
  printf("Usage: %s width height seed %%rabbit %%fox P(Rbreed) P(Fbreed) P(Fdie) iterations\n\tif seed is zero, the system time is substituted.", me);
  exit(1);
}

void count_animals ( Object ** g, int * prabbits, int * pfoxes ) {
  int x, y;
  *prabbits = 0;
  *pfoxes = 0;
  for (x = 0; x < WIDTH; x += 1) {
    for (y = 0; y < HEIGHT; y += 1) {
      if (g[x][y].c == RABBIT) *prabbits += 1;
      else if (g[x][y].c == FOX) *pfoxes += 1;
    }
  }
}

void * pick_one_of_three ( void * a, void * b, void * c ) {
  if ( a == b || b == c || a == c ) *(char*)(0) = 9;
  double r = random_interval();
  if ( r <= 1.0/3.0 ) return a;
  if ( r <= 2.0/3.0 ) return b;
  return c;
}

void * pick_one_of_five ( void * a, void * b, void * c, void * d, void * e ) {
  if ( a == b || a == c || a == d || a == e || b == c || b == d || b == e || c == d || c == e || d == e ) *(char*)(0) = 9;
  double r = random_interval();
  if ( r <= 1.0/5.0 ) return a;
  if ( r <= 2.0/5.0 ) return b;
  if ( r <= 3.0/5.0 ) return c;
  if ( r <= 4.0/5.0 ) return d;
  return e;
}

void * pick_one_of_eight ( void * a, void * b, void * c, void * d, void * e, void * f, void * g, void * h ) {
  if ( a == b || a == c || a == d || a == e || a == f || a == g || a == h ||
                 b == c || b == d || b == e || b == f || b == g || b == h ||
                           c == d || c == e || c == f || c == g || c == h ||
                                     d == e || d == f || d == g || d == h ||
                                               e == f || e == g || e == h ||
                                                         f == g || f == h ||
                                                                   g == h )
    *(char*)(0) = 9;

  double r = random_interval();
  if ( r <= 1.0/8.0 ) return a;
  if ( r <= 2.0/8.0 ) return b;
  if ( r <= 3.0/8.0 ) return c;
  if ( r <= 4.0/8.0 ) return d;
  if ( r <= 5.0/8.0 ) return e;
  if ( r <= 6.0/8.0 ) return f;
  if ( r <= 7.0/8.0 ) return g;
  return h;
}

Object * choose_random_neighbor ( Object ** g, int x, int y ) {
  /*
  Object * n[8];
  n[0] = &(g[x-1][y-1]);
  n[1] = &(g[x][y-1]);
  n[2] = &(g[x+1][y-1]);
  n[3] = &(g[x-1][y]);
  n[4] = &(g[x+1][y]);
  n[5] = &(g[x-1][y+1]);
  n[6] = &(g[x][y+1]);
  n[7] = &(g[x+1][y+1]);
  */

  /****
    A cell's neighbors:
    0 1 2
    3 x 4
    5 6 7
    ****/

  if ( x == 0 ) {
    if ( y == 0 ) {
      /***
        x 4
        6 7
        ***/
      return pick_one_of_three ( &(g[x+1][y]), &(g[x][y+1]), &(g[x+1][y+1]) );
    } else if ( y + 1 == HEIGHT ) {
      /***
        1 2
        x 4
        ***/
      return pick_one_of_three ( &(g[x][y-1]), &(g[x+1][y-1]), &(g[x+1][y]) );
    } else {
      /***
        1 2
        x 4
        6 7
        ***/
      return pick_one_of_five ( &(g[x][y-1]), &(g[x+1][y-1]), &(g[x+1][y]), &(g[x][y+1]), &(g[x+1][y+1]) );
    }
  } else if ( x + 1 == WIDTH ) {
    if ( y == 0 ) {
      /***
        3 x
        5 6
        ***/
      return pick_one_of_three ( &(g[x-1][y]), &(g[x-1][y+1]), &(g[x][y+1]) );
    } else if ( y + 1 == HEIGHT ) {
      /***
        0 1
        3 x
        ***/
      return pick_one_of_three ( &(g[x-1][y-1]), &(g[x][y-1]), &(g[x-1][y]) );
    } else {
      /***
        0 1
        3 x
        5 6
        ***/
      return pick_one_of_five ( &(g[x-1][y-1]), &(g[x][y-1]), &(g[x-1][y]), &(g[x-1][y+1]), &(g[x][y+1]) );
    }
  } else {
    if ( y == 0 ) {
      /***
        3 x 4
        5 6 7
        ***/
      return pick_one_of_five ( &(g[x-1][y]), &(g[x+1][y]), &(g[x-1][y+1]), &(g[x][y+1]), &(g[x+1][y+1]) );
    } else if ( y + 1 == HEIGHT ) {
      /***
        0 1 2
        3 x 4
        ***/
      return pick_one_of_five ( &(g[x-1][y]), &(g[x-1][y-1]), &(g[x][y-1]), &(g[x+1][y-1]), &(g[x+1][y]) );
    } else {
      /***
        0 1 2
        3 x 4
        5 6 7
        ***/
      return pick_one_of_eight ( &(g[x-1][y-1]), &(g[x][y-1]), &(g[x+1][y-1]), &(g[x-1][y]), &(g[x+1][y]), &(g[x-1][y+1]), &(g[x][y+1]), &(g[x+1][y+1]) );
    }
  }
}

_Bool probabilistic_event_happens ( char * s, double p ) {
  double r = random_interval();
  // printf("Checking probability of event %s: %.2f: %.2f -- %s\n", s, p, r, p >= r ? "Passes":"Fails" );
  return p >= r;
}

void a_neighborly_interaction ( Object * a, Object * n, int x, int y ) {
  if ( a->c == FOX ) {
    if ( n->c == RABBIT ) {
      if ( probabilistic_event_happens ( "Fox eats rabbit", p_f_breed ) ) {
        // printf("(%d,%d): Fox eats rabbit, breeds\n", x, y);
        n->c = FOX;
        n->age = 0;
      }
    } else {
      if ( probabilistic_event_happens ( "Fox dies", p_f_die ) ) {
        // printf("(%d,%d): Fox dies\n", x, y);
        a->c = BARE;
        a->age = 0;
      }
    }
  } else if ( a->c == RABBIT ) {
    if ( n->c == BARE && probabilistic_event_happens ( "Rabbit breeds", p_r_breed ) ) {
      // printf("(%d,%d): Rabbit breeds\n", x, y);
      n->c = RABBIT;
      n->age = 0;
    }
  } else {
    if ( n->c != BARE ) {
      // printf("(%d,%d): %s moves\n", x, y, n->c == RABBIT ? "Rabbit" : "Fox");
      a->c = n->c;
      a->age = n->age;
      n->c = BARE;
      n->age = 0;
    }
  }
}

void iterate ( Object ** g, double p_r_breed, double p_f_breed, double p_f_die ) {
  int x, y;

  for (x = 0; x < WIDTH; x += 1) {
    for (y = 0; y < HEIGHT; y += 1) {
      a_neighborly_interaction ( &(g[x][y]), choose_random_neighbor(g,x,y), x, y );
    }
  }
}

int RED(Creature c) {
  if ( c == BARE ) return (RGB_BARE & 0x00ff0000) >> 16;
  if ( c == FOX ) return (RGB_FOX & 0x00ff0000) >> 16;
  if ( c == RABBIT ) return (RGB_RABBIT & 0x00ff0000) >> 16;
  return 0;
}

int GREEN(Creature c) {
  if ( c == BARE ) return (RGB_BARE & 0x0000ff00) >> 8;
  if ( c == FOX ) return (RGB_FOX & 0x0000ff00) >> 8;
  if ( c == RABBIT ) return (RGB_RABBIT & 0x0000ff00) >> 8;
  return 0;
}

int BLUE(Creature c) {
  if ( c == BARE ) return (RGB_BARE & 0x000000ff);
  if ( c == FOX ) return (RGB_FOX & 0x000000ff);
  if ( c == RABBIT ) return (RGB_RABBIT & 0x000000ff);
  return 0;
}

void grid_to_raw(FILE * fp, Object ** g) {
/***
# ImageMagick pixel enumeration: 584,439,255,srgb
0,0: ( 22, 21, 27)  #16151B  srgb(22,21,27)
1,0: (  0,  0,  4)  #000004  srgb(0,0,4)
2,0: (  9,  8, 13)  #09080D  srgb(9,8,13)
3,0: (  0,  0,  0)  #000000  black
4,0: (  0,  1,  0)  #000100  srgb(0,1,0)
5,0: (  0,  1,  0)  #000100  srgb(0,1,0)
6,0: (  4,  6,  0)  #040600  srgb(4,6,0)
7,0: (  0,  2,  0)  #000200  srgb(0,2,0)
8,0: (  0,  2,  0)  #000200  srgb(0,2,0)
...

574,438: (  1,  6,  0)  #010600  srgb(1,6,0)
575,438: (  1,  6,  0)  #010600  srgb(1,6,0)
576,438: (  4,  6,  3)  #040603  srgb(4,6,3)
577,438: (  3,  3,  3)  #030303  grey1
578,438: (  9,  9,  9)  #090909  srgb(9,9,9)
579,438: (  0,  0,  0)  #000000  black
580,438: (  3,  3,  3)  #030303  grey1
581,438: ( 13, 13, 13)  #0D0D0D  grey5
582,438: (  0,  0,  0)  #000000  black
583,438: ( 22, 22, 22)  #161616  srgb(22,22,22)
***/
  int x, y;
  fprintf(fp, "# ImageMagick pixel enumeration: %d,%d,255,srgb\n", WIDTH, HEIGHT);
  for (y = 0; y < HEIGHT; y += 1) {
    for (x = 0; x < WIDTH; x += 1) {
      fprintf(fp, "%d,%d: (%3d,%3d,%3d)  #%02x%02x%02x srgb(%d,%d,%d)\n", x, y, RED(g[x][y].c), GREEN(g[x][y].c), BLUE(g[x][y].c), RED(g[x][y].c), GREEN(g[x][y].c), BLUE(g[x][y].c), RED(g[x][y].c), GREEN(g[x][y].c), BLUE(g[x][y].c));
    }
  }
}

