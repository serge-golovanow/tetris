/*******************************************************************************
         Projet Tetris - Serge G.
  Fichier code source principal
  
  Il reste a supprimer toute les variables globales; ce code a ?t? concu dans un
  but de documentation, pas d'un codage propre et definitif. Les fichiers .h 
  sont aussi un problème a regler.
  Probleme également au niveau des collisions laterales.
  Il semble que certaines pieces se modifient en cours de route ?galement
*******************************************************************************/
/*Documentations LibSDL:
http://sol.planet-d.net/gp/
http://www.game-lab.com/?p=tuts_som&r=124&l=1
http://tastalian.free.fr/index.php?index=SDL
http://www.megastep.org/SDL/
http://prografix.games-creators.org/doc_cat/106
*/


//Chargement de la librairie SDL selon l'IDE (parait-il)
#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif
//#include <stdio.h>
#include <stdlib.h>
#include <time.h>   //Utilisé pour l'apparition "aléatoire" de la figure

#define PITCH (surface->pitch / 4)  // ? http://sol.planet-d.net/gp/ch07.html
#define WIDTH 480	//Largeur de la fenetre
#define HEIGHT 360	//Hauteur de la fenetre
#define PHYSICSFPS 25	//FPS
//La limitation des FPS permet de pas utiliser le CPU a 100%
//30FPS me semble etre suffisant pour un tetris (:
#define ROUGE 0xff0000
#define BLEU 0x0000ff
#define VERT 0x00ff00

//Tableaux de jeu/piece
int jeux=12;
int jeuy=16;
int jeu[16][12];
//La piece dans le tableau 16*12
int piece[16][12];
//La piece dans un tableau 4*4
int figure[4][4];

int timerrendu;	//Utilis? par  la limitation des FPS
int timertouches;	//Utilis? pour pas que touches() se fasse trop
int timerautodesc;  //Utilis? pour la descente de la piece

//Definition de ma variable de surface
SDL_Surface *surface;

//Variable de position de mes blocs de test
int posx=5;
int posy=0;

//Definition des variables des touches
int gKeyLeft=0;
int gKeyRight=0;
int gKeyUp=0;
int gKeyDown=0;

//Tableau en 4 dimensions des pieces
#include "pieces.h"
int pieceencours;
int prochainepiece=0;
int rotation=0;
int figurex, figurey;   //Dimensions de la piece en elle meme

//Utilis? pour ecrire du texte
#include "SFont.h" 
SFont_Font* Font;
#include "SDL/SDL_image.h"
int score=0;
int niveau=1;
int lignes=0;   //nombre de lignes effectu?s

void piecepetittab ();  //Une declaration de fonction (:
//Cette forme de #include enleve tout les probs des variables globales mais est moche
#include "fonctions.h"
#include "figures.h"


void initfont();

int main( int argc, char* argv[] )
{
    //int timerrendu;   <- debut du transformation des variables globales
    //int *pttimerrendu = &timerrendu;
    if ( Init() )
    {
        initfont();
        srand((unsigned) time(NULL));   //Initialise rand()
        prochainepiece = ( rand() % 7 ) +1;
        if (prochainepiece == 7) prochainepiece=0;
        alea();     //regeneration al?atoire de la prochaine piece
        reset();	//Initialise les tableaux de jeu
		piecepetittab();   //Passe la figure du tableau en 4dim a figure
        fond(); //allez, affiche le fond
        timerrendu = SDL_GetTicks();	//Initialisation de la variable de FPS
        timertouches = SDL_GetTicks();  // ...des touches
        while (1)
    	{		
    	    descenteauto();
        	rendu();    //gestion des pixels
			if (events()==0) return 0;	//Gestion des ?v?nements
			touches();
   		}   	
    }
    return 0;
}

void ecritscore()
{
    char textescore[10];   // = strcat("Score : " , score)
    char texteniveau[11];
    sprintf(textescore,"Score : %d",score);
    sprintf(texteniveau,"Niveau : %d",niveau);
	SFont_Write (surface, Font, 278,120,"Prochain bloc");
	SFont_Write (surface, Font, 278,150,textescore);
	SFont_Write (surface, Font, 278,180,texteniveau);
	// Update the screen
	SDL_UpdateRect(surface, 0, 0, 0, 0);
	// Let the user time to look at the font
}    

void initfont()
//Initialise l'affichage de texte
{
	Font = SFont_InitFont(IMG_Load("14P_Copperplate_Blue.png"));
	if(!Font)
	{
		//fprintf(stderr, "An error occured while loading the font.");
		exit(1);
	}
}	

int rendu()
{
    int i;
    //int gni = timerrendu;
    //printf("%d",gni);
	if (SDL_GetTicks() > timerrendu + 1000 / PHYSICSFPS)
	{
	    timerrendu = SDL_GetTicks();
	    {
            fond();	//R?initialise tout en affichant l'image de fond
            
            descente();
            somme();    //Ligne complete ?
            
            jeuasdl();  // tableau jeu -> affichage
        	figasdl();  // tableau figure -> affichage
        	prochaineasdl();    // prochaine figure -> affichage
        	ecritscore();       // score -> affichage
        }      	
    }	
    else
    {
        SDL_Delay(1);         //Attend  ms pour pas utiliser du CPU
    }    
}
