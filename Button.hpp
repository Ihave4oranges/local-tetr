#ifndef __BUTTON_HPP__
#define __BUTTON_HPP__
#include <bits/stdc++.h>
#include <graphics.h>
#include "Const.hpp"
using namespace std;
struct Button{
	int x,y,w,h,mode,fontSize,titlex,titley,lineWidth;
	color_t fillColor,edgeColor,onColor,clickColor,fontColor;
	string title,font;
	void setCoord(int _x,int _y){x=_x;y=_y;}
	void setSize(int _w,int _h){w=_w;h=_h;}
	void setFont(int x){fontSize=x;}
	void setFont(string s){font=s;}
	void setFont(int x,string s){fontSize=x;font=s;}
	void setTitle(string s){title=s;}
	void setTitleCoord(int x,int y){titlex=x;titley=y;}
	void setLineWidth(int w){lineWidth=w;}
	void setFillColor(color_t c){fillColor=c;}
	void setEdgeColor(color_t c){edgeColor=c;}
	void setOnColor(color_t c){onColor=c;}
	void setClickColor(color_t c){clickColor=c;}
	void setFontColor(color_t c){fontColor=c;}
	void setColor(color_t c){fillColor=c;}
	void setColor(color_t c1,color_t c2,color_t c3,color_t c4,color_t c5)
	{fillColor=c1;edgeColor=c2;onColor=c3;clickColor=c4;fontColor=c5;}
	void show(int Mode){
		mode=Mode;
		setcolor(edgeColor);
		setlinewidth(lineWidth);
		if(mode==0) setfillcolor(fillColor);
		else if(mode==1) setfillcolor(onColor);
		else setfillcolor(clickColor);
		ege_fillrect(x,y,w,h);
		ege_rectangle(x,y,w,h);
		setcolor(fontColor);
		setfont(fontSize,0,font.c_str());
		xyprintf(x+titlex,y+titley,title.c_str());
	}
	void hide(){
		setfillcolor(BLACK);
		ege_fillrect(x-lineWidth,y-lineWidth,w+lineWidth,h+lineWidth);
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
struct EditText{
	int x,y,w,h,mode,textx,texty,cursorPos,fontSize,maxLen,lineWidth,charWidth,cursorWidth;
	bool numOnly;
	string text,font;
	color_t fillColor,edgeColor,cursorColor,fontColor;
	EditText(){cursorPos=numOnly=0;text="";maxLen=-1;charWidth=cursorWidth=0;}
	void validate(){
		while(maxLen!=-1&&text.size()>maxLen) text.pop_back();
		string ns="";
		if(numOnly){
			for(int i=0;i<text.size();++i)
				if(isdigit(text[i])) ns+=text[i];
			text=ns;
		}
		cursorPos=min(cursorPos,(int)text.size());
	}
	void setMaxLen(int mxln){maxLen=mxln;}
	void setNumOnly(bool no){numOnly=no;}
	void setCursorWidth(int cw){cursorWidth=cw;}
	void setCoord(int _x,int _y){x=_x;y=_y;}
	void setSize(int _w,int _h){w=_w;h=_h;}
	void setTextCoord(int xx,int yy){textx=xx;texty=yy;}
	void setFont(int x){fontSize=x;}
	void setFont(string s){font=s;}
	void setFont(int x,string s){fontSize=x;font=s;}
	void setCharWidth(int cw){charWidth=cw;}
	void setText(string ss){text=ss;validate();}
	void setCursorPos(int cp){if(cp>=0&&cp<=text.size()) cursorPos=cp;}
	void setLineWidth(int w){lineWidth=w;}
	void setFillColor(color_t c){fillColor=c;}
	void setEdgeColor(color_t c){edgeColor=c;}
	void setCursorColor(color_t c){cursorColor=c;}
	void setFontColor(color_t c){fontColor=c;}
	void setColor(color_t c){fillColor=c;}
	void setColor(color_t c1,color_t c2,color_t c3,color_t c4)
	{fillColor=c1;edgeColor=c2;cursorColor=c3;fontColor=c4;}
	string getText(){return text;}
	int getIntText(){return toi(text);}
	void show(int Mode){
		mode=Mode;
		setlinewidth(lineWidth);
		setcolor(edgeColor);
		setfillcolor(fillColor);
		ege_fillrect(x,y,w,h);
		ege_rectangle(x,y,w,h);
		setcolor(fontColor);
		setfont(fontSize,0,font.c_str());
		xyprintf(x+textx,y+texty,text.c_str());
		if(mode==1){
			setlinewidth(cursorWidth);
			ege_line(x+textx+charWidth*cursorPos,y+texty,x+textx+charWidth*cursorPos,y+texty+fontSize);
		}
	}
	bool isin(int nx,int ny){
		if(nx>=x&&ny>=y&&nx<x+w&&ny<y+h) return 1;
		return 0;
	}
	void update(mouse_msg mouse){
		if(!isin(mouse.x,mouse.y)){
			if(mouse.is_down()&&mouse.is_left()) show(0);
		}else{
			if(mouse.is_up()&&mouse.is_left()){
				cursorPos=(mouse.x-x-textx)/charWidth;
				cursorPos=max(cursorPos,0);
				cursorPos=min(cursorPos,(int)text.size());
				show(1);
			}
		}
	}
	void update(int key){
		if(mode!=1) return;
		if(key>=37&&key<=40){
			if(key==37) cursorPos=max(cursorPos-1,0);
			if(key==38) cursorPos=0;
			if(key==39) cursorPos=min(cursorPos+1,(int)text.size());
			if(key==40) cursorPos=text.size();
			show(1);
			return;
		}
		if(key!=8&&key!=46){
			if(numOnly&&!isdigit(key)) return;
			if(!isdigit(key)&&(key<65||key>90)&&(key<97&&key>122)) return;
		}
		string ns="";
		if(key==8){
			if(cursorPos==0) return;
			--cursorPos;
			for(int i=0;i<cursorPos;++i) ns+=text[i];
			for(int i=cursorPos+1;i<text.size();++i) ns+=text[i];
		}else if(key==46){
			if(cursorPos==text.size()) return;
			for(int i=0;i<cursorPos;++i) ns+=text[i];
			for(int i=cursorPos+1;i<text.size();++i) ns+=text[i];
		}else{
			for(int i=0;i<cursorPos;++i) ns+=text[i];
			ns+=(char)key;
			for(int i=cursorPos;i<text.size();++i) ns+=text[i];
			++cursorPos;
		}
		text=ns;
		validate();
		show(1);
	}
};
#endif
