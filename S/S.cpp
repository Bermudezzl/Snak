#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

bool finJuego; // Variable que indica si el juego ha terminado
const int ancho = 20; // Ancho del tablero del juego
const int alto = 20; // Alto del tablero del juego
int x, y, frutaX, frutaY, puntaje; // Coordenadas de la serpiente, fruta y el puntaje
int colaX[100], colaY[100]; // Coordenadas de la cola de la serpiente
int longitudCola; // Longitud actual de la cola de la serpiente
enum Direccion { PARAR = 0, IZQUIERDA, DERECHA, ARRIBA, ABAJO }; // Direcciones de movimiento
Direccion dir; // Dirección actual de la serpiente

void Configuracion()
{
    finJuego = false; // Inicialmente el juego no ha terminado
    dir = PARAR; // La serpiente está sin moverse al inicio
    x = ancho / 2; // Posición inicial en el centro del ancho del tablero
    y = alto / 2; // Posición inicial en el centro del alto del tablero
    frutaX = rand() % ancho; // Posición aleatoria para la fruta en el ancho del tablero
    frutaY = rand() % alto; // Posición aleatoria para la fruta en el alto del tablero
    puntaje = 0; // El puntaje comienza en cero
}
//
void EstablecerColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); // Configura el color de texto de la consola
}

void LimpiarPantalla()
{
    COORD posicionCursor;
    posicionCursor.X = 0;
    posicionCursor.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posicionCursor); // Mueve el cursor a la posición (0,0) de la consola
}

void Dibujar()
{
    LimpiarPantalla(); // Limpia la pantalla antes de dibujar el tablero

    // Se encarga de recorrer las filas del tablero. La variable i se inicializa en 0 y se incrementa en 1 en cada iteración hasta que llega al valor de alto, que es la altura del tablero
    for (int i = 0; i < alto; i++)
    {
        // Lo mismo de arriba pero con el ancho del tablero
        for (int j = 0; j < ancho; j++)
        {
            // Esta línea comprueba si la posición actual(i, j) está en el borde del tablero. Si se cumple, se dibuja un * en esa posición
            if (i == 0 || i == alto - 1 || j == 0 || j == ancho - 1)
            {
                EstablecerColor(15); // Establece el color blanco para el límite del tablero
                cout << "*";
            }
            // Comprueba si la posición actual (i, j) coincide con la posición de la cabeza de la serpiente. Si se cumple, se dibuja una "O"
            else if (i == y && j == x)
            {
                EstablecerColor(10); // Establece el color verde para la serpiente
                cout << "O";
            }
            // Si la posición actual (i, j) coincide con la posición de la fruta. Si se cumple, se dibuja una "F" en esa posición
            else if (i == frutaY && j == frutaX)
            {
                EstablecerColor(12); // Establece el color rojo para la fruta
                cout << "F";
            }
            else   // Si la posición actual no coincide con ninguna de las condiciones, se dibuja un espacio en blanco en esa posición
            {
                bool imprimirCola = false;

                // Se encarga de recorrer la cola de la serpiente. La variable k se inicializa en 0 y se incrementa en 1 en cada iteración hasta que llega al valor de longitudCola, que es la longitud de la cola de la serpiente
                for (int k = 0; k < longitudCola; k++)
                {
                    // Si la posición actual (i, j) coincide con alguna parte del cuerpo de la serpiente. Si se cumple, se dibuja una "o" en esa posición
                    if (colaX[k] == j && colaY[k] == i)
                    {
                        EstablecerColor(10); // Establece el color verde para la serpiente
                        cout << "o";
                        imprimirCola = true;
                    }
                }

                // Si no se encontró ninguna parte del cuerpo en la posición actual, dibuja un espacio en blanco
                if (!imprimirCola)
                    cout << " ";
            }
        }
        cout << endl; // Imprime un espacio en blanco en la pantalla y luego salta a la siguiente fila
    }

    EstablecerColor(15); // Restaura el color predeterminado
    cout << "Puntaje:" << puntaje << endl; //  Imprime el puntaje actual en la pantalla
}

void Entrada()
{
    if (_kbhit()) // Verifica si se ha presionado una tecla
    {
        char tecla = _getch(); //conio.h Permite leer una tecla del teclado sin mostrarla en la pantalla

        switch (tolower(tecla))// Obtiene la tecla presionada
        {
        case 'a':
            if (dir != DERECHA)
                dir = IZQUIERDA; // Cambia la dirección a la izquierda
            break;
        case 'd':
            if (dir != IZQUIERDA)
                dir = DERECHA; // Cambia la dirección a la derecha
            break;
        case 'w':
            if (dir != ABAJO)
                dir = ARRIBA; // Cambia la dirección hacia arriba
            break;
        case 's':
            if (dir != ARRIBA)
                dir = ABAJO; // Cambia la dirección hacia abajo
            break;
        case 'x':
            finJuego = true; // Termina el juego
            break;
        }
    }
}

void Logica()
{
    // Guardar la posición anterior de la cabeza de la serpiente
    int prevX = colaX[0]; // Guardar la posición X anterior
    int prevY = colaY[0]; // Guardar la posición Y anterior

    // Variables temporales para actualizar la posición de las partes de la cola
    int prev2X, prev2Y;

    // Actualizar la posición de la cabeza de la serpiente
    colaX[0] = x; // Actualizar la posición X de la cabeza
    colaY[0] = y; // Actualizar la posición Y de la cabeza

    // Actualizar la posición de las partes de la cola
    for (int i = 1; i < longitudCola; i++)
    {
        // Guardar la posición anterior de la parte de la cola actual
        prev2X = colaX[i];
        prev2Y = colaY[i];

        // Actualizar la posición de la parte de la cola actual con la posición anterior de la parte anterior
        colaX[i] = prevX;
        colaY[i] = prevY;

        // Actualizar las variables de posición anterior para la siguiente iteración del bucle
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Actualizar la posición de la cabeza de la serpiente según la dirección actual
    switch (dir)
    {
    case IZQUIERDA:
        x--; // Mover hacia la izquierda (disminuir la posición X)
        break;
    case DERECHA:
        x++; // Mover hacia la derecha (aumentar la posición X)
        break;
    case ARRIBA:
        y--; // Mover hacia arriba (disminuir la posición Y)
        break;
    case ABAJO:
        y++; // Mover hacia abajo (aumentar la posición Y)
        break;
    }

    // Verificar si la serpiente ha chocado con los bordes del tablero
    if (x >= ancho || x < 0 || y >= alto || y < 0)
    {
        finJuego = true; // La serpiente ha chocado con los bordes, el juego termina
        return; // Salir de la función para evitar realizar más operaciones
    }

    // Verificar si la serpiente ha chocado con un asterisco
    if (x == ancho - 1 || x == 0 || y == alto - 1 || y == 0)
    {
        finJuego = true; // La serpiente ha chocado con un asterisco, el juego termina
        return; // Salir de la función para evitar realizar más operaciones
    }

    for (int i = 0; i < longitudCola; i++) //El ciclo "for" recorre cada uno de los obstáculos en la cola y verifica si la posición actual del personaje coincide con alguno de ellos.
         //La variable longitudCola representa el número de obstáculos en la cola. La variable colaX y colaY son arreglos que contienen las posiciones X e Y de cada obstáculo.    
    {
        if (colaX[i] == x && colaY[i] == y)  
            //Si se encuentra una coincidencia, se entra en el bloque de código dentro del if y se realiza la acción necesaria.
        {
            finJuego = true; // Si la cabeza de la serpiente colisiona con su cola, el juego termina
            return; // Salir de la función para evitar realizar más operaciones
        }
    }

    if (x == frutaX && y == frutaY)
    {
        puntaje += 10; // Aumenta el puntaje en 10
        frutaX = rand() % ancho; // Genera una nueva posición aleatoria para la fruta en el eje X
        frutaY = rand() % alto; // Genera una nueva posición aleatoria para la fruta en el eje Y
        longitudCola++; // Aumenta la longitud de la cola de la serpiente
    }
}

void ReiniciarJuego()
{
    // Restablecer todas las variables y configuraciones del juego a su estado inicial
    finJuego = false; // Se restablece la variable que indica si el juego ha terminado
    dir = PARAR; // Se restablece la dirección de la serpiente a parar
    x = ancho / 2; // Se restablece la posición horizontal de la serpiente al centro del tablero
    y = alto / 2; // Se restablece la posición vertical de la serpiente al centro del tablero
    frutaX = rand() % ancho; // Se genera una nueva posición aleatoria para la fruta en el ancho del tablero
    frutaY = rand() % alto; // Se genera una nueva posición aleatoria para la fruta en el alto del tablero
    puntaje = 0; // Se restablece el puntaje a cero
    longitudCola = 0; // Se restablece la longitud de la cola de la serpiente a cero
}

int main()
{
    HWND consola = GetConsoleWindow(); // Se obtiene un handle a la ventana de la consola
    RECT r; // Se define una estructura RECT para almacenar las coordenadas de la ventana de la consola
    GetWindowRect(consola, &r); // Se obtienen las coordenadas de la ventana de la consola
    MoveWindow(consola, r.left, r.top, 400, 400, TRUE); // Se cambia el tamaño de la ventana de la consola a 400x400 pixels

    bool jugar = true; // Se define una variable booleana para controlar si el usuario desea volver a jugar

    while (jugar) // Se ejecuta el juego mientras el usuario desee seguir jugando
    {
        Configuracion(); // Se configura el juego inicialmente

        while (!finJuego) // Se ejecuta el juego mientras el juego no haya terminado
        {
            Dibujar(); // Se dibuja el juego en la pantalla
            Entrada(); // Se lee la entrada del usuario
            Logica(); // Se aplica la lógica del juego
            Sleep(100); // Se pausa el programa durante 100 milisegundos para mejorar la experiencia de juego
        }

        // El juego ha terminado, se pregunta al usuario si desea volver a jugar
        char opcion;
        cout << "Has perdido. Quieres volver a jugar? (s/n): "; 
        cin >> opcion;

        if (opcion == 's' || opcion == 'S') // Si el usuario escribe "s" o "S", se reinicia el juego
        {
            ReiniciarJuego(); // Se reinicia el juego
        }
        else // Si el usuario escribe cualquier otra cosa, se sale del bucle principal
        {
            jugar = false; // Termino del juego
        }
    }

    return 0;
}