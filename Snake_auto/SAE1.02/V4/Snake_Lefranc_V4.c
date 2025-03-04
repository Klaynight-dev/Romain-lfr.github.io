/**
* \page Général
*
* \author LEFRANC Romain | GUILLOU Malo
* \version 4
* \date 28 janvier 2025
*
* Ce programme permet de déplacer automatiquement un serpent dans le terminal en affichant
* des bordures et plusieurs Pavés a éviter et des pommes a manger pour gagner
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>

/*****************************************************
*               Définition des Constantes          *
*****************************************************/

/**
* \def TAILLE
* \brief définition de la taille initiale du serpent
*/
#define TAILLE 10

/**
* \def X
* \brief définition de la cordonnés X
*/
#define X 40

/**
* \def Y1
* \brief définition de la cordonnés Y1
*/
#define Y1 13

/**
* \def Y2
* \brief définition de la cordonnés Y2
*/
#define Y2 27

/**
* \def TETE1
* \brief définition du caractére utiliser pour affciher une TETE1
*/
#define TETE1 '1'

/**
* \def TETE2
* \brief définition du caractére utiliser pour affciher une TETE2
*/
#define TETE2 '2'

/**
* \def CORPS
* \brief définition du caractére utiliser pour affciher un CORPS
*/
#define CORPS 'X'

/**
* \def FIN
* \brief définition du caractere qui provoquera l'action FIN
*/
#define FIN 'a'
/**
* \def HAUT
* \brief définition du caractere qui provoquera l'action HAUT
*/
#define HAUT 'z'

/**
* \def BAS
* \brief définition du caractere qui provoquera l'action BAS
*/
#define BAS 's'

/**
* \def GAUCHE
* \brief définition du caractere qui provoquera l'action GAUCHE
*/
#define GAUCHE 'q'

/**
* \def DROITE
* \brief définition du caractere qui provoquera l'action DROITE
*/
#define DROITE 'd'

/**
* \def LARGEUR
* \brief définition de la largeur du tableaux
*/
#define LARGEUR 81

/**
* \def HAUTEUR
* \brief définition de la hauteur du tableaux
*/
#define HAUTEUR 41

/**
* \def BORDURE
* \brief définition du caractére utiliser pour affciher une BORDURE
*/
#define BORDURE '#'

/**
* \def VIDE
* \brief définition du caractére utiliser pour affciher un VIDE
*/
#define VIDE ' '

/**
* \def POMME
* \brief définition du caractére utiliser pour affciher une POMME
*/
#define POMME '6'

/**
* \def NB_POMME
* \brief définition du nombre de POMME a manger pour finir
*/
#define NB_POMMES 10

/**
* \def MAX_PAVE
* \brief définition du nombre de PAVE maximum
*/
#define MAX_PAVE 6

/**
* \def PAVE
* \brief définition de la taille d'un PAVE
*/
#define PAVE 5

/**
* \def TEMPORISATION
* \brief définition de la vitesse du serpent au lancement
*/
#define TEMPORISATION 200000


#define DISTANCE_PORTAIL_COTE 22
#define DISTANCE_PORTAIL_HAUTEUR 11

#define NB_PAVES 6
#define TAILLE_PAVE 5
// définition d'un type pour le plateau : tPlateau
// Attention, pour que les indices du tableau 2D (qui commencent à 0) coincident
// avec les coordonées à l'écran (qui commencent à 1), on ajoute 1 aux dimensions
// et on neutralise la ligne 0 et la colonne 0 du tableau 2D (elles ne sont jamais
// utilisées)
typedef char tPlateau[LARGEUR+1][HAUTEUR+1];

int lesPommesX[NB_POMMES] = {40, 75, 78, 2, 9, 78, 74, 2, 72, 5};
int lesPommesY[NB_POMMES] = {20, 38, 2, 2, 5, 38, 32, 38, 32, 2};
int lesPavesX[NB_PAVES] = { 4, 73, 4, 73, 38, 38};
int lesPavesY[NB_PAVES] = { 4, 4, 33, 33, 14, 22};
int lesX[TAILLE];
int lesY[TAILLE];
int tabX[TAILLE];
int tabY[TAILLE];

void initPlateau(tPlateau plateau);
void initPave(tPlateau plateau, int lesPavesX[NB_PAVES], int lesPavesY[NB_PAVES]);
void dessinerPlateau(tPlateau plateau);
void ajouterPomme(tPlateau plateau, int nbPommes);
void afficher(int, int, char);
void effacer(int x, int y);
void dessinerSerpent(int lesX[], int lesY[]);
void dessinerSerpent2(int tabX[], int tabY[]);
void progresser(int lesX[], int lesY[], int *direction, tPlateau plateau, bool * collision, bool * pomme, int nbPommes);
void progresser2(int tabX[], int tabY[], int *direction, tPlateau plateau, bool * collision, bool * pomme, int nbPommes);
bool estPositionValide(tPlateau plateau, int x, int y, int taille);
bool estPositionValide2(tPlateau plateau, int x, int y, int taille);

// Fonctions et procédures non changés

void gotoxy(int x, int y);
int kbhit();
void disable_echo();
void enable_echo();


int main(){
	// 2 tableaux contenant les positions des éléments qui constituent le serpent

	// représente la touche frappée par l'utilisateur : touche de direction ou pour l'arrêt
	char touche;
	int direction1 = DROITE;
	int direction2 = GAUCHE;
	// le plateau de jeu
	tPlateau lePlateau;

	bool collision=false;
	bool gagne = false;
	bool pommeMangee = false;

	// compteur de pommes mangées
	int nbPommes = 0;
    int mouv1 = 0;
	int mouv2 = 0;
	int pom1 = 0;
	int pom2 = 0;
	// initialisation de la position du serpent : positionnement de la
	// tête en (X, Y), puis des anneaux à sa gauche
	for(int i=0 ; i<TAILLE ; i++){
		lesX[i] = X-i;
		lesY[i] = Y1;
	}

	for(int i=0 ; i<TAILLE ; i++){
		tabX[i] = X-i;
		tabY[i] = Y2;
	}

	// mise en place du plateau
	initPlateau(lePlateau);
	initPave(lePlateau, lesPavesX, lesPavesY);
	system("clear");
	dessinerPlateau(lePlateau);

	srand(time(NULL));
	ajouterPomme(lePlateau, nbPommes);

	// initialisation : le serpent se dirige vers la DROITE
	dessinerSerpent(lesX, lesY);

	dessinerSerpent2(lesX, lesY);
	disable_echo();
	touche = ' ';
	int clock_t_begin=clock();
	// boucle de jeu. Arret si touche STOP, si collision avec une bordure ou
	// si toutes les pommes sont mangées
	do {
		progresser(lesX, lesY, &direction1, lePlateau, &collision, &pommeMangee, nbPommes);
		mouv1++;
		if (pommeMangee){
            nbPommes++;
			pom1++;
			gagne = (nbPommes==NB_POMMES);
			if (!gagne){
				ajouterPomme(lePlateau, nbPommes);
				pommeMangee = false;
			}	
		}
		progresser2(tabX, tabY, &direction2, lePlateau, &collision, &pommeMangee, nbPommes);
		mouv2++;
		if (pommeMangee){
            nbPommes++;
			pom2++;
			gagne = (nbPommes==NB_POMMES);
			if (!gagne){
				ajouterPomme(lePlateau, nbPommes);
				pommeMangee = false;
			}	
		}
		if (!gagne){
			if (!collision){
				usleep(TEMPORISATION);
				if (kbhit()==1){
					touche = getchar();
				}
			}
		}
	} while (touche != FIN && !collision && !gagne);
	gotoxy(1, HAUTEUR+1);
	int clock_t_end=clock();
    double tmpsCPU = ((clock_t_end-clock_t_begin)*1.0) / CLOCKS_PER_SEC;
    printf("Temps CPU : %f secondes\n", tmpsCPU);
	printf("Nombre de mouvement du serpent 1 : %d\n", mouv1);
	printf("Nombre de mouvement du serpent 2 : %d\n", mouv2);
	printf("Nombre de pommes mangées : %d (serpent 1 : %d, serpent 2 : %d)\n", nbPommes, pom1, pom2);
	enable_echo();
	return EXIT_SUCCESS;
}


/************************************************/
/*		FONCTIONS ET PROCEDURES DU JEU 			*/
/************************************************/
void initPlateau(tPlateau plateau){
	// initialisation du plateau avec des espaces
	for (int i=1 ; i<=LARGEUR ; i++){
		for (int j=1 ; j<=HAUTEUR ; j++){
			plateau[i][j] = VIDE;
		}
	}
	// Mise en place la bordure autour du plateau
	// première ligne
	for (int i=1 ; i<=LARGEUR ; i++){
		if(i==LARGEUR/2){
			plateau[i][1] = VIDE;
		}
		else{
			plateau[i][1] = BORDURE;
		}
	}
	// lignes intermédiaires
	for (int j=1 ; j<=HAUTEUR ; j++){
		if(j==HAUTEUR/2){
			plateau[1][j] = VIDE;
			plateau[LARGEUR][j] = VIDE;			
		}
		else{
			plateau[1][j] = BORDURE;
			plateau[LARGEUR][j] = BORDURE;
		}
	}
	// dernière ligne
	for (int i=1 ; i<=LARGEUR ; i++){
		if(i==LARGEUR/2){
			plateau[i][HAUTEUR] = VIDE;
		}
		else{
			plateau[i][HAUTEUR] = BORDURE;
		}
	}
}

void initPave(tPlateau plateau, int pavesX[NB_PAVES], int pavesY[NB_PAVES]){

	for(int i=0 ; i<NB_PAVES; i++){
		for(int j=0 ; j<TAILLE_PAVE;j++){
			for(int k=0 ; k<TAILLE_PAVE; k++){
				plateau[pavesX[i]+j][pavesY[i]+k] = '#';
			}
		}
	}
}

void dessinerPlateau(tPlateau plateau){
	// affiche eà l'écran le contenu du tableau 2D représentant le plateau
	for (int i=1 ; i<=LARGEUR ; i++){
		for (int j=1 ; j<=HAUTEUR ; j++){
			afficher(i, j, plateau[i][j]);
		}
	}
}

void ajouterPomme(tPlateau plateau , int nbPommes){
	// génère aléatoirement la position d'une pomme,
	// vérifie que ça correspond à une case vide
	// du plateau puis l'ajoute au plateau et l'affiche
    plateau[lesPommesX[nbPommes]][lesPommesY[nbPommes]] = POMME;
    afficher(lesPommesX[nbPommes], lesPommesY[nbPommes], POMME);
}

void afficher(int x, int y, char car){
	gotoxy(x, y);
	printf("%c", car);
	gotoxy(1,1);
}

void effacer(int x, int y){
	gotoxy(x, y);
	printf(" ");
	gotoxy(1,1);
}

void dessinerSerpent(int lesX[], int lesY[]){
	// affiche les anneaux puis la tête
	for(int i=1 ; i<TAILLE ; i++){
		afficher(lesX[i], lesY[i], CORPS);
	}
	afficher(lesX[0], lesY[0],TETE1);
}

void progresser(int lesX[], int lesY[],int *direction, tPlateau plateau, bool * collision, bool * pomme, int nbPommes){
	// efface le dernier élément avant d'actualiser la position de tous les 
	// élémentds du serpent avant de le  redessiner et détecte une
	// collision avec une pomme ou avec une bordure
	effacer(lesX[TAILLE-1], lesY[TAILLE-1]);

	for(int i=TAILLE-1 ; i>0 ; i--){
		lesX[i] = lesX[i-1];
		lesY[i] = lesY[i-1];
	}
    int distanceHaut, distanceBas, distanceGauche, distanceDroite;

	//Calcul la distance entre chaque la prochaine position en fonction de chaque mouvement et la pomme
	int distanceSerpentPommeX = sqrt(pow(lesPommesX[nbPommes] - lesX[0], 2));
	int distanceSerpentPommeY = sqrt(pow(lesPommesY[nbPommes] - lesY[0], 2));

	// Détermine si c'est plus intéressant de passer par une issue des côtés, du haut/bas ou d'aller directement à la pomme
	if(distanceSerpentPommeX > LARGEUR/2 + DISTANCE_PORTAIL_COTE){
		// Aller à l'issue de droite
		if(lesX[0] > LARGEUR/2){
			distanceHaut = sqrt(pow(LARGEUR - lesX[0], 2) + pow(HAUTEUR/2 - (lesY[0] - 1), 2));
			distanceBas = sqrt(pow(LARGEUR - lesX[0], 2) + pow(HAUTEUR/2 - (lesY[0] + 1), 2));
			distanceGauche = sqrt(pow(LARGEUR - (lesX[0] - 1), 2) + pow(HAUTEUR/2 - lesY[0], 2));
			distanceDroite = sqrt(pow(LARGEUR - (lesX[0] + 1), 2) + pow(HAUTEUR/2 - lesY[0], 2));
		}

		// Aller à l'issue de gauche
		else if(lesX[0] < LARGEUR/2){
			distanceHaut = sqrt(pow(LARGEUR - lesX[0], 2) + pow(1 - (lesY[0] - 1), 2));
			distanceBas = sqrt(pow(LARGEUR - lesX[0], 2) + pow(1 - (lesY[0] + 1), 2));
			distanceGauche = sqrt(pow(LARGEUR - (lesX[0] - 1), 2) + pow(1 - lesY[0], 2));
			distanceDroite = sqrt(pow(LARGEUR - (lesX[0] + 1), 2) + pow(1 - lesY[0], 2));
		}
	}
	else if(distanceSerpentPommeY > HAUTEUR/2 + DISTANCE_PORTAIL_HAUTEUR){
		// Aller à l'issue du bas
		if(lesY[0] > HAUTEUR/2){
			distanceHaut = sqrt(pow(LARGEUR/2 - lesX[0], 2) + pow(HAUTEUR - (lesY[0] - 1), 2));
			distanceBas = sqrt(pow(LARGEUR/2 - lesX[0], 2) + pow(HAUTEUR - (lesY[0] + 1), 2));
			distanceGauche = sqrt(pow(LARGEUR/2 - (lesX[0] - 1), 2) + pow(HAUTEUR - lesY[0], 2));
			distanceDroite = sqrt(pow(LARGEUR/2 - (lesX[0] + 1), 2) + pow(HAUTEUR - lesY[0], 2));
		}

		// Aller à l'issue du haut
		else if(lesY[0] < HAUTEUR/2){
			distanceHaut = sqrt(pow(LARGEUR/2 - lesX[0], 2) + pow(1 - (lesY[0] - 1), 2));
			distanceBas = sqrt(pow(LARGEUR/2 - lesX[0], 2) + pow(1 - (lesY[0] + 1), 2));
			distanceGauche = sqrt(pow(LARGEUR/2 - (lesX[0] - 1), 2) + pow(1 - lesY[0], 2));
			distanceDroite = sqrt(pow(LARGEUR/2 - (lesX[0] + 1), 2) + pow(1 - lesY[0], 2));
		}
	}
	else{
		distanceHaut = sqrt(pow(lesPommesX[nbPommes] - lesX[0], 2) + pow(lesPommesY[nbPommes] - (lesY[0] - 1), 2));
		distanceBas = sqrt(pow(lesPommesX[nbPommes] - lesX[0], 2) + pow(lesPommesY[nbPommes] - (lesY[0] + 1), 2));
		distanceGauche = sqrt(pow(lesPommesX[nbPommes] - (lesX[0] - 1), 2) + pow(lesPommesY[nbPommes] - lesY[0], 2));
		distanceDroite = sqrt(pow(lesPommesX[nbPommes] - (lesX[0] + 1), 2) + pow(lesPommesY[nbPommes] - lesY[0], 2));
	}

	int plusCourtCheminX, plusCourtCheminY;
	int plusCourtX, plusCourtY;
	// Determine où il y a la plus courte distance pour aller vers la pomme en Y
	if(distanceBas < distanceHaut){
		if(*direction == HAUT){  // Condition pour qu'il ne se retourne pas sur lui même
			plusCourtCheminY = distanceDroite;
			plusCourtY = DROITE;
		}
		else{
			plusCourtCheminY = distanceBas;
			plusCourtY = BAS;
		}
	}
	else{
		if(*direction == BAS){
			plusCourtCheminY = distanceDroite;
			plusCourtY = DROITE;
		}
		else{
			plusCourtCheminY = distanceHaut;
			plusCourtY = HAUT;
		}
	}

	// Determine où il y a la plus courte distance pour aller vers la pomme en X
	if(distanceGauche < distanceDroite){
		if(*direction == DROITE){
			plusCourtCheminX = distanceBas;
			plusCourtX = BAS;
		}
		else{
			plusCourtCheminX = distanceGauche;
			plusCourtX = GAUCHE;
		}
	}
	else{
		if(*direction == GAUCHE){
			plusCourtCheminX = distanceBas;
			plusCourtX = BAS;
		}
		else{
			plusCourtCheminX = distanceDroite;
			plusCourtX = DROITE;	
		}
	}

	//Determine où il y a la plus courte distance pour aller vers la pomme 
	if(plusCourtCheminX < plusCourtCheminY){
		*direction = plusCourtX;
	}
	else{
		*direction = plusCourtY;
	}

    if (lesX[0] == LARGEUR / 2 && lesY[0] == 1) { // Porte en haut
        lesX[0] = LARGEUR / 2;
        lesY[0] = HAUTEUR;
    } else if (lesX[0] == LARGEUR / 2 && lesY[0] == HAUTEUR) { // Porte en bas
        lesX[0] = LARGEUR / 2;
        lesY[0] = 2;
    } else if (lesY[0] == HAUTEUR / 2 && lesX[0] == 1) { // Porte à gauche
        lesX[0] = LARGEUR - 1;
        lesY[0] = HAUTEUR / 2;
    } else if (lesY[0] == HAUTEUR / 2 && lesX[0] == LARGEUR) { // Porte à droite
        lesX[0] = 2;
        lesY[0] = HAUTEUR / 2 + 1 ;
    }

	//faire progresser la tete dans la nouvelle direction
	int nextX = lesX[0];
    int nextY = lesY[0];

    switch (*direction) {
        case HAUT:    nextY--; break;
        case BAS:     nextY++; break;
        case GAUCHE:  nextX--; break;
        case DROITE:  nextX++; break;
    }

    // Vérifiez si la prochaine position est valide

	/*
		Les 14 lignes de codes suivantes permettent de vérifier si le
		serpent peut aller à la prochaine case qu'il a décidé. S'il ne 
		peut pas à cause d'un pavé ou d'un mur, il prend une autre direction.
	*/
	if (estPositionValide(plateau, nextX, nextY, TAILLE)) {
        // Essayez une autre direction
		if (*direction != HAUT && !estPositionValide(plateau, lesX[0], lesY[0] + 1, TAILLE)) {
            *direction = BAS;
		} else if (*direction != BAS && !estPositionValide(plateau, lesX[0], lesY[0] - 1,  TAILLE)) {
            *direction = HAUT;
		} else if (*direction != DROITE && !estPositionValide(plateau, lesX[0] - 1, lesY[0],  TAILLE)) {
            *direction = GAUCHE;
		} else if (*direction != GAUCHE && !estPositionValide(plateau, lesX[0] + 1, lesY[0], TAILLE)) {
            *direction = DROITE;
        } else {
            *collision = true; // Aucune direction valide
            return;
        }
    }

    // Mettre à jour la direction finale
    switch (*direction) {
        case HAUT:    lesY[0]--; break;
        case BAS:     lesY[0]++; break;
        case GAUCHE:  lesX[0]--; break;
        case DROITE:  lesX[0]++; break;
    }


    dessinerSerpent(lesX, lesY);

	*pomme = false;
	// détection d'une "collision" avec une pomme
	if (plateau[lesX[0]][lesY[0]] == POMME){
		*pomme = true;
		// la pomme disparait du plateau
		plateau[lesX[0]][lesY[0]] = VIDE;
	}
	// détection d'une collision avec la bordure
	else if (plateau[lesX[0]][lesY[0]] == BORDURE){
		*collision = true;
	}

	
	for(int i = TAILLE ; i > 0 ; i--){
		if(lesX[0] == lesX[i] && lesY[0] == lesY[i]){
			*collision = true;
		}
	}

   	dessinerSerpent(lesX, lesY);
}

void dessinerSerpent2(int tabX[], int tabY[]){
	afficher(tabX[0], tabY[0],TETE2);
	for(int i=1 ; i<TAILLE ; i++){
		afficher(tabX[i], tabY[i], CORPS);
	}
}

void progresser2(int tabX[], int tabY[],int *direction, tPlateau plateau, bool * collision, bool * pomme, int nbPommes){
	// efface le dernier élément avant d'actualiser la position de tous les 
	// élémentds du serpent avant de le  redessiner et détecte une
	// collision avec une pomme ou avec une bordure
	effacer(tabX[TAILLE-1], tabY[TAILLE-1]);

	for(int i=TAILLE-1 ; i>0 ; i--){
		tabX[i] = tabX[i-1];
		tabY[i] = tabY[i-1];
	}
    int distanceHaut, distanceBas, distanceGauche, distanceDroite;

	//Calcul la distance entre chaque la prochaine position en fonction de chaque mouvement et la pomme
	int distanceSerpentPommeX = sqrt(pow(lesPommesX[nbPommes] - tabX[0], 2));
	int distanceSerpentPommeY = sqrt(pow(lesPommesY[nbPommes] - tabY[0], 2));

	// Détermine si c'est plus intéressant de passer par une issue des côtés, du haut/bas ou d'aller directement à la pomme
	if(distanceSerpentPommeX > LARGEUR/2 + DISTANCE_PORTAIL_COTE){
		// Aller à l'issue de droite
		if(tabX[0] > LARGEUR/2){
			distanceHaut = sqrt(pow(LARGEUR - tabX[0], 2) + pow(HAUTEUR/2 - (tabY[0] - 1), 2));
			distanceBas = sqrt(pow(LARGEUR - tabX[0], 2) + pow(HAUTEUR/2 - (tabY[0] + 1), 2));
			distanceGauche = sqrt(pow(LARGEUR - (tabX[0] - 1), 2) + pow(HAUTEUR/2 - tabY[0], 2));
			distanceDroite = sqrt(pow(LARGEUR - (tabX[0] + 1), 2) + pow(HAUTEUR/2 - tabY[0], 2));
		}

		// Aller à l'issue de gauche
		else if(tabX[0] < LARGEUR/2){
			distanceHaut = sqrt(pow(LARGEUR - tabX[0], 2) + pow(1 - (tabY[0] - 1), 2));
			distanceBas = sqrt(pow(LARGEUR - tabX[0], 2) + pow(1 - (tabY[0] + 1), 2));
			distanceGauche = sqrt(pow(LARGEUR - (tabX[0] - 1), 2) + pow(1 - tabY[0], 2));
			distanceDroite = sqrt(pow(LARGEUR - (tabX[0] + 1), 2) + pow(1 - tabY[0], 2));
		}
	}
	else if(distanceSerpentPommeY > HAUTEUR/2 + DISTANCE_PORTAIL_HAUTEUR){
		// Aller à l'issue du bas
		if(tabY[0] > HAUTEUR/2){
			distanceHaut = sqrt(pow(LARGEUR/2 - tabX[0], 2) + pow(HAUTEUR - (tabY[0] - 1), 2));
			distanceBas = sqrt(pow(LARGEUR/2 - tabX[0], 2) + pow(HAUTEUR - (tabY[0] + 1), 2));
			distanceGauche = sqrt(pow(LARGEUR/2 - (tabX[0] - 1), 2) + pow(HAUTEUR - tabY[0], 2));
			distanceDroite = sqrt(pow(LARGEUR/2 - (tabX[0] + 1), 2) + pow(HAUTEUR - tabY[0], 2));
		}

		// Aller à l'issue du haut
		else if(tabY[0] < HAUTEUR/2){
			distanceHaut = sqrt(pow(LARGEUR/2 - tabX[0], 2) + pow(1 - (tabY[0] - 1), 2));
			distanceBas = sqrt(pow(LARGEUR/2 - tabX[0], 2) + pow(1 - (tabY[0] + 1), 2));
			distanceGauche = sqrt(pow(LARGEUR/2 - (tabX[0] - 1), 2) + pow(1 - tabY[0], 2));
			distanceDroite = sqrt(pow(LARGEUR/2 - (tabX[0] + 1), 2) + pow(1 - tabY[0], 2));
		}
	}
	else{
		distanceHaut = sqrt(pow(lesPommesX[nbPommes] - tabX[0], 2) + pow(lesPommesY[nbPommes] - (tabY[0] - 1), 2));
		distanceBas = sqrt(pow(lesPommesX[nbPommes] - tabX[0], 2) + pow(lesPommesY[nbPommes] - (tabY[0] + 1), 2));
		distanceGauche = sqrt(pow(lesPommesX[nbPommes] - (tabX[0] - 1), 2) + pow(lesPommesY[nbPommes] - tabY[0], 2));
		distanceDroite = sqrt(pow(lesPommesX[nbPommes] - (tabX[0] + 1), 2) + pow(lesPommesY[nbPommes] - tabY[0], 2));
	}

	int plusCourtCheminX, plusCourtCheminY;
	int plusCourtX, plusCourtY;
	// Determine où il y a la plus courte distance pour aller vers la pomme en Y
	if(distanceBas < distanceHaut){
		if(*direction == HAUT){  // Condition pour qu'il ne se retourne pas sur lui même
			plusCourtCheminY = distanceDroite;
			plusCourtY = DROITE;
		}
		else{
			plusCourtCheminY = distanceBas;
			plusCourtY = BAS;
		}
	}
	else{
		if(*direction == BAS){
			plusCourtCheminY = distanceDroite;
			plusCourtY = DROITE;
		}
		else{
			plusCourtCheminY = distanceHaut;
			plusCourtY = HAUT;
		}
	}

	// Determine où il y a la plus courte distance pour aller vers la pomme en X
	if(distanceGauche < distanceDroite){
		if(*direction == DROITE){
			plusCourtCheminX = distanceBas;
			plusCourtX = BAS;
		}
		else{
			plusCourtCheminX = distanceGauche;
			plusCourtX = GAUCHE;
		}
	}
	else{
		if(*direction == GAUCHE){
			plusCourtCheminX = distanceBas;
			plusCourtX = BAS;
		}
		else{
			plusCourtCheminX = distanceDroite;
			plusCourtX = DROITE;	
		}
	}

	//Determine où il y a la plus courte distance pour aller vers la pomme 
	if(plusCourtCheminX < plusCourtCheminY){
		*direction = plusCourtX;
	}
	else{
		*direction = plusCourtY;
	}

    if (tabX[0] == LARGEUR / 2 && tabY[0] == 1) { // Porte en haut
        tabX[0] = LARGEUR / 2;
        tabY[0] = HAUTEUR;
    } else if (tabX[0] == LARGEUR / 2 && tabY[0] == HAUTEUR) { // Porte en bas
        tabX[0] = LARGEUR / 2;
        tabY[0] = 2;
    } else if (tabY[0] == HAUTEUR / 2 && tabX[0] == 1) { // Porte à gauche
        tabX[0] = LARGEUR - 1;
        tabY[0] = HAUTEUR / 2;
    } else if (tabY[0] == HAUTEUR / 2 && tabX[0] == LARGEUR) { // Porte à droite
        tabX[0] = 2;
        tabY[0] = HAUTEUR / 2 + 1 ;
    }

	//faire progresser la tete dans la nouvelle direction
	int nextX = tabX[0];
    int nextY = tabY[0];

    switch (*direction) {
        case HAUT:    nextY--; break;
        case BAS:     nextY++; break;
        case GAUCHE:  nextX--; break;
        case DROITE:  nextX++; break;
    }

    // Vérifiez si la prochaine position est valide

	/*
		Les 14 lignes de codes suivantes permettent de vérifier si le
		serpent peut aller à la prochaine case qu'il a décidé. S'il ne 
		peut pas à cause d'un pavé ou d'un mur, il prend une autre direction.
	*/
	if (estPositionValide2(plateau, nextX, nextY, TAILLE)) {
        // Essayez une autre direction
		if (*direction != HAUT && !estPositionValide2(plateau, tabX[0], tabY[0] + 1, TAILLE)) {
            *direction = BAS;
		} else if (*direction != BAS && !estPositionValide2(plateau, tabX[0], tabY[0] - 1, TAILLE)) {
            *direction = HAUT;
		} else if (*direction != DROITE && !estPositionValide2(plateau, tabX[0] - 1, tabY[0], TAILLE)) {
            *direction = GAUCHE;
		} else if (*direction != GAUCHE && !estPositionValide2(plateau, tabX[0] + 1, tabY[0], TAILLE)) {
            *direction = DROITE;
        } else {
            *collision = true; // Aucune direction valide
            return;
        }
    }

    // Mettre à jour la direction finale
    switch (*direction) {
        case HAUT:    tabY[0]--; break;
        case BAS:     tabY[0]++; break;
        case GAUCHE:  tabX[0]--; break;
        case DROITE:  tabX[0]++; break;
    }


    dessinerSerpent2(tabX, tabY);

	*pomme = false;
	// détection d'une "collision" avec une pomme
	if (plateau[tabX[0]][tabY[0]] == POMME){
		*pomme = true;
		// la pomme disparait du plateau
		plateau[tabX[0]][tabY[0]] = VIDE;
	}
	// détection d'une collision avec la bordure
	else if (plateau[tabX[0]][tabY[0]] == BORDURE){
		*collision = true;
	}

	
	for(int i = TAILLE ; i > 0 ; i--){
		if(tabX[0] == tabX[i] && tabY[0] == tabY[i]){
			*collision = true;
		}
	}
   	dessinerSerpent2(tabX, tabY);
}

/*
	Cette fonction sert à évaluer si une position est valide 
	donc si le serpent à une position n'est pas dans un 
	obstacle ou dans un mur.
*/
bool estPositionValide(tPlateau plateau, int x, int y, int taille) {
	bool collision = false;
	if (plateau[x][y] == BORDURE) {
		collision = true;
	}
	for ( int i = 0 ; i < taille ; i++){
		if (x == tabX[i] && y == tabY[i]){
			collision = true;
		}
	}

	for (int i = 0; i < taille; i++) {
		if (lesX[i] == x && lesY[i] == y) {
			collision = true;
		}
	}
	return collision;
}

bool estPositionValide2(tPlateau plateau, int x, int y, int taille) {
	bool collision = false;
	if (plateau[x][y] == BORDURE) {
		collision = true;
	}
	for ( int i = 0 ; i < taille ; i++){
		if (x == lesX[i] && y == lesY[i]){
			collision = true;
		}
	}
	for (int i = 0; i < taille; i++) {
		if (tabX[i] == x && tabY[i] == y) {
			collision = true;
		}
	}
	return collision;
}

/******************************-****************/
/*				 FONCTIONS UTILITAIRES 			*/
/************************************************/
void gotoxy(int x, int y) { 
    printf("\033[%d;%df", y, x);
}

int kbhit(){
	// la fonction retourne :
	// 1 si un caractere est present
	// 0 si pas de caractere présent
	int unCaractere=0;
	struct termios oldt, newt;
	int ch;
	int oldf;

	// mettre le terminal en mode non bloquant
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
	ch = getchar();

	// restaurer le mode du terminal
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
 
	if(ch != EOF){
		ungetc(ch, stdin);
		unCaractere=1;
	} 
	return unCaractere;
}

// Fonction pour désactiver l'echo
void disable_echo() {
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Désactiver le flag ECHO
    tty.c_lflag &= ~ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

// Fonction pour réactiver l'echo
void enable_echo() {
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Réactiver le flag ECHO
    tty.c_lflag |= ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}