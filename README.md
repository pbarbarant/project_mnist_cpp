# project_mnist_cpp
Projet de C++ pour le cours de 2e année (2022) à l'ENSAE 
realisé par Pierre-Louis BARBARANT et Tanguy ENEZ.


Ce programme permet de realiser de la prédiction de chiffres (MNIST)
en utilisant l'algorithme KD-tree (voir https://en.wikipedia.org/wiki/K-d_tree
pour plus de précisions sur le fonctionnement de l'algorithme) appliqué à
la recherche des plus proches voisins.

Il permet plus précisément de laisser l'utilisateur voir le fonctionnement de
la prédiction et de tester les performances sur une large plage de valeurs
issues des données de validation ainsi que de choisir le nombre de plus
proches voisins a utiliser dans la prédiction.

On notera que nous avons obtenu les meilleurs résultats sur toutes les
données d'entraînement avec K=3 (pour une accuracy de ~97.5%).

Les fichiers main.cpp, kdtree.h et point.h constituent le cœur de l'algorithme
et ont été ecrits par nos soins. Seul le fichier mnist.h servant a charger les
données MNIST provient d'une source externe.

Le projet a été compilé grâce a g++ (Rev6, Built by MSYS2 project) 11.2.0
sous Windows 11.
