#include "Sommets.hpp"
#include "Graph.hpp"
#include "console_app.hpp"

int main(){
    srand(time(NULL));
    
    Graph g;
    Window window(&g);

    window.main_loop();

    return 0;
}