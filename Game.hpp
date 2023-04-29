#ifndef __GAME_HPP__
#define __GAME_HPP__
#include "Const.hpp"
#include "Player.hpp"
#include "SoloGame.hpp"
#include "MultiplayerGame.hpp"
int _40lPage(){
	cleardevice();
	if(best40lines!=3600000){
		setfillcolor(0xff3f3128);
		setcolor(0xfffebb84);
		ege_fillrect(90,90,220,95);
		setfont(30,0,"Consolas");
		xyprintf(100,100,"PERSONAL BEST:");
		setfont(40,0,"Consolas");
		xyprintf(100,135,"%s",gettime(best40lines).c_str());
	}
	Button Start,Back;
	Start.setCoord(90,195);
	Start.setSize(120,60);
	Start.setColor(0xff2a211b,0xff643b1d,0xff322820,0xff3f3128,0xfffebb84);
	Start.setFont(40,"Consolas");
	Start.setTitle("Start");
	Start.setTitleCoord(10,10);
	Start.setLineWidth(4);
	Back.setCoord(4,20);
	Back.setSize(100,40);
	Back.setColor(0xff242424,0xff171717,0x2b2b2b,0xff363636,0xffffffff);
	Back.setFont(25,"Consolas");
	Back.setTitle("BACK");
	Back.setTitleCoord(10,6);
	Back.setLineWidth(4);
	int now;
	for(;is_run();delay_fps(60)){
		while(mousemsg()){
			msgRecord=getmouse();
			redraw=1;
		}
		if(redraw){
			redraw=0;
			if(Start.update(msgRecord)){now=0;break;}
			if(Back.update(msgRecord)){now=-1;break;}
		}
	}
	if(now==0) return 3;
	else return -1;
}
int blitzPage(){
	cleardevice();
	if(bestblitz!=-1){
		setfillcolor(0xff5d2a2a);
		setcolor(0xffffb8b9);
		ege_fillrect(90,90,220,95);
		setfont(30,0,"Consolas");
		xyprintf(100,100,"PERSONAL BEST:");
		setfont(40,0,"Consolas");
		xyprintf(100,135,"%d",bestblitz);
	}
	Button Start,Back;
	Start.setCoord(90,195);
	Start.setSize(120,60);
	Start.setColor(0xff3e1c1c,0xff6b2b2b,0xff4a2222,0xff5d2a2a,0xffffb8b9);
	Start.setFont(40,"Consolas");
	Start.setTitle("Start");
	Start.setTitleCoord(10,10);
	Start.setLineWidth(4);
	Back.setCoord(4,20);
	Back.setSize(100,40);
	Back.setColor(0xff242424,0xff171717,0x2b2b2b,0xff363636,0xffffffff);
	Back.setFont(25,"Consolas");
	Back.setTitle("BACK");
	Back.setTitleCoord(10,6);
	Back.setLineWidth(4);
	int now;
	for(;is_run();delay_fps(60)){
		while(mousemsg()){
			msgRecord=getmouse();
			redraw=1;
		}
		if(redraw){
			redraw=0;
			if(Start.update(msgRecord)){now=0;break;}
			if(Back.update(msgRecord)){now=-1;break;}
		}
	}
	if(now==0) return 4;
	else return -1;
}
int zenPage(){
	cleardevice();
	Button Start,Back;
	Start.setCoord(90,195);
	Start.setSize(120,60);
	Start.setColor(0xff331933,0xff6b2b69,0xff3d1e3d,0xff4c254c,0xffffb9ff);
	Start.setFont(40,"Consolas");
	Start.setTitle("Start");
	Start.setTitleCoord(10,10);
	Start.setLineWidth(4);
	Back.setCoord(4,20);
	Back.setSize(100,40);
	Back.setColor(0xff242424,0xff171717,0x2b2b2b,0xff363636,0xffffffff);
	Back.setFont(25,"Consolas");
	Back.setTitle("BACK");
	Back.setTitleCoord(10,6);
	Back.setLineWidth(4);
	int now;
	for(;is_run();delay_fps(60)){
		while(mousemsg()){
			msgRecord=getmouse();
			redraw=1;
		}
		if(redraw){
			redraw=0;
			if(Start.update(msgRecord)){now=0;break;}
			if(Back.update(msgRecord)){now=-1;break;}
		}
	}
	if(now==0) return 1;
	else return -1;
}
int solopage(){
	cleardevice();
	int mode,now=0;
	Button Fort,Blit,Zen,Tspin,Cust,Back;
	Fort.setCoord(80,100);
	Fort.setSize(240,60);
	Fort.setColor(0xff2a211b,0xff643b1d,0xff322820,0xff3f3128,0xfffebb84);
	Fort.setFont(40,"Consolas");
	Fort.setTitle("40 LINES");
	Fort.setTitleCoord(10,10);
	Fort.setLineWidth(4);
	Blit.setCoord(80,180);
	Blit.setSize(240,60);
	Blit.setColor(0xff3e1c1c,0xff6b2b2b,0xff4a2222,0xff5d2a2a,0xffffb8b9);
	Blit.setFont(40,"Consolas");
	Blit.setTitle("BLITZ");
	Blit.setTitleCoord(10,10);
	Blit.setLineWidth(4);
	Zen.setCoord(80,260);
	Zen.setSize(240,60);
	Zen.setColor(0xff331933,0xff6b2b69,0xff3d1e3d,0xff4c254c,0xffffb9ff);
	Zen.setFont(40,"Consolas");
	Zen.setTitle("ZEN");
	Zen.setTitleCoord(10,10);
	Zen.setLineWidth(4);
	Cust.setCoord(80,340);
	Cust.setSize(240,60);
	Cust.setColor(0xff1e1d2d,0xff2e2d42,0xff242336,0xff2d2b43,0xffb9b7de);
	Cust.setFont(40,"Consolas");
	Cust.setTitle("CUSTOM");
	Cust.setTitleCoord(10,10);
	Cust.setLineWidth(4);
	Tspin.setCoord(340,100);
	Tspin.setSize(240,60);
	Tspin.setColor(0xffd5d5d5,0xff8f8f8f,0xffbfbfbf,0xffb7b7b7,0xff1a1a1a);
	Tspin.setFont(40,"Consolas");
	Tspin.setTitle("T-SPIN");
	Tspin.setTitleCoord(10,10);
	Tspin.setLineWidth(4);
	Back.setCoord(4,20);
	Back.setSize(100,40);
	Back.setColor(0xff242424,0xff171717,0x2b2b2b,0xff363636,0xffffffff);
	Back.setFont(25,"Consolas");
	Back.setTitle("BACK");
	Back.setTitleCoord(10,6);
	Back.setLineWidth(4);
	for(;is_run();delay_fps(60)){
		while(mousemsg()){
			msgRecord=getmouse();
			redraw=1;
		}
		if(redraw){
			redraw=0;
			if(Fort.update(msgRecord)){now=0;break;}
			if(Blit.update(msgRecord)){now=1;break;}
			if(Zen.update(msgRecord)){now=2;break;}
			if(Cust.update(msgRecord)){now=4;break;}
			if(Tspin.update(msgRecord)){now=3;break;}
			if(Back.update(msgRecord)){now=-1;break;}
		}
	}
	Fort.hide();
	Blit.hide();
	Zen.hide();
	Tspin.hide();
	if(now==2) return zenPage();
	else if(now==0) return _40lPage();
	else if(now==1) return blitzPage();
	else if(now==3) mode=7;
	else if(now==4) mode=8;
	else mode=-1;
	return mode;
}
int mainPage(){
	int mode;
	cleardevice();
	setbkcolor(BLACK);
	setfont(36,0,"Consolas");
	setcolor(WHITE);
	setfillcolor(BLACK);
	xyprintf(80,50,"TETR.IO");
	Button Mult,Solo;
	Mult.setCoord(80,100);
	Mult.setSize(240,60);
	Mult.setColor(0xff34222d,0xff472d3d,0xff3e2936,0xff4e3343,0xffefbedc);
	Mult.setFont(40,"Consolas");
	Mult.setTitle("MULTIPLAYER");
	Mult.setTitleCoord(10,10);
	Mult.setLineWidth(4);
	Solo.setCoord(80,180);
	Solo.setSize(240,60);
	Solo.setColor(0xff1e1d2d,0xff2e2d42,0xff242336,0xff2d2b43,0xffb9b7de);
	Solo.setFont(40,"Consolas");
	Solo.setTitle("SOLO");
	Solo.setTitleCoord(10,10);
	Solo.setLineWidth(4);
	Mult.show(0);
	Solo.show(0);
	int now=1;
	for(;is_run();delay_fps(60)){
		while(mousemsg()){
			msgRecord=getmouse();
			redraw=1;
		}
		if(redraw){
			redraw=0;
			if(Mult.update(msgRecord)){now=1;break;}
			if(Solo.update(msgRecord)){now=0;break;}
		}
	}
	Mult.hide();
	Solo.hide();
	cleardevice();
	if(now==0) return solopage();
	else return 2;
}
void playgame(){
	hello();
	while(1){
		int mode=mainPage();
		if(mode==-1) continue;
		cleardevice();
		setlinewidth(1);
		if(mode==2){
			MultiGame game;
			game.mode=mode;
			game.init();
			game.play();
		}else{
			SoloGame game;
			game.mode=mode;
			game.init();
			game.play();
		}
//		game.P1=Player(4,20,200,400,16,true);
//		game.P2=Player(10,20,680,400,16,true);
//		game.init();
//		game.play();
	}
}
#endif
