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
        mutable int distance;
        Sommets(std::unique_ptr<Plateau> element);

        Sommets& operator=(Sommets&& s);

        
        void link(Sommets* som, int poids);
        std::vector<std::unique_ptr<Plateau>> generer();


        Plateau* get_element() const;
        std::vector<const Sommets*> get_node_neighbours() const;
        static void test();

        ~Sommets();
        
};
#endif