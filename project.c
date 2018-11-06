#include <ncurses.h>
#define WIDTH 80
#define HEIGHT 24

int checkButton(int number);
void winSignIn(WINDOW * win);
void winSignUp(WINDOW * win);
void winMainBeforeLogin();
WINDOW* drawBackground();

int checkButton(int number){
    if(number < 0)
        return 2;
    else if(number > 2)
        return 0;
    else
        return number;
}
void winSignIn(WINDOW * win){
    mvwaddstr(win,5,5,"Hello");
}
void winSignUp(WINDOW * win){
    mvwaddstr(win,5,5,"hello");

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

    wattron(win, COLOR_PAIR(1));
    mvwaddstr(win,5,27,"System Softwware Practice");
    mvwaddstr(win,7,35,"OTHELLO");
    mvwaddstr(win,14,68,"2015726056");
    mvwaddstr(win,16,68,"DongHee Cho");
    wattroff(win, COLOR_PAIR(1));
    return win;
}
    //백그라운드 여기까지!!
void win_MainBeforeLogin(){
    WINDOW * win = drawBackground();
    int button;
    while(1) {
        wattron(win, COLOR_PAIR(2));
        mvwaddstr(win,21,10,"SIGN IN");
        mvwaddstr(win,21,35,"SIGN UP");
        mvwaddstr(win,21,65,"EXIT");
        wattroff(win, COLOR_PAIR(2));
        
        wattron(win, COLOR_PAIR(1)|A_UNDERLINE);
        switch(button = checkButton(button)){
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
                endwin();   //curses 모드를 종료한다.
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
    win_MainBeforeLogin();
    curs_set(1);
    endwin();   //curses 모드를 종료한다.
    
    return 0;
}
