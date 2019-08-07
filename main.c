#include "som.h"

int main(int argc, char **argv){
  srand(time(NULL));
  
  int nb_vec, dim;
  data_V *data_vecteurs = NULL;
  node **Network = NULL;
  double *vecteur = NULL;
  net_cfg N;

  data_vecteurs = lecture("iris.data",data_vecteurs,&nb_vec,&dim);
  vecteur = vec_moyen(data_vecteurs, nb_vec, dim);
  N = configure_network(nb_vec,N);

  Network = init_network(Network,vecteur,0.05,0.02,dim,N);
  apprentissage(data_vecteurs, nb_vec, dim, Network, N);
  affiche(Network,N);

  return 0;
}