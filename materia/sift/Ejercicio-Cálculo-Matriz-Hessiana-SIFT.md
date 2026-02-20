# Visión por Computador Universidad Politécnica Salesiana

## Ejercicio de cálculo de la Matriz Hessiana dada una sub-imagen

Este ejercicio tiene como objetivo que los estudiantes puedan comprender de mejor manera la detección de características mediante el análisis de la curvatura local, técnica fundamental en algoritmos como SIFT (Scale-Invariant Feature Transform) y SURF.

A continuación, se describe la propuesta de ejercicio con una matriz diseñada específicamente para que el resultado sea un **Keypoint válido** (determinante positivo y valor de umbral menor a 12.1).

---

## Ejercicio: Análisis de Estabilidad de Keypoints mediante la Matriz Hessiana

**Contexto:** Estamos en la etapa de refinamiento de puntos clave de SIFT. Se ha detectado un candidato a punto clave en una imagen de satélite. La tarea de los estudiantes es determinar si este punto es una esquina estable o si debe ser descartado por ser un borde o un punto de silla.

Dada la siguiente submatriz de $3 \times 3$ centrada en el píxel $I(x,y) = 150$:

$$M = \begin{bmatrix} 50 & 80 & 60 \\ 90 & 150 & 100 \\ 70 & 110 & 80 \\ \end{bmatrix}$$

### 1. Fórmulas de Diferencias Finitas (Segundas Derivadas)

Para construir la Hessiana $H = \begin{bmatrix} D_{xx} & D_{xy} \\ D_{xy} & D_{yy} \end{bmatrix}$, utiliza las siguientes aproximaciones:

* $D_{xx}$: $I(x+1, y) - 2I(x, y) + I(x-1, y)$
* $D_{yy}$: $I(x, y+1) - 2I(x, y) + I(x, y-1)$
* $D_{xy}$: $\frac{I(x+1, y+1) - I(x-1, y+1) - I(x+1, y-1) + I(x-1, y-1)}{4}$

### 2. Desarrollo de la Solución (Cálculo Manual)

A. Calcular $D_{xx}$ (Curvatura Horizontal)

* Miramos a la izquierda y derecha del centro ($150$):
* Derecha ($x+1$): $100$
* Centro ($x$): $150$
I* zquierda ($x-1$): $90$

$$D_{xx} = 100 - 2(150) + 90 = 100 - 300 + 90 = \mathbf{-110}$$
B. Calcular $D_{yy}$ (Curvatura Vertical)
Miramos arriba y abajo del centro ($150$):
Abajo ($y+1$): $110$
Centro ($y$): $150$
Arriba ($y-1$): $80$

$$D_{yy} = 110 - 2(150) + 80 = 110 - 300 + 80 = \mathbf{-110}$$
C. Calcular $D_{xy}$ (Torsión Diagonal)
Miramos las 4 esquinas de la matriz:
Abajo-Derecha ($x+1, y+1$): $80$
Arriba-Izquierda ($x-1, y-1$): $50$
Arriba-Derecha ($x+1, y-1$): $60$
Abajo-Izquierda ($x-1, y+1$): $70$

$$D_{xy} = \frac{(80 + 50) - (60 + 70)}{4} = \frac{130 - 130}{4} = \mathbf{0}$$

### 3. La Matriz Hessiana Resultante

Para el píxel central, la Matriz Hessiana es:

$$H = \begin{bmatrix} -110 & 0 \\ 0 & -110 \end{bmatrix}$$

### 4. Evaluación de SIFT (Prueba de Curvatura Principal)

Calculamos la Traza ($Tr$) y el Determinante ($Det$) para evaluar la estabilidad:

* Traza ($Tr$): $(-110) + (-110) = \mathbf{-220}$
* Determinante ($Det$): $(-110 \cdot -110) - (0)^2 = \mathbf{12,100}$
* Análisis intermedio: Como el determinante es positivo, sabemos que no es un punto de silla. Al ser la traza negativa y el determinante positivo, estamos ante un máximo local (una "montaña" de intensidad).
* Aplicamos la relación de curvaturas de SIFT ($r=10$):

$$\frac{Tr^2}{Det} = \frac{(-220)^2}{12,100} = \frac{48,400}{12,100} = \mathbf{4.0}$$

### 5. Condición de Aceptación (El Umbral)

El umbral estándar de SIFT para descartar bordes es:

$$\text{Umbral Máximo} = \frac{(r+1)^2}{r} = \frac{121}{10} = \mathbf{12.1}$$
Evaluación del punto:

$$4.0 < 12.1$$
Conclusión Final: Como $4.0$ es menor que $12.1$ y el determinante es positivo, el punto SE ACEPTA.

Interpretación física: Las curvaturas en $X$ e $Y$ son idénticas ($-110$), lo que indica que el punto es perfectamente simétrico (como una semiesfera). Es un punto extremadamente estable, ideal para ser un Keypoint, ya que no presenta la inestabilidad de un borde (donde una curvatura sería mucho mayor que la otra).
