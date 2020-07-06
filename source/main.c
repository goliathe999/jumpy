#include "header/SMSlib.h"
#include "header/bank1.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>
#include "time.h" 

#include <math.h>

#define BG_TILES_POSITION 0
#define SPRITE_TILES_POSITION 256
#define SPRITE_TILES_POSITION2 292


#define NUMBER_FRAMES 3
#define NUMBER_FRAMES_BOTH_DIRECTIONS 6
#define NUMBER_TILES_BY_FRAME 2
#define NUMBER_TILES_FRAMES_BOTH_DIRECTIONS 12

#define PLAYER_INITAL_Y 120
#define PLAYER_INITAL_X 30


#define NO_PLAYER_INITAL_Y 120
#define NO_PLAYER_INITAL_X 240


#define DIRECTION_RIGHT 0
#define DIRECTION_LEFT 1

unsigned char scroll_x;
unsigned char scroll_y;

unsigned char player_x;
unsigned char player_y;


signed char noplayer_vx;

unsigned char noplayer_x;
unsigned char noplayer_y;

unsigned char current_frame;

unsigned char frame_counter;

unsigned char direction;
unsigned char keys;
unsigned char keysH;
signed char player_vx;
signed char  player_vy     ;

SMS_EMBED_SEGA_ROM_HEADER(0,0);
SMS_EMBED_SDSC_HEADER(0,1, 2020,6,16, "mkoch", "Test","test");


void init_console(){
    SMS_init();
    SMS_setSpriteMode (SPRITEMODE_NORMAL);
                  SMS_displayOn();
}


// Clear background
void fillBackground()
{
	unsigned int a;
	
	SMS_setNextTileatXY (0,0);
	for(a=0;a<(32*28);a++)
		SMS_setTile (0);
}


void ClearScreen()
{
	// Los sprites
	SMS_initSprites(); 
	SMS_finalizeSprites(); 
	SMS_copySpritestoSAT(); 
	
	// Fill background
	fillBackground();
	
	// El background
	SMS_setBackdropColor(0);
}

void load_font(int positionTile){
       	SMS_loadPSGaidencompressedTiles(font_psgcompr,positionTile);  
}



void writeNum(int num,int nbdgit,int positionTile,int xfont, int yfont){
     int k =0;
    while(nbdgit--)
	{ 
        SMS_setTileatXY(xfont+k,yfont,num%10+positionTile+15); 	      
            num = num/10;
          	 k--;
      
    }
}


void write(char s[],int positionTile,int xfont, int yfont){
     //A = 31
   unsigned int i ;
    for (i = 0 ; i < strlen(s) ; i++)
    {
        if( s[i]==' '){           //espace
            SMS_setTileatXY(xfont+i,yfont,0+positionTile); 
        }else{
          if(s[i]< 64 ){
                 SMS_setTileatXY(xfont+i,yfont,s[i]-33+positionTile);  
          }else{
              SMS_setTileatXY(xfont+i,yfont,s[i]-34+positionTile);  
          }
        }
         
    }
}


void game_over(){     

  SMS_reserveSprite (); 
  SMS_loadBGPalette(fontpalette_bin);    //palette backg  
  
  ClearScreen();
  load_font(0);
  char s[] = "GAME OVER";
  write(s,140,12,20) ;
      

}




void load_titre(){
	SMS_loadBGPalette(titrepalette_bin);    //palette backg  
	SMS_loadPSGaidencompressedTiles (titrebg_psgcompr,BG_TILES_POSITION); // BG at postion 0
	SMS_loadTileMap(0,0,titremap_bin,titremap_bin_size);
    load_font(140);               
    char s[] = "PRESS START 1";    
    write(s,140,15,15) ;
    
            
     
   
}
             
             
void load_graphics2vram (){
    SMS_loadBGPalette(backgroundpalette_bin);    //palette backg
    SMS_loadSpritePalette(spritepalette_bin); //palette sprite
    
    SMS_loadPSGaidencompressedTiles (backgroundtiles_psgcompr,BG_TILES_POSITION); // BG at postion 0
    SMS_loadPSGaidencompressedTiles (spritetiles_psgcompr,SPRITE_TILES_POSITION); //sprite at positon 256
    
    //SMS_loadPSGaidencompressedTiles (spritetiles2_psgcompr,SPRITE_TILES_POSITION2); // BG at postion 0
    SMS_loadTileMap(0,0,backgroundtilemap_bin,backgroundtilemap_bin_size); //bg tilemap
    load_font(140);    
    
    
}
                                        


void draw_main_character() { 
  unsigned char i,j,k;
  unsigned int direction_offset;
  
             //le mechant
    for(j=0;j<3;j++) { // 3x8 de hauteur
    for(i=0;i<2;i++) { // 2 x8 de largeur
        if(noplayer_x%10==0){
            k=2;
        }else{
            k=0;
        }
      SMS_addSprite(noplayer_x+(i<<3),noplayer_y+(j<<3),SPRITE_TILES_POSITION2+k+ NUMBER_TILES_FRAMES_BOTH_DIRECTIONS*j + i); //<< 3 indique que nous faisons des décalages de 3 bits vers la gauche (oui, je sais, cela ressemble au chinois), et c'est une façon plus efficace de faire i * 8 
      //SMS_addSprite(player_x+(i*8),player_y+(j*8),SPRITE_TILES_POSITION + 2*j + i); //<< 3 indique que nous faisons des décalages de 3 bits vers la gauche (oui, je sais, cela ressemble au chinois), et c'est une façon plus efficace de faire i * 8
    }   
  }
  

  
  
  direction_offset = 0;
  if((frame_counter%16) == 0) {
    
    if(player_vx != 0) {
      current_frame++;
      if(current_frame == NUMBER_FRAMES) {    //NUMBER_FRAMES
        current_frame = 0;
      }
    } else {
      current_frame = 0;
    }

    if(player_y < PLAYER_INITAL_Y) {
      player_vy = player_vy + 1; //saut décalage de 1 de vitesse verticale
    }

    
  }
  if(direction == DIRECTION_RIGHT) { //direction du player
      direction_offset = 0;
  } else {
     direction_offset =  NUMBER_FRAMES * NUMBER_TILES_BY_FRAME;
  }
  

  
  for(j=0;j<3;j++) {
    for(i=0;i<2;i++) {
      SMS_addSprite(player_x+(i<<3),player_y+(j<<3),SPRITE_TILES_POSITION + direction_offset + current_frame * NUMBER_TILES_BY_FRAME  + NUMBER_TILES_FRAMES_BOTH_DIRECTIONS*j + i);
 
    }   
  }
}



// On suppose a<b
int rand_a_b(int a, int b){
    return rand()%(b-a) +a;
}



 typedef  struct spriteCol {
    unsigned x;
    unsigned y;
    unsigned width ;
    unsigned height  ;
  } spriteCol ;          

 

int Collision( struct spriteCol *box1, struct spriteCol *box2)
{


     /*     writeNum(box1->x,3,140,15,10);
          writeNum(box1->y,3,140,15,11);
          writeNum(box2->x,3,140,15,12);
          writeNum(box2->y,3,140,15,13);
          */
          
if (box1->x < box2->x + box2->width &&
   box1->x + box1->width > box2->x &&
   box1->y < box2->y + box2->height &&
   box1->height + box1->y > box2->y) {
          return 0; 
   }else{
          return 1;
          } 
}


void main(){
   unsigned int keys;
   scroll_x = 0;
   scroll_y = 0;
   //-------------------------------------------------
   player_x = PLAYER_INITAL_X;
   player_y = PLAYER_INITAL_Y;
   
      noplayer_x = NO_PLAYER_INITAL_X;
   noplayer_y = NO_PLAYER_INITAL_Y;
   noplayer_vx=24;
   player_vx = 0;
   current_frame = 0;
   frame_counter = 0;
   direction = DIRECTION_RIGHT;
   
   init_console();
   
      
   
   load_titre();
   while(1) {
    if (SMS_getKeysStatus()){     
        break;
     }
   }    
   load_graphics2vram();
   SMS_setBGScrollX(scroll_x);
   SMS_setBGScrollY(scroll_y);

 
 

        
   while(1) {

    noplayer_x=noplayer_x-rand_a_b(2,5);
   if( noplayer_x %20==0)
    noplayer_y = rand_a_b(NO_PLAYER_INITAL_Y-20,NO_PLAYER_INITAL_Y); ;
    
    if(player_vx != 0) {         //vitesse horizontale du joueur  player_x
      if(player_x > 200 || player_x < 56) {
        scroll_x = scroll_x - player_vx;
      } else {
        player_x = player_x + player_vx; 
      }
    }

    if(player_vy != 0) {
      if(player_y <= PLAYER_INITAL_Y) {
        player_y = player_y + player_vy;
      } else {
        player_y = PLAYER_INITAL_Y;
        player_vy = 0;
      }
    }
    
    frame_counter++;
    keys = SMS_getKeysHeld();
    if(keys & PORT_A_KEY_LEFT) {
      player_vx = -1;
      direction = DIRECTION_LEFT;
    } else if(keys == PORT_A_KEY_RIGHT) {
      player_vx = 1;
      direction = DIRECTION_RIGHT;
    } else {
      player_vx = 0;
    }
    keys = SMS_getKeysPressed();
    if((keys & PORT_A_KEY_1) && player_vy == 0) {
      player_vy = -2;
    }
       

    struct  spriteCol rect1 ;         
    rect1.x=  player_x;
    rect1.y=  player_y;
    rect1.width=  2;
    rect1.height   = 3;
    
    struct  spriteCol rect2 ;      
    rect2.x=  noplayer_x;
    rect2.y=  noplayer_y;
    rect2.width=  16;
    rect2.height   = 24;

 
      /*writeNum(player_x,3,140,15,10);
      writeNum(player_y,3,140,15,11);
      writeNum(noplayer_x,3,140,15,12);
      writeNum(noplayer_y,3,140,15,13);                 */
 

   if(Collision(&rect1,&rect2)==0){
    // collision détectée !

        SMS_waitForVBlank();
        game_over();
        while(1) {
            if (SMS_getKeysStatus()){     
                 main();
            }
        }
    }
    
    
    SMS_initSprites();
    draw_main_character();
    SMS_finalizeSprites();
		SMS_waitForVBlank();
    SMS_copySpritestoSAT();
    SMS_setBGScrollX(scroll_x);
    SMS_setBGScrollY(scroll_y);
	}
}
