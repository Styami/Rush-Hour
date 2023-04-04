#include "Graph.hpp"

Graph::Graph(Sommets* node) :
    m_racine(node)
{m_hash_map.insert(std::pair<Plateau*, Sommets*>(node->get_element(),node));}

void Graph::generer(Sommets* node){
    std::vector<std::unique_ptr<Plateau>> plateaux_voisins =node->generer();

    for(std::unique_ptr<Plateau>& plateau: plateaux_voisins){
        auto potentiel_sommet=m_hash_map.find(plateau.get());

        if(potentiel_sommet==m_hash_map.end()){ // le voisin est un noeud qui n'existait pas
            Sommets* tmp_ptr=new Sommets(std::move(plateau));
            m_hash_map.insert(std::pair<Plateau*,Sommets*>(tmp_ptr->get_element(), tmp_ptr));
            node->link(tmp_ptr,1);
            tmp_ptr->distance = 1 + node->distance;
            m_file_noeud.push(tmp_ptr);
        }

        else{ // le voisin est un noeud déjà existant
            node->link(potentiel_sommet->second,1);
        }
    }
}

void Graph::restart_parcours(){
    Sommets* tmp=m_file_noeud.front();
    while(tmp!=nullptr){
        m_file_noeud.pop();
        tmp=m_file_noeud.front();
    }
    for(auto &it:m_hash_map)
        delete it.second;
}
Sommets* Graph::parcours(bool chercher_solution){
    m_file_noeud.push(m_racine);
    Sommets* res=nullptr;
    while(!m_file_noeud.empty()){
        Sommets* current_noeud=m_file_noeud.front();
        m_file_noeud.pop();
        
        if(!current_noeud->m_traite){
            if(chercher_solution && current_noeud->get_element()->est_gagnant()){
                std::cout << "Tu as gagné!" << std::endl;
                res=current_noeud;
                break;
            }
            if(!chercher_solution){
                if(res->distance<current_noeud->distance)
                    res=current_noeud;
            }
        
            generer(current_noeud);
            current_noeud->m_traite=true;
        }
    }

    std::cout << "le noeud final:" << std::endl;
    return res;
}


const Sommets* Graph::farthest_node(Sommets* init_node){
    restart_parcours();
    m_file_noeud.push(init_node);
    int farthest_distance=0;
    int current_distance=0;
    Sommets* res=nullptr;
    while(!m_file_noeud.empty()){
        Sommets* current_noeud=m_file_noeud.front();
        m_file_noeud.pop();
        if(!current_noeud->m_traite){
            for(Sommets* noeud: current_noeud->get_node_neighbours()){
                m_file_noeud.push(noeud);
            }
            current_noeud->distance=current_distance+1;
            if(current_noeud->distance>farthest_distance){
                farthest_distance=current_noeud->distance;
                res=current_noeud;
            }
            current_noeud->m_traite=true;
        }
    }
    return res;
}

Graph::~Graph(){
    Sommets* tmp=m_file_noeud.front();
    while(tmp!=nullptr){
        m_file_noeud.pop();
        tmp=m_file_noeud.front();
    }
    for(auto &it:m_hash_map)
        delete it.second;
    m_hash_map.clear();
    m_racine=nullptr;
}

void Graph::test(){
    //test constructeur
    // Graph graph_test(new Sommets<int>(3));
    // assert(graph_test.m_noeud->get_element()==3);
    // assert(graph_test.m_hash_map[3]->get_element()==3);
    // graph_test.generer(graph_test.m_noeud,5);
    // assert(graph_test.m_noeud->m_voisins.size()==5);
    // assert(graph_test.m_file_noeud.size()==5);

}



/*
    fichier
    plateau

    on cherche une solution
    y a une solution -> stock solution
    y en a pas -> on charge un autre truc

    L'autre chose à faire c'est trouver des problèmes intéressant
    On part de la solution -> on cherche chemin le plus long.

*/