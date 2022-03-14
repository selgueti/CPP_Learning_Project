# Se familiariser avec l'existant

## A- Exécution

Compilez et lancez le programme.

Allez dans le fichier `tower_sim.cpp` et recherchez la fonction responsable de gérer les inputs du programme.
Sur quelle touche faut-il appuyer pour ajouter un avion ?
 - Pour ajouter un avion il faut appuyer sur la touche 'C'.

Comment faire pour quitter le programme ?
 - Pour quitter le programme on peut appuyer sur 'X' ou 'Q'.

A quoi sert la touche 'F' ?
 - La touche 'F' sert à passer en mode plein écran.

Ajoutez un avion à la simulation et attendez.
Que est le comportement de l'avion ?
 - L'avion se pose et redécole, en boucle.

Quelles informations s'affichent dans la console ?
 - Différents logs quand l'avion arrive, se pose et redécole. 

Ajoutez maintenant quatre avions d'un coup dans la simulation.
Que fait chacun des avions ?
 - Trois se pose directement et redécole, alors que le 4ème attend un terminal de libre. Les avions ne se pose pas dans l'ordre d'ajout mais lorsqu'un terminal est disonible c'est l'avion "le plus apte à se poser" qui se pose.

## B- Analyse du code

Listez les classes du programme à la racine du dossier src/.
Pour chacune d'entre elle, expliquez ce qu'elle représente et son rôle dans le programme.
 - Aircraft        : Représente un avion
 - Airporttype     : Caractéristiques d'un aéroport
 - Airport         : Représente un aéroport avec ses terminaux et sa tour de contrôle
 - Terminal        : Représente un terminal
 - TowerSimulation : Moteur de la simulation
 - Tower           : Represente une tour de contrôle
 - Waypoint        : Représente un point de passage


Pour les classes `Tower`, `Aircaft`, `Airport` et `Terminal`, listez leurs fonctions-membre publiques et expliquez précisément à quoi elles servent.

 - Aircraft
   - get_flight_num : renvoie le numéros de l'avion
   - distance_to    : renvoie la distance entre l'avion et le point passé en paramètre 
   - display        : affiche l'avion
   - move           : déplace l'avion d'un cran

 - Airport
   - get_tower : renvoie la Tower associé
   - display   : affiche l'aeroport
   - move      : fait avancer la simulation des terminaux d'un cran

 - Terminal
   - in_use         : renvoie True si le Terminal est occupé
   - is_servicing   : renvoie True si le Terminal est reservé
   - assign_craft   : assigne un nouvelle avion au Terminal
   - start_service  : commence un cycle de service
   - finish_service : passe le Terminal en mode disponible
   - move           : avance d'un cran dans le cycle de service

 - Tower
   - get_instructions    : Renvoie une queue des points d'arrêt pour pouvoir attérir
   - arrived_at_terminal : Modifie l'aéroport pour signaler que cette Tower est occupé par un avion


Réalisez ensuite un schéma présentant comment ces différentes classes intéragissent ensemble.

# TODO faire le schema UML

Quelles classes et fonctions sont impliquées dans la génération du chemin d'un avion ?
 - Les classes et fonctions qui sont impliquées dans la génération du chemin d'un avion sont:
    - Aircraft(move, turn_to_waypoint) et Waypoint

Quel conteneur de la librairie standard a été choisi pour représenter le chemin ?
Expliquez les intérêts de ce choix.
 - Il s'agit d'une std::deque\<Waypoint\>, ce choix est interessant car il offre des compléxité en O(1) pour l'ajout et la suppression en tête/fin.

## C- Bidouillons !

1) Déterminez à quel endroit du code sont définies les vitesses maximales et accélération de chaque avion.
Le Concorde est censé pouvoir voler plus vite que les autres avions.
Modifiez le programme pour tenir compte de cela.
 - Les vitesses maximales et accélération de chaque avion sont définies dans la fonction
```cpp 
inline void init_aircraft_types();
```

2) Identifiez quelle variable contrôle le framerate de la simulation.
 - Le framerate de la simulation est contrôlé par la variable `GL::ticks_per_sec`.

Ajoutez deux nouveaux inputs au programme permettant d'augmenter ou de diminuer cette valeur.
```cpp
GL::keystrokes.emplace('u', []() { GL::ticks_per_sec += DEFAULT_TPS_INCREMENT;}); 
GL::keystrokes.emplace('d', []() { GL::ticks_per_sec -= DEFAULT_TPS_INCREMENT;});
```

Essayez maintenant de mettre en pause le programme en manipulant ce framerate. Que se passe-t-il ?\
 - si Gl::ticks_per_sec vaut 0 on observe une floating point exception car dans la fonction `void timer(const int step)` on peut observer l'appelle de fonction suivant : `glutTimerFunc(1000u / ticks_per_sec, timer, step + 1);`
 - de plus Gl::ticks_per_sec étant un unsigned int, (0 - 1) == max unsigned int donc cela va augmenté le framerate au lieu de le diminué

Ajoutez une nouvelle fonctionnalité au programme pour mettre le programme en pause, et qui ne passe pas par le framerate.

3) Identifiez quelle variable contrôle le temps de débarquement des avions et doublez-le.
 - Le temps de débarquement des avions est contrôlé par la varriable SERVICE_CYCLES .

4) Lorsqu'un avion a décollé, il réattérit peu de temps après.
Faites en sorte qu'à la place, il soit retiré du programme.\
Indices :\
A quel endroit pouvez-vous savoir que l'avion doit être supprimé ?\
 - Dans la fonction `Aircraft::move()`

Pourquoi n'est-il pas sûr de procéder au retrait de l'avion dans cette fonction ?
 - Car on modifie la structure alors qu'on la parcours à l'aide d'une boucle foreach.

A quel endroit de la callstack pourriez-vous le faire à la place ?\
 - Dans la fonction `timer()`.

Que devez-vous modifier pour transmettre l'information de la première à la seconde fonction ?
 - Il faut changer la signature de `move()` pour retourner l'information.

5) Lorsqu'un objet de type `Displayable` est créé, il faut ajouter celui-ci manuellement dans la liste des objets à afficher.
Il faut également penser à le supprimer de cette liste avant de le détruire.
Faites en sorte que l'ajout et la suppression de `display_queue` soit "automatiquement gérée" lorsqu'un `Displayable` est créé ou détruit.
Pourquoi n'est-il pas spécialement pertinent d'en faire de même pour `DynamicObject` ?

6) La tour de contrôle a besoin de stocker pour tout `Aircraft` le `Terminal` qui lui est actuellement attribué, afin de pouvoir le libérer une fois que l'avion décolle.
Cette information est actuellement enregistrée dans un `std::vector<std::pair<const Aircraft*, size_t>>` (size_t représentant l'indice du terminal).
Cela fait que la recherche du terminal associé à un avion est réalisée en temps linéaire, par rapport au nombre total de terminaux.
Cela n'est pas grave tant que ce nombre est petit, mais pour préparer l'avenir, on aimerait bien remplacer le vector par un conteneur qui garantira des opérations efficaces, même s'il y a beaucoup de terminaux.\
Modifiez le code afin d'utiliser un conteneur STL plus adapté. Normalement, à la fin, la fonction `find_craft_and_terminal(const Aicraft&)` ne devrait plus être nécessaire.

## D- Théorie

1) Comment a-t-on fait pour que seule la classe `Tower` puisse réserver un terminal de l'aéroport ?
Car la fonction `reserve_terminal` de airport est privée et que seule la classe `Tower` en est friendly.

2) En regardant le contenu de la fonction `void Aircraft::turn(Point3D direction)`, pourquoi selon-vous ne sommes-nous pas passer par une réference ?
 - Car le `Point3D` en question n'est pas un objet (crée par new) mais est le résultat d'un calcul, et par conséquent n'a pas de référence.
Pensez-vous qu'il soit possible d'éviter la copie du `Point3D` passé en paramètre ?
 - Non pour la même raison.

## E- Bonus

Le temps qui s'écoule dans la simulation dépend du framerate du programme.
La fonction move() n'utilise pas le vrai temps. Faites en sorte que si.
Par conséquent, lorsque vous augmentez le framerate, la simulation s'exécute plus rapidement, et si vous le diminuez, celle-ci s'exécute plus lentement.

Dans la plupart des jeux ou logiciels que vous utilisez, lorsque le framerate diminue, vous ne le ressentez quasiment pas (en tout cas, tant que celui-ci ne diminue pas trop).
Pour avoir ce type de résultat, les fonctions d'update prennent généralement en paramètre le temps qui s'est écoulé depuis la dernière frame, et l'utilise pour calculer le mouvement des entités.

Recherchez sur Internet comment obtenir le temps courant en C++ et arrangez-vous pour calculer le dt (delta time) qui s'écoule entre deux frames.
Lorsque le programme tourne bien, celui-ci devrait être quasiment égale à 1/framerate.
Cependant, si le programme se met à ramer et que la callback de glutTimerFunc est appelée en retard (oui oui, c'est possible), alors votre dt devrait être supérieur à 1/framerate.

Passez ensuite cette valeur à la fonction `move` des `DynamicObject`, et utilisez-la pour calculer les nouvelles positions de chaque avion.
Vérifiez maintenant en exécutant le programme que, lorsque augmentez le framerate du programme, vous n'augmentez pas la vitesse de la simulation.

Ajoutez ensuite deux nouveaux inputs permettant d'accélérer ou de ralentir la simulation.
