/*******************************************************************************
         Projet Tetris - Serge G.
  Fichier des fonctions "fixes"

*******************************************************************************/

int colorise (int idpiece)
//Renvoie une couleur differente selon la piece
{
    switch(idpiece)
    {

        case 1:
        {
            return 0xff0000;
            break;
        }
        case 2:
        {
            return 0x996633;
            break;
        }
        case 3:
        {
            return 0x9933ff;
            break;
        }
        case 4:
        {
            return 0x3333cc;
            break;
        }
        case 5:
        {
            return 0x006600;
            break;
        }
        case 6:
        {
            return 0xff9933;
            break;
        }
        case 7:
        {
            return 0x66cccc;
            break;
        }  
        case 0:
        {
            return 0x555555;
            break;
        }     
        default:
        {
            return 0xf2d6a6;
            break;
        } 
    }
}                                

void drawrect(SDL_Surface* srfce, int x, int y, int width, int height, int c)
//Dessine un rectangle: http://sol.planet-d.net/gp/ch07.html
{
    //x = (x+1)*20;	//Convert des coordonnées du tableau en coordonnées pixel
	//y = (y+1)*20;
  int i, j;
  for (i = 0; i < height; i++)
  {

    // vertical clipping: (top and bottom)
    if ((y + i) >= 0 && (y + i) < HEIGHT)
    {
      int len = width;
      int xofs = x;

      // left border
      if (xofs < 0)
      {
        len += xofs;
        xofs = 0;
      }

      // right border
      if (xofs + len >= WIDTH)
      {
        len -= (xofs + len) - WIDTH;
      }
      int ofs = (i + y) * PITCH + xofs;

      // note that len may be 0 at this point, 
      // and no pixels get drawn!
      for (j = 0; j < len; j++)
        ((unsigned int*)srfce->pixels)[ofs + j] = c;
    }
  }
}

int Init( void )
//Initialisations
{
    //Initialisation de la LibSDL
    if ( SDL_Init( SDL_INIT_VIDEO ) == -1 )
    {
        printf( "Echec lors de l'initialisation de la vidéo : %s", SDL_GetError() );
        SDL_Quit();
    }
    else
    {
        if ( surface = SDL_SetVideoMode( WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF ) )
        {
            SDL_WM_SetCaption("Projet Tetris", "SFont");
        return 1;
        }    
        else
        {
            printf( "Echec lors de la création de la fenetre : %s", SDL_GetError() );
                SDL_Quit();
        }
    }
    
    return 0;
}

int fond()
//Charge l'image: http://www.game-lab.com/?p=viewtut&id=172&l=1
{
    SDL_Rect rect;
    SDL_Surface *Background = SDL_LoadBMP( "fond.bmp" );

    // Si l'image n'a pas été trouvée, on retourne 0
    if (Background == NULL)
    {
        return 0;
    }

    //rect declaré par  SDL_Rect rect;
	rect.x = rect.y = 0;		
    rect.w = WIDTH;
    rect.h = HEIGHT;

	//"Dessinage" du fond
    SDL_BlitSurface( Background, &rect, surface, &rect);
    SDL_Flip( surface );

    SDL_FreeSurface( Background );
    return 1;
}

int events()
{
    //Gestion des évènements
   	SDL_Event event;	//Charge la gestion des évènements
    while (SDL_PollEvent(&event)) 	//pas compri la difference par rapport a while(1)
    //while (1)
    {
   	//Gestion des evenements au cas par cas
     	//SDL_WaitEvent(&event);  	//Attente d'un evenement
    	switch (event.type)
        {
            case SDL_VIDEOEXPOSE:
           	//Gestion du deplacement de la fenetre
            {
            	rendu();
                break;
     		}   	
            case SDL_KEYDOWN:
            //Gestion lors de l'enfoncement d'une touche
            {
            	switch (event.key.keysym.sym)
                {
                    case SDLK_LEFT:
                    {
                    	gKeyLeft = 1;
            			break;
            		}		
                    case SDLK_RIGHT:
                    {
            			gKeyRight = 1;
            			break;
            		}			
                    case SDLK_UP:
                    {
            			gKeyUp = 1;
            			break;
            		}
                    case SDLK_DOWN:
                    {
            			gKeyDown = 1;
                		break;
                    }
            	}    
                break;
        	}   
        
    		case SDL_KEYUP:  
	    	//Gestion d'une touche relevée
        	{
            	switch (event.key.keysym.sym)
            	{
            		case SDLK_ESCAPE:
            		{
            		    //Si il s'agit d'Esc on quitte
              			return 0;
         			}			
            		case SDLK_LEFT:
        		    {
                        gKeyLeft = 0;
                        break;
                    }   
        			case SDLK_RIGHT:
    			    {
    					gKeyRight = 0;
    					break;
    				}			
            		case SDLK_UP:
        		    {
                        gKeyUp = 0;
                        break;
                    }   
            		case SDLK_DOWN:
        		    {
                        gKeyDown = 0;
                        break;
                    }   
    			}
            break;
    		}   
        
    		//Gestion de la fermeture du programme
    		case SDL_QUIT:
		    {
            	SDL_Quit();      //decharge la SDL
     		}   	
        	return 0;	// arrete l'execution
     	}
	}
}	

void alea()
{
    pieceencours = prochainepiece;
    //srand((int) time(NULL));
    prochainepiece = rand() % 7;
}   

int tourne (int sens)
{
    if (sens == 1)
    {
        rotation++;
        if (rotation == 4) rotation = 0;
    }
    else
    {
        rotation--;
        if (rotation == -1) rotation = 3;
    }
    piecepetittab();
}  

int touches()
{
 	
    int decalage=1;
    if (SDL_GetTicks() > timertouches + 100) //Limitation a 10/sec
    {
	    timertouches = SDL_GetTicks();
	    //fond();   
	    if (gKeyDown==1 && posy<17-figurey)
	    {
		    posy = posy + decalage;
		}
	    if (gKeyLeft==1 && posx>0)
	    {
     	    posx = posx - decalage;
	    }
	    if (gKeyRight==1 && posx<(12-figurex))
	    {
	 	    posx = posx + decalage;
	 	    
	    }    
	    if (gKeyUp==1 && posy>0)
	    {
	        //posy = posy-decalage;
	        tourne(2);
	    }    
	    return 1;
    }
}  

void reset()
{
 	//(ré)initialise le tableau de jeu
 	int i;
 	int j;
 	for (i=0;i<16;i++)
 	{
	 	for (j=0;j<12;j++)
	 	{
		 	jeu[i][j] = 0;
		}
    }
    /*
    for (i=0;i<12;i++) jeu[15][i] = 1;
    jeu[15][5] = 0;
    */
    
    //(ré)initialise le tableau de la figure courante
    for (i=0;i<4;i++)
    {
        for (j=0;j<4;j++)
        {
            figure[i][j] = 0;
        }
    }        
}

