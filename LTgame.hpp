#include "LTdef.hpp"
mt19937 mt_rand(time(NULL));
struct Player{
	int W,H,BX,BY,size;
	bool _down,isTspin;
	Player(){}
	Player(int _W,int _H,int _BX,int _BY,int _size,bool d){W=_W;H=_H;BX=_BX;BY=_BY;size=_size;_down=d;}
	int lastktime,lastkey,isfirst;
	int mp[22][66],lstmp[22][66],done,lines,pieces,combo,B2B,totattk;
	int nowb,nowr,nowx,nowy,hold,nxtb[5],pack[7],packpos,lstb,px[4],py[4];
	bool gameover,allclear;
	deque<pair<int,int> > attacks;
	int getx(int i){return nowx+d[nowb][nowr][i][0];}
	int gety(int i){return nowy+d[nowb][nowr][i][1];}
	void clear(){
		setfillcolor(BLACK);
		ege_fillrect(BX-size*10,BY-size*(H+4),size*(W+19),size*(H+5)+1);
		for(int i=1;i<=W;++i)
			for(int j=1;j<=H+4;++j) lstmp[i][j]=114514;
	}
	void win(int mode,int duration){
		clear();
		if(mode==2){
			setcolor(YELLOW);setfont(size*H*2/7,0,"Consolas");
			xyprintf(BX+size*(W+2-H)/2,BY-size*H*9/14,"VICTORY");
			setcolor(WHITE);setfont(size,0,"Consolas");
			xyprintf(BX+size*(W-9)/2,BY-size*H*5/14,"SURVIVED FOR %s",gettime(duration).c_str());
		}else if(mode==3){
			setcolor(WHITE);setfont(size*3,0,"Consolas");
			xyprintf(BX+size*(W*2-25)/4,BY-size*(H+3)/2,gettime(duration).c_str());
			if(duration<best40lines){
				best40lines=duration;
				xyprintf(BX+size*(W*2-37)/4,BY-size*(H-3)/2,"PERSONAL BEST");
			}
		}else if(mode==7){
			setcolor(WHITE);setfont(size*3,0,"Consolas");
			xyprintf(BX+size*(W*2-25)/4,BY-size*(H+3)/2,gettime(duration).c_str());
			if(duration<bestTspin){
				bestTspin=duration;
				xyprintf(BX+size*(W*2-37)/4,BY-size*(H-3)/2,"PERSONAL BEST");
			}
		}else{
			setcolor(WHITE);setfont(size*2,0,"Consolas");
			xyprintf(BX+size*(W-18)/2,BY-size*(H+2)/2,getlevel(lines).c_str());
			if(lines>bestblitz){
				bestblitz=lines;
				xyprintf(BX+size*(W-12)/2,BY-size*(H-2)/2,"PERSONAL BEST");
			}
		}
		setcolor(WHITE);
		setfont(size,0,"Consolas");
		xyprintf(BX+size*W/2,BY-size,"NEXT");
	}
	void lose(int duration){
		clear();
		setcolor(EGERGB(0x7f,0x7f,0x7f));
		setfont(size*H*2/7,0,"Consolas");
		xyprintf(BX+size*(7*W+7-H*6)/14,BY-size*H*9/14,"DEFEAT");
		setcolor(WHITE);setfont(size,0,"Consolas");
		xyprintf(BX+size*(W-9)/2,BY-size*H*5/14,"SURVIVED FOR %s",gettime(duration).c_str());
		setfont(size,0,"Consolas");
		xyprintf(BX+size*(W-1)/2,BY-size,"NEXT");
	}
	void getok(){
		setfont(size,0,"Consolas");
		xyprintf(BX+size*(W*2-5)/4,BY-size,"WAITING");
	}
	bool checkl(){for(int i=0;i<4;++i)if(getx(i)<1)return 1;return 0;}
	bool checkr(){for(int i=0;i<4;++i)if(getx(i)>W)return 1;return 0;}
	bool checkb(){for(int i=0;i<4;++i)if(gety(i)<1)return 1;return 0;}
	bool checkd(){for(int i=0;i<4;++i)if(getx(i)>0&&getx(i)<=W&&mp[getx(i)][gety(i)]>=0)return 1;return 0;}
	bool check(){return checkl()||checkr()||checkb()||checkd();}
	bool down(){--nowy;bool f=check();if(f) ++nowy;return !f;}
	void left(){--nowx;if(check()) ++nowx;}
	void right(){++nowx;if(check()) --nowx;}
	void checkTspin(){
		if(nowb!=2) return;
		isTspin=1;
		++nowx;if(!check()) isTspin=0;
		nowx-=2;if(!check()) isTspin=0;
		++nowx;++nowy;if(!check()) isTspin=0;
		--nowy;
	}
	void spin(){
		nowr=(nowr+1)&3;
		int tx=nowx,ty=nowy;
		while(checkl()) ++nowx;
		while(checkr()) --nowx;
		while(checkb()) ++nowy;
		if(!check()){checkTspin();return;}
		nowx=tx;nowy=ty;
		for(int i=0;i<=2;++i){
			nowx=tx;if(!check()){checkTspin();return;}
			--nowx;if(!check()){checkTspin();return;}
			nowx+=2;if(!check()){checkTspin();return;}
			--nowy;
		}
		nowy+=4;
		nowx=tx;if(!check()) return;
		--nowx;if(!check()) return;
		nowx+=2;if(!check()) return;
		--nowy;nowx=tx;nowr=(nowr+3)&3;
	}
	void _spin(){
		nowr=(nowr+3)&3;
		int tx=nowx,ty=nowy;
		while(checkl()) ++nowx;
		while(checkr()) --nowx;
		while(checkb()) ++nowy;
		if(!check()){checkTspin();return;}
		nowx=tx;nowy=ty;
		for(int i=0;i<=2;++i){
			nowx=tx;if(!check()){checkTspin();return;}
			--nowx;if(!check()){checkTspin();return;}
			nowx+=2;if(!check()){checkTspin();return;}
			--nowy;
		}
		nowy+=4;
		nowx=tx;if(!check()) return;
		--nowx;if(!check()) return;
		nowx+=2;if(!check()) return;
		nowy=ty;nowx=tx;nowr=(nowr+1)&3;
	}
	int remove(){
		int cnt=0;
		for(int j=1;j<=H+2;++j){
			bool f=1;
			for(int i=1;i<=W;++i)
				if(mp[i][j]==-1){f=0;break;}
			if(f){
				++cnt;
				for(int i=1;i<=W;++i) mp[i][j]=-1;
				for(int i=0;i<4;++i)
					if(py[i]==j) px[i]=py[i]=0;
			}else if(cnt){
				for(int i=1;i<=W;++i) mp[i][j-cnt]=mp[i][j],mp[i][j]=-1;
				for(int i=0;i<4;++i)
					if(px[i]&&py[i]==j) py[i]-=cnt;
			}
		}
		if(cnt){
			setfont(size*2,0,"Consolas");
			if(cnt==1) xyprintf(BX-size*10,BY-size*7.5,"SINGLE");
			if(cnt==2) xyprintf(BX-size*10,BY-size*7.5,"DOUBLE");
			if(cnt==3) xyprintf(BX-size*10,BY-size*7.5,"TRIPLE");
			if(cnt==4) xyprintf(BX-size*10,BY-size*7.5,"QUAD  ");
		}
		return cnt;
	}
	bool checkallclear(){
		for(int i=1;i<=W;++i)
			if(mp[i][1]!=-1) return 0;
		return 1;
	}
	void updatenext(){
		setfillcolor(BLACK);
		ege_fillrect(BX+size*(W+3),BY-size*(H-2),size*4,size*14);
		for(int i=0;i<5;++i)
			for(int j=0;j<4;++j){
				int x=d[nxtb[i]][0][j][0],y=d[nxtb[i]][0][j][1];
				setfillcolor(col[nxtb[i]]);
				ege_fillrect(BX+size*(W+4+x),BY-size*(H-i*3-3+y),size,size);
			}
	}
	void updatehold(){
		if(hold==-1) return;
		setfillcolor(BLACK);
		ege_fillrect(BX-size*7,BY-size*(H-2),size*4,size*3);
		for(int i=0;i<4;++i){
			int x=d[hold][0][i][0],y=1-d[hold][0][i][1];
			setfillcolor(col[hold]);
			ege_fillrect(BX-size*(6-x),BY-size*(H-2-y),size,size);
		}
	}
	void updateattack(deque<pair<int,int> > q){
		int cnt=0;
		while(!q.empty()) cnt+=q.front().second,q.pop_front();
		setfillcolor(RED);
		ege_fillrect(BX-2,BY-size*cnt,size,size*cnt);
		setfillcolor(BLACK);
		ege_fillrect(BX-2,BY-size*H,size,size*(H-cnt));
	}
	void newpack(){
		for(int i=0;i<7;++i) pack[i]=i;
		for(int i=0;i<7;++i) swap(pack[i],pack[mt_rand()%(i+1)]);
		packpos=0;
	}
	void newblock(){
		lstb=nowb;
		for(int i=0;i<4;++i) px[i]=getx(i),py[i]=gety(i);
		nowb=nxtb[0];for(int i=0;i<4;++i) nxtb[i]=nxtb[i+1];
		++packpos;if(packpos==7) newpack();
		nxtb[4]=pack[packpos];
		nowr=0;nowx=W/2;nowy=H+3;isTspin=0;
		updatenext();
	}
	void gethold(){
		isTspin=0;
		for(int i=0;i<4;++i){
			int x=getx(i),y=gety(i);
			setfillcolor(BLACK);
			ege_fillrect(BX+size*x,BY-size*y,size,size);
			lstmp[x][y]=114514;
		}
		if(hold==-1){hold=nowb;newblock();}
		else{swap(hold,nowb);nowr=0;nowx=W/2;nowy=H+3;}
		for(int i=0;i<4;++i){
			int x=getx(i),y=gety(i);
			setfillcolor(col[nowb]);
			ege_fillrect(BX+size*x,BY-size*y,size,size);
			lstmp[x][y]=114514;
		}
		updatehold();
	}
	void addblocks(int bc){
		if(bc<0) return;
		int p=mt_rand()%W+1;
		while(bc--){
			for(int i=1;i<=W;++i)
				for(int j=H+2;j;--j) mp[i][j+1]=mp[i][j],mp[i][j]=-1;
			for(int i=1;i<=W;++i)
				if(i!=p) mp[i][1]=7;
			if(check()) ++nowy;
			for(int i=1;i<=W;++i)
				if(mp[i][H+3]!=-1){gameover=1;return;}
		}
		return;
	}
	void finish(int mode){
		setfillcolor(BLACK);
		ege_fillrect(BX-size*10,BY-size*15,size*8,size*9.5);
		if(allclear) ege_fillrect(BX+size*(W*2-7)/4,BY-size*(H*2+5)/4,size*5,size*3),allclear=0;
		done=0;
		bool b=isTspin;
		for(int i=0;i<4;++i){
			int x=getx(i),y=gety(i);
			mp[x][y]=nowb;
		}
		newblock();
		int l=remove();nowy+=l;
		if(mode==7){if(b&&l>0) ++lines;}
		else lines+=l;
		if(l>0) ++combo;else combo=-1;
		for(int i=0;i<4;++i)
			if(py[i]>H+1){gameover=1;return;}
		++pieces;
		if(!l) return;
		if(l==4||b) ++B2B;
		else B2B=-1;
		if(l!=4) --l;
		if(b){
			l*=2;
			setcolor(EGERGB(0xc3,0x52,0xce));setfont(size*2,0,"Consolas");
			xyprintf(BX-size*10,BY-size*10,"T-SPIN");
		}
		if(combo>=1){
			setcolor(WHITE);setfont(size*2,0,"Consolas");
			xyprintf(BX-size*10,BY-size*12.5,"COMBOx%d",combo);
		}
		if(B2B>=1){
			setcolor(EGERGB(0xff,0xff,0x0));setfont(size*2,0,"Consolas");
			xyprintf(BX-size*10,BY-size*15,"B2Bx%d",B2B);
		}
		l=calcgarbage(B2B,l,combo);
		if(checkallclear()){
			l+=10;
			setcolor(GREEN);setfont(size*2,0,"Consolas");
			xyprintf(BX+size*(W*2-3)/4,BY-size*(H*2+5)/4,"ALL");
			xyprintf(BX+size*(W*2-7)/4,BY-size*H/2,"CLEAR");
			allclear=1;
		}
		totattk+=l;attacks.push_back({GetTickCount(),l});
		return;
	}
	void drop(int mode){while(down());finish(mode);}
	bool isbottom(){--nowy;bool f=check();++nowy;return f;}
	void printinfo(int mode,int duration){
		setcolor(WHITE);setfillcolor(BLACK);
		ege_fillrect(BX-size*10,BY-size*5.5,size*8,size*5.5);
		ege_fillrect(BX+size*(W+2),BY-size*3,size*6.5,size*3);
		setfont(size,0,"Consolas");
		xyprintf(BX-size*10,BY-size*5.5,"PPS:%.2lf PIECES/s",(double)pieces*1000.0/(double)duration);
		if(mode==4) xyprintf(BX-size*10,BY-size*4,getlevel(lines).c_str());
		else if(mode==7) xyprintf(BX-size*10,BY-size*4,"T-SPINS:%d/10",lines);
		else if(mode==3) xyprintf(BX-size*10,BY-size*4,"LINES:%d/40",lines);
		else xyprintf(BX-size*10,BY-size*4,"LINES:%d",lines);
		xyprintf(BX-size*10,BY-size*2.5,"PIECES:%d",pieces);
		if(mode==4) xyprintf(BX-size*10,BY-size,"TIME:%s",gettime(120000-duration).c_str());
		else xyprintf(BX-size*10,BY-size,"TIME:%s",gettime(duration).c_str());
		xyprintf(BX+size*(W+2),BY-size*3,"TOTAL ATKS:");
		xyprintf(BX+size*(W+2),BY-size*1.5,"%d",totattk);
	}
	void printall(int mode){
		if(mode==6){
			for(int i=1;i<=W;++i)
				for(int j=1;j<=H+7&&j<=BY;++j){
					int now=-1;
					if(abs(i-nowx)+abs(j-nowy)<=5) now=(mp[i][j]==-1?7:mp[i][j]);
					if(now!=lstmp[i][j]){
						lstmp[i][j]=now;
						setfillcolor(now>=0?col[now]:BLACK);
						ege_fillrect(BX+size*i,BY-size*j,size,size);
					}
				}
		}else{
			for(int i=1;i<=W;++i)
				for(int j=1;j<=H+7&&j<=BY;++j)
					if(mp[i][j]!=lstmp[i][j]){
						int now=lstmp[i][j]=mp[i][j];
						setfillcolor(now>=0?col[now]:BLACK);
						ege_fillrect(BX+size*i,BY-size*j,size,size);
					}
		}
		int yy=nowy;while(!check()) --nowy;++nowy;
		for(int i=0;i<4;++i){
			int x=getx(i),y=gety(i);
			setfillcolor(EGERGB(0xbf,0xbf,0xbf));
			ege_fillrect(BX+size*x,BY-size*y,size,size);
			setfillcolor(EGERGB(0x3f,0x3f,0x3f));
			ege_fillrect(BX+size*x+size/4.0,BY-size*y+size/4.0,size/2.0,size/2.0);
			lstmp[x][y]=114514;
		}
		nowy=yy;
		for(int i=0;i<4;++i){
			int x=getx(i),y=gety(i);
			setcolor(col[nowb]);setfillcolor(col[nowb]);
			ege_fillrect(BX+size*x,BY-size*y,size,size);
			lstmp[x][y]=114514;
		}
	}
	void show(int x){
		setfillcolor(BLACK);
		ege_fillrect(BX+size*(W+2)/2.0-size*2,BY-size*H/2.0-size*4,size*4,size*8);
		if(x==0) return;
		setcolor(YELLOW);setfont(size*8,0,"Consolas");
		xyprintf(BX+size*(W+2)/2.0-size*2,BY-size*H/2.0-size*4,"%d",x);
	}
	void init(){
		memset(mp,-1,sizeof(mp));
		memset(lstmp,-1,sizeof(lstmp));
		attacks.clear();
		done=isTspin=gameover=lines=pieces=totattk=allclear=0;
		combo=B2B=hold=lstb=-1;
		setcolor(WHITE);setfillcolor(WHITE);
		setfont(size,0,"Consolas");
		xyprintf(BX-size*6,BY-size*H,"HOLD");
		setfont(size,0,"Consolas");
		xyprintf(BX+size*(W+2),BY-size*H,"NEXT");
		ege_line(BX+size-1,BY,BX+size-1,BY-size*H);
		ege_line(BX+size*(W+1)+1,BY,BX+size*(W+1)+1,BY-size*H);
		ege_line(BX-3,BY,BX-3,BY-size*H);
		ege_line(BX-1,BY+1,BX+size*(W+1),BY+1);
		newpack();
		for(int i=0;i<5;++i) nxtb[i]=pack[i];
		updatenext();
		packpos=4;
		memset(px,0,sizeof(px));
		memset(py,0,sizeof(py));
		lastkey=-1;
	}
};
struct Game{
	int mode;double speed;
	int starttime,lastturn,lastblock,lastinc;
	Player P1,P2;
	Game(){}
	void init(){
		speed=startspeed;
		P1.init();if(mode==2) P2.init();
		P1.show(3);if(mode==2) P2.show(3);
		Sleep(1000);
		P1.show(2);if(mode==2) P2.show(2);
		Sleep(1000);
		P1.show(1);if(mode==2) P2.show(1);
		Sleep(1000);
		P1.show(0);if(mode==2) P2.show(0);
		lastturn=lastblock=lastinc=starttime=GetTickCount();
		P1.lastktime=P2.lastktime=GetTickCount()-delayt;
		P1.isfirst=P2.isfirst=0;
		clearkb();
		P1.newblock();if(mode==2) P2.newblock();
	}
	void play(){
		if(mode==2){
			while(1){
				int duration=GetTickCount()-starttime;
				P1.printinfo(mode,duration);P2.printinfo(mode,duration);
				if(P1.gameover){
					P1.lose(duration);P2.win(mode,duration);
					clearkb();
					int f=0;
					while(f<3){
						do updatekb();while(!kb);
						if(kb==65&&P1.lastktime<GetTickCount()-delayt*2) P1.getok(),f|=1;
						if(kb==191&&P2.lastktime<GetTickCount()-delayt*2) P2.getok(),f|=2;
					}
					break;
				}
				if(P2.gameover){
					P1.win(mode,duration);P2.lose(duration);
					clearkb();
					int f=0;
					while(f<3){
						do updatekb();while(!kb);
						if(kb==65&&P1.lastktime<GetTickCount()-delayt*2) P1.getok(),f|=1;
						if(kb==191&&P2.lastktime<GetTickCount()-delayt*2) P2.getok(),f|=2;
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
				updatekb();
				if(kb){
					if(P1.lastktime<GetTickCount()-delayt||kb!=P1.lastkey||(!P1.isfirst&&P1.lastktime<GetTickCount()-delay2)){
						int tmp=P1.lastktime,tmpk=P1.lastkey;bool f=1;
						if(kb!=P1.lastkey) P1.isfirst=1;else P1.isfirst=0;
						P1.lastktime=GetTickCount();P1.lastkey=kb;
						switch(kb){
							case 82:P1.spin();break;
							case 70:
								if(P1._down) while(!P1.isbottom()) P1.down();
								else P1.down();
								break;
							case 68:P1.left();break;
							case 71:P1.right();break;
							case 65:P1.drop(mode);break;
							case 49:P1.gethold();break;
							default:P1.lastktime=tmp;f=0;P1.isfirst=0,P1.lastkey=-1;
						}
						if(f) P1.printall(mode);
					}
					if(P2.lastktime<GetTickCount()-delayt||kb!=P2.lastkey||(!P2.isfirst&&P2.lastktime<GetTickCount()-delay2)){
						int tmp=P2.lastktime,tmpk=P2.lastkey;bool f=1;
						if(kb!=P2.lastkey) P2.isfirst=1;else P2.isfirst=0;
						P2.lastktime=GetTickCount();P2.lastkey=kb;
						switch(kb){
							case 38:P2.spin();break;
							case 40:
								if(P2._down) while(!P2.isbottom()) P2.down();
								else P2.down();
								break;
							case 37:P2.left();break;
							case 39:P2.right();break;
							case 191:P2.drop(mode);break;
							case 80:P2.gethold();break;
							default:P2.lastktime=tmp;f=0;P2.isfirst=0,P2.lastkey=-1;
						}
						if(f) P2.printall(mode);
					}
					P1.printall(mode);P2.printall(mode);
				}else P1.isfirst=0,P1.lastkey=-1,P2.isfirst=0,P2.lastkey=-1;
				if(GetTickCount()-lastturn<=1.0/speed) continue;
				lastturn=GetTickCount();
				if(P1.isbottom()){if(P1.done) P1.finish(mode);else P1.done=1;}
				else P1.down(),P1.done=0;
				if(P2.isbottom()){if(P2.done) P2.finish(mode);else P2.done=1;}
				else P2.down(),P2.done=0;
				if(GetTickCount()-lastinc>inc_turn){lastinc=GetTickCount();speed+=speed_inc;}
				P1.printall(mode);P2.printall(mode);
			}
		}else{
			while(1){
				int duration=GetTickCount()-starttime;
				P1.printinfo(mode,duration);
				if(P1.gameover){
					P1.lose(duration);clearkb();
					while(1){do updatekb();while(!kb);if(kb==32) break;}
					break;
				}
				if(mode==3){
					if(P1.lines>=40){
						P1.win(mode,duration);clearkb();
						while(1){do updatekb();while(!kb);if(kb==32) break;}
						break;
					}
				}
				if(mode==4){
					if(GetTickCount()-starttime>=120000){
						P1.win(mode,duration);
						char ch='0';
						while(1){do updatekb();while(!kb);if(kb==32) break;}
						break;
					}
				}
				if(mode==7){
					if(P1.lines>=10){
						P1.win(mode,duration);clearkb();
						while(1){do updatekb();while(!kb);if(kb==32) break;}
						break;
					}
				}
				updatekb();
				if(kb&&(P1.lastktime<GetTickCount()-delayt||kb!=P1.lastkey||(!P1.isfirst&&P1.lastktime<GetTickCount()-delay2))){
					int tmp=P1.lastktime,tmpk=P1.lastkey,tmpf=P1.isfirst;bool f=1;
					if(kb!=P1.lastkey) P1.isfirst=1;else P1.isfirst=0;
					P1.lastktime=GetTickCount();P1.lastkey=kb;
					switch(kb){
						case 38:P1.spin();break;
						case 40:
							if(P1._down) while(!P1.isbottom()) P1.down();
							else P1.down();
							break;
						case 37:P1.left();break;
						case 39:P1.right();break;
						case 90:P1._spin();break;
						case 32:P1.drop(mode);break;
						case 160:P1.gethold();break;
						case 161:P1.gethold();break;
						case 70:exit(0);
						case 88:
							ShowWindow(GetConsoleWindow(),SW_SHOWMINIMIZED);
							break;
						default:P1.lastktime=tmp;f=0;P1.isfirst=0,P1.lastkey=-1;
					}
					if(f) P1.printall(mode);
				}else if(!kb) P1.isfirst=0,P1.lastkey=-1;
				if(GetTickCount()-lastturn<=1.0/speed) continue;
				lastturn=GetTickCount();
				if(P1.isbottom()){
					if(P1.done==0) P1.done=GetTickCount();
					else if(GetTickCount()-P1.done>500) P1.finish(mode);
				}else P1.down();
				if(GetTickCount()-lastinc>inc_turn){
					lastinc=GetTickCount();
					speed+=speed_inc;
				}
				P1.printall(mode);
			}
		}
	}
};
int mainpage(){
	int mode;
	cleardevice();
	setbkcolor(BLACK);
	setfont(36,0,"Consolas");
	setcolor(WHITE);
	setfillcolor(BLACK);
	xyprintf(80,50,"TETR.IO");
	int now=1;
	setlinewidth(4);
	Button Mult(80,100,240,60,EGERGB(0x34,0x22,0x2d),EGERGB(0x47,0x2d,0x3d),EGERGB(0x3e,0x29,0x36),EGERGB(0x4e,0x33,0x43),
	"MULTIPLAYER","Consolas",40,10,10,WHITE);
	Button Solo(80,180,240,60,EGERGB(0x1e,0x1d,0x2d),EGERGB(0x2e,0x2d,0x42),EGERGB(0x24,0x23,0x36),EGERGB(0x2d,0x2b,0x43),
	"SOLO","Consolas",40,10,10,WHITE);
	Mult.show(0);
	Solo.show(0);
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
	if(now==0){
		Button Fort(80,100,240,60,EGERGB(0x2a,0x21,0x1b),EGERGB(0x64,0x3b,0x1d),EGERGB(0x32,0x28,0x20),EGERGB(0x3f,0x31,0x28),
		"40 LINES","Consolas",40,10,10,WHITE);
		Button Blit(80,180,240,60,EGERGB(0x3e,0x1c,0x1c),EGERGB(0x6b,0x2b,0x2b),EGERGB(0x4a,0x22,0x22),EGERGB(0x5d,0x2a,0x2a),
		"BLITZ","Consolas",40,10,10,WHITE);
		Button Zen(80,260,240,60,EGERGB(0x33,0x19,0x33),EGERGB(0x6b,0x2b,0x69),EGERGB(0x3d,0x1e,0x3d),EGERGB(0x4c,0x25,0x4c),
		"ZEN","Consolas",40,10,10,WHITE);
		Button Tspin(340,100,240,60,EGERGB(0xd5,0xd5,0xd5),EGERGB(0x8f,0x8f,0x8f),EGERGB(0xbf,0xbf,0xbf),EGERGB(0xb7,0xb7,0xb7),
		"T-SPIN","Consolas",40,10,10,BLACK);
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
				if(Tspin.update(msgRecord)){now=3;break;}
			}
		}
		if(now==2) mode=1;
		else if(now==0) mode=3;
		else if(now==1) mode=4;
		else if(now==3) mode=7;
		Fort.hide();
		Blit.hide();
		Zen.hide();
		Tspin.hide();
	}else mode=2;
	return mode;
}
void playgame(){
	hello();
	while(1){
		Game game;
		game.mode=mainpage();
		cleardevice();
		setlinewidth(1);
		game.P1=Player(4,20,200,400,16,true);
		game.P2=Player(10,20,680,400,16,true);
		game.init();
		game.play();
	}
}
