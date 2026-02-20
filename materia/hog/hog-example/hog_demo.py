"""
Pipeline completo de Visión por Computador: HOG + SVM.
Uso: Organiza tus datos en carpetas 'positivos' y 'negativos'.
"""

import sys
import cv2
import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path
from skimage.feature import hog
from sklearn.svm import LinearSVC
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, classification_report
from sklearn.preprocessing import LabelEncoder
import joblib  # Para guardar el modelo (opcional)

# --- CONFIGURACIÓN ---
# Es CRÍTICO que todas las imágenes tengan el mismo tamaño para HOG
IMG_SIZE = (64, 128)  # (ancho, alto) - Estándar para detección de personas
HOG_ORIENTATIONS = 9
HOG_PIXELS_PER_CELL = (8, 8)
HOG_CELLS_PER_BLOCK = (2, 2)

def load_and_prep_image(path: str) -> np.ndarray:
    """Carga, convierte a gris y redimensiona la imagen."""
    img = cv2.imread(str(path), cv2.IMREAD_GRAYSCALE)
    if img is None:
        return None
    # Redimensionar es obligatorio: el vector HOG debe tener siempre la misma longitud
    img = cv2.resize(img, IMG_SIZE)
    return img

def extract_hog_features(image: np.ndarray, visualize: bool = False):
    """
    Calcula los descriptores HOG.
    Si visualize=False, solo devuelve el array de features (más rápido para entrenar).
    """
    result = hog(
        image,
        orientations=HOG_ORIENTATIONS,
        pixels_per_cell=HOG_PIXELS_PER_CELL,
        cells_per_block=HOG_CELLS_PER_BLOCK,
        block_norm='L2-Hys', # Normalización recomendada
        visualize=visualize,
        transform_sqrt=True  # Reduce efectos de sombras/iluminación
    )
    return result

def load_dataset(dataset_path: str):
    """
    Carga imágenes de carpetas.
    Estructura esperada:
      dataset/
        clase_A/ (ej. peatones)
        clase_B/ (ej. fondo/no-peatones)
    """
    base_path = Path(dataset_path)
    if not base_path.exists():
        print(f"Error: La ruta {dataset_path} no existe.")
        sys.exit(1)

    features = []
    labels = []
    
    # Recorremos cada subcarpeta como una clase distinta
    for category_path in base_path.iterdir():
        if category_path.is_dir():
            label = category_path.name
            print(f"Procesando clase: {label}...")
            
            for img_path in category_path.glob("*.jpg"): # O *.png
                img = load_and_prep_image(img_path)
                if img is not None:
                    # Extraer solo el vector de características (no la imagen visual)
                    fd = extract_hog_features(img, visualize=False)
                    features.append(fd)
                    labels.append(label)
    
    return np.array(features), np.array(labels)

def main():
    # 1. Cargar Datos
    # Asume que tienes una carpeta llamada 'dataset' con subcarpetas
    print("--- 1. Cargando Dataset y Extrayendo HOG ---")
    # Para probar esto, crea una carpeta 'dataset' con dos subcarpetas dentro
    dataset_dir = "dataset" 
    
    # Verificación simple para evitar error si no existe la carpeta
    if not Path(dataset_dir).exists():
        print(f"Crea una carpeta llamada '{dataset_dir}' con subcarpetas de imágenes para probar.")
        return

    X, y = load_dataset(dataset_dir)
    print(f"Total imágenes procesadas: {len(X)}")
    print(f"Longitud del vector de características: {X.shape[1]}")

    # 2. Preprocesamiento de Etiquetas
    le = LabelEncoder()
    y_encoded = le.fit_transform(y) # Convierte etiquetas texto a 0, 1, 2...
    print(f"Clases detectadas: {le.classes_}")

    # 3. Dividir en Entrenamiento (80%) y Prueba (20%)
    print("\n--- 2. Dividiendo datos (Train/Test) ---")
    X_train, X_test, y_train, y_test = train_test_split(
        X, y_encoded, test_size=0.2, random_state=42, stratify=y_encoded
    )

    # 4. Entrenar el SVM
    print("\n--- 3. Entrenando SVM (LinearSVC) ---")
    # LinearSVC es generalmente mejor para HOG que SVC(kernel='rbf') y más rápido
    clf = LinearSVC(random_state=42, dual='auto') 
    clf.fit(X_train, y_train)

    # 5. Evaluar
    print("\n--- 4. Evaluando Modelo ---")
    y_pred = clf.predict(X_test)
    accuracy = accuracy_score(y_test, y_pred)
    print(f"Precisión (Accuracy): {accuracy * 100:.2f}%")
    print("\nReporte de Clasificación:")
    print(classification_report(y_test, y_pred, target_names=le.classes_))

    # 6. Prueba unitaria (Inferencia)
    print("\n--- 5. Prueba de Inferencia (Demo) ---")
    if len(X_test) > 0:
        sample_idx = 0
        sample_feat = X_test[sample_idx].reshape(1, -1)
        prediction = clf.predict(sample_feat)
        predicted_label = le.inverse_transform(prediction)[0]
        real_label = le.inverse_transform([y_test[sample_idx]])[0]
        
        print(f"Ejemplo del set de test:")
        print(f"Real: {real_label} vs Predicho: {predicted_label}")

if __name__ == "__main__":
    main()