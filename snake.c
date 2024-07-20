#include <ncurses/ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN
} direction;

typedef struct node {
    int sx;
    int sy;
    struct node *next;
    direction dir;
} node;

typedef struct {
    int ax;
    int ay;
} apple;

void update_time(int *min, int *sec, clock_t *last_clock) {
    clock_t current_clock = clock();
    double elapsed_time = (double)(current_clock - *last_clock) / CLOCKS_PER_SEC;

    if (elapsed_time >= 1.0) {
        *sec += (int)elapsed_time;
        if (*sec >= 60) {
            *min += *sec / 60;
            *sec %= 60;
        }
        *last_clock = current_clock;
    }
}

node *growth(node *head) {
    node *c = head;
    node *temp = malloc(sizeof(node));

    if (temp == NULL) {

        return NULL;
    }

    temp->next = NULL;

    while (c->next != NULL) {
        c = c->next;
    }

    c->next = temp;
    temp->dir = c->dir;

    switch (c->dir) {
        case LEFT:
            temp->sx = c->sx + 1;
            temp->sy = c->sy;
            break;
        case RIGHT:
            temp->sx = c->sx - 1;
            temp->sy = c->sy;
            break;
        case UP:
            temp->sx = c->sx;
            temp->sy = c->sy + 1;
            break;
        case DOWN:
            temp->sx = c->sx;
            temp->sy = c->sy - 1;
            break;
    }

    return head;
}

node *movement(node *head) {
    if (head == NULL) {
        return NULL;
    }

    node *c = head;
    int oldx = c->sx;
    int oldy = c->sy;
    int t_x, t_y;

    switch (c->dir) {
        case RIGHT:
            c->sx++;
            break;
        case LEFT:
            c->sx--;
            break;
        case UP:
            c->sy--;
            break;
        case DOWN:
            c->sy++;
            break;
    }

    c = c->next;
    while (c != NULL) {
        t_x = c->sx;
        t_y = c->sy;

        c->sx = oldx;
        c->sy = oldy;

        oldx = t_x;
        oldy = t_y;

        c = c->next;
    }

    return head;
}

void free_list(node *head) {
    node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    node *head = malloc(sizeof(node));
    if (head == NULL) {
        return -1;
    }
    node *current;

    head->sx = 75;
    head->sy = 20;
    head->dir = RIGHT;
    head->next = NULL;

    int ch;
    apple a;

    srand(time(NULL));
    int min = 0, sec = 0;
    clock_t last_clock = clock();

    int x = 0;
    int y = 0;
    int height = 41;
    int width = 156;
    char ans[10];

    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    WINDOW *win = newwin(height, width, y, x);
    refresh();
    box(win, 0, 0);
    wrefresh(win);
    nodelay(win, TRUE);

    a.ax = rand() % (width - 2) + 1;
    a.ay = rand() % (height - 2) + 1;
    int score = 0;

    while (1) {
        timeout(10);
        ch = getch();
        if (ch != ERR) {
            if (ch == KEY_RIGHT && head->dir != LEFT) {
                head->dir = RIGHT;
            } 
            else if (ch == KEY_LEFT && head->dir != RIGHT) {
                head->dir = LEFT;
            } 
            else if (ch == KEY_UP && head->dir != DOWN) {
                head->dir = UP;
            } 
            else if (ch == KEY_DOWN && head->dir != UP) {
                head->dir = DOWN;
            }
        }

        if (head->sx <= 0 || head->sx >= width - 1 || head->sy <= 0 || head->sy >= height - 1) {
            break;
        }

        current = head;
        while (current != NULL) {
            mvwaddch(win, current->sy, current->sx, ' ');
            current = current->next;
        }

        movement(head);

        current = head;
        while (current != NULL) {
            mvwaddch(win, current->sy, current->sx, '*');
            current = current->next;
        }

        mvwaddch(win, a.ay, a.ax, 'o');

        if (head->sx == a.ax && head->sy == a.ay) {
            growth(head);
            a.ax = rand() % (width - 2) + 1;
            a.ay = rand() % (height - 2) + 1;
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
    WINDOW *gameoverwin = newwin(10, 40, (height / 2) - 5, (width / 2) - 20);
    box(gameoverwin, 0, 0);
    mvwprintw(gameoverwin, 3, 8, "Game over, your score was %d", score);
    mvwprintw(gameoverwin, 5, 8, "Your time was %02d:%02d", min, sec);
    wrefresh(gameoverwin);
    wgetch(gameoverwin);
    delwin(gameoverwin);
    endwin();

    free_list(head);
    return 0;
}
