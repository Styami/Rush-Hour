#include "console_app.hpp"

int main(){
    srand(time(NULL));
    
    Window window;

    window.main_loop();

    return 0;
}