#include "bloc.hpp"
#include "plateau.hpp"

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
        std::cout << "Tous les tests ont réussi";
    else
        std::cout << err << " tests ont échoué"; 

    std::cout << std::endl;
    
    return 0;
}