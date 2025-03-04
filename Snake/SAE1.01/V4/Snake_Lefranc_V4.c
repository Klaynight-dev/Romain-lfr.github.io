/**
* \page Général 
* 
* \author LEFRANC Romain
* \version 4
* \date 24 novembre 2024
*
* Ce programme permet de déplacer un serpent dans le terminal avec z, q, s, d.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
#include <time.h>

/** \fn Définition des Constantes */
#define TAILLE 10
#define TAILLE_MAX 21
#define X 40
#define Y 20
#define TETE 'O'
#define CORPS 'X'
#define FIN 'a'
#define HAUT 'z'
#define BAS 's'
#define GAUCHE 'q'
#define DROITE 'd'
#define LARGEUR 80
#define HAUTEUR 40
#define BORDURE '#'
#define VIDE ' '
#define PAVE 5
#define NB_PAVE 4
#define POMME '6'
#define MAX_POMME 10
#define TEMPORISATION 150000
#define AUGM_VITESSE 12500

char plateau[HAUTEUR][LARGEUR];

/** \fn Définition des fonctions et procédures */
void afficher(int x, int y, char c);
void effacer(int x, int y);
void dessinerSerpent(int tabX[], int tabY[], int taille);
void progresser(int tabX[], int tabY[], char direction, char plateau[HAUTEUR][LARGEUR], bool *collision, bool *pommeManger, int *nbPomme); // procédure pour le faire avancer
void initPlateau(char plateau[HAUTEUR][LARGEUR]);
void dessinerPlateau (char plateau[HAUTEUR][LARGEUR]);
void afficherPave (char plateau[HAUTEUR][LARGEUR], int tabX[], int tabY[]);
void ajouterPomme(char plateau[HAUTEUR][LARGEUR], int tabX[], int tabY[]);

//Fonctions et procédures non changés

void gotoXY(int x, int y);
int kbhit();
void disableEcho();
void enableEcho();

//Définition des variables globals

int tailleSerpent = TAILLE;
int temps = TEMPORISATION;

/**
* \fn int main()
* \brief Programme principal.
* \return Code de sortie du programme (0 : sortie normale).
*
* Ce programme permet de manipuler les fonctions pour le snake
*/

 int main(){
    //definition des variables
    int x, y ;
    char direction = DROITE ;
    char nouvelleDirection = DROITE ; 
    char verification = DROITE ;
    int tabX[TAILLE_MAX], tabY[TAILLE_MAX];
    bool collision = false ;
    bool pommeManger = false;
    int nbPomme = 0;

    srand(time(NULL));

    //Initialisation
    x=X;
    y=Y;

    system("clear");
    initPlateau(plateau);
    afficherPave(plateau, tabX, tabY);
    ajouterPomme(plateau, tabX, tabY);
    dessinerPlateau (plateau);

    //Traitement
    for (int i = 0;  i< tailleSerpent ; i++)
    {
        tabX[i]=x-i;
        tabY[i]=y;
    }

    disableEcho();
    dessinerSerpent(tabX, tabY, tailleSerpent);

    while(1)
    {
        usleep(temps);
        progresser(tabX, tabY, direction, plateau, &collision, &pommeManger, &nbPomme);
        if (nbPomme == MAX_POMME)
        {
            enableEcho();
            system("clear");
            printf("Vous avez gagné !\n");
            return 0;
        }

        if (kbhit() == 1) {
            verification = getchar();
            if (verification == HAUT || verification == BAS || verification == DROITE || verification == GAUCHE) {
                nouvelleDirection = verification;
            } else if (verification == FIN) {
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

        if (collision) {
            system("clear");
            break;
        }
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

void afficher(int x, int y, char c)
{
    gotoXY( x, y);
    printf("%c", c);
    gotoXY(1,1);
}

/**
* \fn void effacer(int x, int y)
* \brief Permet d'effacer un caractère 
* \param x : numéro de la colonne
* \param y : numéro de la ligne
* 
* Cette procédure doit afficher un espace (c'est à directione effacer) à la position (x, y),
* où x représente le numéro de colonne et y le numéro de ligne.
*/

void effacer(int x, int y)
{
    gotoXY( x, y);
    printf(" ");
}

/**
* \fn void dessinerSerpent(int tabX[], int tabY[], int taille)
* \brief Procédure pour afficher le serpent
* \param tabX : tableau des colonnes
* \param tabY : tableau des lignes
* \param taille : taille actuelle du serpent
*/

void dessinerSerpent(int tabX[], int tabY[], int taille)
{
    int i;
    for (i = 0; i < taille; i++)
    {
        if (i==0)
        {
            afficher(tabX[i], tabY[i], TETE); //affiche la tête
        }
        else
        {
            afficher(tabX[i], tabY[i], CORPS); //affiche le corps
        }
    }
}


/**
* \fn void progresser(int tabX[], int tabY[], char direction, bool *collision, bool *pommeMangee, char plateau[HAUTEUR][LARGEUR], int tailleSerpent)
* \brief Procédure pour faire progresser le serpent
* \param tabX : tableau des coordonnées X
* \param tabY : tableau des coordonnées Y
* \param direction : direction du serpent
* \param collision : état de la collision
* \param pommeMangee : état de la pomme mangée
* \param plateau : plateau de jeu
* \param tailleSerpent : taille actuelle du serpent
*/

void progresser(int tabX[], int tabY[], char direction, char plateau[HAUTEUR][LARGEUR], bool *collision, bool *pommeManger, int *nbPomme)
{
    int i, x, y;
    effacer(tabX[tailleSerpent-1], tabY[tailleSerpent-1]);
    
    for(i=tailleSerpent; i > 0 ; i--)
    {
        tabX[i]=tabX[i-1];
        tabY[i]=tabY[i-1];
    }

    switch(direction)
    {
        case (HAUT):
        {
            tabY[0]+=-1;
            break;
        }

        case (BAS):
        {
            tabY[0]+=1;
            break;
        }

        case (GAUCHE):
        {
            tabX[0]+=-1;
            break;
        }

        case (DROITE):
        {
            tabX[0]+=1;
            break;
        }
    }


    // Gestion des sorties
    if (tabY[0] == 1 && tabX[0] == LARGEUR / 2)
    {
        tabY[0] = HAUTEUR - 2;
    }
    else if (tabY[0] == HAUTEUR - 1 && tabX[0] == LARGEUR / 2)
    {
        tabY[0] = 2;
    }
    else if (tabX[0] == 1 && tabY[0] == HAUTEUR / 2)
    {
        tabX[0] = LARGEUR - 2;
    }
    else if (tabX[0] == LARGEUR - 1 && tabY[0] == HAUTEUR / 2)
    {
        tabX[0] = 2; 
    }


    // Arrête le serpent si collision avec la bordure
    if (tabX[0] == 1 || tabX[0] == LARGEUR -1 || tabY[0] == 1 || tabY[0] == HAUTEUR -1 )
    {
        *collision = true;
    }

    // Arrête le serpent si collision avec le corps
    for (i = 1; i < tailleSerpent; i++)
    {
        if (tabX[0] == tabX[i] && tabY[0] == tabY[i])
        {
            *collision = true;
        }
    }

    if (plateau[tabY[0]][tabX[0]]==POMME)
    {


        //modifie la taille du serpent
        tailleSerpent++;
        tabX[tailleSerpent] = tabX[tailleSerpent - 1];
        tabY[tailleSerpent] = tabY[tailleSerpent - 1];
        
        // Augmente la vitesse du serpent
        temps = temps - AUGM_VITESSE;

        //on augmente le compteur
        *nbPomme = *nbPomme + 1;

        //rajoute une pomme
        *pommeManger = true;
        plateau[tabY[0]][tabX[0]] = VIDE;
        ajouterPomme(plateau, tabX, tabY);
    }


    // Arrête le serpent si collision avec un pavé
    for (y = 0; y < HAUTEUR; y++)
    {
        for (x = 0; x < LARGEUR; x++)
        {
            if (plateau[y][x] == BORDURE) // Vérifie si la position actuelle est un pavé
            {
                // Vérifie si la tête du serpent est sur le pavé
                if (tabX[0] == x && tabY[0] == y)
                {
                    *collision = true; // Collision détectée avec le pavé
                }
            }
        }
    }

    dessinerSerpent(tabX, tabY, tailleSerpent);  // dessine le serpent à la position suivante
}

/**
* \fn void initPlateau(char plateau[HAUTEUR][LARGEUR])
* \brief Initialise le plateau de jeu
*
* Cette fonction initialise le plateau en dessinant les bordures et en remplissant 
* les cases internes avec des espaces vides.
* Les bordures sont représentées par le caractère `#`.
*/

void initPlateau(char plateau[HAUTEUR][LARGEUR]) {

    int x, y;
    for (y = 1; y < HAUTEUR; y++) {
        for (x = 1; x < LARGEUR; x++) {
            plateau[y][x] = VIDE;
        }
    }

    // Dessiner les bordures
    for (x = 0; x < LARGEUR; x++) {
        plateau[1][x] = BORDURE;
        plateau[HAUTEUR -1][x] = BORDURE;
    }
    for (y = 0; y < HAUTEUR; y++) {
        plateau[y][1] = BORDURE;
        plateau[y][LARGEUR -1] = BORDURE;
    }

    //Systeme de téleportation
    plateau[1][LARGEUR / 2] = VIDE;          
    plateau[HAUTEUR - 1][LARGEUR / 2] = VIDE;
    plateau[HAUTEUR / 2][1] = VIDE;            
    plateau[HAUTEUR / 2][LARGEUR - 1] = VIDE;
}

/**
* \fn void dessinerPlateau()
* \brief Affiche le plateau de jeu à l'écran
*
* Cette fonction affiche le plateau de jeu en utilisant les coordonnées des cases dans le tableau `plateau`.
* Les bordures et les espaces vides sont affichés en fonction de la valeur de chaque case.
*/    

void dessinerPlateau (char plateau[HAUTEUR][LARGEUR])
{
    int x, y;
    for (y = 0; y < HAUTEUR; y++) 
    {
        for (x = 0; x < LARGEUR; x++) 
        {
            afficher(x, y,plateau[y][x]); //affiche le plateau de jeu
        }
    }
}

/**
* \fn void afficherrPave(char plateau[HAUTEUR][LARGEUR], int tabX[], int tabY[])
* \brief Place des pavés (obstacles) aléatoires sur le plateau
*
* Cette fonction place un certain nombre de pavés (obstacles) sur le plateau, 
* en s'assurant qu'ils ne chevauchent pas le serpent.
*/

void afficherPave(char plateau[HAUTEUR][LARGEUR], int tabX[], int tabY[]) {
    int i, j, k, x, y;
    int nbPavé = 0;
    bool hit;
    hit = false;
    srand(time(NULL)); // Initialise le générateur de nombres aléatoires

    while (nbPavé < NB_PAVE) {
        // crée les coordonnées du coin supérieur gauche du pavé
        x = rand() % (LARGEUR - 3 - PAVE) + 2; // +1 pour éviter les bordures
        y = rand() % (HAUTEUR - 3 - PAVE) + 2; // +1 pour éviter les bordures

        hit = false; // Réinitialiser hit pour chaque nouvelle position

        // Vérifier si le carré peut être placé sans toucher la bordure
        for (i = 0; i < PAVE; i++) {
            for (j = 0; j < PAVE; j++) {
                // Vérifier si la position est une bordure
                if (plateau[y + i][x + j] == BORDURE) {
                    hit = true; // Le pavé est au même endroit que la bordure donc apparaît pas
                }

                // Vérifier si la position touche le serpent
                for (k = 0; k < TAILLE; k++) {
                    // On vérifie avec les coordonnées du serpent, en tenant compte de l'affichage
                    if (tabX[k] == (x + i) && tabY[k] == (y + j)) {
                        hit = true; // Le pavé est au même endroit que le serpent donc apparaît pas
                    }
                }
            }
        }

        // Si pas de collision, placer le carré
        if (!hit) {
            for (i = 0; i < PAVE; i++) {
                for (j = 0; j < PAVE; j++) {
                    plateau[y + i][x + j] = BORDURE; // Placer le pavé
                }
            }
            nbPavé++; // On augmente de 1 le nombre total de pavés
        }
    }
}

/**
* \fn void ajouterPomme(char plateau[HAUTEUR][LARGEUR], int tabX[], int tabY[])
* \brief Ajoute une pomme à une position aléatoire du plateau
*
* Cette fonction place une pomme (représentée par le caractère `6`) sur le plateau, à une position qui ne chevauche pas le serpent.
*/

void ajouterPomme(char plateau[HAUTEUR][LARGEUR], int tabX[], int tabY[]) {
    int x, y;
    bool positionValide = false;

    while (!positionValide) {
        positionValide = true;
        x = rand() % (LARGEUR - 2) + 1;
        y = rand() % (HAUTEUR - 2) + 1;

        if (plateau[y][x] == BORDURE) {
            positionValide = false;
        }

        for (int i = 0; i < TAILLE; i++) {
            positionValide = positionValide && !(tabX[i] == x && tabY[i] == y);
        }
    }
    plateau[y][x] = POMME;
    afficher(x, y, POMME);
}

/**
* \fn void gotoXY(int x, int y)
* \brief Procédure pour positionner le curseur à un endroit précis.
* \param x : numéro de la colonne
* \param y : numéro de la ligne
* 
* Cette fonction positionne le curseur à la position (x, y) avant d'afficher un caractère.
*/

void gotoXY(int x, int y) 
{ 
    printf("\033[%d;%df", y, x);
}

/**
* \fn int kbhit()
* \brief Fonction qui permet de vérifier si une touche a été pressée
* \return 1 si une touche est pressée, 0 sinon
*
* Cette fonction vérifie si un caractère est disponible dans le tampon d'entrée.
* Elle est utilisée pour détecter les pressions de touches sans bloquer l'exécution du programme.
*/

int kbhit()
{

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

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
 
	if(ch != EOF)
    {
		ungetc(ch, stdin);
		unCaractere=1;
	} 
	return unCaractere;
}

/**
* \fn void disableEcho()
* \brief Désactive l'écho des caractères sur le terminal
*
* Cette fonction désactive l'écho dans le terminal, ce qui signifie que les caractères tapés par l'utilisateur ne seront pas affichés à l'écran.
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
* \brief Réactive l'écho des caractères sur le terminal
*
* Cette fonction réactive l'écho des caractères dans le terminal, ce qui signifie que les caractères tapés par l'utilisateur seront affichés à l'écran.
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