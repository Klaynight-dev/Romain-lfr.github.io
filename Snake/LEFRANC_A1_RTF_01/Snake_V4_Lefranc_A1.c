/**
* \page Général 
* 
* \author LEFRANC Romain
* \version 4
* \date 26 novembre 2024
*
* Ce programme permet de déplacer un serpent dans le terminal en affichant 
* des bordures et plusieurs Pavés a éviter et des pommes a manger pour gagner 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
#include <time.h>


/*****************************************************
*               Définition des Constantes            *
*****************************************************/

/**
* \def TAILLE
* \brief définition de la taille initiale du serpent
*/
#define TAILLE 10

/**
* \def TAILLE_MAX
* \brief définition de la taille maximum du serpent
*/
#define TAILLE_MAX 21

/**
* \def X
* \brief définition de la cordonnés X
*/
#define X 40

/**
* \def Y
* \brief définition de la cordonnés Y
*/
#define Y 20

/**
* \def TETE
* \brief définition du caractére utiliser pour affciher une TETE
*/
#define TETE 'O'

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
* \brief définition de la largeur du tableau
*/
#define LARGEUR 80 

/**
* \def HAUTEUR
* \brief définition de la hauteur du tableau
*/
#define HAUTEUR 40 

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
* \def PAVE
* \brief définition de la taille d'un PAVE
*/
#define PAVE 5 

/**
* \def NB_PAVE
*               Définition des Constantes            *
* \brief définition du nombre de PAVE a afficher
*/
#define NB_PAVE 4 

/**
* \def POMME
* \brief définition du caractére utiliser pour affciher une POMME
*/
#define POMME '6' 

/**
* \def MAX_POMME
* \brief définition du nombre de POMME a manger pour finir
*/
#define MAX_POMME 10 

/**
* \def TEMPORISATION
* \brief définition de la vitesse du serpent au lancement
*/
#define TEMPORISATION 150000 

/**
* \def AUGM_VITESSE
* \brief définiton de l'augmentation de la vitese 
*/
#define AUGM_VITESSE 12500  


char plateau[HAUTEUR][LARGEUR];

/** \fn Définition des fonctions et procédures */
void afficher(int x, int y, char c);
void effacer(int x, int y);
void dessinerSerpent(int tabX[], int tabY[], int taille);
void progresser(int tabX[], int tabY[], char direction, char plateau[HAUTEUR][LARGEUR], bool *collision, bool *pommeManger, int *nbPomme);
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

/*****************************************************
*                 PROGRAMME PRINCIPAL                *
*****************************************************/

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
*
* Cette procédure permet de dessiner le serpent en commencant par une 
* tete puis le corps en fonction du nombre de pomme mangée
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
* \fn void progresser(int tabX[], int tabY[], char direction, char plateau[HAUTEUR][LARGEUR], bool *collision, bool *pommeManger, int *nbPomme)
* \brief Procédure pour faire progresser le serpent
* \param tabX : tableau des coordonnées X
* \param tabY : tableau des coordonnées Y
* \param direction : direction du serpent
* \param plateau : plateau de jeu
* \param collision : état de la collision
* \param pommeMangee : état de la pomme mangée
* \param tailleSerpent : taille actuelle du serpent
* \param nbPomme : Nombre de pomme mangée
*
* cette procédure s'occupe des collisions,
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
* \param plateau : plateau de jeu
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
* \fn void dessinerPlateau(char plateau[HAUTEUR][LARGEUR])
* \brief Affiche le plateau de jeu à l'écran
* \param plateau : plateau de jeu
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
* \fn void afficherPave(char plateau[HAUTEUR][LARGEUR], int tabX[], int tabY[])
* \brief Place des pavés (obstacles) aléatoires sur le plateau
* \param plateau : plateau de jeu
* \param tabX : tableau des coordonnées X
* \param tabY : tableau des coordonnées Y
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
        x = rand() % (LARGEUR - 3 - PAVE) + 2;
        y = rand() % (HAUTEUR - 3 - PAVE) + 2;

        hit = false;

        // Vérifier si le carré peut être placé sans toucher la bordure
        for (i = 0; i < PAVE; i++) {
            for (j = 0; j < PAVE; j++) {
                // Vérifier si la position est une bordure
                if (plateau[y + i][x + j] == BORDURE) {
                    hit = true;
                }

                // Vérifier si la position touche le serpent
                for (k = 0; k < TAILLE; k++) {
                    if (tabX[k] == (x + i) && tabY[k] == (y + j)) {
                        hit = true;
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
* \param plateau : plateau de jeu
* \param tabX : tableau des coordonnées X
* \param tabY : tableau des coordonnées Y
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