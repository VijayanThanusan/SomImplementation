#include "som.h"

double carre(double c){
  return c*c;
}

double normalise(double vec[4], int dim){
  double som = 0.0; int i;
  for(i=0; i<dim; i++)
  	som += carre(vec[i]);
  som = sqrt(som);

  for(i=0; i<dim; i++)
    vec[i] /= som;

  return som;
}

data_V* lecture(char *nom, struct data_V *data_vecteurs, int *nb_vec, int *dim){

  static char * lu;
  static size_t nlu;
  int nligne;
  const char sep[2] = ",\n";
  char *token;
  int j = 0;

  FILE* fichier = NULL;
  fichier = fopen(nom,"r");
  assert(fichier);

  getline(&lu, &nlu, fichier);
  *nb_vec = atoi(lu);
  lu = NULL;
  getline(&lu, &nlu, fichier);
  *dim = atoi(lu);
  lu = NULL;

  data_vecteurs = malloc(sizeof(data_vecteurs[0])* *nb_vec);

  for(nligne = 0; getline(&lu, &nlu, fichier) != -1; nligne += 1){
    data_vecteurs[nligne].vec = malloc(sizeof(data_vecteurs[0].vec[0])* *dim);

    token = strtok(lu,sep);
    for(j=0; j<*dim; j++){
      data_vecteurs[nligne].vec[j] = atof(token);
      token = strtok(NULL, sep);
    }

    data_vecteurs[nligne].norme = normalise(data_vecteurs[nligne].vec, *dim);
    data_vecteurs[nligne].nom = malloc(sizeof(data_vecteurs[0].nom[0])*strlen(token));
    strcpy(data_vecteurs[nligne].nom, token);

    lu = NULL;
  }
  free(lu);
  return data_vecteurs;
}
