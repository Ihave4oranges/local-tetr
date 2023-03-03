#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <bits/stdc++.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <conio.h>

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "114514"

using namespace std;

HWND hwnd;

WSADATA wsaData;
int iResult;

string PORT;
int SocketCnt=0;
SOCKET ListenSocket = INVALID_SOCKET;
SOCKET ClientSocket[10] = {INVALID_SOCKET};
set<string> SOCKETS;

struct addrinfo *result = NULL;
struct addrinfo hints;

int iSendResult;
char recvbuf[DEFAULT_BUFLEN],Empty[DEFAULT_BUFLEN];
int recvbuflen = DEFAULT_BUFLEN;
unsigned int seed=time(NULL),seed2;
unsigned int mt_rand(){return seed=seed*998244353ll+19260817ll;}
unsigned int mt_rand2(){return seed2=seed2*998244353ll+19260817ll;}

string RandomPort(){
	string s="";
	for(int i=0;i<4;++i) s+=(char)(48+mt_rand()%10);
	return s;
}
int connect(string PORT, SOCKET &ClientSocket){
	if(SOCKETS.find(PORT)!=SOCKETS.end()) return 1;
	SOCKETS.insert(PORT);
	cout<<"connect("<<PORT<<")\n";

	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(NULL, PORT.c_str(), &hints, &result);
	if ( iResult != 0 ) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	closesocket(ListenSocket);

	cout<<"connected\n";
	++SocketCnt;
	return 0;
}
void gotoxy(float x,float y){SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),{x*2,y});}
void hidecursor(){
	CONSOLE_CURSOR_INFO cursor_info={1,0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}
#define SetColor(x) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),x)
void clear_color(){SetColor(7);}
template<typename T>void write(string colf,string colb,T s){
	int x=0;
	if(colb=="red") x=12;
	else if(colb=="green") x=10;
	else if(colb=="blue") x=9;
	else if(colb=="yellow") x=14;
	else if(colb=="purple") x=13;
	else if(colb=="cyan") x=11;
	else if(colb=="white") x=15;
	else if(colb=="grey") x=8;
	else x=0;
	x<<=4;
	if(colf=="red") x|=12;
	else if(colf=="green") x|=10;
	else if(colf=="blue") x|=9;
	else if(colf=="yellow") x|=14;
	else if(colf=="purple") x|=13;
	else if(colf=="cyan") x|=11;
	else if(colf=="white") x|=15;
	else if(colf=="grey") x|=8;
	else x|=0;
	SetColor(x);
	cout<<s;
	clear_color();
}
#define white "black","white"
#define black "white","black"
#define grey "white","grey"
const string col[8]={"cyan","yellow","purple","blue","red","red","green","grey"};
const int d[7][4][4][2]=
{
	{//I
		{{-1,0},{0,0},{1,0},{2,0}},
		{{0,1},{0,0},{0,-1},{0,-2}},
		{{-1,0},{0,0},{1,0},{2,0}},
		{{0,1},{0,0},{0,-1},{0,-2}}
	},
	{//o
		{{0,0},{1,0},{0,-1},{1,-1}},
		{{0,0},{1,0},{0,-1},{1,-1}},
		{{0,0},{1,0},{0,-1},{1,-1}},
		{{0,0},{1,0},{0,-1},{1,-1}}
	},
	{//t
		{{0,1},{-1,0},{0,0},{1,0}},
		{{0,1},{0,0},{1,0},{0,-1}},
		{{-1,0},{0,0},{1,0},{0,-1}},
		{{0,1},{-1,0},{0,0},{0,-1}}
	},
	{//l1
		{{0,0},{0,-1},{1,-1},{2,-1}},
		{{0,0},{1,0},{0,-1},{0,-2}},
		{{-1,0},{0,0},{1,0},{1,-1}},
		{{1,1},{1,0},{0,-1},{1,-1}}
	},
	{//l2
		{{1,0},{-1,-1},{0,-1},{1,-1}},
		{{0,1},{0,0},{0,-1},{1,-1}},
		{{0,0},{1,0},{2,0},{0,-1}},
		{{0,0},{1,0},{1,-1},{1,-2}}
	},
	{//z1
		{{-1,0},{0,0},{0,-1},{1,-1}},
		{{1,1},{0,0},{1,0},{0,-1}},
		{{-1,0},{0,0},{0,-1},{1,-1}},
		{{1,1},{0,0},{1,0},{0,-1}}
	},
	{//z2
		{{0,0},{1,0},{-1,-1},{0,-1}},
		{{0,1},{0,0},{1,0},{1,-1}},
		{{0,0},{1,0},{-1,-1},{0,-1}},
		{{0,1},{0,0},{1,0},{1,-1}}
	}
};
const int image[4][5][7]=
{
	{
		{0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0}
	},
	{
		{0,0,0,0,0,0,0},
		{1,0,0,0,0,1,0},
		{1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0},
		{0,0,0,0,0,0,0}
	},
	{
		{1,0,0,0,0,1,0},
		{1,1,0,0,0,0,1},
		{1,0,1,0,0,0,1},
		{1,0,0,1,0,0,1},
		{1,0,0,0,1,1,0}
	},
	{
		{0,1,0,0,0,1,0},
		{1,0,0,0,0,0,1},
		{1,0,0,1,0,0,1},
		{1,0,0,1,0,0,1},
		{0,1,1,0,1,1,0}
	}
};
string tos(int x){
	if(x==0) return "0";
	string res="";
	while(x){
		res=(char)((x%10)^48)+res;
		x/=10;
	}
	return res;
}
string toos(int x){
	string res="";
	for(int i=0;i<3;++i){
		res=(char)((x%10)^48)+res;
		x/=10;
	}
	return res;
}
string tooos(int x){
	string res="";
	for(int i=0;i<12;++i){
		res=(char)((x%10)^48)+res;
		x/=10;
	}
	return res;
}
int kb;
int Getkey(){
	int res=0;
	for(int i=0;i<strlen(recvbuf);++i) res=res*10+recvbuf[i]-48;
	return res;
}
int last1,last2;
string P1data,P2data;
const double start_speed=0.001;
const double speed_inc=0.0003;
const int inc_turn=15000;
const int blockturn=10000;
const int attackdelay=3000;
const int delay=150;
const int delay2=40;
int best40lines=600000;
int bestblitz=-1;
double speed;
int starttime,lastturn,lastblock,lastinc;
int mode;
string gettime(int t){
	if(mode==4) t=120000-t;
	string res="";
	int x=t/60000;res+=(char)((x/10)^48);res+=(char)((x%10)^48);res+=':';
	x=t/1000%60;res+=(char)((x/10)^48);res+=(char)((x%10)^48);res+=':';
	x=t%1000;res+=(char)((x/100)^48);res+=(char)((x/10%10)^48);res+=(char)((x%10)^48);
	return res;
}
string getlevel(int x){
	string res="LEVEL ";
	for(int i=1;;++i){
		x-=i*2+1;
		if(x<0){
			x+=i*2+1;
			res+=tos(i);
			res+="  ";
			res+=tos(x);
			res+="/";
			res+=tos(i*2+1);
			res+="LINES  ";
			return res;
		}
	}
}
struct Player{
	int SocketID;
	int W,H,BX,BY;
	bool _down;
	Player(int _W,int _H,int _BX,int _BY,bool d){W=_W;H=_H;BX=_BX;BY=_BY;_down=d;}
	int lastktime,lastkey,isfirst;
	int mp[22][66],lstmp[22][66],done,lines,pieces,combo;
	int nowb,nowr,nowx,nowy,hold,nxtb[5];
	int lstb,px[4],py[4];
	bool gameover;
	deque<pair<int,int> > attacks;
	int getx(int i){return nowx+d[nowb][nowr][i][0];}
	int gety(int i){return nowy+d[nowb][nowr][i][1];}
	void clear(){
		for(int i=BX-10;i<=BX+W+6;++i)
			for(int j=BY+2;j>=BY-H-6;--j){
				gotoxy(i,j);
				write("white","black","  ");
			}
		for(int i=1;i<=W;++i)
			for(int j=1;j<=H+4;++j) lstmp[i][j]=114514;
	}
	void win(){
		clear();
		if(mode==2){
			gotoxy(BX+(W-2)/2,BY-H/2);
			write("yellow","black","VICTORY");
			gotoxy(BX+(W-2)/2,BY-H/2+2);
			write("yellow","black","SURVIVED FOR");
			gotoxy(BX+(W-2)/2,BY-H/2+3);
			write("yellow","black",gettime(GetTickCount()-starttime));
		}else if(mode==3){
			int t=GetTickCount()-starttime;
			gotoxy(BX+(W-2)/2,BY-H/2);
			write("white","black",gettime(t));
			if(t<best40lines){
				best40lines=t;
				gotoxy(BX+(W-2)/2,BY-H/2+2);
				write("yellow","black","PERSONAL BEST");
			}
		}else{
			gotoxy(BX+(W-2)/2,BY-H/2);
			write("white","black",getlevel(lines));
			if(lines>bestblitz){
				bestblitz=lines;
				gotoxy(BX+(W-2)/2,BY-H/2+2);
				write("yellow","black","PERSONAL BEST");
			}
		}
		gotoxy(BX+W/2,BY+2);
		write("white","purple","NEXT");
	}
	void lose(){
		clear();
		gotoxy(BX+(W-2)/2,BY-H/2);
		write("grey","black"," DEFEAT");
		gotoxy(BX+(W-2)/2,BY-H/2+2);
		write("grey","black"," SURVIVED FOR");
		gotoxy(BX+(W-2)/2,BY-H/2+3);
		write("grey","black"," "+gettime(GetTickCount()-starttime));
		gotoxy(BX+W/2,BY+2);
		write("white","purple","NEXT");
	}
	void getok(){
		gotoxy(BX+(W-3)/2,BY+2);
		write("white","black"," WAITING");
	}
	bool checkl(){
		for(int i=0;i<4;++i)
			if(getx(i)<1) return 1;
		return 0;
	}
	bool checkr(){
		for(int i=0;i<4;++i)
			if(getx(i)>W) return 1;
		return 0;
	}
	bool checkb(){
		for(int i=0;i<4;++i)
			if(gety(i)<1) return 1;
		return 0;
	}
	bool checkd(){
		for(int i=0;i<4;++i)
			if(getx(i)>0&&getx(i)<=W&&mp[getx(i)][gety(i)]>=0) return 1;
		return 0;
	}
	bool check(){return checkl()||checkr()||checkb()||checkd();}
	bool down(){--nowy;bool f=check();if(f) ++nowy;return !f;}
	void left(){--nowx;if(check()) ++nowx;}
	void right(){++nowx;if(check()) --nowx;}
	void spin(){
		nowr=(nowr+1)&3;
		if(!check()) return;
		++nowy;
		int tx=nowx;
		while(checkl()) ++nowx;
		while(checkr()) --nowx;
		int f=0;
		if(checkb()) f=1,++nowy;
		if(checkd()){
			nowr=(nowr+3)&3;
			nowy-=f+1;
			nowx=tx;
			return;
		}
		--nowy;
		if(checkb()||checkd()) ++nowy;
		else if(f){
			--nowy;
			if(checkb()||checkd()) ++nowy;
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
		lines+=cnt;
		if(cnt){
			gotoxy(BX-8,BY-14);
			if(cnt==1) cout<<"SINGLE";
			if(cnt==2) cout<<"DOUBLE";
			if(cnt==3) cout<<"TRIPLE";
			if(cnt==4) cout<<"QUAD  ";
		}
		return cnt;
	}
	bool checkTspin(){
		if(nowb!=2) return 0;
		if(nowr!=2) return 0;
		if(mp[nowx-1][nowy+1]>=0) return 1;
		if(mp[nowx+1][nowy+1]>=0) return 1;
		return 0;
	}
	bool checkallclear(){
		for(int i=1;i<=W;++i)
			if(mp[i][1]!=-1) return 0;
		return 1;
	}
	int countattack(){
		int res=0;
		for(auto att:attacks) res+=att.second;
		return res;
	}
	void updatenext(){
		for(int i=BX+W+3;i<=BX+W+6;++i)
			for(int j=BY-H+1;j<=BY-H+20;++j)
				{gotoxy(i,j);write(black,"  ");}
		for(int i=0;i<5;++i)
			for(int j=0;j<4;++j){
				int x=d[nxtb[i]][0][j][0],y=1-d[nxtb[i]][0][j][1];
				gotoxy(x+BX+W+4,y+BY-H+i*4+2);
				write("white",col[nxtb[i]],"  ");
			}
	}
	void updatehold(){
		if(hold==-1) return;
		for(int i=BX-7;i<=BX-4;++i)
			for(int j=BY-H+2;j<=BY-H+5;++j) gotoxy(i,j),write(black,"  ");
		for(int i=0;i<4;++i){
			int x=d[hold][0][i][0],y=d[hold][0][i][1];
			gotoxy(BX-6+x,BY-H+3-y);write("white",col[hold],"  ");
		}
	}
	void updateattack(deque<pair<int,int> > q){
		int cnt=0;
		while(!q.empty()) cnt+=q.front().second,q.pop_front();
		for(int i=1;i<=cnt;++i) gotoxy(BX-0.5,BY-i),write("white","red","  ");
		for(int i=cnt+1;i<=H;++i) gotoxy(BX-0.5,BY-i),write("white","black","  ");
	}
	void newblock(){
		lstb=nowb;
		for(int i=0;i<4;++i) px[i]=getx(i),py[i]=gety(i);
		nowb=nxtb[0];
		for(int i=0;i<4;++i) nxtb[i]=nxtb[i+1];
		if(SocketID==0) nxtb[4]=mt_rand()%7;
		else nxtb[4]=mt_rand2()%7;
		nowr=0;nowx=W/2;nowy=H+3;
		updatenext();
	}
	void gethold(){
		for(int i=0;i<4;++i){
			int x=getx(i),y=gety(i);
			gotoxy(BX+x,BY-y);
			write(black,"  ");
			lstmp[x][y]=114514;
		}
		if(hold==-1){
			hold=nowb;
			newblock();
		}else{
			swap(hold,nowb);
			nowr=0;nowx=W/2;nowy=H+3;
		}
		for(int i=0;i<4;++i){
			int x=getx(i),y=gety(i);
			gotoxy(BX+x,BY-y);
			write("white",col[nowb],"  ");
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
				if(mp[i][H+3]!=-1){
					gameover=1;
					return;
				}
		}
		return;
	}
	void finish(){
		bool b=checkTspin();
		done=0;
		for(int i=0;i<4;++i){
			int x=getx(i),y=gety(i);
			mp[x][y]=nowb;
		}
		newblock();
		int l=remove();
		nowy+=l;
		if(l>0) ++combo;
		else combo=-1;
		for(int i=0;i<4;++i)
			if(py[i]>H+1){
				gameover=1;
				return;
			}
		++pieces;
		if(!l||mode==1) return;
		if(l==4) ++l;
		if(b) l=l+l+1;
		l=l-1+(combo+1)/2;
		if(checkallclear()) l+=5,attacks.push_back({GetTickCount()+500,5});
		attacks.push_back({GetTickCount(),l});
		return;
	}
	void drop(){while(down());finish();}
	bool isbottom(){--nowy;bool f=check();++nowy;return f;}
	void printinfo(){
		int t=GetTickCount()-starttime;
		SetColor(15);
		gotoxy(BX-10,BY-1);
		cout<<"TIME:"<<gettime(t);
		gotoxy(BX-10,BY-3);
		if(mode==4) cout<<getlevel(lines);
		else cout<<"LINES:"<<lines;
		if(mode==3) cout<<"/40";
		gotoxy(BX-10,BY-5);
		cout<<"BLOCKS:"<<pieces;
		gotoxy(BX-10,BY-7);
		cout<<"PPS:"<<fixed<<setprecision(2)<<(double)pieces/(double)t/1000.0<<fixed<<setprecision(0)<<"PIECES/S";
	}
	void printall(){
		if(mode==5){
			for(int i=1;i<=W;++i)
				for(int j=1;j<=H+4;++j){
					gotoxy(BX+i,BY-j);
					write(black,"  ");
				}
		}else if(mode==6){
			for(int i=1;i<=W;++i)
				for(int j=1;j<=H+7&&j<=BY;++j){
					int now=-1;
					if(abs(i-nowx)+abs(j-nowy)<=5) now=(mp[i][j]==-1?7:mp[i][j]);
					if(now!=lstmp[i][j]){
						lstmp[i][j]=now;
						gotoxy(BX+i,BY-j);
						if(now>=0) write("white",col[now],"  ");
						else write(black,"  ");
					}
				}
		}else{
			for(int i=1;i<=W;++i)
				for(int j=1;j<=H+7&&j<=BY;++j)
					if(mp[i][j]!=lstmp[i][j]){
						lstmp[i][j]=mp[i][j];
						gotoxy(BX+i,BY-j);
						if(mp[i][j]>=0) write("white",col[mp[i][j]],"  ");
						else write(black,"  ");
					}
		}
		int yy=nowy;while(!check()) --nowy;++nowy;
		for(int i=0;i<4;++i){
			int x=getx(i),y=gety(i);
			gotoxy(BX+x,BY-y);
			write(black,"[]");
			lstmp[x][y]=114514;
		}
		nowy=yy;
		for(int i=0;i<4;++i){
			int x=getx(i),y=gety(i);
			gotoxy(BX+x,BY-y);
			write("white",col[nowb],"  ");
			lstmp[x][y]=114514;
		}
		if(mode==5&&lstb!=-1)
		for(int i=0;i<4;++i){
			if(!px[i]) continue;
			gotoxy(BX+px[i],BY-py[i]);
			write("white",col[lstb],"  ");
			lstmp[px[i]][py[i]]=114514;
		}
	}
	void show(int x){
		for(int i=0;i<5;++i)
			for(int j=0;j<7;++j){
				gotoxy(BX+(W+1)/2.0-2+i,BY-(H+1)/2.0+3-j);
				if(image[x][i][j]) write("white","yellow","  ");
				else write("white","black","  ");
			}
	}
	void init(){
		memset(mp,-1,sizeof(mp));
		memset(lstmp,-1,sizeof(lstmp));
		attacks.clear();
		done=0;
		gameover=0;
		lines=pieces=0;
		combo=-1;
		hold=lstb=-1;
		gotoxy(BX-6,BY-H);
		write(black,"HOLD");
		gotoxy(BX+W+4,BY-H);
		write(black,"NEXT");
		for(int i=1;i<=H;++i){
			gotoxy(BX,BY-i);cout<<" |";
			gotoxy(BX+W+1,BY-i);cout<<"|";
			gotoxy(BX-1,BY-i);cout<<"|";
		}
		gotoxy(BX-1,BY);cout<<" -";
		for(int i=0;i<=W;++i) gotoxy(BX+i,BY),cout<<"--";
		if(SocketID==0) for(int i=0;i<5;++i) nxtb[i]=mt_rand()%7;
		else for(int i=0;i<5;++i) nxtb[i]=mt_rand2()%7;
		newblock();
		memset(px,0,sizeof(px));
		memset(py,0,sizeof(py));
		lastkey=-1;
	}
};
Player P1(10,20,11,27,true);
Player P2(10,20,39,27,true);
void addtime(int x){
	starttime+=x;
	lastturn+=x;
	lastblock+=x;
	lastinc+=x;
	if(mode==1) return;
	deque<pair<int,int> > q;
	while(!P1.attacks.empty()) q.push_back(P1.attacks.front()),q.back().first+=x,P1.attacks.pop_front();
	while(!q.empty()) P1.attacks.push_back(q.front()),q.pop_front();
	while(!P2.attacks.empty()) q.push_back(P2.attacks.front()),q.back().first+=x,P2.attacks.pop_front();
	while(!q.empty()) P2.attacks.push_back(q.front()),q.pop_front();
}
void hello(){
	while(1){
		int n,a[555],s=0;
		cin>>n;
		if(n==114514) break;
		for(int i=1;i<=n;++i) cin>>a[i];
		for(int i=1;i<=n;++i) s^=a[i];
		cout<<s<<endl;
	}
	system("cls");
	hidecursor();
}

void init(){
	hwnd=GetForegroundWindow();
	system("cls");
	cout<<"TETR.IO(SERVER)\n\n";
	PORT=RandomPort();
	cout<<"PLAYER 1 PORT: "<<PORT<<endl;
	connect(PORT.c_str(),ClientSocket[0]);
	PORT=RandomPort();
	cout<<"PLAYER 2 PORT: "<<PORT<<endl;
	connect(PORT.c_str(),ClientSocket[1]);
	int tt=GetTickCount();
	system("cls");
	cout<<"PRESS SPACE TO START\n";
	while(getch()!=' ');
	system("cls");
	P1.SocketID=0;
	P2.SocketID=1;
	send(ClientSocket[0],tooos(seed).c_str(),12,0);
	send(ClientSocket[1],tooos(seed).c_str(),12,0);
	seed2=seed;
	speed=start_speed;
	P1.init();
	P2.init();
	P1.show(3);
	if(mode==2) P2.show(3);
	Sleep(1000);
	P1.show(2);
	if(mode==2) P2.show(2);
	Sleep(1000);
	P1.show(1);
	if(mode==2) P2.show(1);
	Sleep(1000);
	P1.show(0);
	if(mode==2) P2.show(0);
	lastturn=lastblock=lastinc=starttime=GetTickCount();
	P1.lastktime=P2.lastktime=GetTickCount()-delay;
	P1.isfirst=P2.isfirst=0;
}
int __cdecl main(void) 
{
	hello();
	init();
	while(1){
		if(P1.gameover){
			
		}
		if(P2.gameover){
			
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
		P1data=P2data="";
		int sum1=0,sum2=0;
		while(!P1.attacks.empty()&&GetTickCount()-P1.attacks.front().first>attackdelay){
			sum1+=P1.attacks.front().second;
			P2.addblocks(P1.attacks.front().second);
			P1.attacks.pop_front();
		}
		while(!P2.attacks.empty()&&GetTickCount()-P2.attacks.front().first>attackdelay){
			sum2+=P2.attacks.front().second;
			P1.addblocks(P2.attacks.front().second);
			P2.attacks.pop_front();
		}
		P1data=P2data="";
		P1data=toos(sum2)+" "+toos(sum1)+" ";
		P2data=toos(sum1)+" "+toos(sum2)+" ";
		sum1=P1.countattack();
		sum2=P2.countattack();
		P1data+=toos(sum2)+" "+toos(sum1)+" ";
		P2data+=toos(sum1)+" "+toos(sum2)+" ";
		memset(recvbuf,0,sizeof(recvbuf));
		recv(ClientSocket[0],recvbuf,recvbuflen,0);
		last1=kb=Getkey();
		if(kb){
			if(P1.lastktime<GetTickCount()-delay||kb!=P1.lastkey||(!P1.isfirst&&P1.lastktime<GetTickCount()-delay2)){
				int tmp=P1.lastktime,tmpk=P1.lastkey,tmpf=P1.isfirst;
				bool f=1;
				if(kb!=P1.lastkey) P1.isfirst=1;
				P1.lastktime=GetTickCount();
				P1.lastkey=kb;
				switch(kb){
					case 38:P1.spin();break;
					case 40:
						if(P1._down) while(!P1.isbottom()) P1.down();
						else P1.down();
						break;
					case 37:P1.left();break;
					case 39:P1.right();break;
					case 32:P1.drop();break;
					case 160:case 161:P1.gethold();break;
					default:P1.lastktime=tmp;P1.lastkey=tmpk;P1.isfirst=tmpf;f=0;
				}
				if(f) P1.printall();
			}else P1.isfirst=0,P1.lastkey=-1;
		}
		P1data+=toos(last2);
		memset(recvbuf,0,sizeof(recvbuf));
		recv(ClientSocket[1],recvbuf,recvbuflen,0);
		last2=kb=Getkey();
		if(kb){
			if(P2.lastktime<GetTickCount()-delay||kb!=P2.lastkey||(!P2.isfirst&&P2.lastktime<GetTickCount()-delay2)){
				int tmp=P2.lastktime,tmpk=P2.lastkey,tmpf=P2.isfirst;
				bool f=1;
				if(kb!=P2.lastkey) P2.isfirst=1;
				P2.lastktime=GetTickCount();
				P2.lastkey=kb;
				switch(kb){
					case 38:P2.spin();break;
					case 40:
						if(P2._down) while(!P2.isbottom()) P2.down();
						else P2.down();
						break;
					case 37:P2.left();break;
					case 39:P2.right();break;
					case 32:P2.drop();break;
					case 160:case 161:P2.gethold();break;
					default:P2.lastktime=tmp;P2.lastkey=tmpk;P2.isfirst=tmpf;f=0;
				}
				if(f) P2.printall();
			}else P2.isfirst=0,P2.lastkey=-1;
		}
		P2data+=toos(last1);
		P1data+=" nn";
		P2data+=" nn";
		if(GetTickCount()-lastturn<=1.0/speed){
			send(ClientSocket[0],P1data.c_str(),P1data.size(),0);
			send(ClientSocket[1],P2data.c_str(),P2data.size(),0);
			continue;
		}
		lastturn=GetTickCount();
		if(P1.isbottom()){
			if(P1.done){
				P1.finish();
				P1data[20]='f';
				P2data[21]='f';
			}else P1.done=1;
		}else{
			P1.down();
			P1.done=0;
			P1data[20]='d';
			P2data[21]='d';
		}
		if(P2.isbottom()){
			if(P2.done){
				P2.finish();
				P1data[20]='f';
				P2data[21]='f';
			}else P2.done=1;
		}else{
			P2.down();
			P2.done=0;
			P1data[20]='d';
			P2data[21]='d';
		}
		if(GetTickCount()-lastinc>inc_turn){
			lastinc=GetTickCount();
			speed+=speed_inc;
		}
		cout<<P1data.size()<<" "<<P2data.size();
		system("pause");
		send(ClientSocket[0],P1data.c_str(),P1data.size(),0);
		send(ClientSocket[1],P2data.c_str(),P2data.size(),0);
		P1.printall();
		P2.printall();
	}

	for(int SocketID=0;SocketID<2;++SocketID){
		iResult = shutdown(ClientSocket[SocketID], SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket[SocketID]);
			WSACleanup();
			return 1;
		}
		closesocket(ClientSocket[SocketID]);
	}
	WSACleanup();
	return 0;
}
