#include <ncurses/ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> 

typedef struct {
    int sx;
    int sy;
} snake;

typedef struct {
    int ax;
    int ay;
} apple;

void update_time(int *min, int *sec, clock_t *last_clock) {
    clock_t current_clock=clock();
    double elapsed_time=(double)(current_clock-*last_clock)/CLOCKS_PER_SEC;
    
    if (elapsed_time >= 1.0) {
        *sec += (int)elapsed_time;
        if (*sec>=60){
            *min+=*sec/60;
            *sec%=60;
        }
        *last_clock = current_clock;
    }
}

int main() {
    snake s;
    apple a;
    srand(time(NULL));
    int min= 0, sec =0;
    clock_t last_clock = clock(); 
    
    int x=0;
    int y=0;
    int height=41;
    int width=156;
    char ans[10];

    initscr();
    cbreak();
    keypad(stdscr, TRUE);  
    noecho();
	curs_set(0);
    WINDOW* win=newwin(height, width, y, x);
    refresh();
    box(win, 0, 0);
    wrefresh(win);
    keypad(win, TRUE);
    nodelay(win, TRUE);

    s.sx=1;
    s.sy=0;

    int posx=75;
    int posy=20;

    a.ax=rand()%(width-2)+1;
    a.ay=rand()%(height-2)+1;
    int score=0;
    int old_posx=posx;
    int old_posy=posy;
    
    while (1) {
        int press=wgetch(win);
        if (press==KEY_UP && s.sy!=1) {
            s.sy=-1;
            s.sx=0;
        } else if (press ==KEY_DOWN&& s.sy != -1) {
            s.sy=1;
            s.sx=0;
        } else if (press==KEY_LEFT&&s.sx != 1) {
            s.sy=0;
            s.sx=-1;
        } else if (press==KEY_RIGHT&&s.sx != -1) {
            s.sy=0;
            s.sx=1;
        }

        old_posx=posx;
        old_posy=posy;
        posx+=s.sx;
        posy+=s.sy;

        if (posx<=0 || posx>=width-1 || posy<=0 || posy>=height-1) {
			break;
        }

      
        mvwaddch(win, old_posy, old_posx, ' ');
        mvwaddch(win, posy, posx, '*');
        mvwaddch(win, a.ay, a.ax, 'o');
        
        if (a.ax==posx && a.ay==posy) {
            a.ax=rand()%(width-2)+1;
            a.ay=rand()%(height-2)+1;
            score++;
        }

        
        update_time(&min, &sec, &last_clock);
        mvwprintw(win, 1, 1, "%02d:%02d", min, sec);
        mvwprintw(win, 1, 142, "Score:%d", score);
        wrefresh(win);

        Sleep(100); 
    }
	
	wclear(win);
	wrefresh(win);
    delwin(win);
    refresh();
    WINDOW *gameoverwin = newwin(10, 40, (height/2)-5,(width/2)-20);
    box(gameoverwin, 0, 0);
    mvwprintw(gameoverwin, 3, 8, "Game over, your score was %d", score);
	mvwprintw(gameoverwin, 5,8, "Your time was %d:%d", min, sec);
    wrefresh(gameoverwin);
    wgetch(gameoverwin);
    delwin(gameoverwin);
    endwin();
    return 0;
}
