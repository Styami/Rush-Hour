#include "Sommets.hpp"
#include "Graph.hpp"
#include "affichage.hpp"

int main(){
    srand(time(NULL));
    
    std::unique_ptr<Plateau> first_plat= std::make_unique<Plateau>("data/niveau_0.rh");
    Window window(first_plat->get_block_count());

    std::unique_ptr<Sommets> node = std::make_unique<Sommets> (std::move(first_plat));
    std::shared_ptr<Sommets> resultat;
    window.dessiner_plateau(node->get_plateau()->get_block_array(), node->get_plateau()->get_block_count());
    getchar();
    {
        Graph mon_graph(std::move(node));
        resultat = std::move(mon_graph.parcours(true).lock());
        
    }
    window.dessiner_plateau(resultat->get_plateau()->get_block_array(), resultat->get_plateau()->get_block_count());
    getchar();
    {
        Graph niveau_complique(std::move(resultat));
        resultat = std::move(niveau_complique.parcours(false).lock());
    }
    window.dessiner_plateau(resultat->get_plateau()->get_block_array(), resultat->get_plateau()->get_block_count());
    getchar();

    // if(node == nullptr)
    //     std::cout << "Il n'y a pas de solution possible à partir du plateau initialisé." << '\n';
    // else{
    //     Graph mon_graph = Graph(node);
    //     const Sommets* res = std::move(mon_graph.parcours(false)); 
    // }
    // window.dessiner_plateau(node->get_plateau()->get_block_array(), node->get_plateau()->get_block_count());
    // getchar();
    return 0;
}