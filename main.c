#include <ncurses.h>
#include <unistd.h>

void mover_janela(WINDOW *win, int *pos_y, int *pos_x, int tecla) {
    int nova_y = *pos_y;
    int nova_x = *pos_x;

    switch(tecla) {
        case KEY_UP:
            if(nova_y > 2) nova_y--;
            break;
        case KEY_DOWN:
            if(nova_y < LINES - 9) nova_y++;
            break;
        case KEY_LEFT:
            if(nova_x > 0) nova_x--;
            break;
        case KEY_RIGHT:
            if(nova_x < COLS - 31) nova_x++;  // COLS - largura da janela
            break;
    }

    // Atualiza posição
    *pos_y = nova_y;
    *pos_x = nova_x;

    mvwin(win, nova_y, nova_x);

    clear();
    attron(A_BOLD);
    mvprintw(0, 2, "Janelas! Use as SETAS para mover");
    attroff(A_BOLD);
    refresh();
    wrefresh(win);
}

int main() {
    WINDOW *win1;
    int win_y = 3, win_x = 2;

    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    init_pair(1, COLOR_WHITE, COLOR_BLUE);

    attron(A_BOLD);
    mvprintw(0, 2, "Janelas! Use as SETAS para mover");
    attroff(A_BOLD);
    refresh();

    win1 = newwin(8, 30, win_y, win_x);
    wbkgd(win1, COLOR_PAIR(1));
    box(win1, 0, 0);
    mvwprintw(win1, 0, 11, "Quase OS");
    wrefresh(win1);

    int ch, contador = 0;
    while((ch = getch()) != 'q') {
        switch(ch) {
            case KEY_UP:
            case KEY_DOWN:
            case KEY_LEFT:
            case KEY_RIGHT:
                mover_janela(win1, &win_y, &win_x, ch);
                break;

            case '1':
                contador++;
                mvwprintw(win1, 5, 2, "Atualizada %d vezes  ", contador);
                wrefresh(win1);
                break;

            case 'c':
                wclear(win1);
                box(win1, 0, 0);
                mvwprintw(win1, 1, 2, "Janela limpa!");
                wrefresh(win1);
                break;
        }
    }

    delwin(win1);
    endwin();
    return 0;
}
