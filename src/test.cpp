#include "bloc.hpp"
#include "plateau.hpp"
#include "console_app.hpp"

int main(void) {
    int err = 0;

    if(!Bloc::test()) {
        std::cout << "Le test de bloc a échoué\n";
        err++;
    }

    if(!Plateau::test()) {
        std::cout << "Le test de plateau a échoué\n";
        err++;
    }

    if(err == 0)
        std::cout << "Tous les tests ont réussi\n";
    else
        std::cout << err << " tests ont échoué\n"; 


    std::cout << "Appuyer sur une touche pour lancer l'affichage d'un plateau\n";
    getchar();

    Window::test();

    std::cout << std::endl;
    
    return 0;
}