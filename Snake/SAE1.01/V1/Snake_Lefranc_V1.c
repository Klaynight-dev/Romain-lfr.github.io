/**
* \page Général 
* 
* \author LEFRANC
* \version 1.0
* \date 23 octobre 2024
*
* Ce programme propose plusieurs opérations de manipulation
* de chaînes de caractères où les chaînes sont implémentées
* par des listes chainées de caractères.Programme de manipulation de chaînes de caractères.
*/

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>

//Déclaration des constantes

const int TAILLE = 10;
const char FIN = 'a';

/** \fn Définition des fonctions et procédures */
void afficher(int x, int y, char c);
void effacer(int x, int y);
void dessinerSerpent(int tabX[TAILLE], int tabY[TAILLE]);
void progresser(int tabX[TAILLE], int tabY[TAILLE]);
void gotoXY(int x, int y);
int kbhit();

/**
* \fn int main()
* \brief Programme principal.
* 
*/

int main(void){

    // Déclaration des variables
    int x , y;
	int tabX[TAILLE], tabY[TAILLE];
	printf("Quelle est la colonne de départ :");
	scanf("%d", &x);
	printf("Quelle est la ligne de départ :");
	scanf("%d", &y);
	system("clear");
	//initialisation des tableaux
	if (x <= 0 || y <= 0){
		printf("Erreur merci d'inscrire des valeurs positive \n");
	}
	else {
		for (int i = 0; i < TAILLE; i++){
			tabX[i]=x-i ;
			tabY[i]=y;
		}

		while (1) {
			dessinerSerpent(tabX, tabY);
			if(kbhit()==1 && getchar()==FIN){
				system("clear");
				break;
			}
			sleep(1);
			progresser(tabX, tabY);
		}
	}
	return EXIT_SUCCESS;
}

/**
* \fn void afficher(int x, int y, char c)
* \brief Procédure afficher
* \param x : numéro de colonne
* \param y : numéro de ligne
* \param O : caractère à afficher
* 
*  Cette procédure doit afficher le caractère O à la position (x, y), où x représente le numéro de colonne
*  et y le numéro de ligne.
*/

void afficher(int x, int y, char c){

    gotoXY(x,y);
    printf("%c",c);

}

/**
* \fn void effacer(int x, int y)
* \brief Procédure 
* \param x : numéro de colonne
* \param y : numéro de ligne
* 
* Cette procédure doit afficher un espace à la position (x, y), où x représente le numéro de colonne et y
* le numéro de ligne.
*/

void effacer(int x, int y){
	gotoXY(x, y);
	printf(" ");
}


/**X'
* \fn void dessinerSerpent(int tabX[], int tabY[])
* \brief Procédure
* \param tabX : tableau des colonnes
* \param tabY : tableau des lignes
* 
* 
* Cette procédure sera chargée d’afficher à l’écran un à un les éléments du serpent dont les coordonnées
* sont fournies dans le(s) tableau(x) en paramètre(s)
*/

void dessinerSerpent(int tabX[TAILLE], int tabY[TAILLE]){
	for(int i = 0; i < TAILLE ; i++){
		if (tabX[i] > 0){
			afficher(tabX[i], tabY[i],'X');
		}
		afficher(tabX[0], tabY[0],'O');
	}	
}

/**
* \fn void progresser(int tabX[], int tabY[])
* \brief Procédure
* \param tabX : tableau des colonnes
* \param tabY : tableau des lignes
* 
* Cette procédure sera chargée de calculer et d’afficher la prochaine position du serpent
* la procédure doit faire en sorte que le serpent évolue d’une position vers
* la droite et donc l’affiche avec la tête en position
*/

void progresser(int tabX[TAILLE], int tabY[TAILLE]){
	effacer(tabX[TAILLE-1], tabY[TAILLE-1]);
	for(int i=TAILLE -1; i > 0 ; i--){
        tabX[i]=tabX[i-1];
        tabY[i]=tabY[i-1];
    }
	tabX[0]++;
}


/**
* \fn void gotoXY(int x, int y)
* \brief Procédure
* \param x : numéro de colonne
* \param y : numéro de ligne
* 
* Pour positionner le curseur à un endroit précis (avant l’affichage d’un caractère par exemple),
* utilisez la procédure
*/

void gotoXY(int x, int y) { 
    printf("\033[%d;%df", y, x);
}

/**
* \fn int kbhit()
* \brief Fonction
* \return getchar()
* 
* Le programme principal permet de savoir
* si un caractere est préssé sur le clavier
*/

int kbhit(){
	// la fonction retourne :
	// 1 si un caractere est present
	// 0 si pas de caractere present
	
	int unCaractere=0;
	struct termios oldt, newt;
	int ch;
	int oldf;
	
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