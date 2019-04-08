# Projet C++

Ce projet à été conçu afin de faire travailler les étudiants sur la STL. La position est de les forcer à respecter une interface pour concevoir un moteur complet d’une application qui peuvent valoriser s’il le veulent.

Concrètement, on leur demande d’écrire un parseur csv, et d’implémenter un algorithme de pathfinding. Celui-ci peut être modifié au fil des années, par exemple Dijkstra, A*, etc.

Un jeu de données test et une base de données RATP sont mises à disposition. La base RATP provient de leur base de données GTFS, modifiée/simplifiée afin d’obtenir un graphe orienté en tant qu’exemple jouet, afin de limiter la gestion d’erreur nécessaire pour les algorithmes de parcours.

On leur impose une interface, ils doivent donc créer une classe network héritant de Generic_mapper avec les fonctions read_stations et read_connections surchargées. Elles doivent rester protected. Ensuite les étudiants implémentent leur algorithme de pathfinding dans compute_travel, et un affichage facilement compréhensible avec compute_and_display_travel.

<hr>

Le projet consiste à parser un csv avec des stations de métro, un autre csv avec les connections, mettre les données dans une table de hachage, et résoudre un Djikstra.

Les stations étant identifiées par leur ID, les étudiants qui auront fini peuvent implémenter une distance de Levensthein afin d'appeler la station de métro par son nom en résistant aux erreurs.

Le sujet peut être rapidement réecrit avec d'autres formats de fichiers, ou d'autres alogirthmes du plus court chemin.

L'accent au point de vue pédagogique est mis sur une certaine contextualisation en entreprise avec un senior qui serait passé faire l'interface, et où il n'ont plus qu'a parser les csv et implémenter l'algo.

<hr>

Des exemples de résolution sont postées sur la page d'accueil Moodle du projet. Le fichier objet Grade.o est fourni afin qu’ils vérifient leur code.

Les étudiant ont tendance à s'emmêler les pinceaux et à perdre du temps avec Grade.o et Grade.hpp. Il doivent assimiler que c’est simplement une aide et que ce n’est pas nécessaire dans leur projet. Une erreur souvent rencontrée avec Grade est l’utilisation de l’objet statique Grade::evaluate, dans un exécutable ou le lien n’as pas été correctement fait avec Grade.o. Dans ce cas, ils peuvent avoir un résultat “seems ok” alors que leur code n’est pas comparé.

Lors de l’édition de liens, il est impératif de lier Grade.o avant main.o, car l’objet statique Grade::evaluate est crée dans le header Grade.hpp, erreur parfois commise par les étudiants.

Enfin, la fonction compute_travel présente dans Grade.o effectue ses opérations et renvoie un plus court chemin dans un certain format, que les étudiants doivent respecter s’il veulent obtenir un résultat positif avec la comparaison de la correction. Il semble que cela n’est en rien obligatoire et des étudiants peuvent implémenter parfaitement la consigne tout en ayant un résultat différent.

<hr>

Le projet à été fourni en 2018 et 2019 avec plusieurs fichiers objets pour plusieurs systèmes.

1/ clang_macos : Compilé avec g++, qui est un symlink du compilateur clang par défaut, avec les flags -std=c++11 -Wall -Wextra -pedantic -O3, sur un Macbook Pro sur High Sierra.

2/ g++7_macos : Compilé avec g++-7 avec les flags -std=c++11 -Wall -Wextra -pedantic -O3, sur un Macbook Pro sur High Sierra.

3/ linux : Compilé avec g++ 6.3 avec les flags -std=c++11 -Wall -Wextra -Werror -pedantic -pedantic-errors -O3,sur un Dell OptiPlex 7040 sur Debian 9.

Pas de version Windows, les étudiant sont invités à télécharger Ubuntu sur le Windows Store et suivre la procédure pour activer le mode développeur. Sur ce dernier, l'objet compilé sous linux semble fonctionner correctement.

Le fichier objet peut ne pas fonctionner correctement :
 - Sur l'OS ou le compilateur des ordinateurs à Esclangon (à vérifier)
 - Sur des versions spéciales de Mac OS ou de compilateur sous Mac
 - Sur tout le reste

<hr>

La version v2 est la version actuelle du projet sur lequel à été conçu le sujet. Les fonctions virtuelles protégées, et les fonctions qui ne contiennent qu'un lancer d'exception dans le corps sont des constructions pédagogiques.

Le projet remplaçant des anciens TP le but est de les accompagner au début et de progressivement les laisser faire.

<hr>

Une solution compilée est fournie, notamment en incorporant les données dans des littéraux de std::string afin d'encapsuler la base de données et la solution dans un seul fichier objet. Par convention, un std::string ne doit pas dépasser 2^16 éléments, mais les compilateurs testés géraient ce fait.

<hr>
Liste des applications nécéssaires à la compilation des éléments du projet :
  - make
  - pdflatex
  - inkscape
  - zip
  - g++

Liste des application nécéssaires à la mise à jour de la base de données :
  - MATLAB 2018+
