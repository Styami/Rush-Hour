# Rush Hour
> BOMARD Stéphane \
> BERTHOLON Noah


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