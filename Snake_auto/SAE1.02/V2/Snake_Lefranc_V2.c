/**
* \page Général
*
* \author LEFRANC Romain ET GUILLOU Malo
* \version 2
* \date 18 décembre 2024
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
#define LARGEUR 81

/**
* \def HAUTEUR
* \brief définition de la hauteur du tableau
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
* \def MAX_POMME
* \brief définition du nombre de POMME a manger pour finir
*/
#define MAX_POMME 10

/**
* \def TEMPORISATION
* \brief définition de la vitesse du serpent au lancement
*/
#define TEMPORISATION 200000

char plateau[HAUTEUR][LARGEUR];

/** \fn Définition des fonctions et procédures*/
void afficher(int x, int y, char c);
void effacer(int x, int y);
void dessinerSerpent(int tabX[], int tabY[]);
void progresser(int tabX[], int tabY[], char plateau[HAUTEUR][LARGEUR], bool*collision, bool*pommeManger, char*direction);
void initPlateau(char plateau[HAUTEUR][LARGEUR]);
void dessinerPlateau(char plateau[HAUTEUR][LARGEUR]);
void ajouterPomme(char plateau[HAUTEUR][LARGEUR], int tabX[], int tabY[]);
char calculerDirectionAuto(int tabX[], int tabY[], char verification);

// Fonctions et procédures non changés

void gotoXY(int x, int y);
int kbhit();
void disableEcho();
void enableEcho();

// Définition des variables globals
int temps = TEMPORISATION;
int nbPomme;
int lesPommesX[MAX_POMME] = {75, 75, 78, 2, 8, 78, 74, 2, 72, 5}; //entre 2 et 79
int lesPommesY[MAX_POMME] = {8, 39, 2, 2, 5, 39, 33, 38, 35, 2}; //entre 2 et 39

/*****************************************************
*                 PROGRAMME PRINCIPAL              *
*****************************************************/

int main()
{
    // definition des variables
    int x, y;
    char direction;
    int tabX[TAILLE], tabY[TAILLE];
    bool collision = false;
    bool pommeManger = false;
    int nbPomme = 0;
    int nbDeplacement = 0;

    srand(time(NULL));

    // Initialisation
    x = X;
    y = Y;

    system("clear");
    initPlateau(plateau);
    ajouterPomme(plateau, tabX, tabY);
    dessinerPlateau(plateau);

    // Traitement
    for (int i = 0; i < TAILLE; i++)
    {
        tabX[i] = x - i;
        tabY[i] = y;
    }

    disableEcho();
    dessinerSerpent(tabX, tabY);
    srand(time(NULL));
    long clock_t_begin = clock();
    while (1)
    {
        usleep(temps);
        if (pommeManger) {
                pommeManger = false;
                nbPomme++;
                
            }
        direction = calculerDirectionAuto(tabX, tabY, direction);  
        // Appelle la fonction pour faire progresser le serpent
        progresser(tabX, tabY, plateau, &collision, &pommeManger, &direction);
        nbDeplacement++;
        if (kbhit() == 1)
        {
            if (getchar() == FIN)
            {
                system("clear");
                break;
            }
        }


        if (collision) {
            system("clear");
            break;
        }
        if (nbPomme == MAX_POMME)
        {
            enableEcho();
            system("clear");
            printf("Vous avez gagné !\n");
            long clock_t_end = clock();
            double tmpsCPU = ((clock_t_end - clock_t_begin)*1.0) / CLOCKS_PER_SEC;
            printf( "Temps CPU = %.3f secondes\n",tmpsCPU);
            printf("Nombre de déplacements : %d\n",nbDeplacement);
            return 0;
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
    gotoXY(x, y);
    printf("%c", c);
    gotoXY(1, 1);
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
    gotoXY(x, y);
    printf(" ");
}

/**
* \fn void dessinerSerpent(int tabX[], int tabY[])
* \brief Procédure pour afficher le serpent
* \param tabX : tableau des colonnes
* \param tabY : tableau des lignes
* \param taille : taille actuelle du serpent
*
* Cette procédure permet de dessiner le serpent en commencant par une
* tete puis le corps en fonction du nombre de pomme mangée
*/

void dessinerSerpent(int tabX[], int tabY[])
{
    int i;
    for (i = 0; i < TAILLE; i++)
    {
        if (i == 0)
        {
            afficher(tabX[i], tabY[i], TETE); // affiche la tête
        }
        else
        {
            afficher(tabX[i], tabY[i], CORPS); // affiche le corps
        }
    }
}

/**
* \fn void progresser(int tabX[], int tabY[], char direction, char plateau[HAUTEUR][LARGEUR], bool*collision, bool*pommeManger, int*nbPomme)
* \brief Procédure pour faire progresser le serpent
* \param tabX : tableau des coordonnées X
* \param tabY : tableau des coordonnées Y
* \param direction : direction du serpent
* \param plateau : plateau de jeu
* \param collision : état de la collision
* \param pommeMangee : état de la pomme mangée
* \param TAILLE : taille actuelle du serpent
* \param nbPomme : Nombre de pomme mangée
* \param xPomme : coordonnée X de la pomme
* \param yPomme : coordonnée Y de la pomme
*
* cette procédure s'occupe des collisions,
*/

void progresser(int tabX[], int tabY[], char plateau[HAUTEUR][LARGEUR], bool*collision, bool*pommeManger, char*direction)
{
    int i, x, y;
    effacer(tabX[TAILLE - 1], tabY[TAILLE - 1]);
    // Coordonnées de la prochaine pomme
    
    if (plateau[tabY[0]][tabX[0]] == POMME)
    {
        // on augmente le compteur
        nbPomme = nbPomme + 1;

        // rajoute une pomme
      *pommeManger = true;
        plateau[tabY[0]][tabX[0]] = VIDE;
        ajouterPomme(plateau, tabX, tabY);
    }

    if (*pommeManger) {
       *direction = calculerDirectionAuto(tabX, tabY, *direction);
    }

    for (i = TAILLE; i > 0; i--)
    {
        tabX[i] = tabX[i - 1];
        tabY[i] = tabY[i - 1];
    }
    switch (*direction){
        case HAUT:
        {
            tabY[0] += -1;
            break;
        }

        case BAS:
        {
            tabY[0] += 1;
            break;
        }

        case GAUCHE:
        {
            tabX[0] += -1;
            break;
        }

        case DROITE:
        {
            tabX[0] += 1;
            break;
        }
    }
    
    if (tabY[0] == 0 && tabX[0] == LARGEUR / 2) { // Portail haut
        tabY[0] = HAUTEUR - 1;
    }
    else if (tabY[0] == HAUTEUR - 1 && tabX[0] == LARGEUR / 2) { // Portail bas
        tabY[0] = 0;
    }
    else if (tabX[0] == 0 && tabY[0] == HAUTEUR / 2) { // Portail gauche
        tabX[0] = LARGEUR - 1;
    }
    else if (tabX[0] == LARGEUR - 1 && tabY[0] == HAUTEUR / 2) { // Portail droit
        tabX[0] = 0;
    }

    // Vérifie la collision avec les bordures (hors portails)
    if ((tabX[0] == 0 || tabX[0] == LARGEUR - 1 || tabY[0] == 0 || tabY[0] == HAUTEUR - 1) &&
        !((tabX[0] == LARGEUR / 2 && (tabY[0] == 0 || tabY[0] == HAUTEUR - 1)) ||
          (tabY[0] == HAUTEUR / 2 && (tabX[0] == 0 || tabX[0] == LARGEUR - 1)))) 
    {
        *collision = true;
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

    dessinerSerpent(tabX, tabY); // dessine le serpent à la position suivante
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

void initPlateau(char plateau[HAUTEUR][LARGEUR])
{

    int x, y;
    for (y = 1; y < HAUTEUR; y++)
    {
        for (x = 1; x < LARGEUR; x++)
        {
            plateau[y][x] = VIDE;
        }
    }

    // Dessiner les bordures
    for (x = 0; x < LARGEUR; x++)
    {
        plateau[1][x] = BORDURE;
        plateau[HAUTEUR - 1][x] = BORDURE;
    }
    for (y = 0; y < HAUTEUR; y++)
    {
        plateau[y][1] = BORDURE;
        plateau[y][LARGEUR - 1] = BORDURE;
    }
    plateau[HAUTEUR / 2][1] = VIDE;              // Portail de gauche
    plateau[HAUTEUR / 2][LARGEUR - 1] = VIDE;    // Portail de droite
    plateau[1][LARGEUR / 2] = VIDE;              // Portial du haut
    plateau[HAUTEUR - 1][LARGEUR / 2] = VIDE;    // Portail du bas
}

/**
* \fn void dessinerPlateau(char plateau[HAUTEUR][LARGEUR])
* \brief Affiche le plateau de jeu à l'écran
* \param plateau : plateau de jeu
*
* Cette fonction affiche le plateau de jeu en utilisant les coordonnées des cases dans le tableau `plateau`.
* Les bordures et les espaces vides sont affichés en fonction de la valeur de chaque case.
*/

void dessinerPlateau(char plateau[HAUTEUR][LARGEUR])
{
    int x, y;
    for (y = 0; y < HAUTEUR; y++)
    {
        for (x = 0; x < LARGEUR; x++)
        {
            afficher(x, y, plateau[y][x]); // affiche le plateau de jeu
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
    if (nbPomme < MAX_POMME) {
        // Récupère la position de la prochaine pomme à partir des tableaux
        int x = lesPommesX[nbPomme];
        int y = lesPommesY[nbPomme];

        // Vérifie si la position est occupée par le serpent ou la bordure
        bool positionValide = true;
        for (int i = 0; i < TAILLE; i++) {
            if (tabX[i] == x && tabY[i] == y) {
                positionValide = false;
                break;
            }
        }

        if (positionValide && plateau[y][x] != BORDURE) {
            // Place la pomme à la position valide
            plateau[y][x] = POMME;
            afficher(x, y, POMME);
        }
    }
}

/**
* \fn char calculerDirectionAuto(int tabX[], int tabY[], char verification)
* \brief Calcule la prochaine direction automatique pour aller vers la pomme
* \param tabX : Coordonnée X de la tête du serpent
* \param tabY : Coordonnée Y de la tête du serpent
* \param xPomme : Coordonnée X de la pomme
* \param yPomme : Coordonnée Y de la pomme
* \return La direction suivante : HAUT, BAS, GAUCHE ou DROITE
*/
char calculerDirectionAuto(int tabX[], int tabY[], char verification) {
    char direction = verification;  
    int dist_reel, dist_portail_haut, dist_portail_bas, dist_portail_gauche, dist_portail_droit;
    bool col = false;
    int nextX = tabX[0];
    int nextY = tabY[0];

    dist_reel = abs(tabX[0] - lesPommesX[nbPomme]) + abs(tabY[0] - lesPommesY[nbPomme]);
    dist_portail_haut = abs(tabX[0] - (LARGEUR / 2)) + abs(tabY[0] - 0) + abs((LARGEUR / 2) - lesPommesX[nbPomme]) + abs((HAUTEUR - 1) - lesPommesY[nbPomme]);
    dist_portail_bas = abs(tabX[0] - (LARGEUR / 2)) + abs(tabY[0] - (HAUTEUR - 1)) + abs((LARGEUR / 2) - lesPommesX[nbPomme]) + abs(0 - lesPommesY[nbPomme]);
    dist_portail_gauche = abs(tabX[0] - 0) + abs(tabY[0] - (HAUTEUR / 2)) + abs((LARGEUR - 1) - lesPommesX[nbPomme]) + abs((HAUTEUR / 2) - lesPommesY[nbPomme]);
    dist_portail_droit = abs(tabX[0] - (LARGEUR - 1)) + abs(tabY[0] - (HAUTEUR / 2)) + abs(0 - lesPommesX[nbPomme]) + abs((HAUTEUR / 2) - lesPommesY[nbPomme]);


    if (dist_reel <= dist_portail_haut && dist_reel <= dist_portail_bas && dist_reel <= dist_portail_gauche && dist_reel <= dist_portail_droit) {
        if (tabX[0] < lesPommesX[nbPomme]) {
            direction = DROITE;
        } else if (tabX[0] > lesPommesX[nbPomme]) {
            direction = GAUCHE;
        } else if (tabY[0] > lesPommesY[nbPomme]) {
            direction = HAUT;
        } else if (tabY[0] < lesPommesY[nbPomme]) {
            direction = BAS;
        }
    }
    else {
        if (dist_portail_haut < dist_portail_bas && dist_portail_haut < dist_portail_gauche && dist_portail_haut < dist_portail_droit) {
            if (tabX[0] < LARGEUR / 2) {
                direction = DROITE;
            } else if (tabX[0] > LARGEUR / 2) {
                direction = GAUCHE;
            } else {
                direction = HAUT;
            }
        }
        else if (dist_portail_bas < dist_portail_haut && dist_portail_bas < dist_portail_gauche && dist_portail_bas < dist_portail_droit) {
            if (tabX[0] < LARGEUR / 2) {
                direction = DROITE;
            } else if (tabX[0] > LARGEUR / 2) {
                direction = GAUCHE;
            } else {
                direction = BAS;
            }
        }
        else if (dist_portail_gauche < dist_portail_haut && dist_portail_gauche < dist_portail_bas && dist_portail_gauche < dist_portail_droit) {
            if (tabY[0] < HAUTEUR / 2) {
                direction = BAS;
            } else if (tabY[0] > HAUTEUR / 2) {
                direction = HAUT;
            } else {
                direction = GAUCHE;
            }
        }
        else {
            if (tabY[0] < HAUTEUR / 2) {
                direction = BAS;
            } else if (tabY[0] > HAUTEUR / 2) {
                direction = HAUT;
            } else {
                direction = DROITE;
            }
        }
    }


    // Le serpent ne se touche pas
    switch (direction) {
        case HAUT: nextY -= 1; break;
        case BAS: nextY += 1; break;
        case GAUCHE: nextX -= 1; break;
        case DROITE: nextX += 1; break;
    }

    for (int i = 1; i < TAILLE; i++) {
        if (tabX[i] == nextX && tabY[i] == nextY) {
            col = true;
            break;
        }
    }

    if (col) {
        if (verification != direction) {
            if (direction == HAUT || direction == BAS) {
                direction = (tabX[0] < lesPommesX[nbPomme]) ? DROITE : GAUCHE;
            } else {
                direction = (tabY[0] < lesPommesY[nbPomme]) ? BAS : HAUT;
            }
        }
    }

    return direction;
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

    int unCaractere = 0;
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

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        unCaractere = 1;
    }
    return unCaractere;
}

/**
* \fn void disableEcho()
* \brief Désactive l'écho des caractères sur le terminal
*
* Cette fonction désactive l'écho dans le terminal, ce qui signifie que les caractères tapés par l'utilisateur ne seront pas affichés à l'écran.
*/

void disableEcho()
{
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1)
    {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Desactiver le flag ECHO
    tty.c_lflag &= ~ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1)
    {
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

void enableEcho()
{
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1)
    {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Reactiver le flag ECHO
    tty.c_lflag |= ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1)
    {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}