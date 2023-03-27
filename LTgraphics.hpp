#include <bits/stdc++.h>
#include <graphics.h>
using namespace std;
struct Button{
	int x,y,w,h,mode,fontsize,titlex,titley;
	color_t fillcolor,color,oncolor,clickcolor,fontcolor;
	string name,font;
	Button(int _x,int _y,int _w,int _h,
	color_t _fillcol,color_t _col,color_t _oncol,color_t _clickcol,
	string _name,string _font,int _fontsize,int _titlex,int _titley,color_t _fontcolor)
	{mode=-1;x=_x;y=_y;w=_w;h=_h;fillcolor=_fillcol;color=_col;oncolor=_oncol;clickcolor=_clickcol;
	name=_name;font=_font;fontsize=_fontsize;titlex=_titlex;titley=_titley;fontcolor=_fontcolor;}
	void show(int Mode){
		if(mode==Mode) return;
		mode=Mode;
		setcolor(color);
		if(mode==0) setfillcolor(fillcolor);
		else if(mode==1) setfillcolor(oncolor);
		else setfillcolor(clickcolor);
		ege_fillrect(x,y,w,h);
		ege_rectangle(x,y,w,h);
		setcolor(fontcolor);
		setfont(fontsize,0,font.c_str());
		xyprintf(x+titlex,y+titley,name.c_str());
	}
	void hide(){
		setfillcolor(BLACK);
		ege_fillrect(x-4,y-4,w+8,h+8);
	}
	bool isin(int nx,int ny){
		if(nx>=x&&ny>=y&&nx<x+w&&ny<y+h) return 1;
		return 0;
	}
	bool update(mouse_msg mouse){
		if(!isin(mouse.x,mouse.y)){show(0);return 0;}
		if(mouse.is_down()&&mouse.is_left()){show(2);return 0;}
		if(mode==2&&mouse.is_up()&&mouse.is_left()){show(1);return 1;}
		show(1);return 0;
	}
};
