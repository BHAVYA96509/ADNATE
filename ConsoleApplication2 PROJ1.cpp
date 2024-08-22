


#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <limits>
#include <set>

using namespace std;

#define num 60

// Dijkstra's Pathfinding
vector<pair<int, int>> pathD;
bool sptSet[num][num];

void findmin(float dist[num][num], int& min_x, int& min_y) {
    float mini = numeric_limits<float>::max();
    for (int i = 0; i < num; i++)
        for (int j = 0; j < num; j++)
            if (!sptSet[i][j] && dist[i][j] < mini) {
                mini = dist[i][j];
                min_x = i;
                min_y = j;
            }
}

void findpath(pair<int, int> previous[num][num], float dist[num][num], int dest_x, int dest_y, int source_x, int source_y) {
    cout << "\nLength of Dijkstra path is: " << dist[dest_x][dest_y] << endl;
    while (previous[dest_x][dest_y].first != source_x || previous[dest_x][dest_y].second != source_y) {
        pathD.push_back(make_pair(previous[dest_x][dest_y].first, previous[dest_x][dest_y].second));
        int save_x = dest_x, save_y = dest_y;
        dest_x = previous[save_x][save_y].first;
        dest_y = previous[save_x][save_y].second;
    }
}

void dijkstra(int source_x, int source_y, int dest_x, int dest_y, int grid[num][num]) {
    pair<int, int> previous[num][num];
    float dist[num][num];
    for (int i = 0; i < num; i++)
        for (int j = 0; j < num; j++)
            dist[i][j] = numeric_limits<float>::max();
    dist[source_x][source_y] = 0.0;
    int found = 0;
    for (int i = 0; i < num && found == 0; i++) {
        for (int j = 0; j < num && found == 0; j++) {
            
            int min_x = 0, min_y = 0;
            findmin(dist, min_x, min_y);
            sptSet[min_x][min_y] = true;
            if (sptSet[dest_x][dest_y] == true) {
                found = 1;
                break;
            }
            int possibleX[] = { 0, 0, 1, -1, 1, -1, -1, 1 };
            int possibleY[] = { 1, -1, 0, 0, 1, 1, -1, -1 };

            for (int i = 0; i < 8; ++i) {
                int newRow = min_x + possibleX[i];
                int newCol = min_y + possibleY[i];
                if (newRow >= 0 && newRow < num && newCol >= 0 && newCol < num && grid[newRow][newCol] == 1 && !sptSet[newRow][newCol] && dist[newRow][newCol] > dist[min_x][min_y] + 1.0) {
                    dist[newRow][newCol] = dist[min_x][min_y] + 1.0;
                    previous[newRow][newCol] = make_pair(min_x, min_y);
                }
            }
        }
    }

    findpath(previous, dist, dest_x, dest_y, source_x, source_y);
}

// A* Pathfinding
typedef pair<int, int> Pair;
typedef pair<float, pair<int, int>> Ppair;
bool closedList[num][num];
vector<Pair> pathA;

struct cell {
    int parent_x, parent_y;
    float f, g, h;
    cell() : f(numeric_limits<float>::max()), g(numeric_limits<float>::max()), h(numeric_limits<float>::max()), parent_x(-1), parent_y(-1) {};
};

bool isDestination(int row, int col, Pair dest) {
    return row == dest.first && col == dest.second;
}

float calculateHvalue(int row, int col, Pair dest) {
    int dx = abs(dest.first - row);
    int dy = abs(dest.second - col);
    return abs(dx - dy) + sqrt(2) * min(dx, dy);
}

void tracePath(Pair source, Pair dest, cell cellDetails[][num]) {
    int i = cellDetails[dest.first][dest.second].parent_x, j = cellDetails[dest.first][dest.second].parent_y;
    while (!(i == source.first && j == source.second)) {
        pathA.push_back(make_pair(i, j));
        int temp_i = i;
        int temp_j = j;
        i = cellDetails[temp_i][temp_j].parent_x;
        j = cellDetails[temp_i][temp_j].parent_y;
    }
    cout << "\nLength of A* path(g) is: " << cellDetails[dest.first][dest.second].g << endl;
}

void Astar(Pair source, Pair dest, int grid[][num]) {
    set<Ppair> openList;
    cell cellDetails[num][num];
    int i = source.first, j = source.second;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_x = i;
    cellDetails[i][j].parent_y = j;
    openList.insert(make_pair(0.0, make_pair(i, j)));
    bool destFound = false;
    int possibleX[] = { 0, 0, 1, -1, 1, -1, -1, 1 };
    int possibleY[] = { 1, -1, 0, 0, 1, 1, -1, -1 };
    while (!openList.empty()) {
        Ppair p = *openList.begin();
        openList.erase(openList.begin());
        int i = p.second.first, j = p.second.second;
        closedList[i][j] = true;
        if (isDestination(i, j, dest)) {
            destFound = true;
            break;
        }
        for (int k = 0; k < 8; ++k) {
            int newRow = i + possibleX[k];
            int newCol = j + possibleY[k];
            if (newRow >= 0 && newRow < num && newCol >= 0 && newCol < num && grid[newRow][newCol] == 1 && !closedList[newRow][newCol]) {
                cell successor;
                successor.g = cellDetails[i][j].g + 1.0;
                successor.h = calculateHvalue(newRow, newCol, dest);
                successor.f = successor.g + successor.h;
                successor.parent_x = i;
                successor.parent_y = j;
                if (cellDetails[newRow][newCol].g > successor.g) {
                    cellDetails[newRow][newCol] = successor;
                    openList.insert(make_pair(successor.f, make_pair(newRow, newCol)));
                }
            }
        }
    }
    if (!destFound)
        cout << "Destination cell not found.\n";
    else
        tracePath(source, dest, cellDetails);
}

// Main function
int main() {
    int filled[num][num];
    int grid[num][num];
    for (int i = 0; i < num; i++)
        for (int j = 0; j < num; j++) {
            if (i == 0 || i == num - 1 || j == 0 || j == num - 1)
                grid[i][j] = 0;
            else
                grid[i][j] = 1;
        }
    for (int i = 0; i < num; i++)
        for (int j = 0; j < num; j++) {
            sptSet[i][j] = false;
            filled[i][j] = 0;
        }
    memset(closedList, false, sizeof(closedList));

    int source_x = 2, source_y = 2, dest_x = 50, dest_y = 56;

    // Run Dijkstra's and A* algorithms
    dijkstra(source_x, source_y, dest_x, dest_y, grid);
    Astar(make_pair(source_x, source_y), make_pair(dest_x, dest_y), grid);

    // Display results in console
    cout << "Grid with paths:\n";
    for (int i = 0; i < num; i++) {
        for (int j = 0; j < num; j++) {
            if (find(pathD.begin(), pathD.end(), make_pair(i, j)) != pathD.end())
                cout << "D "; // Path found by Dijkstra's
            else if (find(pathA.begin(), pathA.end(), make_pair(i, j)) != pathA.end())
                cout << "A "; // Path found by A*
            else if (grid[i][j] == 0)
                cout << "# "; // Obstacle
            else
                cout << ". "; // Empty space
        }
        cout << endl;
    }

    return 0;
}
