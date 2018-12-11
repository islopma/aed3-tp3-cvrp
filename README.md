# Heurísticas para CVRP

## Instrucciones para compilar

1. Ejecutar CMake: **cmake CMakeLists.txt**
2. Compilar: **make**

## Ejecutables

1. savings -> solución del algoritmo de Savings
2. closest -> solución del algoritmo constructivo goloso Closest
3. cfrs -> solución del primer algoritmo constructivo de Cluster First, Route Second
4. cfrs2 -> solución del segundo algoritmo constructivo de Cluster First, Route Second
5. simanneal -> solución del algoritmo de Simulated Annealing
6. savings_time -> tiempo del algoritmo de Savings
7. closest_time -> tiempo del algoritmo constructivo goloso Closest
8. cfrs_time -> tiempo del primer algoritmo constructivo de Cluster First, Route Second
9. cfrs2_time -> tiempo del segundo algoritmo constructivo de Cluster First, Route Second
10. simanneal_time -> tiempo del algoritmo de Simulated Annealing

Los algoritmos reciben por entrada estándar una instancia de CVRP, por ejemplo:
    ./savings < A-n37-k5.vrp

Adicionalmente, simanneal puede recibir los siguientes parámetros:
    simanneal [R=cantidad de resets] [initialSolution={Savings,Closest}] [SwapOnly=solo usa swap, no hace insert] [ProgressMode=imprime temperatura y costo de cada solucion que explora]
    e.g.,:
        ./simanneal 5 Savings SwapOnly ProgressMode < A-n37-k5.vrp
        ./simanneal 3 Closest < A-n37-k5.vrp
Ídem para simanneal_time excepto por 'ProgressMode'