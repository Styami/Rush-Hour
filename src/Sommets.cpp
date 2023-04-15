#include "Sommets.hpp"
#include <memory>

Sommets::Sommets(std::unique_ptr<Plateau> plateau) :
    m_plateau(std::move(plateau)),
    m_traite(false),
    m_distance(0)
{}

void Sommets::link(std::shared_ptr<Sommets> som, int poids){
    m_chemin_voisins.push_back({som, poids});
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
        res.push_back(link.voisin);
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
    Plateau plateau_test = Plateau("data/test_data_human_readable");
    std::shared_ptr<Sommets> noeud_test = std::make_shared<Sommets>(std::make_unique<Plateau>("data/test_data_human_readable"));
    assert(*noeud_test->get_plateau() == plateau_test);
    assert(noeud_test->m_traite == false);
    assert(noeud_test->m_distance == 0);
    assert(noeud_test->precedent.expired());
    assert(noeud_test->m_chemin_voisins.empty());

    std::vector<std::unique_ptr<Plateau>> nouveaux_plateaux = noeud_test->generer_voisins();
    assert(nouveaux_plateaux.empty() != false);
    for(std::unique_ptr<Plateau>& nouveau_plateau : nouveaux_plateaux){
        std::shared_ptr<Sommets> nouveau_sommet = std::make_shared<Sommets>(std::move(nouveau_plateau));
        noeud_test->link(nouveau_sommet,1);
        nouveau_sommet->precedent = noeud_test;
        assert(nouveau_sommet->precedent.expired() == false);
    }
    
    std::vector<std::shared_ptr<Sommets>> voisins = noeud_test->get_voisins();
    assert(voisins.size() == noeud_test->m_chemin_voisins.size());
}