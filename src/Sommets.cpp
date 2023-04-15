#include "Sommets.hpp"
#include <memory>

Sommets::Sommets(std::unique_ptr<Plateau> plateau) :
    m_plateau(std::move(plateau)),
    m_traite(false),
    m_distance(0)
{}

Sommets::Sommets(const Sommets& s) :
    m_plateau(s.m_plateau),
    m_traite(s.m_traite),
    m_distance((s.m_distance))
{}

Sommets::Sommets(Sommets&& s) :
    m_plateau(std::move(s.m_plateau)),
    m_traite(std::move(s.m_traite)),
    m_distance(std::move(s.m_distance))
{}

Sommets::~Sommets(){
    m_traite = false;
    m_distance = 0;
    m_chemin_voisins.clear();
}

void Sommets::link(std::shared_ptr<Sommets> som, int poids){
    //std::weak_ptr<Sommets> new_som=std::move(std::make_shared<Sommets>(som));
    //std::cout << new_som << '\n';
    m_chemin_voisins.push_back({som, poids});
    //std::weak_ptr<Sommets> sois_meme=std::move(std::make_shared<Sommets>(*this));
    som->m_chemin_voisins.push_back({std::make_shared<Sommets>(*this), poids});
}

std::shared_ptr<Plateau> Sommets::get_plateau() const
{
    return m_plateau;
}

std::vector<std::shared_ptr<Sommets>> Sommets::get_voisins() const
{
    std::vector<std::shared_ptr<Sommets>> res;
    for(Lien link: m_chemin_voisins){
        res.push_back(link.voisin.lock());
    }
    return res;
}

std::vector<std::unique_ptr<Plateau>> Sommets::generer_voisins(){
    return m_plateau->get_neighbours();
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