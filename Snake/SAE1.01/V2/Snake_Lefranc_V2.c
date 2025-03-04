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
const int RIGHT = 0, LEFT = 1, UP = 2, DOWN = 3 ;
const int X = 20 ;
const int Y = 20 ;
const int TEMPORISATION = 150000 ;
#define FIN 'a'
#define DROITE 'd'
#define GAUCHE 'q'
#define HAUT 'z'
#define BAS 's'
#define HEAD "O" 
#define BODY "X"

/** \fn Définition des fonctions et procédures */
void afficher(int x, int y, char *c);
void effacer(int x, int y);
void dessinerSerpent(int tabX[TAILLE], int tabY[TAILLE]);
void progresser(int tabX[], int tabY[], char direction);
void gotoXY(int x, int y);
int kbhit();
void disableEcho();
void enableEcho();

/**
* \fn int main()
* \brief Programme principal.
* 
*/

int main(){

    // Déclaration des variables
	int direction = DROITE;
	char nouvelleDirection = DROITE;
	int tabX[TAILLE], tabY[TAILLE];

	system("clear");

	//initialisation des tableaux


	for (int i = 0; i < TAILLE; i++) {
        if(X-i>0){tabX[i] = X - i;}
        else{tabX[i]=1;}     
        tabY[i] = Y;   
    }

	
	while (1) {
		disableEcho();
		usleep(TEMPORISATION);
		dessinerSerpent(tabX, tabY);

		if (kbhit()==1) {
            char verification = getchar();
            if (verification == FIN) {
                system("clear");
                enableEcho();
                return EXIT_SUCCESS;
            }
            else if ((verification == DROITE && direction != LEFT) ||
                (verification == GAUCHE && direction != RIGHT) ||
                (verification == HAUT && direction != DOWN) ||
                (verification == BAS && direction != UP)) {
                nouvelleDirection = verification;
            }
        }
        if(nouvelleDirection==DROITE){
            direction = RIGHT;
        }
        else if(nouvelleDirection==GAUCHE){
            direction = LEFT;
        }
        else if(nouvelleDirection==HAUT){
            direction = UP;
        }
        else if(nouvelleDirection==BAS){
            direction = DOWN;
        }
        progresser(tabX, tabY, nouvelleDirection);
        
        enableEcho();
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

void afficher(int x, int y, char *c){

    gotoXY(x,y);
    printf("%s",c);

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


/**
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
		if (i == 0){
			afficher(tabX[i], tabY[i],HEAD);
		}
		else {
			afficher(tabX[i], tabY[i],BODY);
		}
	}	
	gotoXY(tabX[0] + 1, tabY[0]);
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

void progresser(int tabX[], int tabY[], char direction) {
    effacer(tabX[TAILLE-1], tabY[TAILLE-1]);
    for (int i = TAILLE - 1; i > 0; i--) {
        tabX[i] = tabX[i - 1]; // Déplacer chaque segment vers la droite
        tabY[i] = tabY[i - 1]; // Maintenir Y constant
    }
    switch(direction){
        case HAUT:
            if(tabY[0]>0){
                tabY[0]--;
            }
            else{
                tabY[0]=0;
            }
            break;
        case BAS:
            tabY[0]++;
            break;
        case DROITE:
            tabX[0]++;
            break;
        case GAUCHE:
            if(tabX[0]>0){
                tabX[0]--;
            }
            else{
                tabX[0] = 0;
            }
            break;
    }    
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

/**
* \fn void disableEcho()
* \brief procedure
* 
* Le programme principal permet de désactiver
* l'affichage de caractere dans le terminal
*/

void disableEcho() {
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Desactiver le flag ECHO
    tty.c_lflag &= ~ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

/**
* \fn void enableEcho()
* \brief procedue
* 
* Le programme principal permet de réactiver
* l'affichgage de caractere dans le terminal.
*/

void enableEcho() {
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Reactiver le flag ECHO
    tty.c_lflag |= ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}
