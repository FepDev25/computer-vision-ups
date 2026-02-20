"""
Ejemplo SIMPLE de Descriptores de Fourier
Versión educativa paso a paso
"""

import numpy as np
import matplotlib
matplotlib.use('Agg')  # Usar backend sin GUI
import matplotlib.pyplot as plt
from scipy.fftpack import fft


def crear_circulo_simple():
    """
    Crear un círculo parametrizado en forma compleja
    """
    t = np.linspace(0, 2*np.pi, 100, endpoint=False)
    radio = 50
    centro = (100, 100)

    # Círculo en forma paramétrica: z(t) = centro + radio * e^(it)
    x = centro[0] + radio * np.cos(t)
    y = centro[1] + radio * np.sin(t)

    # Representación compleja
    z = x + 1j * y

    return z, x, y


def transformar_contorno(z, tipo='rotar', angulo=45, escala=1.5, dx=50, dy=50):
    """
    Aplicar transformaciones al contorno complejo
    """
    z_transformado = z.copy()

    if tipo == 'rotar':
        # Rotación: multiplicar por e^(iθ)
        theta = np.deg2rad(angulo)
        centro = np.mean(z)
        z_transformado = centro + (z - centro) * np.exp(1j * theta)

    elif tipo == 'escalar':
        # Escalado: multiplicar por constante
        centro = np.mean(z)
        z_transformado = centro + (z - centro) * escala

    elif tipo == 'trasladar':
        # Traslación: sumar constante
        z_transformado = z + (dx + 1j * dy)

    return z_transformado


def ejemplo_paso_a_paso():
    """
    Demostración paso a paso con visualización clara
    """
    print("="*70)
    print("EJEMPLO SIMPLE: DESCRIPTORES DE FOURIER")
    print("="*70)

    # PASO 1: Crear forma simple
    print("\n[PASO 1] Crear un círculo simple")
    z_original, x_orig, y_orig = crear_circulo_simple()
    print(f"  Puntos del contorno: {len(z_original)}")
    print(f"  Primeros 3 puntos: {z_original[:3]}")

    # PASO 2: Calcular descriptores originales
    print("\n[PASO 2] Calcular descriptores de Fourier (FFT)")
    Z_original = fft(z_original)
    print(f"  Número de descriptores: {len(Z_original)}")
    print(f"  Z(0) [DC, centroide]: {Z_original[0]:.2f}")
    print(f"  Z(1) [fundamental]: {Z_original[1]:.2f}")
    print(f"  |Z(1)|: {np.abs(Z_original[1]):.2f}")

    # PASO 3: Aplicar transformaciones
    print("\n[PASO 3] Aplicar transformaciones")
    transformaciones = {
        'rotado': transformar_contorno(z_original, 'rotar', angulo=45),
        'escalado': transformar_contorno(z_original, 'escalar', escala=1.5),
        'trasladado': transformar_contorno(z_original, 'trasladar', dx=50, dy=30)
    }

    # PASO 4: Calcular descriptores de las transformaciones
    print("\n[PASO 4] Calcular descriptores de las transformaciones")
    Z_transformados = {}
    for nombre, z_trans in transformaciones.items():
        Z_trans = fft(z_trans)
        Z_transformados[nombre] = Z_trans
        print(f"\n  {nombre.upper()}:")
        print(f"    Z(0): {Z_trans[0]:.2f}")
        print(f"    Z(1): {Z_trans[1]:.2f}")
        print(f"    |Z(1)|: {np.abs(Z_trans[1]):.2f}")

    # PASO 5: Comparar SIN normalización
    print("\n[PASO 5] Comparar descriptores SIN normalización")
    print("\n  Magnitudes de Z(1) (segunda armónica):")
    print(f"    Original:   {np.abs(Z_original[1]):.4f}")
    for nombre, Z in Z_transformados.items():
        print(f"    {nombre.capitalize():12} {np.abs(Z[1]):.4f}")
    print("\n  ⚠️  LOS VALORES SON DIFERENTES!")

    # PASO 6: Normalizar
    print("\n[PASO 6] Aplicar NORMALIZACIÓN")
    print("  Fórmula: Z_norm(k) = |Z(k)| / |Z(1)|")

    def normalizar(Z):
        return np.abs(Z) / np.abs(Z[1])

    Z_orig_norm = normalizar(Z_original)
    Z_trans_norm = {nombre: normalizar(Z) for nombre, Z in Z_transformados.items()}

    print(f"\n  Original normalizado Z_norm(2): {Z_orig_norm[2]:.6f}")
    for nombre, Z_norm in Z_trans_norm.items():
        print(f"  {nombre.capitalize():12} Z_norm(2): {Z_norm[2]:.6f}")

    # PASO 7: Comparar errores
    print("\n[PASO 7] Calcular errores después de normalización")
    print("\n  Comparando los primeros 10 descriptores:")

    for nombre, Z_norm in Z_trans_norm.items():
        error = np.mean(np.abs(Z_orig_norm[2:12] - Z_norm[2:12]))
        print(f"    Original vs {nombre.capitalize():12} Error: {error:.8f}")

    print("\n  ✓ LOS VALORES SON CASI IDÉNTICOS!")

    # VISUALIZACIÓN
    print("\n[PASO 8] Generando visualización...")
    visualizar_ejemplo(z_original, transformaciones, Z_original,
                      Z_transformados, Z_orig_norm, Z_trans_norm)


def visualizar_ejemplo(z_orig, z_trans_dict, Z_orig, Z_trans_dict,
                       Z_orig_norm, Z_trans_norm):
    """
    Visualización clara del ejemplo
    """
    fig = plt.figure(figsize=(16, 12))

    # Fila 1: Contornos en el espacio
    nombres = ['Original', 'Rotado', 'Escalado', 'Trasladado']
    contornos = [z_orig] + [z_trans_dict[k] for k in ['rotado', 'escalado', 'trasladado']]

    for i, (nombre, z) in enumerate(zip(nombres, contornos)):
        plt.subplot(3, 4, i + 1)
        plt.plot(z.real, z.imag, 'b-', linewidth=2)
        plt.plot(z.real[0], z.imag[0], 'ro', markersize=8, label='Inicio')
        plt.title(f'{nombre}')
        plt.axis('equal')
        plt.grid(True, alpha=0.3)
        if i == 0:
            plt.legend()

    # Fila 2: Espectro de magnitud SIN normalizar
    plt.subplot(3, 1, 2)
    plt.plot(np.abs(Z_orig[:20]), 'o-', linewidth=2, label='Original', markersize=6)
    for nombre, Z in Z_trans_dict.items():
        plt.plot(np.abs(Z[:20]), 'o-', linewidth=2, label=nombre.capitalize(),
                markersize=4, alpha=0.7)
    plt.title('Descriptores SIN Normalizar - Magnitudes |Z(k)| - DIFERENTES', fontsize=12, fontweight='bold')
    plt.xlabel('Índice k del descriptor')
    plt.ylabel('Magnitud |Z(k)|')
    plt.legend()
    plt.grid(True, alpha=0.3)

    # Fila 3: Espectro CON normalización
    plt.subplot(3, 1, 3)
    plt.plot(Z_orig_norm[2:20], 'o-', linewidth=2, label='Original', markersize=6)
    for nombre, Z_norm in Z_trans_norm.items():
        plt.plot(Z_norm[2:20], 'o-', linewidth=2, label=nombre.capitalize(),
                markersize=4, alpha=0.7)
    plt.title('Descriptores CON Normalización - Z_norm(k) = |Z(k)|/|Z(1)| - CASI IDÉNTICOS',
             fontsize=12, fontweight='bold', color='green')
    plt.xlabel('Índice k del descriptor (empezando en k=2)')
    plt.ylabel('Magnitud normalizada')
    plt.legend()
    plt.grid(True, alpha=0.3)

    plt.tight_layout()
    plt.savefig('ejemplo_simple_fourier.png', dpi=150, bbox_inches='tight')
    print("  ✓ Gráfico guardado: 'ejemplo_simple_fourier.png'")
    plt.close()


def ejemplo_conceptual():
    """
    Explicación conceptual de las transformaciones
    """
    print("\n" + "="*70)
    print("CONCEPTOS CLAVE")
    print("="*70)

    print("""
1. REPRESENTACIÓN COMPLEJA:
   Un contorno 2D se representa como: z(t) = x(t) + i·y(t)

   Ventaja: Las transformaciones geométricas se vuelven operaciones algebraicas simples

2. TRANSFORMACIONES:
   ┌────────────────┬─────────────────────┬────────────────────┐
   │ Transformación │ Operación en z(t)   │ Efecto en Z(k)     │
   ├────────────────┼─────────────────────┼────────────────────┤
   │ Traslación     │ z(t) + c            │ Solo afecta Z(0)   │
   │ Rotación       │ z(t)·e^(iθ)         │ Z(k)·e^(iθ)        │
   │ Escalado       │ a·z(t)              │ a·Z(k)             │
   │ Punto inicial  │ z(t + t₀)           │ Z(k)·e^(i2πkt₀/N)  │
   └────────────────┴─────────────────────┴────────────────────┘

3. NORMALIZACIÓN:

   Para hacer los descriptores INVARIANTES:

   a) Ignorar Z(0) → Invariancia a traslación
   b) Dividir por |Z(1)| → Invariancia a escalado
   c) Tomar |Z(k)| → Invariancia a rotación y punto inicial

   Fórmula final: Z_inv(k) = |Z(k)| / |Z(1)|  para k ≥ 2

4. INTERPRETACIÓN:
   - Z(0): Centroide (posición promedio)
   - Z(1): Componente fundamental (tamaño y orientación)
   - Z(2), Z(3), ...: Detalles de la forma (baja a alta frecuencia)

5. APLICACIONES:
   - Reconocimiento de formas
   - Matching de contornos
   - Clasificación de objetos
   - Análisis de formas médicas
""")


if __name__ == "__main__":
    # Ejecutar ejemplo paso a paso
    ejemplo_paso_a_paso()

    # Mostrar conceptos
    ejemplo_conceptual()

    print("\n" + "="*70)
    print("✓ EJEMPLO COMPLETADO")
    print("="*70)
    print("\nArchivos generados:")
    print("  - ejemplo_simple_fourier.png")
    print("\nPara más detalles, ver: fourier_descriptors_demo.py")
