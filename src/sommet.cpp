#include "sommet.hpp"

#include <memory>
#include <ctime>
#include <cstdlib>
#include <assert.h>

Sommets::Sommets(std::unique_ptr<Plateau> plateau) :
    m_traite(false),
    m_distance(0),
    m_plateau(std::move(plateau))
{}

Sommets::Sommets(const Sommets& s) :
    m_traite(s.m_traite),
    m_distance(s.m_distance),
    m_plateau(s.m_plateau)
{}

Sommets::Sommets(Sommets&& s) :
    m_traite(std::move(s.m_traite)),
    m_precedent(s.m_precedent),
    m_distance(std::move(s.m_distance)),
    m_plateau(std::move(s.m_plateau))
{}

Sommets& Sommets::operator=(const Sommets& s)
{
    m_traite = s.m_traite;
    m_precedent = s.m_precedent;
    m_distance = s.m_distance;
    m_plateau = s.m_plateau;
    return *this;
}

Sommets& Sommets::operator=(Sommets&& s)
{
    if(&s != this) {
        m_traite = std::move(s.m_traite);
        m_precedent = std::move(s.m_precedent);
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



void Sommets::creer_lien(const std::shared_ptr<Sommets>& sommet_A, const std::shared_ptr<Sommets>& sommet_B, int poids){
    sommet_A->m_chemin_voisins.push_back({sommet_B, poids});
    sommet_B->m_chemin_voisins.push_back({sommet_A, poids});
}

std::vector<std::shared_ptr<Sommets>> Sommets::get_voisins() const
{
    std::vector<std::shared_ptr<Sommets>> res;
    for(const Lien& link: m_chemin_voisins){
        assert(link.voisin.lock().get() != nullptr);
        res.push_back(link.voisin.lock());
    }
    return res;
}


//
//
//     TESTS UNITAIRES
//
//


void Sommets::test(){
    Plateau plateau_test = Plateau("data/niveaux/niveau_0.rh");
    //test constructeur
    std::shared_ptr<Sommets> noeud_test = std::make_shared<Sommets>(std::make_unique<Plateau>("data/niveaux/niveau_0.rh"));
    noeud_test->m_precedent = noeud_test;
    assert(*noeud_test->get_plateau() == plateau_test);
    assert(noeud_test->m_traite == false);
    assert(noeud_test->m_distance == 0);
    assert(noeud_test->m_chemin_voisins.empty());
    std::cout << "Constructeur Sommets OK.\n";

    //test sur la génération des voisins
    std::vector<std::unique_ptr<Plateau>> nouveaux_plateaux = noeud_test->generer_voisins();
    assert(nouveaux_plateaux.empty() != true);
    for(std::unique_ptr<Plateau>& nouveau_plateau : nouveaux_plateaux){
        std::shared_ptr<Sommets> nouveau_sommet = std::make_shared<Sommets>(std::move(nouveau_plateau));
        assert(nouveau_sommet != nullptr);
        nouveau_sommet->m_precedent = nouveau_sommet;
        noeud_test->creer_lien(nouveau_sommet, noeud_test, 1);
        nouveau_sommet->m_precedent = noeud_test;
    }
    std::cout << "Génération des voisins d'un sommet OK.\n";
}