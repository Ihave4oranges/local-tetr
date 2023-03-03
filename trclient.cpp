#define WIN32_LEAN_AND_MEAN

#include <bits/stdc++.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <conio.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "114514"

using namespace std;

HWND hwnd;
WSADATA wsaData;
SOCKET ConnectSocket = INVALID_SOCKET;
struct addrinfo *result = NULL,*ptr = NULL,hints;
string sendbuf;
char recvbuf[DEFAULT_BUFLEN];
int iResult;
int recvbuflen = DEFAULT_BUFLEN;
string target;
string PORT;
unsigned int seed,seed2;
unsigned int mt_rand(){return seed=seed*998244353ll+19260817ll;}
unsigned int mt_rand2(){return seed2=seed2*998244353ll+19260817ll;}

int connect(string ServerIP,string PORT,SOCKET &ConnectSocket){
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	iResult = getaddrinfo(ServerIP.c_str(), PORT.c_str(), &hints, &result);
	if ( iResult != 0 ) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}
	cout<<"connected\n>>>";
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
int readint(int l,int r){
	int res=0;
	for(int i=l;i<=r;++i) res=res*10+recvbuf[i]-48;
	return res;
}
HHOOK keyboardHook=0;
int kb=0;
LRESULT CALLBACK LowLevelKeyboardProc(_In_ int nCode,_In_ WPARAM wParam,_In_ LPARAM lParam){
	KBDLLHOOKSTRUCT *ks=(KBDLLHOOKSTRUCT*)lParam;
	int k=0;
	if(GetForegroundWindow()==hwnd&&(ks->flags==0||ks->flags==1)) k=(ks->vkCode);
	kb=k;
	return CallNextHookEx(NULL,nCode,wParam,lParam);
}
MSG msg;
void updatekb(){
	if(PeekMessageA(&msg,NULL,NULL,NULL,PM_REMOVE)){
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}else Sleep(0);
}
void clearkb(){
	do updatekb();
	while(kb);
}
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
	void updateattack(int att){
		for(int i=1;i<=att;++i) gotoxy(BX-0.5,BY-i),write("white","red","  ");
		for(int i=att+1;i<=H;++i) gotoxy(BX-0.5,BY-i),write("white","black","  ");
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
void pause(int c){
	int Tnow=GetTickCount();
	Sleep(300);
	clearkb();
	while(1){
		updatekb();
		if(kb==c) break;
	}
	Tnow=GetTickCount()-Tnow;
	addtime(Tnow);
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
	SetConsoleOutputCP(65001);
	keyboardHook=SetWindowsHookEx(WH_KEYBOARD_LL,LowLevelKeyboardProc,GetModuleHandleA(NULL),NULL);
	if(keyboardHook==0){
		cout<<"¹Ò¹³¼üÅÌÊ§°Ü";
		exit(-1);
	}
	hidecursor();
}

void init(){
	hwnd=GetForegroundWindow();
	system("cls");
	cout<<"TETR.IO\n\n";
	cout<<"ServerIP:";
	cin>>target;
	cout<<"port:";
	cin>>PORT;
	connect(target,PORT,ConnectSocket);
	recv(ConnectSocket,recvbuf,recvbuflen,0);
	seed=seed2=readint(0,11);
	P1.SocketID=0;
	P2.SocketID=1;
	clearkb();
	system("cls");
	speed=start_speed;
	P1.init();
	P2.init();
	P1.show(3);
	P2.show(3);
	Sleep(1000);
	P1.show(2);
	P2.show(2);
	Sleep(1000);
	P1.show(1);
	P2.show(1);
	Sleep(1000);
	P1.show(0);
	P2.show(0);
	lastturn=lastblock=lastinc=starttime=GetTickCount();
	P1.lastktime=P2.lastktime=GetTickCount()-delay;
	P1.isfirst=P2.isfirst=0;
	clearkb();
}
int __cdecl main(int argc, char **argv) 
{
	hello();
	init();

	while(1){
		P1.printinfo();
		P2.printinfo();
		updatekb();
		send(ConnectSocket,toos(kb).c_str(),3,0);
//		switch(kb){
//			case 38:P1.spin();break;
//			case 40:
//				if(P1._down) while(!P1.isbottom()) P1.down();
//				else P1.down();
//				break;
//			case 37:P1.left();break;
//			case 39:P1.right();break;
//			case 32:P1.drop();break;
//			case 160:case 161:P1.gethold();break;
//		}
		memset(recvbuf,0,sizeof(recvbuf));
		recv(ConnectSocket,recvbuf,recvbuflen,0);
		P1.addblocks(readint(0,2));
		P2.addblocks(readint(4,6));
		P1.updateattack(readint(8,10));
		P2.updateattack(readint(12,14));
		int kbd=readint(16,18);
//		switch(kbd){
//			case 38:P2.spin();break;
//			case 40:
//				if(P2._down) while(!P2.isbottom()) P2.down();
//				else P2.down();
//				break;
//			case 37:P2.left();break;
//			case 39:P2.right();break;
//			case 32:P2.drop();break;
//			case 160:case 161:P2.gethold();break;
//		}
//		switch(recvbuf[20]){
//			case 'f':P1.finish();break;
//			case 'd':P1.down();break;
//		}
//		switch(recvbuf[21]){
//			case 'f':P2.finish();break;
//			case 'd':P2.down();break;
//		}
		P1.printall();
		P2.printall();
	}

	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}
