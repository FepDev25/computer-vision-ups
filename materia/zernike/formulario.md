# Formulario de Momentos de Zernike (Cheat Sheet)

## 1. Configuración del tablero (Paso 0)

Antes de calcular los momentos, define correctamente el sistema de coordenadas.

### A. Centro de la matriz (cx, cy)

Para una matriz de tamaño (N \times N):

[
c_x = c_y = \frac{N - 1}{2}
]

Ejemplo:

* En una matriz (5 \times 5), el centro es ((2, 2))

---

### B. Coordenadas relativas ((x', y'))

Para cada píxel en la fila (i) y columna (j):

[x' = j - c_x]
[y' = c_y - i]

Nota:

* Algunos autores usan (y' = i - c_y) (eje Y hacia abajo)
* Zernike es independiente de la convención, pero el signo del ángulo (\theta) cambia

---

### C. Radio máximo (R_{\text{max}})

Distancia desde el centro hasta la esquina más lejana (normalmente el píxel ((0,0))):

[
R_{\text{max}} = \sqrt{(x'*{\text{esq}})^2 + (y'*{\text{esq}})^2}
]

Tip práctico:

* Guarda (R_{\text{max}}^2) para evitar raíces innecesarias

---

## 2. Variables por píxel

Se calculan solo para píxeles activos donde (I(x,y) = 1).

### A. Radio normalizado (\rho)

[
\rho = \frac{\sqrt{(x')^2 + (y')^2}}{R_{\text{max}}}
]

Forma optimizada:

[
\rho^2 = \frac{(x')^2 + (y')^2}{R_{\text{max}}^2}
]

Condición:
[
0 \le \rho \le 1
]

---

### B. Ángulo (\theta)

[
\theta = \operatorname{atan2}(y', x')
]

* Retorna el ángulo correcto en el cuadrante adecuado
* Rango típico: ((-\pi, \pi])

---

## 3. Polinomios radiales comunes (R_n^m(\rho))

Estos son los más usados y conviene memorizarlos.

| Orden (n) | Repetición (m) | (R_n^m(\rho))           |
| --------- | -------------- | ----------------------- |
| 1         | 1              | (\rho)                  |
| 2         | 0              | (2\rho^2 - 1)           |
| 2         | 2              | (\rho^2)                |
| 3         | 1              | (3\rho^3 - 2\rho)       |
| 3         | 3              | (\rho^3)                |
| 4         | 0              | (6\rho^4 - 6\rho^2 + 1) |
| 4         | 2              | (4\rho^4 - 3\rho^2)     |

---

## 4. Caso A: Momentos reales (m = 0)

Se usa cuando piden (A_{n,0}).
El resultado es **real**.

### Fórmula simplificada

[
A_{n,0} = \frac{n+1}{\pi} \sum_x \sum_y I(x,y), R_n^0(\rho)
]

---

### Algoritmo rápido

* Agrupa píxeles por distancia al centro
* Calcula (R_n^0(\rho)) una sola vez por grupo
* Suma:

  * (cantidad de píxeles) × (R_n^0(\rho))
* Multiplica por (\frac{n+1}{\pi})

---

## 5. Caso B: Momentos complejos (m ≠ 0)

Se usa cuando (m > 0).
El resultado tiene parte real e imaginaria.

### Fórmula general

[
A_n^m = \frac{n+1}{\pi} \sum_x \sum_y I(x,y), R_n^m(\rho), e^{-j m \theta}
]

---

### Expansión de Euler

[
e^{-j m \theta} = \cos(m\theta) - j \sin(m\theta)
]

---

### Tabla de sumatorias

Calcula dos sumas independientes:

* Suma real:
  [
  S_r = \sum R_n^m(\rho)\cos(m\theta)
  ]

* Suma imaginaria:
  [
  S_i = \sum R_n^m(\rho)(- \sin(m\theta))
  ]

Atención al signo menos.

---

### Resultado final

[
A_n^m = \frac{n+1}{\pi},(S_r + j S_i)
]

---

### Magnitud (invariante a rotación)

[
|A_n^m| = \frac{n+1}{\pi}\sqrt{S_r^2 + S_i^2}
]

---

## 6. Guía rápida de ángulos (\theta)

### Ángulos sobre los ejes

* (x > 0, y = 0 \rightarrow 0)
* (x = 0, y > 0 \rightarrow \frac{\pi}{2})
* (x < 0, y = 0 \rightarrow \pi)
* (x = 0, y < 0 \rightarrow -\frac{\pi}{2})
* ((0,0)) indefinido, pero (\rho = 0) anula el término

---

### Corrección de cuadrantes (si usas (\tan^{-1}(y/x)))

Si la calculadora devuelve un ángulo (\alpha):

* Si (x > 0):

  * (\theta = \alpha)

* Si (x < 0):

  * En radianes:

    * (\theta = \alpha + \pi) si (\alpha < 0)
    * (\theta = \alpha - \pi) si (\alpha > 0)

---

## 7. Propiedades de verificación

Úsalas para detectar errores.

* Paridad:
  [
  n - m \text{ debe ser par}
  ]
  Si te piden (A_3^2), el resultado es cero o inválido

* Restricción:
  [
  m \le n
  ]

* Simetría:
  [
  A_n^{-m} = (A_n^m)^*
  ]

* Rotación:
  [
  |A_n^m| \text{ no cambia si la imagen rota}
  ]

---
