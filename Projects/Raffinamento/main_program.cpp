#include "empty_class.hpp"

int main()
{
  //ProjectLibrary::Empty empty;
  //empty.Show();
   //IMPORTAZIONE DATI
   ProjectLibrary::TriangularMesh mesh;
   string nome_file="/Users/francescamusella/Desktop/PCS git/PCS2023_Exercises/Projects/Raffinamento/Dataset/Test1/Cell0Ds.csv";
   if(!ImportCell0Ds(mesh, nome_file))  //controllo se è andata a buon fine la lettura del file
   {
     return -1;
   }
   nome_file="/Users/francescamusella/Desktop/PCS git/PCS2023_Exercises/Projects/Raffinamento/Dataset/Test1/Cell1Ds.csv";
   if(!ImportCell1Ds(mesh, nome_file))
   {
     return -1;
   }
   nome_file="/Users/francescamusella/Desktop/PCS git/PCS2023_Exercises/Projects/Raffinamento/Dataset/Test1/Cell2Ds.csv";
   if(!ImportCell2Ds(mesh, nome_file))
   {
     return -1;
   }

   //ORDINO I PUNTI IN BASE ALL'ID (nel nostro caso sono già ordinati ma lo facciamo per generalità)
   //ProjectLibrary::MergeSort(mesh.Cell0D_Id_coord,0,mesh.NumberCell0D-1);

   /*for(unsigned int i=0;i<mesh.NumberCell0D;i++)  //controllo sui punti
         cout<<"ID: "<<mesh.Cell0D_Id_coord[i].id<<" X: "<<mesh.Cell0D_Id_coord[i].x<<" Y: "<<mesh.Cell0D_Id_coord[i].y<<endl;*/

   //CALCOLO DELLA LUNGHEZZA DEI SEGMENTI (che serve per individuare il lato più lungo dei triangoli)
   for (unsigned int i=0; i<mesh.NumberCell1D;i++)
       mesh.Cell1D_segmenti[i].lunghezza=Lunghezza(mesh.Cell0D_Id_coord, mesh.Cell1D_segmenti[i]);
   /*for(unsigned int i=0;i<mesh.NumberCell1D;i++)  //controllo sui segmenti
           cout<<"ID: "<<mesh.Cell1D_segmenti[i].id<<" VERTICI: "<<mesh.Cell1D_segmenti[i].id_iniz<<" "<<mesh.Cell1D_segmenti[i].id_fin<<" LUNGHEZZA: "<<mesh.Cell1D_segmenti[i].lunghezza<<endl;*/

   //CALCOLO DELLE AREE DEI TRIANGOLI
   for (unsigned int i=0; i<mesh.NumberCell2D;i++)
       mesh.Cell2D_triangoli[i].area=Area(mesh.Cell0D_Id_coord, mesh.Cell2D_triangoli[i]);
   /*for(unsigned int i=0;i<mesh.NumberCell2D;i++)  //controllo sulle aree
             cout<<"ID: "<<mesh.Cell2D_triangoli[i].id<<" VERTICI: "<<mesh.Cell2D_triangoli[i].vertices[0]<<"-"<<mesh.Cell2D_triangoli[i].vertices[1]<<"-"<<mesh.Cell2D_triangoli[i].vertices[2]<<" LATI: "<<mesh.Cell2D_triangoli[i].edges[0]<<"-"<<mesh.Cell2D_triangoli[i].edges[1]<<"-"<<mesh.Cell2D_triangoli[i].edges[2]<<" AREA: "<<mesh.Cell2D_triangoli[i].area<<endl;*/

   //ORDINARE I TRIANGOLI IN BASE ALLE AREE
   ProjectLibrary::MergeSort(mesh.Cell2D_triangoli,0,mesh.NumberCell2D-1);
   /*for(unsigned int i=0;i<mesh.NumberCell2D;i++)
            cout<<mesh.Cell2D_triangoli[i].id<<" "<<mesh.Cell2D_triangoli[i].area<<endl;*/ //controllo se le aree sono ordinate in modo decrescente

   //definisco e creo teta: valore soglia dell'area oltre il quale divido il triangolo
   double teta=0.0;
   for (unsigned int i=0;i<mesh.NumberCell2D;i++)
   {
       teta+=mesh.Cell2D_triangoli[i].area; //area del trinagoli i-esimo
   }
  teta=teta/mesh.NumberCell2D;
  int id_lato_max=0;
  //DIVIDO LE AREE FINO A QUANDO QUELLA MAGGIORE DIVENTA MINORE DI TETA
  //while (mesh.Cell2D_triangoli[0].area>teta)
  //{
    //calcolo il lato più lungo, con il max o con il mergesort
    /*if (mesh.Cell1D_segmenti[mesh.Cell2D_triangoli[0].edges[0]].lunghezza < mesh.Cell1D_segmenti[mesh.Cell2D_triangoli[0].edges[1]].lunghezza)
    {
        if (mesh.Cell1D_segmenti[mesh.Cell2D_triangoli[0].edges[1]].lunghezza < mesh.Cell1D_segmenti[mesh.Cell2D_triangoli[0].edges[2]].lunghezza)

        {
            id_lato_max=mesh.Cell2D_triangoli[0].edges[2];
        }
        else
            id_lato_max=mesh.Cell2D_triangoli[0].edges[1];
    }
    else
    {
        if (mesh.Cell1D_segmenti[mesh.Cell2D_triangoli[0].edges[0]].lunghezza < mesh.Cell1D_segmenti[mesh.Cell2D_triangoli[0].edges[2]].lunghezza)
            id_lato_max=mesh.Cell2D_triangoli[0].edges[2];
        else
            id_lato_max=mesh.Cell2D_triangoli[0].edges[0];*/

  vector<ProjectLibrary::Segmento> supp_lato_lungo={};  //calcolo del lato più lungo con il merge sort
  supp_lato_lungo.push_back(mesh.Cell1D_segmenti[mesh.Cell2D_triangoli[0].edges[0]]);
  supp_lato_lungo.push_back(mesh.Cell1D_segmenti[mesh.Cell2D_triangoli[0].edges[1]]);
  supp_lato_lungo.push_back(mesh.Cell1D_segmenti[mesh.Cell2D_triangoli[0].edges[2]]);
  ProjectLibrary::MergeSort(supp_lato_lungo,0,2);
  id_lato_max=supp_lato_lungo[0].id;
   // }
    //cout<<id_lato_max<<endl;

    //CALCOLO PUNTO MEDIO LATO PIU' LUNGO
    puntoMedio(mesh.Cell0D_Id_coord, mesh.Cell1D_segmenti[id_lato_max],mesh.NumberCell0D); //punto medio del triangolo con area maggiore

    /*for(unsigned int i=0;i<mesh.NumberCell0D;i++)  //controllo che il punto medio del triangolo con area maggiore è giusto
          cout<<"ID: "<<mesh.Cell0D_Id_coord[i].id<<" X: "<<mesh.Cell0D_Id_coord[i].x<<" Y: "<<mesh.Cell0D_Id_coord[i].y<<endl;*/

  //}



   return 0;
}
