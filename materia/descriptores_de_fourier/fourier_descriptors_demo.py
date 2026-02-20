"""
Demostración de Descriptores de Fourier
Muestra la invariancia a rotación, escalado y traslación mediante normalización
"""

import cv2
import numpy as np
import matplotlib
matplotlib.use('Agg')  # Usar backend sin GUI
import matplotlib.pyplot as plt
from scipy.fftpack import fft


def remuestrear_contorno(contorno, num_puntos):
    """
    Remuestrear contorno a un número fijo de puntos equidistantes
    """
    contorno = contorno.squeeze()
    if len(contorno.shape) == 1:
        contorno = contorno.reshape(-1, 2)

    # Calcular longitud acumulada del contorno
    diffs = np.diff(contorno, axis=0, prepend=contorno[-1:])
    distancias = np.sqrt(np.sum(diffs**2, axis=1))
    longitud_acumulada = np.cumsum(distancias)
    longitud_total = longitud_acumulada[-1]

    # Normalizar a [0, 1]
    longitud_acumulada = longitud_acumulada / longitud_total

    # Crear puntos de muestreo equidistantes
    puntos_muestreo = np.linspace(0, 1, num_puntos, endpoint=False)

    # Interpolar para obtener nuevos puntos
    x_nuevo = np.interp(puntos_muestreo, longitud_acumulada, contorno[:, 0])
    y_nuevo = np.interp(puntos_muestreo, longitud_acumulada, contorno[:, 1])

    contorno_nuevo = np.column_stack([x_nuevo, y_nuevo])
    return contorno_nuevo.reshape(-1, 1, 2).astype(np.int32)


def preprocesar_imagen(imagen_path):
    """
    Preprocesamiento: Color → Grises → Binaria → Contorno
    """
    # Leer imagen
    img = cv2.imread(imagen_path)

    # Convertir a grises
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # Binarizar (invertir para que el objeto sea blanco)
    _, binary = cv2.threshold(gray, 127, 255, cv2.THRESH_BINARY_INV)

    # Encontrar contornos
    contours, _ = cv2.findContours(binary, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

    # Tomar el contorno más grande
    contorno = max(contours, key=cv2.contourArea)

    # Remuestrear el contorno a un número fijo de puntos para consistencia
    # Esto ayuda a que la FFT tenga suficientes puntos y sea más estable
    num_puntos = 256
    contorno = remuestrear_contorno(contorno, num_puntos)

    return contorno, img, binary


def contorno_a_complejo(contorno):
    """
    Convertir contorno (x, y) a representación compleja z = x + iy
    """
    contorno = contorno.squeeze()  # Remover dimensiones extra
    z = contorno[:, 0] + 1j * contorno[:, 1]  # z = x + iy
    return z


def calcular_descriptores_fourier(z, num_descriptores=50):
    """
    Calcular descriptores de Fourier mediante FFT
    """
    # Aplicar FFT
    Z = fft(z)

    # Tomar solo los primeros descriptores
    return Z[:num_descriptores]


def normalizar_descriptores(Z):
    """
    Normalización para invariancia:
    - Traslación: ya es invariante (DC component no se usa)
    - Escalado: dividir por |Z(1)|
    - Rotación: tomar valor absoluto
    - Punto de inicio: dividir por Z(1)
    """
    if len(Z) < 2:
        return Z

    # Verificar que Z(1) no sea cero
    if np.abs(Z[1]) < 1e-10:
        print("  ⚠️  Advertencia: Z(1) es casi cero, usando otro coeficiente para normalización")
        # Buscar el primer coeficiente no cero
        for i in range(1, min(10, len(Z))):
            if np.abs(Z[i]) > 1e-10:
                Z_norm = Z / np.abs(Z[i])
                break
        else:
            return np.abs(Z)
    else:
        # Normalización por escala (dividir por magnitud del primer armónico)
        Z_norm = Z / np.abs(Z[1])

    # Invariancia a rotación y punto de inicio (tomar magnitud)
    Z_invariante = np.abs(Z_norm)

    return Z_invariante


def generar_variaciones_contorno(z_complejo):
    """
    Generar 3 versiones transformadas del contorno directamente
    Operaciones en números complejos son más precisas
    """
    # Calcular el centroide
    centroide = np.mean(z_complejo)

    # 1. ROTACIÓN (45 grados)
    theta = np.deg2rad(45)
    z_rotado = centroide + (z_complejo - centroide) * np.exp(1j * theta)

    # 2. ESCALADO (1.5x)
    z_escalado = centroide + (z_complejo - centroide) * 1.5

    # 3. TRASLACIÓN (50 + 50i pixels)
    z_trasladado = z_complejo + (50 + 50j)

    return {
        'rotado': z_rotado,
        'escalado': z_escalado,
        'trasladado': z_trasladado
    }


def generar_variaciones_imagen(img, contorno):
    """
    Generar 3 versiones transformadas de la imagen para visualización
    """
    h, w = img.shape[:2]

    # 1. ROTACIÓN (45 grados)
    M_rot = cv2.getRotationMatrix2D((w/2, h/2), 45, 1.0)
    img_rotada = cv2.warpAffine(img, M_rot, (w, h),
                                 borderMode=cv2.BORDER_CONSTANT,
                                 borderValue=(255, 255, 255))

    # 2. ESCALADO (1.5x)
    img_escalada = cv2.resize(img, None, fx=1.5, fy=1.5,
                              interpolation=cv2.INTER_LINEAR)
    # Recortar al tamaño original
    h_s, w_s = img_escalada.shape[:2]
    start_h, start_w = (h_s - h) // 2, (w_s - w) // 2
    img_escalada = img_escalada[start_h:start_h+h, start_w:start_w+w]

    # 3. TRASLACIÓN (50 pixels en x e y)
    M_tras = np.float32([[1, 0, 50], [0, 1, 50]])
    img_trasladada = cv2.warpAffine(img, M_tras, (w, h),
                                     borderMode=cv2.BORDER_CONSTANT,
                                     borderValue=(255, 255, 255))

    return {
        'rotada': img_rotada,
        'escalada': img_escalada,
        'trasladada': img_trasladada
    }


def extraer_contorno_de_imagen(img):
    """
    Extraer contorno de una imagen ya procesada
    """
    if len(img.shape) == 3:
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    else:
        gray = img

    _, binary = cv2.threshold(gray, 127, 255, cv2.THRESH_BINARY_INV)
    contours, _ = cv2.findContours(binary, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

    if len(contours) == 0:
        return None

    contorno = max(contours, key=cv2.contourArea)

    # Remuestrear el contorno
    num_puntos = 256
    contorno = remuestrear_contorno(contorno, num_puntos)

    return contorno


def comparar_descriptores(desc1, desc2, nombre1="Desc1", nombre2="Desc2"):
    """
    Comparar dos conjuntos de descriptores
    """
    # Calcular diferencia
    diff = np.abs(desc1 - desc2)
    error_medio = np.mean(diff)
    error_max = np.max(diff)

    print(f"\n--- Comparación {nombre1} vs {nombre2} ---")
    print(f"Error medio: {error_medio:.6f}")
    print(f"Error máximo: {error_max:.6f}")
    print(f"Primeros 5 valores {nombre1}: {desc1[:5]}")
    print(f"Primeros 5 valores {nombre2}: {desc2[:5]}")

    return error_medio


def visualizar_resultados(imagenes_dict, descriptores_dict, descriptores_norm_dict, contornos_z=None):
    """
    Visualizar imágenes y gráficos de descriptores
    """
    n_imgs = len(imagenes_dict)

    plt.figure(figsize=(20, 12))

    # Plotear contornos (no imágenes)
    nombres = list(imagenes_dict.keys())

    for i, nombre in enumerate(nombres):
        plt.subplot(3, n_imgs, i + 1)

        if contornos_z is not None and nombre in contornos_z:
            z = contornos_z[nombre]
            plt.plot(z.real, z.imag, 'b-', linewidth=2)
            plt.plot(z.real[0], z.imag[0], 'ro', markersize=8, label='Inicio')
            plt.title(f'Contorno {nombre}')
            plt.axis('equal')
            plt.grid(True, alpha=0.3)
            if i == 0:
                plt.legend()
        else:
            # Mostrar imagen si no hay contorno
            img = imagenes_dict[nombre]
            if img is not None:
                if len(img.shape) == 3:
                    plt.imshow(cv2.cvtColor(img, cv2.COLOR_BGR2RGB))
                else:
                    plt.imshow(img, cmap='gray')
            plt.title(f'Imagen {nombre}')
            plt.axis('off')

    # Plotear descriptores SIN normalizar (magnitud)
    plt.subplot(3, 1, 2)
    for nombre, desc in descriptores_dict.items():
        plt.plot(np.abs(desc), label=nombre, marker='o', markersize=3)
    plt.title('Descriptores de Fourier SIN Normalizar (Magnitud) - DIFERENTES')
    plt.xlabel('Índice del descriptor')
    plt.ylabel('Magnitud')
    plt.legend()
    plt.grid(True)

    # Plotear descriptores CON normalización
    plt.subplot(3, 1, 3)
    for nombre, desc in descriptores_norm_dict.items():
        plt.plot(desc, label=nombre, marker='o', markersize=3)
    plt.title('Descriptores de Fourier Normalizados - CASI IDÉNTICOS')
    plt.xlabel('Índice del descriptor')
    plt.ylabel('Magnitud normalizada')
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.savefig('fourier_descriptors_demo.png', dpi=150, bbox_inches='tight')
    print("\n✓ Gráfico guardado como 'fourier_descriptors_demo.png'")
    plt.close()


def demo_completa(imagen_path):
    """
    Demostración completa del proceso
    """
    print("="*70)
    print("DEMOSTRACIÓN DE DESCRIPTORES DE FOURIER")
    print("="*70)

    # 1. PREPROCESAMIENTO
    print("\n[1] Preprocesamiento: Color → Grises → Binaria → Contorno")
    contorno_original, img_original, binary_original = preprocesar_imagen(imagen_path)
    z_original = contorno_a_complejo(contorno_original)
    print(f"✓ Contorno extraído: {len(z_original)} puntos")

    # 2. GENERAR VARIACIONES
    print("\n[2] Generar variaciones transformando el contorno directamente")
    z_variaciones = generar_variaciones_contorno(z_original)
    print(f"✓ Generadas {len(z_variaciones)} variaciones: {list(z_variaciones.keys())}")

    for nombre, z in z_variaciones.items():
        print(f"  ✓ Contorno {nombre}: {len(z)} puntos")

    # Generar imágenes de las variaciones para visualización
    variaciones_img = generar_variaciones_imagen(img_original, contorno_original)

    # 3. CALCULAR DESCRIPTORES
    print("\n[3] Calcular FFT de todos los contornos")
    num_desc = 50

    Z_original = calcular_descriptores_fourier(z_original, num_desc)
    print(f"✓ Descriptores originales calculados: {len(Z_original)} coeficientes")

    Z_variaciones = {}
    for nombre, z in z_variaciones.items():
        Z_variaciones[nombre] = calcular_descriptores_fourier(z, num_desc)
        print(f"  ✓ Descriptores {nombre}: {len(Z_variaciones[nombre])} coeficientes")

    # 4. DEMOSTRACIÓN
    print("\n" + "="*70)
    print("[4] DEMOSTRACIÓN DE INVARIANCIA")
    print("="*70)

    # 4.1 Mostrar que valores crudos son DIFERENTES
    print("\n--- DESCRIPTORES SIN NORMALIZAR (DIFERENTES) ---")
    descriptores_crudos = {'original': Z_original}
    descriptores_crudos.update(Z_variaciones)

    for nombre in list(Z_variaciones.keys()):
        comparar_descriptores(
            np.abs(Z_original[1:6]),  # Magnitud de los primeros 5
            np.abs(Z_variaciones[nombre][1:6]),
            "Original",
            nombre.capitalize()
        )

    # 4.2 Aplicar NORMALIZACIÓN
    print("\n\n--- DESCRIPTORES CON NORMALIZACIÓN (CASI IDÉNTICOS) ---")
    Z_original_norm = normalizar_descriptores(Z_original)
    print(f"✓ Normalización aplicada")

    Z_variaciones_norm = {}
    for nombre, Z in Z_variaciones.items():
        Z_variaciones_norm[nombre] = normalizar_descriptores(Z)

    descriptores_normalizados = {'original': Z_original_norm}
    descriptores_normalizados.update(Z_variaciones_norm)

    # 4.3 Verificar que firmas son casi IDÉNTICAS
    print("\nComparando descriptores normalizados:")
    errores = []
    for nombre in list(Z_variaciones_norm.keys()):
        error = comparar_descriptores(
            Z_original_norm[1:6],
            Z_variaciones_norm[nombre][1:6],
            "Original normalizado",
            f"{nombre.capitalize()} normalizado"
        )
        errores.append(error)

    print("\n" + "="*70)
    print("CONCLUSIÓN")
    print("="*70)
    print(f"Error medio promedio después de normalización: {np.mean(errores):.6f}")
    print("✓ Los descriptores normalizados son INVARIANTES a:")
    print("  - Rotación")
    print("  - Escalado")
    print("  - Traslación")
    print("="*70)

    # 5. VISUALIZACIÓN
    print("\n[5] Generando visualización...")
    imagenes_todas = {'original': img_original}
    imagenes_todas.update(variaciones_img)

    # Preparar contornos para visualización
    contornos_todos = {'original': z_original}
    contornos_todos.update(z_variaciones)

    visualizar_resultados(imagenes_todas, descriptores_crudos, descriptores_normalizados, contornos_todos)


def crear_imagen_ejemplo():
    """
    Crear una imagen de ejemplo con una forma interesante (estrella)
    """
    img = np.ones((400, 400, 3), dtype=np.uint8) * 255

    # Crear una estrella de 5 puntas
    center = (200, 200)
    radius_outer = 100
    radius_inner = 40

    # Generar puntos de la estrella
    points = []
    for i in range(10):
        angle = i * np.pi / 5 - np.pi / 2
        if i % 2 == 0:
            # Punto exterior
            x = int(center[0] + radius_outer * np.cos(angle))
            y = int(center[1] + radius_outer * np.sin(angle))
        else:
            # Punto interior
            x = int(center[0] + radius_inner * np.cos(angle))
            y = int(center[1] + radius_inner * np.sin(angle))
        points.append([x, y])

    points = np.array(points, dtype=np.int32)

    # Dibujar la estrella
    cv2.fillPoly(img, [points], (0, 0, 0))

    cv2.imwrite('forma_ejemplo.png', img)
    print("✓ Imagen de ejemplo creada: 'forma_ejemplo.png' (estrella de 5 puntas)")
    return 'forma_ejemplo.png'


if __name__ == "__main__":
    # Si no tienes una imagen, crear una de ejemplo
    import os

    # Puedes cambiar esto por tu propia imagen
    imagen_path = 'forma_ejemplo.png'

    if not os.path.exists(imagen_path):
        print("No se encontró imagen, creando ejemplo...")
        imagen_path = crear_imagen_ejemplo()

    # Ejecutar demostración completa
    demo_completa(imagen_path)
