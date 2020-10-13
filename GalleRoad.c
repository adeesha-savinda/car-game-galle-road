/************************************************************************************
*   This is a project by SLIIT 2nd year students for the subject of MBS II          *
*                                      by                                           *
*                            Adeesha Savinda de Silva                               *
*                               26 September 2014                                   *
/************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "GLCD.h"
#include "DisplayElements.h"
#include "LPC17xx.H"


/*external variable*/
extern uint8_t  clock_1ms;
/*Global Variables*/
int interrupt = 0;
int highscore = 0;
unsigned short colors[] = {Blue, Navy, Maroon, Purple, Yellow, Olive};

//pause menu
int pause_menu(){
	int select = 0;
	int joy;
	int enable = 1;
	char joy_str[5];
	int exit = 0;
	interrupt = 0;
	do{
		joy = 0x0000000F & (LPC_GPIO1->FIOPIN >> 23);
		if(joy == 7 && enable){
			enable = 0;
			if(select++ >= 1){
				select = 1;
			}
		}
		else if(joy == 13 && enable){
			enable = 0;
			if(select-- <= 0){
				select = 0;
			}
		}
		else if(joy == 15 && !enable){
			enable = 1;
		}
		if(interrupt == 1){
			interrupt = 0;	
			exit = 1;
		}
		if(select == 0){
			button_high("Continue",6);
			button_low("Exit",8);
			
		}
		else if(select == 1){
			button_low("Continue",6);
			button_high("Exit",8);
		}
	}while(!exit);
	interrupt = 0;
	return select;
}

//options menu
	unsigned short options_menu(){
	int joy = 0;
	unsigned short color = 0;
	int enable = 1;
	int pos = 5;
	
	char col_str[6][10];
	strcpy(col_str[0],"Blue");
	strcpy(col_str[1],"Navy");
	strcpy(col_str[2],"Maroon");
	strcpy(col_str[3],"Purple");
	strcpy(col_str[4],"Yellow");
	strcpy(col_str[5],"Olive");
	interrupt = 0;
	GLCD_Clear(White);
	
	while(!interrupt){
		joy = 0x0000000F & (LPC_GPIO1->FIOPIN >> 23);
		if(joy == 11 && enable){
			enable = 0;
			if(color++ >= 5){
				color = 5;
			}
		}
		else if(joy == 14 && enable){
			enable = 0;
			if(color-- <= 0){
				color = 0;
			}
		}
		if(enable == 0){
			for(pos = 6 ; pos < 23 ; pos++){
				clear_car(5,pos-1,White);
				if(pos < 14){
					draw_car(5,pos,colors[color-1]);
				}
				else{
					draw_car(5,pos,colors[color]);
				}
			}
			
			pos = 5;
			enable = 1;
		}
		if(joy == 15 && !enable){
			enable = 1;
		}
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(DarkGrey);
		GLCD_DisplayString(1,2,1,"Choose Car");
		draw_car(pos,6,colors[color]);
		button_low(col_str[color],10);
	}
	for(pos = 5 ; pos > -4 ; pos-- ){
		clear_car(pos+1,6,White);
		draw_car(pos,6,colors[color]);
	}
	return color;
}

//Initialization of Systic, EINT , GLCD
void init(){
  GLCD_Init();
	GLCD_Clear(White);
	LPC_GPIO1->FIODIR &= ~(15<<23);
	SysTick_Config(SystemCoreClock/5000); //initialize the systic timer
	LPC_GPIO2->FIODIR      &= ~(1 << 10);    /* PORT2.10 defined as input       */
  LPC_GPIOINT->IO2IntEnF |=  (1 << 10);    /* enable falling edge irq         */
  NVIC_EnableIRQ(EINT3_IRQn);              /* enable irq in nvic              */
}

//The game runs here
void start(unsigned short color){
	
	uint8_t game_mode = 0; // 0 - play * 1 - pause * 2 - game over 
	int a = 0;
	
	int h_pos = 3;
	int v_pos = 0;
	int v_pos2 = 0;
	int gen_h = 3;
	int gen_h2 = 3;
	int r = color+1;
	int joy;
	int select;
	char joy_str[3];
	
	/*user car variables*/
	int mov_left = 0;
	int mov_right = 0;
	int display = 0;
	int counter = 0;
	
	/*flags*/
	int accel = 0;
	int temp = 0;
	int clear_enable = 0;
	int mode_change = 0;
	int exit = 0;
	
	/*Game Variables*/
	int score = 0;
	int lives = 3;
	int level = 1;
	int LEVEL = 3;
	int level_cnt = 300;
	
	interrupt = 0;
	GLCD_Clear(LightGrey);
	draw_road(1);
	start_count();/*count*/
	
	if(r>5){
			r-=2;
	}
	
	while(!exit){
		
		if(game_mode == 1){ /*Pause*/
			if(mode_change){
				GLCD_Clear(White);
				mode_change = 0;
			}
			GLCD_SetBackColor(White);
			GLCD_SetTextColor(DarkGrey);
			GLCD_DisplayString(3,0,1,"    Paused");
			select = pause_menu();
			if(select == 0){
				game_mode = 0;
				exit = 0;
			}
			else if(select == 1){
				game_mode = 2;
				exit = 0;
			}
			GLCD_Clear(LightGrey);	
		}
		
		else if(game_mode == 2){/*game over*/
			interrupt = 0;
			if(mode_change){
				GLCD_Clear(White);
				mode_change = 0;
			}
			game_over(score);
			if(interrupt == 1){
				game_mode = 10;
				exit = 1;
				interrupt = 0;
			}
		}

		else if(game_mode == 4){
			
		}
		
		else if(game_mode == 5){/*Game Complete*/
			interrupt = 0;
			GLCD_Clear(White);
			game_win(score);
			while(!interrupt);
			exit = 1;
			interrupt = 0;
		}
		
		else if(game_mode == 0){/*run game */
			if(interrupt ==1){
				game_mode = 1;
				mode_change = 1;
			}
			joy = 0x0000000F & (LPC_GPIO1->FIOPIN >> 23);
			GLCD_SetTextColor(White);
			
			/*Game time*/
			if(clock_1ms == 1){
				clock_1ms = 0;
				if(level_cnt-- <= 0){
					if(level--  < 0 && !accel){
						game_mode = 5;
					}
					if(level >= 0){
						level_cnt = 300;
						level_up(level,v_pos,gen_h,h_pos,color,colors,r);
					}
				}
				if(level){
					score++;
				}
				else{
					score+=5;
				}
				display = 1;
				if(counter++ >= level){
					display = 0;
					counter = 0;
				}
			}
			
			//sprintf(joy_str,"%02d",game_mode);
			//GLCD_DisplayString(8,6,1,joy_str);
		
			/*Joystick checking and moving the car*/
			if(joy == 11 && h_pos != 9){
				mov_right = 1;
				clear_enable = 1;
			}
			else if(joy == 14 && h_pos != 3){
				mov_left = 1;
				clear_enable = 1 ;
			}
			else if(joy == 13 && !accel){
				if(level){
					temp = level;
					level = 0;
					accel = 1;
					GLCD_ScrollVertical(8);
				}
			}
			else if(joy == 15 && accel){
				level = temp;
				accel = 0;
				GLCD_ScrollVertical(-8);
			}
		
			/*Code used to change the position of the car*/
			if(mov_right && h_pos < 9){
				h_pos+=2;
				if(h_pos >= 9){
					mov_right = 0;
					h_pos = 9;
				}	
			}
			else if(mov_left && h_pos > 3){
				h_pos-=2;
				if(h_pos <= 3){
					mov_left = 0;
					h_pos = 3;
				}
			}
		
			/*Code used to set the timing and display*/
			if(display == 0){
				a++;
				clear_car(v_pos -4,gen_h,LightGrey);
				if(v_pos++ >= 18){
					v_pos = 0;
					r = rand()%6;
					if(r == color){
						if(++r > 5){
							r -=2;
						}
					}
					if(rand()%2 == 1){ /*Generating Random cars*/
						gen_h = 9;
					}
					else{
						gen_h = 3;
					}
				}
			}
			if(h_pos == gen_h && v_pos-4 > 5 && v_pos-4 < 13){ /*crash*/
				v_pos = 17;
				crashed(lives,v_pos,gen_h,h_pos,color,colors,r);
				if( --lives <= 0){
					game_mode = 2;
					lives = 3;
					mode_change = 1;
				}
			}
			else if(game_mode == 0){
				/*Display the main window*/
				draw_road(a);
				if(mov_right || clear_enable){
					clear_car(9,h_pos-2,LightGrey);
				}
				if(mov_left || clear_enable){
					clear_car(9,h_pos+2,LightGrey);
				}
				if(clear_enable && (h_pos == 3 || h_pos == 9)){
					clear_enable = 0;
				}
				draw_car(v_pos - 4,gen_h,colors[r]);
				display_score(score,lives);
				draw_car(9,h_pos,colors[color]);
				if(a>1){
					a = 0; 
				}
			}
		}
	}
}
//main menu
void main_menu(){
	char str[6];
	int select = 0;
	unsigned short car_color = 0;
	uint8_t joy;
	uint8_t enable = 1;
	GLCD_SetTextColor(DarkGrey);
	GLCD_DisplayString(2,2,1,"Galle Road");
	do{
		joy = 0x0000000F & (LPC_GPIO1->FIOPIN >> 23);
		if(joy == 13 && enable){
			enable = 0;
			if(select-- <= 0){
				select = 0;
			}
		}
		else if(joy == 7 && enable){
			enable = 0;
			if(select++ >=2){
				select = 2;
			}
		}
		else if(joy == 15 && !enable){
			enable =1;
		}
		
		if(interrupt == 0){
			if(select == 0){
				button_high("Start",5);
				button_low("Options",7);
				button_low("About",9);
			}
			else if(select == 1){
				button_high("Options",7);
				button_low("Start",5);
				button_low("About",9);
			}
			else if(select == 2){
				button_low("Options",7);
				button_low("Start",5);
				button_high("About",9);
			}
			
			sprintf(str,"%05d",highscore);
			GLCD_SetBackColor(White);
			GLCD_SetTextColor(DarkGrey);
			GLCD_DisplayString(11,2,1,"High  Score");
			GLCD_DisplayString(12,5,1,str);
		}
		else{
			interrupt = 0;
			switch(select){
				case 0:
					start(car_color);
					select = 0;
					break;
				case 1:
					car_color = options_menu();
					
					break;
				case 2:
					GLCD_Clear(Black);
					do{
						display_about();
						
					}while(!interrupt);
					break;
			}
			GLCD_Clear(White);
			GLCD_SetBackColor(White);
			GLCD_SetTextColor(DarkGrey);
			GLCD_DisplayString(2,2,1,"Galle Road");
			interrupt = 0;	
		}
	}while(select != 3);
	
}
//main programme
int main(){
	init();
	main_menu();
	return 0;
}
