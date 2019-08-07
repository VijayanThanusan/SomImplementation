#include "som.h"


double* gen_vecteur(double* vecteur_moyen, double max, double min, int dim){
  int i;
  double* vecteur = malloc(sizeof(vecteur[0])*dim);
  double coord = 0.0;

  for(i=0; i<dim; i++){ 
    coord = vecteur_moyen[i];
    vecteur[i] = ( (coord+max) - (coord-min)) * ((double)rand()/RAND_MAX) + (coord-min);
  }
  return vecteur;
}

double* vec_moyen(struct data_V *data_vecteurs, int nb_vec, int dim){
  int i,j;
  double *vecteur = malloc(sizeof(vecteur[0])*dim);
	
  for (i = 0; i < dim; i++)
    vecteur[i] = 0;

  for (j = 0; j < dim; j++){
    for(i=0; i<nb_vec; i++)
      vecteur[j] += data_vecteurs[i].vec[j];
    vecteur[j] /= i;
  }

  return vecteur;
}

node **init_network(node** Net, double*vecteur_moyen,double max, double min, int dim, net_cfg N){
  int i,j;
  Net = malloc(sizeof(Net[0])*N.ligne);

  for(i=0; i<N.ligne; i++)
    Net[i] = malloc(sizeof(Net[i][0])*N.colonne);

  for(i=0; i<N.ligne; i++)    
    for(j=0; j<N.colonne; j++)
      Net[i][j].w = gen_vecteur(vecteur_moyen,max,min,dim);

  return Net;
}

net_cfg configure_network(int nb_vec, net_cfg N){
  double or = 1.6; /*Nombre d'OR 1.61803398875 */
  int nb_node_max = 5*sqrt(nb_vec);
  float colonne = 0,colonne_max = 0, ligne = 0, ligne_max = 0;

  while(colonne_max * ligne_max <= nb_node_max){
    colonne = colonne_max;
    ligne = ligne_max;

    ligne_max++;
    colonne_max = ligne_max * or;
  }

  N.colonne = (int)(colonne+0.5);
  N.ligne= (int)(ligne+0.5);
  N.nb_node = N.ligne * N.colonne;

  return N;
}

void affiche(node** Net, net_cfg N){
  int i,j;

  for(i=0;i<N.ligne; i++){
    for(j=0; j<N.colonne; j++)
      if(Net[i][j].etiq != 0)
        printf("%c ", Net[i][j].etiq);
      else
        printf(". ");
    printf("\n");
  }
}
