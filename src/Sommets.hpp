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
            Sommets* voisin;
            int poids;
        };
        std::unique_ptr<Plateau> m_element;
        std::vector<Lien> m_chemin_voisins;
    public:
        bool m_traite;
        Sommets(std::unique_ptr<Plateau> element);

        
        void link(Sommets* som, int poids);
        std::vector<std::unique_ptr<Plateau>> generer();


        const Plateau& get_element() const;
        static void test();

        ~Sommets();
        
};
#endif