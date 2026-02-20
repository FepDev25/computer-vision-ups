# Visión por Computador

## Universidad Politécnica Salesiana

## Ejercicio de cálculo de Momentos de Zernike

**Evaluación Práctica:** Cálculo de Descriptores de Forma mediante Momentos de Zernike
Instrucciones: Lea detenidamente el planteamiento y realice los cálculos manuales solicitados. Emplee al menos cuatro decimales en sus operaciones intermedias para garantizar la precisión del resultado final.

### 1. Definición del Escenario

Se presenta una matriz de intensidad de $5 \times 5$ píxeles que contiene un patrón en forma de "X" (cruz diagonal). Se requiere calcular la contribución específica de ciertos puntos y el valor final del Momento de Zernike de orden $n=2$ y repetición $m=0$ ($A_{2,0}$).
La matriz de entrada $I$ es la siguiente:

$$I = \begin{bmatrix} 1 & 0 & 0 & 0 & 1 \\ 0 & 1 & 0 & 1 & 0 \\ 0 & 0 & 1 & 0 & 0 \\ 0 & 1 & 0 & 1 & 0 \\ 1 & 0 & 0 & 0 & 1 \end{bmatrix}$$

Parámetros de cálculo:

* Orden ($n$): 2
* Repetición ($m$): 0
* Factor de escala: $\frac{n+1}{\pi} = \frac{3}{\pi} \approx 0.9549$
* Polinomio Radial: $R_{2,0}(\rho) = 2\rho^2 - 1$

### 2. Formulación Matemática

El momento de Zernike se obtiene mediante la proyección de la función de intensidad sobre el polinomio radial, dada la ausencia de componente angular ($m=0$):

$$A_{2,0} = \frac{3}{\pi} \sum_{x} \sum_{y} I(x,y) \cdot R_{2,0}(\rho)$$

Donde $\rho$ representa la distancia radial normalizada de cada píxel al centro de la imagen.

### 3. Solución Detallada Paso a Paso

**Paso A:** Definición del Espacio de Coordenadas

Para una imagen de $5 \times 5$, se deben establecer las referencias espaciales:
Centro Geométrico $(c_x, c_y)$:

$$c_x = \frac{5-1}{2} = 2, \quad c_y = \frac{5-1}{2} = 2$$

* El origen del sistema $(0,0)$ se sitúa en el píxel central de la matriz.
* Distancia Máxima ($R_{max}$):
* Para asegurar que todos los píxeles se encuentren dentro del círculo unitario ($\rho \le 1$), se calcula la distancia desde el centro a la esquina más alejada (ej. píxel en $0,0$):

$$R_{max} = \sqrt{(0-2)^2 + (0-2)^2} = \sqrt{4 + 4} = \sqrt{8} \approx 2.8284$$

Por conveniencia en los cálculos posteriores, se observa que $R_{max}^2 = 8$.

**Paso B:** Cálculo del Radio Normalizado ($\rho$)

* Se debe calcular el valor de $\rho^2$ para cada píxel donde la intensidad es $I(x,y) = 1$:
* Píxeles en las esquinas $(0,0), (0,4), (4,0), (4,4)$:
* Distancia al centro al cuadrado: $d^2 = 2^2 + 2^2 = 8$

$$\rho^2 = \frac{d^2}{R_{max}^2} = \frac{8}{8} = 1.0000$$

Píxeles internos $(1,1), (1,3), (3,1), (3,3)$:

Distancia al centro al cuadrado: $d^2 = 1^2 + 1^2 = 2$

$$\rho^2 = \frac{d^2}{R_{max}^2} = \frac{2}{8} = 0.2500$$
Píxel central $(2,2)$:
Distancia al centro al cuadrado: $d^2 = 0^2 + 0^2 = 0$

$$\rho^2 = \frac{0}{8} = 0.0000$$

**Paso C:** Evaluación del Polinomio Radial $R_{2,0}$

Se aplica la fórmula $R_{2,0}(\rho) = 2\rho^2 - 1$ a los valores obtenidos:

1. Para las esquinas ($\rho^2 = 1$):

$R_{2,0} = 2(1) - 1 = \mathbf{1.0000}$

1. Para los píxeles internos ($\rho^2 = 0.25$):
$R_{2,0} = 2(0.25) - 1 = 0.5 - 1 = \mathbf{-0.5000}$

2. Para el centro ($\rho^2 = 0$):
$R_{2,0} = 2(0) - 1 = \mathbf{-1.0000}$

**Paso D:** Sumatoria de Contribuciones
Se procede a sumar los productos $I(x,y) \cdot R_{2,0}(\rho)$ de todos los píxeles activos (donde $I=1$):

* 4 esquinas: $4 \times (1 \times 1.0) = 4.0$
* 4 puntos internos: $4 \times (1 \times -0.5) = -2.0$
* 1 punto central: $1 \times (1 \times -1.0) = -1.0$

**Suma Total:** $4.0 - 2.0 - 1.0 = \mathbf{1.0}$

**Paso E:** Resultado Final del Momento

Finalmente, se multiplica la suma por el factor de escala $\frac{3}{\pi}$:

$$A_{2,0} = 0.9549 \times 1.0 = \mathbf{0.9549}$$

### 4. Pregunta de Análisis para el Estudiante

Si se rotara la matriz de entrada $I$ en 90 grados, el patrón en "X" permanecería visualmente idéntico debido a su simetría. Sin embargo, en patrones menos simétricos, la magnitud del momento de Zernike $|A_{nm}|$ se mantiene invariante ante la rotación.

**Consigna:** Explique brevemente por qué el resultado de $A_{2,0}$ en este ejercicio es un número real puro y bajo qué condiciones de la repetición $m$ se obtendría un número complejo.

* **Respuesta esperada:** El resultado es un número real porque $m=0$. Al ser la repetición nula, el término angular de la base de Zernike ($e^{-jm\theta}$) se convierte en $e^0 = 1$, eliminando la parte imaginaria de la proyección. Se obtendría un número complejo siempre que $m \neq 0$, debido a la presencia de componentes de fase (senos y cosenos) en la base.
