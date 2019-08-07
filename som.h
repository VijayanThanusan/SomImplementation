#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <time.h>

struct data_V{
  double *vec;
  double norme;
  char *nom;
};
typedef struct data_V data_V;

struct node{
  double *w;
  double act;
  char etiq;
};
typedef struct node node;

struct bmu{
	int bmu_l;
	int bmu_c;
	struct bmu *suiv;
};
typedef struct bmu bmu;

struct bmu_Hdr{
	int cpt;
	struct bmu *first;
	struct bmu *last;
};
typedef struct bmu_Hdr bmu_Hdr;

struct net_cfg{
	int nb_node;
	int ligne;
	int colonne;
};
typedef struct net_cfg net_cfg;

data_V* lecture(char *nom, struct data_V *data_vecteurs, int *nb_vec, int *dim);
double normalise(double vec[4], int dim);
double carre(double c);

double* gen_vecteur(double* vecteur_moyen, double max, double min, int dim);
double* vec_moyen(struct data_V *data_vecteurs, int nb_vec, int dim);
node **init_network(node** Net, double*vecteur_moyen,double max, double min, int dim, net_cfg N);
net_cfg configure_network(int nb_vec, net_cfg N);
void affiche(node** Net, net_cfg N);

double distance_euclid(double *v, double *w, int dim);
int rayon_vng(net_cfg N);
void shuffle(int *indice, int nb_vec);
int* init_tab_indice(int *indice, int nb_vec);
bmu *selectBMU(double* vecteur, int dim, node** Net, net_cfg N);
void neighborhood(node **Net, net_cfg N, int rayon, double alpha, bmu *winner, double* vec, int dim);
void apprentissage(data_V *data_vecteur,int nb_vec, int dim, node** Net, net_cfg N);