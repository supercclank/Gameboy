#include <stdlib.h>
#include <stdio.h>
#include "mylib.h"
#include "text.h"
#include "Splash.h"
#include "jetpacman.h"
#include "shippart1.h"
#include "shippart2.h"
#include "shippart3.h"
#include "background.h"
#include "shippart1.h"
#include "shippart2.h"
#include "shippart3.h"
#include "bluefireball.h"
#include "pinkfireball.h"
#include "redfireball.h"
#include "gplatform1.h"
#include "gplatform2.h"
#include "yplatform.h"
#include "losescreen.h"
#include "winscreen.h"
enum {SPLASH, GAME, WIN, LOSE};
// Prototypes
int splash();
int game(int);
void win();
void lose();
void initializeFlameList(flame *flames, int numberOfFlames);
//loop of main, used t oset states of game
int main()
{
	int seed = 0;
	int state = SPLASH;
	while(1)
	{
		switch(state)
		{
		case SPLASH:
			seed = splash();
			state = GAME;
			break;
		case GAME:
			state = game(seed);
			break;
		case WIN:
			win();
			state = SPLASH;
			break;
		case LOSE:
			lose();
			state = SPLASH;
			break;
		default:
			// handle error
			break;
		} // switch
	} // while
}
//function that displays splashscreen and waits for user to continue	
int splash()
{
	int seed = 0;
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	drawPicture(jetpac_title);
	drawString(150, 5, "Press Start To Begin (Enter)", YELLOW);
	while(!KEY_DOWN_NOW(BUTTON_START))
	{
		seed++;
	}
	while(KEY_DOWN_NOW(BUTTON_START));
	return seed;
}	
//function that displays the win screen	
void win()
{
	drawPicture(winscreen);
	drawString(100,15,"YOU WIN! Press START to play again.",BLACK);
	while(!KEY_DOWN_NOW(BUTTON_START));
	while(KEY_DOWN_NOW(BUTTON_START));
}
//function that displays the lose screen
void lose()
{
	drawPicture(losescreen);
	drawString(100,35,"Press START to play again.",YELLOW);
	while(!KEY_DOWN_NOW(BUTTON_START));
	while(KEY_DOWN_NOW(BUTTON_START));
}
//game loop
int game(int seed)
{
	//initialization of variables to be used
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	fillScreen(BLACK);
	int numberOfFlames = 6;
	jetman jet = {120,125,JETPACMAN_WIDTH, JETPACMAN_HEIGHT,0,(u16*)Jetpacman};
	shippart part1 ={190,18,SHIP_PART1_WIDTH, SHIP_PART1_HEIGHT,0,(u16*)ship_part1,0,0};
	shippart part2 ={45,40,SHIP_PART2_WIDTH, SHIP_PART2_HEIGHT,0,(u16*)ship_part2,0,0};
	shippart part3 ={180,135,SHIP_PART3_WIDTH, SHIP_PART3_HEIGHT,0,(u16*)ship_part3,1,0};
	flame flames[numberOfFlames];
	initializeFlameList(flames, numberOfFlames);
	platform greenplatform1={25,55,GPLATFORM1_WIDTH,GPLATFORM1_HEIGHT,(u16*)gplatform1};
	platform greenplatform2={175,35,GPLATFORM2_WIDTH,GPLATFORM2_HEIGHT,(u16*)gplatform2};
	platform yellowplatform={0,152,YPLATFORM_WIDTH,YPLATFORM_HEIGHT,(u16*)yplatform};
	int oldy, oldx;
	int livesLeft = 3;
	//main loop
	while(1)
	{
	//section waits for input and performs relevant actions
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
			return SPLASH;
		}
		if (KEY_DOWN_NOW(BUTTON_LEFT) && jet.x > 0) {
			jet.x--;
			if (part1.beingCarried) {
			part1.x--;
			}
			if (part2.beingCarried) {
			part2.x--;
			}
			drawRect(jet.x + 1, jet.y, 15, 15, BLACK);
		}

		if (KEY_DOWN_NOW(BUTTON_RIGHT) && jet.x + jet.width < 240) {
			jet.x++;
			if (part1.beingCarried) {
			part1.x++;
			}
			if (part2.beingCarried) {
			part2.x++;
			}
		}

		if (KEY_DOWN_NOW(BUTTON_UP) && jet.y > 0) {
			jet.y--;
			if (part1.beingCarried) {
			part1.y--;
			drawRect(part1.x , part1.y + 1, 20, 20, BLACK);
			}
			if (part2.beingCarried) {
			part2.y--;
			drawRect(part2.x , part2.y + 1, 20, 20, BLACK);
			}
		}

		if (
			(KEY_DOWN_NOW(BUTTON_UP) == 0)
			 && 
			 (jet.y + jet.height != yellowplatform.y)
			  &&
		!((jet.x + jet.width >= greenplatform1.x)&&(jet.x <= greenplatform1.x + greenplatform1.width)&&(jet.y + jet.height == greenplatform1.y)
			)
		&&
		!((jet.x + jet.width >= greenplatform2.x)&&(jet.x <= greenplatform2.x + greenplatform2.width)&&(jet.y + jet.height == greenplatform2.y)
			)
		)
		 {
			jet.y++;
			if (part1.beingCarried) {
			part1.y++;
			}
			if (part2.beingCarried) {
			part2.y++;
			}
		}
		//checks to see if part 2 can be picked up
		if(part2.inPlace == 0 && (jet.x + part2.width >= part2.x)&&(jet.x <= part2.x + part2.width)&&(jet.y + jet.height >= part2.y)&&(jet.y <= part2.y + part2.height) && (part1.inPlace)){
			part2.beingCarried = 1;
		}
		//checks to see if part 1 can be picked up
		if(part1.inPlace == 0 &&(jet.x + part1.width >= part1.x)&&(jet.x <= part1.x + part1.width)&&(jet.y + jet.height >= part1.y)&&(jet.y <= part1.y + part1.height)){
			
			part1.beingCarried = 1;
		}
		//checks for dropping off parts
		if((jet.x + part3.width >= part3.x)&&(jet.x <= part3.x + part3.width)&&(jet.y + jet.height >= part3.y)&&(jet.y <= part3.y + part3.height)){
			if (part1.beingCarried){
				oldx = part1.x;
				oldy = part1.y;
				part1.beingCarried = 0;
				part1.x = part3.x - 1;
				part1.y = part3.y - part1.height + 2;
				part1.inPlace = 1;
				drawRect(oldx , oldy, 20, 20, BLACK);
			}
			if (part2.beingCarried){
				oldx = part2.x;
				oldy = part2.y;
				part2.beingCarried = 0;
				part2.x = part1.x + 4;
				part2.y = part1.y - part2.height + 2;
				part2.inPlace = 1;
				drawRect(oldx , oldy, 20, 20, BLACK);
			}
		}
		//check for lives left
		if (livesLeft == 3) {
		drawRect(0,0,10,45,BLACK);
		drawString(0,0, "3 Lives", WHITE);
		}
		if (livesLeft == 2) {
		drawRect(0,0,10,45,BLACK);
		drawString(0,0, "2 Lives", WHITE);
		}
		if (livesLeft == 1) {
		drawRect(0,0,10,45,BLACK);
		drawString(0,0, "1 Lives", WHITE);
		}
		//draw all the things
		drawImage3(part3.x, part3.y, part3.width, part3.height, part3.imageArray);
		drawImage3(part1.x,part1.y, part1.width, part1.height, part1.imageArray);
		drawImage3(part2.x, part2.y, part2.width, part2.height, part2.imageArray);
		drawImage3(greenplatform1.x, greenplatform1.y, greenplatform1.width, greenplatform1.height, greenplatform1.imageArray);
		drawImage3(greenplatform2.x, greenplatform2.y, greenplatform2.width, greenplatform2.height, greenplatform2.imageArray);
		drawImage3(yellowplatform.x, yellowplatform.y, yellowplatform.width, yellowplatform.height, yellowplatform.imageArray);
		drawImage3(jet.x,jet.y, jet.width,jet.height, jet.imageArray);
		//loop that moves flames
		int q;
		for (q =0; q <numberOfFlames; q++) {
			if (flames[q].displayed) {
				oldx = flames[q].x;
				oldy = flames[q].y;
				drawRect(oldx, oldy, 15, 20, BLACK);
				if (flames[q].x > -1 && flames[q].x < 240 - flames[q].width) {
				flames[q].x = flames[q].x + flames[q].direction;
				drawImage3(flames[q].x,flames[q].y,flames[q].width,flames[q].height,flames[q].imageArray);
				} else {
					flames[q].displayed = 0;
				}
				//collision detection with flames
		if((jet.x + flames[q].width >= flames[q].x)&&(jet.x <= flames[q].x + flames[q].width)&&(jet.y + jet.height >= flames[q].y)&&(jet.y <= flames[q].y + flames[q].height)){
							livesLeft--;
					flames[q].displayed = 0;
					drawRect(flames[q].x, flames[q].y, 15, 20, BLACK);
				}

					}

		}
		//spawns new flames	
		int r = rand() % 10;
		int i;
		int found;
		if (r == 0){
			found = 1;
			for (i=0; i<numberOfFlames && found; i++) {
				if (flames[i].displayed == 0) {
					flames[i].displayed =1;
					int rr = rand() % 100;
					
					if (rr < 50) {
						flames[i].direction = 1;
						flames[i].x = 0;
					} else {
						flames[i].direction = -1;
						flames[i].x = 220-flames[i].width;
					}
					int qr = rand() % (160 - yellowplatform.height - flames[i].height);
					flames[i].y = qr;
					found = 0;
				}
			}
		}
		//check win/lose conditions
		if (part1.inPlace && part2.inPlace && part3.inPlace) {
			return WIN;
		}
		if (livesLeft == 0) {
			return LOSE;
		}
		waitForVblank();
	}
}
//function that initializes the flames array	
void initializeFlameList(flame *flames, int numberOfFlames) {
	int f;
	for (f = 0; f < numberOfFlames/3; f++) {
		flames[f*3].x=0;
		flames[f*3].y=0;
		flames[f*3].width = BLUEFIREBALL_WIDTH;
		flames[f*3].height= BLUEFIREBALL_HEIGHT;
		flames[f*3].displayed = 0;
		flames[f*3].direction = 0;
		flames[f*3].imageArray = (u16*)bluefireball;

		flames[f*3+1].x=0;
		flames[f*3+1].y=0;
		flames[f*3+1].width = BLUEFIREBALL_WIDTH;
		flames[f*3+1].height= BLUEFIREBALL_HEIGHT;
		flames[f*3+1].displayed = 0;
		flames[f*3+1].direction = 0;
		flames[f*3+1].imageArray = (u16*)redfireball;

		flames[f*3+2].x=0;
		flames[f*3+2].y=0;
		flames[f*3+2].width = BLUEFIREBALL_WIDTH;
		flames[f*3+2].height= BLUEFIREBALL_HEIGHT;
		flames[f*3+2].displayed = 0;
		flames[f*3+2].direction = 0;
		flames[f*3+2].imageArray = (u16*)pinkfireball;
		
	}
}
	