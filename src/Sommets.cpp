#include "Sommets.hpp"
#include <memory>

Sommets::Sommets(std::unique_ptr<Plateau> plateau, std::shared_ptr<Sommets> precedent) :
    m_plateau(std::move(plateau)),
    m_traite(false),
    m_precedent(precedent),
    m_distance(0)
{}

Sommets::Sommets(const Sommets& s, std::shared_ptr<Sommets> precedent) :
    m_plateau(s.m_plateau),
    m_traite(s.m_traite),
    m_precedent(precedent),
    m_distance(s.m_distance)
{}

Sommets::Sommets(Sommets&& s) :
    m_plateau(std::move(s.m_plateau)),
    m_traite(std::move(s.m_traite)),
    m_precedent(s.m_precedent),
    m_distance(std::move(s.m_distance))
{}

Sommets& Sommets::operator=(const Sommets& s)
{
    m_traite = s.m_traite;
    m_distance = s.m_distance;
    m_precedent = s.m_precedent;
    m_plateau = s.m_plateau;
    return *this;
}

Sommets& Sommets::operator=(Sommets&& s)
{
    if(&s != this) {
        m_traite = std::move(s.m_traite);
        m_distance = std::move(s.m_distance);
        m_precedent = std::move(s.m_precedent);
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

void Sommets::link(std::shared_ptr<Sommets> autre_sommet, std::shared_ptr<Sommets> sois_meme, int poids){
    m_chemin_voisins.push_back({autre_sommet, poids});
    autre_sommet->m_chemin_voisins.push_back({sois_meme, poids});
}

std::shared_ptr<Plateau> Sommets::get_plateau() const
{
    return m_plateau;
}

std::vector<std::shared_ptr<Sommets>> Sommets::get_voisins() const
{
    std::vector<std::shared_ptr<Sommets>> res;
    for(Lien link: m_chemin_voisins){
        if(!link.voisin.expired())
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
    std::shared_ptr<Sommets> noeud_test = std::make_shared<Sommets>(std::make_unique<Plateau>("data/test_data_human_readable"), noeud_test);
    assert(*noeud_test->get_plateau() == plateau_test);
    assert(noeud_test->m_traite == false);
    assert(noeud_test->m_distance == 0);
    //assert(noeud_test->m_precedent.expired());
    assert(noeud_test->m_chemin_voisins.empty());

    //test sur la génération des voisins
    std::vector<std::unique_ptr<Plateau>> nouveaux_plateaux = noeud_test->generer_voisins();
    assert(nouveaux_plateaux.empty() != false);
    for(std::unique_ptr<Plateau>& nouveau_plateau : nouveaux_plateaux){
        std::shared_ptr<Sommets> nouveau_sommet = std::make_shared<Sommets>(std::move(nouveau_plateau), nouveau_sommet);
        noeud_test->link(nouveau_sommet, noeud_test, 1);
        assert(nouveau_sommet->m_chemin_voisins[0].voisin.expired());
        nouveau_sommet->m_precedent = noeud_test;
        //assert(nouveau_sommet->m_precedent.expired() == false);
    }
    
    //test permettant de savoir si le linkage à bien eu lieu
    std::vector<std::shared_ptr<Sommets>> voisins = noeud_test->get_voisins();
    for(std::shared_ptr<Sommets> voisin : voisins){
        assert(voisin != nullptr);
    }
    assert(voisins.size() == noeud_test->m_chemin_voisins.size());
}