#include <stdio.h>
#include <stdlib.h>
#include <conio.h>     // For _kbhit(), _getch()
#include <windows.h>   // For Sleep()

#define MAP_ROWS 13
#define MAP_COLS 20

int main() {
    char map[MAP_ROWS][MAP_COLS + 1] = {
        "####################",
        "#........#........#",
        "#.####...#...####.#",
        "#o#  #...#...#  #o#",
        "#.####...#...####.#",
        "#........ ........#",
        "###.###.#####.###.#",
        "#........#........#",
        "#.####...#...####.#",
        "#o#  #...#...#  #o#",
        "#.####...#...####.#",
        "#........P.......G#",
        "####################"
    };

    int pac_r = 11, pac_c = 9;
    int ghost_r = 11, ghost_c = 17;
    int dr = 0, dc = 0;
    int score = 0;
    int running = 1;

    // Hide cursor (Windows)
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hout, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hout, &cursorInfo);

    while (running) {
        // Input
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'w') { dr = -1; dc = 0; }
            else if (ch == 's') { dr = 1; dc = 0; }
            else if (ch == 'a') { dr = 0; dc = -1; }
            else if (ch == 'd') { dr = 0; dc = 1; }
            else if (ch == 'q') { running = 0; }
        }

        // Move pacman
        int nr = pac_r + dr;
        int nc = pac_c + dc;
        if (map[nr][nc] != '#') {
            pac_r = nr;
            pac_c = nc;
        }

        // Eat pellet
        if (map[pac_r][pac_c] == '.' || map[pac_r][pac_c] == 'o') {
            score++;
            map[pac_r][pac_c] = ' ';
        }

        // Move ghost
        if (map[ghost_r][ghost_c - 1] != '#') ghost_c--;
        else ghost_c = 17;

        // Check collision
        if (pac_r == ghost_r && pac_c == ghost_c)
            running = 0;

        // Move cursor to top-left instead of clearing
        printf("\033[H");

        // Draw map
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

        printf("Score: %d  (WASD to move, Q to quit)\n", score);

        Sleep(100);
    }

    // Show cursor again
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(hout, &cursorInfo);

    printf("\nGame Over! Final Score: %d\n", score);
    return 0;
}
