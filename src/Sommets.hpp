#ifndef __SOMMETS__
#define __SOMMETS__
#include <stack>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <assert.h>
#include"plateau.hpp"
#include <memory>


class Sommets{
    private:
        struct Lien{
            std::weak_ptr<Sommets> voisin;
            int poids;
        };
        std::shared_ptr<Plateau> m_plateau;
        std::vector<Lien> m_chemin_voisins;

    public:
        bool m_traite;
        std::weak_ptr<Sommets> m_precedent;
        mutable int m_distance;

        Sommets(std::unique_ptr<Plateau> plateau, std::shared_ptr<Sommets> precedent);
        Sommets(const Sommets& s, std::shared_ptr<Sommets> precedent);
        Sommets(Sommets&& s);
        Sommets& operator=(const Sommets& s);
        Sommets& operator=(Sommets&& s);

        ~Sommets();

        
        void link(std::shared_ptr<Sommets> autre_sommet, std::shared_ptr<Sommets> sois_meme, int poids);
        std::vector<std::unique_ptr<Plateau>> generer_voisins();


        std::shared_ptr<Plateau> get_plateau() const;
        std::vector<std::shared_ptr<Sommets>> get_voisins() const;
        static void test();

        
};
#endif