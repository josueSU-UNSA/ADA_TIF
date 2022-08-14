#include <iostream>
#include <cctype>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;
class Red
{
private:
    vector<int> nodos;
    vector<vector<int>> relaciones;
public:
    Red();
    void agregar_nodo(int);
    void agregar_relacion(int,int,int);
    void imprimir();
    ~Red();
};
Red::Red(){}
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
    if(nodos.empty())
        return;
    auto ubi_a=find(nodos.begin(),nodos.end(),a);
    auto ubi_b=find(nodos.begin(),nodos.end(),b);
    if(ubi_a!=nodos.end()&&ubi_b!=nodos.end()){
        int a_d=0,b_d=0;
        bool c=true,d=true;
        for (auto i = nodos.begin(); i !=nodos.end(); i++){
            if(i!=ubi_a&&c)
                a_d++;
            else
                c=false;
            if(i!=ubi_b&&d)
                b_d++;
            else
                d=false;
            if(!(c||d))
                break;
        }
        relaciones[a_d][b_d]=v;
        relaciones[b_d][a_d]=v;
    }
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
    primero.agregar_relacion(10,8,1);
    primero.agregar_relacion(5,7,1);
    cout<<"\nRepresentacion matricial de nuestro objeto Red con nodos"<<endl;
    primero.imprimir();
}