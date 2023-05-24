#ifndef __EMPTY_H
#define __EMPTY_H

#include <iostream>
#include "Eigen/Eigen"
#include <fstream>
#include "map"
using namespace std;
using namespace Eigen;


namespace ProjectLibrary
{
constexpr double max_tolerance(const double& x, const double& y) //funzione per calcolare il max come costant expression
{
  return x > y ? x : y;
}

struct Point
{
    double x;
    double y;  //attributi
    unsigned int id;

    static constexpr double geometricTol = 1.0e-12;  //tolleranze
    static constexpr double geometricTol_Squared = max_tolerance(Point::geometricTol * Point::geometricTol,
                                                                 numeric_limits<double>::epsilon());

    Point(const double& x=0.0,
          const double& y=0.0,
          const unsigned int& id=0):  //costruttore+costruttore vuoto
      x(x), y(y), id(id)
    {
    }
};

struct Segmento
{
    unsigned int id_iniz;
    unsigned int id_fin;  //attributi
    unsigned int id;
    double lunghezza;

    Segmento(const unsigned int& id_iniz=0,
          const unsigned int& id_fin=0,
          const unsigned int& id=0,
          const double& lunghezza=0):  //costruttore+costruttore vuoto
      id_iniz(id_iniz), id_fin(id_fin), id(id), lunghezza(lunghezza)
    {
    }
};

class Triangolo
{
public:
    unsigned int id;
    Vector3i vertices;  //attributi
    Vector3i edges;
    double area;

    Triangolo(const unsigned int& id=0,
          const Vector3i& vertices=Vector3i::Zero(),
          const Vector3i& edges=Vector3i::Zero(), double area=0.0):  //costruttore+costruttore vuoto
      id(id), vertices(vertices), edges(edges), area(area)
    {
    }
};

inline double normSquared(const double& x, const double& y)  //definizione degli operatori usando la tollerenza
{
  return x * x + y * y;
}
/*
inline bool operator==(const Triangolo& t1, const Triangolo& t2)
{
  return (abs(t1.area - t2.area) <=
          Point::geometricTol * Point::geometricTol *
          max(t1.area, t2.area));
}
inline bool operator!=(const Triangolo& t1, const Triangolo& t2)
{
  return !(t1 == t2);
}*/

inline bool operator==(const Point& p1, const Point& p2)
{
  return (normSquared(p1.x - p2.x, p1.y - p2.y) <=
          Point::geometricTol * Point::geometricTol *
          max(normSquared(p1.x, p1.y), normSquared(p2.x, p2.y)));
}
inline bool operator!=(const Point& p1, const Point& p2)
{
  return !(p1 == p2);
}

inline ostream& operator<<(ostream& os, const Point& p2)
{
  os << p2.id;
  return os;
}
//OPERATORI PER I PUNTI: ID
inline bool operator>(const Point& p1, const Point& p2)
{
  return (p1.id>p2.id); //ordino gli id in modo crescente
}

inline bool operator<=(const Point& p1, const Point& p2)
{
  return !(p1 > p2);
}

/*inline bool operator>(const Point& p1, const Point& p2)
{
  return p1.x > p2.x + Point::geometricTol * max(p1.x, p2.x); //da mettere nel progetto
}

inline bool operator<=(const Point& p1, const Point& p2)
{
  return !(p1 > p2);
}*/

//OPERATORI PER I TRIANGOLI: AREE
inline bool operator>(const Triangolo& t1, const Triangolo& t2)  //nel merge sort dobbiamo ordinare in base alle aree
{
  return t1.area < t2.area + Point::geometricTol * max(t1.area, t2.area); //metto il < perchè voglio l'ordinamento dell aree in modo DECRESCENTE
}

inline bool operator<=(const Triangolo& t1, const Triangolo& t2)
{
  return !(t1 > t2);
}

//OPERATORI PER I SEGMENTI: LATO
inline bool operator>(const Segmento& t1, const Segmento& t2)  //per cercare il lato più lungo
{
  return t1.lunghezza < t2.lunghezza + Point::geometricTol * max(t1.lunghezza, t2.lunghezza);
}

inline bool operator<=(const Segmento& t1, const Segmento& t2)
{
  return !(t1 > t2);
}

struct TriangularMesh  //struct posso associare metodi e attributi, a differenza della classe ha tutto pubblico, posso accedere a tutti gli attributi della struct tramite il punto
{
    //celle0D
    unsigned int NumberCell0D = 0;
    vector<Point> Cell0D_Id_coord;
    std::map<unsigned int, list<unsigned int>> Cell0DMarkers = {};

    //celle 1D
    unsigned int NumberCell1D = 0;
    vector<Segmento> Cell1D_segmenti;  //id, vertci e i lati
    std::map<unsigned int, list<unsigned int>> Cell1DMarkers = {};

    //celle 2D
    unsigned int NumberCell2D = 0;
    vector<Triangolo> Cell2D_triangoli;
};

bool ImportCell0Ds(TriangularMesh& mesh, string nome_file);  //vertici

bool ImportCell1Ds(TriangularMesh& mesh, string nome_file);  //segmenti

bool ImportCell2Ds(TriangularMesh& mesh, string nome_file);  //triangoli

double Lunghezza(vector<Point> punti,Segmento segm);

double Area(vector<Point> punti, Triangolo trian);  //come metodo di triangolo non riconosce TriangularMesh

void puntoMedio(vector<Point>& punti, Segmento segm, unsigned int& numPunti); //ci vogliono &?Solo se ci interessa fare delle modifiche

template<typename T>  //mergesort implementato nell'hpp perchè con i template quando si dichiara una funzione va definita
void Merge(vector<T>& v,
           const unsigned int& sx,
           const unsigned int& cx,
           const unsigned int& dx){
    unsigned int i=sx;
    unsigned int j=cx+1;
    vector <T> b;
    while ((i<=cx)&&(j<=dx)){
        if (v[i]<=v[j]){
            b.push_back(v[i]);
            i=i+1;
        }
        else{
            b.push_back(v[j]);
            j=j+1;
            }
    }
    for (unsigned int s=i;s<=cx;s++){
        b.push_back(v[s]);
    }
    for (unsigned int t=j;t<=dx;t++){
        b.push_back(v[t]);
        }
    for (unsigned int k=sx;k<=dx;k++){
        v[k]=b[k-sx];
    }

  }
//algoritmo del merge sort come nelle slide
template<typename T>
void MergeSort(vector<T>& v, //prende in input una referenaza non costante, può modificare il vettore
               const unsigned int& sx,
               const unsigned int& dx){
    if(sx<dx){
        int cx;
        cx=(sx+dx)/2;
        MergeSort(v,sx,cx);
        MergeSort(v,cx+1,dx);
        Merge(v,sx,cx,dx);
        }
}
}

#endif // __EMPTY_H
