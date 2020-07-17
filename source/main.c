#include "header/SMSlib.h"#include "header/PSGlib.h"#include "header/bank1.h"#include "string.h"#include <stdlib.h>#include <stdio.h>#include "time.h" #include <math.h>#define BG_TILES_POSITION 0#define SPRITE_TILES_POSITION 256#define SPRITE_TILES_POSITION2 292#define NUMBER_FRAMES 3#define NUMBER_FRAMES_BOTH_DIRECTIONS 6#define NUMBER_TILES_BY_FRAME 2#define NUMBER_TILES_FRAMES_BOTH_DIRECTIONS 12#define PLAYER_INITAL_Y 120#define PLAYER_INITAL_X 30#define NO_PLAYER_INITAL_Y 120#define NO_PLAYER_INITAL_X 240#define DIRECTION_RIGHT 0#define DIRECTION_LEFT 1unsigned char titre_zero;unsigned int score;unsigned char scroll_x;unsigned char scroll_y;unsigned char player_x;unsigned char player_y; unsigned char check_score;signed char noplayer_vx;unsigned char noplayer_x;unsigned char noplayer_y;unsigned char noplayerdirection;unsigned char current_frame;unsigned char current_frame_mechant; unsigned char frame_counter_jump;unsigned char frame_counter;unsigned char direction;unsigned char keys;unsigned char keysH;signed char player_vx;signed char  player_vy     ;SMS_EMBED_SEGA_ROM_HEADER(0,0);SMS_EMBED_SDSC_HEADER(0,1, 2020,6,16, "goliathe", "jumpy","jumpy");void init_console(){    SMS_init();    SMS_setSpriteMode (SPRITEMODE_NORMAL);                      SMS_displayOn();}// Clear backgroundvoid fillBackground(){	unsigned int a;		SMS_setNextTileatXY (0,0);	for(a=0;a<(32*28);a++)		SMS_setTile (0);}void ClearScreen(){//les sprites	SMS_initSprites(); 	SMS_finalizeSprites(); 	SMS_copySpritestoSAT(); 		// Fill background	fillBackground();		// 	SMS_setBackdropColor(0);}void load_font(int positionTile){       	SMS_loadPSGaidencompressedTiles(font_psgcompr,positionTile);  }// ecrit un nombre nombre, nbr décimales, tilepos , x , yvoid writeNum(int num,int nbdgit,int positionTile,int xfont, int yfont){     int k =0;    while(nbdgit--)	{         SMS_setTileatXY(xfont+k,yfont,num%10+positionTile+15); 	                  num = num/10;          	 k--;          }}// ecrit une chaine chaine, tilepos , x , yvoid write(char s[],int positionTile,int xfont, int yfont){     //A = 31   unsigned int i ;    for (i = 0 ; i < strlen(s) ; i++)    {            if( s[i]==' '){           //espace            SMS_setTileatXY(xfont+i,yfont,0+positionTile);         }else{          if(s[i]< 64 ){                 SMS_setTileatXY(xfont+i,yfont,s[i]-33+positionTile);            }else{              SMS_setTileatXY(xfont+i,yfont,s[i]-34+positionTile);            }        }             }}void game_over(){  PSGStop();       SMS_setBGScrollX(0);  SMS_setBGScrollY(0);  SMS_reserveSprite ();   SMS_loadBGPalette(fontpalette_bin);    //palette backg      ClearScreen();  load_font(0);  char s[] = "GAME OVER";  write(s,140,12,10) ;  char s2[] = "SCORE";  write(s2,140,14,16) ;  writeNum(score,3,140,17,17);      }void load_titre_zero(){  SMS_setBGScrollX(0);  SMS_setBGScrollY(0);  titre_zero=1;  ClearScreen();  SMS_reserveSprite ();   SMS_loadBGPalette(fontpalette_bin);    //palette backg  load_font(140);  char s2[] = "BY GOLIATHE";  write(s2,140,14,16) ; } void load_titre(){      score=0;    if(frame_counter==0){                SMS_loadPSGaidencompressedTiles (titrebg_psgcompr,BG_TILES_POSITION); // BG at postion 0        SMS_loadTileMap(0,0,titremap_bin,titremap_bin_size);            load_font(140);    }        char s[] = "PRESS START 1";   if( frame_counter > 20){     //switch de la palette                            unsigned char titrepalette3_bin[] = {0x00,0x15,0x00,0x00,0x00,0x15,0x15,0x15,0x15,0x15,0x3f};         SMS_loadBGPalette(titrepalette3_bin);    //palette backg    }else{            unsigned char titrepalette0_bin[] = {0x00,0x2a,0x00,0x00,0x00,0x15,0x15,0x15,0x15,0x15,0x3e};        SMS_loadBGPalette(titrepalette0_bin);    //palette backg    }        write(s,140,15,18) ;    }                          void load_graphics2vram (){    SMS_loadBGPalette(backgroundpalette_bin);    //palette backg    SMS_loadSpritePalette(spritepalette_bin); //palette sprite        SMS_loadPSGaidencompressedTiles (backgroundtiles_psgcompr,BG_TILES_POSITION); // BG at postion 0    SMS_loadPSGaidencompressedTiles (spritetiles_psgcompr,SPRITE_TILES_POSITION); //sprite at positon 256        //SMS_loadPSGaidencompressedTiles (spritetiles2_psgcompr,SPRITE_TILES_POSITION2); // BG at postion 0    SMS_loadTileMap(0,0,backgroundtilemap_bin,backgroundtilemap_bin_size); //bg tilemap    load_font(140);            }                                        // un petit random On suppose a<bint rand_a_b(int a, int b){    return rand()%(b-a) +a;}//util pour les collisions typedef  struct spriteCol {    unsigned x;    unsigned y;    unsigned width ;    unsigned height  ;  } spriteCol ;          void draw_main_character() {   unsigned char i,j;  unsigned int direction_offset;           //direction du player pour repérer les bonnes tiles du sprite décale du nimbre de frams * nombre de tiles apr frame  direction_offset = 0;  if(direction == DIRECTION_RIGHT) {       direction_offset = 0;        } else {     direction_offset =  NUMBER_FRAMES * NUMBER_TILES_BY_FRAME;//     noplayer_x++;  }        //pour l'anim du mechant (2 frames)    if(frame_counter%24==0){           if(current_frame_mechant==0){ //décalage pour le sprite de 2 sur les tiles              current_frame_mechant=2;          }else{              current_frame_mechant=0;          }            }            if(score%3==0){    //ondulation du mechant      if(frame_counter%2==0){                              if( noplayer_y < 121 && noplayerdirection==0){ //monte                 noplayer_y--;                                if(noplayer_y==107 ){                      noplayerdirection=1;                                    }            }              if( noplayer_y > 106  && noplayerdirection==1){ //descent                                      noplayer_y++;                 if(noplayer_y==120 ){                      noplayerdirection=0;                  }                             }                }              }else{            //accélération  du méchant        if( noplayer_x < rand_a_b(140,160) ){ //                noplayer_x =noplayer_x-3;                       }            }      //le mechant affichage du sprite         for(j=0;j<3;j++) { // 3x8 de hauteur      for(i=0;i<2;i++) { // 2 x8 de largeur            SMS_addSprite(noplayer_x+(i<<3),noplayer_y+(j<<3),SPRITE_TILES_POSITION2+current_frame_mechant+ NUMBER_TILES_FRAMES_BOTH_DIRECTIONS*j + i); //<< 3 indique que nous faisons des décalages de 3 bits vers la gauche (oui, je sais, cela ressemble au chinois), et c'est une façon plus efficace de faire i * 8              }       }        //marcher toute les 16 frame on change le sprite qu'on décale d'un fois le nombre de tiles par frame  if((frame_counter%16) == 0) {    if(player_vx != 0) {      current_frame++;      if(current_frame == NUMBER_FRAMES) {    //NUMBER_FRAMES        current_frame = 0;      }    } else {      current_frame = 0;    }      }    //saut                 on utilise le frame_counter_jump=0 lors du saut sinon on a un décalage sur le modulo   if((frame_counter_jump%12) == 0) {        if(player_y < PLAYER_INITAL_Y  ) {            player_vy = player_vy + 1; //saut décalage de 1 de vitesse verticale          }   }    //affiche le sprite du joueur  for(j=0;j<3;j++) {      for(i=0;i<2;i++) {          SMS_addSprite(player_x+(i<<3),player_y+(j<<3),SPRITE_TILES_POSITION + direction_offset + current_frame * NUMBER_TILES_BY_FRAME  + NUMBER_TILES_FRAMES_BOTH_DIRECTIONS*j + i);       }     }}// detextion de collisions int Collision( struct spriteCol *box1, struct spriteCol *box2){          if (box1->x < box2->x + box2->width &&       box1->x + box1->width > box2->x &&       box1->y < box2->y + box2->height &&       box1->height + box1->y > box2->y) {            return 0;     }else{            return 1;    } }void main(){   unsigned int keys;     check_score=1;   scroll_x = 0;   scroll_y = 0;   //-------------------------------------------------   player_x = PLAYER_INITAL_X;   player_y = PLAYER_INITAL_Y; //120         noplayer_x = NO_PLAYER_INITAL_X;   noplayer_y = NO_PLAYER_INITAL_Y;   noplayer_vx=24;   player_vx = 0;   current_frame = 0;   current_frame_mechant=0;   frame_counter_jump=0;   frame_counter = 0;   direction = DIRECTION_RIGHT;   noplayerdirection=0;   init_console();      //titre "by goliathe" une fois seuelement  if( titre_zero <1){      load_titre_zero();      while(1) {             SMS_waitForVBlank();              keys = SMS_getKeysPressed();        if((keys & PORT_A_KEY_1) ) {            break;        }      }      }      while(1) {            load_titre();    if( frame_counter == 40)frame_counter=0;    frame_counter++;    SMS_waitForVBlank();          keys = SMS_getKeysPressed();    if((keys & PORT_A_KEY_1) ) {        break;     }   }         frame_counter=0;   load_graphics2vram();   //on scroll le fond en x y   SMS_setBGScrollX(scroll_x);   SMS_setBGScrollY(scroll_y);    PSGPlay(zikcomp_psg);        noplayer_y   =NO_PLAYER_INITAL_Y;           while(1) {                        noplayer_x=noplayer_x-rand_a_b(2,4);         if(player_vx != 0) {         //vitesse horizontale du joueur  player_x      if(player_x > 200 || player_x < 56) {        scroll_x = scroll_x - player_vx;       //scroll du décor      } else {        player_x = player_x + player_vx;       }    }      //pour le saut      if(player_y <= PLAYER_INITAL_Y) {          player_y = player_y + player_vy; //119 1118       } else {        player_y = PLAYER_INITAL_Y;        player_vy = 0;      }        frame_counter++;   //global    frame_counter_jump++;      //pour le saut     keys = SMS_getKeysHeld();    //marcher    if(keys & PORT_A_KEY_LEFT) {      player_vx = -1;      direction = DIRECTION_LEFT;    } else if(keys == PORT_A_KEY_RIGHT) {      player_vx = 1;      direction = DIRECTION_RIGHT;    } else {      player_vx = 0;    }        //sauter    keys = SMS_getKeysPressed();    if((keys & PORT_A_KEY_1) && player_y >= PLAYER_INITAL_Y) {      player_vy = -2;      frame_counter_jump= 0;      //pour le son du saut (marche pas de ouf ...)      PSGSetMusicVolumeAttenuation(15);      PSGSFXPlay(jpcomp_psg,SFX_CHANNEL3)  ;      PSGSFXFrame();    }else{      PSGSetMusicVolumeAttenuation(0); //on remet la musique a fond (je pense..)    }                           //joueur    struct  spriteCol rect1 ;             rect1.x=  player_x;    rect1.y=  player_y;    rect1.width=  16;    rect1.height   = 24;        //mechant    struct  spriteCol rect2 ;          rect2.x=  noplayer_x;    rect2.y=  noplayer_y;    rect2.width=  16;    rect2.height   = 24;        //pour le score en gros a chaque fois que le mechant dépasse le joueurs    if( noplayer_x<20 && check_score==1){      score++;      check_score=0;    }    if(noplayer_x>200){      check_score=1;    }             if(Collision(&rect1,&rect2)==0){         // collision détectée !        SMS_waitForVBlank();                game_over();        while(1) {            keys = SMS_getKeysPressed();            if((keys & PORT_A_KEY_1) ) {                             main();            }        }    }        PSGFrame();    SMS_initSprites();    draw_main_character();    SMS_finalizeSprites();    SMS_waitForVBlank();    SMS_copySpritestoSAT();    SMS_setBGScrollX(scroll_x);    SMS_setBGScrollY(scroll_y);	}}