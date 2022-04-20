#include <iostream>
#include <array>

/*
 * Classe representant un point.
 */

class Point {
private:
    /*
    * Un point est represente par un array des 784
    * pixels de l'image ainsi que son label. Un pixel
    * est un double compris entre 0 (noir) et 1 (blanc).
    */
    double coords[784];
    unsigned int label;
public:
    // Constructeur de la classe
    Point(double arr[784], unsigned int l){
        for(int i = 0; i < 784; ++i)  coords[i] = arr[i];
        label = l;   
    }

    // Renvoie la i-eme coordonnee du point
    double get(unsigned int index) const {
        return coords[index];
    }

    // Renvoie le label du point.
    unsigned int get_label() const {
        return label;
    }

    // Renvoie la distance euclidienne (au carre) a un autre point
    double distance(const Point& pt) const {
        double dist = 0;
        for (int i = 0; i < 784; ++i) {
            double d = get(i) - pt.get(i);
            dist += d*d;
        }
        return dist;
    }

    // Overload l'operateur << afin de permettre 
    // un joli affichage de l'image dans un terminal
    friend std::ostream& operator<<(std::ostream& out, const Point& pt){
        for (size_t i = 0; i < 784; ++i) {
            out << (pt.coords[i] > 0 ? "#" : " ");
            if (i%28==0) out << "\n";
        }
        out << "Label : " << pt.label;
        return out;
    } 
};


