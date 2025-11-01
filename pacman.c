#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <math.h>

#define MAP_ROWS 30
#define MAP_COLS 55

int main() {
    char map[MAP_ROWS][MAP_COLS + 1] = {
        "#######################################################",
        "#.........................#.........................#",
        "#.#####.#########.#######.#.#######.#########.#####.#",
        "#o#   #.#       #.#     #.#.#     #.#       #.#   #o#",
        "#.###.#.#.###.#.#.#.###.#.#.#.###.#.#.###.#.#.#.###.#",
        "#.....#.#.....#.#.#.....#.#.#.....#.#.....#.#.#.....#",
        "###.#.#.#######.#.#######.#.#######.#.#######.#.###.#",
        "#...#.#........#.........#.#.........#........#.#...#",
        "#.###.#########.#########.#.#########.#########.###.#",
        "#.#...............................................#.#",
        "#.#.#########.#########################.#########.#.#",
        "#.#.#...........................................#.#.#",
        "#.#.#.#########.###########.###########.#########.#.#",
        "#.#.#.#.......#.#.........#.#.........#.#.......#.#.#",
        "#.#.#.#.###.#.#.#.#######.#.#.#######.#.#.###.#.#.#.#",
        "#.#.#.#.....#.#.#.......#.#.#.......#.#.#.....#.#.#.#",
        "#.#.#.#####.#.#.#######.#.#.#.#######.#.#####.#.#.#.#",
        "#.#.#.......#.#.........#.#.#.........#.#.......#.#.#",
        "#.#.#########.###########.#.###########.#########.#.#",
        "#.#...............................................#.#",
        "#.#################################################.#",
        "#...................................................#",
        "#.#################################################.#",
        "#.#...............................................#.#",
        "#.#.###########.###############.###############.#.#.#",
        "#.#...........#...............#.................#.#.#",
        "#.###########.#.#############.#.###############.#.#.#",
        "#.....................P.........................G...#",
        "#######################################################"
    };

    int pac_r = 27, pac_c = 22;
    int ghost_r = 27, ghost_c = 47;
    int dr = 0, dc = 0;
    int gdr = 0, gdc = -1;
    int score = 0;
    int running = 1;
    int chase_timer = 0; 

    srand((unsigned)time(NULL));

    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hout, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hout, &cursorInfo);

    while (running) {

        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'w') { dr = -1; dc = 0; }
            else if (ch == 's') { dr = 1; dc = 0; }
            else if (ch == 'a') { dr = 0; dc = -1; }
            else if (ch == 'd') { dr = 0; dc = 1; }
            else if (ch == 'q') { running = 0; }
        }

        int nr = pac_r + dr;
        int nc = pac_c + dc;
        if (map[nr][nc] != '#') {
            pac_r = nr;
            pac_c = nc;
        }

        if (map[pac_r][pac_c] == '.' || map[pac_r][pac_c] == 'o') {
            score++;
            map[pac_r][pac_c] = ' ';
        }

        double dist = sqrt((pac_r - ghost_r)*(pac_r - ghost_r) + (pac_c - ghost_c)*(pac_c - ghost_c));

        if (dist < 4) {
            chase_timer = 200; 
        }

        int ngr = ghost_r + gdr;
        int ngc = ghost_c + gdc;
        int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};

        if (chase_timer > 0) {

            chase_timer--;
            int best_r = ghost_r, best_c = ghost_c;
            double best_dist = 1e9;
            int best_dr = 0, best_dc = 0;

            for (int i = 0; i < 4; i++) {
                int tr = ghost_r + dirs[i][0];
                int tc = ghost_c + dirs[i][1];
                if (map[tr][tc] != '#') {
                    double d = sqrt((pac_r - tr)*(pac_r - tr) + (pac_c - tc)*(pac_c - tc));
                    if (d < best_dist) {
                        best_dist = d;
                        best_r = tr;
                        best_c = tc;
                        best_dr = dirs[i][0];
                        best_dc = dirs[i][1];
                    }
                }
            }
            gdr = best_dr;
            gdc = best_dc;
            ngr = best_r;
            ngc = best_c;
        } else {

            int blocked = (map[ngr][ngc] == '#');
            int openCount = 0;
            for (int i = 0; i < 4; i++) {
                int tr = ghost_r + dirs[i][0];
                int tc = ghost_c + dirs[i][1];
                if (map[tr][tc] != '#') openCount++;
            }

            if (blocked || openCount > 2) {
                int validDirFound = 0;
                for (int tries = 0; tries < 10 && !validDirFound; tries++) {
                    int d = rand() % 4;
                    int tr = ghost_r + dirs[d][0];
                    int tc = ghost_c + dirs[d][1];
                    if (map[tr][tc] != '#' && !(dirs[d][0] == -gdr && dirs[d][1] == -gdc)) {
                        gdr = dirs[d][0];
                        gdc = dirs[d][1];
                        validDirFound = 1;
                    }
                }
                ngr = ghost_r + gdr;
                ngc = ghost_c + gdc;
            }
        }

        if (map[ngr][ngc] != '#') {
            ghost_r = ngr;
            ghost_c = ngc;
        }

        if (pac_r == ghost_r && pac_c == ghost_c)
            running = 0;

        printf("\033[H");
        for (int r = 0; r < MAP_ROWS; r++) {
            for (int c = 0; c < MAP_COLS; c++) {
                if (r == pac_r && c == pac_c)
                    putchar('C');
                else if (r == ghost_r && c == ghost_c)
                    putchar('G');
                else
                    putchar(map[r][c]);
            }
            putchar('\n');
        }

        if (chase_timer > 0)
            printf("Score: %d  [GHOST CHASING! %d]\n", score, chase_timer / 50);
        else
            printf("Score: %d  (WASD to move, Q to quit)\n", score);

        Sleep(20);
    }

    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(hout, &cursorInfo);

    printf("\nGame Over! Final Score: %d\n", score);
    return 0;
}
