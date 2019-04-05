# Projet C++

Le projet consiste à parser un csv avec des stations de métro, un autre csv avec les connections, mettre les données dans une table de hachage, et résoudre un Djikstra.

Les stations étant identifiées par leur ID, les étudiants qui auront fini peuvent implémenter une distance de Levensthein afin d'appeler la station de métro par son nom en résistant aux erreurs.

Le sujet peut être rapidement réecrit avec d'autres formats de fichiers, ou d'autres alogirthmes du plus court chemin.

L'accent au point de vue pédagogique est mis sur une certaine contextualisation en entreprise avec un senior qui serait passé faire l'interface, et où il n'ont plus qu'a parser les csv et implémenter l'algo.

<hr>

Le projet à été fourni en 2018 avec plusieurs fichiers objets pour plusieurs systèmes.

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
