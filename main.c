#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void move_win(WINDOW *win, WINDOW *win2,
              int *pos_y, int *pos_x, int tecla) {
    int nova_y = *pos_y;
    int nova_x = *pos_x;

    switch (tecla) {
        case KEY_UP:
            if (win2 == NULL && nova_y > 2)
                nova_y--;
            break;

        case KEY_DOWN:
            if (win2 == NULL && nova_y < LINES - 9)
                nova_y++;
            break;

        case KEY_LEFT:
            if (win2 == NULL && nova_x > 0)
                nova_x--;
            break;

        case KEY_RIGHT:
            if (win2 == NULL && nova_x < COLS - 31)
                nova_x++;
            break;
    }

    *pos_y = nova_y;
    *pos_x = nova_x;

    mvwin(win, nova_y, nova_x);

    erase();  // melhor que clear()

    attron(A_BOLD);
    mvprintw(0, 2, "Inux System");
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
    int pount;
    FILE *arquivo;
    char linha[256];

    arquivo = fopen("aplications.inux", "r");
    if (!arquivo) {
        perror("Erro ao abrir aplications.inux");
        return 1;
    }

    WINDOW *win1;
    WINDOW *win2 = NULL;

    int win_y = 3, win_x = 2;
    int ch;

    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    init_pair(1, COLOR_WHITE, COLOR_BLUE);

    attron(A_BOLD);
    mvprintw(0, 2, "Inux System");
    attroff(A_BOLD);
    refresh();

    win1 = newwin(8, 30, win_y, win_x);
    wbkgd(win1, COLOR_PAIR(1));
    box(win1, 0, 0);
    mvwprintw(win1, 0, 13, "Inux");
    wrefresh(win1);

    while ((ch = getch()) != 'q') {
        switch (ch) {

            case KEY_UP:
            case KEY_DOWN:
            case KEY_LEFT:
            case KEY_RIGHT:
                move_win(win1, win2, &win_y, &win_x, ch);
                break;

            case 'c':
                werase(win1);
                box(win1, 0, 0);
                mvwprintw(win1, 1, 2, "Isso nao existe mais...");
                wrefresh(win1);
                break;

            case 'n':
                if (win2 == NULL) {
                    rewind(arquivo);
                    pount = 1;

                    win2 = newwin(8, 30, 30, 50);
                    wbkgd(win2, COLOR_PAIR(1));
                    box(win2, 0, 0);
                    mvwprintw(win2, 0, 4, " Bem-vindo ao Inux ");

                    while (fgets(linha, sizeof(linha), arquivo)) {
                        if (pount > 6)
                            break;

                        linha[strcspn(linha, "\n")] = '\0';
                        mvwprintw(win2, pount, 1, "%s", linha);
                        pount++;
                    }

                    wrefresh(win2);

                } else {
                    werase(win2);
                    wrefresh(win2);
                    delwin(win2);
                    win2 = NULL;

                    erase();
                    attron(A_BOLD);
                    mvprintw(0, 2, "Inux System");
                    attroff(A_BOLD);
                    refresh();

                    touchwin(win1);
                    wrefresh(win1);
                }
                break;
        }
    }

    if (win2)
        delwin(win2);

    delwin(win1);
    fclose(arquivo);
    endwin();
    return 0;
}
