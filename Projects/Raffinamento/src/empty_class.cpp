#include "empty_class.hpp"

namespace ProjectLibrary
{
//IMPORTO DA FILE
bool ImportCell0Ds(TriangularMesh& mesh, string nome_file)
{

  ifstream file;
  file.open(nome_file);

  if(file.fail())
    return false;

  list<string> listLines;
  string line;
  while (getline(file, line))    //vado avanti fino alla fine del file, getline memorizza tutta la linea
    listLines.push_back(line);  //inserisco la linea del file nella lista

  file.close();

  listLines.pop_front();  //elimino la prima riga con i nomi

  mesh.NumberCell0D = listLines.size();  //la dimensione della lista è il numero di celle 0D lette

  if (mesh.NumberCell0D == 0)
  {
    cerr << "There is no cell 0D" << endl;
    return false;
  }

  for (const string& line : listLines)  //creo un oggetto che legge da stringa e converte
  {
    istringstream converter(line);  //leggo la linea, la splitto in modo opportuno

    unsigned int marker;
    Point punto;  //coordinate x, y e id

    converter >> punto.id >> marker >>punto.x >> punto.y;

    mesh.Cell0D_Id_coord.push_back(punto);  //accedo alle celle 0D e inserisco id, x e y

    if( marker != 0)  //se il marker è 0 non lo memorizzo, se esiste il marker devo aggiungere un elemento, altrimenti devo aggiungere alla mappa la coppia chiave-valore
    {
      if (mesh.Cell0DMarkers.find(marker) == mesh.Cell0DMarkers.end())  //se l'oggetto trovato è uguale a end l'oggetto non esiste, devo creare la prima coppia chiave-valore
        mesh.Cell0DMarkers.insert({marker, {punto.id}});  //con insert inserisco la coppia chiave-valore, il valore è una lista che per adesso contiene solo id
      else
        mesh.Cell0DMarkers[marker].push_back(punto.id);  //se esiste già, uso la chiave come indice e aggiungo solo l'elemento
    }

  }
  file.close();
  return true;
}
bool ImportCell1Ds(TriangularMesh& mesh, string nome_file)
{

  ifstream file;
  file.open(nome_file);

  if(file.fail())
    return false;

  list<string> listLines;
  string line;
  while (getline(file, line))
    listLines.push_back(line);

  listLines.pop_front();

  mesh.NumberCell1D = listLines.size();

  if (mesh.NumberCell1D == 0)
  {
    cerr << "There is no cell 1D" << endl;
    return false;
  }

  for (const string& line : listLines)
  {
    istringstream converter(line);
    Segmento seg;  //id-iniz e id-fin, id
    unsigned int marker;

    converter >>  seg.id >> marker >> seg.id_iniz >> seg.id_fin;

    mesh.Cell1D_segmenti.push_back(seg);

    if( marker != 0)
    {
      if (mesh.Cell1DMarkers.find(marker) == mesh.Cell1DMarkers.end())  //creo la coppia chiave-valore se non prsente
        mesh.Cell1DMarkers.insert({marker, {seg.id}});
      else
        mesh.Cell1DMarkers[marker].push_back(seg.id);  //aggiungo l'id alla lista dei valori associati al marker
    }
  }

  file.close();
  return true;
}

bool ImportCell2Ds(TriangularMesh& mesh, string nome_file)
{

  ifstream file;
  file.open(nome_file);

  if(file.fail())
    return false;

  list<string> listLines;
  string line;
  while (getline(file, line))
    listLines.push_back(line);

  listLines.pop_front();

  mesh.NumberCell2D = listLines.size();

  if (mesh.NumberCell2D == 0)
  {
    cerr << "There is no cell 2D" << endl;
    return false;
  }

  for (const string& line : listLines)
  {
    istringstream converter(line);
    Triangolo tria;  //id, vertici e lati

    converter >>  tria.id;
    for(unsigned int i = 0; i < 3; i++)
      converter >> tria.vertices[i];
    for(unsigned int i = 0; i < 3; i++)
      converter >> tria.edges[i];

    mesh.Cell2D_triangoli.push_back(tria);

  }
  file.close();
  return true;
}

//CALCOLO DELLA LUNGHEZZA DI UN SEGMENTO
double Lunghezza(vector<Point> punti,Segmento segm){
    double distanza=0;
    distanza=sqrt((punti[segm.id_iniz].x-punti[segm.id_fin].x)*(punti[segm.id_iniz].x-punti[segm.id_fin].x)+(punti[segm.id_iniz].y-punti[segm.id_fin].y)*(punti[segm.id_iniz].y-punti[segm.id_fin].y));
    return distanza;
}
//CALCOLO DELL'AREA DI UN TRIANGOLO
double Area(vector<Point> punti, Triangolo trian){
    double area=0;
    for (unsigned int i=0; i<3; i++)
    {
        area+=punti[trian.vertices[i%3]].x*punti[trian.vertices[(i+1)%3]].y-punti[trian.vertices[(i+1)%3]].x*punti[trian.vertices[i%3]].y;
    }
    area=abs(area/2);
    return area;
}
//CALCOLO DEL PUNTO MEDIO DEL LATO PIU' LUNGO
void puntoMedio(vector<Point>& punti, Segmento segm, unsigned int& numPunti){
    Point puntoMedio;
    puntoMedio.x=(punti[segm.id_iniz].x+punti[segm.id_fin].x)/2;
    puntoMedio.y=(punti[segm.id_iniz].y+punti[segm.id_fin].y)/2;
    puntoMedio.id=numPunti;  //devo aggiungere il punto medio ai punti nella mesh 0d, gli do come id il valore dell'ultimo id +1
    punti.push_back(puntoMedio);
    numPunti+=1; //c'è un punto in più nella mesh od
}

}
