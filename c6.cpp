#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <random>
#include <ctime>

using namespace std;

// Tamaño del tablero Connect6
const int BOARD_SIZE = 19;

// Estructura para representar un movimiento
struct Move {
    int x1, y1; // Primera ficha
    int x2, y2; // Segunda ficha
};

// Estructura para representar el estado del juego
class GameState {
public:
    char board[BOARD_SIZE][BOARD_SIZE]; // Tablero de juego

    GameState() {
        for (int i = 0; i < BOARD_SIZE; ++i)
            for (int j = 0; j < BOARD_SIZE; ++j)
                board[i][j] = '.'; // Casilla vacía
    }

    // Aplica un movimiento
    void applyMove(const Move &move, char player) {
        board[move.x1][move.y1] = player;
        board[move.x2][move.y2] = player;
    }
/*
    // Genera todos los movimientos válidos
    vector<Move> getValidMoves() const {
        vector<Move> moves;

        // Verificar si es el primer turno (tablero completamente vacío)
        bool isFirstTurn = true;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] != '.') {
                    isFirstTurn = false;
                    break;
                }
            }
            if (!isFirstTurn) break;
        }

        // Generar movimientos válidos
        if (isFirstTurn) {
            // Primer turno: un solo movimiento (una ficha)
            for (int x = 0; x < BOARD_SIZE; ++x) {
                for (int y = 0; y < BOARD_SIZE; ++y) {
                    if (board[x][y] == '.') {
                        moves.push_back({x, y, -1, -1}); // Segundo movimiento (-1, -1) porque no aplica
                    }
                }
            }
        } else {
            // Turnos normales: dos fichas
            for (int x1 = 0; x1 < BOARD_SIZE; ++x1) {
                for (int y1 = 0; y1 < BOARD_SIZE; ++y1) {
                    if (board[x1][y1] == '.') {
                        for (int x2 = 0; x2 < BOARD_SIZE; ++x2) {
                            for (int y2 = 0; y2 < BOARD_SIZE; ++y2) {
                                if (board[x2][y2] == '.' && !(x1 == x2 && y1 == y2)) {
                                    moves.push_back({x1, y1, x2, y2});
                                }
                            }
                        }
                    }
                }
            }
        }

        return moves;
    }
*/

    vector<Move> getValidMoves() const {
        vector<Move> moves;
        int centerX = BOARD_SIZE / 2;
        int centerY = BOARD_SIZE / 2;
    
        for (int x1 = 0; x1 < BOARD_SIZE; ++x1) {
            for (int y1 = 0; y1 < BOARD_SIZE; ++y1) {
                if (board[x1][y1] == '.') {
                    // Calcular distancia al centro
                    int dist1 = abs(centerX - x1) + abs(centerY - y1);
                
                    for (int x2 = 0; x2 < BOARD_SIZE; ++x2) {
                        for (int y2 = 0; y2 < BOARD_SIZE; ++y2) {
                            if (board[x2][y2] == '.' && !(x1 == x2 && y1 == y2)) {
                                int dist2 = abs(centerX - x2) + abs(centerY - y2);
                            
                                // Empujar el movimiento con prioridad al centro
                                moves.push_back({x1, y1, x2, y2});
                            }
                        }
                    }
                }
            }
        }
    
        // Ordenar movimientos por distancia al centro (prioridad estratégica)
        sort(moves.begin(), moves.end(), [centerX, centerY](const Move &a, const Move &b) {
            int distA = abs(centerX - a.x1) + abs(centerY - a.y1) + abs(centerX - a.x2) + abs(centerY - a.y2);
            int distB = abs(centerX - b.x1) + abs(centerY - b.y1) + abs(centerX - b.x2) + abs(centerY - b.y2);
            return distA < distB;
        });
    
        return moves;
    }

    void printBoard() const {
        for (int i = 0; i < 19; ++i) {
            for (int j = 0; j < 19; ++j) {
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
    }

    // Verifica si el juego ha terminado
    bool isTerminal(char player) const {
        const int dx[] = {1, 0, 1, 1};
        const int dy[] = {0, 1, 1, -1};

        for (int x = 0; x < 19; ++x) {
            for (int y = 0; y < 19; ++y) {
                // Continuar si la celda actual no pertenece al jugador
                if (board[x][y] != player) continue;

                // Verificar cada dirección
                for (int d = 0; d < 4; ++d) {
                    int count = 1; // Contar la celda actual

                    // Avanzar en la dirección actual
                    for (int step = 1; step < 6; ++step) {
                        int nx = x + step * dx[d];
                        int ny = y + step * dy[d];

                        // Verificar si las coordenadas están fuera de rango
                        if (nx < 0 || ny < 0 || nx >= 19 || ny >= 19) break;

                        // Verificar si la celda pertenece al mismo jugador
                        if (board[nx][ny] == player) {
                            count++;
                        } else {
                            break;
                        }
                    }

                    // Si se encuentran 6 o más consecutivos, el jugador gana
                    if (count >= 6) {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    int scoreLine(int x, int y, char player) const {
        int lineScore = 0;

        // Direcciones: horizontal, vertical, diagonales
        const int dx[] = {1, 0, 1, 1};
        const int dy[] = {0, 1, 1, -1};

        for (int d = 0; d < 4; ++d) {
            int count = 1;
            bool openStart = true, openEnd = true;

            // Busca en la dirección positiva
            for (int step = 1; step < 6; ++step) {
                int nx = x + step * dx[d];
                int ny = y + step * dy[d];
                if (nx >= BOARD_SIZE || ny >= BOARD_SIZE || nx < 0 || ny < 0 || board[nx][ny] != player) {
                    if (nx >= 0 && ny >= 0 && nx < BOARD_SIZE && ny < BOARD_SIZE && board[nx][ny] != '.') {
                        openEnd = false;
                    }
                    break;
                }
                count++;
            }

            // Busca en la dirección negativa
            for (int step = 1; step < 6; ++step) {
                int nx = x - step * dx[d];
                int ny = y - step * dy[d];
                if (nx >= BOARD_SIZE || ny >= BOARD_SIZE || nx < 0 || ny < 0 || board[nx][ny] != player) {
                    if (nx >= 0 && ny >= 0 && nx < BOARD_SIZE && ny < BOARD_SIZE && board[nx][ny] != '.') {
                        openStart = false;
                    }
                    break;
                }
                count++;
            }

            // Calcula puntaje basado en la longitud de la línea
            if (count >= 6) {
                lineScore += 10000; // Jugador gana
            } else if (count == 5 && openStart && openEnd) {
                lineScore += 500; // Línea de 5 abierta
            } else if (count == 4 && openStart && openEnd) {
                lineScore += 100; // Línea de 4 abierta
            } else if (count == 3 && openStart && openEnd) {
                lineScore += 50; // Línea de 3 abierta
            }
        }
        return lineScore;
    }

    // Función de evaluación para estados terminales
    int evaluate() const {
        int score = 0;
        char player = 'B';  // Supongamos que evaluamos para el jugador negro
        char opponent = 'W';

        // Recorremos el tablero
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] == player) {
                    // Evalúa líneas para el jugador
                    score += scoreLine(i, j, player);
                } else if (board[i][j] == opponent) {
                    // Penaliza líneas del oponente
                    score -= scoreLine(i, j, opponent);
                }
            }
        }
        return score;
    }
};

// Nodo del árbol de búsqueda MCTS
class Node {
public:
    GameState state;
    Move move;
    Node* parent;
    vector<Node*> children;
    int visits;
    double wins;

    Node(const GameState &state, const Move &move, Node *parent = nullptr)
        : state(state), move(move), parent(parent), visits(0), wins(0) {}

    // Liberar memoria de nodos hijos
    ~Node() {
        for (Node* child : children) {
            delete child;
        }
    }

    // Expande los nodos hijos
    void expand(char player) {
        for (const Move &m : state.getValidMoves()) {
            GameState childState = state;
            childState.applyMove(m, player);
            children.push_back(new Node(childState, m, this));
        }
    }

    // Selecciona el mejor hijo basado en UCT
    Node* selectChild() {
        double bestUCT = -numeric_limits<double>::infinity();
        Node* bestChild = nullptr;

        for (Node* child : children) {
            double uctValue = (child->wins / (child->visits + 1e-6)) +
                              sqrt(2 * log(visits + 1) / (child->visits + 1e-6));
            if (uctValue > bestUCT) {
                bestUCT = uctValue;
                bestChild = child;
            }
        }
        return bestChild;
    }

    // Actualiza las estadísticas del nodo
    void update(int result) {
        visits++;
        wins += result;
    }
};

// Clase para el agente con algoritmo MCTS
class MCTSAgent {
public:
    char player;

    MCTSAgent(char player) : player(player) {}

    Move getBestMove(GameState &state, int maxTimeInSeconds) {
        Node* root = new Node(state, Move{-1, -1, -1, -1});
        char opponent = (player == 'B' ? 'W' : 'B');
    
        // Obtén el tiempo inicial
        clock_t startTime = clock();
    
        while (true) {
            // Calcula el tiempo transcurrido en segundos
            double elapsedTime = static_cast<double>(clock() - startTime) / CLOCKS_PER_SEC;
            if (elapsedTime >= maxTimeInSeconds) {
                break; // Salir del loop si se alcanzó el tiempo límite
            }
    
            Node* node = root;
    
            // Fase 1: Selección
            while (!node->children.empty() && !node->state.isTerminal(player)) {
                node = node->selectChild();
            }
    
            // Fase 2: Expansión
            if (!node->state.isTerminal(player)) {
                node->expand(player);
                if (!node->children.empty()) {
                    node = node->children[0];
                }
            }
    
            // Fase 3: Simulación
            GameState simulationState = node->state;
            char currentPlayer = player;
            while (!simulationState.isTerminal(currentPlayer)) {
                vector<Move> validMoves = simulationState.getValidMoves();
                if (validMoves.empty()) break;
    
                Move randomMove = validMoves[rand() % validMoves.size()];
                simulationState.applyMove(randomMove, currentPlayer);
                currentPlayer = (currentPlayer == 'B' ? 'W' : 'B');
            }
    
            // Fase 4: Retropropagación
            int result = simulationState.evaluate(); // 1: victoria, 0: empate, -1: derrota
            while (node != nullptr) {
                node->update(result);
                result = -result; // Cambia la perspectiva del resultado
                node = node->parent;
            }
        }
    
        // Seleccionar el mejor movimiento
        Node* bestChild = nullptr;
        int mostVisits = -1;
        for (Node* child : root->children) {
            if (child->visits > mostVisits) {
                mostVisits = child->visits;
                bestChild = child;
            }
        }
    
        Move bestMove = bestChild->move;
        delete root;
        return bestMove;
    }
};

int main(int argc, char* argv[]) {
    string fichas;
    int tiempo = 0;

    // Leer los argumentos desde la línea de comandos
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg.find("--fichas=") == 0) {
            fichas = arg.substr(9);
        } else if (arg.find("--tiempo=") == 0) {
            tiempo = stoi(arg.substr(9));
        }
    }

    // Validar los argumentos
    if (fichas != "negras" && fichas != "blancas") {
        cerr << "Error: las fichas deben ser 'negras' o 'blancas'.\n";
        return 1;
    }
    if (tiempo <= 0) {
        cerr << "Error: el tiempo debe ser mayor a 0 segundos.\n";
        return 1;
    }

    char player = (fichas == "negras") ? 'B' : 'W';
    GameState game;
    MCTSAgent agent(player);

    // Ciclo principal de juego
    while (!game.isTerminal()) {
        game.printBoard();

        if (player == 'B') { // Turno del agente
            cout << "El agente (negras) está calculando su movimiento...\n";
            auto move = agent.getBestMove(game, tiempo);
            game.applyMove(move.first, move.second, -1, -1, 'B'); // Simula un movimiento de 1 ficha
        } else { // Turno del jugador humano
            cout << "Tu turno (blancas). Ingresa las coordenadas de tu ficha (x y): ";
            int x, y;
            cin >> x >> y;
            game.applyMove(x, y, -1, -1, 'W'); // Movimiento del jugador
        }

        // Alternar entre el agente y el jugador humano
        player = (player == 'B') ? 'W' : 'B';
    }

    // Mostrar el resultado final
    game.printBoard();
    cout << "El juego ha terminado.\n";

    return 0;
}