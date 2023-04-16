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
    m_distance(s.m_distance)
{}

Sommets::Sommets(Sommets&& s) :
    m_plateau(std::move(s.m_plateau)),
    m_traite(std::move(s.m_traite)),
    m_distance(std::move(s.m_distance))
{}

Sommets& Sommets::operator=(const Sommets& s)
{
    m_traite = s.m_traite;
    m_distance = s.m_distance;
    m_plateau = s.m_plateau;
    return *this;
}

Sommets& Sommets::operator=(Sommets&& s)
{
    if(&s != this) {
        m_traite = std::move(s.m_traite);
        m_distance = std::move(s.m_distance);
        m_plateau = std::move(s.m_plateau);

        s.m_traite = 0;
        s.m_distance = 0;
    }

    return *this;
}

Sommets::~Sommets(){
    m_traite = false;
    m_distance = 0;
    m_chemin_voisins.clear();
}

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
        res.push_back(link.voisin.lock());
    }
    return res;
}

std::vector<std::unique_ptr<Plateau>> Sommets::generer_voisins(){
    return m_plateau->get_neighbours();
}

void Sommets::test(){
    Plateau plateau_test = Plateau("data/test_data_human_readable");
    //test constructeur
    std::shared_ptr<Sommets> noeud_test = std::make_shared<Sommets>(std::make_unique<Plateau>("data/test_data_human_readable"));
    assert(*noeud_test->get_plateau() == plateau_test);
    assert(noeud_test->m_traite == false);
    assert(noeud_test->m_distance == 0);
    assert(noeud_test->precedent.expired());
    assert(noeud_test->m_chemin_voisins.empty());

    //test sur la génération des voisins
    std::vector<std::unique_ptr<Plateau>> nouveaux_plateaux = noeud_test->generer_voisins();
    assert(nouveaux_plateaux.empty() != false);
    for(std::unique_ptr<Plateau>& nouveau_plateau : nouveaux_plateaux){
        std::shared_ptr<Sommets> nouveau_sommet = std::make_shared<Sommets>(std::move(nouveau_plateau));
        noeud_test->link(nouveau_sommet,1);
        assert(nouveau_sommet->m_chemin_voisins[0].voisin.expired());
        nouveau_sommet->precedent = noeud_test;
        assert(nouveau_sommet->precedent.expired() == false);
    }
    
    //test permettant de savoir si le linkage à bien eu lieu
    std::vector<std::shared_ptr<Sommets>> voisins = noeud_test->get_voisins();
    for(std::shared_ptr<Sommets> voisin : voisins){
        assert(voisin != nullptr);
    }
    assert(voisins.size() == noeud_test->m_chemin_voisins.size());
}