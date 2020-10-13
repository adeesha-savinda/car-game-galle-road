#include "GLCD.h"
#include <stdio.h>
#include <string.h>
extern int highscore;
void num_delay(int i){
	int j;
	for(j = i; j> 0 ; j--){}
}

void draw_car(int r, int c , unsigned short color){
	GLCD_SetBackColor(color);
	GLCD_DisplayChar(r,c+1,1,' ');
	GLCD_DisplayChar(r+1,c+1,1,' ');
	GLCD_DisplayChar(r+2,c+1,1,' ');
	GLCD_SetBackColor(Black);
	GLCD_DisplayChar(r+1,c,1,' ');
	GLCD_DisplayChar(r+1,c+2,1,' ');
	GLCD_DisplayChar(r+3,c,1,' ');
	GLCD_DisplayChar(r+3,c+2,1,' ');
}

void clear_car(int r, int c, unsigned short bg_color){
	GLCD_SetBackColor(bg_color);
	GLCD_DisplayChar(r,c+1,1,' ');
	GLCD_DisplayChar(r+1,c,1,' ');
	GLCD_DisplayChar(r+1,c+1,1,' ');
	GLCD_DisplayChar(r+1,c+2,1,' ');
	GLCD_DisplayChar(r+2,c+1,1,' ');
	GLCD_DisplayChar(r+3,c,1,' ');
	GLCD_DisplayChar(r+3,c+2,1,' ');
}

void draw_road(int run){
	int i;
	if(run ==1){
		for(i = 0 ; i< 13 ; i+=2){
			GLCD_SetBackColor(Black);
			GLCD_DisplayChar(i,0,1,' ');
			GLCD_DisplayChar(i,14,1,' ');
			GLCD_SetBackColor(Yellow);
			GLCD_DisplayChar(i+1,0,1,' ');
			GLCD_DisplayChar(i+1,14,1,' ');
			GLCD_SetBackColor(LightGrey);
		}
	}
	else{
		for(i = 1 ; i<= 13 ; i+=2){
			GLCD_SetBackColor(Black);
			GLCD_DisplayChar(i,0,1,' ');
			GLCD_DisplayChar(i,14,1,' ');
			GLCD_SetBackColor(Yellow);
			GLCD_DisplayChar(i-1,0,1,' ');
			GLCD_DisplayChar(i-1,14,1,' ');
			GLCD_SetBackColor(LightGrey);
		}
	}
}



void display_score(int score , int lives){
	char scr[5];
	char lvs[2];
	char hscr[5];
	char display[20];
	
	GLCD_SetBackColor(Black);
	GLCD_SetTextColor(White);
	
	sprintf(scr,"%04d",score);
	strcpy(display,"Score : ");
	strcat(display,scr);
	GLCD_DisplayString(1,4,0,display);
	sprintf(lvs,"%d",lives);
	strcpy(display,"Lives : ");
	strcat(display,lvs);
	GLCD_DisplayString(2,4,0,display);
	sprintf(hscr,"%04d",highscore);
	strcpy(display,"High Score : ");
	strcat(display,hscr);
	GLCD_DisplayString(3,4,0,display);
}

void button_high(char *caption , int row){
	char str[12];
	int i,lim,len;
	len = strlen(caption);
	lim = (11 - len)/2;
	for(i = 0 ; i< lim ; i++){
		str[i] = ' ';
	}
	strcat(str,caption);
	len = strlen(str);
	for(i = 0 ; i< 11 - len ; i++){
		str[i+len] = ' ';
	}
	GLCD_SetBackColor(Navy);
	GLCD_SetTextColor(White);
	GLCD_DisplayString(row,2,1,str);
}

void button_low(char *caption , int row){
	char str[11];
	int i,lim,len;
	len = strlen(caption);
	lim = (11 - len)/2;
	for(i = 0 ; i< lim ; i++){
		str[i] = ' ';
	}
	strcat(str,caption);
	len = strlen(str);
	for(i = 0 ; i< 11 - len ; i++){
		str[i+len] = ' ';
	}
	GLCD_SetBackColor(White);
	GLCD_SetTextColor(Black);
	GLCD_DisplayString(row,2,1,str);
}
void display_about(){
	GLCD_SetBackColor(Black);
	GLCD_SetTextColor(White);
	GLCD_DisplayString(2,3,0,"This is a game Developed by");
	GLCD_DisplayString(3,3,0,"2nd year students of SLIIT");
	GLCD_DisplayString(4,3,0,"Electronic Engineering");
	GLCD_DisplayString(7,3,0,"Adeesha Hasitha Padukka");
	GLCD_DisplayString(8,3,0,"Thank you Mayantha");
	GLCD_DisplayString(8,3,0,"Enjoy! Cheers!!");
}
void display_highscore(){
	char str[6];
	GLCD_SetBackColor(Red);
	GLCD_SetTextColor(White);
	GLCD_DisplayString(10,2,1,"    NEW    ");
	GLCD_DisplayString(11,2,1,"HIGH  SCORE");
}
void game_over(int score){
	char str[5];
	GLCD_SetBackColor(White);
	GLCD_SetTextColor(DarkGrey);
	sprintf(str,"%05d",score);
	GLCD_DisplayString(3,3,1,"Game Over");
	GLCD_DisplayString(5,5,1,"Score");
	GLCD_SetTextColor(Black);
	GLCD_DisplayString(6,5,1,str);
	
	if(score > highscore){
		highscore = score;
		display_highscore();
	}
}
void game_win(int score){
	char str[5];
	sprintf(str,"%05d",score);
	GLCD_SetBackColor(White);
	GLCD_SetTextColor(DarkGrey);
	GLCD_DisplayString(3,2,1,"  You Win  ");
	GLCD_DisplayString(5,5,1,"Score");
	GLCD_SetTextColor(Black);
	GLCD_DisplayString(6,5,1,str);
	if(score > highscore){
		highscore = score;
		display_highscore();
	}
}
void start_count(){
	int delay = 0;
	int count = 3;
	char str[2];
	//GLCD_Clear(White);
	GLCD_SetBackColor(LightGrey);
	GLCD_SetTextColor(Black);
	for(count = 3; count >= 0 ; count--){
		sprintf(str,"%d",count);
		if(count){
			GLCD_DisplayString(6,7,1,str);
		}
		else{
			GLCD_SetTextColor(Red);
			GLCD_DisplayString(6,6,1,"GO!");
		}
		num_delay(20000000);
	}
	GLCD_SetTextColor(LightGrey);
	GLCD_DisplayString(6,6,1,"GO!");
}

void display_continued(int v_pos,int gen_h,int h_pos,int color,unsigned short *colors,int r){
	GLCD_Clear(LightGrey);
	draw_road(1);
	draw_car(9,h_pos,colors[color]);
	draw_car(v_pos-4,gen_h,colors[r]);
}
void level_up(int level,int v_pos,int gen_h,int h_pos,int color,unsigned short *colors,int r){
	int count = 3;
	char str[2];
	char str2[2];
	GLCD_SetBackColor(LightGrey);
	GLCD_SetTextColor(Black);
	GLCD_DisplayString(6,3,1,"Level Up!");
	GLCD_DisplayString(7,3,1,"Level : ");
	sprintf(str2,"%d",(6-level));
	GLCD_DisplayString(7,9,1,str2);
	num_delay(30000000);
	display_continued(v_pos,gen_h,h_pos,color,colors,r);
	start_count();
}
void crashed(int lives,int v_pos,int gen_h,int h_pos,int color,unsigned short *colors,int r){
	char str[2];
	int delay = 0;
	GLCD_SetBackColor(LightGrey);
	GLCD_SetTextColor(Red);
	GLCD_DisplayString(6,3,1,"Crashed!");
	GLCD_SetTextColor(Black);
	GLCD_DisplayString(7,3,1,"Lives : ");
	sprintf(str,"%d",(lives-1));
	GLCD_SetTextColor(Red);
	GLCD_DisplayString(7,10,1,str);
	num_delay(30000000);
	display_continued(v_pos,gen_h,h_pos,color,colors,r);
	
}




