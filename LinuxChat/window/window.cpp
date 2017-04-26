#include "window.h"
using namespace std;


Window::Window()
{
	initscr();
}

Window::~Window()
{
	endwin();
}

void Window::putstrTowin(WINDOW* win,int starty,int startx,const \
		std::string msg)
{
	mvwaddstr(win,starty,startx,msg.c_str());
}

std::string Window::getInputmsg(WINDOW* win)
{
	char buf[SIZE];
	memset(buf,0,sizeof(buf));
	wgetnstr(win,buf,SIZE);
	//putstrTowin(output,1,1,buf);
	return buf;
}

void Window::clrWinLine(WINDOW* win,int begin,int lines)
{
	while(lines-->0){
		wmove(win,begin++,0);
		wclrtoeol(win);
	}
}

void Window::destroyWin(WINDOW* win)
{
	delwin(win);
}

void Window::winRefresh(WINDOW *win)
{
	wrefresh(win);
}

WINDOW* Window::create_newwin(int heigth,int width,int starty,int startx)
{
	WINDOW* local = newwin(heigth,width,starty,startx); 
	box(local,0,0);
	return local;
}

void Window::createHeader()
{
	int starty = 0;
	int startx = 0;
	int heigth = LINES/6;
	int width = COLS;
	header = create_newwin(heigth, width, starty, startx);
}

void Window::createOutput()
{
	int starty = LINES/6;
	int startx = 0;
	int heigth = (LINES*10)/18;
	int width = (COLS*3)/4;
	output = create_newwin(heigth, width, starty, startx);
}

void Window::createFriendlist()
{
	int starty = LINES/6;
	int startx = COLS*3/4;
	int heigth = LINES*10/18;
	int width = COLS*1/4;
	friend_list = create_newwin(heigth, width, starty, startx);
}
void Window::createInput(std::string& out)
{
	int starty = 13*LINES/18;
	int startx = 0;
	int heigth = LINES*5/18;
	int width = COLS;
	input = create_newwin(heigth, width, starty, startx);

	int output_starty=2;
	int output_startx=2;
	std::string msg = "Enter >:";
	putstrTowin(input,output_starty,output_startx,msg);
	out = getInputmsg(input);
}



#ifdef _DEBUG_

int main()
{
	Window win;
	win.createHeader();
	win.winRefresh(win.header);
	sleep(1);
	win.createOutput();
	win.winRefresh(win.output);
	sleep(1);
	win.createFriendlist();
	win.winRefresh(win.friend_list);
	sleep(1);
	std::string out;
	int step=1;
	int h,w;
	getmaxyx(win.output,h,w);
	while(1){
		out="";
		win.createInput(out);
		win.winRefresh(win.input);
		win.putstrTowin(win.output,step++,2,out);
		win.winRefresh(win.output);
		step %= h;
		step = (step==0?1:step);
		usleep(300000);
	}
	return 0;
}

#endif //_DEBUG_
