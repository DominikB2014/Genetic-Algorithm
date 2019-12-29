#include "a4.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int compare(const void *s1, const void *s2) {
  Individual *e1 = (Individual *)s1;
  Individual *e2 = (Individual *)s2;
  double compared = e1->fitness - e2->fitness;

  if (fabs(compared) < 10e-9)
    return 0;
  else if (compared < 0)
    return -1;
  return 1;
}

PPM_IMAGE *evolve_image(const PPM_IMAGE *image, int num_generations,
                        int population_size, double rate) {
  time_t t;
  srand((unsigned)time(&t));

  Individual *individual = generate_population(population_size, image->width,
                                               image->height, image->max_color);
  comp_fitness_population(image->data, individual, population_size);
  qsort(individual, population_size, sizeof(Individual), compare);

  for (int i = 1; i <= num_generations; i++) {
    crossover(individual, population_size);
    mutate_population(individual, population_size, rate);
    comp_fitness_population(image->data, individual, population_size);
    qsort(individual, population_size, sizeof(Individual), compare);
    printf("Generation: %d Fitness: %.12e\n", i, individual[0].fitness);
  }

  PPM_IMAGE *ReturnImg = malloc(sizeof(PPM_IMAGE));
  ReturnImg->data=individual[0].image.data;
  ReturnImg->width=image->width;
  ReturnImg->height=image->height;
  ReturnImg->max_color=image->max_color;

  for (int i=1;i<population_size;i++){
    free(individual[i].image.data);
  }
  free(individual);
  
  return ReturnImg;
}

void free_image(PPM_IMAGE *p) {
  free(p->data);
  free(p);
}
