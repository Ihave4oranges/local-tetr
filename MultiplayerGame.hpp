#ifndef __MULTIPLAYERGAME_HPP__
#define __MULTIPLAYERGAME_HPP__
#include "Const.hpp"
#include "Player.hpp"
struct MultiGame{
	int mode;double speed;
	int starttime,lastturn,lastblock,lastinc;
	Player P1,P2;
	Game(){}
	void init(){
		P1=Player(10,20,200,400,16,true);
		P2=Player(10,20,680,400,16,true);
		for(int i=0;i<256;++i) kb[i].delay1=135,kb[i].delay2=25;
		kb[65].delay1=kb[65].delay2=10000000;
		kb[82].delay1=kb[82].delay2=10000000;
		kb[49].delay1=kb[49].delay2=10000000;
		kb[191].delay1=kb[191].delay2=10000000;
		kb[38].delay1=kb[38].delay2=10000000;
		kb[80].delay1=kb[80].delay2=10000000;
		speed=startspeed;
		P1.init();P2.init();
		P1.printinfo(mode,1);P2.printinfo(mode,1);
		P1.show(3);P2.show(3);
		delay_ms(1000);
		P1.show(2);P2.show(2);
		delay_ms(1000);
		P1.show(1);P2.show(1);
		delay_ms(1000);
		P1.show(0);P2.show(0);
		lastturn=lastblock=lastinc=starttime=GetTickCount();
		P1.drawline();P2.drawline();
		P1.newblock();P2.newblock();
		for(int i=0;i<256;++i)
			if(keystate(i)) kb[i].update(0);
			else kb[i].update(-1);
		flushkey();
	}
	void play(){
		P1.printall(mode);P2.printall(mode);
		while(1){
			delay_ms(0);
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
			int duration=GetTickCount()-starttime;
			P1.printinfo(mode,duration);P2.printinfo(mode,duration);
			if(P1.gameover){
				P1.lose(duration);P2.win(mode,duration);
				int f=0;
				flushkey();
				keys.clear();
				delay_ms(0);
				while(f<3){
					while(kbmsg()){
						key_msg kmsg=getkey();
						if(kmsg.msg==key_msg_down){
							if(kb[kmsg.key].update(GetTickCount())) keys.push_back(kmsg.key);
						}
						if(kmsg.msg==key_msg_up) kb[kmsg.key].update(-1);
					}
					for(auto k:keys){
						if(k==65) P1.getok(),f|=1;
						if(k==191) P2.getok(),f|=2;
					}
				}
				break;
			}
			if(P2.gameover){
				P1.win(mode,duration);P2.lose(duration);
				int f=0;
				flushkey();
				keys.clear();
				delay_ms(0);
				while(f<3){
					while(kbmsg()){
						key_msg kmsg=getkey();
						if(kmsg.msg==key_msg_down){
							if(kb[kmsg.key].update(GetTickCount())) keys.push_back(kmsg.key);
						}
						if(kmsg.msg==key_msg_up) kb[kmsg.key].update(-1);
					}
					for(auto k:keys){
						if(k==65) P1.getok(),f|=1;
						if(k==191) P2.getok(),f|=2;
					}
				}
				break;
			}
			while(!P1.attacks.empty()&&!P2.attacks.empty()){
				if(P1.attacks.back().second==P2.attacks.back().second){
					P1.attacks.pop_back();
					P2.attacks.pop_back();
				}else if(P1.attacks.back().second>P2.attacks.back().second){
					P1.attacks.back().second-=P2.attacks.back().second;
					P2.attacks.pop_back();
				}else{
					P2.attacks.back().second-=P1.attacks.back().second;
					P1.attacks.pop_back();
				}
			}
			P1.updateattack(P2.attacks);
			P2.updateattack(P1.attacks);
			bool f=0;
			while(!P1.attacks.empty()&&GetTickCount()-P1.attacks.front().first>attackdelay){
				P2.addblocks(P1.attacks.front().second);
				P1.attacks.pop_front();f=1;
			}
			if(f) P2.printall(mode);
			f=0;
			while(!P2.attacks.empty()&&GetTickCount()-P2.attacks.front().first>attackdelay){
				P1.addblocks(P2.attacks.front().second);
				P2.attacks.pop_front();f=1;
			}
			if(f) P1.printall(mode);
			bool upd1=0,upd2=0;
			for(auto k:keys){
				bool f1=1,f2=1;
				switch(k){
					case 82:P1.spin();break;
					case 70:
						if(P1._down) while(!P1.isbottom()) P1.down();
						else P1.down();
						break;
					case 68:P1.left();break;
					case 71:P1.right();break;
					case 65:P1.drop(mode);break;
					case 49:P1.gethold();break;
					default:f1=0;
				}
				switch(k){
					case 38:P2.spin();break;
					case 40:
						if(P2._down) while(!P2.isbottom()) P2.down();
						else P2.down();
						break;
					case 37:P2.left();break;
					case 39:P2.right();break;
					case 191:P2.drop(mode);break;
					case 80:P2.gethold();break;
					default:f2=0;
				}
				upd1|=f1;upd2|=f2;
			}
			if(upd1) P1.printall(mode);
			if(upd2) P2.printall(mode);
			if(GetTickCount()-lastinc>inc_turn){lastinc=GetTickCount();speed+=speed_inc;}
			if(GetTickCount()-lastturn<=1.0/speed) continue;
			lastturn=GetTickCount();
			if(P1.isbottom()){if(P1.done) P1.finish(mode);else P1.done=1;}
			else P1.down(),P1.done=0;
			if(P2.isbottom()){if(P2.done) P2.finish(mode);else P2.done=1;}
			else P2.down(),P2.done=0;
			P1.printall(mode);P2.printall(mode);
		}
	}
};
#endif
