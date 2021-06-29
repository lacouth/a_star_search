#include <algorithm>  
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

using std::cout;
using std::setw;
using std::ifstream;
using std::istringstream;
using std::sort;
using std::string;
using std::vector;
using std::abs;
using std::find;

struct No
{
    No(){}
    No(int a, int b) : i(a), j(b){}
    No(int a, int b, float c) : i(a), j(b), prioridade(c){} 
    No(int a, int b, float c, No *n): i(a), j(b),prioridade(c),anterior(n){}

    int i;
    int j;
    float prioridade;
    No *anterior;

};

bool operator==(No a, No b){
    return a.i == b.i && a.j == b.j;
}

const vector<vector<int>> delta{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

vector<vector<int>> LeMapa(string nome_arquivo) {
  ifstream arquivo (nome_arquivo);
  vector<vector<int>> mapa{};
  if (arquivo.is_open()) {
    string linha_str;
    while (getline(arquivo, linha_str)) {
        istringstream linha(linha_str);
        vector<int> linha_int{};
        int no;
        while(linha >> no){
            linha.ignore(); // para ignorar a virgula
            linha_int.push_back(no);
        }
      mapa.push_back(linha_int);
    }
  }
  return mapa;
}

string ConverterParaSimbolo(int e){
    switch(e){
        case 1: return "🚧   ";
        case 2: return "🚦   ";
        case 3: return "🏁   ";
        case 4: return "🚗   ";
        default: return "⬛   ";;
    }
}

void ImprimirMapa(const vector<vector<int>> mapa) {
  for (auto linha : mapa) {
    for (auto elemento : linha) {
      cout <<  ConverterParaSimbolo(elemento) << "  ";
    }
    cout << "\n";
  }
}

bool VerificarLimites(int i, int j, vector<vector<int>> &mapa){
    bool limite_i = i>=0 and i<mapa.size();
    bool limite_j = j>=0 and j<mapa[0].size();
    return limite_i and limite_j;
}

bool CelulaVazia(int i, int j, vector<vector<int>> &mapa){
    return mapa[i][j] == 0;
}

vector<No> BuscarVizinhos(No no, vector<vector<int>> &mapa){
    vector<No> vizinhos{};
    for(auto posicao : delta){
        int novo_i = no.i + posicao[0];
        int novo_j = no.j + posicao[1];
        if(VerificarLimites(novo_i, novo_j,mapa) and CelulaVazia(novo_i,novo_j,mapa)){
            vizinhos.push_back(No{novo_i,novo_j});
        } 
    }
    return vizinhos;
}

float Heuristica(No atual, No destino){
    return abs(atual.i - destino.i) + abs(atual.j - atual.j);
}

bool CompararPrioridade(No a, No b){
    return a.prioridade > b.prioridade;
}

void MarcarCaminho(vector<vector<int>> &mapa, vector<No> &caminho, No inicio, No destino){
    No no = caminho.back();
    while(no.anterior != nullptr){
        mapa[no.i][no.j] = 4;
        no = *no.anterior;
    }
    mapa[inicio.i][inicio.j] = 2;
    mapa[destino.i][destino.j] = 3;
}

int main(){
    No inicio{0, 0};
    No destino{4, 5, 0};

    inicio.prioridade = Heuristica(inicio,destino);
    inicio.anterior = nullptr;

    No atual;
    vector<No> fronteira{};
    vector<No> caminho{};
    int custo_g = 0;

    auto mapa = LeMapa("mapa_1.csv");
    cout << "\n============Mapa Original=========\n\n";
    ImprimirMapa(mapa);
    
    fronteira.push_back(inicio);

    while(!fronteira.empty()){
        atual = fronteira.back();
        fronteira.pop_back();
        caminho.push_back(atual);

        if(atual == destino) break;

        auto vizinhos = BuscarVizinhos(atual, mapa);
        custo_g++;
        for(auto& vizinho : vizinhos){
            float prioridade = custo_g + Heuristica(vizinho,destino);
            auto it = find(caminho.begin(), caminho.end(),vizinho);
            if (it == caminho.end() or prioridade < it->prioridade){
                vizinho.prioridade = prioridade;
                vizinho.anterior = new No{atual.i,atual.j,atual.prioridade,atual.anterior};
                if(it != caminho.end()) caminho.erase(it);
                fronteira.push_back(vizinho);
            }
        }
        sort(fronteira.begin(), fronteira.end(),CompararPrioridade);
    }
    cout << "\n\n==========Caminho==========\n\n";
    MarcarCaminho(mapa,caminho,inicio,destino);
    ImprimirMapa(mapa);
}