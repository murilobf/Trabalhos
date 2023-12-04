#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <locale>
#include <algorithm>
#include <cctype>
#include <windows.h>

#define CAMINHO "Historia.txt"

using namespace std;

class Palavra
{
    public: string palavra = "";
    int posicoes[100];
    int quant = 0;
    int numOcorr = 0;

    Palavra()
    {
        for(int ind = 0; ind < 100; ind++)
        {
            posicoes[ind] = -1;
        }
    }
    void adicionaPos(int pos)
    {
        posicoes[quant++] = pos;
    }
};

int verificaCaracter(char caractere)
{
    if(caractere != ' ' && caractere != '.' && caractere != ',' && caractere != '!' && caractere != '?' && caractere != '-' && caractere != ':' && caractere != '\n' && caractere !='\"' && caractere != '\\')
        return 1;
    return 0;
}

void leTexto(Palavra *listaPalavras,string *texto)
{
    string caminho;
    int escolha;

    cout << "Insira 1 para usar o texto padrao e 2 para um outro texto\n";
    cin >> escolha;

    if(escolha == 1)
        caminho = CAMINHO;
    else if (escolha == 2)
        cin >> caminho;
    else
    {
        cout << "Escolha invalida";
        return;
    }

    /// Insere todo o texto numa única string a partir do .txt
    string line;

    ifstream myfile (caminho);
    if (myfile.is_open())
    {
        for(int i=0; getline(myfile,line);i++)
        {
            (*texto) += line;
        }
    }
}

int emLista(Palavra *listaPalavras,string palavra)
{
    int i = 0;
    while(listaPalavras[i].palavra != "")
    {
        if(listaPalavras[i].palavra == palavra)
            return 1;
        i++;
    }
    return 0;
}

int retornaIndex(Palavra *listaPalavras,string palavra)
{
    int i = 0;
    while(listaPalavras[i].palavra != "")
    {
        if(listaPalavras[i].palavra == palavra)
            return i;
        i++;
    }
}

void criaIndice(Palavra *listaPalavras,string texto)
{
    ///Zera todos os campos da lista de palavras pra evitar lixo;
    for(int i = 0; i < 1000; i++)
    {
       // listaPalavras[i] = new Palavra();
        listaPalavras[i].palavra = "";
    }

    int tamLista = 0;

    for(int i=0; i < texto.size();i++)
    {
        /// Vê se o caractere atual é uma letra
        if(verificaCaracter(texto[i]))
        {
            string aux = "";
            int pos = i;
            while(verificaCaracter(texto[i]))
            {
                aux +=  tolower(texto[i]);
                i++;
            }

            /// Caso a palavra não esteja na lista de palavras, adiciona-a nela
            if(!emLista(listaPalavras,aux))
            {
                listaPalavras[tamLista].palavra = aux;
                listaPalavras[tamLista++].adicionaPos(pos);
            }

            else
            {
                int index = retornaIndex(listaPalavras,aux);

                listaPalavras[index].adicionaPos(pos);
            }
        }
    }
}

void ordenaLista(Palavra *listaPalavras)
{
    /// Como não há muitas palavras, foi usado o algoritmo insertion sort

    for(int i=1;listaPalavras[i].palavra != "";i++)
    {
        int j = i-1;
        Palavra aux = listaPalavras[i];

        while(aux.palavra < listaPalavras[j].palavra && j >= 0)
        {
            listaPalavras[j+1] = listaPalavras[j];
            j--;
        }
        listaPalavras[j+1] = aux;
        //(*mov)++;
    }
}

void veTamanho(Palavra *listaPalavras,int *tam)
{
    int i = 0;
    while(listaPalavras[i].palavra != "")
    {
        (*tam)++;
        i++;
    }
}

void mostraArqInver(Palavra *listaPalavras, int tam)
{
    for(int i = 0; i < tam; i++)
    {
            cout << "\n\nPalavra: " << listaPalavras[i].palavra << "\nPosicao(oes): ";
            int j = 0;
            while(listaPalavras[i].posicoes[j] != -1)
                cout << listaPalavras[i].posicoes[j++] << " ";
    }
}

string pegaFrase(string texto, Palavra *palavra)
{
    string pedaco = "";
    int inicio;
    int fim;
    int pos = (*palavra).posicoes[(*palavra).numOcorr];

    /// Define o valor em que a frase começará ou terminará com um valor qualquer
    inicio = pos-100;
    fim = pos+100;

    /// Isso faz com que, caso o valor seja invalido, ele seja modificado até que se torne valido
    while(inicio < 0 )
        inicio++;
    while(fim > texto.size())
        fim--;

    for(int i = inicio; i < fim; i++)
        pedaco += texto[i];

    return pedaco;
}

void procuraArqInver(Palavra *listaPalavras,int tamanho,string texto,Palavra *ocorrencia)
{
    int encontrado = 0;

    string palavra;
    cout << "Escolha a palavra que deseja procurar: \n";

    cin >> palavra;
    /// Torna a palavra toda minuscula pra evitar diferenca
    transform(palavra.begin(),palavra.end(),palavra.begin(), ::tolower);

    for(int i=0; i < tamanho;i++)
    {
        if(listaPalavras[i].palavra == palavra)
        {
            (*ocorrencia) = listaPalavras[i];
            (*ocorrencia).numOcorr = 0;
            encontrado = 1;
            cout << "\nQuantidade de ocorrencias da palavra: " << listaPalavras[i].quant;
            cout << "\nPosicao da primeira ocorrencia da palavra: " << listaPalavras[i].posicoes[0];
            cout << "\nPedaco da frase : (...)" << pegaFrase(texto,ocorrencia) << "(...)\n";
        }
    }

    if(!encontrado)
    {
        cout << "\nPalavra nao encontrada.\n";
    }
}

void procuraProxOcorrencia(Palavra *ocorrencia,string texto)
{
    if((*ocorrencia).posicoes[(*ocorrencia).numOcorr+1] != -1)
    {
        (*ocorrencia).numOcorr++;
        cout << "\nA proxima ocorrencia da palavra eh na posicao: " << (*ocorrencia).posicoes[(*ocorrencia).numOcorr];
        cout << "\nPedaco da frase: (...)" << pegaFrase(texto,ocorrencia) << "(...)\n";
    }
    else
        cout << "\nNao ha proxima ocorrencia dessa palavra\n";
}

int main()
{
    SetConsoleCP(CP_UTF8); // Input codepage (affects cin, scanf, etc.)
    SetConsoleOutputCP(CP_UTF8); // Output codepage (affects cout, printf, etc.)
    Palavra *listaPalavras = new Palavra[1000];
    string texto = "";
    string *ptrTexto = &texto;
    int tam = 0;
    int *ptrTam = &tam;
    Palavra ocorrencia;
    Palavra *ptrOcorrencia = &ocorrencia;
    int escolha = 0;

    while(escolha != 5)
    {
        cout << "\n================================================\n";
        cout << "Escolha qual acao realizar:\n";
        cout << "1 Para ler um arquivo texto\n";
        cout << "2 Para mostrar o arquivo invertido\n";
        cout << "3 Para procurar uma palavra no arquivo invertido\n";
        cout << "4 Para procurar a proxima ocorrencia da palavra\n";
        cout << "5 Para sair do programa\n";
        cout << "================================================\n";
        cin >> escolha;

        if(escolha == 1)
        {
            leTexto(listaPalavras,ptrTexto);
            criaIndice(listaPalavras,texto);
            veTamanho(listaPalavras,ptrTam);
            ordenaLista(listaPalavras);
        }

        else if(escolha == 2)
        {
            mostraArqInver(listaPalavras,tam);
        }

        else if(escolha == 3)
        {
            procuraArqInver(listaPalavras,tam,texto,ptrOcorrencia);
        }

        else if(escolha == 4)
        {
            procuraProxOcorrencia(ptrOcorrencia,texto);
        }
    }
}
