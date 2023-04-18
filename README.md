# Rush Hour

BOMARD Stéphane \
BERTHOLON Noah

---
## Compilation & Exécutable
|Commande|Fonction|
|--|--|
`make` | compilera tous les exécutables
`make release` | compilera `main.cpp` : l'exécutable de rendu du projet
`make test` | compilera `test.cpp` : l'exécutable de test
`make clean` | supprime les exécutables
`make cleanall` | supprimer les exécutables et les fichiers objets
`bin/release`| lance l'exécutable de rendu du projet
`bin/test` | lance l'exécutable de test

### Release
Une application console s'affichera, dont le [fonctionnement](#Menu) est décrit plus loin.\
Après avoir choisi ce que l'on souhaite faire avec l'application, une animation de la résolution du niveau est censé s'afficher.

L'objectif de l'algorithme est de faire sortir le bloc jaune vif vers la droite de l'écran (les autres blocs ayant des teints plus ternes)

Il est vivement conseillé d'utiliser une console avec un thème sombre, et de laisser suffisament d'espace pour l'affichage afin d'éviter un flood de la console (14 lignes au moins)

### Test
Le code de test s'exécutera, et affichera les sources d'erreur ainsi que leur nombre. Si tout se passe bien, un message sera affiché.\
Un affichage d'un plateau sera également effectué après les tests.

## Organisation du code
```
Projet
├─ bin         : exécutables
├─ data
│   ├─ niveaux : niveaux générés par l'application
│   └─ test    : niveaux pour les fonctions de test
├─ obj         : fichier objet de la compilation
└─ src         : fichier sources
    └─ RPL     : "Bibliothèque" pour l'affichage console
```

## Dépannage

### Si l'animation est trop lente ou trop rapide
Changer les framerates dans\
`src/console_app.cpp > Window::Window(...) > m_window(..., framerate)`\
Ce n'est pas censé arriver, mais RPL n'a jamais été testé ailleurs que sur nos machines

---

## Contrôle 
|||
|--|--|
| <kbd>z</kbd> : monter  | <kbd>s</kbd> : descendre |
| <kbd>v</kbd> : valider | <kbd>m</kbd> : menu |
| <kbd>q</kbd> : quitter | |

## Menu
### Charger
Affiche la liste des fichiers contenu dans data/niveaux/\
Permet de charger ce niveau dans le programme et de générer une solution ainsi que son animation.

### Générer
Génère un nouveau niveau de manière aléatoire.\
On peut choisir la difficulté, cela change uniquement le nombre de bloc dans le niveau.\
Il ne serait pas difficile de générer des niveaux avec un nombre de blocs aléatoire jusqu'à ce que le nombre de coup pour résoudre le problème soit supérieur à un certain seuil. Nous avons privilégié les algorithmes principaux et l'ergonomie.\
Le niveau est automatiquement enregistré à la suite de ceux déjà existant.

---
## Sources
Nous avons principalement regardé la [documentation](https://en.cppreference.com/w/) 

Seul la génération des clefs de hashage a été reprise de [StackOverflow](https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x)

Et RPLconsole est basé sur du [code](https://forge.univ-lyon1.fr/p2002981/keskonmeh/-/blob/main/src/winTxt.cpp) qui nous avait été fourni durant un projet de L2, mais qui a été grandement transformé.