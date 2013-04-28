#include <sys/stat.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "autonome.h"

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

int main ( int arfc, char ** arfv ) {
  int i;
  int seed;

  double init_rabbit, init_fox;

  int iterations;
  int rabbits, foxes;

  if ( arfc != 10 ) usage(arfv[0]);

  getparams(arfv, &WIDTH, &HEIGHT, &seed, &init_rabbit, &init_fox, &p_r_breed, &p_f_breed, &p_f_die, &iterations);

  if (seed == 0) seed = time(NULL);

  // printf("sizeof(Object): %d\n", sizeof(Object));
  printf("seed: %d\n", seed);
  printf("Probability of rabbit breeding: %.2f\n", p_r_breed);
  printf("Probability of fox eating rabbit/breeding: %.2f\n", p_f_breed);
  printf("Probability of fox dying: %.2f\n", p_f_die);
  srand(seed);

  char dirname[512] = {0};
  char filename[512] = {0};
  sprintf(dirname, "%d-%d-%d-%f-%f-%f-%f-%f", WIDTH, HEIGHT, seed, init_rabbit, init_fox, p_r_breed, p_f_breed, p_f_die);
  mkdir(dirname, 0755);

  grid = malloc((sizeof*grid)*WIDTH);

  for (i = 0; i < WIDTH; i += 1) {
    grid[i] = malloc((sizeof*grid[i])*HEIGHT);
  }

  clear_board(grid);
  sprinkle(grid, init_rabbit, init_fox);
  count_animals(grid, &rabbits, &foxes);
  printf("%8d: Rabbits: %5d, foxes: %5d\n", 0, rabbits, foxes);

  for (i = 0; i < iterations; i+=1 ) {
    iterate(grid, p_r_breed, p_f_breed, p_f_die);
    sprintf(filename, "%s/%05d.txt", dirname, i);
    FILE * raw = fopen(filename, "w");
    grid_to_raw(raw, grid);
    fclose(raw);
    count_animals(grid, &rabbits, &foxes);
    printf("%8d: Rabbits: %5d, foxes: %5d\n", i, rabbits, foxes);
    if ( rabbits == 0 || foxes == 0 ) break;
  }

  for (i = 0; i < WIDTH; i += 1) {
    free(grid[i]);
  }
  free(grid);

  return 0;
}

