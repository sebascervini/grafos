#include <iostream>
#include <fstream>
#include <filesystem>
#include <queue>
#include <algorithm>


using namespace std;

struct Grafo {
    int numVertices;
    int** matrizAdyacencia;
};

//Mostrar ubicacion del .txt


Grafo* leerGrafoDesdeArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo) {
        cout << "El archivo no existe. Creando archivo..." << endl;
        ofstream nuevoArchivo(nombreArchivo);
        if (!nuevoArchivo) {
            cout << "Error al crear el archivo." << endl;
            return nullptr;
        }
        nuevoArchivo.close();
        archivo.open(nombreArchivo);
        if (!archivo) {
            cout << "Error al abrir el archivo." << endl;
            return nullptr;
        }
    }

    int numVertices;
    archivo >> numVertices;

    Grafo* grafo = new Grafo;
    grafo->numVertices = numVertices;
    grafo->matrizAdyacencia = new int*[numVertices];
    for (int i = 0; i < numVertices; i++) {
        grafo->matrizAdyacencia[i] = new int[numVertices];
        for (int j = 0; j < numVertices; j++) {
            grafo->matrizAdyacencia[i][j] = 0;
        }
    }

    int origen, destino, peso;
    for (int i = 0; i < numVertices; i++) {
        archivo >> origen >> destino >> peso;
        grafo->matrizAdyacencia[origen - 1][destino - 1] = peso;
        grafo->matrizAdyacencia[destino - 1][origen - 1] = peso;
    }

    archivo.close();
    return grafo;
}

//Recorrer grafo con algoritmo Prim
void recorrerGrafoPrim(Grafo* grafo) {
    if (grafo == nullptr) {
        cout << "Grafo no inicializado." << endl;
        return;
    }

    int* padre = new int[grafo->numVertices];
    int* llave = new int[grafo->numVertices];
    bool* conjuntoCerrado = new bool[grafo->numVertices];

    for (int i = 0; i < grafo->numVertices; i++) {
        llave[i] = INT_MAX;
        conjuntoCerrado[i] = false;
    }

    llave[0] = 0;
    padre[0] = -1;

    for (int i = 0; i < grafo->numVertices - 1; i++) {
        int minimo = INT_MAX;
        int minimoIndice;

        for (int j = 0; j < grafo->numVertices; j++) {
            if (!conjuntoCerrado[j] && llave[j] < minimo) {
                minimo = llave[j];
                minimoIndice = j;
            }
        }

        conjuntoCerrado[minimoIndice] = true;

        for (int j = 0; j < grafo->numVertices; j++) {
            if (grafo->matrizAdyacencia[minimoIndice][j] != 0 && !conjuntoCerrado[j] && grafo->matrizAdyacencia[minimoIndice][j] < llave[j]) {
                padre[j] = minimoIndice;
                llave[j] = grafo->matrizAdyacencia[minimoIndice][j];
            }
        }
    }

    cout << "Aristas del árbol de expansión mínima:" << endl;
    for (int i = 1; i < grafo->numVertices; i++) {
        cout << padre[i] + 1 << " - " << i + 1 << " (Peso: " << grafo->matrizAdyacencia[i][padre[i]] << ")" << endl;
    }
}

//Recorrer grafo con algoritmo Kruskal
void recorrerGrafoKruskal(Grafo* grafo) {
    if (grafo == nullptr) {
        cout << "Grafo no inicializado." << endl;
        return;
    }

    int* padre = new int[grafo->numVertices];
    int* rango = new int[grafo->numVertices];
    vector<pair<int, pair<int, int>>> aristas;

    for (int i = 0; i < grafo->numVertices; i++) {
        padre[i] = i;
        rango[i] = 0;
    }

    for (int i = 0; i < grafo->numVertices; i++) {
        for (int j = i + 1; j < grafo->numVertices; j++) {
            if (grafo->matrizAdyacencia[i][j] != 0) {
                aristas.push_back({grafo->matrizAdyacencia[i][j], {i, j}});
            }
        }
    }

    sort(aristas.begin(), aristas.end());

    int pesoMinimoTotal = 0;
    cout << "Aristas del árbol de expansión mínima:" << endl;
    for (int i = 0; i < aristas.size(); i++) {
        int origen = aristas[i].second.first;
        int destino = aristas[i].second.second;

        int origenPadre = origen;
        while (origenPadre != padre[origenPadre]) {
            origenPadre = padre[origenPadre];
        }

        int destinoPadre = destino;
        while (destinoPadre != padre[destinoPadre]) {
            destinoPadre = padre[destinoPadre];
        }

        if (origenPadre != destinoPadre) {
            cout << "(" << origen + 1 << ", " << destino + 1 << ", " << aristas[i].first << ")" << endl;
            pesoMinimoTotal += aristas[i].first;

            if (rango[origenPadre] < rango[destinoPadre]) {
                padre[origenPadre] = destinoPadre;
            } else if (rango[origenPadre] > rango[destinoPadre]) {
                padre[destinoPadre] = origenPadre;
            } else {
                padre[destinoPadre] = origenPadre;
                rango[origenPadre]++;
            }
        }
    }

    cout << "Peso mínimo total: " << pesoMinimoTotal << endl;
}

//Recorrido algoritmo Dijkstra
void recorrerGrafoDijkstra(Grafo* grafo, int vertice) {
    if (grafo == nullptr) {
        cout << "Grafo no inicializado." << endl;
        return;
    }

    int* distancia = new int[grafo->numVertices];
    bool* conjuntoCerrado = new bool[grafo->numVertices];

    for (int i = 0; i < grafo->numVertices; i++) {
        distancia[i] = INT_MAX;
        conjuntoCerrado[i] = false;
    }

    distancia[vertice] = 0;

    for (int i = 0; i < grafo->numVertices - 1; i++) {
        int minimo = INT_MAX;
        int minimoIndice;

        for (int j = 0; j < grafo->numVertices; j++) {
            if (!conjuntoCerrado[j] && distancia[j] <= minimo) {
                minimo = distancia[j];
                minimoIndice = j;
            }
        }

        conjuntoCerrado[minimoIndice] = true;

        for (int j = 0; j < grafo->numVertices; j++) {
            if (!conjuntoCerrado[j] && grafo->matrizAdyacencia[minimoIndice][j] != 0 && distancia[minimoIndice] != INT_MAX && distancia[minimoIndice] + grafo->matrizAdyacencia[minimoIndice][j] < distancia[j]) {
                distancia[j] = distancia[minimoIndice] + grafo->matrizAdyacencia[minimoIndice][j];
            }
        }
    }

    cout << "Distancias más cortas desde el vértice " << vertice + 1 << ":" << endl;
    for (int i = 0; i < grafo->numVertices; i++) {
        cout << vertice + 1 << " - " << i + 1 << ": " << distancia[i] << endl;
    }
}

//Mostrar ubicacion del archivo creado en los archivos
void mostrarUbicacionArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo) {
        cout << "El archivo no existe." << endl;
        return;
    }

    string linea;
    while (getline(archivo, linea)) {
        cout << linea << endl;
    }

    archivo.close();
}

//Recorrer grafo en profundidad
void recorrerGrafoProfundidad(Grafo* grafo, int vertice, bool* visitado) {
    if (grafo == nullptr) {
        cout << "Grafo no inicializado." << endl;
        return;
    }

    visitado[vertice] = true;
    cout << vertice + 1 << " ";

    for (int i = 0; i < grafo->numVertices; i++) {
        if (grafo->matrizAdyacencia[vertice][i] != 0 && !visitado[i]) {
            recorrerGrafoProfundidad(grafo, i, visitado);
        }
    }
}

//Recorrer grafo en anchura


void recorrerGrafoAnchura(Grafo* grafo, int vertice, bool* visitado) {
    if (grafo == nullptr) {
        cout << "Grafo no inicializado." << endl;
        return;
    }

    queue<int> cola;
    cola.push(vertice);
    visitado[vertice] = true;

    while (!cola.empty()) {
        int actual = cola.front();
        cola.pop();
        cout << actual + 1 << " ";

        for (int i = 0; i < grafo->numVertices; i++) {
            if (grafo->matrizAdyacencia[actual][i] != 0 && !visitado[i]) {
                cola.push(i);
                visitado[i] = true;
            }
        }
    }
}


//Mostrar grafo
void mostrarGrafo(Grafo* grafo) {
    if (grafo == nullptr) {
        cout << "Grafo no inicializado." << endl;
        return;
    }
    
    cout << "Matriz de adyacencia:" << endl;
    for (int i = 0; i < grafo->numVertices; i++) {
        for (int j = 0; j < grafo->numVertices; j++) {
            cout << grafo->matrizAdyacencia[i][j] << " ";
        }
        cout << endl;
    }
}

//Menu
void menu(){
    cout << ".:Menu de opciones:.\n";
    cout << "1. Recorrer grafo en profundidad\n";
    cout << "2. Recorrer grafo en anchura\n";
    cout << "3. Recorrer grafo en Kruskal\n";
    cout << "4. Recorrer grafo en Prim\n";
    cout << "5. Recorrer grafo en Dijkstra\n";
    cout << "6. Mostrar grafo\n";
    cout << "7. Ubicacion grafo\n";
    cout << "0. Salir\n";
    cout << "Ingrese una opcion: ";
}

int main() {
    string nombreArchivo = "grafo.txt";
    string rutaArchivo = filesystem::canonical(nombreArchivo).string();
    int n4, n3;
    Grafo* grafo = nullptr; // Move the declaration of grafo outside of the switch statement
    bool* visitado = nullptr; // Move the declaration of visitado outside of the switch statement
    do{
        menu();
        cin >> n4;
        switch (n4)
        {
        case 1:
            cout << "Recorrer grafo en profundidad\n";
            grafo = leerGrafoDesdeArchivo("grafo.txt"); // Remove the redeclaration of grafo
            visitado = new bool[grafo->numVertices];
            for (int i = 0; i < grafo->numVertices; i++) {
                visitado[i] = false;
            }
            recorrerGrafoProfundidad(grafo, 0, visitado);
            cout << endl;
            break;
        case 2:
            cout << "Recorrer grafo en anchura\n";
            grafo = leerGrafoDesdeArchivo("grafo.txt"); // Remove the redeclaration of grafo
            visitado = new bool[grafo->numVertices];
            for (int i = 0; i < grafo->numVertices; i++) {
                visitado[i] = false;
            }
            recorrerGrafoAnchura(grafo, 0, visitado);
            cout << endl;
            break;
        case 3:
            cout << "Recorrer grafo en Kruskal\n";
            grafo = leerGrafoDesdeArchivo("grafo.txt"); // Remove the redeclaration of grafo
            recorrerGrafoKruskal(grafo);
            cout << endl;
            break;
        case 4:
            cout << "Recorrer grafo en Prim\n";
            grafo = leerGrafoDesdeArchivo("grafo.txt"); // Remove the redeclaration of grafo
            recorrerGrafoPrim(grafo);
            cout << endl;
            break;
        case 5:
            cout << "Recorrer grafo en Dijkstra\n";
            grafo = leerGrafoDesdeArchivo("grafo.txt"); // Remove the redeclaration of grafo
            recorrerGrafoDijkstra(grafo, 0);
            cout << endl;
            break;
        case 6:
            cout << "Mostrar grafo\n";
            grafo = leerGrafoDesdeArchivo("grafo.txt"); // Remove the redeclaration of grafo
            mostrarGrafo(grafo);
            break;
        case 7:
            cout << "La ruta del archivo es: " << rutaArchivo << endl;
            break;
        case 0:
            cout << "Saliendo del programa...";
            break;
        default:
            cout << "Opcion no valida. Intente de nuevo.\n";
            break;
        }
    } while (n4 != 0);
}
