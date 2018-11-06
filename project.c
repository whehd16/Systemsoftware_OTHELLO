#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#define ALT_BACKSPACE 127

#define WIDTH 80
#define HEIGHT 24
#define CURSOR 35
#define BACKSPACE 


int checkButton(int number, int limit);
void winSignIn(WINDOW * win);
void winSignUp(WINDOW * win);
void winMainBeforeLogin();
bool signUp(char * id, char * pw);
int sizeArr(char* arr);
WINDOW* drawBackground();

int checkButton(int number, int limit){
    if(number < 0)
        return limit;
    else if(number > limit)
        return 0;
    else
        return number;
}

WINDOW* drawBackground(){
    int drawPercent = (3*WIDTH*HEIGHT)/4;
    initscr();  //curses 모드 시작
    curs_set(0); // 커서 Off
    start_color();  //색 쓰기 시작
    keypad(stdscr, TRUE);
    WINDOW * win = newwin(24,80,0,0);   //24*80 윈도우 생성

    init_color(1, 500, 500, 400);//번호 RGB 색상
    init_color(2, 0, 0, 400);

    init_pair(1,2,1);
    init_pair(2,1,2);
    for(int i = 0; i < drawPercent; i++)
        waddch(win, ' ' | COLOR_PAIR(1));
     for(int i = drawPercent; i < WIDTH*HEIGHT; i++)
        waddch(win, ' ' | COLOR_PAIR(2));

    return win;
}
    //백그라운드 여기까지!!

void winSignIn(WINDOW * win){ //회원가입
    win = drawBackground();
    curs_set(1);
    int button = 0;
    int cursor;
    int i=0;
    int j=0;
    char c;
    int ascii;
    char id[128] = {};
    char pw[128] = {};
    bool loop = true;

    wattron(win, COLOR_PAIR(1));
    mvwaddstr(win,5,35,"SIGN UP");
    mvwaddstr(win,7,30,"ID : ");
    mvwaddstr(win,9,24,"PASSWORD : ");
    wattroff(win, COLOR_PAIR(1));

    wattron(win, COLOR_PAIR(2));
    mvwaddstr(win,21,15,"SIGN UP");
    mvwaddstr(win,21,60,"BACK");
    wattroff(win, COLOR_PAIR(2));
    refresh();
    wrefresh(win);


    wattron(win, COLOR_PAIR(1));
    noecho();
    cbreak();
    move(7,36);
    wmove(win,7,36);
    cursor = CURSOR;
    while(loop){
        switch( ascii = getch() ) { 
            case 263:
            case 127:
                if(cursor < CURSOR + 1)
                    continue;
                id[--i]='\0';
                mvwaddch(win,7,cursor--,' ');
                wmove(win,7,++cursor);
                cursor--;
                refresh();
                wrefresh(win);
                continue;
            default:
                if((char)ascii == '\n'){
                    loop = false;
                    break;
                }
                id[i++] = (char)ascii;
                cursor++;
                mvwaddch(win,7,cursor, (char)ascii);
                refresh();
                wrefresh(win);
                continue;
        }
    }
    
    move(9,36);
    wmove(win,9,36);
    cursor = CURSOR;
    loop =true;

    while(loop){
        switch( ascii = getch() ) { 
            case 263:
            case 127:
                if(cursor < CURSOR + 1)
                    continue;
                pw[--j]='\0';
                mvwaddch(win,9,cursor--,' ');
                wmove(win,9,++cursor);
                cursor--;
                refresh();
                wrefresh(win);
                continue;
            default:
                if((char)ascii == '\n'){
                    loop = false;
                    break;
                }
                pw[j++] = (char)ascii;
                cursor++;
                mvwaddch(win,9,cursor, (char)ascii);
                refresh();
                wrefresh(win);
                continue;
        }
    }
    wattroff(win,COLOR_PAIR(1));
        
    curs_set(0);

    while(1){
        wattron(win, COLOR_PAIR(2));
        mvwaddstr(win,21,15,"SIGN UP");
        mvwaddstr(win,21,60,"BACK");
        wattroff(win, COLOR_PAIR(2));
        
        wattron(win, COLOR_PAIR(1)|A_UNDERLINE);

        switch(button = checkButton(button, 1)){
            case 0:
                mvwaddstr(win,21,15,"SIGN UP");
                break;
            case 1:
                mvwaddstr(win,21,60,"BACK");
                break;
        }
        refresh();
        wrefresh(win);
        wattroff(win, COLOR_PAIR(1)|A_UNDERLINE);
        switch(getch()){
            case KEY_LEFT :
                button--;
                break;
            case KEY_RIGHT :
                button++;
                break;
            case '\n':
                if(button == 0)
                    return;
                    //signUp(id,pw);
                else
                    winMainBeforeLogin();
        }
        refresh();
        wrefresh(win);
    }
}

int sizeArr(char* arr){
     
    int count=0;
    for(int i = 0; arr[i] != '\0'; i++)
        count++;
    return count;

}

bool signUp(char* id, char* pw){
    int fd1 = open("User.txt", O_RDONLY);
    int fd2 = open("User.txt", O_RDONLY);
    int fd3 = open("User.txt", O_WRONLY|O_APPEND);
    int line = 0;
    bool exist = true;
    char buffer[5];
    long total = 0;
    ssize_t nread=0;
    //먼저 파일을 순회하면서 일치하는 이름이 있는지 확인한다.
    //어떻게?? \n을 구분자로 사용하는데 
    while(read(fd1, buffer, 1))
        nread++;
    char buffer2[nread];
    char delimeter[10] = "'\n'";
    char *pch;
    int count_line = 0;
    read(fd2, buffer2, nread);
    pch = strtok(buffer2,delimeter);
    close(fd1);
    close(fd2);
    
    while(pch)
    {
       count_line++;
       if((count_line%4) == 1 && !strcmp(id, pch))
           return false;
       pch = strtok(NULL,delimeter);
    }
   
    int idSize = sizeArr(id);
    int pwSize = sizeArr(pw);
    write(fd3, id, idSize);
    write(fd3, "\n",1);
    write(fd3, pw, pwSize);
    write(fd3, "\n",1);
    write(fd3, "0", 1);
    write(fd3, "\n",1);
    write(fd3, "0", 1);
    write(fd3, "\n",1);
    close(fd3);
    return true;
}

void winSignUp(WINDOW * win){ //회원가입
    win = drawBackground();
    curs_set(1);
    int button = 0;
    int cursor;
    int i=0;
    int j=0;
    char c;
    int ascii;
    char id[128]={};
    char pw[128]={};
    bool loop = true;

    wattron(win, COLOR_PAIR(1));
    mvwaddstr(win,5,35,"SIGN UP");
    mvwaddstr(win,7,30,"ID : ");
    mvwaddstr(win,9,24,"PASSWORD : ");
    wattroff(win, COLOR_PAIR(1));

    wattron(win, COLOR_PAIR(2));
    mvwaddstr(win,21,15,"SIGN UP");
    mvwaddstr(win,21,60,"BACK");
    wattroff(win, COLOR_PAIR(2));
    refresh();
    wrefresh(win);


    wattron(win, COLOR_PAIR(1));
    noecho();
    cbreak();
    move(7,36);
    wmove(win,7,36);
    cursor = CURSOR;
    while(loop){
        switch( ascii = getch() ) { 
            case 263:
            case 127:
                if(cursor < CURSOR + 1)
                    continue;
                id[--i]='\0';
                mvwaddch(win,7,cursor--,' ');
                wmove(win,7,++cursor);
                cursor--;
                refresh();
                wrefresh(win);
                continue;
            default:
                if((char)ascii == '\n'){
                    loop = false;
                    break;
                }
                id[i++] = (char)ascii;
                cursor++;
                mvwaddch(win,7,cursor, (char)ascii);
                refresh();
                wrefresh(win);
                continue;
        }
    }
    
    move(9,36);
    wmove(win,9,36);
    cursor = CURSOR;
    loop =true;

    while(loop){
        switch( ascii = getch() ) { 
            case 263:
            case 127:
                if(cursor < CURSOR + 1)
                    continue;
                pw[--j]='\0';
                mvwaddch(win,9,cursor--,' ');
                wmove(win,9,++cursor);
                cursor--;
                refresh();
                wrefresh(win);
                continue;
            default:
                if((char)ascii == '\n'){
                    loop = false;
                    break;
                }
                pw[j++] = (char)ascii;
                cursor++;
                mvwaddch(win,9,cursor,'*');
                refresh();
                wrefresh(win);
                continue;
        }
    }
    wattroff(win,COLOR_PAIR(1));
        
    curs_set(0);

    while(1){
        wattron(win, COLOR_PAIR(2));
        mvwaddstr(win,21,15,"SIGN UP");
        mvwaddstr(win,21,60,"BACK");
        wattroff(win, COLOR_PAIR(2));
        
        wattron(win, COLOR_PAIR(1)|A_UNDERLINE);

        switch(button = checkButton(button, 1)){
            case 0:
                mvwaddstr(win,21,15,"SIGN UP");
                break;
            case 1:
                mvwaddstr(win,21,60,"BACK");
                break;
        }
        refresh();
        wrefresh(win);
        wattroff(win, COLOR_PAIR(1)|A_UNDERLINE);
        switch(getch()){
            case KEY_LEFT :
                button--;
                break;
            case KEY_RIGHT :
                button++;
                break;
            case '\n':
                if(button == 0)
                    if(signUp(id,pw)){
                        wattron(win, COLOR_PAIR(2));
                        mvwaddstr(win,23,1,">>> Welcome to OTHELLO World! (Press any key...)");

                        refresh();
                        wrefresh(win);
                        wattroff(win, COLOR_PAIR(2));
                        getch();
                        winMainBeforeLogin();
                    }
                    else{
                        wattron(win, COLOR_PAIR(2));
                        mvwaddstr(win,23,1,">>> ");
                        waddstr(win,id);
                        waddstr(win," has already exist in DB! (Please any key...)");
                        refresh();
                        wrefresh(win);
                        wattroff(win, COLOR_PAIR(2));
                        getch();
                        winSignUp(win);
                    }
                else
                    winMainBeforeLogin();
        }
        refresh();
        wrefresh(win);
    }
}


void winMainBeforeLogin(){
    WINDOW * win = drawBackground();
    int button;
    wattron(win, COLOR_PAIR(1));
    mvwaddstr(win,5,27,"System Softwware Practice");
    mvwaddstr(win,7,35,"OTHELLO");
    mvwaddstr(win,14,68,"2015726056");
    mvwaddstr(win,16,68,"DongHee Cho");
    wattroff(win, COLOR_PAIR(1));
    while(1) {
        wattron(win, COLOR_PAIR(2));
        mvwaddstr(win,21,10,"SIGN IN");
        mvwaddstr(win,21,35,"SIGN UP");
        mvwaddstr(win,21,65,"EXIT");
        wattroff(win, COLOR_PAIR(2));
        
        wattron(win, COLOR_PAIR(1)|A_UNDERLINE);
        switch(button = checkButton(button,2)){
            case 0:
                mvwaddstr(win,21,10,"SIGN IN");
                break;
            case 1:
                mvwaddstr(win,21,35,"SIGN UP");
                break;
            case 2:
                mvwaddstr(win,21,65,"EXIT");
                break;
        }
        wattroff(win, COLOR_PAIR(1)|A_UNDERLINE);
       
        refresh();  //화면에 갱신
        wrefresh(win);  //윈도우 단위로 갱신
        switch(getch()){
            case KEY_LEFT:
                button--;
                break;
            case KEY_RIGHT:
                button++;
                break;
            case '\n':
                //endwin();   //curses 모드를 종료한다.
                werase(win);
                erase();
                if(button == 0)
                    winSignIn(win);
                else if(button == 1)
                    winSignUp(win);
                else
                    return;
        }

    }
}


int main(){
    int fd;
    fd = open("User.txt", O_WRONLY | O_CREAT, 0666);
    close(fd);
    winMainBeforeLogin();
    curs_set(1);
    endwin();   //curses 모드를 종료한다.

    return 0;
}
