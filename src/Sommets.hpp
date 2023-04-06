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
            const Sommets& voisin;
            int poids;
        };
        std::shared_ptr<Plateau> m_plateau;
        std::vector<Lien> m_chemin_voisins;

    public:
        bool m_traite;
        int m_distance;

        Sommets(std::unique_ptr<Plateau> plateau);
        ~Sommets();

        
        void link(Sommets& som, int poids);
        std::vector<std::unique_ptr<Plateau>> generer_voisins();


        std::shared_ptr<Plateau> get_plateau() const;
        std::vector<std::weak_ptr<Sommets>> get_voisins() const;
        static void test();

        
};
#endif