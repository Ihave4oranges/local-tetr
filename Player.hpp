#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__
#include "Const.hpp"
struct Player{
	int W,H,BX,BY,size;
	bool _down;
	Player(){}
	Player(int _W,int _H,int _BX,int _BY,int _size,bool d){W=_W;H=_H;BX=_BX;BY=_BY;size=_size;_down=d;}
	int mp[22][44],lstmp[22][44],done,lines,pieces,combo,B2B,totattk,isTspin;
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
	bool down(){--nowy;bool f=check();if(f) ++nowy;else isTspin=0;return !f;}
	void left(){--nowx;if(check()) ++nowx;else isTspin=0;}
	void right(){++nowx;if(check()) --nowx;else isTspin=0;}
	void checkTspin(){
		isTspin=0;
		if(nowb!=2) return;
		int cornercnt=0,cnt2=0;
		for(int i=0;i<4;++i){
			int nx=nowx+dxT[i],ny=nowy+dyT[i];
			if(nx<1||nx>W||ny<1||mp[nx][ny]!=-1){
				++cornercnt;
				if(i==nowr||i==(nowr+1)%4) ++cnt2;
			}
		}
		if(cornercnt>=3&&cnt2==2) isTspin=2;
		else if(cornercnt>=3) isTspin=1;
	}
	void spin(){
		for(int i=0;i<5;++i){
			nowx+=spind[nowb][nowr][i][0];
			nowy+=spind[nowb][nowr][i][1];
			nowr=(nowr+1)&3;
			if(!check()){checkTspin();return;}
			nowr=(nowr+3)&3;
			nowx-=spind[nowb][nowr][i][0];
			nowy-=spind[nowb][nowr][i][1];
		}
	}
	void _spin(){
		for(int i=0;i<5;++i){
			nowx+=_spind[nowb][nowr][i][0];
			nowy+=_spind[nowb][nowr][i][1];
			nowr=(nowr+3)&3;
			if(!check()){checkTspin();return;}
			nowr=(nowr+1)&3;
			nowx-=_spind[nowb][nowr][i][0];
			nowy-=_spind[nowb][nowr][i][1];
		}
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
		if(nowb>=0&&nowb<7&&nowr>=0&&nowr<4)for(int i=0;i<4;++i) px[i]=getx(i),py[i]=gety(i);
		nowb=nxtb[0];for(int i=0;i<4;++i) nxtb[i]=nxtb[i+1];
		++packpos;if(packpos==7) newpack();
		nxtb[4]=pack[packpos];
		nowr=0;nowx=(W+1)/2;nowy=H+3;isTspin=0;
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
	void drawline(){
		setlinewidth(1);
		setcolor(WHITE);
		ege_line(BX+size-1,BY,BX+size-1,BY-size*H);
		ege_line(BX+size*(W+1)+1,BY,BX+size*(W+1)+1,BY-size*H);
		ege_line(BX-3,BY,BX-3,BY-size*H);
		ege_line(BX-1,BY+1,BX+size*(W+1),BY+1);
	}
	void finish(int mode){
		setfillcolor(BLACK);
		ege_fillrect(BX-size*10,BY-size*15,size*8,size*9.5);
		if(allclear) ege_fillrect(BX+size*(W*2-7)/4,BY-size*(H*2+5)/4,size*5,size*3),drawline(),allclear=0;
		done=0;
		int b=isTspin;
		for(int i=0;i<4;++i){
			int x=getx(i),y=gety(i);
			mp[x][y]=nowb;
		}
		newblock();
		int l=remove();nowy+=l;
		if(l>0) ++combo;else combo=-1;
		if(mode==7){if(b&&l>0) ++lines;}
		else lines+=l;
		for(int i=0;i<4;++i)
			if(py[i]>H+1){gameover=1;return;}
		++pieces;
		if(!l) return;
		if(l==4||b) ++B2B;
		else B2B=-1;
		if(l!=4) --l;
		if(b==2){
			l=(l+1)*2;
			setcolor(EGERGB(0xc3,0x52,0xce));setfont(size*2,0,"Consolas");
			xyprintf(BX-size*10,BY-size*10,"T-SPIN");
		}else if(b==1){
			setcolor(EGERGB(0xc3,0x52,0xce));setfont(size,0,"Consolas");
			xyprintf(BX-size*10,BY-size*10,"MINI T-SPIN");
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
		ege_fillrect(BX-size*10,BY-size*5.5,size*8.5,size*5.5);
		setfont(size,0,"Consolas");
		xyprintf(BX-size*10,BY-size*5.5,"PPS:%.2lf PIECES/s",(double)pieces*1000.0/(double)duration);
		if(mode==4) xyprintf(BX-size*10,BY-size*4,getlevel(lines).c_str());
		else if(mode==7) xyprintf(BX-size*10,BY-size*4,"T-SPINS:%d/10",lines);
		else if(mode==3) xyprintf(BX-size*10,BY-size*4,"LINES:%d/40",lines);
		else xyprintf(BX-size*10,BY-size*4,"LINES:%d",lines);
		xyprintf(BX-size*10,BY-size*2.5,"PIECES:%d",pieces);
		if(mode==4) xyprintf(BX-size*10,BY-size,"TIME:%s",gettime(120000-duration).c_str());
		else xyprintf(BX-size*10,BY-size,"TIME:%s",gettime(duration).c_str());
	}
	void printall(int mode){
		for(int i=1;i<=W;++i)
			for(int j=1;j<=H+7&&j<=BY;++j)
				if(mp[i][j]!=lstmp[i][j]){
					int now=lstmp[i][j]=mp[i][j];
					setfillcolor(now>=0?col[now]:BLACK);
					ege_fillrect(BX+size*i,BY-size*j,size,size);
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
		if(x==0){delay_ms(0);return;}
		setcolor(YELLOW);setfont(size*8,0,"Consolas");
		xyprintf(BX+size*(W+2)/2.0-size*2,BY-size*H/2.0-size*4,"%d",x);
		delay_ms(0);
	}
	void init(){
		memset(mp,-1,sizeof(mp));
		memset(lstmp,-1,sizeof(lstmp));
		attacks.clear();
		done=isTspin=gameover=lines=pieces=totattk=allclear=nowb=0;
		combo=B2B=hold=lstb=-1;
		setfillcolor(BLACK);
		ege_fillrect(BX-size*7,BY-size*(H-2),size*4,size*3);
		ege_fillrect(BX+size*(W+3),BY-size*(H-2),size*4,size*14);
		ege_fillrect(BX-size*10,BY-size*15,size*8,size*9.5);
		ege_fillrect(BX+size,BY-size*(H+4),size*W,size*(H+4));
		setcolor(WHITE);setfillcolor(WHITE);
		setfont(size,0,"Consolas");
		xyprintf(BX-size*6,BY-size*H,"HOLD");
		setfont(size,0,"Consolas");
		xyprintf(BX+size*(W+2),BY-size*H,"NEXT");
		drawline();
		newpack();
		for(int i=0;i<5;++i) nxtb[i]=pack[i];
		updatenext();
		packpos=4;
		memset(px,0,sizeof(px));
		memset(py,0,sizeof(py));
	}
};
#endif
