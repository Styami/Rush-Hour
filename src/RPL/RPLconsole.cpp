#include "RPLconsole.hpp"

using namespace std;

namespace RPL {


consoleWindow::consoleWindow(unsigned int w, unsigned int h, const string& _title, unsigned char console_mode, int _framerate) :
    dimx(w), dimy(h), window(new Cell[w * h]), current_color(), current_color_mode(38), framerate(_framerate), title(_title)
{
    termInit();

    for(int i = 0; i < dimx*dimy; i++) {
        window[i].c = ' ';
        window[i].col = current_color;
        window[i].col_mode = 38;
    }

    if(console_mode & CONSOLE_COMPACT) {
        spaced = false;
    }
    if(console_mode & CONSOLE_SPACED) {
        spaced = true;
    }
    if(console_mode & CONSOLE_BORDERLESS) {
        bordered = false;
    }
    if(console_mode & CONSOLE_BORDERED) {
        bordered = true;
    }

    draw_window();
}

consoleWindow::~consoleWindow() {
    delete [] window;
}

void consoleWindow::clear_window() {
    printf("\e[%iA", dimy + (bordered ? 2 : 0));
    printf("\e[0J");
    
    for(int i=0; i<dimy*dimx; i++) {
        window[i].c=' ';
        window[i].col=current_color;
        window[i].col_mode=current_color_mode;
    }
}

void consoleWindow::draw_window() const{
    //Dessine le haut de la bordure
    if(bordered) {
        printf("┌%s", title.c_str());
        
        for(std::size_t i = 0; i < dimx * (spaced ? 2 : 1) - title.length(); i++) {
            
            printf("─");
        }
        printf("┐\n");
    }


    for(int j= 0; j < dimy; j++) {
        if(bordered) printf("│");
        for(int i = 0; i < dimx; i++) {
            printf("\e[%u;2;%u;%u;%um%c%c\e[0m", 
                window[i + j * dimx].col_mode,  //%u
                window[i + j * dimx].col.r,     //%u
                window[i + j * dimx].col.g,     //%u
                window[i + j * dimx].col.b,     //%u
                window[i + j * dimx].c,         //%c
                (spaced ? ' ' : '\0')           //%c
            );
        }
        if(bordered) printf("│");
        printf("\n");
    }

    //Dessine le bas de la bordure
    if(bordered) {
        printf("└");
        for(int i = 0; i < dimx  * (spaced ? 2 : 1); i++) {
            printf("─");
        }
        printf("┘\n");
    }
}

void consoleWindow::set_color(unsigned char r, unsigned char g, unsigned char b) {
    current_color = Color(r, g, b);
}

void consoleWindow::set_color(Color col) {
    current_color = col;
}

void consoleWindow::set_color_mode(int color_background) {
    if(color_background == CONSOLE_COLOR_BACKGROUND)
        current_color_mode=48;
    else
        current_color_mode=38;
}

void consoleWindow::print_char(int x, int y, char c) {
    window[x + y * dimx].c = c;
    window[x + y * dimx].col = current_color;
    window[x + y * dimx].col_mode = current_color_mode;
}

void consoleWindow::print_char(int x, int y, const char* c) {
    int offset = 0;
    while(*(c + offset) != '\0') {
        print_char(x + offset, y, *(c+offset));
        offset++;
    }
}

void consoleWindow::print_char(int x, int y, const std::string& string) {
    print_char(x, y, string.c_str());
}

int consoleWindow::kbhit() {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}

char consoleWindow::getch() { // lire un caractere si une touche a ete pressee
    char touche=0;
    if (kbhit())
        touche = fgetc(stdin);
    return touche;
}

void consoleWindow::set_framerate(int fps) {
    framerate = fps;
}

bool consoleWindow::handle_framerate() {
    std::chrono::duration<double> elapsed_time = std::chrono::steady_clock::now() - last_update;
    if(elapsed_time.count() * framerate >= 1.f) {
        last_update = std::chrono::steady_clock::now();
        return true;
    }
    return false;
}

void consoleWindow::termInit() {
    struct termios ttystate;
    bool state = true;

    //get the terminal state
    tcgetattr(STDIN_FILENO, &ttystate);

    if (state) {
        //turn off canonical mode
        ttystate.c_lflag &= ~ICANON;
        //minimum of number input read.
        ttystate.c_cc[VMIN] = 1;
    }
    else {
        //turn on canonical mode
        ttystate.c_lflag |= ICANON;
    }
    //set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
}