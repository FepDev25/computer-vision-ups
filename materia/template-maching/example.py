import cv2
import numpy as np
import imutils

def non_max_suppression_fast(boxes, overlapThresh):
    """
    Aplica supresión de no-máximos para eliminar detecciones redundantes/superpuestas.
    Algoritmo esencial para limpiar la salida del template matching.
    """
    if len(boxes) == 0:
        return []

    if boxes.dtype.kind == "i":
        boxes = boxes.astype("float")

    pick = []
    x1 = boxes[:,0]
    y1 = boxes[:,1]
    x2 = boxes[:,2]
    y2 = boxes[:,3]
    score = boxes[:,4]

    area = (x2 - x1 + 1) * (y2 - y1 + 1)
    idxs = np.argsort(score) # Ordenar por confianza

    while len(idxs) > 0:
        last = len(idxs) - 1
        i = idxs[last]
        pick.append(i)

        xx1 = np.maximum(x1[i], x1[idxs[:last]])
        yy1 = np.maximum(y1[i], y1[idxs[:last]])
        xx2 = np.minimum(x2[i], x2[idxs[:last]])
        yy2 = np.minimum(y2[i], y2[idxs[:last]])

        w = np.maximum(0, xx2 - xx1 + 1)
        h = np.maximum(0, yy2 - yy1 + 1)

        overlap = (w * h) / area[idxs[:last]]

        idxs = np.delete(idxs, np.concatenate(([last],
            np.where(overlap > overlapThresh))))

    return boxes[pick].astype("int")

def template_matching_avanzado(imagen_path, template_path, threshold=0.8, visualizar=True):
    # 1. Carga y Preprocesamiento
    # Convertir a escala de grises para reducir dimensionalidad y enfocarse en luminancia
    img = cv2.imread(imagen_path)
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    template = cv2.imread(template_path, 0)
    (tH, tW) = template.shape[:2]

    # Detección de Bordes (Canny)
    # Estrategia robusta: Matching sobre bordes en lugar de intensidad cruda
    # Elimina variaciones de color y gradientes suaves de iluminación.
    gray_edges = cv2.Canny(gray, 50, 200)
    template_edges = cv2.Canny(template, 50, 200)

    found_matches = []

    # 2. Bucle Multiescala
    # Explorar escalas desde 20% hasta 150% del tamaño original
    for scale in np.linspace(0.2, 1.5, 40):
        # Redimensionar la imagen (manteniendo aspect ratio)
        resized = imutils.resize(gray_edges, width=int(gray_edges.shape[1] * scale))
        r = gray_edges.shape[1] / float(resized.shape[1])

        # Verificar si la imagen redimensionada es menor que el template
        if resized.shape[0] < tH or resized.shape[1] < tW:
            break

        # 3. Ejecutar MatchTemplate (CCOEFF_NORMED)
        # Se usa correlación normalizada para comparar formas de bordes
        result = cv2.matchTemplate(resized, template_edges, cv2.TM_CCOEFF_NORMED)
        
        # Filtrar por umbral de confianza
        locs = np.where(result >= threshold)
        
        # Recolectar candidatos
        for pt in zip(*locs[::-1]):
            match_score = result[pt[1], pt[0]]
            # Escalar coordenadas al tamaño original
            startX = int(pt[0] * r)
            startY = int(pt[1] * r)
            endX = int((pt[0] + tW) * r)
            endY = int((pt[1] + tH) * r)
            found_matches.append((startX, startY, endX, endY, match_score))

    # 4. Post-procesamiento: Supresión de No-Máximos (NMS)
    # Fusionar rectángulos superpuestos que corresponden al mismo objeto
    if len(found_matches) == 0:
        print("No se encontraron coincidencias.")
        return img if visualizar else []
    
    found_matches = np.array(found_matches)
    final_boxes = non_max_suppression_fast(found_matches, 0.3)

    print(f"Se encontraron {len(final_boxes)} instancias únicas.")

    # 5. Visualización
    if visualizar:
        for (startX, startY, endX, endY, score) in final_boxes:
            cv2.rectangle(img, (startX, startY), (endX, endY), (0, 255, 0), 2)
            cv2.putText(img, f"{score:.2f}", (startX, startY - 10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
        return img

    return final_boxes


if __name__ == "__main__":
    import sys
    
    # Verificar argumentos
    if len(sys.argv) < 3:
        print("Uso: python example.py <ruta_imagen> <ruta_template> [threshold]")
        print("Ejemplo: python example.py imagen.jpg plantilla.jpg 0.8")
        sys.exit(1)
    
    imagen_path = sys.argv[1]
    template_path = sys.argv[2]
    threshold = float(sys.argv[3]) if len(sys.argv) > 3 else 0.8
    
    # Ejecutar template matching
    resultado = template_matching_avanzado(imagen_path, template_path, threshold, visualizar=True)
    
    if resultado is not None:
        # Mostrar resultado
        cv2.imshow('Template Matching Result', resultado)
        
        # Guardar resultado
        cv2.imwrite('resultado_template_matching.jpg', resultado)
        print("Resultado guardado en: resultado_template_matching.jpg")
        print("Presiona cualquier tecla para cerrar...")
        
        cv2.waitKey(0)
        cv2.destroyAllWindows()