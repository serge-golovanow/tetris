/*******************************************************************************
         Projet Tetris - Serge G.
  Fichier des figures du jeu (vite fait)

*******************************************************************************/
void prochaineasdl()
{
    //affiche la prochaine piece
    int i;
    int j;
    int x,y;
 	for (i=0;i<4;i++)
 	{
        for (j=0;j<4;j++)
	    {
	        //if (pieces[0][1][j][i]==1)
	  	    if (figures[prochainepiece][0][j][i]>=1)
	  	    {	
                x = (i+14)*20+10;	//Convert des coordonnées du tableau en coordonnées pixel
    	        y = (j+1)*20+10;	  		  
	            drawrect(surface,x,y,20,20,colorise(figures[prochainepiece][0][j][i]));
		    }
		}
    }
    SDL_UpdateRect(surface, 0, 0, WIDTH, HEIGHT); 
}

void figasdl()
{
    //affiche la piece
 	 int i;
 	 int j;
 	 int x,y;
 	 for (i=0;i<12;i++)
 	 {
	     for (j=0;j<16;j++)
	     {
		  	 //if (pieces[0][1][j][i]==1)
	  	     if (piece[j][i]>=1)
	  	     {	
                 x = (i+1)*20;	//Convert des coordonnées du tableau en coordonnées pixel
    	         y = (j+1)*20;	  		  
	             drawrect(surface,x,y,20,20,colorise(piece[j][i]));
		     }
		 }
    }
    SDL_UpdateRect(surface, 0, 0, WIDTH, HEIGHT); 
}

void jeuasdl()
{
    //affiche les anciennes pieces
 	 int i;
 	 int j;
 	 int x,y;
 	 for (i=0;i<12;i++)
 	 {
	     for (j=0;j<16;j++)
	     {
	  	     if (jeu[j][i]>=1)
	  	     {	
    	         x = (i+1)*20;	//Convert des coordonnées du tableau en coordonnées pixel
    	         y = (j+1)*20;	  
	             drawrect(surface,x,y,20,20,colorise(jeu[j][i]));
	             //drawrect(surface,i,j,20,20,BLEU);
		     }
		 }
    }
    SDL_UpdateRect(surface, 0, 0, WIDTH, HEIGHT); 
}

          

void piecepetittab ()
{
    /*
    prend la piece du tableau en 4dim pour la passer dans un 2dim en recuperant 
    ses dimensions (pour les collisions). Trop beau l'algo!
    */
    int i,j;
	int imin=5;
	int jmin=5;
	int imax=-1;
	int jmax=-1;
    for (i=0;i<4;i++)
    {
	 	for (j=0;j<4;j++)
	 	{
		 	if (figures[pieceencours][rotation][j][i] >= 1)
		 	{
	      	    if (j<jmin) { jmin = j; }
				if (j>jmax) { jmax = j; }
				
				if (i<imin) { imin = i; }
				if (i>imax) { imax = i; }
			}
	    }
	}
	figurex = imax - imin + 1;
	figurey = jmax - jmin + 1;
	
	for (i=0;i<figurex;i++)
	{
	 	for (j=0;j<figurey;j++)
	 	{
		 	figure[j][i] = figures[pieceencours][rotation][j+jmin][i+imin];
	    }
	}
  		
}

void descente()
{
 	 int i;
 	 int j;
 	 int colli=collision();
 	 switch (colli)
 	 {
 		 case 0:
	 	 {
         //Pas de collision, on descend en inserant le figure dans piece
		 	 for(i=0;i<jeux;i++)
		 	 {
				 //jeu[1][1] = 1;
				 for(j=0;j<jeuy;j++)
				 {
				     if (i>=posx && j>=posy && j-posy<figurey && i-posx<figurex) 
					 { 
  			 		   
  			 		     //piece[j][i] = figures[0][0][j-posy][i-posx];
					 	 //piece[j][i] = figureA[j-posy][i-posx];
					 	 piece[j][i] = figure[j-posy][i-posx];
					 }
					 else
					 {
					  	 piece[j][i]=0;
					 }
				 }
		    }
		    break;
	    }
	    case 1:
	    {
	        //collision, on reprend une nouvelle piece
	        score++;    //incremente le score
		 	posy = 0;
		 	posx = 5;
		 	rotation = 0;
		 	//reset();
            alea();
		 	piecepetittab();
		 	prochaineasdl();
		 	for(i=0;i<jeux;i++)
	 	 	{
			    //jeu[1][1] = 1;
				for(j=0;j<jeuy;j++)
				{
				    if (jeu[j][i]==0) jeu[j][i] = piece[j][i];
	   			    //jeu[j][i] = jeu[j][i] | piece[j][i];
	   			    piece[j][i] = 0;
				}
	        }
	        break;
	    }
	    case 2:
		{
		    //game over !
		    reset();
		    alea();
		    piecepetittab();
		 	break;
		 	score = 0;
		 	niveau = 1;
	    } 
	}
}

int collision()
//Detecte les collisions (pas encore parfait)
{
 	int i;
 	int j;
 	if (posy+figurey == 17) return 1;
 	for(i=0;i<jeux;i++)
 	{
	    for(j=jeuy-2;j>=0;j--)
	    {
  		    if (piece[j][i] && jeu[j+1][i])
  		    {
 				//if (j<figurey-1)
 				if (posy == 0)
                {
		   		    return 2;
		        }
		        else
		        { 	
			    	return 1;
		        }
		    }
		}
    }
    return 0;
}

void decallage(int origine)
{
    //Decalle toutes les lignes si une est complete (appelé par somme())
 	int i;
 	int j;
 	for(i=origine;i>0;i--)
 	{
	    for (j=0;j<jeux;j++)
	    {
		 	jeu[i][j] = jeu[i-1][j];
		}
    }
    for (j=0;j<jeux;j++)
    {
	 	jeu[0][j] = 0;
    }
}



void somme()
{
    int compteur=0;
 	int i;
 	int j;
 	int k;
 	int somme;
 	int zero=0;
 	for (i=15;i>=0;i--)
 	{
	 	do
	 	{
	  	    zero = 0;
	  	    for(j=0;j<12;j++)
	  	    {
	  	        if (jeu[i][j] == 0) zero = 1;
 	 		    //somme = somme + jeu[i][j];
			}
			if (zero==0) 
			{
			    decallage(i);
			    compteur++;
			    alea();
			}
		}while(zero==0);
    }
    score += compteur*niveau*5;
    if (lignes == 10)
    {
        lignes = 0;
        niveau++;
    }    
}

void descenteauto()
{
    int coli;
    if (SDL_GetTicks() > timerautodesc + (750-10*niveau) )
    {
	    timerautodesc = SDL_GetTicks();
	    coli = collision();
	    if (coli == 0 ) posy++;
    }    
}    
