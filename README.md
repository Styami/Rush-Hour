# Rush Hour
> BOMARD Stéphane \
> BERTHOLON Noah

Fonctionnement:
    Chaque noeud du graph sera un plateau.
    On itère sur chaque noeud, si un déplacement de bloc est possible sur l'un des noeuds
        ce dernier devra renvoyer une nouvelle instance de plateau, dans la nouvelle configuration
    La structure du graph sera responsable de la durée de vie des pointeurs sur plateau.

Conventions et bonne pratique:

Fichier:
    hpp:
        #ifndef __NOM_FICHIER__
        #define __NOM_FICHIER__
        
        class NomFichier {
        public:    
            /* ... */
        protected:
            /* ... */
        private:
            /* ... */
        };

        #endif
    

Classe:
    m_nom_long : Variable membre
    s_nom_long : Variable static
    foo_long : Fonction membre

    Fonction de test:
        static bool test();

        Renvoie vrai si le test a réussi, faux sinon.
        Doit cerr des éléments de débuggage
    
    Penser aux const
    Penser aux références