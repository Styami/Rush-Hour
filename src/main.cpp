#include "Sommets.hpp"
#include <vector>
#include "Graph.hpp"

int main(){
    std::vector<Sommets<int>*> lil_graph;
    for (int i=0; i<15; i++) {
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
    lil_graph[13]->link(lil_graph[14]);
    Graph<int> graph= Graph(lil_graph);
    graph.parcours(6);
    return 0;
}