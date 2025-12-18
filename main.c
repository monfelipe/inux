#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

void mover_janela(WINDOW *win, WINDOW *win2,
                  int *pos_y, int *pos_x, int tecla) {
    int nova_y = *pos_y;
    int nova_x = *pos_x;

    switch (tecla) {
        case KEY_UP:
            if (win2 == NULL){
                if (nova_y > 2) nova_y--;
            }
            break;
        case KEY_DOWN:
            if (win2 == NULL){
                if (nova_y < LINES - 9) nova_y++;
            }
            break;
        case KEY_LEFT:
            if (win2 == NULL){
                if (nova_x > 0) nova_x--;
            }
            break;
        case KEY_RIGHT:
            if (win2 == NULL){
                if (nova_x < COLS - 31) nova_x++;
            }
            break;
    }

    *pos_y = nova_y;
    *pos_x = nova_x;

    mvwin(win, nova_y, nova_x);
    clear();

    attron(A_BOLD);
    mvprintw(0, 2, "Inux");
    attroff(A_BOLD);
    refresh();

    if (win2 != NULL) {
        touchwin(win2);
        wrefresh(win2);
    }

    touchwin(win);
    wrefresh(win);
}

int main() {
    int porc = 2;
    FILE *arquivo;
    char linha[256];

    arquivo = fopen("aplications.inux", "r");

    WINDOW *win1;
    WINDOW *win2 = NULL;
    int win_y = 3, win_x = 2;
    int ch, contador = 0;

    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    init_pair(1, COLOR_WHITE, COLOR_BLUE);

    attron(A_BOLD);
    mvprintw(0, 2, "Inux");
    attroff(A_BOLD);
    refresh();

    win1 = newwin(8, 30, win_y, win_x);
    wbkgd(win1, COLOR_PAIR(1));
    box(win1, 0, 0);
    mvwprintw(win1, 0, 11, "Inux");
    wrefresh(win1);

    while ((ch = getch()) != 'q') {
        switch (ch) {

            case KEY_UP:
            case KEY_DOWN:
            case KEY_LEFT:
            case KEY_RIGHT:
                mover_janela(win1, win2, &win_y, &win_x, ch);
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

                case 'n':
                    if (win2 == NULL) {
                        porc = 1;          // reinicia linha
                        rewind(arquivo);   // volta ao início do arquivo

                        win2 = newwin(8, 30, 30, 50);
                        wbkgd(win2, COLOR_PAIR(1));
                        box(win2, 0, 0);

                        mvwprintw(win2, 0, 4, " Bem-vindo ao Inux ");

                        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
                            if (porc >= 7) break;
                            mvwprintw(win2, porc, 1, "%s", linha);
                            porc++;
                        }

                        wrefresh(win2);
                    } else {
                        werase(win2);
                        wrefresh(win2);
                        delwin(win2);
                        win2 = NULL;

                        clear();
                        attron(A_BOLD);
                        mvprintw(0, 2, "Inux");
                        attroff(A_BOLD);
                        refresh();
                        touchwin(win1);
                        wrefresh(win1);
                    }
                    break;

        }
    }

    if (win2 != NULL)
        delwin(win2);

    fclose(arquivo);
    delwin(win1);
    endwin();
    return 0;
}
