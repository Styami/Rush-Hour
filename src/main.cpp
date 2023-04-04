#include "Sommets.hpp"
#include "Graph.hpp"
#include "affichage.hpp"

int main(){
    
    std::unique_ptr<Plateau> first_plat= std::make_unique<Plateau>("../data/niveau0.rh");
    Sommets* node= new Sommets(std::move(first_plat));
    

    {
        Graph mon_graph = Graph(node);
        node = mon_graph.parcours(true);
    }
    if(node == nullptr)
        std::cout << "Il n'y a pas de solution possible à partir du plateau initialisé." << '\n';
    else{
        Graph mon_graph = Graph(node);
        const Sommets* res = mon_graph.parcours(false); 
    }
    return 0;
}