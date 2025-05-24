
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// === คลาส Ship ===
class Ship {
public:
    string name;
    int size;
    int hitCount = 0;
    vector<pair<int, int>> positions;

    Ship(string n, int s) : name(n), size(s) {}

    void placeShip(vector<pair<int, int>> pos) {
        positions = pos;
    }

    bool isSunk() {
        return hitCount >= size;
    }

    bool isHit(int row, int col) {
        for (auto &p : positions) {
            if (p.first == row && p.second == col) {
                hitCount++;
                return true;
            }
        }
        return false;
    }
};

// === คลาส Board ===
class Board {
private:
    int rows = 10;
    int cols = 10;
    vector<vector<char>> ocean;
    vector<Ship> ships;

public:
    Board() {
        ocean.resize(rows, vector<char>(cols, '~'));
    }

    bool canPlaceShip(int r, int c, int size, bool horizontal) {
        if (horizontal) {
            if (c + size > cols) return false;
            for (int i = 0; i < size; ++i) {
                if (ocean[r][c + i] != '~') return false;
            }
        } else {
            if (r + size > rows) return false;
            for (int i = 0; i < size; ++i) {
                if (ocean[r + i][c] != '~') return false;
            }
        }
        return true;
    }

    void placeShip(Ship ship, int r, int c, bool horizontal) {
        vector<pair<int, int>> pos;
        if (horizontal) {
            for (int i = 0; i < ship.size; ++i) {
                ocean[r][c + i] = 'S';
                pos.push_back({r, c + i});
            }
        } else {
            for (int i = 0; i < ship.size; ++i) {
                ocean[r + i][c] = 'S';
                pos.push_back({r + i, c});
            }
        }
        ship.placeShip(pos);
        ships.push_back(ship);
    }

    bool attack(int r, int c) {
        for (auto &ship : ships) {
            if (ship.isHit(r, c)) {
                ocean[r][c] = 'X';
                cout << "โดนเรือ: " << ship.name << endl;
                if (ship.isSunk()) {
                    cout << "เรือ " << ship.name << " จมแล้ว!" << endl;
                }
                return true;
            }
        }
        ocean[r][c] = 'O';
        cout << "พลาด!" << endl;
        return false;
    }

    void display() {
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                cout << ocean[r][c] << ' ';
            }
            cout << endl;
        }
    }

    bool allShipsSunk() {
        for (auto &s : ships) {
            if (!s.isSunk()) return false;
        }
        return true;
    }
};

// === คลาส Player ===
class Player {
public:
    string name;
    Board board;

    Player(string n) : name(n) {}

    void placeShips() {
        cout << "วางเรือของ " << name << endl;
        board.placeShip(Ship("Destroyer", 2), 0, 0, true);
        board.placeShip(Ship("Submarine", 3), 2, 2, false);
    }

    bool attackOpponent(Player &opponent) {
        int r, c;
        cout << name << " เลือกตำแหน่งยิง (row col): ";
        cin >> r >> c;
        return opponent.board.attack(r, c);
    }
};

// === คลาส Game ===
class Game {
private:
    Player player1;
    Player player2;

public:
    Game() : player1("ผู้เล่นที่ 1"), player2("ผู้เล่นที่ 2") {}

    void setup() {
        player1.placeShips();
        player2.placeShips();
    }

    void start() {
        bool gameOver = false;
        while (!gameOver) {
            cout << "\n===== เทิร์นของผู้เล่น 1 =====" << endl;
            player1.attackOpponent(player2);
            player2.board.display();

            if (player2.board.allShipsSunk()) {
                cout << "ผู้เล่น 1 ชนะ!" << endl;
                break;
            }

            cout << "\n===== เทิร์นของผู้เล่น 2 =====" << endl;
            player2.attackOpponent(player1);
            player1.board.display();

            if (player1.board.allShipsSunk()) {
                cout << "ผู้เล่น 2 ชนะ!" << endl;
                break;
            }
        }
    }
};

int main() {
    Game game;
    game.setup();
    game.start();
    return 0;
}
