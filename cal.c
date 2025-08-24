#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <wchar.h>
#include <wctype.h>

#define MAX 500

wchar_t cal_mode[2] = { L'.', L'\0' };
wchar_t cal_temp[20] = {0};
wchar_t cal_total[MAX] = {0};

long double cal_ans = 0.0L;

BOOL is_fst = FALSE;
// BOOL is_err = FALSE;

int now_mode = 0;  // NOTE:-  1>[+] , 2>[-] , 3>[*] , 4>[/] , 5>[%]
int temp_len = 0;
int total_len = 0;



void enable_ansi() {

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

void dis_print() {

    printf("\n\n\033[90mpress E to Exit | C to Clear\n");
    wprintf(L"\n\033[33m %ls \033[90m| \033[97m%ls\n\033[90m---+------------------\n\033[90m = | \033[33m%Lf\n\033[90m---+------------------\n\033[31m > \033[90m|\033[31m %ls\033[0m\n\n", cal_mode, cal_total, cal_ans, cal_temp);
    fflush(stdout);
}

void dis_update() {

    printf("\033[10A");
    printf("\033[0J");
    dis_print();
}

void now_cal(int m) {

    long double value = wcstold(cal_temp, NULL);

    if (m == 0) {

        cal_ans = value;
        is_fst = TRUE;

    } else if (m == 1) {

        if (is_fst) {
            cal_ans += value;
            wcscat(cal_total, L"\033[90m + \033[97m");
        } else {
            cal_ans = value;
            is_fst = TRUE;
        }

    } else if (m == 2) {

        if (is_fst) {
            cal_ans -= value;
            wcscat(cal_total, L"\033[90m - \033[97m");
        } else {
            cal_ans = value;
            is_fst = TRUE;
        }

    } else if (m == 3) {

        if (is_fst) {
            cal_ans *= value;
            wcscat(cal_total, L"\033[90m * \033[97m");
        } else {
            cal_ans = value;
            is_fst = TRUE;
        }

    } else if (m == 4) {

        if (is_fst) {
            cal_ans /= value;
            wcscat(cal_total, L"\033[90m / \033[97m");
        } else {
            cal_ans = value;
            is_fst = TRUE;
        }
    } else if (m == 5) {

        if (is_fst) {
            cal_ans = (cal_ans / 100.0L) * value;
            wcscat(cal_total, L"\033[90m \% \033[97m");
        } else {
            cal_ans = value;
            is_fst = TRUE;
        }
    }

    int now_len = temp_len + 15;
    total_len += now_len;

    if (total_len > MAX) {

        cal_total[0] = '\0';
        wcscat(cal_total, L"\033[90m.. \033[97m");
        wcscat(cal_total, cal_temp);

        total_len = now_len;

    } else wcscat(cal_total, cal_temp);

    cal_temp[0] = L'\0';
    temp_len = 0;
}



int main() {

    wchar_t i;
    
    enable_ansi();
    dis_print();

    while ((i = _getch()) != L'e') {

        switch (i) {

            case L'+':
                if (temp_len > 0) {
                    now_mode = 1;
                    cal_mode[0] = L'+';
                    now_cal(now_mode);
                    dis_update();
                }
            break;

            case L'-':
                if (temp_len > 0) {
                    now_mode = 2;
                    cal_mode[0] = L'-';
                    now_cal(now_mode);
                    dis_update();
                }
            break;

            case L'*':
                if (temp_len > 0) {
                    now_mode = 3;
                    cal_mode[0] = L'x';
                    now_cal(now_mode);
                    dis_update();
                }
            break;

            case L'/':
                if (temp_len > 0) {
                    now_mode = 4;
                    cal_mode[0] = L'/';
                    now_cal(now_mode);
                    dis_update();
                }
            break;

            case L'%':
                if (temp_len > 0) {
                    now_mode = 5;
                    cal_mode[0] = L'%';
                    now_cal(now_mode);
                    dis_update();
                }
            break;

            case L'c':

                cal_total[0] = L'\0';
                cal_temp[0] = L'\0';
                cal_mode[0] = L'.';
                
                is_fst = FALSE;

                cal_ans = 0.0L;

                now_mode = 0;
                temp_len = 0;
                total_len = 0;

                dis_update();


            break;

            case L'\r':
                if (temp_len > 0) {

                    now_cal(now_mode);
                    dis_update();
                }
            break;

            case L'\b':
                if (temp_len > 0) {

                    cal_temp[temp_len-1] = L'\0';
                    temp_len--;
                    dis_update();
                }
            break;

            case L'.':
                if (temp_len < 18) {

                    wchar_t fix[2] = { i, L'\0' };
                    wcscat(cal_temp, fix);
                    temp_len++;
                    dis_update();
                }
            break;

            default:
                if (iswdigit(i) && temp_len < 18) {

                    wchar_t fix[2] = { i, L'\0' };
                    wcscat(cal_temp, fix);
                    temp_len++;
                    dis_update();
                }
            break;
        }
    }
    return 0;
}
