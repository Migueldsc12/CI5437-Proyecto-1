# === Instrucciones para Compilar y Ejecutar ===

1. Compilación:
   - Abre una terminal en la carpeta donde se encuentra el archivo `makefile`.
   - Ejecuta el siguiente comando para compilar el programa:
     ```
     make
     ```
   - Esto generará un ejecutable llamado `puzzle_solver`.

2. Ejecución:
   - Ejecuta el programa con el siguiente comando:
     ```
     ./puzzle_solver
     ```
   - El programa leerá el tablero inicial desde la entrada estándar. Ingresa el tablero en formato 4x4, usando 0 para representar el espacio vacío.

3. Ejemplo de Entrada:

    1 2 3 4
    5 6 7 8
    9 10 11 12
    13 14 0 15


4. Ejemplo de Salida:

   Secuencia de tableros:
    1 2 3 4 
    5 6 7 8 
    9 10 11 12 
    13 14 0 15 

    1 2 3 4 
    5 6 7 8 
    9 10 11 12 
    13 14 15 0 

    Longitud del camino: 1
   
    Estados generados: 13



6. Caso sin Solución:
- Si el tablero no tiene solución, el programa imprimirá:
  ```
  No hay solucion
  ```

=== Explicación del Algoritmo ===

El programa utiliza el algoritmo IDA* (Iterative Deepening A*) con una heurística híbrida que combina la distancia Manhattan dividida por 3 y la heurística de conflicto lineal mas walking distance. 
Esta heurística guía la búsqueda para encontrar la solución óptima al 15-Puzzle.

=== Autores ===

- Nombre:
  
    - Miguel Salomon - 1910274
  
    - Laura Parilli - 1710778


