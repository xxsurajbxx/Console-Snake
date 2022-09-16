#include <iostream>
#include <windows.h>
#include <ctime>

using namespace std;

char screen[120*21] = {' '};
char map[20][20];
int score=0;

class Snake {
    private:
    int px, py;
    int fx, fy;
    int segcoords[400][2]={0};
    void clear() {
        for(int x=0; x<score+1; x++) {
            map[segcoords[x][1]][segcoords[x][0]] = ' ';
        }
    }
    void put() {
        for(int x=0; x<score+1; x++) {
            map[segcoords[x][1]][segcoords[x][0]] = 'O';
        }
    }
    void spawnFood() {
        fx = (rand()%18)+1;
        fy = (rand()%18)+1;
        map[fy][fx] = '$';
    }
    public:
    char dir;
    Snake() {
        srand(time(0));
        px=10;
        py=10;
        segcoords[0][0] = px;
        segcoords[0][1] = py;
        spawnFood();
    }
    bool collision() {
        if(score>0) {            for(int x=1; x<score+1; x++) {
                if(px==segcoords[x][0]&&py==segcoords[x][1]) {return true;}
            }        }
        if(px==fx&&py==fy) {
            score++;
            segcoords[score][0] = px;
            segcoords[score][1] = py;
            spawnFood();
        }        switch(dir) {
            case 'N':
                if(map[py-1][px]=='-') {
                    return true;
                }
                break;
            case 'S':
                if(map[py+1][px]=='-') {
                    return true;
                }
                break;
            case 'E':
                if(map[py][px+1]=='|') {
                    return true;
                }
                break;
            case 'W':
                if(map[py][px-1]=='|') {
                    return true;
                }
                break;
        }
        return false;
    }
    void move() {
        clear();
        for(int x=score; x>=0; x--) {
            for(int i=0; i<2; i++) {
                segcoords[x+1][i] = segcoords[x][i];
            }
        }
        switch(dir) {
            case 'N':
                py--;
                break;
            case 'E':
                px++;
                break;
            case 'S':
                py++;
                break;
            case 'W':
                px--;
                break;
        }
        segcoords[0][0] = px;
        segcoords[0][1] = py;
        put();
    }
};

void buildMap() {
    for(int y=0; y<20; y++) {
        for(int x=0; x<20; x++) {
            if(x==0||x==19) {
                if(y==0||y==19) {
                    map[y][x] = '+';
                }
                else {map[y][x]='|';}
            }
            else if(y==0||y==19) {
                if(!(x==0||x==19)) {
                    map[y][x] = '-';
                }
            }
            else {map[y][x] = ' ';}
        }
    }
}

void sync() {
    string str = "Score: ";
    for(int x=0; x<7; x++) {
        screen[x] = str[x];

    }
    string String = to_string(score);
    for(int y=0; y<String.length(); y++) {
        screen[7+y] = String[y];
    }
    int counter=0;
    for(int y=0; y<20; y++) {
        for(int x=0; x<20; x++) {
            screen[((y+1)*120)+counter] = map[y][x];
            counter+=2;
        }
        counter=0;
    }
    for(int x=1; x<38; x++) {
        screen[120+x] = '-';
    }
    for(int x=1; x<38; x++) {
        screen[2400+x] = '-';
    }
}

int main() {
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;
    bool loss=false;
    buildMap();
    Snake snek;
    while(!GetAsyncKeyState(0x1B)&&!loss) {
        loss=snek.collision();
        snek.move();
        sync();
        if(GetAsyncKeyState(0x53)&&snek.dir!='N') {
            snek.dir = 'S';
        }
        if(GetAsyncKeyState(0x44)&&snek.dir!='W') {
            snek.dir = 'E';
        }
        if(GetAsyncKeyState(0x41)&&snek.dir!='E') {
            snek.dir = 'W';
        }
        if(GetAsyncKeyState(0x57)&&snek.dir!='S') {
            snek.dir = 'N';
        }
        WriteConsoleOutputCharacterA(hConsole, screen, 120*21, { 0,0 }, &dwBytesWritten);
        Sleep(350);
    }
    for(int x=0; x<2520; x++) {
        screen[x] = ' ';
    }
    WriteConsoleOutputCharacterA(hConsole, screen, 120*21, { 0,0 }, &dwBytesWritten);
    string str = "You LostFeels BadScore: " + to_string(score);
    for(int x=0; x<str.length(); x++) {
        if(x>7) {
            if(x>16) {
                screen[240+x-17] = str[x];
            }
            else {
                screen[120+x-8] = str[x];
            }
        }
        else {
            screen[x] = str[x];
        }
    }
    WriteConsoleOutputCharacterA(hConsole, screen, 120*5, { 0,0 }, &dwBytesWritten);
    while(!GetAsyncKeyState(0x0D)) {}
}