#include <iostream>
#include <cctype>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include <math.h>
#include <fstream>
using namespace std;
class Red
{
public:
    vector<int> nodos;
    vector<vector<int>> relaciones;
    float m_num_edges;

public:
    Red();
    void agregar_nodo(int);
    void agregar_relacion(int, int, int);
    void imprimir();
    int get_A(int i, int j); // Retorna el peso de la arista entre 2 nodos
    int get_K(int i_j);
    int get_A(list<int> *, list<int> *);
    int get_K(list<int> *);
    void Louvain_Comunidades();
    list<int> *get_Listaadyacencia();
    bool get_Delta();
    ~Red();
};
Red::Red()
{
    this->m_num_edges = 0;
}
void Red::agregar_nodo(int v)
{
    if (!nodos.empty())
    {
        auto ubi = find(nodos.begin(), nodos.end(), v);
        if (ubi != nodos.end())
            return;
    }
    nodos.push_back(v);
    vector<int> m;
    for (int i = 0; i < relaciones.size(); i++)
    {
        relaciones[i].push_back(0);
        m.push_back(0);
    }
    m.push_back(0);
    relaciones.push_back(m);
}
void Red::agregar_relacion(int a, int b, int v)
{
    int index_a = -1, index_b = -1;
    if (nodos.empty())
        return;
    for (int i = 0; i < nodos.size(); i++)
    {
        if (nodos[i] == a)
        {
            index_a = i;
        }
        if (nodos[i] == b)
        {
            index_b = i;
        }
    }
    if ((index_a * index_b) >= 0)
    {
        relaciones[index_a][index_b] = v;
        relaciones[index_b][index_a] = v;
        m_num_edges += v;
    }
}
void Red::Louvain_Comunidades()
{
    double Q;
    float sumatoria = 0;
    vector<list<int> *> C_nodos;
    list<list<int> *> Comunidades;
    for (int k_2 = 0; k_2 < nodos.size(); k_2++)
    {
        list<int> *m = new list<int>;
        m->push_back(k_2);
        C_nodos.push_back(m);
        Comunidades.push_back(m);
        sumatoria += ((float)relaciones[k_2][k_2] - ((2 * (float)get_K(nodos[k_2])) / (2 * m_num_edges)));
    }
    Q = sumatoria * (1 / (2 * m_num_edges));
    /*
    cout<<"Q: "<<Q<<endl;
    for(auto it=Comunidades.begin(); it!=Comunidades.end(); it++){
        cout<<*((*it)->begin())<<" ";
    }*/
    list<int> *adyacencia = get_Listaadyacencia();
    auto comunidad_i = Comunidades.begin();
    while (comunidad_i != Comunidades.end())
    {
        if ((*comunidad_i) != nullptr)
        {
            float dif_Q_max = 0;
            list<int> *resul = nullptr;
            list<int> caminos;
            for (auto bus = (*comunidad_i)->begin(); bus != (*comunidad_i)->end(); bus++)
                for (auto ad = adyacencia[*bus].begin(); ad != adyacencia[*bus].end(); ad++)
                    if (find((*comunidad_i)->begin(), (*comunidad_i)->end(), *ad) == (*comunidad_i)->end())
                        caminos.push_back(*ad);
            // Paso 1: Sacar i de su comunidad
            list<int> *copy = (*comunidad_i);
            (*comunidad_i) = nullptr;
            // Paso 2: Diferencia de Q
            for (auto it = caminos.begin(); it != caminos.end(); it++)
            {
                list<int> *comunidad_j = C_nodos[*it];
                float k_i = get_K(copy);
                float in = get_A(comunidad_j, comunidad_j);
                float tot = get_K(comunidad_j);
                float k_i_in = get_A(copy, comunidad_j);
                // cout<<k_i<<" --- "<<in<<" --- "<<tot<<" --- "<<k_i_in<<endl;
                float dif_Q = (((in + (2 * k_i_in)) / (2 * m_num_edges)) - pow(((tot + k_i) / (2 * m_num_edges)), 2)) - ((in / (2 * m_num_edges)) - pow((tot / (2 * m_num_edges)), 2) - pow((k_i / (2 * m_num_edges)), 2));
                if (dif_Q > dif_Q_max)
                {
                    resul = comunidad_j;
                    dif_Q_max = dif_Q;
                }
            }
            if (dif_Q_max != 0)
            {
                Q += dif_Q_max;
                for (auto it1 = copy->begin(); it1 != copy->end(); it1++)
                {
                    resul->push_back(*it1);
                    C_nodos[*it1] = resul;
                    cout << nodos[*it1] << " ";
                }
                cout << " ------> ";
                for (auto it1 = resul->begin(); it1 != resul->end(); it1++)
                {
                    cout << nodos[*it1] << " ";
                }
                cout << endl;
                delete copy;
            }
            else
            {
                (*comunidad_i) = copy;
            }
        }
        comunidad_i++;
    }
    // Impresion de las comunidades
    int contador = 1;
    for (auto i = Comunidades.begin(); i != Comunidades.end(); i++)
    {
        if ((*i) != nullptr)
        {
            cout << "Comunidad " << contador << ": ";
            for (auto j = (*i)->begin(); j != (*i)->end(); j++)
                cout << nodos[*j] << " ";
            cout << endl;
            contador++;
        }
    }

    // Generación de imagen de las comunidades
    ofstream output;
    output.open("Result.dot");
    output << "graph Communities {" << endl;
    output << "\t"
           << "label=Communities;" << endl;
    output << "\t"
           << "layout=fdp;" << endl;
    int count = 1;
    srand(time(nullptr));
    for (auto index = Comunidades.begin(); index != Comunidades.end(); index++)
    {

        if ((*index) != nullptr)
        {
            string color;
            char hex[6];
            vector<char> hexValues = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
            for (int i = 0; i < 6; i++)
            {
                hex[i] = hexValues[rand() % 16];
            }
            for (int j = 0; j < 6; j++)
            {
                color += hex[j];
            }
            string subgraph = "subgraph cluster";
            string label = to_string(count);
            output << "\t" << subgraph + label << "{" << endl;
            output << "\t"
                   << "\t"
                   << "label =  \"Community " << count << "\";" << endl;

            output << "\t"
                   << "\t"
                   << "node[color=\"#" << color << "\"];" << endl;
            output << "\t"
                   << "\t"
                   << "edge[color=\"#" << color << "\"];" << endl;
            vector<int> nodes;
            for (auto subindex = (*index)->begin(); subindex != (*index)->end(); subindex++)
            {
                nodes.push_back(nodos[*subindex]);
            }
            for (int it = 0; it < nodes.size(); it++)
            {
                if (it != nodes.size() - 1)
                {
                    output << "\t"
                           << "\t" << nodes[it] << "--{";
                    for (int it2 = it + 1; it2 < nodes.size(); it2++)
                    {

                        output << nodes[it2] << " ";
                    }
                    output << "};" << endl;
                }
            }
            output << "\t"
                   << "}" << endl;
            count++;
            color.clear();
        }
    }
    int count2 = 1;
    for (auto index = Comunidades.begin(); index != Comunidades.end(); index++)
    {
        if ((*index) != nullptr)
        {
            for (auto index = Comunidades.begin()++; index != Comunidades.end(); index++)
            {
                if ((*index) != nullptr && count2 + 1 < count)
                {
                    output << "\t"
                           << "cluster" + to_string(count2) << "--"
                           << "cluster" + to_string(count2 + 1) << ";" << endl;
                    count2++;
                }
            }
        }
    }
    output << "}" << endl;
    system("dot -Tpng Result.dot -o louvain.png");
    system("louvain.png");
    output.close();
}
list<int> *Red::get_Listaadyacencia()
{
    list<int> *punt = new list<int>[nodos.size()];
    for (int i = 0; i < nodos.size(); i++)
    {
        for (int j = 0; j < nodos.size(); j++)
        {
            if (relaciones[i][j] != 0)
            {
                punt[i].push_back(j);
            }
        }
    }
    return punt;
}
int Red::get_A(int i, int j)
{
    int index_i = -1, index_j = -1;
    auto it = this->nodos.begin();
    int count = 0;
    for (; it != this->nodos.end(); it++)
    {
        if (*it == i)
            index_i = count;
        if (*it == j)
            index_j = count;
        count++;
    }

    return relaciones[index_i][index_j];
}
int Red::get_K(int i_j)
{
    int index_i_j = -1;
    auto it = this->nodos.begin();
    int count = 0;
    for (; it != this->nodos.end(); it++)
    {
        if (*it == i_j)
            index_i_j = count;
        count++;
    }
    count = 0;
    auto it_row = this->relaciones[index_i_j].begin();
    for (; it_row != this->relaciones[index_i_j].end(); it_row++)
    {
        count += *(it_row);
    }
    return count;
}
int Red::get_A(list<int> *comunidad_i, list<int> *comunidad_j)
{
    int resul = 0;
    for (auto it = comunidad_i->begin(); it != comunidad_i->end(); it++)
        for (auto it_2 = comunidad_j->begin(); it_2 != comunidad_j->end(); it_2++)
            resul += relaciones[*it][*it_2];
    return resul;
}
int Red::get_K(list<int> *comunidad)
{
    int resul = 0;
    for (auto it = comunidad->begin(); it != comunidad->end(); it++)
    {
        auto it_row = this->relaciones[*it].begin();
        for (; it_row != this->relaciones[*it].end(); it_row++)
        {
            resul += *(it_row);
        }
    }
    return resul;
}
void Red::imprimir()
{
    for (int i = 0; i <= nodos.size(); i++)
    {
        for (int j = 0; j <= nodos.size(); j++)
        {
            if (i == 0 && j == 0)
                cout << " " << '\t';
            else if (i * j == 0)
            {
                if (i == 0)
                {
                    cout << nodos[j - 1] << '\t';
                }
                else
                {
                    cout << nodos[i - 1] << '\t';
                }
            }
            else
                cout << relaciones[i - 1][j - 1] << '\t';
        }
        cout << endl;
    }
}
Red::~Red() {}
int main()
{
    Red primero;
    for (int k = 10; k > 4; k--)
    {
        primero.agregar_nodo(k);
    }
    cout << "Representacion matricial de nuestro objeto Red" << endl;
    primero.imprimir();
    for (int k = 10; k > 5; k--)
    {
        primero.agregar_relacion(k, k - 1, 1);
    }

    primero.agregar_relacion(5, 7, 1);
    primero.agregar_relacion(10, 8, 1);
    cout << "\nRepresentacion matricial de nuestro objeto Red con nodos" << endl;
    primero.imprimir();

    cout << "Numero  de aristas: " << primero.m_num_edges << endl;
    // cout<<primero.get_A(8,7)<<endl;
    // cout<<primero.get_K(8);
    primero.Louvain_Comunidades();
}