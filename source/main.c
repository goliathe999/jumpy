#include "header/SMSlib.h"
#include "header/bank1.h"
#include "string.h"
#include <stdlib.h>
#include "time.h" 


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


void game_over(){     
      unsigned char j;
      SMS_reserveSprite ();
 //  SMS_loadSpritePalette(fontpalette_bin); //palette sprite     
   	SMS_loadBGPalette(fontpalette_bin);    //palette backg  
	SMS_loadPSGaidencompressedTiles(font_psgcompr,0);    //palette backg  
    //SMS_loadTileMap(0,0,fontmap_bin,fontmap_bin_size);
   // SMS_loadTileMapArea ( 0, 0, fontmap_bin, 0,10)                 ;
     //A = 31
     
       
    int i ;
         for (i = 0 ; i < 32 ; i++){
                 for (j = 0 ; j < 24 ; j++){
            SMS_setTileatXY(i,j,1);
         }
    }
    
    char s[] = "GAME OVER MEC";     
    
    for (i = 0 ; i < sizeof(s)-1 ; i++)
    {
        if( s[i]==' '){           //espace
            SMS_setTileatXY(i,0,0); 
        }else{
          if(s[i]< 64 ){
                 SMS_setTileatXY(i,0,s[i]-32);  
          }else{
       
              SMS_setTileatXY(i,0,s[i]-34);  
          }
        }
         
    }
  
   
    
         

}
void load_titre(){
	SMS_loadBGPalette(titrepalette_bin);    //palette backg  
	SMS_loadPSGaidencompressedTiles (titrebg_psgcompr,BG_TILES_POSITION); // BG at postion 0
	SMS_loadTileMap(0,0,titremap_bin,titremap_bin_size);
}
             
             
void load_graphics2vram (){
	SMS_loadBGPalette(backgroundpalette_bin);    //palette backg
  SMS_loadSpritePalette(spritepalette_bin); //palette sprite
  
	SMS_loadPSGaidencompressedTiles (backgroundtiles_psgcompr,BG_TILES_POSITION); // BG at postion 0
  SMS_loadPSGaidencompressedTiles (spritetiles_psgcompr,SPRITE_TILES_POSITION); //sprite at positon 256
  
  SMS_loadPSGaidencompressedTiles (spritetiles2_psgcompr,SPRITE_TILES_POSITION2); // BG at postion 0
	SMS_loadTileMap(0,0,backgroundtilemap_bin,backgroundtilemap_bin_size); //bg tilemap
    
    
    
}
                                        

void draw_main_character_old() {
  unsigned char i,j;
  for(j=0;j<3;j++) {
    for(i=0;i<2;i++) {
      SMS_addSprite(noplayer_x+(i<<3),noplayer_y+(j<<3),SPRITE_TILES_POSITION+ 2*j + i); //<< 3 indique que nous faisons des décalages de 3 bits vers la gauche (oui, je sais, cela ressemble au chinois), et c'est une façon plus efficace de faire i * 8 
      //SMS_addSprite(player_x+(i*8),player_y+(j*8),SPRITE_TILES_POSITION + 2*j + i); //<< 3 indique que nous faisons des décalages de 3 bits vers la gauche (oui, je sais, cela ressemble au chinois), et c'est une façon plus efficace de faire i * 8
    }   
  }
}

      /*
void draw_main_character() { 
  unsigned char i,j;
  unsigned char direction_offset;  
  if(direction == DIRECTION_RIGHT) {
    direction_offset = 0;
  } else {
    direction_offset = NUMBER_FRAMES * NUMBER_TILES_BY_FRAME;
  }

  
  if(player_y < PLAYER_INITAL_Y){
    player_vy++;
  }

      for(j=0;j<3;j++) {
    for(i=0;i<2;i++) {
      SMS_addSprite(player_x+(i<<3),player_y+(j<<3),SPRITE_TILES_POSITION + direction_offset + current_frame * NUMBER_TILES_BY_FRAME  + NUMBER_TILES_FRAMES_BOTH_DIRECTIONS*j + i);

    }   
  }  
  
  
  if((frame_counter%16) == 0) {
    current_frame++;
    if(current_frame == NUMBER_FRAMES) {
      current_frame = 0;
    }
    

    
      
  }
}      */


void draw_main_character() { 
  unsigned char i,j,k;
  unsigned int direction_offset;
  
    for(j=0;j<3;j++) { // 3x8 de hauteur
    for(i=0;i<2;i++) { // 2 x8 de largeur
      SMS_addSprite(noplayer_x+(i<<3),noplayer_y+(j<<3),SPRITE_TILES_POSITION2+ 2*j + i); //<< 3 indique que nous faisons des décalages de 3 bits vers la gauche (oui, je sais, cela ressemble au chinois), et c'est une façon plus efficace de faire i * 8 
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
  k = 0;
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
   
 /*  load_font();
   while(1) {
    if (SMS_getKeysStatus()){     
        break;
     }
   }*/  
   
      
   
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
   //  noplayer_y-=   rand_a_b(2,20);
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
    
    
    
    //colision
    

    // collision détectée !

             /*          NUMBER_TILES_BY_FRAME
    var rect1 = {x: 2, y: 5, width: 2, height: 3}
var rect2 = {x: 2, y: 10, width: 2, height: 3}

if (rect1.x < rect2.x + rect2.width &&
   rect1.x + rect1.width > rect2.x &&
   rect1.y < rect2.y + rect2.height &&
   rect1.height + rect1.y > rect2.y) {
    // collision détectée !
}

// remplissage des valeurs =>

if (5 < 30 &&
    55 > 20 &&
    5 < 20 &&
    55 > 10) {
    // collision détectée !
}      */

    //if( noplayer_x == player_x && player_y > noplayer_y ){
    if (player_x < noplayer_x + 2 &&
   player_x + 2 > noplayer_x &&
   player_y < noplayer_y + 3 &&
   3 + player_y > noplayer_y ) {
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

void main00(){
   scroll_x = 0;
   scroll_y = 0;
   player_x = PLAYER_INITAL_X;
   player_y = PLAYER_INITAL_Y;
   current_frame = 0;
   frame_counter = 0;
   player_vy=0;
   player_vx=0;
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
 
    /* while (1){
      
        SMS_setBGScrollX(scroll_x);
        SMS_setBGScrollY(scroll_y);
        break;   
    }    */
     
        //title screen

   
    if (SMS_getKeysStatus()){
        keys = SMS_getKeysHeld();
        if(direction == DIRECTION_RIGHT) {
          if(player_x >120){            // max a droite  avant scroll 
            
          scroll_x--;
          }else{
            player_x++;
          }
        } else {
               if(player_x <56){  //max a gauche avant scroll
           scroll_x++;
          }else{
            player_x--;
          }
        }
        frame_counter++;
    }
    
                 

    
           
        keysH = SMS_getKeysHeld();
        if(keysH & PORT_A_KEY_LEFT) {
          player_vx = -1;
          direction = DIRECTION_LEFT;
        } else if(keysH == PORT_A_KEY_RIGHT) {
          player_vx = 1;
          direction = DIRECTION_RIGHT;
        } else {
          player_vx = 0;
        }
        
        keys = SMS_getKeysPressed();
        if( (keys & PORT_A_KEY_1)  && player_vy==0){
          player_vy=-1    ;
        }
        
  
     if(player_vy != 0) {
      if(player_y <= PLAYER_INITAL_Y) {
        player_y = player_y + player_vy;
      } else {
        player_y = PLAYER_INITAL_Y;
        player_vy = 0;
      }
    }
   /* if(frame_counter == 255) {
      direction = !direction;

    }*/
    
    SMS_initSprites();
    draw_main_character();
    SMS_finalizeSprites();
		SMS_waitForVBlank();
    SMS_copySpritestoSAT();    //qui copie tous les sprites dans le SAT (Sprite Attribute Table)
    SMS_setBGScrollX(scroll_x);
    SMS_setBGScrollY(scroll_y);
	}
}