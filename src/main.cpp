#include "Sommets.hpp"
#include "Graph.hpp"


int main(){
    std::srand(std::time(nullptr));
    int r=rand()%10;
    std::vector<Sommets<int>*> lil_graph;
    Sommets<int>* init_noeud= new Sommets<int>(r);
    std::unordered_map<int,Sommets<int>*> lil_graph_map;
    //(lil_graph.begin(),lil_graph.end());
    lil_graph_map[r]= init_noeud;
    Graph<int> graph=Graph(lil_graph_map);

    /*for (int i=0; i<15; i++) {
        lil_graph.push_back(new Sommets<int>(i));
    }
    lil_graph[0]->link(lil_graph[2]);
    lil_graph[2]->link(lil_graph[1]);
    lil_graph[2]->link(lil_graph[7]);
    lil_graph[1]->link(lil_graph[3]);
    lil_graph[7]->link(lil_graph[8]);
    lil_graph[7]->link(lil_graph[10]);
    lil_graph[8]->link(lil_graph[9]);
    lil_graph[10]->link(lil_graph[11]);
    lil_graph[11]->link(lil_graph[12]);
    lil_graph[13]->link(lil_graph[13]);
    lil_graph[13]->link(lil_graph[14]);*/
    //Graph<int> graph= Graph(lil_graph, lil_graph_map);
    graph.parcours(5);
    
    return 0;
}