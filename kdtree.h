#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <vector>
#include "point.h"

/*
 * Classe representant un arbre KD.
 * Cet arbre contient des noeuds et
 * des liens entre ces noeuds via des
 * pointeurs.
 */
class Kdtree {
private:
    /*
     * Structure definissant un noeud.
     * Celui-ci contient un point et
     * deux pointeurs vers les noeuds
     * fils a gauche et a droite.
     */
    struct KDNode
    {
        Point point;
        KDNode* left;
        KDNode* right;

        // Renvoie la i-eme coordonnee du point associe au noeud
        unsigned int get(unsigned int index) const {
            return point.get(index);
        }

        // Renvoie la distance euclidienne (au carre) entre deux noeuds
        double distance(const Point& pt) const {
            return point.distance(pt);
        }

        // Renvoie le label du point associe au noeud
        unsigned int get_label() const {
            return point.get_label();
        }

        // Constructeur du KDNode
        KDNode(const Point& pt) : point(pt), left(nullptr), right(nullptr) {}
    };


    KDNode* root = nullptr; // Pointeur vers le noeud root
    std::vector<KDNode> nodes; // Vecteur contenant les noeuds
    std::vector<std::pair<double, KDNode*>> best_list; // Vecteur contenant les K noeuds les plus proches
                                                       // ainsi que leurs distances au noeud recherche


    /*
     * Structure permettant la comparaison de 2 noeuds
     * par rapport a une coordonnee. 
     */
    struct node_comparison {
        unsigned int index;
        node_comparison(unsigned int axis) : index(axis) {}
        bool operator()(const KDNode &n1, const KDNode &n2) {
            return (n1.get(index) < n2.get(index));
        }
    };

    // Fonction construisant l'arbre KD recursivement
    KDNode* make_tree(unsigned int begin, unsigned int end, unsigned int axis) {
        // S'il n'y a pas de points a inserer on ne renvoie que le pointeur nul
        if (end <= begin)
            return nullptr;
        unsigned int n = begin + (end - begin)/2; // L'indice median
        auto i = nodes.begin();

        // nth_element permet de placer le point median au milieu du vecteur
        // tout en placant les points plus petits a sa gauche et les plus grands
        // a sa droite mais ceux-ci ne sont pas tries (ce qui n'importe pas dans notre cas).
        // Cela nous permet d'eviter la complexite d'un sort.
        std::nth_element(i + begin, i + n, i + end, node_comparison(axis));
        axis = (axis + 1) % 784;
        nodes[n].left = make_tree(begin, n, axis); // On cree recursivement un sous-arbre avec les points plus
        nodes[n].right = make_tree(n + 1, end, axis); // petits a gauche et les plus grands a droite.
        return &nodes[n]; // On renvoie un pointeur vers le node median.
    }

    // Modifie la liste best_list recursivement jusqu'a obtenir les K noeuds les plus proches du
    // point passe en argument
    void nearest(KDNode* root, const Point& pt, unsigned int index) {
        // Si l'on pointe vers rien, on arrete la recherche
        if (root == nullptr)
            return;

        double d = root->distance(pt);

        // Si le noeud actuel est plus proche que le dernier point de best_list,
        // il prend sa place
        if (best_list.back().second == nullptr || d < best_list.back().first) {
            std::pair<double, KDNode*> couple;
            couple.first = d;
            couple.second = root;
            best_list.back() = couple;
        }
        // On trie best_list pour que les points soient ordonnes par 
        // leur distance au point passe en argument
        std::sort(best_list.begin(), best_list.end());

        // Si le meilleur point a une distance nulle, alors on s'arrete
        // car le point est dans l'arbre
        if (best_list.front().first == 0)
            return;
        
        double dx = root->get(index) - pt.get(index);
        index = (index + 1) % 784;
        
        // On poursuit la recherche recursivement en descendant 
        // l'arbre en respectant sa structure
        nearest(dx > 0 ? root->left : root->right, pt, index);
        if (dx * dx >= best_list.back().first)
            return;
        nearest(dx > 0 ? root->right : root->left, pt, index);
    }

    // Renvoie l'entier apparaissant le plus de fois dans un vecteur d'entiers
    int most_occurences(std::vector<unsigned int> v){
        std::vector<int> res(10, 0);
        int index;
        for (int i = 0; i < v.size(); i++) {
            res[v[i]]++;
        }
        // Renvoie l'index du max
        index = std::distance(res.begin(), std::max_element(res.begin(), res.end()));
        return index;
    }

public:
    Kdtree() {}

    // Constructeur prenant en parametre des iterateurs sur
    // notre dataset et creant l'arbre grace a make_tree
    template<typename iterator>
    Kdtree(iterator begin, iterator end) : nodes(begin, end) {
        root = make_tree(0, nodes.size(), 0);
    }

    // Renvoie la prediction par rapport au point passe 
    // en parametre en utilisant les K plus proche voisins
    int predict(const Point& pt, unsigned int k) {
        if (root == nullptr)
            throw std::logic_error("L'arbre est vide");
        best_list.clear();

        // On initialise la liste avec les 3 memes valeurs
        // dont la distance est 1000 afin que les premieres
        // distance calculees soient necessairement mises
        // dans la liste.
        for (int i = 0; i < k; i++)
        {
            std::pair<double, KDNode*> rootpair;
            rootpair.first = 1000.0;
            rootpair.second = nullptr;
            best_list.push_back(rootpair);
        };

        // Lance la methode nearest pour ce point
        nearest(root, pt, 0);

        // Cree un vecteur des labels de best_list
        std::vector<unsigned int> res;
        for (int i = 0; i < k; i++)
        {
            res.push_back(best_list[i].second->get_label());
        }
        
        // Renvoie le label le plus frequent dans les K points
        // les plus proches
        return most_occurences(res); 
    }
};


