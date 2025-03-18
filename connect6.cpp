#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <random>
#include <ctime>
#include <memory>
#include <chrono>

using namespace std;
char turn = 'B';

class GameState {
    public:
        char board[19][19];
        char currentPlayer;
        bool isFirstMove;

        GameState() : isFirstMove(true), currentPlayer('B') {
            for (int i = 0; i < 19; ++i) {
                for (int j = 0; j < 19; ++j) {
                    board[i][j] = '-';
                }
            }
            currentPlayer = 'B'; 
        }

        void applyMove(int x1, int y1, int x2, int y2) {
            board[x1][y1] = currentPlayer;
            if (x2 != -1 && y2 != -1) {
                board[x2][y2] = currentPlayer;
            }
            if (isFirstMove) {
                isFirstMove = false;
            }
            currentPlayer = (currentPlayer == 'B') ? 'W' : 'B';
        }
    
        bool isValidMove(int x, int y) const {
            return x >= 0 && x < 19 && y >= 0 && y < 19 && board[x][y] == '-';
        }
    
        bool checkWin(char player) const {
            const int dx[] = {1, 0, 1, 1};
            const int dy[] = {0, 1, 1, -1};
    
            for (int x = 0; x < 19; ++x) {
                for (int y = 0; y < 19; ++y) {
                    if (board[x][y] != player) continue;
    
                    for (int d = 0; d < 4; ++d) {
                        int count = 1;
                        for (int step = 1; step < 6; ++step) {
                            int nx = x + step * dx[d];
                            int ny = y + step * dy[d];
                            if (nx < 0 || ny < 0 || nx >= 19 || ny >= 19 || board[nx][ny] != player) {
                                break;
                            }
                            count++;
                        }
                        if (count >= 6) return true;
                    }
                }
            }
            return false;
        }
    
        vector<pair<int, int>> getAvailableMoves() const {
            vector<pair<int, int>> moves;
            for (int i = 0; i < 19; ++i) {
                for (int j = 0; j < 19; ++j) {
                    if (board[i][j] == '-') {
                        moves.emplace_back(i, j);
                    }
                }
            }
            return moves;
        }
    
        GameState simulateMove(int x1, int y1, int x2, int y2) const {
            GameState nextState = *this;
            nextState.board[x1][y1] = currentPlayer;
            if (x2 != -1 && y2 != -1) {
                nextState.board[x2][y2] = currentPlayer;
            }
            nextState.currentPlayer = (currentPlayer == 'B') ? 'W' : 'B';
            return nextState;
        }

        void printBoard() const {
            cout << "    "; // Extra space for row numbers alignment
            for (int j = 0; j < 19; ++j) {
                if (j < 10) {
                    cout << " " << j << " "; // Add extra space for single-digit columns
                } else {
                    cout << j << " "; // No extra space for double-digit columns
                }
            }
            cout << endl;
        
            for (int i = 0; i < 19; ++i) {
                if (i < 10) {
                    cout << " " << i << "  "; // Extra spacing for single-digit rows
                } else {
                    cout << i << "  "; // Align double-digit rows
                }
        
                for (int j = 0; j < 19; ++j) {
                    cout << board[i][j] << "  "; // Consistent spacing between cells
                }
                cout << endl;
            }
        }
};

vector<pair<int, int>> detectThreats(GameState rootState, char opponentPlayer) {
    const int dx[] = {1, 0, 1, 1};  // Direcciones: horizontal, vertical, diagonal (\ y /)
    const int dy[] = {0, 1, 1, -1};

    vector<pair<int, int>> threatPositions; // Lista de amenazas detectadas

    // Iterar sobre cada celda del tablero
    for (int x = 0; x < 19; ++x) {
        for (int y = 0; y < 19; ++y) {
            if (rootState.board[x][y] != opponentPlayer) continue; // Revisar solo las fichas del oponente

            // Revisar las 4 direcciones posibles
            for (int d = 0; d < 4; ++d) {
                int count = 1;  // Incluir la ficha actual en el conteo
                pair<int, int> leftOpen = {-1, -1};  // Espacio vacío al extremo izquierdo
                pair<int, int> rightOpen = {-1, -1}; // Espacio vacío al extremo derecho

                // Explorar hacia adelante (extremo derecho de la línea)
                for (int step = 1; step <= 5; ++step) {
                    int nx = x + step * dx[d];
                    int ny = y + step * dy[d];

                    if (nx < 0 || ny < 0 || nx >= 19 || ny >= 19) break; // Fuera del tablero
                    if (rootState.board[nx][ny] == opponentPlayer) {
                        count++;
                    } else if (rootState.board[nx][ny] == '-') {
                        if (rightOpen.first == -1) rightOpen = {nx, ny};
                        break;  // Solo consideramos un espacio vacío por lado
                    } else {
                        break; // Línea bloqueada
                    }
                }

                // Explorar hacia atrás (extremo izquierdo de la línea)
                for (int step = -1; step >= -5; --step) {
                    int nx = x + step * dx[d];
                    int ny = y + step * dy[d];

                    if (nx < 0 || ny < 0 || nx >= 19 || ny >= 19) break; // Fuera del tablero
                    if (rootState.board[nx][ny] == opponentPlayer) {
                        count++;
                    } else if (rootState.board[nx][ny] == 'N') {
                        if (leftOpen.first == -1) leftOpen = {nx, ny};
                        break;  // Solo consideramos un espacio vacío por lado
                    } else {
                        break; // Línea bloqueada
                    }
                }

                // Verificar amenazas reales
                if (count == 4) {
                    // Si hay 4 fichas del oponente y al menos un extremo está abierto
                    if (leftOpen.first != -1) threatPositions.push_back(leftOpen);
                    if (rightOpen.first != -1) threatPositions.push_back(rightOpen);
                } else if (count == 5) {
                    // Si hay 5 fichas del oponente y al menos un extremo está abierto
                    if (leftOpen.first != -1) threatPositions.push_back(leftOpen);
                    if (rightOpen.first != -1) threatPositions.push_back(rightOpen);
                }
            }
        }
    }

    return threatPositions;
}

// Monte Carlo Node structure for MCTS
struct MCTSNode {
    GameState state;
    int visits = 0;
    double wins = 0.0;
    vector<pair<int, int>> availableMoves;
    vector<shared_ptr<MCTSNode>> children;
    MCTSNode* parent = nullptr;

    MCTSNode(const GameState& state, MCTSNode* parent = nullptr)
        : state(state), parent(parent) {
        availableMoves = state.getAvailableMoves();
    }

    bool isFullyExpanded() const {
        return children.size() == availableMoves.size();
    }

    shared_ptr<MCTSNode> expand() {
        // Expand using the next available move
        auto move = availableMoves[children.size()];
        GameState nextState = state.simulateMove(move.first, move.second, -1, -1); // Only one move example
        auto child = make_shared<MCTSNode>(nextState, this);
        children.push_back(child);
        return child;
    }

    shared_ptr<MCTSNode> bestChild(double explorationParam = 1.41) const {
        shared_ptr<MCTSNode> best = nullptr;
        double bestScore = -numeric_limits<double>::infinity();

        for (const auto& child : children) {
            double winRate = child->wins / (child->visits + 1e-6); // Win rate
            double ucb = winRate + explorationParam * sqrt(log(visits + 1) / (child->visits + 1e-6));
            if (ucb > bestScore) {
                bestScore = ucb;
                best = child;
            }
        }
        return best;
    }

    void backpropagate(double result) {
        visits++;
        wins += result;
        if (parent) parent->backpropagate(result);
    }
};

// Simulates a random playout and returns the result
double simulatePlayout(GameState state) {
    char player = state.currentPlayer;
    while (true) {
        auto moves = state.getAvailableMoves();
        if (moves.empty()) break;

        // Random move
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, moves.size() - 1);
        auto move = moves[dis(gen)];

        state.applyMove(move.first, move.second, -1, -1); // Only one move in simulation

        if (state.checkWin(player)) return 1.0; // Current player wins
        player = (player == 'B') ? 'W' : 'B';
    }
    return 0.5; // Draw
}

// Main MCTS function
pair<pair<int, int>, pair<int, int>> mcts(GameState& rootState, int timeLimit) {
    auto root = make_shared<MCTSNode>(rootState);
    auto startTime = chrono::high_resolution_clock::now();
    char opponent = (rootState.currentPlayer == 'B') ? 'W' : 'B'; // Determine the opponent's color
    vector<pair<int, int>> threatPositions = detectThreats(rootState, opponent);
    if (threatPositions.size() > 0){
        cout << threatPositions[0].first << " " << threatPositions[0].second << endl;
    }

    // Handle threats if detected
    if (!threatPositions.empty()) {
        if (threatPositions.size() >= 2) {
            // If two or more threats are found, use the first two
            return {threatPositions[0], threatPositions[1]};
        } else if (threatPositions.size() == 1) {
            // If only one threat is found, pair it with another valid move
            vector<pair<int, int>> availableMoves = rootState.getAvailableMoves();
            for (const auto move : availableMoves) {
                cout << availableMoves.size() << endl;
                if (move.first != threatPositions[0].first && move.second != threatPositions[0].second) { // Ensure the second move is not the same
                    cout << move.first << " " << move.second << endl;
                    return {threatPositions[0], move};
                }
            }
            // Fallback: If no other move is available, return the single move (edge case)
            return {threatPositions[0], {-1, -1}};
        }
    }

    // No threats detected; proceed with regular MCTS

    while (true) {
        auto currentTime = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = currentTime - startTime;

        if (elapsed.count() >= timeLimit) break; // Stop after time limit

        // Selection
        auto node = root.get();
        while (!node->children.empty() && node->isFullyExpanded()) {
            node = node->bestChild().get();
        }

        // Expansion
        if (!node->isFullyExpanded()) {
            node = node->expand().get();
        }

        // Simulation
        double result = simulatePlayout(node->state);

        // Backpropagation
        node->backpropagate(result);
    }

    // Return the best two moves from the root
    auto bestChild = root->bestChild(0.0); // Use exploitation only
    auto availableMoves = bestChild->state.getAvailableMoves();

    // Handle the first move (single move only)
    if (rootState.isFirstMove) {
        return {{availableMoves[0].first, availableMoves[0].second}, {-1, -1}};
    }

    // Handle two moves for subsequent turns
    return {{availableMoves[0].first, availableMoves[0].second},
            {availableMoves[1].first, availableMoves[1].second}};
}


void playGame(GameState& state, char userColor, char aiColor, int timeLimit) {
    bool isUserTurn = (userColor == 'B'); // User goes first if they are black

    while (true) {
        state.printBoard(); // Display the board

        if (isUserTurn) {
            // User's Turn
            cout << "Tu turno (" << (userColor == 'B' ? "Negras" : "Blancas") << ").\n";
            
            if (state.isFirstMove) { // Handle first move logic
                cout << "Es tu primer turno, ingresa un solo movimiento (x1 y1): ";
                int x1, y1;
                cin >> x1 >> y1;

                if (state.isValidMove(x1, y1)) {
                    state.applyMove(x1, y1, -1, -1); // Single move
                } else {
                    cout << "Movimiento inválido. Intenta de nuevo.\n";
                    continue; // Retry the user's turn
                }
            } else { // Handle all other turns
                cout << "Ingresa tus dos movimientos (x1 y1 x2 y2): ";
                int x1, y1, x2, y2;
                cin >> x1 >> y1 >> x2 >> y2;

                if (state.isValidMove(x1, y1) && state.isValidMove(x2, y2)) {
                    state.applyMove(x1, y1, x2, y2);
                } else {
                    cout << "Movimientos inválidos. Intenta de nuevo.\n";
                    continue; // Retry the user's turn
                }
            }

            // Check for a win after the user's move
            if (state.checkWin(userColor)) {
                state.printBoard();
                cout << "¡Felicidades! Has ganado el juego.\n";
                break;
            }
        } else {
            // AI's Turn
            cout << "Turno del agente. Pensando...\n";
            
            auto move = mcts(state, timeLimit); // Use MCTS for the AI's move
            state.applyMove(move.first.first, move.first.second, move.second.first, move.second.second);

            // Print the AI's moves
            cout << "AI movió: (" << move.first.first << ", " << move.first.second << ")";
            if (move.second.first != -1 && move.second.second != -1) {
                cout << " y (" << move.second.first << ", " << move.second.second << ")";
            }
            cout << endl;

            // Check for a win after the AI's move
            if (state.checkWin(aiColor)) {
                state.printBoard();
                cout << "El agente ha ganado el juego.\n";
                break;
            }
        }

        isUserTurn = !isUserTurn; // Toggle between user and AI turns
    }
}

int main(int argc, char* argv[]) {
    string fichas;
    int timeLimit = 0;

    // Leer los argumentos desde la línea de comandos
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg.find("--fichas=") == 0) {
            fichas = arg.substr(9);
        } else if (arg.find("--tiempo=") == 0) {
            timeLimit = stoi(arg.substr(9));
        }
    }

    // Validar los argumentos
    if (fichas != "negras" && fichas != "blancas") {
        cerr << "Error: las fichas deben ser 'negras' o 'blancas'.\n";
        return 1;
    }
    if (timeLimit <= 0) {
        cerr << "Error: el tiempo debe ser mayor a 0 segundos.\n";
        return 1;
    }

    char userColor = (fichas == "negras") ? 'B' : 'W';
    char aiColor = (userColor == 'W') ? 'B' : 'W';

    GameState state;
    playGame(state, userColor, aiColor, timeLimit);
    

    return 0;
}