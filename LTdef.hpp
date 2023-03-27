#include <bits/stdc++.h>
#include <windows.h>
#include "LTgraphics.hpp"
using namespace std;
const int dx[4]={-1,1,0,0};
const int dy[4]={0,0,-1,1};
const int col[8]={
EGERGB(0x52,0xce,0xac),EGERGB(0xce,0xc5,0x52),EGERGB(0xc3,0x52,0xce),EGERGB(0x66,0x52,0xce),
EGERGB(0xce,0x81,0x52),EGERGB(0xce,0x52,0x59),EGERGB(0x80,0xce,0x52),EGERGB(0x7f,0x7f,0x7f)};
const int d[7][4][4][2]=
{
	{//I
		{{-1,0},{0,0},{1,0},{2,0}},
		{{0,1},{0,0},{0,-1},{0,-2}},
		{{-1,0},{0,0},{1,0},{2,0}},
		{{0,1},{0,0},{0,-1},{0,-2}}
	},
	{//O
		{{0,1},{1,1},{0,0},{1,0}},
		{{0,1},{1,1},{0,0},{1,0}},
		{{0,1},{1,1},{0,0},{1,0}},
		{{0,1},{1,1},{0,0},{1,0}}
	},
	{//T
		{{0,1},{-1,0},{0,0},{1,0}},
		{{0,1},{0,0},{1,0},{0,-1}},
		{{-1,0},{0,0},{1,0},{0,-1}},
		{{0,1},{-1,0},{0,0},{0,-1}}
	},
	{//J
		{{-1,1},{-1,0},{0,0},{1,0}},
		{{0,1},{1,1},{0,0},{0,-1}},
		{{-1,0},{0,0},{1,0},{1,-1}},
		{{0,1},{0,0},{-1,-1},{0,-1}}
	},
	{//L
		{{1,1},{-1,0},{0,0},{1,0}},
		{{0,1},{0,0},{0,-1},{1,-1}},
		{{-1,0},{0,0},{1,0},{-1,-1}},
		{{-1,1},{0,1},{0,0},{0,-1}}
	},
	{//Z
		{{-1,1},{0,1},{0,0},{1,0}},
		{{1,1},{0,0},{1,0},{0,-1}},
		{{-1,1},{0,1},{0,0},{1,0}},
		{{1,1},{0,0},{1,0},{0,-1}}
	},
	{//S
		{{0,1},{1,1},{-1,0},{0,0}},
		{{0,1},{0,0},{1,0},{1,-1}},
		{{0,1},{1,1},{-1,0},{0,0}},
		{{0,1},{0,0},{1,0},{1,-1}}
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
string gettime(int t){
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
int getB2Blevel(int B2Bs){
	switch(B2Bs){
		case -1 ... 0:return 0;
		case 1 ... 2:return 1;
		case 3 ... 7:return 2;
		case 8 ... 23:return 3;
		case 24 ... 66:return 4;
		case 67 ... 184:return 5;
		case 185 ... 503:return 6;
		case 504 ... 1369:return 7;
		default:return 8;
	}
}
int calcgarbage(int B2B,int lines,int combo){
	B2B=getB2Blevel(B2B);
	if(combo==-1) combo=0;
	if(lines==0&&B2B==0){
		switch(combo){
			case 0 ... 1:return 0;
			case 2 ... 5:return 1;
			case 6 ... 15:return 2;
			case 16 ... 42:return 3;
			default:return 4;
		}
	}
	return (lines+B2B)*(1.0+(double)combo/4.0);
}
const double startspeed=0.001;
const double speed_inc=0.0003;
const int inc_turn=15000;
const int blockturn=15000;
const int attackdelay=3000;
const int delayt=140;
const int delay2=30;
int best40lines=3600000;
int bestTspin=3600000;
int bestblitz=-1;
bool redraw=1;
HHOOK keyboardHook=0;
int kb=0;
LRESULT CALLBACK LowLevelKeyboardProc(_In_ int nCode,_In_ WPARAM wParam,_In_ LPARAM lParam){
	KBDLLHOOKSTRUCT *ks=(KBDLLHOOKSTRUCT*)lParam;
	int k=0;
	if(ks->flags==0||ks->flags==1) k=(ks->vkCode);
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
mouse_msg msgRecord={0};
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
	system("cls");
	initgraph(1000,440,INIT_RENDERMANUAL);
	movewindow(300,300,1);
	setbkmode(TRANSPARENT);
}
