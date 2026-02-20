# Visión por Computador

## Universidad Politécnica Salesiana

## Ejercicio de cálculo de Momentos de Zernike con Números Complejos

**Evaluación Práctica:** Cálculo de Descriptores de Forma mediante Momentos de Zernike con $m \neq 0$

Instrucciones: Lea detenidamente el planteamiento y realice los cálculos manuales solicitados. Emplee al menos cuatro decimales en sus operaciones intermedias para garantizar la precisión del resultado final. Este ejercicio involucra aritmética de números complejos.

---

### 1. Condiciones de Validez de los Momentos de Zernike

Antes de proceder, recordemos las **condiciones necesarias** para que un momento de Zernike $A_{n,m}$ esté bien definido:

1. **Orden no negativo:** $n \geq 0$
2. **Restricción de repetición:** $|m| \leq n$
3. **Paridad:** $n - |m|$ debe ser un número **par**

Para este ejercicio elegimos $n = 2$ y $m = 2$:

- $n = 2 \geq 0$ ✓
- $|m| = 2 \leq n = 2$ ✓
- $n - |m| = 2 - 2 = 0$ (par) ✓

---

### 2. Definición del Escenario

Se presenta una matriz de intensidad de $5 \times 5$ píxeles que contiene un patrón en forma de "L". Se requiere calcular el Momento de Zernike de orden $n=2$ y repetición $m=2$ ($A_{2,2}$).

La matriz de entrada $I$ es la siguiente:

$$I = \begin{bmatrix} 1 & 0 & 0 & 0 & 0 \\ 1 & 0 & 0 & 0 & 0 \\ 1 & 0 & 0 & 0 & 0 \\ 1 & 0 & 0 & 0 & 0 \\ 1 & 1 & 1 & 1 & 1 \end{bmatrix}$$

**Parámetros de cálculo:**

- Orden ($n$): 2
- Repetición ($m$): 2
- Factor de escala: $\frac{n+1}{\pi} = \frac{3}{\pi} \approx 0.9549$
- Polinomio Radial: $R_{2,2}(\rho) = \rho^2$

---

### 3. Formulación Matemática

El momento de Zernike se define como:

$$A_{n,m} = \frac{n+1}{\pi} \sum_{x} \sum_{y} I(x,y) \cdot V^*_{n,m}(\rho, \theta)$$

Donde la función base de Zernike es:

$$V_{n,m}(\rho, \theta) = R_{n,|m|}(\rho) \cdot e^{im\theta}$$

Y su conjugado complejo:

$$V^*_{n,m}(\rho, \theta) = R_{n,|m|}(\rho) \cdot e^{-im\theta}$$

Para $m = 2$, utilizamos la identidad de Euler:

$$e^{-i2\theta} = \cos(2\theta) - i\sin(2\theta)$$

Por lo tanto, el momento se expresa como:

$$A_{2,2} = \frac{3}{\pi} \sum_{x} \sum_{y} I(x,y) \cdot \rho^2 \cdot [\cos(2\theta) - i\sin(2\theta)]$$

---

### 4. Cálculo del Polinomio Radial $R_{2,2}(\rho)$

La fórmula general del polinomio radial es:

$$R_{n,|m|}(\rho) = \sum_{s=0}^{\frac{n-|m|}{2}} \frac{(-1)^s (n-s)!}{s! \left(\frac{n+|m|}{2}-s\right)! \left(\frac{n-|m|}{2}-s\right)!} \rho^{n-2s}$$

Para $n = 2$, $|m| = 2$:

- El límite superior de la suma es: $\frac{n-|m|}{2} = \frac{2-2}{2} = 0$
- Solo existe el término $s = 0$

$$R_{2,2}(\rho) = \frac{(-1)^0 (2-0)!}{0! \cdot 2! \cdot 0!} \rho^{2-0} = \frac{1 \cdot 2}{1 \cdot 2 \cdot 1} \rho^2 = \rho^2$$

---

### 5. Solución Detallada Paso a Paso

**Paso A:** Definición del Espacio de Coordenadas

Para una imagen de $5 \times 5$:

Centro Geométrico $(c_x, c_y)$:

$$c_x = \frac{5-1}{2} = 2, \quad c_y = \frac{5-1}{2} = 2$$

Distancia Máxima ($R_{max}$):

$$R_{max} = \sqrt{(0-2)^2 + (0-2)^2} = \sqrt{8} \approx 2.8284$$

Por conveniencia: $R_{max}^2 = 8$

---

**Paso B:** Identificación de Píxeles Activos

Los píxeles donde $I(x,y) = 1$ son (usando indexación desde 0):

| Píxel | Posición (fila, col) | $(x - c_x)$ | $(y - c_y)$ |
| ------- | --------------------- | ------------- | ------------- |
| P1 | (0, 0) | -2 | -2 |
| P2 | (1, 0) | -1 | -2 |
| P3 | (2, 0) | 0 | -2 |
| P4 | (3, 0) | 1 | -2 |
| P5 | (4, 0) | 2 | -2 |
| P6 | (4, 1) | 2 | -1 |
| P7 | (4, 2) | 2 | 0 |
| P8 | (4, 3) | 2 | 1 |
| P9 | (4, 4) | 2 | 2 |

**Nota:** Aquí definimos $x$ como la coordenada vertical (fila) y $y$ como la horizontal (columna), ambas centradas.

---

**Paso C:** Cálculo de $\rho^2$ y $\theta$ para cada píxel

Para cada píxel calculamos:

- $d^2 = (x - c_x)^2 + (y - c_y)^2$
- $\rho^2 = \frac{d^2}{R_{max}^2} = \frac{d^2}{8}$
- $\theta = \arctan2(y - c_y, x - c_x)$

| Píxel | $d^2$ | $\rho^2$ | $\rho$ | $\theta$ (rad) | $\theta$ (grados) |
| ------- | ------- | ---------- | -------- | ---------------- | ------------------- |
| P1 | $(-2)^2+(-2)^2=8$ | $1.0000$ | $1.0000$ | $\arctan2(-2,-2) = -\frac{3\pi}{4}$ | $-135°$ |
| P2 | $(-1)^2+(-2)^2=5$ | $0.6250$ | $0.7906$ | $\arctan2(-2,-1) \approx -2.0344$ | $-116.57°$ |
| P3 | $(0)^2+(-2)^2=4$ | $0.5000$ | $0.7071$ | $\arctan2(-2,0) = -\frac{\pi}{2}$ | $-90°$ |
| P4 | $(1)^2+(-2)^2=5$ | $0.6250$ | $0.7906$ | $\arctan2(-2,1) \approx -1.1071$ | $-63.43°$ |
| P5 | $(2)^2+(-2)^2=8$ | $1.0000$ | $1.0000$ | $\arctan2(-2,2) = -\frac{\pi}{4}$ | $-45°$ |
| P6 | $(2)^2+(-1)^2=5$ | $0.6250$ | $0.7906$ | $\arctan2(-1,2) \approx -0.4636$ | $-26.57°$ |
| P7 | $(2)^2+(0)^2=4$ | $0.5000$ | $0.7071$ | $\arctan2(0,2) = 0$ | $0°$ |
| P8 | $(2)^2+(1)^2=5$ | $0.6250$ | $0.7906$ | $\arctan2(1,2) \approx 0.4636$ | $26.57°$ |
| P9 | $(2)^2+(2)^2=8$ | $1.0000$ | $1.0000$ | $\arctan2(2,2) = \frac{\pi}{4}$ | $45°$ |

---

**Paso D:** Evaluación del Polinomio Radial $R_{2,2}(\rho) = \rho^2$

| Píxel | $\rho^2$ | $R_{2,2}(\rho)$ |
| ------- | ---------- | ----------------- |
| P1 | $1.0000$ | $1.0000$ |
| P2 | $0.6250$ | $0.6250$ |
| P3 | $0.5000$ | $0.5000$ |
| P4 | $0.6250$ | $0.6250$ |
| P5 | $1.0000$ | $1.0000$ |
| P6 | $0.6250$ | $0.6250$ |
| P7 | $0.5000$ | $0.5000$ |
| P8 | $0.6250$ | $0.6250$ |
| P9 | $1.0000$ | $1.0000$ |

---

**Paso E:** Cálculo de la Componente Angular Compleja

Para cada píxel calculamos $e^{-i2\theta} = \cos(2\theta) - i\sin(2\theta)$:

| Píxel | $\theta$ | $2\theta$ | $\cos(2\theta)$ | $\sin(2\theta)$ | $e^{-i2\theta}$ |
| ------- | ---------- | ----------- | ----------------- | ----------------- | ----------------- |
| P1 | $-\frac{3\pi}{4}$ | $-\frac{3\pi}{2}$ | $0.0000$ | $1.0000$ | $0 - i(1) = -i$ |
| P2 | $-2.0344$ | $-4.0689$ | $-0.6000$ | $0.8000$ | $-0.6000 - 0.8000i$ |
| P3 | $-\frac{\pi}{2}$ | $-\pi$ | $-1.0000$ | $0.0000$ | $-1 - 0i = -1$ |
| P4 | $-1.1071$ | $-2.2143$ | $-0.6000$ | $-0.8000$ | $-0.6000 + 0.8000i$ |
| P5 | $-\frac{\pi}{4}$ | $-\frac{\pi}{2}$ | $0.0000$ | $-1.0000$ | $0 - (-1)i = i$ |
| P6 | $-0.4636$ | $-0.9273$ | $0.6000$ | $-0.8000$ | $0.6000 + 0.8000i$ |
| P7 | $0$ | $0$ | $1.0000$ | $0.0000$ | $1 + 0i = 1$ |
| P8 | $0.4636$ | $0.9273$ | $0.6000$ | $0.8000$ | $0.6000 - 0.8000i$ |
| P9 | $\frac{\pi}{4}$ | $\frac{\pi}{2}$ | $0.0000$ | $1.0000$ | $0 - i(1) = -i$ |

---

**Paso F:** Cálculo de la Contribución de Cada Píxel

La contribución de cada píxel es: $C_k = I(x,y) \cdot R_{2,2}(\rho) \cdot e^{-i2\theta}$

| Píxel | $R_{2,2}$ | $e^{-i2\theta}$ | Contribución $C_k$ |
| ------- | ----------- | ----------------- | ------------------- |
| P1 | $1.0000$ | $-i$ | $-i$ |
| P2 | $0.6250$ | $-0.6 - 0.8i$ | $-0.3750 - 0.5000i$ |
| P3 | $0.5000$ | $-1$ | $-0.5000$ |
| P4 | $0.6250$ | $-0.6 + 0.8i$ | $-0.3750 + 0.5000i$ |
| P5 | $1.0000$ | $i$ | $i$ |
| P6 | $0.6250$ | $0.6 + 0.8i$ | $0.3750 + 0.5000i$ |
| P7 | $0.5000$ | $1$ | $0.5000$ |
| P8 | $0.6250$ | $0.6 - 0.8i$ | $0.3750 - 0.5000i$ |
| P9 | $1.0000$ | $-i$ | $-i$ |

---

**Paso G:** Sumatoria Total

Sumamos todas las contribuciones complejas:

**Parte Real:**
$$\text{Re} = -0.3750 - 0.5000 - 0.3750 + 0.3750 + 0.5000 + 0.3750 = 0.0000$$

**Parte Imaginaria:**
$$\text{Im} = -1 - 0.5000 + 0.5000 + 1 + 0.5000 - 0.5000 - 1 = -1.0000$$

**Suma Total:**
$$\sum C_k = 0.0000 - 1.0000i = -i$$

---

**Paso H:** Resultado Final del Momento

Aplicamos el factor de escala $\frac{3}{\pi} \approx 0.9549$:

$$A_{2,2} = \frac{3}{\pi} \cdot (-i) = -0.9549i$$

**Expresión en forma polar:**

$$|A_{2,2}| = 0.9549$$
$$\phi = \arg(A_{2,2}) = -\frac{\pi}{2} = -90°$$

Por lo tanto:

$$\boxed{A_{2,2} = -0.9549i \approx 0.9549 \angle -90°}$$

---

### 6. Interpretación del Resultado

El momento de Zernike $A_{2,2}$ es un **número imaginario puro** con magnitud $0.9549$.

- La **magnitud** $|A_{2,2}| = 0.9549$ es invariante ante rotaciones de la imagen y representa cuánto "se parece" el patrón a la base de Zernike $V_{2,2}$.

- La **fase** $\arg(A_{2,2}) = -90°$ codifica la orientación del patrón. Si rotáramos la imagen original, la magnitud permanecería constante pero la fase cambiaría proporcionalmente.

---

### 7. Pregunta de Análisis para el Estudiante

**Consigna:** Si rotamos la imagen de entrada $I$ en un ángulo $\alpha = 45°$ en sentido antihorario, ¿cómo cambiaría el momento $A_{2,2}$?

**Respuesta esperada:**

La magnitud $|A_{2,2}|$ permanece invariante: $|A'_{2,2}| = |A_{2,2}| = 0.9549$

La fase se modifica según: $\arg(A'_{2,2}) = \arg(A_{2,2}) - m \cdot \alpha$

$$\arg(A'_{2,2}) = -90° - 2 \times 45° = -90° - 90° = -180°$$

Por lo tanto:
$$A'_{2,2} = 0.9549 \angle -180° = -0.9549$$

Esto demuestra la propiedad fundamental: los momentos de Zernike capturan la orientación en su fase, mientras que la magnitud describe la forma independientemente de la rotación.

---

### 8. Resumen de Propiedades Complejas

| Condición | Resultado |
| ----------- | ----------- |
| $m = 0$ | Momento **real puro** (sin componente imaginaria) |
| $m \neq 0$ | Momento **complejo** (tiene fase angular) |
| $m > 0$ | Captura patrones con simetría rotacional de orden $m$ |
| Rotación por $\alpha$ | La fase cambia en $-m\alpha$, la magnitud es invariante |
