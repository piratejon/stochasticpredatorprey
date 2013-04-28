#ifndef _AUTONOME_H
#define _AUTONOME_H

#include <stdio.h>

#define RGB_BARE 0x0022ff22
#define RGB_FOX 0x00ff2222
#define RGB_RABBIT 0x002222ff

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

double random_interval();
void clear_board (Object ** g);
void sprinkle(Object ** g, double init_rabbit, double init_fox);
void usage ( char * me );
void count_animals ( Object ** g, int * prabbits, int * pfoxes );
void * pick_one_of_three ( void * a, void * b, void * c );
void * pick_one_of_five ( void * a, void * b, void * c, void * d, void * e );
void * pick_one_of_eight ( void * a, void * b, void * c, void * d, void * e, void * f, void * g, void * h );
Object * choose_random_neighbor ( Object ** g, int x, int y );
_Bool probabilistic_event_happens ( char * s, double p );
void a_neighborly_interaction ( Object * a, Object * n, int x, int y );
void iterate ( Object ** g, double p_r_breed, double p_f_breed, double p_f_die );
int RED(Creature c);
int GREEN(Creature c);
int BLUE(Creature c);
void grid_to_raw(FILE * fp, Object ** g);

#endif // _AUTONOME_H

