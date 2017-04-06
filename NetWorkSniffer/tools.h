#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <stdio.h>
#include <signal.h>

void SignalWhileNow(int signum);
#define CLEARSCREEN() printf("\033[H\033[2J")
#define	INITCOLOR(color) printf("\033[%sm", color)
#define	RED_COLOR "31"
#define	GREEN_COLOR	"32"
#define	YELLOW_COLOR "33"
#define	BLUE_COLOR "34"
#define	ZERO_COLOR "0"

#endif //__TOOLS_H__
