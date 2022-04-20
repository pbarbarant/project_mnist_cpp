/*
 * 31/01/2022 - ENSAE
 * Projet de C++ realise par Pierre-Louis BARBARANT et Tanguy ENEZ.
 * 
 * Ce programme permet de realiser de la prediction de chiffres (MNIST)
 * en utilisant l'algorithme KD-tree (voir https://en.wikipedia.org/wiki/K-d_tree
 * pour plus de precision sur le fonctionnement de l'algorithme) applique a 
 * la recherche des plus proches voisins.
 * 
 * Il permet plus precisement de laisser l'utilisateur voir le fonctionnement de
 * la prediction et de tester les performances sur une large plage de valeurs
 * issues des donnees de validation ainsi que de choisir le nombre de plus
 * proches voisins a utiliser dans la prediction.
 * 
 * On notera que nous avons obtenu les meilleurs resultats sur toutes les
 * donnees d'entrainement avec K=3 (pour une accuracy de ~97.5%).
 * 
 * Les fichiers main.cpp, kdtree.h et point.h constituent le coeur de l'algorithme
 * et ont ete ecrits par nos soins. Seul le fichier mnist.h servant a charger les
 * donnees MNIST provient d'une source externe.
 * 
 * Le projet a ete compile grace a g++ (Rev6, Built by MSYS2 project) 11.2.0
 * sous Windows 11.
 */

// Macros du loader mnist.h
#define USE_MNIST_LOADER
#define MNIST_DOUBLE

#include <iostream>
#include <vector>
#include "mnist.h"
#include "kdtree.h"

using namespace std;

// On definit notre propre type
using dataset = vector<Point>;


// Transformation des donnees
void transformData(mnist_data* rawData, unsigned int cnt, dataset &data) {
    for (int idx = 0; idx < cnt; idx++) {
        double tmp_coords [784];
        for (int i = 0; i < 28; i++) {
            for (int j = 0; j < 28; j++) {
                tmp_coords[j + 28*i] = rawData[idx].data[i][j];
            }
        }
        data.push_back(Point(tmp_coords, rawData[idx].label));
    }
}

int main(int argc, char **argv) {

    // Chargement des donnees depuis les fichiers
    mnist_data *rawTrainData;
    unsigned int trainCnt;
    mnist_data *rawTestData;
    unsigned int testCnt;
    mnist_load("train-images-idx3-ubyte", "train-labels-idx1-ubyte", &rawTrainData, &trainCnt);
    mnist_load("t10k-images-idx3-ubyte", "t10k-labels-idx1-ubyte", &rawTestData, &testCnt);
    std::cout << "Donnees chargees !" << endl
        << "Taille des donnees d'entrainement : " << trainCnt << endl
        << "Taille des donnees de validation : " << testCnt << endl;

    // On convertit les donnees en type dataset
    dataset trainData;
    dataset testData;
    transformData(rawTrainData, trainCnt, trainData);
    transformData(rawTestData, testCnt, testData);
    std::cout << "Conversion des donnees terminee !" << endl;

    // Construction de l'arbre KD
    Kdtree mnist_tree(trainData.begin(), trainData.end());

    int test_len;
    int K;
    std::cout << "Choisissez le nombre d'images du dataset de validation a tester \n" 
            << "(on ne recommande pas plus de 100 pour des raisons de temps de calcul)" << endl;
    while (std::cout << "Entrez un nombre entre 0 et 10000 : " && !(std::cin >> test_len)) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input; please re-enter." << endl;
    }
    while (std::cout << "Choisissez le nombre K de plus proches voisins (entre 1 et 20) : " && !(std::cin >> K)) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input; please re-enter." << endl;
    }
    if (test_len < 0 || test_len > testCnt || K < 1 || K > 20)
        throw std::runtime_error("Les parametres sont incorrects");

    double res = 0;    
    std::cout << "\n==============================================\n\n  Exemple sur les "<< test_len 
                <<" premieres images de test" 
                << "\n\n=============================================="<< endl;

    // Lancement des predictions
    for (int idx = 0; idx < test_len; idx++)
    {
        std::cout << "Image originale numero "<< idx << " :" << testData[idx] << endl;
        std::cout << "Prediction : " << mnist_tree.predict(testData[idx], K) << "\n"<< endl;
        res = res + (mnist_tree.predict(testData[idx], K) == testData[idx].get_label() ? 1 : 0);
    }
    
    // On renvoie l'accuracy des predictions sur la plage de donnees
    // de test dont l'utilisateur s'est servi
    std::cout << "Accuracy : " << res * 100.0 / test_len << " %." << endl;


    std::free(rawTrainData);
    std::free(rawTestData);
    return 0;
}