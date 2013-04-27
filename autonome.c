#include <stdlib.h>
#include <stdio.h>

int WIDTH;
int HEIGHT;

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
      x = (double)WIDTH*(double)(rand()/(double)RAND_MAX);
      y = (double)HEIGHT*(double)(rand()/(double)RAND_MAX);
      if (g[x][y].c == BARE) {
        g[x][y].c = RABBIT;
        g[x][y].age = 0;
        break;
      }
    } while (1);
  }

  for (i = squares*init_fox; i > 0; -- i) {
    do {
      x = (double)WIDTH*(double)(rand()/(double)RAND_MAX);
      y = (double)HEIGHT*(double)(rand()/(double)RAND_MAX);
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

void iterate ( Object ** g, double p_r_breed, double p_f_breed, double p_f_die ) {
  int x, y;

  for (x = 0; x < WIDTH; x += 1) {
    for (y = 0; y < HEIGHT; y += 1) {
    }
  }
}

int main ( int arfc, char ** arfv ) {
  int i;
  int seed;

  double init_rabbit, init_fox, p_r_breed, p_f_breed, p_f_die;

  int iterations;

  if ( arfc != 10 ) usage(arfv[0]);

  getparams(arfv, &WIDTH, &HEIGHT, &seed, &init_rabbit, &init_fox, &p_r_breed, &p_f_breed, &p_f_die, &iterations);

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

