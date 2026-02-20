# Análisis Exhaustivo del Histograma de Gradientes Orientados (HOG): Paradigmas, Implementación y Vigencia en la Visión Computacional Contemporánea

## 1. Introducción: La Génesis de la Descripción de Forma Local

La evolución de la visión por computador se puede cartografiar a través de los hitos en la ingeniería de características. Antes de la democratización de las arquitecturas de aprendizaje profundo, la capacidad de un sistema para "entender" una imagen dependía enteramente de la robustez matemática de los descriptores diseñados manualmente (hand-crafted features). En este panorama, el Histograma de Gradientes Orientados (HOG), formalizado y popularizado por Navneet Dalal y Bill Triggs en su presentación seminal ante la Conferencia de Visión por Computador y Reconocimiento de Patrones (CVPR) en 2005 1, representa un punto de inflexión fundamental. Su trabajo, motivado por la necesidad de una detección de peatones más robusta en entornos estáticos, estableció un estándar de rendimiento que perduraría durante más de una década y que, incluso en 2025, sigue siendo relevante en nichos específicos de alta eficiencia.3

Aunque los conceptos subyacentes de utilizar la orientación de los gradientes para codificar la información visual fueron explorados preliminarmente por investigadores como Robert K. McConnell en patentes de 1986 y por laboratorios de Mitsubishi Electric en 1994 1, fue la configuración específica de Dalal y Triggs —con su énfasis en la normalización de bloques superpuestos y el uso de gradientes sin signo— la que catalizó su adopción masiva. A diferencia de los métodos contemporáneos de la época, como las características de Haar utilizadas en el marco de Viola-Jones, que dependían de diferencias de intensidad en regiones rectangulares simples, HOG se fundamentó en la premisa de que la apariencia y la forma de los objetos locales pueden caracterizarse de manera efectiva mediante la distribución estadística de los gradientes de intensidad o las direcciones de los bordes, incluso sin un conocimiento preciso de la posición espacial exacta de dichos bordes.5

Este informe técnico desglosa exhaustivamente la teoría, la matemática, la implementación algorítmica y las aplicaciones modernas de HOG. Se analiza no solo cómo funciona, sino por qué funciona, examinando las decisiones de diseño críticas —desde la elección de núcleos de derivadas simples hasta la interpolación trilineal— y contrastando su eficacia con las arquitecturas de redes neuronales convolucionales (CNN) que dominan el paisaje actual.6

## 2. Fundamentos Matemáticos: La Física del Gradiente de Imagen

Para comprender la robustez de HOG, es imperativo diseccionar su unidad fundamental: el gradiente de imagen. En el procesamiento digital, una imagen se modela como una función discreta de intensidad $I(x, y)$. La información semántica más rica, aquella que define la forma y la estructura, reside no en los valores absolutos de los píxeles (que son vulnerables a cambios de iluminación global), sino en las transiciones entre ellos.

### 2.1 Cálculo de Derivadas y Selección del Núcleo

El primer paso crítico en el algoritmo HOG es la computación del gradiente, que representa la magnitud y la dirección del cambio de intensidad. Matemáticamente, buscamos aproximar las derivadas parciales de la imagen con respecto a las coordenadas espaciales $x$ e $y$.

Dalal y Triggs realizaron un estudio empírico exhaustivo sobre la eficacia de diferentes máscaras de convolución (kernels) para esta tarea. Evaluaron máscaras de suavizado gaussiano, operadores de Sobel (3x3), y máscaras diagonales. Sus hallazgos fueron contraintuitivos para la doctrina establecida de la época: la máscara más simple posible, el operador de derivada centrada 1-D [-1, 0, 1], ofreció el mejor rendimiento consistentemente para la detección humana.1

La operación de convolución para obtener los gradientes en $x$ ($G_x$) y en $y$ ($G_y$) se define como:

$$G_x(x,y) = I(x+1, y) - I(x-1, y)$$

$$G_y(x,y) = I(x, y+1) - I(x, y-1)$$

Esta preferencia por la ausencia de suavizado (a diferencia del operador Sobel que incorpora un ligero suavizado en la dirección perpendicular a la derivada) sugiere que, para la detección de peatones a las escalas típicas (ventanas de 64x128), la información de los bordes finos y los detalles de alto contraste es crucial. El suavizado previo tiende a eliminar información vital sobre la textura fina y los contornos de las extremidades, reduciendo la capacidad discriminativa del descriptor final.2

### 2.2 Transformación a Coordenadas Polares: Magnitud y Fase

Una vez obtenidos los componentes cartesianos del gradiente, el algoritmo requiere una transformación a coordenadas polares para aislar la "fuerza" del borde (magnitud) de su "dirección" (orientación).

La magnitud del gradiente ($|G|$), que servirá como el peso del voto en el histograma, se calcula mediante la norma euclidiana:

$$|G(x,y)| = \sqrt{G_x(x,y)^2 + G_y(x,y)^2}$$

La orientación ($\theta$), que determinará en qué contenedor (bin) del histograma se deposita el voto, se obtiene mediante la función arcotangente:

$$\theta(x,y) = \arctan\left(\frac{G_y(x,y)}{G_x(x,y)}\right)$$

Es fundamental notar que la orientación del gradiente es siempre perpendicular a la dirección visual del borde. Un borde vertical (como el costado de un edificio) implica un cambio de intensidad horizontal, resultando en un gradiente de $0^\circ$ o $180^\circ$. Inversamente, un borde horizontal (como el horizonte) genera un gradiente vertical de $90^\circ$.1 Esta ortogonalidad es una fuente común de confusión al interpretar las visualizaciones de HOG.

### 2.3 La Importancia del "Signo" en el Gradiente

Una decisión de diseño crucial en HOG es el rango angular. ¿Debemos considerar el rango completo de $0^\circ$ a $360^\circ$ (gradientes con signo) o limitarlo a $0^\circ$ a $180^\circ$ (gradientes sin signo)?

- Gradientes con Signo ($0-360^\circ$): Distinguen la polaridad de la transición. Un borde que pasa de negro a blanco es distinto de uno que pasa de blanco a negro.
- Gradientes sin Signo ($0-180^\circ$): Tratan ambas transiciones como idénticas. Matemáticamente, esto se logra tomando el ángulo módulo $180^\circ$ o asumiendo simetría.

La investigación de Dalal y Triggs demostró que para la detección de humanos, los gradientes sin signo superan significativamente a los con signo.1 La razón subyacente es la variabilidad en la vestimenta y el fondo. Una persona puede vestir una camisa oscura sobre un fondo claro o una camisa clara sobre un fondo oscuro; la estructura del contorno es la misma, pero la dirección del gradiente se invierte. Al usar gradientes sin signo, el descriptor se vuelve invariante a esta inversión de contraste. Sin embargo, para objetos como automóviles o señales de tráfico, donde los patrones de color son más predecibles (ej. luces traseras rojas, faros blancos), la información de signo puede mejorar el rendimiento.1

## 3. Arquitectura Algorítmica: De Píxeles a Descriptores

El proceso de construcción del descriptor HOG es una cadena secuencial de transformaciones diseñada para convertir la información densa de píxeles en una representación compacta y robusta. A continuación, se detalla cada etapa del flujo de procesamiento estándar.

### 3.1 Preprocesamiento: Normalización Gamma y Espacio de Color

Históricamente, se asumía que la normalización previa de la imagen (corrección gamma, $I_{out} = I_{in}^\gamma$, o conversión a escala de grises) era esencial. Sin embargo, los experimentos mostraron que, en el contexto de HOG, el preprocesamiento tiene un impacto mínimo en el rendimiento final.1 Esto se debe a que la normalización de bloques posterior (etapa 3.5) es tan agresiva y eficaz que compensa las variaciones de iluminación global, haciendo redundante la corrección gamma previa. No obstante, para imágenes en color, se suelen calcular los gradientes en los tres canales (R, G, B) y seleccionar, para cada píxel, el gradiente con la mayor norma como representante, asegurando que se capture el borde más prominente independientemente del canal de color donde ocurra.8

### 3.2 Cuantización de la Orientación: Binning

El núcleo del algoritmo es la creación de histogramas en regiones locales. Para ello, el espacio continuo de orientaciones ($0-180^\circ$) se discretiza en contenedores o bins.

- Número de Bins ($B$): La configuración óptima encontrada es de 9 bins, lo que resulta en un ancho de bin de $20^\circ$ (0°, 20°, 40°,..., 160°).1
- Impacto de la Granularidad: Aumentar el número de bins permite capturar orientaciones más finas (por ejemplo, distinguir una línea a 10° de una a 15°), pero aumenta el tamaño del descriptor y el riesgo de aliasing (ruido por discretización). Reducir los bins simplifica demasiado la forma. El valor de 9 representa un equilibrio empírico entre discriminabilidad y compacidad.

### 3.3 Agrupación Espacial: Celdas (Cells)

La imagen se divide en pequeñas regiones espaciales conectadas llamadas "celdas" (cells). El tamaño estándar es de 8x8 píxeles.11

- Razón de ser: La celda es la unidad de acumulación de votos. Al agregar los gradientes de 64 píxeles en un solo histograma, logramos una invarianza local a la posición. Si un brazo o una pierna se mueve unos pocos píxeles, la distribución estadística de orientaciones dentro de la celda de 8x8 apenas cambia, aunque los píxeles individuales sí lo hagan. Esto dota al descriptor de robustez ante pequeñas deformaciones de postura.

### 3.4 Mecanismo de Votación e Interpolación Trilineal

Aquí reside una de las sutilezas matemáticas más importantes de HOG. Si un píxel tiene un ángulo de $19.9^\circ$, asignarlo completamente al bin de $0-20^\circ$ (cuyo centro es $10^\circ$) sería un error de cuantización que introduciría ruido ante cambios mínimos en la imagen. Para evitar esto, HOG emplea un sistema de votación ponderada con interpolación trilineal.13

La interpolación ocurre en tres dimensiones: $x$ (posición horizontal), $y$ (posición vertical) y $\theta$ (orientación).

1. Interpolación Angular: Un gradiente con orientación $\theta$ contribuye a los dos bins más cercanos. Si $\theta = 15^\circ$, y los centros de los bins son $10^\circ$ y $30^\circ$, el voto se reparte. Específicamente, el bin de $10^\circ$ recibe el 75% del peso (ya que 15 está más cerca de 10) y el bin de $30^\circ$ recibe el 25%.
2. Interpolación Espacial: Un píxel no vota solo en la celda en la que se encuentra geométricamente. Su contribución se distribuye bilinealmente entre las celdas vecinas basándose en su distancia a los centros de las celdas.15

La fórmula de interpolación para un píxel en $(x, y, \theta)$ que contribuye a un bin centrado en $(x_c, y_c, \theta_c)$ implica pesos del tipo:

$$w = |G| \cdot (1 - \frac{|x - x_c|}{w_x}) \cdot (1 - \frac{|y - y_c|}{w_y}) \cdot (1 - \frac{|\theta - \theta_c|}{w_\theta})$$

Este mecanismo asegura que el descriptor varíe suavemente cuando el objeto se traslada o rota ligeramente, evitando saltos abruptos en el vector de características que confundirían al clasificador.17

### 3.5 Bloques y Normalización de Contraste

Los gradientes locales son proporcionales a la iluminación. Si la iluminación de la escena se duplica, los valores de los gradientes se duplican, cambiando la magnitud del vector descriptor. Para corregir esto, se agrupan las celdas en bloques (blocks) más grandes, típicamente de 16x16 píxeles ($2 \times 2$ celdas).1

La normalización se realiza a nivel de bloque. Sea $v$ el vector concatenado de todos los histogramas en un bloque (para $2 \times 2$ celdas y 9 bins, $v$ tiene dimensión 36). Existen varios esquemas de normalización 8:

- L2-Norm: $v \to \frac{v}{\sqrt{\|v\|_2^2 + \epsilon^2}}$
- L2-Hys (Histéresis): Es el estándar de facto. Primero se aplica L2-Norm. Luego, se limitan (clipping) los valores máximos a un umbral (usualmente 0.2) para suprimir picos extremos causados por brillos especulares. Finalmente, se vuelve a normalizar con L2-Norm.
- L1-Sqrt: $v \to \sqrt{\frac{v}{\|v\|_1 + \epsilon}}$. Ofrece un rendimiento sorprendentemente bueno, casi igualando a L2-Hys, debido a que la raíz cuadrada comprime el rango dinámico de manera similar a la corrección gamma.

Los bloques se superponen (overlap). Con un paso (stride) de 8 píxeles (1 celda), cada celda contribuye a la normalización de 4 bloques diferentes. Esta redundancia es vital: permite que cada componente del histograma sea evaluado en contexto con diferentes vecindarios, mejorando significativamente la detección.2

## 4. Análisis Dimensional del Vector de Características

Para dimensionar adecuadamente los sistemas de aprendizaje automático que consumirán los datos de HOG, es esencial entender cómo se calcula la longitud final del vector. Tomemos como caso de estudio la configuración clásica para detección de peatones en una ventana de 64x128 píxeles.10

Los parámetros típicos son:

- Tamaño de Ventana: 64 (ancho) x 128 (alto).
- Tamaño de Celda: 8x8.
- Tamaño de Bloque: 16x16 ($2 \times 2$ celdas).
- Paso de Bloque (Stride): 8 píxeles (1 celda).
- Bins: 9.

Cálculo paso a paso:

- Geometría de Celdas:
  - Horizontal: $64 / 8 = 8$ celdas.
  - Vertical: $128 / 8 = 16$ celdas.
- Geometría de Bloques (con superposición):
  - Dado que el bloque abarca 2 celdas y avanza de 1 en 1, el número de bloques horizontales es $8 - 2 + 1 = 7$.
  - El número de bloques verticales es $16 - 2 + 1 = 15$.
  - Total de bloques por ventana: $7 \times 15 = 105$ bloques.
- Dimensión por Bloque:
  - Cada bloque contiene 4 celdas.
  - Cada celda contiene 9 valores de histograma.
  - Valores por bloque: $4 \times 9 = 36$.
- Vector Total:
  - Dimensiones finales: $105 \text{ bloques} \times 36 \text{ valores/bloque} = \mathbf{3780}$ características.
  
Esta longitud (3780) es fija para una ventana de 64x128. Si se utiliza una imagen más grande, se aplica una técnica de "ventana deslizante" (sliding window), donde esta ventana de 64x128 recorre toda la imagen en múltiples escalas para detectar personas de diferentes tamaños.22

## 5. Variantes Geométricas: R-HOG vs. C-HOG

Aunque la variante rectangular (R-HOG) descrita anteriormente es la más ubicua, Dalal y Triggs también formalizaron una variante circular (C-HOG).1

### 5.1 Rectangular HOG (R-HOG)

Es la forma estándar basada en cuadrículas cartesianas. Sus parámetros son celdas cuadradas y bloques rectangulares. Es ideal para la mayoría de las aplicaciones de visión general debido a su simplicidad de implementación y mapeo directo a la memoria de la imagen.

### 5.2 Circular HOG (C-HOG)

Esta variante utiliza una disposición log-polar de celdas, similar al descriptor Shape Context.

- Estructura: Consiste en un bloque central circular dividido en celdas definidas por radios y sectores angulares.
- Parámetros: Número de bins angulares, número de bins radiales, radio central y factor de expansión.
- Rendimiento: Aunque teóricamente más elegante para objetos con simetría radial, Dalal y Triggs encontraron que C-HOG y R-HOG ofrecen un rendimiento casi idéntico para la detección de peatones. Sin embargo, C-HOG es computacionalmente más complejo de implementar debido a la necesidad de cálculos polares, por lo que R-HOG se convirtió en el estándar industrial.1

## 6. Interpretación Visual: El "Rose Plot"

La visualización de un descriptor HOG es una herramienta de diagnóstico poderosa. Se representa comúnmente superponiendo "estrellas" o "rosas" sobre la cuadrícula de la imagen original. Cada celda de 8x8 muestra una estrella con 9 brazos (uno por bin).25

| Característica Visual | Interpretación Técnica |
| ---------------------- | ------------------------ |
| Longitud del brazo | Proporcional a la suma de magnitudes de gradiente en esa orientación. Un brazo largo indica un borde fuerte y predominante. |
| Dirección del brazo | Indica la dirección del gradiente. **Nota Crucial**: El gradiente es ortogonal al borde. Una línea vertical en el gráfico HOG representa un borde horizontal en la imagen. |
| Ausencia de estrellas | Indica una región plana o uniforme (textura suave) donde los gradientes son insignificantes (ej. cielo despejado, asfalto liso). |

Al observar un HOG de un peatón, se pueden identificar patrones distintivos: la silueta de la cabeza y los hombros forma un arco de gradientes fuertes, y las piernas generan patrones verticales (bordes verticales de los pantalones) que se traducen en brazos horizontales en la visualización del gradiente.10

## 7. Implementación Práctica y Parámetros

En el ecosistema de desarrollo actual, OpenCV (cv2) y Scikit-Image (skimage) son las bibliotecas de referencia. Sus implementaciones difieren en flexibilidad y velocidad.

### 7.1 OpenCV HOGDescriptor

Optimizada para velocidad y uso en tiempo real (C++/Python). Es la elección para despliegue en producción.24

- Parámetros Clave:
  - winSize: Tamaño de la ventana de detección (ej. 64x128). Crítico: debe coincidir con la relación de aspecto del objeto físico.
  - blockSize, blockStride, cellSize: Definen la granularidad. Reducir el cellSize a 4x4 aumenta drásticamente el tamaño del vector y el tiempo de cómputo, a veces capturando detalles innecesarios (ruido).
  - nbins: 9 es el estándar. Valores como 18 o 36 raramente mejoran la precisión en peatones.
  
### 7.2 Scikit-Image hog

Diseñada para análisis científico y flexibilidad. Permite explorar diferentes normas (L1, L1-sqrt) y visualización directa.29

```python
from skimage.feature import hog
# Ejemplo de configuración para investigación
fd, hog_image = hog(image, orientations=9, pixels_per_cell=(8, 8),
                    cells_per_block=(2, 2), visualize=True, multichannel=True,
                    block_norm='L2-Hys')
```

La opción visualize=True retorna la imagen del descriptor lista para renderizar, algo que OpenCV no ofrece nativamente de forma simple.9

## 8. Análisis Comparativo con Descriptores Clásicos

Para contextualizar HOG, debemos medirlo frente a sus contemporáneos y predecesores.

### 8.1 HOG vs. SIFT (Scale-Invariant Feature Transform)

- Mecanismo: Ambos usan histogramas de gradientes. SIFT los calcula alrededor de puntos clave detectados (keypoints) y alinea la orientación principal para lograr invarianza rotacional.
- Diferencia: HOG es denso (toda la imagen) y rígido (cuadrícula fija). SIFT es disperso (solo puntos interesantes) y flexible.
- Veredicto: SIFT es superior para matching (unir fotos panorámicas), pero HOG es superior para detección de categorías de objetos (como humanos) donde la estructura global es consistente y la rotación es limitada.1

### 8.2 HOG vs. LBP (Local Binary Patterns)

- Mecanismo: LBP codifica textura comparando píxeles vecinos (mayor/menor). Es extremadamente rápido y robusto a cambios monótonos de iluminación.
- Sinergia: HOG captura forma (bordes); LBP captura textura (patrones micro). Estudios demuestran que combinarlos (HOG+LBP) aumenta la precisión en detección facial y de peatones, ya que sus errores no están correlacionados.33

#### Tabla de Rendimiento Relativo

| Descriptor | Precisión (Peatones) | Velocidad | Invarianza Rotación | Invarianza Iluminación |
| ------------ | ---------------------- | ----------- | --------------------- | ------------------------ |
| HOG | Alta | Media | Baja | Alta (L2-Hys) |
| SIFT | Media | Baja | Alta | Alta |
| LBP | Media/Baja | Muy Alta | Media | Alta |
| Haar | Baja (muchos FP) | Muy Alta | Baja | Baja |

## 9. HOG en la Era del Deep Learning (2020-2025)

Es un error común considerar a HOG obsoleto frente a las Redes Neuronales Convolucionales (CNN). En 2025, HOG ha encontrado un nuevo rol en arquitecturas híbridas y sistemas de recursos limitados.

### 9.1 Modelos Híbridos HOG-CNN

En dominios como la imagen médica, la disponibilidad de datos etiquetados es escasa. Entrenar una CNN profunda desde cero ("training from scratch") con pocos datos lleva al sobreajuste.

- Estrategia HOG-CNN: Investigaciones recientes (2024-2025) proponen alimentar la red neuronal no solo con píxeles, sino también con características HOG pre-calculadas. HOG actúa como un "prior" de conocimiento estructural, estabilizando el entrenamiento.
- Caso de Estudio - Retinopatía: El modelo HOG-CNN de Faisal Ahmed (2025) para clasificación de imágenes de retina (enfermedades como retinopatía diabética y glaucoma) integra características HOG con representaciones profundas. Los resultados mostraron una precisión del 98.5% en el dataset APTOS 2019, superando a las CNN puras en escenarios de datos limitados debido a la capacidad de HOG para resaltar texturas vasculares y bordes de lesiones.6

### 9.2 Biometría y Seguridad

En sistemas de verificación de firmas y reconocimiento facial, HOG sigue siendo competitivo. Un estudio comparativo de 2025 sobre reconocimiento de firmas demostró que las características basadas en HOG alcanzaron una precisión del 94.6%, superando a SIFT (90.3%) y LBP (92.5%) debido a su capacidad superior para modelar la geometría de los trazos manuscritos.34

## 10. Hardware y Eficiencia Energética: La Brecha HOG-CNN

La ventaja más duradera de HOG es su eficiencia energética. Un estudio fundamental del MIT analizó la "brecha de energía" entre HOG y CNN para visión embebida.35

- Consumo: Ejecutar un detector HOG en hardware dedicado consume órdenes de magnitud menos energía que una CNN moderna (como YOLO o Faster R-CNN) que requiere acceso masivo a memoria y multiplicaciones de matrices flotantes.
- Aplicaciones de Borde (Edge AI): Para micro-drones, sensores IoT remotos o wearables que funcionan con baterías de botón, las CNN completas son a menudo inviables. HOG ofrece un compromiso "bueno-bonito-barato": suficiente precisión para tareas como evitación de obstáculos o despertar del sistema (wake-up word visual), con un coste energético mínimo.37

## 11. Conclusión

El Histograma de Gradientes Orientados no es simplemente un algoritmo del pasado; es un testimonio de la elegancia en la ingeniería de características. Su diseño, basado en la comprensión profunda de que la forma visual puede abstraerse mediante distribuciones estadísticas de gradientes locales, resolvió problemas de robustez que los métodos basados en intensidad simple no podían abordar.

Aunque el aprendizaje profundo ha superado a HOG en precisión bruta en benchmarks masivos con hardware ilimitado, HOG mantiene su supremacía en tres frentes críticos en 2025: eficiencia computacional/energética, rendimiento en escenarios de datos escasos (small data) mediante modelos híbridos, y explicabilidad visual. Mientras las CNN funcionan a menudo como cajas negras, un descriptor HOG es totalmente transparente: si el sistema detecta un borde, podemos ver exactamente qué celda y qué bin de orientación se activó. En un mundo que avanza hacia la IA explicable y la computación en el borde, HOG sigue siendo una herramienta indispensable en el arsenal del ingeniero de visión por computador.
