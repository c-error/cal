#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <wchar.h>
#include <wctype.h>

#define MAX_TEMP 1024

void enable_ansi() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

int get_cursor_position() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return csbi.dwCursorPosition.Y; // current Y position
    }
    return 0;
}

void print_data(const wchar_t *data_all, long double data_ans, const wchar_t *data_now) {

    wprintf(L"\n\033[97m> %-14ls\n\033[33m> %-14.3Lf\n\033[31m> %-14ls\033[0m\n\n",
        data_all,
        data_ans,
        data_now);
    fflush(stdout);
}

void set_cursor_position(int p, const wchar_t *data_all, long double data_ans, const wchar_t *data_now) {
    COORD loc = {0, (SHORT)p};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), loc);
    print_data(data_all, data_ans, data_now);
}

int main() {

    wchar_t temp[MAX_TEMP] = {0};
    wchar_t total[MAX_TEMP] = {0};
    long double ANS = 0.0L;

    wchar_t KEY;
    size_t LEN = 0;

    size_t CARRY = 0;

    int MODE = 0;

    enable_ansi();
    print_data(L".", ANS, L".");

    while ((KEY = _getch()) != L'q') {
        int POS = get_cursor_position() - 5;

        switch (KEY) {

            case L'+':
                if (LEN > 0) {

                    size_t value = (size_t)wcstoull(temp, NULL, 10);

                    ANS = ANS + value;

                    if (MODE) wcscat(total, L" + ");
                    else MODE = 1;

                    wcscat(total, temp);
                    temp[0] = L'\0';
                    set_cursor_position(POS, total, ANS, temp);
                    LEN = 0;
                }
            break;

            case L'-':
                if (LEN > 0) {

                    size_t value = (size_t)wcstoull(temp, NULL, 10);

                    ANS = ANS - value;

                    if (MODE) wcscat(total, L" - ");
                    else MODE = 1;

                    wcscat(total, temp);
                    temp[0] = L'\0';
                    set_cursor_position(POS, total, ANS, temp);
                    LEN = 0;
                }
            break;

            case L'*':
                if (LEN > 0) {

                    size_t value = (size_t)wcstoull(temp, NULL, 10);

                    if (ANS == 0) ANS = value;
                    else ANS = ANS * value;

                    if (MODE) wcscat(total, L" * ");
                    else MODE = 1;

                    wcscat(total, temp);
                    temp[0] = L'\0';
                    set_cursor_position(POS, total, ANS, temp);
                    LEN = 0;
                }
            break;

            case L'/':
                if (LEN > 0) {

                    size_t value = (size_t)wcstoull(temp, NULL, 10);

                    if (ANS == 0) ANS = value;
                    else ANS = ANS / value;

                    if (MODE) wcscat(total, L" / ");
                    else MODE = 1;

                    wcscat(total, temp);
                    temp[0] = L'\0';
                    set_cursor_position(POS, total, ANS, temp);
                    LEN = 0;
                }
            break;

            case L'c':

                total[0] = L'\0';
                ANS = 0.0L;
                temp[0] = L'\0';
                LEN = 0;
                MODE = 0;

                printf("\033[5A");
                printf("\033[0J");
                fflush(stdout);

                set_cursor_position(POS, total, ANS, temp);

            break;

            case L'\b':
                if (LEN > 0) {

                    temp[LEN-1] = L'\0';
                    LEN--;
                    
                    set_cursor_position(POS, total, ANS, temp);
                }
            break;

            default:
                if (iswdigit(KEY) && LEN < 12) {
                    wchar_t fix[2] = { KEY, L'\0' };
                    wcscat(temp, fix);
                    LEN++;

                    set_cursor_position(POS, total, ANS, temp);
                }
            break;
        }
    }
    return 0;
}
