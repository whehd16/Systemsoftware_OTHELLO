#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#define ALT_BACKSPACE 127

#define WIDTH 80
#define HEIGHT 24
#define CURSOR 35
#define BACKSPACE 

void exitMode(char* id, WINDOW* win);
void winPlay(char* id, WINDOW* win);
void play(char* id, WINDOW* win);
void signOut();
void winWithdrawal(char * id, WINDOW* win);
bool withdrawal(char* id, char* pw);


int checkButton(int number, int limit);
void winSignIn(WINDOW* win);
void winSignUp(WINDOW* win);
void winMainAfterLogin(char* id, WINDOW* win);
void winMainBeforeLogin();
bool signUp(char* id, char* pw);
bool signIn(char* id, char* pw);
int sizeArr(char* arr);

WINDOW* drawBackground();
WINDOW* drawPlayground(WINDOW* win);

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

WINDOW* drawPlayground(WINDOW* win){
    int drawPercent = (3*WIDTH*HEIGHT)/4;
    int line = 5;
    wclear(win);
    curs_set(0);
    for(int i = 0; i < WIDTH*HEIGHT; i++){
        if((i%80)<60)
            waddch(win, ' ' | COLOR_PAIR(1));
        else
            waddch(win, ' ' | COLOR_PAIR(2));
    }

    wattron(win,COLOR_PAIR(1));
    mvwaddstr(win,5,15,  "+---+---+---+---+---+---+");
    mvwaddstr(win,6,15,  "|   |   |   |   |   |   |");
    mvwaddstr(win,7,15,  "+---+---+---+---+---+---+");
    mvwaddstr(win,8,15,  "|   |   |   |   |   |   |");
    mvwaddstr(win,9,15,  "+---+---+---+---+---+---+");
    mvwaddstr(win,10,15, "|   |   |   |   |   |   |");
    mvwaddstr(win,11,15, "+---+---+---+---+---+---+");
    mvwaddstr(win,12,15, "|   |   |   |   |   |   |");
    mvwaddstr(win,13,15, "+---+---+---+---+---+---+");
    mvwaddstr(win,14,15, "|   |   |   |   |   |   |");
    mvwaddstr(win,15,15, "+---+---+---+---+---+---+");
    mvwaddstr(win,16,15, "|   |   |   |   |   |   |");
    mvwaddstr(win,17,15, "+---+---+---+---+---+---+");
    wattroff(win,COLOR_PAIR(2));
    return win;
}
 
bool signIn(char* id, char* pw){
    int fd1 = open("2015726056.txt", O_RDONLY);
    int fd2 = open("2015726056.txt", O_RDONLY);
    int nread = 0;
    char buffer[5];
    while(read(fd1,buffer,1))
        nread++;
    close(fd1);
    char buffer2[nread];
    char delimeter[10] = "'\n'";
    char *pch;
    int count_line = 0;
    read(fd2, buffer2, nread);
    close(fd2);
    pch = strtok(buffer2, delimeter);
    while(pch){
        count_line ++;
        if((count_line%4) == 1 && !strcmp(id, pch)){
            pch = strtok(NULL,delimeter);
            count_line++;
            if(!strcmp(pw, pch))
                return true;
            else
                return false;
        }
        pch = strtok(NULL,delimeter);
    }
    return false;
}

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
    mvwaddstr(win,5,35,"SIGN IN");
    mvwaddstr(win,7,30,"ID : ");
    mvwaddstr(win,9,24,"PASSWORD : ");
    wattroff(win, COLOR_PAIR(1));

    wattron(win, COLOR_PAIR(2));
    mvwaddstr(win,21,15,"SIGN IN");
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
        switch( ascii =getch() ) { 
            case 263:
                if(cursor < CURSOR + 1)
                    continue;
                id[--i]='\0';
                mvwaddch(win,7,cursor,' ');
                wmove(win,7,cursor--);
                refresh();
                wrefresh(win);
                continue;
            case KEY_LEFT:
            case KEY_RIGHT:
            case KEY_DOWN:
            case KEY_UP:
                continue;
            default:
                if((char)ascii == '\n'){
                    loop = false;
                    break;
                }
                id[i++] = (char)ascii;
                mvwaddch(win,7,++cursor, (char)ascii);
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
                if(cursor < CURSOR + 1)
                    continue;
                pw[--j]='\0';
                mvwaddch(win,9,cursor,' ');
                wmove(win,9,cursor--);
                refresh();
                wrefresh(win);
                continue;
            case KEY_LEFT:
            case KEY_RIGHT:
            case KEY_DOWN:
            case KEY_UP:
                continue;
            default:
                if((char)ascii == '\n'){
                    loop = false;
                    break;
                }
                pw[j++] = (char)ascii;
                mvwaddch(win,9,++cursor, '*');
                refresh();
                wrefresh(win);
                continue;
        }
    }
    wattroff(win,COLOR_PAIR(1));
        
    curs_set(0);

    while(1){
        wattron(win, COLOR_PAIR(2));
        mvwaddstr(win,21,15,"SIGN IN");
        mvwaddstr(win,21,60,"BACK");
        wattroff(win, COLOR_PAIR(2));
        
        wattron(win, COLOR_PAIR(1)|A_UNDERLINE);

        switch(button = checkButton(button, 1)){
            case 0:
                mvwaddstr(win,21,15,"SIGN IN");
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
                    if(signIn(id,pw))
                        winMainAfterLogin(id,win);
                    else{
                        wattron(win,COLOR_PAIR(2));
                        mvwaddstr(win,23,1,">>> ID doesn't exist, or wrong Password. Please try again(press any key...)");
                        refresh();
                        wrefresh(win);
                        wattroff(win,COLOR_PAIR(2));
                        getch();
                        winSignIn(win);
                    }
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
    int fd1 = open("2015726056.txt", O_RDONLY);
    int fd2 = open("2015726056.txt", O_RDONLY);
    int fd3 = open("2015726056.txt", O_WRONLY|O_APPEND);
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
    srand((int)time(NULL));
    int w = (rand()%100);
    int l = (rand()%100);
    char s1[128];
    char s2[128];
    sprintf(s1, "%d",w);
    sprintf(s2, "%d",l);
    write(fd3, id, idSize);
    write(fd3, "\n",1);
    write(fd3, pw, pwSize);
    write(fd3, "\n",1);
    write(fd3, s1, 1);
    write(fd3, "\n",1);
    write(fd3, s2, 1);
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
                if(cursor < CURSOR + 1)
                    continue;
                id[--i]='\0';
                mvwaddch(win,7,cursor,' ');
                wmove(win,7,cursor--);
                refresh();
                wrefresh(win);
                continue;
            case KEY_LEFT:
            case KEY_RIGHT:
            case KEY_DOWN:
            case KEY_UP:
                continue;
            default:
                if((char)ascii == '\n'){
                    if(sizeArr(id)==0)
                        break;
                    loop = false;
                    break;
                }
                id[i++] = (char)ascii;
                mvwaddch(win,7,++cursor, (char)ascii);
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
                if(cursor < CURSOR + 1)
                    continue;
                pw[--j]='\0';
                mvwaddch(win,9,cursor,' ');
                wmove(win,9,cursor--);
                refresh();
                wrefresh(win);
                continue;
            case KEY_LEFT:
            case KEY_RIGHT:
            case KEY_DOWN:
            case KEY_UP:
                continue;
            default:
                if((char)ascii == '\n'){
                    if(sizeArr(pw) == 0)
                        break;
                    loop = false;
                    break;
                }
                pw[j++] = (char)ascii;
                mvwaddch(win,9,++cursor,'*');
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

void exitMode(char* id, WINDOW* win){
    while(1){
        switch(getch()){
            case '\n':
                winMainAfterLogin(id, win);
            case 'g' :
                winPlay(id,win);
        }
    }
}

void winPlay(char* id, WINDOW* win){
    win = drawPlayground(win);
    refresh();
    wrefresh(win);
    int xpos = 2;
    int ypos = 2;
    char stone[6][6] = {
        {' ',' ',' ',' ',' ',' '},
        {' ',' ',' ',' ',' ',' '},
        {' ',' ','O','X',' ',' '},
        {' ',' ','X','O',' ',' '},
        {' ',' ',' ',' ',' ',' '},
        {' ',' ',' ',' ',' ',' '},
    };
    wattron(win,COLOR_PAIR(1));
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 6; j++){
            switch(stone[i][j]){
                case 'O':
                case 'X':
                    mvwaddch(win,5+(2*j+1),15+(4*i+2),stone[i][j]);
                    break;
            }
        }
    }
    wattroff(win,COLOR_PAIR(1));
    wattron(win, COLOR_PAIR(2));
    mvwaddch(win,18,65,'E');
    wattroff(win, COLOR_PAIR(2));
    wattron(win, COLOR_PAIR(2)|A_UNDERLINE);
    waddch(win,'X');
    wattroff(win, COLOR_PAIR(2)|A_UNDERLINE);
    wattron(win, COLOR_PAIR(2));
    waddstr(win,"IT");
    wattroff(win, COLOR_PAIR(2));  
    move(10,25);
    wmove(win,10,25);
    refresh();
    wrefresh(win);
 
    noecho();
    while(1){
        wattron(win,COLOR_PAIR(2));
        mvwaddch(win, 5+(2*ypos+1), 15+(4*xpos+2), stone[xpos][ypos]);
        refresh();
        wrefresh(win);
        wattron(win,COLOR_PAIR(1));
        switch(getch()){
            case KEY_LEFT:
                mvwaddch(win,5+(2*ypos+1), 15+(4*xpos+2), stone[xpos][ypos]);
                if( --xpos < 0)
                    xpos = 5;
                break;
            case KEY_RIGHT:
                mvwaddch(win,5+(2*ypos+1), 15+(4*xpos+2), stone[xpos][ypos]);
                if( ++xpos > 5)
                    xpos = 0;
                break;
            case KEY_UP:
                mvwaddch(win,5+(2*ypos+1), 15+(4*xpos+2), stone[xpos][ypos]);
                if( --ypos < 0 )
                    ypos = 5;
                break;
            case KEY_DOWN:
                mvwaddch(win,5+(2*ypos+1), 15+(4*xpos+2), stone[xpos][ypos]);
                if( ++ypos > 5)
                    ypos = 0;
                break;
            case 'x' :
                wattron(win,COLOR_PAIR(1));
                mvwaddch(win,18,65,'E');
                wattroff(win,COLOR_PAIR(1));
                wattron(win,COLOR_PAIR(1)|A_UNDERLINE);
                waddch(win,'X');
                wattroff(win,COLOR_PAIR(1)|A_UNDERLINE);
                wattron(win,COLOR_PAIR(1));
                waddstr(win,"IT");
                mvwaddch(win,5+(2*ypos+1), 15+(4*xpos+2), stone[xpos][ypos]);
                wattroff(win,COLOR_PAIR(1));
                refresh();
                wrefresh(win);
                exitMode(id, win);
        }
        wattroff(win,COLOR_PAIR(2));


    }
            
}


int parse(char* id, char* what){
    int fd = open("2015726056.txt",O_RDONLY);
    int buffer[5] = {};
    int nread = 0;
    char delimeter[10] = "'\n'";
    char *pch;
    
    nread = lseek(fd,0,SEEK_END);
    close(fd);

    char buffer2[nread];
    
  
    fd = open("2015726056.txt",O_RDONLY);
    read(fd,buffer2,nread);
    close(fd);
    pch = strtok(buffer2, delimeter);
    while(pch){
        if(!strcmp(pch,id)){
            pch = strtok(NULL, delimeter);
            pch = strtok(NULL, delimeter);
            if(!strcmp(what,"lose"))
                pch=strtok(NULL, delimeter);
            return atoi(pch);
        }
        pch = strtok(NULL, delimeter);
    }
}

int checkWin(char* id){
    return parse(id, "win");
}
int checkLose(char* id){
    return parse(id, "lose");
}

void play(char* id, WINDOW* win){
    
    int w = checkWin(id);
    int l = checkLose(id);

    win = drawBackground();
    wattron(win,COLOR_PAIR(1));
    mvwaddstr(win,5,31,"PLAYER ID : ");
    waddstr(win,id);
    mvwaddstr(win,7,35,"STATISTICS");
    mvwaddstr(win,9,29,"WIN : ");
    wprintw(win,"%d",w);
    waddstr(win," / LOSE : " );
    wprintw(win,"%d",l);
    
    double rate;
    if(w == 0 && l == 0)
       rate = 0;
    else
        rate = (double)w/(w+l);
    waddstr(win," (");
    wprintw(win,"%.3f", rate*100);
    waddstr(win,"%)");
    curs_set(1);
    mvwaddstr(win,21,39,"OK");
    wattroff(win,COLOR_PAIR(1));
    refresh();
    wrefresh(win);
    while(getch() != '\n'){}
    winPlay(id, win);
          
}

void signOut(){
    winMainBeforeLogin();
}

bool withdrawal(char* id, char* pw){
    int fd1 = open("2015726056.txt", O_RDONLY);
    int nread = 0;
    int nread2 = 0;
    int nread3 = 0;
    int count_line = 0;
    int line = 1;
    int target = 0;
    char buffer[5] = {};
    char delimeter[10] = "'\n'";
    char *pch;
   
    nread = lseek(fd1,0,SEEK_END);
    close(fd1);
    char buffer2[nread];

    fd1 = open("2015726056.txt", O_RDONLY);
    read(fd1, buffer2, nread);
    close(fd1);

    pch = strtok(buffer2, delimeter);
    while(pch){
        count_line++;
        if( (count_line%4) == 1 && !strcmp(pch,id) ){
            pch = strtok(NULL, delimeter);
            count_line++;
            if(!strcmp(pch,pw))
                target = count_line-1;
        }
        pch = strtok(NULL, delimeter);
    }
    
    if(target){
        fd1 = open("2015726056.txt", O_RDONLY);
        while(read(fd1,buffer,1)){
            if(line < target)
                nread2++;
            if(line > target + 3)
                nread3++;
            if(!strcmp(buffer,"\n"))
                line++;
        }
        close(fd1);
        char front[nread2];
        char back[nread3];
        int temp = nread-nread2-nread3;
        char mid[temp];
        fd1 = open("2015726056.txt", O_RDONLY);
        read(fd1,front,nread2);
        read(fd1,mid,temp);
        read(fd1,back,nread3);
        close(fd1);

        int fd2 = open("2015726056.txt", O_WRONLY|O_TRUNC);

        write(fd2,front,nread2);
        write(fd2,back,nread3);
        close(fd2);
        return true;
    }
    else
        return false;
}
void winWithdrawal(char * id, WINDOW* win){
    win = drawBackground();
    curs_set(1);
    bool loop = true;
    int button = 0;
    int ascii;
    int cursor = CURSOR + 1;
    int j = 0;
    char pw[128] = {};

    wattron(win,COLOR_PAIR(1));
    mvwaddstr(win,5,35,"WITHDRAWAL");
    mvwaddstr(win,7,25,"PLAYER ID : ");
    waddstr(win,id);
    mvwaddstr(win,9,26,"PASSWORD : ");
    wattroff(win,COLOR_PAIR(1));

    wattron(win,COLOR_PAIR(2));
    mvwaddstr(win,21,15,"WITHDRAWAL");
    mvwaddstr(win,21,60,"BACK");
    wattroff(win,COLOR_PAIR(2));
    
    refresh();
    wrefresh(win);

    wattron(win, COLOR_PAIR(1));
    noecho();
    cbreak();
    move(9,CURSOR+2);
    wmove(win,9,CURSOR+2);
    while(loop){
        switch( ascii = getch() ){
            case 263:
                if(cursor <= CURSOR+1)
                    continue;
                pw[--j] = '\0';
                mvwaddch(win,9,cursor,' ');
                wmove(win,9,cursor--);
                refresh();
                wrefresh(win);
                break;
            case KEY_UP:
            case KEY_DOWN:
            case KEY_LEFT:
            case KEY_RIGHT:
                break;
            default:
                if((char)ascii == '\n'){
                    loop = false;
                    break;
                }
                pw[j++]=(char)ascii;
                mvwaddch(win,9,++cursor,(char)ascii);
                refresh();
                wrefresh(win);
                break;
        }
    }
    wattroff(win, COLOR_PAIR(1));
    curs_set(0);

    while(1){
        wattron(win, COLOR_PAIR(2));
        mvwaddstr(win,21,15,"WITHDRAWAL");
        mvwaddstr(win,21,60,"BACK");
        wattroff(win, COLOR_PAIR(2));
        
        wattron(win, COLOR_PAIR(1)|A_UNDERLINE);

        switch(button = checkButton(button, 1)){
            case 0:
                mvwaddstr(win,21,15,"WITHDRAWAL");
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
                if(button == 0){
                    if(withdrawal(id,pw)){
                        wattron(win, COLOR_PAIR(2));
                        mvwaddstr(win,23,1,">>> Success withdrawal (press any key...)");
                        refresh();
                        wrefresh(win);
                        wattroff(win, COLOR_PAIR(2));
                        getch();
                        winMainBeforeLogin();
                    }

                    else{
                        wattron(win, COLOR_PAIR(2));
                        mvwaddstr(win,23,1,">>> Wrong password. Please try again(press any key)");
                      
                        refresh();
                        wrefresh(win);
                        wattroff(win, COLOR_PAIR(2));
                        getch();
                        winWithdrawal(id,win);
                    }
                }
                else
                    winMainAfterLogin(id,win);
        }
        
    
        refresh();
        wrefresh(win);
    }


}

void winMainAfterLogin(char* id, WINDOW* win){
    int button = 0;
    win = drawBackground();
    wattron(win,COLOR_PAIR(1));
    mvwaddstr(win,6,30,"PLAYER ID : ");
    waddstr(win,id);
    wattroff(win,COLOR_PAIR(1));
    while(1){
        wattron(win, COLOR_PAIR(2));
        mvwaddstr(win,21,10,"PLAY");
        mvwaddstr(win,21,35,"SIGN OUT");
        mvwaddstr(win,21,65,"WITHDRAWAL");
        wattroff(win, COLOR_PAIR(2));

        wattron(win, COLOR_PAIR(1)|A_UNDERLINE);
        switch(button = checkButton(button,2)){
            case 0:
                mvwaddstr(win,21,10,"PLAY");
                break;
            case 1:
                mvwaddstr(win,21,35,"SIGN OUT");
                break;
            case 2:
                mvwaddstr(win,21,65,"WITHDRAWAL");
                break;
        }
        wattroff(win,COLOR_PAIR(1)|A_UNDERLINE);
        refresh();
        wrefresh(win);
        
        switch(getch()){
            case KEY_LEFT:
                button--;
                break;
            case KEY_RIGHT:
                button++;
                break;
            case '\n':
                if(button == 0)
                    play(id, win);
                else if(button == 1)
                    signOut();
                else
                    winWithdrawal(id,win);
        }

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
                else{
                    curs_set(1);
                    endwin();
                    exit(1);
                }
        }

    }
}


int main(){
    int fd;
    fd = open("2015726056.txt", O_WRONLY | O_CREAT, 0666);
    close(fd);
    winMainBeforeLogin();
    return 0;
}
