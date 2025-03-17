#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <random>
#include <ctime>

using namespace std;

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

    cout << player << endl;
    // GameState game;
    // MCTSAgent agent(player);

    // // Ciclo principal de juego
    // while (!game.isTerminal()) {
    //     game.printBoard();

    //     if (player == 'B') { // Turno del agente
    //         cout << "El agente (negras) está calculando su movimiento...\n";
    //         auto move = agent.getBestMove(game, tiempo);
    //         game.applyMove(move.first, move.second, -1, -1, 'B'); // Simula un movimiento de 1 ficha
    //     } else { // Turno del jugador humano
    //         cout << "Tu turno (blancas). Ingresa las coordenadas de tu ficha (x y): ";
    //         int x, y;
    //         cin >> x >> y;
    //         game.applyMove(x, y, -1, -1, 'W'); // Movimiento del jugador
    //     }

    //     // Alternar entre el agente y el jugador humano
    //     player = (player == 'B') ? 'W' : 'B';
    // }

    // // Mostrar el resultado final
    // game.printBoard();
    // cout << "El juego ha terminado.\n";

    return 0;
}