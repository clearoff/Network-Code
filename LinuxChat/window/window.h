#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <cstring>


#define SIZE 1024

class Window{
	public:
		WINDOW* header;
		WINDOW* output;
		WINDOW* friend_list;
		WINDOW* input;
	public:
		Window();
		~Window();
		void putstrTowin(WINDOW* win,int starty,int startx,const std::string msg);
		std::string getInputmsg(WINDOW* win);
		void clrWinLine(WINDOW* win,int begin,int lines);
		void destroyWin(WINDOW* win);
		WINDOW* create_newwin(int heigth,int width,int startx,int starty);
		void winRefresh(WINDOW* win);
		void createHeader();
		void createOutput();
		void createFriendlist();
		void createInput(std::string& out);
};


#endif //__WINDOW_H__
