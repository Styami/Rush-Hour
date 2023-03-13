#include "Graph.hpp"


template <class T>
Graph<T>::Graph(vector<Sommets<T>*> init_val) :
    m_noeud(init_val)
{}

template <class T>
void Graph<T>::parcours(T val){
    std::cout << "byfvgfbgyubuiybrfrbh" << std::endl;
    m_file_noeud.push(m_noeud[0]);
    while(!m_file_noeud.empty()){
        Sommets<T>* current_noeud=m_file_noeud.front();
        m_file_noeud.pop();
        if(!current_noeud->m_traite){
            if(current_noeud->get_element()==val){
                std::cout << "valeur trouvée" << std::endl;
                return;
            }
            for(int i=0; i<current_noeud->m_voisins.size(); i++){
                m_file_noeud.push(current_noeud->m_voisins[i]);
            }
            std::cout << current_noeud->get_element() << std::endl;
            current_noeud->m_traite=true;
        }
    }
    std::cout << "la valeur n'a pas été trouvé" << std::endl;
}