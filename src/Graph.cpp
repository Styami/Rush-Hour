#include "Graph.hpp"
#include <memory>

// m_racine n'est qu'une référence faible vers le sommet stocké dans la hashmap
Graph::Graph(std::shared_ptr<Sommets> node) :
    m_racine(node)
{
    m_hash_map.insert(std::pair<Plateau*, std::shared_ptr<Sommets>>(node->get_plateau().get(), node));
}

Graph::Graph()
{}

Graph::~Graph() {
    while(!m_file_noeud.empty())
        m_file_noeud.pop();

    // On rappel que m_racine est détenu par l'unordered map, et non pas par le graph
    // elle sera donc bien libérée par cette boucle
    for(auto &it:m_hash_map) {
        //if(it.second != nullptr) // inutile à priori
        it.second.reset();
    }

    m_hash_map.clear();
}

void Graph::charger_plateau(std::unique_ptr<Plateau> plateau) 
{
    std::shared_ptr<Sommets> s = std::make_shared<Sommets>(std::move(plateau));
    s->m_precedent = s;
    m_hash_map.clear();
    m_hash_map.insert(
        std::make_pair(
            s->get_plateau().get(),
            s
        )
    );
    m_racine = s;
}

void Graph::generer(std::shared_ptr<Sommets> sommet_courrant){
    std::vector<std::unique_ptr<Plateau>> plateaux_voisins = sommet_courrant->generer_voisins();

    for(std::unique_ptr<Plateau>& plateau : plateaux_voisins) {
        auto potentiel_sommet = m_hash_map.find(plateau.get());

        // le voisin est un noeud qui n'existait pas
        if(potentiel_sommet == m_hash_map.end())
        { 
            // On crée le sommet à partir du plateau
            std::shared_ptr<Sommets> nouveau_sommet = std::make_shared<Sommets>(std::move(plateau));
            nouveau_sommet->m_precedent = sommet_courrant;

            // On fait les liens avec le sommets actuel
            sommet_courrant->creer_lien(nouveau_sommet, sommet_courrant, 1);
            nouveau_sommet->m_distance = 1 + sommet_courrant->m_distance;
            m_file_noeud.push(nouveau_sommet);

            // On l'ajoute dans la hashmap
            m_hash_map.insert(std::pair<Plateau*, std::shared_ptr<Sommets>>(nouveau_sommet->get_plateau().get(), std::move(nouveau_sommet)));
        }
        // le voisin est un noeud déjà existant
        else // On fait les liens avec le sommets actuel
            sommet_courrant->creer_lien(potentiel_sommet->second, sommet_courrant,1);
    }
}

void Graph::init_parcours(bool generate_graph, std::vector<std::shared_ptr<Sommets>> solutions) 
{
    // On pousse les noeuds de base, soit c'est la racine du graph, soit c'est les noeuds solutions passé en paramètre
    if(generate_graph)
        m_file_noeud.push(m_racine.lock());
    else {
        for(std::shared_ptr<Sommets>& s : solutions)
            m_file_noeud.push(s);

        // Si le graph existe déjà, on reset les infos des sommets, mais pas les liens entres eux (ni leur contenu)
        for(auto& it: m_hash_map){
            it.second->m_traite=false;
            it.second->m_distance=0;
            it.second->m_precedent.reset();
        }
    }
}

void Graph::ajouter_voisin_file (std::shared_ptr<Sommets> sommet_courant, bool generate_graph)
{
    // On trouve les voisins du noeud à explorer
    if(generate_graph)
        generer(sommet_courant);
    else
        for(std::shared_ptr<Sommets>& s : sommet_courant->get_voisins()){
            if(!s->m_traite){
                m_file_noeud.push(s);
                s->m_distance = sommet_courant->m_distance + 1;
                s->m_precedent = sommet_courant;
            }
        }
}

std::vector<std::shared_ptr<Sommets>> Graph::parcours(bool parcours_complet, bool generer_graph, std::vector<std::shared_ptr<Sommets>> solutions) 
{
    init_parcours(generer_graph, solutions);

    std::vector<std::shared_ptr<Sommets>> res;
    std::shared_ptr<Sommets> current_noeud = nullptr;
    
    while(!m_file_noeud.empty())
    {
        current_noeud = m_file_noeud.front();
        m_file_noeud.pop();
        
        if(!current_noeud->m_traite){
            current_noeud->m_traite = true;

            // On ajoute les gagnants à la liste solution
            if(current_noeud->get_plateau()->est_gagnant()){
                res.push_back(current_noeud);
                if(!parcours_complet)
                    break;
            }
        
            ajouter_voisin_file(current_noeud, generer_graph);
        }
    }
    if(parcours_complet && !generer_graph)
        return {current_noeud};
    return res;
}

void Graph::test(){
    std::shared_ptr<Sommets> init_probleme = std::make_shared<Sommets>(std::move(std::make_unique<Plateau>("data/test/test_data_human_readable")));
    init_probleme->m_precedent = init_probleme;
    Graph graph_test = Graph(init_probleme);
    //test constructeur
    assert(graph_test.m_hash_map.empty());
    assert(graph_test.m_file_noeud.empty());

    //tests de parcours
    std::vector<Bloc> bloc_gagnant;
    bloc_gagnant.push_back(Bloc(4, 1, false, Orientation::horizontal));
    std::unique_ptr<Plateau> plateau_test_gagnant = std::make_unique<Plateau>(bloc_gagnant);
    std::vector<std::shared_ptr<Sommets>> solutions;
    //solutions.push_back(std::move(plateau_test_gagnant));
    solutions = graph_test.parcours(false, false, {std::make_shared<Sommets>(std::move(plateau_test_gagnant))});
    assert(solutions.size() == 1);
    assert(solutions[0]->get_plateau()->est_gagnant());

    //test où l'on cherche la solution d'un problème impossible
    bloc_gagnant.clear();
    bloc_gagnant.push_back(Bloc(4, 1, false, Orientation::vertical));
    plateau_test_gagnant = std::make_unique<Plateau>(bloc_gagnant);
    assert(!plateau_test_gagnant->est_gagnant()); //
    solutions = graph_test.parcours(true, true, {std::make_shared<Sommets>(std::move(plateau_test_gagnant))});
    assert(solutions.size() == 0);

    
    //test constructeur
    // Graph graph_test(new Sommets<int>(3));
    // assert(graph_test.m_noeud->get_plateau()==3);
    // assert(graph_test.m_hash_map[3]->get_plateau()==3);
    // graph_test.generer(graph_test.m_noeud,5);
    // assert(graph_test.m_noeud->m_voisins.size()==5);
    // assert(graph_test.m_file_noeud.size()==5);

}