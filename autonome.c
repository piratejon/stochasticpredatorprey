#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int WIDTH;
int HEIGHT;

double p_r_breed, p_f_breed, p_f_die;

typedef enum _tag_creature {
  BARE = 0,
  RABBIT,
  FOX,
} Creature;

typedef struct _tag_object {
  Creature c;
  int age;
} Object;

Object ** grid;

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
  printf("Usage: %s width height seed %%rabbit %%fox P(Rbreed) P(Fbreed) P(Fdie) iterations\n", me);
  exit(1);
}

void getparams ( char ** arfv,
    int * pwidth, int * pheight,
    int * pseed, double * pinit_rabbit, double * pinit_fox, double * pp_r_breed,
    double * pp_f_breed, double * pp_f_die, int * piterations
    ) {
  int p = 1;
  *pwidth = strtol(arfv[p++],NULL,10);
  *pheight = strtol(arfv[p++],NULL,10);
  *pseed = strtol(arfv[p++],NULL,10);
  *pinit_rabbit = strtod(arfv[p++], NULL);
  *pinit_fox = strtod(arfv[p++], NULL);
  *pp_r_breed = strtod(arfv[p++], NULL);
  *pp_f_breed = strtod(arfv[p++], NULL);
  *pp_f_die = strtod(arfv[p++], NULL);
  *piterations = strtol(arfv[p++], NULL,10);
}

void print_summary ( Object ** g ) {
  int rabbits, foxes, x, y;
  rabbits = 0;
  foxes = 0;
  for (x = 0; x < WIDTH; x += 1) {
    for (y = 0; y < HEIGHT; y += 1) {
      if (g[x][y].c == RABBIT) rabbits += 1;
      else if (g[x][y].c == FOX) foxes += 1;
    }
  }
  printf("Rabbits: %5d, foxes: %5d\n", rabbits, foxes);
}

void * pick_one_of_three ( void * a, void * b, void * c ) {
  double r = random_interval();
  if ( r <= 1.0/3.0 ) return a;
  if ( r <= 2.0/3.0 ) return b;
  return c;
}

void * pick_one_of_five ( void * a, void * b, void * c, void * d, void * e ) {
  double r = random_interval();
  if ( r <= 1.0/5.0 ) return a;
  if ( r <= 2.0/5.0 ) return b;
  if ( r <= 3.0/5.0 ) return c;
  if ( r <= 4.0/5.0 ) return d;
  return e;
}

void * pick_one_of_eight ( void * a, void * b, void * c, void * d, void * e, void * f, void * g, void * h ) {
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
      return pick_one_of_three ( &(g[x+1][y]), &(g[x][y+1]), &(g[x+1][y+1]) );
    } else if ( y + 1 == HEIGHT ) {
      return pick_one_of_three ( &(g[x][y-1]), &(g[x+1][y-1]), &(g[x+1][y]) );
    } else {
      return pick_one_of_five ( &(g[x][y-1]), &(g[x+1][y-1]), &(g[x+1][y]), &(g[x][y+1]), &(g[x+1][y+1]) );
    }
  } else if ( x + 1 == WIDTH ) {
    if ( y == 0 ) {
      return pick_one_of_three ( &(g[x-1][y]), &(g[x-1][y+1]), &(g[x][y+1]) );
    } else if ( y + 1 == HEIGHT ) {
      return pick_one_of_three ( &(g[x-1][y-1]), &(g[x][y-1]), &(g[x-1][y]) );
    } else {
      return pick_one_of_five ( &(g[x-1][y-1]), &(g[x][y-1]), &(g[x-1][y]), &(g[x-1][y+1]), &(g[x][y+1]) );
    }
  } else {
    if ( y == 0 ) {
      return pick_one_of_five ( &(g[x-1][y]), &(g[x+1][y]), &(g[x-1][y+1]), &(g[x][y+1]), &(g[x+1][y+1]) );
    } else if ( y + 1 == HEIGHT ) {
      return pick_one_of_five ( &(g[x-1][y]), &(g[x-1][y-1]), &(g[x][y-1]), &(g[x+1][y-1]), &(g[x+1][y]) );
    } else {
      return pick_one_of_eight ( &(g[x-1][y-1]), &(g[x][y-1]), &(g[x+1][y-1]), &(g[x-1][y]), &(g[x+1][y]), &(g[x-1][y+1]), &(g[x][y+1]), &(g[x+1][y+1]) );
    }
  }
}

void a_neighborly_interaction ( Object * a, Object * n ) {
  if ( a->c == FOX ) {
    if ( n->c == RABBIT ) {
      if ( p_f_breed >= random_interval() ) {
        // printf("Fox eats rabbit, breeds\n");
        n->c = FOX;
        n->age = 0;
      }
    } else {
      if ( p_f_die >= random_interval() ) {
        // printf("Fox dies\n");
        a->c = BARE;
        a->age = 0;
      }
    }
  } else if ( a->c == RABBIT ) {
    if ( n->c == BARE && p_r_breed >= random_interval() ) {
      // printf("Rabbit breeds\n");
      n->c = RABBIT;
      n->age = 0;
    }
  } else {
    if ( n->c != BARE ) {
      // printf("%s moves\n", n->c == RABBIT ? "Rabbit" : "Fox");
      a->c = n->c;
      a->age = n->age;
      a->c = BARE;
      a->age = 0;
    }
  }
}

void iterate ( Object ** g, double p_r_breed, double p_f_breed, double p_f_die ) {
  int x, y;

  for (x = 0; x < WIDTH; x += 1) {
    for (y = 0; y < HEIGHT; y += 1) {
      // printf("Interacting at (%d,%d): ", x, y);
      a_neighborly_interaction ( &(g[x][y]), choose_random_neighbor(g,x,y) );
    }
  }
}

int main ( int arfc, char ** arfv ) {
  int i;
  int seed;

  double init_rabbit, init_fox;

  int iterations;

  if ( arfc != 10 ) usage(arfv[0]);

  getparams(arfv, &WIDTH, &HEIGHT, &seed, &init_rabbit, &init_fox, &p_r_breed, &p_f_breed, &p_f_die, &iterations);

  if (seed == 0) seed = time(NULL);

  printf("seed: %d\n", seed);
  srand(seed);

  grid = malloc((sizeof*grid)*WIDTH);

  for (i = 0; i < WIDTH; i += 1) {
    grid[i] = malloc((sizeof*grid[i])*HEIGHT);
  }

  clear_board(grid);
  sprinkle(grid, init_rabbit, init_fox);
  print_summary(grid);

  while ( iterations -- > 0 ) {
    iterate(grid, p_r_breed, p_f_breed, p_f_die);
    print_summary(grid);
  }

  for (i = 0; i < WIDTH; i += 1) {
    free(grid[i]);
  }
  free(grid);

  return 0;
}

