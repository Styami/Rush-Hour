template <class T>
Graph<T>::Graph(Sommets<T>* node) :
    m_noeud(node)
{m_hash_map.insert(std::pair<T, Sommets<T>*>(node->get_element(),node));}

template <class T>
void Graph<T>::generer(Sommets<T>* node,int nb_voisins){
    std::vector<Sommets<T>*> tab_tmp=node->generer(nb_voisins);
    for(Sommets<T>* t: tab_tmp){
        auto it=m_hash_map.find(t->get_element());
        if(it==m_hash_map.end()){ // le voisin est un noeud qui n'existait pas
            m_hash_map.insert(std::pair<T,Sommets<T>*>(t->get_element(), t));
            node->link(t);
            m_file_noeud.push(t);
        }
        else{ // le voisin est un noeud déjà existant
            node->link(it->second);
            delete t;
            m_file_noeud.push(it->second);
        }
    }
}

template <class T>
void Graph<T>::parcours(T val,int nb_voisins){
    m_file_noeud.push(m_noeud);
    while(!m_file_noeud.empty()){
        Sommets<T>* current_noeud=m_file_noeud.front();
        m_file_noeud.pop();
        if(!current_noeud->m_traite){
            if(current_noeud->get_element()==val){
                std::cout << "valeur trouvée" << std::endl;
                return;
            }
            generer(current_noeud, nb_voisins);
            current_noeud->m_traite=true;
        }
    }
    std::cout << "la valeur n'a pas été trouvé" << std::endl;
}

template<class T>
Graph<T>::~Graph(){
    Sommets<T>* tmp=m_file_noeud.front();
    while(tmp!=nullptr){
        m_file_noeud.pop();
        tmp=m_file_noeud.front();
    }
    for(auto &it:m_hash_map)
        delete it.second;
    m_hash_map.clear();
    m_noeud=nullptr;
}

template<class T>
void Graph<T>::test(){
    //test constructeur
    Graph graph_test(new Sommets<int>(3));
    assert(graph_test.m_noeud->get_element()==3);
    assert(graph_test.m_hash_map[3]->get_element()==3);
    graph_test.generer(graph_test.m_noeud,5);
    assert(graph_test.m_noeud->m_voisins.size()==5);
    assert(graph_test.m_file_noeud.size()==5);

}