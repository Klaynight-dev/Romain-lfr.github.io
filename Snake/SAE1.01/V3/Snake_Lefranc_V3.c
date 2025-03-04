/**
* \page Général 
* 
* \author LEFRANC Romain
* \version 3
* \date 11 novembre 2024
*
* Ce programme permet de déplacer un serpent dans le terminal avec z, q, s, d.
*/

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

//Déclaration des constantes

#define TAILLE 10
#define FIN 'a'
#define X 40
#define Y 20
#define DROITE 'd'
#define GAUCHE 'q'
#define HAUT 'z'
#define BAS 's'
#define TETE 'O'
#define ANNEAUX 'X'
#define TEMPORISATION 100000
#define VIDE ' '
#define BORDURE '#'
#define LARGEUR 80
#define HAUTEUR 40
#define NOMBRE_PAVES 4
#define PAVE_TAILLE 5

char plateau[HAUTEUR][LARGEUR];


/** \fn Définition des fonctions et procédures */
void afficher(int x, int y, char c);
void effacer(int x, int y);
void dessinerSerpent(int tabX[TAILLE], int tabY[TAILLE]);
void progresser(int tabX[TAILLE], int tabY[TAILLE], char direction, bool *collision);
void gotoXY(int x, int y);
int kbhit();
void disableEcho();
void enableEcho();
void initPlateau(char plateau[HAUTEUR][LARGEUR]);
void dessinerPlateau();
void placerPaves(char plateau[HAUTEUR][LARGEUR], int tabX[TAILLE], int tabY[TAILLE]);


/**
* \fn int main()
* \brief Programme principal.
* \return Code de sortie du programme (0 : sortie normale).
*
*Ce programme permet de manipuler les fonctions pour le snake
*/

int main(void) {
    // Déclaration des variables
    bool collision = false;
    char direction = DROITE;
    char nouvelleDirection = DROITE;
    char verification;
    int tabX[TAILLE], tabY[TAILLE];

    // Initialisation du terminal
    disableEcho();
    system("clear");

    //Initialisation du plateau et du serpent
    initPlateau(plateau);
    placerPaves(plateau, tabX, tabY);
    for (int i = 0; i < TAILLE; i++) {
        tabX[i] = X - i;
        tabY[i] = Y;
    }
    
    while (1) {
        dessinerPlateau();
        dessinerSerpent(tabX, tabY);
        
        if (kbhit() == 1) {
            verification = getchar();
            if (verification == HAUT || verification == BAS || verification == DROITE || verification == GAUCHE) {
                nouvelleDirection = verification;
            }
			else if (verification == FIN)
			{
				system("clear");
				break;
			}
			
        }
        
        // Ne permet pas au serpent de se retourner sur lui-même
        if (!(nouvelleDirection == HAUT && direction == BAS) &&
            !(nouvelleDirection == BAS && direction == HAUT) &&
            !(nouvelleDirection == DROITE && direction == GAUCHE) &&
            !(nouvelleDirection == GAUCHE && direction == DROITE)) {
            direction = nouvelleDirection;
        }

        progresser(tabX, tabY, direction, &collision);
        if (collision) {
            enableEcho();
            system("clear");
            break;
        }
        usleep(TEMPORISATION);
    }
    enableEcho();
	return EXIT_SUCCESS;
}

/**
* \fn void afficher(int x, int y, char c)
* \brief Affiche un caractère
* \param x : numéro de la colonne
* \param y : numéro de la ligne
* \param c : caractère à afficher
* 
* Cette procédure doit afficher le caractère demandé à la position (x, y),
* où x représente le numéro de colonne et y le numéro de ligne.
*/

void afficher(int x, int y, char c) {
    gotoXY(x + 1, y + 1);
    printf("%c", c);
    fflush(stdout);
}

/**
* \fn void effacer(int x, int y)
* \brief Permet d'effacer un caractère 
* \param x : numéro de la colonne
* \param y : numéro de la ligne
* 
* Cette procédure doit afficher un espace (c'est à dire effacer) à la position (x, y),
* où x représente le numéro de colonne et y le numéro de ligne.
*/

void effacer(int x, int y) {
    gotoXY(x + 1, y + 1);
    printf("%c", VIDE);
    fflush(stdout);
}


/**
* \fn void dessinerSerpent(int tabX[], int tabY[])
* \brief Procédure
* \param tabX : tableau des colonnes
* \param tabY : tableau des lignes
* 
* 
* Cette procédure sera chargée d’afficher à l’écran un à un les éléments du serpent,
* dont les coordonnées sont fournies dans le(s) tableau(x) en paramètre(s)
*/

void dessinerSerpent(int tabX[TAILLE], int tabY[TAILLE]){
	for(int i = 0; i < TAILLE ; i++){
		if (i == 0){
			afficher(tabX[i], tabY[i],TETE);
		}
		else {
			afficher(tabX[i], tabY[i],ANNEAUX);
		}
		
	}	
}

/**
* \fn void progresser(int tabX[TAILLE], int tabY[TAILLE], char direction, bool *collision, char plateau[HAUTEUR][LARGEUR])
* \brief Procédure
* \param tabX : tableau des colonnes
* \param tabY : tableau des lignes
* \param direction : caractère de la direction
* 
* Cette procédure sera chargée de calculer et d’afficher la prochaine position du serpent
* la procédure doit faire en sorte que le serpent évolue d’une position vers une autre et détecte les collisions
*/

void progresser(int tabX[TAILLE], int tabY[TAILLE], char direction, bool *collision) {
    int suivX = tabX[0];
    int suivY = tabY[0];

    // Déplacement basé sur la direction
    switch (direction) {
        case HAUT:    suivY--; break;
        case BAS:     suivY++; break;
        case DROITE:  suivX++; break;
        case GAUCHE:  suivX--; break;
    }

    // Vérifier collision avec bordures ou pavés
    if (plateau[suivY][suivX] == BORDURE) {
        *collision = true;
        return;
    }

    // Vérifier collision avec soi-même
    for (int i = 0; i < TAILLE; i++) {
        if (tabX[i] == suivX && tabY[i] == suivY) {
            *collision = true;
            return;
        }
    }

    // Mise à jour des coordonnées du serpent
    effacer(tabX[TAILLE - 1], tabY[TAILLE - 1]);
    for (int i = TAILLE - 1; i > 0; i--) {
        tabX[i] = tabX[i - 1];
        tabY[i] = tabY[i - 1];
    }
    tabX[0] = suivX;
    tabY[0] = suivY;

    afficher(tabX[0], tabY[0], TETE);
    for (int i = 1; i < TAILLE; i++) {
        afficher(tabX[i], tabY[i], ANNEAUX);
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
    printf("\033[%d;%dH", y, x);
    fflush(stdout);
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

/**
* \fn void initPlateau(char plateau[HAUTEUR][LARGEUR])
* \brief Initialise le plateau
*
* Initialise les bordures et les espaces vides du plateau
*/
void initPlateau(char plateau[HAUTEUR][LARGEUR]) {
    for (int i = 0; i < HAUTEUR; i++) {
        for (int j = 0; j < LARGEUR; j++) {
            if (i == 0 || i == HAUTEUR - 1 || j == 0 || j == LARGEUR - 1) {
                plateau[i][j] = BORDURE;
            } else {
                plateau[i][j] = VIDE;
            }
        }
    }
}


/**
* \fn void dessinerPlateau(char plateau[HAUTEUR][LARGEUR])
* \brief Affiche le plateau de jeu
*
* Affiche le plateau de jeu
*/
void dessinerPlateau() {
    for (int y = 0; y < HAUTEUR; y++) {
        for (int x = 0; x < LARGEUR; x++) {
            gotoXY(x + 1, y + 1);
            printf("%c", plateau[y][x]);
        }
    }
}

/**
* \fn void placerPaves()
* \brief Place des pavés aléatoires
*/
void placerPaves(char plateau[HAUTEUR][LARGEUR], int tabX[TAILLE], int tabY[TAILLE]) {
    srand(time(NULL));
    
    for (int p = 0; p < NOMBRE_PAVES; p++) {
        int pavX, pavY;
        bool serpent;

        do {
            serpent = false;
            pavX = rand() % (LARGEUR - 2 * PAVE_TAILLE) + 1; // Évite les bordures
            pavY = rand() % (HAUTEUR - 2 * PAVE_TAILLE) + 1;

            // Vérifie si le pavé chevauche le serpent au départ
            for (int i = 0; i < TAILLE; i++) {
                if (tabX[i] >= pavX && tabX[i] < pavX + PAVE_TAILLE &&
                    tabY[i] >= pavY && tabY[i] < pavY + PAVE_TAILLE) {
                    serpent = true;
                    break;
                }
            }

        } while (serpent); // Réessaie si le pavé chevauche le serpent

        // Place le pavé dans le plateau
        for (int i = 0; i < PAVE_TAILLE; i++) {
            for (int j = 0; j < PAVE_TAILLE; j++) {
                plateau[pavY + i][pavX + j] = BORDURE;
            }
        }
    }
}