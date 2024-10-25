# Simulador de Autómata de Pila (PDA) en C++

## Descripción

Este proyecto es un simulador de Autómata de Pila (PDA) implementado en C++ utilizando principios de programación orientada a objetos. El simulador puede procesar cadenas de entrada y determinar si son aceptadas o rechazadas por un PDA definido por el usuario. Admite dos criterios de aceptación:

- APf: Aceptación por estado final.
- APv: Aceptación por pila vacía.
El simulador también ofrece un modo de trazado que muestra paso a paso la ejecución del PDA, lo cual es útil para fines educativos y de depuración.

## Requisitos

- Compilador C++ compatible con el estándar C++11 o superior.
- Sistema operativo: El código es multiplataforma y puede compilarse en Windows, Linux o macOS.
- Herramienta de compilación: Se proporciona un Makefile para facilitar la compilación.

## Instalación

1. Clonar el repositorio (si corresponde) o descargar los archivos del proyecto.

2. Compilar el proyecto utilizando el Makefile proporcionado con el comando `make`.

## Uso
El simulador se ejecuta desde la línea de comandos y acepta varias opciones:
  
  ``` bash
  ./pda_simulator [opciones] <archivo_definicion_automata> <archivo_cadenas_entrada>
  ```

## Opciones disponibles

- -h, --help: Muestra el mensaje de ayuda y sale.
- -m, --mode \<modo>: Especifica el modo de ejecución:
  - none: Ejecución regular (por defecto).
  - trace: Modo de trazado, salida por consola.
  - tracefile: Modo de trazado, salida a un archivo especificado.
- -o, --output \<archivo>: Especifica el archivo de salida para el modo de trazado.
- -a, --acceptance \<tipo>: Especifica el criterio de aceptación. Tipos disponibles:
- apf: Aceptación por estado final (por defecto).
- apv: Aceptación por pila vacía.

## Ejemplos de uso

Ejecución regular:
``` bash
./pda_simulator automata.txt entradas.txt
```

Modo de trazado con salida por consola:
``` bash
./pda_simulator -m trace automata.txt entradas.txt
```

Modo de trazado con salida a un archivo:
``` bash
./pda_simulator -m tracefile -o output.txt automata.txt entradas.txt
```

Ejecución con criterio de aceptación por pila vacía:
``` bash
./pda_simulator -a apv automata.txt entradas.txt
```

## Formato de los archivos de definición

### Archivo de definición del autómata

El archivo debe contener la definición del PDA siguiendo un formato específico:

1. Estados: Lista de estados separados por espacios.

    > q1 q2 q3
2. Alfabeto de entrada: Lista de símbolos separados por espacios.

    > a b

3. Alfabeto de pila: Lista de símbolos separados por espacios.
  
    > A B

4. Estado inicial: Un solo estado.
  
    > q1

5. Símbolo inicial de la pila: Un solo símbolo.
  
    > A

6. Transiciones: Lista de transiciones en el formato:

 `estado_actual simbolo_entrada simbolo_pila estado_siguiente cadena_pila`.

- Epsilon se representa con un punto (.)
- Ejemplo:
  > q1 a S q1 AS

  > q1 a A q1 AA

  > q1 b A q2 .

  > q2 b A q2 .

  > q2 . S q3 S

## Archivo de cadenas de entrada (entradas.txt)
Lista de cadenas a procesar por el PDA, una por línea. Las líneas en blanco o que comienzan con # se ignoran.

``` bash
ab
aabb
aaabbb
```

## Clases Definidas

El proyecto está estructurado utilizando clases para representar los componentes del PDA.

### 1. Clase `State`

**Descripción**: Representa un estado en el PDA.

- **Atributos**:
  - `std::string name`: Nombre del estado.
- **Constructores**:
  - `State()`: Constructor por defecto.
  - `State(const std::string& name)`: Constructor con nombre de estado.
- **Métodos**:
  - `std::string getName() const`: Retorna el nombre del estado.
- **Operadores sobrecargados**:
  - `bool operator<(const State& other) const`: Permite comparar estados (necesario para usar en `std::set`).
  - `bool operator==(const State& other) const`: Permite verificar si dos estados son iguales.

### 2. Clase `Transition`

**Descripción**: Representa una transición en el PDA.

- **Atributos**:
  - `State currentState`: Estado actual.
  - `char inputSymbol`: Símbolo de entrada (usar `'\0'` para epsilon).
  - `char stackSymbol`: Símbolo en la cima de la pila (usar `'\0'` para epsilon).
  - `State nextState`: Estado siguiente.
  - `std::string symbolsToPush`: Secuencia de símbolos a apilar (puede ser vacía para epsilon).
- **Constructor**:
  - `Transition(const State& currentState, char inputSymbol, char stackSymbol, const State& nextState, const std::string& symbolsToPush)`

### 3. Clase `Stack`

**Descripción**: Simula la pila del PDA.

- **Atributos**:
  - `std::vector<char> stackContainer`: Contenedor interno para la pila.
- **Constructor**:
  - `Stack(char initialSymbol)`: Inicializa la pila con el símbolo inicial.
- **Métodos**:
  - `void push(const std::string& symbols)`: Apila una secuencia de símbolos.
  - `char pop()`: Desapila el símbolo en la cima.
  - `char peek() const`: Devuelve el símbolo en la cima sin desapilar.
  - `bool isEmpty() const`: Verifica si la pila está vacía.
  - `void display() const`: Muestra el contenido actual de la pila (para depuración).
  - `void display(std::string& out) const`: Añade el contenido de la pila a una cadena (para modo de trazado).

### 4. Clase `PDA`

**Descripción**: Representa el autómata de pila completo.

- **Atributos**:
  - `std::set<State> states`: Conjunto de estados.
  - `std::set<char> inputAlphabet`: Alfabeto de entrada.
  - `std::set<char> stackAlphabet`: Alfabeto de la pila.
  - `State initialState`: Estado inicial.
  - `char initialStackSymbol`: Símbolo inicial de la pila.
  - `std::set<State> finalStates`: Conjunto de estados finales (si se usa APf).
  - `std::vector<Transition> transitions`: Lista de transiciones.
  - `bool useFinalStateAcceptance`: Indica el criterio de aceptación (APf o APv).
- **Constructor**:
  - `PDA(bool useFinalStateAcceptance = true)`: Por defecto, utiliza aceptación por estado final.
- **Métodos para construir el PDA**:
  - `void addState(const std::string& stateName)`: Añade un estado.
  - `void addInputSymbol(char symbol)`: Añade un símbolo al alfabeto de entrada.
  - `void addStackSymbol(char symbol)`: Añade un símbolo al alfabeto de la pila.
  - `void setInitialState(const std::string& stateName)`: Define el estado inicial.
  - `void setInitialStackSymbol(char symbol)`: Define el símbolo inicial de la pila.
  - `void addFinalState(const std::string& stateName)`: Añade un estado final (si se usa APf).
  - `void addTransition(const std::string& currentState, char inputSymbol, char stackSymbol, const std::string& nextState, const std::string& symbolsToPush)`: Añade una transición.
- **Métodos para procesar cadenas de entrada**:
  - `bool processInput(const std::string& input)`: Procesa una cadena en modo regular.
  - `bool processInputTrace(const std::string& input, const std::string& outputMode)`: Procesa una cadena en modo de trazado.
- **Métodos auxiliares**:
  - `bool processRecursive(const State& currentState, const std::string& remainingInput, Stack& stack)`: Método recursivo para el procesamiento.
  - `bool processRecursiveTrace(const State& currentState, const std::string& remainingInput, Stack& stack, std::string& trace, int depth = 0)`: Método recursivo para el modo de trazado.
  - `std::vector<Transition> getPossibleTransitions(const State& state, char inputSymbol, char stackSymbol)`: Obtiene las transiciones posibles desde un estado dado.

### 5. Clase `InputHandler`

**Descripción**: Maneja la lectura y escritura de archivos.

- **Métodos estáticos**:
  - `static bool loadAutomatonDefinition(PDA& pda, const std::string& filename)`: Carga la definición del PDA desde un archivo.
  - `static std::vector<std::string> loadInputStrings(const std::string& filename)`: Carga las cadenas de entrada desde un archivo.
  - `static void writeTraceOutput(const std::string& trace, const std::string& filename)`: Escribe la salida del modo de trazado en un archivo.

## Compilación y Ejecución

- **Compilar el programa**:

```bash
make
``` 

- **Ejecutar el programa**:

```bash
./pda_simulator [opciones] <archivo_definicion_automata> <archivo_cadenas_entrada>
```

- **Limpiar archivos de compilación**:

```bash
make clean
```

