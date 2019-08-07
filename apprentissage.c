#include "som.h"

double distance_euclid(double *v, double *w, int dim){
  int i; double som=0.0;

  for(i=0; i<dim; i++)
    som+= carre(v[i] - w[i]);

  return sqrt(som);
}

int rayon_vng(net_cfg N){
  int rayon = 0;
  int nb_vng = 0; 

  while(nb_vng < N.nb_node *0.50){
    rayon++;
    nb_vng += 8 * rayon;
  }
  return rayon;
}

void shuffle(int *indice, int nb_vec){
  int randindice = 0;
  int i,tmp;

  for(i=0; i<nb_vec; i++){
    randindice = rand()%nb_vec;

    tmp = indice[i];
    indice[i] = indice[randindice];
    indice[randindice] = tmp;
  }
}

int* init_tab_indice(int *indice, int nb_vec){
  int i; 
  indice = malloc(sizeof(indice[0])*nb_vec);
  for(i=0; i<nb_vec; i++)
    indice[i] = i;
  return indice;
}

bmu *selectBMU(double* vecteur, int dim, node** Net, net_cfg N){
	int i,j, rand_bmu;
	bmu_Hdr header;
	header.cpt = 0; header.first = NULL; header.last = NULL;
	double min = 100.0, distance = 0;
	bmu *winner;

	for(i=0; i<N.ligne;i++){
		for(j=0;j<N.colonne;j++){
			distance = distance_euclid(vecteur, Net[i][j].w, dim);
			if(distance <= min){
				if(distance < min){
					free(header.first);
					header.first = NULL; header.last = NULL; header.cpt = 0;
				}

				bmu *best = malloc(sizeof(best[0]));
				best->bmu_l = i; best->bmu_c = j; best->suiv = NULL;

				if(header.first == NULL){
					header.first = best; header.last = best;
				}else{
					header.last->suiv = best; header.last = best;
				}
				header.cpt++;
				min = distance;
			}
		}
	}
	winner = header.first;
	
	if(header.cpt == 1)
		return winner;

	else if(header.cpt > 1){
		rand_bmu = rand()%header.cpt;
		while(rand_bmu){
			winner = winner->suiv;
			rand_bmu--;
		}
		return winner;
	}
	return NULL;
}

void neighborhood(node **Net, net_cfg N, int rayon, double alpha, bmu *winner, double* vec, int dim){
	int i,j,k,l,m;
	i = winner->bmu_l-rayon; if(i < 0) i = 0;
	j = winner->bmu_c-rayon; if(j < 0) j = 0;
	k = winner->bmu_l+rayon; if(k >= N.ligne) k = N.ligne-1;
	l = winner->bmu_c+rayon; if(l >= N.colonne) l = N.colonne-1;

	for(i=i; i<=k; i++){
		j = winner->bmu_c-rayon; if(j < 0) j = 0;
		for(j=j; j<=l; j++)
			for(m=0; m<dim; m++)
				Net[i][j].w[m] += alpha * (vec[m] - Net[i][j].w[m]);
	}
}

void apprentissage(data_V *data_vecteur,int nb_vec, int dim, node** Net, net_cfg N){
	bmu *winner;
	int i,rayon,it,nb_it = 500*nb_vec;
	double alpha_init = 0.7, rap;
	double alpha = alpha_init;

	int ord = (int)(nb_it*0.25);
	int aff = nb_it - ord;

	int *tab_indice = NULL;
	tab_indice = init_tab_indice(tab_indice, nb_vec);
	
	rayon = rayon_vng(N);

	for(it=0; it<=ord; it++){
		if(rayon > 1){
			if(it == (int)(ord*1/3)) rayon--;
			if(it == (int)(ord*2/3)) rayon--;
		}
		shuffle(tab_indice, nb_vec);
		for(i=0; i<nb_vec; i++){
			winner = selectBMU(data_vecteur[tab_indice[i]].vec, dim,Net,N);
			Net[winner->bmu_l][winner->bmu_c].etiq = data_vecteur[tab_indice[i]].nom[10];
			neighborhood(Net, N, rayon, alpha, winner,data_vecteur[tab_indice[i]].vec, dim);
			rap = (double)it/(double)ord;
			alpha = alpha_init*(1 - rap);
		}
	}
	printf("Fin phase ordonnancement %d / %d ~> alpha %f ~> %f\n",ord,nb_it,alpha_init,alpha);

	alpha_init /= 10;
	alpha = alpha_init;
	rayon = 1;
	for(it=0; it<=aff; it++){
		shuffle(tab_indice,nb_vec);
		for(i=0; i<nb_vec; i++){
			winner = selectBMU(data_vecteur[tab_indice[i]].vec, dim,Net,N);
			Net[winner->bmu_l][winner->bmu_c].etiq = data_vecteur[tab_indice[i]].nom[10];
			neighborhood(Net, N, rayon, alpha, winner,data_vecteur[tab_indice[i]].vec, dim);
			rap = (double)it / (double) aff;
			alpha = alpha_init*(1-(rap));
		}
	}
	printf("Fin phase affinage %d / %d ~> alpha %f ~> %f\n",aff,nb_it,alpha_init,alpha);


}