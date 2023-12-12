#include <stdbool.h> // TYP BOOL
#include <stdio.h>   // INPUT & OUTPUT
#include <stdlib.h>  // MALLOC
#include <time.h>    // RANDOM

// KOLORY WYJSCIA I WEJSCIA Z LABIRYNTU
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_RESET "\x1b[0m"

// MAKSYMALNA WIELKOSC TABLICY LABIRYNTU
#define MAX_SIZE 100

// MAKSYMALNA WAGA SCIEZKI
#define MAX_DIST 1000.01

typedef struct cell cell; // PRE-DEKLARACJA STRUKTURY

// STRUKTURA KOMORKI LABIRYNTU
struct cell {
    int id;
    double weight;
    char content;
    int x;
    int y;
    bool isVisited;
    bool isStart;
    bool isEnd;
    cell *neighbours[4];
    int neighboursCount;
};

// STRUKTURA WEZLA GRAFU
struct Node {
    int data;
    double weight;
    struct Node *next;
};

// STRUKTURA GRAFU
struct Graph {
    int vertices;
    struct Node **adjacency_list;
};

// NUMEROWANIE KOMOREK LABIRYNTU OD 1
void idCells(cell *cells, int cellCount) {
    for (int i = 0; i < cellCount; i++) {
        cells[i].id = i + 1;
    }
}

// USTAWIENIE WSZYSTKICH KOMOREK NA "NIEODWIEDZONE"
void unvisitAllCells(cell *cells, int cellCount) {
    for (int i = 0; i < cellCount; i++) {
        cells[i].isVisited = 0;
    }
}

// LOSOWE PRZYPISANIE WAG KOMORKOM LABIRYNTU
void assignWeightsToCells(cell *cells, int cellCount) {
    srand(time(NULL));

    for (int i = 0; i < cellCount; i++) {
        int randomInt = rand() % 1001;
        double randomDouble = (double)randomInt / 100.0;
        cells[i].weight = randomDouble;

        if (randomDouble == 0)
            i--;
    }
}

// WYPEŁNIENIE KOMOREK ZNAKIEM
void fillCells(cell *cells, int cellCount) {
    for (int i = 0; i < cellCount; i++) {
        cells[i].content = '.';
    }
}

// LOSOWE WYBRANIE WEJSCIA I WYJSCIA LABIRYNTU
void chooseGates(cell *cells, int cellWidth, int cellCount) {
    srand(time(NULL));

    for (int i = 0; i < cellCount; i++) {
        cells[i].isStart = 0;
        cells[i].isEnd = 0;
    }

    int s = rand() % cellWidth;
    cells[s].isStart = 1;
    cells[s].content = '@';

    int e = rand() % cellWidth;
    cells[cellCount - 1 - e].isEnd = 1;
    cells[cellCount - 1 - e].content = '@';
}

// GENERACJA SUROWEGO LABIRYNTU (SCIANY WSZEDZIE)
void mazeGen(cell *cells, int mazeSize, char maze[MAX_SIZE][MAX_SIZE]) {
    int cellsIteration = 0;
    for (int i = 0; i < mazeSize; i++) {
        for (int j = 0; j < mazeSize; j++) {
            if (i % 2 == 0 || j % 2 == 0) {
                maze[i][j] = '#';
            } else {
                maze[i][j] = cells[cellsIteration].content;
                cells[cellsIteration].x = i;
                cells[cellsIteration].y = j;
                cellsIteration++;
            }
        }
    }
}

// WYSWIETLENIE GRAFICZNE LABIRYNTU
void mazePrint(char maze[MAX_SIZE][MAX_SIZE], int mazeSize) {
    for (int i = 0; i < mazeSize; i++) {
        for (int j = 0; j < mazeSize; j++) {
            if (i == 1 && maze[i - 1][j] == ' ') {
                printf(COLOR_GREEN " %c " COLOR_RESET, maze[i][j]);
            } else if (i == mazeSize - 2 && maze[i + 1][j] == ' ') {
                printf(COLOR_RED " %c " COLOR_RESET, maze[i][j]);
            } else {
                printf(" %c ", maze[i][j]);
            }
        }
        printf("\n\n");
    }
    printf("\n");
}

// OTWORZENIE WEJSCIA I WYJSCIA LABIRYNTU
void openGates(cell *cells, char maze[MAX_SIZE][MAX_SIZE], int cellCount) {
    for (int i = 0; i < cellCount; i++) {
        if (cells[i].isStart == 1) {
            maze[cells[i].x - 1][cells[i].y] = ' ';
        }
        if (cells[i].isEnd == 1) {
            maze[cells[i].x + 1][cells[i].y] = ' ';
        }
    }
}
// SPRAWDZENIE CZY WSZYSTKIE KOMORKI LABIRYNTU SA "ODWIEDZONE"
bool allCellsVisited(cell *cells, int cellCount) {
    for (int i = 0; i < cellCount; i++) {
        if (cells[i].isVisited == 0) {
            return 0;
        }
    }
    return 1;
}
// SPRAWDZENIE CZY KOMORKA ISTNIEJE
bool isCellValid(cell cell, int cellCount) {
    if (cell.id > 0 && cell.id <= cellCount) {
        return 1;
    }
    return 0;
}

// PRZYPISANIE SĄSIADOW KOMORKOM
void setNeighbours(cell *cells, int cellCount, int cellWidth) {
    for (int i = 0; i < cellCount; i++) {
        for (int j = 0; j < 4; j++) {
            cells[i].neighbours[j] = NULL;
        }
    }

    for (int i = 0; i < cellCount; i++) {
        int j = 0;
        int up = i - cellWidth;
        if (isCellValid(cells[up], cellCount)) {
            cells[i].neighbours[j] = &cells[up];
            j++;
        }

        int right = i + 1;
        if (isCellValid(cells[right], cellCount)) {
            cells[i].neighbours[j] = &cells[right];
            j++;
        }

        int down = i + cellWidth;
        if (isCellValid(cells[down], cellCount)) {
            cells[i].neighbours[j] = &cells[down];
            j++;
        }

        int left = i - 1;
        if (isCellValid(cells[left], cellCount)) {
            cells[i].neighbours[j] = &cells[left];
            j++;
        }
        cells[i].neighboursCount = j;
    }
}

// SPRAWDZENIE CZY KOMORKA MA "NIEODWIEDZONYCH" SASIADOW
bool hasUnvisitedNeighbour(cell cell, int cellCount) {
    for (int i = 0; i < 4; i++) {
        if (cell.neighbours[i] != NULL) {
            if (cell.neighbours[i]->isVisited == 0) {
                // printf("tak");
                return 1;
            }
        }
    }
    return 0;
}

// ZNISZCZENIE SCIANY MIEDZY DWOMA KOMORKAMI
void breakWall(cell current, cell adjacent, char maze[MAX_SIZE][MAX_SIZE]) {
    if (adjacent.x < current.x) {
        maze[current.x - 1][current.y] = ' ';
    }
    if (adjacent.x > current.x) {
        maze[current.x + 1][current.y] = ' ';
    }
    if (adjacent.y < current.y) {
        maze[current.x][current.y - 1] = ' ';
    }
    if (adjacent.y > current.y) {
        maze[current.x][current.y + 1] = ' ';
    }
}

// ZWROCENIE KOMORKI O PODANYCH KOORDYNATACH
cell returnCellByCoordinates(cell *cells, int x, int y, int cellCount) {
    for (int i = 0; i < cellCount; i++) {
        if (cells[i].x == x && cells[i].y == y) {
            return cells[i];
        }
    }
    return cells[0];
}

// USTAWIENIE KOMORKI O PODANYCH KOORDYNATACH JAKO "ODWIEDZONA"
void setCellAsVisitedByCoordinates(cell *cells, int x, int y, int cellCount) {
    for (int i = 0; i < cellCount; i++) {
        if (cells[i].x == x && cells[i].y == y) {
            cells[i].isVisited = 1;
        }
    }
}

// ZWROCENIE KOMORKI O PODANYM NUMERZE
cell returnCellById(cell *cells, int id, int cellCount) {
    for (int i = 0; i < cellCount; i++) {
        if (cells[i].id == id) {
            return cells[i];
        }
    }
    return cells[0];
}

// STWORZENIE GRAFU
struct Graph *createGraph(int vertices) {
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->vertices = vertices;
    graph->adjacency_list =
            (struct Node **)malloc(vertices * sizeof(struct Node *));

    for (int i = 0; i < vertices; ++i) {
        graph->adjacency_list[i] = NULL;
    }
    return graph;
}

// DODAWANIE KRAWEDZI GRAFU
void addEdge(struct Graph *graph, int src, int dest, double weight) {
    // Add an edge from src to dest
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data = dest;
    newNode->weight = weight;
    newNode->next = graph->adjacency_list[src - 1];
    graph->adjacency_list[src - 1] = newNode;
}

// WYSWIETLENIE LISTY SASIEDZTWA GRAFU
void printGraph(struct Graph *graph, cell *cells, int cellCount) {
    printf("LABIRYNT W FORMIE GRAFU (LISTA SASIEDZTWA):\n\n");
    for (int i = 0; i < graph->vertices; ++i) {
        printf("[%d] ", i + 1);

        cell tmp = returnCellById(cells, i + 1, cellCount);

        if (tmp.isStart) {
            //printf("- start (%.2f)", tmp.weight);
        }
        if (tmp.isEnd) {
            //printf("- koniec");
        }
        printf(":\n");

        struct Node *temp = graph->adjacency_list[i];
        while (temp) {
            //printf("  -> [%d] (%.2f) ", temp->data, temp->weight);
            printf(" -> [%d] ", temp->data);

            if (returnCellById(cells, temp->data, cellCount).isEnd) {
                //printf("(koniec)");
            }
            printf("\n");
            temp = temp->next;
        }
        printf("\n");
    }
}

// ZWOLNIENIE PAMIECI ZAALOKOWANEJ DLA GRAFU
void freeGraph(struct Graph *graph) {
    for (int i = 0; i < graph->vertices; ++i) {
        struct Node *current = graph->adjacency_list[i];
        while (current) {
            struct Node *next = current->next;
            free(current);
            current = next;
        }
    }
    free(graph->adjacency_list);
    free(graph);
}

// USTAWIENIE KOMORKI O PODANYM NUMERZE JAKO "ODWIEDZONEJ"
void setCellAsVisitedById(cell *cells, int id, int cellCount) {
    for (int i = 0; i < cellCount; i++) {
        if (cells[i].id == id) {
            cells[i].isVisited = 1;
        }
    }
}

// SPRAWDZENIE CZY POMIEDZY KOMORKAMI JEST SCIANA
bool isWallBetween(cell current, cell adjacent, char maze[MAX_SIZE][MAX_SIZE]) {
    if (adjacent.x < current.x) {
        if (maze[current.x - 1][current.y] == ' ') {
            return 0;
        }
    }
    if (adjacent.x > current.x) {
        if (maze[current.x + 1][current.y] == ' ') {
            return 0;
        }
    }
    if (adjacent.y < current.y) {
        if (maze[current.x][current.y - 1] == ' ') {
            return 0;
        }
    }
    if (adjacent.y > current.y) {
        if (maze[current.x][current.y + 1] == ' ') {
            return 0;
        }
    }

    return 1;
}

// LOSOWE OTWIERANIE SCIEZEK W LABIRYNCIE + GENEROWANIE GRAFU LABIRYNTU
void randomPathOpenAndGraphGen(cell *cells, char maze[MAX_SIZE][MAX_SIZE], int mazeSize, int cellCount, int cellWidth, struct Graph *graph) {
    srand(time(NULL));
    int randomCellIndex = rand() % cellCount;

    for (int i = 0; i < cellCount; i++) {
        if (cells[i].isStart) {
            randomCellIndex = cells[i].id - 1;
        }
    }

    cell current = cells[randomCellIndex];

    while (!allCellsVisited(cells, cellCount)) {
        setCellAsVisitedByCoordinates(cells, current.x, current.y, cellCount);

        int direction = rand() % 4;
        int move = 0;
        cell next = current;

        switch (direction) {
        // GORA
        case 0:
            move = current.x - 2;
            if (move > 0) {
                next = returnCellByCoordinates(cells, move, current.y, cellCount);
                if (!next.isVisited) {
                    breakWall(current, next, maze);
                    addEdge(graph, current.id, next.id, next.weight);
                }
                current = next;
            }
            break;
        // PRAWO
        case 1:
            move = current.y + 2;
            if (move < mazeSize - 1) {
                next = returnCellByCoordinates(cells, current.x, move, cellCount);
                if (!next.isVisited) {
                    breakWall(current, next, maze);
                    addEdge(graph, current.id, next.id, next.weight);
                }
                current = next;
            }
            break;
        // DOL
        case 2:
            move = current.x + 2;
            if (move < mazeSize - 1) {
                next = returnCellByCoordinates(cells, move, current.y, cellCount);
                if (!next.isVisited) {
                    breakWall(current, next, maze);
                    addEdge(graph, current.id, next.id, next.weight);
                }
                current = next;
            }
            break;
        // LEWO
        case 3:
            move = current.y - 2;
            if (move > 0) {
                next = returnCellByCoordinates(cells, current.x, move, cellCount);
                if (!next.isVisited) {
                    breakWall(current, next, maze);
                    addEdge(graph, current.id, next.id, next.weight);
                }
                current = next;
            }
            break;
        default:
            break;
        }
    }
    return;
}

// ALGORYTM DIJKSTRY (NAJKROTSZA SCIEZKA MIEDZY DWOMA WEZLAMI SKIEROWANEGO GRAFU WAZONEGO)
void dijkstra(struct Graph *graph, int start, int end, int *path, double *distance) {
    
    // WSTEPNE USTAWIENIE WEKTOROW: ODLEGLOSCI I SCIEZKI (ZGODNIE Z ZALOZENIAMI ALGORYTMU)
    for (int i = 0; i < graph->vertices; ++i) {
        distance[i] = MAX_DIST;
        path[i] = -1;
    }

    distance[start - 1] = 0;

    // WEKTOR ODWIEDZONYCH KOMOREK LABIRYNTU
    bool *visited = (bool *)malloc(graph->vertices * sizeof(bool));
    for (int i = 0; i < graph->vertices; ++i) {
        visited[i] = false;
    }

    // ZNAJDOWANIE NAJKROTSZEJ SCIEZKI
    for (int count = 0; count < graph->vertices - 1; ++count) {
        int u = -1;
        double minDistance = MAX_DIST;

        // ZNAJDOWANIE WEZLA O NAJMNIEJSZEJ ODLEGLOSCI MINIMALNEJ
        for (int v = 0; v < graph->vertices; ++v) {
            if (!visited[v] && distance[v] < minDistance) {
                u = v;
                minDistance = distance[v];
            }
        }
        visited[u] = true;

        struct Node *temp = graph->adjacency_list[u];
        while (temp) {
            int v = temp->data - 1;
            if (!visited[v] && distance[u] + temp->weight < distance[v]) {
                distance[v] = distance[u] + temp->weight;
                path[v] = u;
            }
            temp = temp->next;
        }
    }
    free(visited);
}

// WYSWIETLENIE NAJKROTSZEJ SCIEZKI PRZEJSCIA LABIRYNTU ORAZ WAGI CALKOWITEJ TEJ SCIEZKI
void shortestPath(int *path, double *distance, int start, int end, cell *cells, int cellCount) {
    printf("NAJKROTSZA SCIEZKA OD POCZATKU [%d] DO KONCA [%d] LABIRYNTU:\n\n", start, end);
    int current = end - 1;
    int reverse[cellCount];
    int i = 0;
    while (current != -1) {
        reverse[i] = current + 1;
        i++;
        current = path[current];
    }
    int j = i;
    i = 0;
    // ODWROCENIE KOLEJNOSCI WYSWIETLANIA KOLEJNYCH KOMOREK SCIEZKI
    while (i != j) {
        printf("--(%.2f)--> [%d] ", returnCellById(cells, reverse[j-1], cellCount).weight, reverse[j - 1]);
        j--;
    }
    printf("\n\n");
    
    // SUMA WAG WSZYSTKICH KOMOREK SCIEZKI (KRAWEDZI) + WAGA KOMORKI STARTOWEJ
    double totalWeight = distance[end - 1] + returnCellById(cells, start, cellCount).weight;
    printf("CALKOWITA WAGA SCIEZKI: %.2f\n\n", totalWeight);
}

// USTAW ZMIENNE START I END NA ID KOMOREK POCZATKU I KONCA LABIRYNTU
void getIdOfStartAndEnd(cell *cells, int *start, int *end, int cellCount){
    for (int i = 0; i < cellCount; i++) {
        if (cells[i].isStart) {
            *start = cells[i].id;
        }
        if (cells[i].isEnd) {
            *end = cells[i].id;
        }
    }
}

// SPRAWDZENIE ILE MOZLIWYCH SCIEZEK PRZEJSCIA LABIRYNTU ISTNIEJE
int countPaths(struct Graph* graph, int cellCount, int endID) {
    int count = 0;
    for (int i = 0; i < cellCount; i++) {
        struct Node* current = graph->adjacency_list[i];

        while (current != NULL) {
            if (current->data == endID) {
                count++;
            }
            current = current->next;
        }
    }
    return count;
}

int main() {
    int cellWidth = 0; // ROZMIAR LABIRYNTU (LICZBA KOMOREK W WIERSZU/SZEROKOSC LABIRYNTU)
    printf("PODAJ WYMIAR LABIRYNTU (2 - 10): ");
    scanf("%d", &cellWidth);
    printf("\n");

    while (cellWidth < 2 || cellWidth > 10) {
        printf("WYMIAR OD 2 DO 10! SPROBUJ JESZCZE RAZ: ");
        scanf("%d", &cellWidth);
    }

    int cellCount = cellWidth * cellWidth; // LICZBA KOMOREK LABIRYNTU
    int mazeSize = cellWidth * 2 + 1;      // ROZMIAR TABLICY ZNAKOW LABIRYNTU

    cell *cells = malloc(cellCount * sizeof(cell)); // WEKTOR KOMOREK
    struct Graph *graph = createGraph(cellCount);   // UTWORZ GRAF

    idCells(cells, cellCount);              // PONUMERUJ KOMORKI
    unvisitAllCells(cells, cellCount);      // USTAW KOMORKI NA "NIEODWIEDZONE"
    assignWeightsToCells(cells, cellCount); // PRZYPISZ LOSOWE WAGI KOMORKOM
    fillCells(cells, cellCount);

    chooseGates(cells, cellWidth, cellCount); // LOSOWO WYBIERZ WEJSCIE I WYJSCIE Z LABIRYNTU

    char maze[MAX_SIZE][MAX_SIZE];  // TABLICA ZNAKOW LABIRYNTU
    mazeGen(cells, mazeSize, maze); // GENEROWANIE "SUROWEGO" LABIRYNTU

    openGates(cells, maze, cellCount); // OTWORZ KOMORKI WEJSCIA I WYJSCIA

    // OTWIERANIE LOSOWO SCIEZEK W LABIRYNCIE + LISTA SASIEDZTWA
    randomPathOpenAndGraphGen(cells, maze, mazeSize, cellCount, cellWidth,graph);

    printf("LABIRYNT W FORMIE GRAFICZNEJ:\n\n");
    mazePrint(maze, mazeSize); // WYSWIETL LABIRYNT GRAFICZNIE

    int start, end;
    getIdOfStartAndEnd(cells, &start, &end, cellCount); // USTAW ID KOMORKI POCZATKU I KONCA LABIRYNTU

    printf("KOMORKA WEJSCIOWA - [%d]\n", start); // POCZATEK LABIRYNTU
    printf("Komorka WYJSCIOWA - [%d]\n\n", end); // KONIEC LABIRYNTU

    printGraph(graph, cells, cellCount); // WYSTWIETL GRAF W POSTACI LISTY SASIEDZTWA

    int *path = (int *)malloc(cellCount * sizeof(int)); // WEKTOR WEZLOW SCIEZKI
    double *distance = (double *)malloc(cellCount * sizeof(double)); // WEKTOR ODLEGLOSCI KOMOREK OD KOMORKI STARTOWEJ

    printf("LICZBA MOZLIWYCH SCIEZEK PRZEJSCIA LABIRYNTU: %d\n\n",countPaths(graph, cellCount, end)); 

    dijkstra(graph, start, end, path, distance); // ALGORYTM DIJKSTRY (ZNAJDOWANIE NAJKROTSZEJ SCIEZKI MIEDZY WEZLAMI GRAFU)
    shortestPath(path, distance, start, end, cells, cellCount); // WYSWIETLANIE NAJKROTSZEJ SCIEZKI I JEJ WAGI

    free(path); // ZWOLNIJ PAMIEC ZAALOKOWANA NA WEKTOR WEZLOW SCIEZKI
    free(distance); // ZWOLNIJ PAMIEC ZAALOKOWANA DLA WEKTORA ODLEGLOSCI KOMOREK OD KOMORKI STARTOWEJ
    freeGraph(graph); // ZWOLNIJ PAMIEC ZAALOKOWANA DLA GRAFU
    free(cells); // ZWOLNIJ PAMIEC ZAALOKOWANA DLA WEKTORA KOMOREK LABIRYNTU

    return 0;
}
