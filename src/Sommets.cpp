#include "Sommets.hpp"
#include <memory>

Sommets::Sommets(std::unique_ptr<Plateau> plateau) :
    m_plateau(std::move(plateau)),
    m_traite(false),
    m_distance(0)
{}

void Sommets::link(Sommets& som, int poids){
    m_chemin_voisins.push_back({som, poids});
    som.m_chemin_voisins.push_back({*this, poids});
}

std::shared_ptr<Plateau> Sommets::get_plateau() const
{
    return m_plateau;
}

std::vector<std::weak_ptr<Sommets>> Sommets::get_voisins() const
{
    std::vector<std::weak_ptr<Sommets>> res;
    for(Lien node: m_chemin_voisins){
        res.push_back(std::make_shared<Sommets>(node.voisin));
    }
    return res;
}

std::vector<std::unique_ptr<Plateau>> Sommets::generer_voisins(){
    return m_plateau->get_neighbours();
}

Sommets::~Sommets(){
    m_traite = false;
    m_distance = 0;
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