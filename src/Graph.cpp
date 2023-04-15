#include "Graph.hpp"

// m_racine n'est qu'une référence faible vers le sommet stocké dans la hashmap
Graph::Graph(std::shared_ptr<Sommets> node) :
    m_racine(node)
{m_hash_map.insert(std::pair<Plateau*, std::shared_ptr<Sommets>>(node->get_plateau().get(), node));}

void Graph::generer(std::shared_ptr<Sommets> node){
    std::vector<std::unique_ptr<Plateau>> plateaux_voisins = node->generer_voisins();

    for(std::unique_ptr<Plateau>& plateau: plateaux_voisins){
        auto potentiel_sommet = m_hash_map.find(plateau.get());

        // le voisin est un noeud qui n'existait pas
        if(potentiel_sommet == m_hash_map.end())
        { 
            // On crée le sommet à partir du plateau
            std::shared_ptr<Sommets> nouveau_sommet = std::make_shared<Sommets>(std::move(plateau));

            // On fait les liens avec le sommets actuel
            node->link(nouveau_sommet,1);
            nouveau_sommet->m_distance = 1 + node->m_distance;
            m_file_noeud.push(nouveau_sommet);

            // On l'ajoute dans la hashmap
            m_hash_map.insert(std::pair<Plateau*, std::shared_ptr<Sommets>>(nouveau_sommet->get_plateau().get(), std::move(nouveau_sommet)));
        }
        // le voisin est un noeud déjà existant
        else{ 
            // On fait les liens avec le sommets actuel
            node->link(potentiel_sommet->second,1);
            // Le plateau sera delete puisque c'est un unique ptr
        }
    }
}

void Graph::restart_parcours()
{
    std::shared_ptr<Sommets> tmp = m_file_noeud.front();
    while(!m_file_noeud.empty())
    {
        m_file_noeud.pop();
    }
    
    for(auto &it:m_hash_map)
        it.second.reset();
}
std::vector<std::shared_ptr<Sommets>> Graph::parcours(bool chercher_solution){
    std::vector<std::shared_ptr<Sommets>> res;
    m_file_noeud.push(m_racine.lock());
    std::shared_ptr<Sommets> current_noeud = nullptr;
    
    while(!m_file_noeud.empty())
    {
        current_noeud = m_file_noeud.front();
        m_file_noeud.pop();
        
        if(!current_noeud->m_traite){
            if(current_noeud->get_plateau()->est_gagnant()){
                res.push_back(current_noeud);
                if(chercher_solution)
                    break;
            }
        
            generer(current_noeud);
            current_noeud->m_traite=true;
        }
    }
    return res;
}


std::shared_ptr<Sommets> Graph::generer_lvl(std::vector<std::shared_ptr<Sommets>> solutions){
    for(auto& it: m_hash_map){
        it.second->m_traite=false;
        it.second->m_distance=0;
    }
    for(std::shared_ptr<Sommets> s: solutions)
        m_file_noeud.push(s);
    
    std::shared_ptr<Sommets> current_node;
    while(!m_file_noeud.empty()){
        current_node=m_file_noeud.front();
        m_file_noeud.pop();
        if(!current_node->m_traite){
            for(std::shared_ptr<Sommets> s : current_node->get_voisins()){
                if(!s->m_traite){
                    m_file_noeud.push(s);
                    s->m_distance=current_node->m_distance+1;
                }
            }
            current_node->m_traite=true;
        }
    }
    
    return current_node;
}


Graph::~Graph() {
    while(!m_file_noeud.empty())
        m_file_noeud.pop();

    // On rappel que m_racine est détenu par l'unordered map, et non pas par le graph
    // elle sera donc bien libérée par cette boucle
    for(auto &it:m_hash_map) {
        //if(it.second != nullptr) // inutile à priori
        it.second.reset();
    }

    m_hash_map.clear();
}

void Graph::test(){
    //test constructeur
    // Graph graph_test(new Sommets<int>(3));
    // assert(graph_test.m_noeud->get_plateau()==3);
    // assert(graph_test.m_hash_map[3]->get_plateau()==3);
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