#ifndef __SOLOGAME_HPP__
#define __SOLOGAME_HPP__
#include "Const.hpp"
#include "Button.hpp"
#include "Player.hpp"
struct SoloGame{
	int mode;double speed;
	int starttime,lastturn,lastblock,lastinc;
	Player P1;
	Game(){}
	void initSettings(){
		cleardevice();
		EditText boardW,boardH;
		Button Start;
		setfont(30,0,"Consolas");
		setcolor(WHITE);
		xyprintf(100,100,"board width:");
		xyprintf(100,140,"board height:");
		ege_rectangle(90,90,320,90);
		boardW.setMaxLen(2);
		boardW.setNumOnly(true);
		boardW.setCursorWidth(1);
		boardW.setCoord(300,100);
		boardW.setSize(100,30);
		boardW.setTextCoord(2,0);
		boardW.setFont(30,"Consolas");
		boardW.setCharWidth(15);
		boardW.setText("10");
		boardW.setCursorPos(2);
		boardW.setLineWidth(1);
		boardW.setColor(0xff000000,0xffcfcfcf,0xffffffff,0xffffffff);
		boardW.show(0);
		boardH.setMaxLen(2);
		boardH.setNumOnly(true);
		boardH.setCursorWidth(1);
		boardH.setCoord(300,140);
		boardH.setSize(100,30);
		boardH.setTextCoord(2,0);
		boardH.setFont(30,"Consolas");
		boardH.setCharWidth(15);
		boardH.setText("20");
		boardH.setCursorPos(2);
		boardH.setLineWidth(1);
		boardH.setColor(0xff000000,0xffcfcfcf,0xffffffff,0xffffffff);
		boardH.show(0);
		Start.setCoord(90,190);
		Start.setSize(120,60);
		Start.setColor(0xff1e1d2d,0xff2e2d42,0xff242336,0xff2d2b43,0xffb9b7de);
		Start.setFont(40,"Consolas");
		Start.setTitle("Start");
		Start.setTitleCoord(10,10);
		Start.setLineWidth(4);
		Start.show(0);
		for(;is_run();delay_fps(60)){
			while(mousemsg()){
				msgRecord=getmouse();
				redraw=1;
			}
			if(redraw){
				redraw=0;
				boardW.update(msgRecord);
				boardH.update(msgRecord);
				if(Start.update(msgRecord)) break;
			}
			while(kbmsg()){
				key_msg kmsg=getkey();
				if(kmsg.msg==key_msg_down){
					if(kmsg.key>=65&&kmsg.key<=90){
						if(keystate(16)){
							boardW.update(kmsg.key);
							boardH.update(kmsg.key);
						}else{
							boardW.update(kmsg.key+32);
							boardH.update(kmsg.key+32);
						}
					}else{
						boardW.update(kmsg.key);
						boardH.update(kmsg.key);
					}
				}
			}
		}
		P1=Player(boardW.getIntText(),boardH.getIntText(),200,400,16,true);
	}
	void init(){
		if(mode==8) initSettings();
		else P1=Player(10,20,200,400,16,true);
		cleardevice();
		for(int i=0;i<256;++i) kb[i].delay1=135,kb[i].delay2=25;
		kb[32].delay1=kb[32].delay2=10000000;
		kb[38].delay1=kb[38].delay2=10000000;
		kb[16].delay1=kb[16].delay2=10000000;
		speed=startspeed;
		P1.init();
		P1.printinfo(mode,1);
		P1.show(3);
		delay_ms(1000);
		P1.show(2);
		delay_ms(1000);
		P1.show(1);
		delay_ms(1000);
		P1.show(0);
		lastturn=lastblock=lastinc=starttime=GetTickCount();
		P1.drawline();
		P1.newblock();
		for(int i=0;i<256;++i)
			if(keystate(i)) kb[i].update(0);
			else kb[i].update(-1);
		flushkey();
	}
	void play(){
		P1.printall(mode);
		bool ret;
		for(;is_run();delay_fps(60)){
			vector<int> keys;
			keys.clear();
			for(int i=0;i<256;++i)
				if(keystate(i)){
					if(kb[i].update(GetTickCount())) keys.push_back(i);
				}else kb[i].update(-1);
//			while(kbmsg()){
//				key_msg kmsg=getkey();
//				if(kmsg.msg==key_msg_down){
//					if(kb[kmsg.key].update(GetTickCount())) keys.push_back(kmsg.key);
//				}
//				if(kmsg.msg==key_msg_up) kb[kmsg.key].update(-1);
//			}
			ret=0;
			int duration=GetTickCount()-starttime;
			P1.printinfo(mode,duration);
			if(P1.gameover){
				P1.lose(duration);
				flushkey();keys.clear();
				delay_ms(0);
				while(1){
					bool f=0;
					while(kbmsg()){
						key_msg kmsg=getkey();
						if(kmsg.msg==key_msg_down&&kmsg.key==32){f=1;break;}
					}
					if(f) break;
				}
				break;
			}
			if(mode==3){
				if(P1.lines>=40){
					P1.win(mode,duration);
					flushkey();keys.clear();
					delay_ms(0);
					while(1){
						bool f=0;
						while(kbmsg()){
							key_msg kmsg=getkey();
							if(kmsg.msg==key_msg_down&&kmsg.key==32){f=1;break;}
						}
						if(f) break;
					}
					break;
				}
			}
			if(mode==4){
				if(GetTickCount()-starttime>=120000){
					P1.win(mode,duration);
					flushkey();keys.clear();
					delay_ms(0);
					while(1){
						bool f=0;
						while(kbmsg()){
							key_msg kmsg=getkey();
							if(kmsg.msg==key_msg_down&&kmsg.key==32){f=1;break;}
						}
						if(f) break;
					}
					break;
				}
			}
			if(mode==7){
				if(P1.lines>=10){
					P1.win(mode,duration);
					flushkey();keys.clear();
					delay_ms(0);
					while(1){
						bool f=0;
						while(kbmsg()){
							key_msg kmsg=getkey();
							if(kmsg.msg==key_msg_down&&kmsg.key==32){f=1;break;}
						}
						if(f) break;
					}
					break;
				}
			}
			bool upd=0;
			for(auto k:keys){
				int f=1;
				switch(k){
					case 38:P1.spin();break;
					case 40:
						if(P1._down) while(!P1.isbottom()) P1.down();
						else P1.down();
						break;
					case 37:P1.left();break;
					case 39:P1.right();break;
					case 90:P1._spin();break;
					case 32:P1.drop(mode);break;
					case 16:P1.gethold();break;
					case 82:ret=1;init();break;
					case 27:ret=2;break;
					case 70:exit(0);
					case 88:
						ShowWindow(GetConsoleWindow(),SW_SHOWMINIMIZED);
						break;
					default:f=0;
				}
				upd|=f;
			}
			if(upd) P1.printall(mode);
			if(ret==1) continue;
			if(ret==2) break;
			if(GetTickCount()-lastinc>inc_turn){lastinc=GetTickCount();speed+=speed_inc;}
			if(GetTickCount()-lastturn<=1.0/speed) continue;
			lastturn=GetTickCount();
			if(P1.isbottom()){
				if(P1.done==0) P1.done=GetTickCount();
				else if(GetTickCount()-P1.done>1000) P1.finish(mode);
			}else P1.down();
			P1.printall(mode);
		}
	}
};
#endif
