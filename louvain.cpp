#include <iostream>
#include <cctype>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include<math.h>
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
    void agregar_relacion(int,int,int);
    void imprimir();
    int get_A(int i, int j);//Retorna el peso de la arista entre 2 nodos
    int get_K(int i_j);
    void Louvain_Comunidades();
    list<int> *get_Listaadyacencia();
    bool get_Delta();
    ~Red();
};
Red::Red(){
    this->m_num_edges=0;
}
void Red::agregar_nodo(int v){
    if(!nodos.empty()){
        auto ubi=find(nodos.begin(),nodos.end(),v);
        if(ubi!=nodos.end())
            return;
    }
    nodos.push_back(v);
    vector<int>m;
    for (int i = 0; i < relaciones.size(); i++){
        relaciones[i].push_back(0);
        m.push_back(0);
    }
    m.push_back(0);
    relaciones.push_back(m);
    
}
void Red::agregar_relacion(int a, int b, int v){
    int index_a=-1,index_b=-1;
    if(nodos.empty())
        return;
    for (int i = 0; i < nodos.size(); i++){
        if(nodos[i]==a){
            index_a=i;
        }  
        if(nodos[i]==b){
            index_b=i;
        }
    }
    if((index_a*index_b)>=0){
        relaciones[index_a][index_b]=v;
        relaciones[index_b][index_a]=v;
        m_num_edges+=v;
    }
}
void Red::Louvain_Comunidades(){
    double Q;
    float sumatoria=0;
    vector<list<int>*> Comunidades;
    for (int k_2 = 0; k_2 < nodos.size(); k_2++){
        list<int> *m=new list<int>;
        m->push_back(nodos[k_2]);
        Comunidades.push_back(m);
        sumatoria+=((float)relaciones[k_2][k_2]-((2*(float)get_K(nodos[k_2]))/(2*m_num_edges)));
    }
    Q=sumatoria*(1/(2*m_num_edges));
    /*
    cout<<"Q: "<<Q<<endl;
    for(auto it=Comunidades.begin(); it!=Comunidades.end(); it++){
        cout<<*((*it)->begin())<<" ";
    }*/
    list<int>* L_adyacencia=get_Listaadyacencia();
    int i=0;
    while (i<nodos.size()){
        list<int>* comunidad_i=Comunidades[i];
        auto resul=L_adyacencia[i].begin();
        float dif_Q_max=0;
        for (auto it = L_adyacencia[i].begin(); it != L_adyacencia[i].end(); it++){
            if(comunidad_i!=Comunidades[*it]){
                //Paso 1: Sacar i de su comunidad
                list<int>* comunidad_j=Comunidades[*it];
                auto eliminador=find(comunidad_i->begin(),comunidad_i->end(),nodos[i]);
                comunidad_i->erase(eliminador);
                //Paso 2: Diferencia de Q
                float k_i=get_K(nodos[i]);
                float in=0;
                float tot=0;
                float k_i_in=0;
                for (auto it = comunidad_j->begin(); it != comunidad_j->end(); it++){
                    tot+=get_K(*it);
                    k_i_in+=get_A(nodos[i],*it);
                    for (auto it2 = comunidad_j->begin(); it2 != comunidad_j->end(); it2++)
                        in+=get_A(*it,*it2);    
                }
                cout<<k_i<<" --- "<<in<<" --- "<<tot<<" --- "<<k_i_in<<endl;
                float dif_Q=(((in+(2*k_i_in))/(2*m_num_edges))-pow(((tot+k_i)/(2*m_num_edges)),2))-((in/(2*m_num_edges))-pow((tot/(2*m_num_edges)),2)-pow((k_i/(2*m_num_edges)),2));
                if(dif_Q>dif_Q_max){
                    resul=it;
                    dif_Q_max=dif_Q;
                }
                comunidad_i->push_front(nodos[i]);
            }
        }
        if(dif_Q_max!=0){
            Q+=dif_Q_max;
            auto eliminador=find(comunidad_i->begin(),comunidad_i->end(),nodos[i]);
            Comunidades[i]->erase(eliminador);
            Comunidades[*resul]->push_back(nodos[i]);
            for (auto it1 = Comunidades[i]->begin(); it1 != Comunidades[i]->end(); it1++){
                cout<<*it1<<" ";
            }
            cout<<" ------ ";
            for (auto it1 = Comunidades[*resul]->begin(); it1 != Comunidades[*resul]->end(); it1++){
                cout<<*it1<<" ";
            }
            cout<<endl;
            Comunidades[i]=Comunidades[*resul];
        }
        else{
            comunidad_i->push_back(nodos[i]);
        }
        i++;
    }
    //Esto sirve para separar Comunidades entre si, ya que al usar punteros se señalan a si mismos y
    //las comunidades se repiten
    list<list<int>*> Comunidades_resul;
    for (auto it_1 = Comunidades.begin(); it_1 != Comunidades.end(); it_1++){
        list<int>*punt=(*it_1);
        bool b=true;
        for (auto it_2 = Comunidades_resul.begin(); it_2 != Comunidades_resul.end(); it_2++){
            if(punt==(*it_2)){
                b=false;
            }
        }
        if(b){
            Comunidades_resul.push_back(*it_1);
        }
    }
    //Trabajo terminado amigos;
    for (auto it_1 = Comunidades_resul.begin(); it_1 != Comunidades_resul.end(); it_1++){
        for (auto it_2 = (*it_1)->begin(); it_2 != (*it_1)->end(); it_2++){
            cout<<*it_2<<" ";
        }
        cout<<endl;
    }
}
list<int> *Red::get_Listaadyacencia(){
    list<int>*punt=new list<int>[nodos.size()];
    for (int i = 0; i < nodos.size(); i++){
        for (int j = 0; j < nodos.size(); j++){
            if(relaciones[i][j]!=0){
                punt[i].push_back(j);
            }
        }
    }
    return punt;
}
void Red::imprimir(){
    for (int i = 0; i <= nodos.size(); i++){
        for (int j = 0; j <= nodos.size(); j++){
            if(i==0&&j==0)
                cout<<" "<<'\t';
            else if(i*j==0){
                if(i==0){
                    cout<<nodos[j-1]<<'\t';
                }
                else{
                    cout<<nodos[i-1]<<'\t';
                }
            }
            else
                cout<<relaciones[i-1][j-1]<<'\t';
        }
        cout<<endl;
    }
}
int Red::get_A(int i, int j){
    

    int index_i=-1,index_j=-1;
    
    auto it=this->nodos.begin();
    
    
    int count=0;
    for(;it!=this->nodos.end();it++){
        if(*it==i)index_i=count;
        if(*it==j)index_j=count;
        count++;
    }
    
    return relaciones[index_i][index_j];   
}
int Red::get_K(int i_j){
    int index_i_j=-1;
    auto it=this->nodos.begin();
    int count=0;
    for(;it!=this->nodos.end();it++){
        if(*it==i_j)index_i_j=count;
        count++;
    }
    count=0;
    auto it_row=this->relaciones[index_i_j].begin();
    for(;it_row!=this->relaciones[index_i_j].end();it_row++){
        count+=*(it_row);
    }
    return count;
}
Red::~Red(){}
int main(){
    Red primero;
    for (int k = 10; k > 4; k--){
        primero.agregar_nodo(k);
    }
    cout<<"Representacion matricial de nuestro objeto Red"<<endl;
    primero.imprimir();
    for (int k = 10; k >5; k--){
        primero.agregar_relacion(k,k-1,1);
    }
    
    primero.agregar_relacion(5,7,1);
    primero.agregar_relacion(10,8,1);
    cout<<"\nRepresentacion matricial de nuestro objeto Red con nodos"<<endl;
    primero.imprimir();


    cout<<"Numero  de aristas: "<<primero.m_num_edges<<endl;
    // cout<<primero.get_A(8,7)<<endl;
    // cout<<primero.get_K(8);
    primero.Louvain_Comunidades();
    
}