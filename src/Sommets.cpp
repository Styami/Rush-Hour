#include "Sommets.hpp"

Sommets::Sommets(std::unique_ptr<Plateau> element) :
    m_element(std::move(element)),
    m_traite(false)
{}

void Sommets::link(Sommets* som, int poids){
    m_chemin_voisins.push_back({som, poids});
    som->m_chemin_voisins.push_back({this,poids});
}

Plateau* Sommets::get_element() const{
    return m_element.get();
}

std::vector<Sommets*> Sommets::get_node_neighbours(){
    std::vector<Sommets*> res;
    for(Lien node: m_chemin_voisins){
        res.push_back(node.voisin);
    }
    return res;
}

std::vector<std::unique_ptr<Plateau>> Sommets::generer(){
    return m_element->get_neighbours();
}

Sommets::~Sommets(){
    m_element=0;
    m_traite=false;
    m_chemin_voisins.clear();
}

void Sommets::test(){
    // Sommets<int>* n=new Sommets<int>(5);
    // assert(n->get_element()==5);
    // assert(n->m_traite==false);
    // Sommets<int>* an=new Sommets<int> (6);
    // n->link(an);
    // an=nullptr;
    // assert(n->m_voisins[0]->get_element()==6);
    // delete n->m_voisins[0];
    // n->m_voisins.clear();
}