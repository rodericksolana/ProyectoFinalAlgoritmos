#include <iostream>
#include <omp.h>
#include <X11/Xlib.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/config.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <vector>
#include <algorithm>

using namespace std;
using namespace boost;

struct vertex_info {
    std::string name;
    int value1;
    int value2;
};



typedef property<edge_weight_t, int> EdgeWeightProperty;
typedef adjacency_list<vecS, vecS, directedS, vertex_info, EdgeWeightProperty> graph_t;
typedef graph_traits<graph_t>::vertex_iterator vertex_iter;
typedef graph_traits<graph_t>::vertex_descriptor Vertice;
typedef graph_traits<graph_t>::edge_iterator edge_iter;
typedef graph_traits <graph_t >::edge_descriptor Arista;

#include <fstream>
#include <stdio.h>
#include <graphics.h>
#include <stdlib.h>
#define M 300

//LGRAPH

void generarGrafo(graph_t& graph, char* filename)
{
        int numeros[3];
        int i;
        std::ifstream  data(filename);

        std::string line;
        while(std::getline(data,line))
        {
            i = 0;
            std::stringstream  lineStream(line);
            std::string        cell;
            while(std::getline(lineStream,cell,','))
            {
                std::cout << cell << " ";
                if(isdigit(cell[0]))
                {
                    numeros[i] = atoi(cell.c_str());
                    i++;
                }
                if (i == 3)
                    add_edge(numeros[0],numeros[1],numeros[2], graph);
            }


            std::cout << std::endl;
        }
}
void printEdgeWeights(const graph_t graph) {

  std::pair<edge_iter, edge_iter> ei;

  typedef boost::property_map<graph_t, boost::edge_weight_t>::const_type WeightMap;
  // The following line will not compile:
  WeightMap weights = boost::get(boost::edge_weight_t(), graph);

 // EdgeIterator edge;

  std::ofstream out("test.csv");
  out << "Estacion Origen,Estacion Destino,Tiempo\n";
  for (ei = edges(graph); ei.first != ei.second; ++ei.first) {
    //std::cout <<*ei.first << "," << boost::get(weights, *ei.first) << std::endl;
   /* std::stringstream temp;
    temp << *edge;
    std::string tempstring = temp.str();

    tempstring = tempstring.substr(1,tempstring.length()-2);
    */
    out << source(*ei.first,graph) << ","  << target(*ei.first,graph) << "," << boost::get(weights, *ei.first) << std::endl;
    //out << (std::string)(*edge).substr(1,(std::string)(*edge).length() - 1) << "," << boost::get(weights, *edge) << std::endl;
  }
  out.close();
}

void printNodos(const graph_t& graph)
{
    std::ofstream outNodos("Nodos.csv");

    outNodos << "Nombre,CoordenadaX,CoordenadaY\n";

    for (int i =0; i < num_vertices(graph); i++)
    {
        /*std::cout << "nombre " << graph[i].name << "    valor x " << graph[i].value1 << "     valor y " << graph[i].value2 << std::endl;
*/
        outNodos<<graph[i].name<< "," << graph[i].value1 << "," << graph[i].value2<<std::endl;
    }
}

void generarNodos(graph_t& graph, char* filename)
{
        int i = -1;
        std::ifstream  data(filename);

        std::string line;

        while(std::getline(data,line))
        {
            std::stringstream  lineStream(line);
            std::string        cell;
            int n = 0;
            if(i >= 0)
            {
                while(std::getline(lineStream,cell,','))
                {
                    std::cout << cell << " ";

                    if(n == 0)
                    {
                        graph[i].name = cell.c_str();
                    }

                    if (n == 1)
                    {
                        graph[i].value1 = atoi(cell.c_str());
                    }
                    if (n == 2)
                        graph[i].value2 = atoi(cell.c_str());
                    n++;
                }
            }


          i++;
          std::cout << std::endl;
        }
}


void swap(int* aa,int* bb)
{
	int tt=*aa;
	*aa=*bb;
	*bb=tt;
}
 
void midpointline(int x1,int y1,int x2,int y2)
{
	int dx,dy,dd,incry,incre,incrne,slopegt1=0;
	dx=abs(x1-x2);dy=abs(y1-y2);
	if(dy>dx)
	{
		swap(&x1,&y1);
		swap(&x2,&y2);
		swap(&dx,&dy);
		slopegt1=1;
	}
	if(x1>x2)
	{
		swap(&x1,&x2);
		swap(&y1,&y2);
	}
	if(y1>y2)
		incry=-1;
	else
		incry=1;
	dd=2*dy-dx;
	incre=2*dy;
	incrne=2*(dy-dx);
	while(x1<x2)
	{
		if(dd<=0)
			dd+=incre;
		else
		{
			dd+=incrne;
			y1+=incry;
		}
		x1++;
		if(slopegt1)
			putpixel(y1,x1,RED);
		else
			putpixel(x1,y1,RED);
	}
}//Cierre de MidPoint


class DFSVertexVisitor : public default_dfs_visitor
{
public:
  void discover_vertex(graph_t::vertex_descriptor v, graph_t g)
  {
    std::cout << "DFS: Nodo #: " <<  v << " con nombre: " << g[v].name << " y coordenadas: " << g[v].value1 << "," << g[v].value2 << std::endl;
  }
};//Cierre clase DFS

class BFSVertexVisitor : public default_bfs_visitor
{
public:
  void discover_vertex(graph_t::vertex_descriptor v, graph_t g)
  {
    std::cout << "BFS: Nodo #: "<< v << " con nombre: " << g[v].name << " y coordenadas: " << g[v].value1 << "," << g[v].value2 << std::endl;;
  }
};//Cierre clase BFS



//Función para buscar un dato
int buscarNombre(graph_t g, std::string busqueda)
{
    vector<string> nombres;
    for (int i =0; i < num_vertices(g); i++)
    {
        nombres.push_back(g[i].name);

    }//Cierre de for

    vector<string>::iterator vec_it;
    vec_it = std::find (nombres.begin(), nombres.end(), busqueda);
    if(vec_it!=nombres.end())
        //cout << "Se encuentra en la posicion: " << vec_it - nombres.begin();
        return vec_it - nombres.begin();
    else
        return -1;

    nombres.clear();

}//Cierre función buscar

//Función del tiempo
double diffclock(clock_t clock1,clock_t clock2)
{
double diffms = ((long double )(clock1 - clock2) / CLOCKS_PER_SEC)* 1000.0;
    return diffms;
}

int main()
{

XInitThreads();

clock_t start; clock_t end;
 graph_t g;
	
/*
    add_edge(0,1, 2, g);
    add_edge(1,4, 8, g);
    add_edge(1,3, 8, g);
    add_edge(3,2, 7, g);
    add_edge(4,7, 3, g);
    add_edge(7,4, 6, g);
    add_edge(4,5, 1, g);
    add_edge(5,6, 0, g);
    add_edge(2,5, 7, g);

g[0].name = "Reforma";
g[0].value1 = -100;
g[0].value2 = 0;
g[1].name = "Popotla";
g[1].value1 = -100;
g[1].value2 = 50;
g[2].name = "Tacubaya";
g[2].value1 = 100;
g[2].value2 = 50;
g[3].name = "Polanco";
g[3].value1 = 150;
g[3].value2 = 0;
g[4].name ="Auditorio";
g[4].value1= 50;
g[4].value2= -50;
g[5].name ="Camarones";
g[5].value1=0;
g[5].value2=-50;
g[6].name="Hidalgo";
g[6].value2=0;
g[6].value1=-100;
g[7].name="Taxqueña";
g[7].value1=50;
g[7].value2=-100;

*/
//MENÚ

 int n; int temp1; int temp2; string a; string b; int w; int ejex; int ejey; int n2;
 	//variables de GEOMETRÍA
	int x1,x2,y1,y2;
	int gd=DETECT,gm;

while (1) {
n=0;
		cout<<endl;
cout << endl <<"¿Qué quieres hacer?  1.Configurar Grafo, 2.Ejecutar un algoritmo 3. Importar Grafo 4.Exportar Grafo 5.Salir del programa " <<endl;
cin>>n2;
if (n2==1)
{
while (1) {
		cout<<endl;
		std::ofstream dotfile("test.dot");
write_graphviz(dotfile,g);
      cout <<"Este es el grafo" << endl;  
   	std::pair<edge_iter, edge_iter> ei;
		for(ei = edges(g); ei.first != ei.second; ++ei.first)
{
	cout << g[source(*ei.first,g)].name <<" a " << g[target(*ei.first,g)].name<< endl;
}
cout << endl <<"¿Qué quieres hacer?  1.Insertar un nuevo enlace, 2.Eliminar un enlace, 3.Eliminar un nodo 4.Imprimir la gráfica 5.Regresar al menú anterior " <<endl;
cin>>n;
   if (n==1)
{

cout<<"Ingresa el nodo inicial"<< endl;
cin>> a;
temp1 = buscarNombre(g, a);
if(temp1==-1)
{
add_vertex(g);
g[num_vertices(g)-1].name=a;

cout<<"La estación " << a <<" se acaba de agregar" << endl;
cout<<"Ingrese el eje x: ";
cin>> ejex;
g[num_vertices(g)-1].value1=ejex;
cout<<"Ingrese el eje y: ";
cin>> ejey;
g[num_vertices(g)-1].value2=ejey;
temp1=num_vertices(g)-1;
}
cout<<endl <<"Ingresa el nodo final"<< endl;
cin>> b;
temp2 = buscarNombre(g, b);
if(temp2==-1)
{
add_vertex(g);
g[num_vertices(g)-1].name=b;

cout<<"La estación " << b <<" se acaba de agregar" << endl;
cout<<"Ingrese el eje x: ";
cin>> ejex;
g[num_vertices(g)-1].value1=ejex;
cout<<"Ingrese el eje y: ";
cin>> ejey;
g[num_vertices(g)-1].value2=ejey;
temp2=num_vertices(g)-1;
}
cout<<"Ingrese el tiempo: ";
cin >> w;

   add_edge(temp1, temp2, w , g );
}//Cierre de 1
else if(n==2)
{
cout<<"Ingresa el nodo inicial"<< endl;
cin>> a;
temp1 = buscarNombre(g, a);
cout<<"Ingresa el nodo final"<< endl;
cin>> b;
temp2 = buscarNombre(g, b);

if(temp1==-1||temp2==-1)
	cout<<"Ese enlace no existe"<<endl;
else {
cout<<"Se eliminó el enlace" <<endl;
remove_edge(temp1, temp2, g );
}

}//Cierre de 2
else if (n==3)
{
cout<<"¿Qué nodo quieres eliminar: ";
cin >>a;
temp1 = buscarNombre(g,a);

if (temp1!=-1)
{
    clear_vertex(temp1,g);
    remove_vertex(temp1,g);
	cout<<"El nodo "<< a <<" se eliminó correctamente" << endl;
}
else
cout<<"El nodo no existe";
}
else if(n==4)
{

initgraph(&gd,&gm,"..\\bgi");

for(ei = edges(g); ei.first != ei.second; ++ei.first)
{
string str=g[source(*ei.first,g)].name;
char* writable=new char[str.size()+1];
copy(str.begin(), str.end(), writable);
writable[str.size()]='\0';
 str=g[target(*ei.first,g)].name;
char* writable2=new char[str.size()+1];
copy(str.begin(), str.end(), writable2);
writable2[str.size()]='\0';
	x1= g[source(*ei.first,g)].value1 +320;
	y1=240 - g[source(*ei.first,g)].value2 ;
	x2= g[target(*ei.first,g)].value1 +320;
	y2=240 - g[target(*ei.first,g)].value2 ;
	
	outtextxy(x1-2,y1-3, writable);
	outtextxy(x2-2,y2-3, writable2);
	midpointline(x1,y1,x2,y2);
}
	getch();
	closegraph();

}//Cierre de 3

else if (n==5)
	break;
else cout<<endl <<"Esa opción no es válida";
}//Cierre de while 2
}//Cierre de if 1 menú general

else if (n2==2)
{
/*Menú opción 2 del general*/
while(1)
{
if(num_vertices(g)==0)
{
cout<<"Genera o importa un grafo antes de ejecutar los algoritmos"<<endl;
break;
}
else
{
cout << endl <<"¿Qué algoritmo quieres ejecutar?  1.DFS, 2.BFS, 3.KRUSKAL 4.PRIM 5.DIJKSTRA 6.FLOYD WARSHALL  7.Ejecutar Todos los algoritmos 8.Regresar al menú anterior" <<endl;
cin>> n;
if(n==7)
{
	int nhilos = 6;
	double tiempos[6];
	omp_set_num_threads(nhilos);
#pragma omp parallel private (start,end)
{
	
	int id = omp_get_thread_num();  //obtener el id de cada hilo
	// cout<<"Prueba " << id;
if (id==0)
{
 DFSVertexVisitor dfsVisitor;
    start = clock();
    depth_first_search(g, visitor(dfsVisitor).root_vertex(1));
    end = clock();
tiempos[0]=diffclock(end,start);
    //std::cout << diffclock(end,start) << "MILISEGUNDOS Terminó DFS"<<endl;
}//Cierre de 0
if(id==1)
{
    BFSVertexVisitor bfsVisitor;
    start = clock();
    breadth_first_search(g, vertex(1, g), visitor(bfsVisitor));
    end = clock();
tiempos[1]=diffclock(end,start);
    //std::cout << diffclock(end,start) << "MILISEGUNDOS Terminó BFS"<<endl;

}//Cierre de 1
if(id==2)
{
property_map<graph_t, edge_weight_t>::type EdgeWeightMap = get(edge_weight_t(), g);
//Kruskal
    std::vector < Arista > spanning_tree;
    start = clock();
    kruskal_minimum_spanning_tree(g, std::back_inserter(spanning_tree));
    end = clock();
   tiempos[2]=diffclock(end,start);
    //std::cout << diffclock(end,start) << "MILISEGUNDOS Terminó Kruskal"<<endl;
}//Cierre de 2

if(id==3)
{
std::vector < Vertice > p(num_vertices(g));


    start = clock();
    prim_minimum_spanning_tree(g, &p[0]);
    end = clock();
    tiempos[3]=diffclock(end,start);
    //std::cout << diffclock(end,start) << "MILISEGUNDOS Terminó Prim"<<endl;

}//Cierre de 3

if(id == 4)
{
std::vector < Vertice > p(num_vertices(g));
    start = clock();
    std::vector<int> d(num_vertices(g));

    Vertice s = vertex(1, g); 
    dijkstra_shortest_paths(g, s, predecessor_map(make_iterator_property_map(p.begin(),
                           get(vertex_index, g))).distance_map(make_iterator_property_map(d.begin(),
                           get(vertex_index, g))));

    end = clock();
    tiempos[4]=diffclock(end,start);
  // std::cout << diffclock(end,start) << "MILISEGUNDOS Terminó Dijkstra"<<endl;

}//Cierre 4

if(id == 5)
{
property_map<graph_t, edge_weight_t>::type EdgeWeightMap = get(edge_weight_t(), g);
//FLOYD WARSHALL
    start = clock();
    std::map<Vertice, std::map<Vertice, int> > matrix;
    floyd_warshall_all_pairs_shortest_paths(g,matrix,weight_map(EdgeWeightMap));
    end = clock();
    tiempos[5]=diffclock(end,start);
   // std::cout << diffclock(end,start) << "MILISEGUNDOS Terminó Floyd Warshal"<<endl;
}//Cierre 5
	//if(id== nhilos - 1) { cout<<"Prueba " << id; }

}//Cierre de pragma
for (int i=0; i<6;++i)
{
	cout<< tiempos[i] <<endl;
}//Cierre de for
w=0;
cout<<"¿Quieres ver la comparación del tiempo de ejecución?  si/1 no/0: ";
cin>>w;
if(w==1)
{
//cout << *max_element(tiempos,tiempos+6) << endl;
initgraph(&gd, &gm, "C:\\TC\\BGI");
char str[16];// = malloc(16);

snprintf(str,16,"%d", (int)(*max_element(tiempos,tiempos+6)));
outtextxy(0,0,str);
outtextxy(0,450, "0ms");
for( int i = 0; i < 6; i++)
{
	snprintf(str,16,"%d", (int)(i*(*max_element(tiempos,tiempos+6)/5)));
	outtextxy(0, 450 -  i*(450/5), str);
snprintf(str,16,"%d", i + 1);
outtextxy(100 + (i*75), 450, str);
} 

for(int i = 0; i < 6; i++)
{
   bar(100 + i*75, 450 - (int)(tiempos[i]*(450/(*max_element(tiempos,tiempos+6)))) + 1, 150 + i*75, 450);
   //bar(175,200,225,450); 
}
line(0,450,600,450);
line(50,0,50,450);
   getch();
   closegraph();
}//Cierre de if imprimir tablas

}//Cierre de 7

 else if (n==1)
{
 std::cout << std::endl << "Búsqueda DFS" << std::endl;
    DFSVertexVisitor dfsVisitor;
    start = clock();
    depth_first_search(g, visitor(dfsVisitor).root_vertex(1));
    end = clock();
    std::cout << diffclock(end,start) << "MILISEGUNDOS";
}//Cierre de 1
else if(n==2)
{

std::cout << std::endl << "Búsqueda BFS" << std::endl;
    BFSVertexVisitor bfsVisitor;
    start = clock();
    breadth_first_search(g, vertex(1, g), visitor(bfsVisitor));
    end = clock();
    std::cout << diffclock(end,start) << "MILISEGUNDOS";

}//Cierre de 2
else if(n==3)
{
property_map<graph_t, edge_weight_t>::type EdgeWeightMap = get(edge_weight_t(), g);
//Kruskal
    std::vector < Arista > spanning_tree;
    start = clock();
    kruskal_minimum_spanning_tree(g, std::back_inserter(spanning_tree));
    end = clock();
    std::cout << diffclock(end,start) << "MILISEGUNDOS";

    std::cout << std::endl << "Imprimiendo nodos en MST (kruskal):" << std::endl;
    for (std::vector <Arista>::iterator aristait = spanning_tree.begin();
    aristait != spanning_tree.end(); ++aristait)
    {
        std::cout << g[source(*aristait, g)].name << " <--> " << g[target(*aristait, g)].name
        << " con tiempo " << EdgeWeightMap[*aristait] << " minutos" << std::endl;
    }

}//Cierre de 3

else if(n==4)
{

//PRIM

    std::vector < Vertice > p(num_vertices(g));


    start = clock();
    prim_minimum_spanning_tree(g, &p[0]);
    end = clock();
    std::cout << diffclock(end,start) << "MILISEGUNDOS";

    std::cout << std::endl << "Prim..." << std::endl;

#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  property_map<graph_t, vertex_distance_t>::type distance = get(vertex_distance, g);
  property_map<graph_t, vertex_index_t>::type indexmap = get(vertex_index, g);
  prim_minimum_spanning_tree
    (g, *vertices(g).first, &p[0], distance, weightmap, indexmap, 
     default_dijkstra_visitor());
#else
  prim_minimum_spanning_tree(g, &p[0]);
#endif

 for (std::size_t i = 0; i != p.size(); ++i)
    if (p[i] != i)
      std::cout << "parent[" << g[i].name << "] = " << g[p[i]].name << std::endl;
    else
 std::cout << "parent[" << g[i].name << "] = no parent" << std::endl;


}//Cierre de 4

else if(n==5)
{
//Dijkstra
std::vector < Vertice > p(num_vertices(g));
    start = clock();
    std::vector<int> d(num_vertices(g));
cout<<"¿De qué estación quieres partir?: ";
string op;
cin>>op;
temp1 = buscarNombre(g, op);
if(temp1!=-1)
{
    Vertice s = vertex(temp1, g); 
    dijkstra_shortest_paths(g, s, predecessor_map(make_iterator_property_map(p.begin(),
                           get(vertex_index, g))).distance_map(make_iterator_property_map(d.begin(),
                           get(vertex_index, g))));

    end = clock();
   std::cout << diffclock(end,start) << "MILISEGUNDOS";

    std::cout << std::endl << "Dijkstra partiendo de la estación: " << op << std::endl;
    graph_traits <graph_t >::vertex_iterator vi, vend;
    for (tie(vi, vend) = vertices(g); vi != vend; ++vi)
       {
           if(d[*vi] != std::numeric_limits<int>::max())
{
               std::cout << "distancia a " << g[*vi].name << " = " << d[*vi] << ", ";
 std::cout << "proviene del nodo " << g[p[*vi]].name << std::endl;
}//Cierre de if
           else
               std::cout << "distancia a " << g[*vi].name << " = No se puede " << endl;
          
       }
}
else cout <<"EL nodo no se encuentra";

}//Cierre de 5

else if(n==6)
{

property_map<graph_t, edge_weight_t>::type EdgeWeightMap = get(edge_weight_t(), g);
//FLOYD WARSHALL
    std::cout << std::endl << "Floyd Warshall...." << std::endl;


    start = clock();
    std::map<Vertice, std::map<Vertice, int> > matrix;
    floyd_warshall_all_pairs_shortest_paths(g,matrix,weight_map(EdgeWeightMap));
    end = clock();
    std::cout << diffclock(end,start) << "MILISEGUNDOS";
cout<<"¿De qué estación quieres partir?: ";
string op;
string op2;
cin>>op;
temp1 = buscarNombre(g, op);
cout<<"¿A qué estación quieres llegar?: ";
cin>>op2;
temp1 = buscarNombre(g, op);
temp2 = buscarNombre(g, op2);
if(temp1!=-1&&temp2!=-1)
{
if(matrix[temp1][temp2] == numeric_limits<int>::max())
	cout << "No se puede llegar a esa estación";
else
	cout << "El tiempo es de: "<< matrix[temp1][temp2] <<" minutos";
}
else
cout<<endl <<"No se encontró alguna de las estaciones";
}//Cierre de 6

else if(n==8)
 break;
else 
cout<<"Esa opción no es válida";
}
}//Cierre de while opcion 2
}//Cierre de if 2 menu general
else if(n2==3)
{

generarGrafo(g, "test.csv");
generarNodos(g, "Nodos.csv");


}//Cierre de 3
else if(n2==4)
{

printEdgeWeights(g);
printNodos(g);

}//Cierre de 4
else if(n2==5)
	exit(0);
else
cout<<"Esa opción no es válida";

}//Cierre de while general

   
    return 0;
}
