#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

bool readInitialState(const string& filename, vector<vector<char>>& board) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл " << filename << endl;
        return false;
    }

    int rows, cols;
    file >> rows >> cols;

    board.assign(rows, vector<char>(cols, '-'));

    int row, col;
    while (file >> row >> col) {
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            board[row][col] = '*'; 
        }
        else {
            cerr << "Предупреждение: координаты (" << row << "," << col
                << ") выходят за границы поля " << rows << "x" << cols << endl;
        }
    }

    file.close();
    return true;
}

void printBoard(const vector<vector<char>>& board, int generation, int liveCells) {
    cout << "Поколение: " << generation << endl;
    cout << "Живых клеток: " << liveCells << endl;

    for (const auto& row : board) {
        for (size_t j = 0; j < row.size(); ++j) {
            cout << row[j];
            if (j < row.size() - 1) cout << " "; 
        }
        cout << endl;
    }
    cout << endl; 
}


int countNeighbors(const vector<vector<char>>& board, int x, int y) {
    int count = 0;
    int rows = board.size();
    int cols = board[0].size();

 
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue; 

            int nx = x + dx;
            int ny = y + dy;

           
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
                if (board[nx][ny] == '*') {
                    count++;
                }
            }
        }
    }
    return count;
}


bool nextGeneration(vector<vector<char>>& board, int& liveCellsCount) {
    int rows = board.size();
    int cols = board[0].size();
    vector<vector<char>> newBoard = board; 
    int newLiveCells = 0;

    bool changed = false; 

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int neighbors = countNeighbors(board, i, j);

            if (board[i][j] == '*') { 
                
                if (neighbors < 2 || neighbors > 3) {
                    newBoard[i][j] = '-';
                    changed = true;
                }
                else {
                    newLiveCells++;
                }
            }
            else { 
                if (neighbors == 3) {
                    newBoard[i][j] = '*';
                    newLiveCells++;
                    changed = true;
                }
                else {
                    
                }
            }

            
            if (newBoard[i][j] == '*') {
                newLiveCells++;
            }
        }
    }

    board = newBoard;
    liveCellsCount = newLiveCells;
    return changed; 
}

int countLiveCells(const vector<vector<char>>& board) {
    int count = 0;
    for (const auto& row : board) {
        for (char cell : row) {
            if (cell == '*') count++;
        }
    }
    return count;
}

int main(int argc, char* argv[]) {
    string filename;

   
    if (argc > 1) {
        filename = argv[1];
    }
    else {
        cout << "Введите имя файла с начальным состоянием: ";
        cin >> filename;
    }

    vector<vector<char>> board;

    
    if (!readInitialState(filename, board)) {
        return 1;
    }

    int generation = 0;
    int liveCells = countLiveCells(board);

    
    printBoard(board, generation, liveCells);

    const int MAX_GENERATIONS = 1000; 
    bool stable = false;

  
    for (generation = 1; generation <= MAX_GENERATIONS; ++generation) {
        bool changed = nextGeneration(board, liveCells);

        
        printBoard(board, generation, liveCells);

        
        if (!changed) {
            stable = true;
            break;
        }

       
        if (liveCells == 0) {
            break;
        }
    }

    
    cout << "=== ИГРА ЗАВЕРШЕНА ===" << endl;
    cout << "Последнее поколение: " << generation << endl;
    cout << "Живых клеток: " << liveCells << endl;

    if (liveCells == 0) {
        cout << "Причина: все клетки умерли" << endl;
    }
    else if (stable) {
        cout << "Причина: достигнуто стабильное состояние" << endl;
    }
    else {
        cout << "Причина: достигнуто максимальное количество поколений ("
            << MAX_GENERATIONS << ")" << endl;
    }

    return 0;
}