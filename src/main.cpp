#include "Sommets.hpp"
#include "Graph.hpp"
#include "affichage.hpp"

int main(){
    Window window;

    std::unique_ptr<Plateau> first_plat= std::make_unique<Plateau>("data/niveau0.rh");
    Sommets* node= new Sommets(std::move(first_plat));
    Plateau* p;
    window.dessiner_plateau(node->get_element()->get_block_array(), node->get_element()->get_block_count());
    getchar();
    {
        Graph mon_graph = Graph(node);
        node = std::move(mon_graph.parcours(true)[0]);
        window.dessiner_plateau(node->get_element()->get_block_array(), node->get_element()->get_block_count());
        getchar();

        Graph new_graph = Graph(node);
        std::vector<Sommets*> vec_node;
        vec_node = new_graph.parcours(false);
        std::cout << node->distance << "\n";
        getchar();
        window.dessiner_plateau(node->get_element()->get_block_array(), node->get_element()->get_block_count());
        getchar();
    }

    if(node == nullptr)
        std::cout << "Il n'y a pas de solution possible à partir du plateau initialisé." << '\n';
    else{
        std::vector<Sommets*> vec_node;
        Graph mon_graph = Graph(node);
        vec_node = mon_graph.parcours(false);
        //mon_graph.generer_lvl(vec_node);
        const Sommets* res = std::move(mon_graph.generer_lvl(vec_node)); 
        window.dessiner_plateau(res->get_element()->get_block_array(), res->get_element()->get_block_count());
        getchar();
    }
    return 0;
}