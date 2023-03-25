#include "Sommets.hpp"
#include "Graph.hpp"


int main(){
    std::srand(std::time(nullptr));
    int r=rand()%10;

    Sommets<int>* init_noeud= new Sommets<int>(r); //création du noeud initial
    Graph<int> graph=Graph(init_noeud);

    int nb_voisins_node=3;
    graph.parcours(5,nb_voisins_node);
    
    return 0;
}