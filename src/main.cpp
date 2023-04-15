#include "Sommets.hpp"
#include "Graph.hpp"
#include "affichage.hpp"

int main(){
    srand(time(NULL));
    
    std::unique_ptr<Plateau> first_plat= std::make_unique<Plateau>("data/test_data_human_readable.rh");
    Window window(first_plat->get_block_count());

    std::unique_ptr<Sommets> node = std::make_unique<Sommets> (std::move(first_plat));
    std::shared_ptr<Sommets> resultat;
    //Initialisation
    window.dessiner_plateau(node->get_plateau()->get_block_array(), node->get_plateau()->get_block_count());
    getchar();

    //Recherche de la solution la plus proche
    {
        Graph mon_graph(std::move(node));
        resultat = std::move(mon_graph.parcours(true)[0]);
        
    }
    window.dessiner_plateau(resultat->get_plateau()->get_block_array(), resultat->get_plateau()->get_block_count());
    getchar();

    //Recherche de toutes les solutions
    std::vector<std::shared_ptr<Sommets>> all_solution;
    std::shared_ptr<Sommets> init_hard;
    {
        Graph niveau_complique(std::move(resultat));
        all_solution = std::move(niveau_complique.parcours(false));
        //std::cout << all_solution.size() << '\n';

        //Une fois toutes les solutions trouvées, il suffit de chercher la solution la plus loin.
        init_hard=std::move(niveau_complique.generer_lvl(all_solution));
        //std::cout << "Fonctionne toujours?" << '\n';
    }
    window.dessiner_plateau(init_hard->get_plateau()->get_block_array(), init_hard->get_plateau()->get_block_count());
    getchar();

    // if(resultat == nullptr)
    //     std::cout << "Il n'y a pas de solution possible à partir du plateau initialisé." << '\n';
    // else{
    //     std::vector<std::shared_ptr<Sommets>> vec_node;
    //     Graph mon_graph = Graph(std::move(node));
    //     vec_node = mon_graph.parcours(false);
    //     //mon_graph.generer_lvl(vec_node);
    //     std::shared_ptr<Sommets> res = std::move(mon_graph.generer_lvl(vec_node)); 
    //     window.dessiner_plateau(res->get_plateau()->get_block_array(), res->get_plateau()->get_block_count());
    //     getchar();
    // }
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