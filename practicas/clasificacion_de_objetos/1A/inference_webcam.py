import cv2
import time
import os
from ultralytics import YOLO


RUTA_MODELO = 'best.pt' 

if not os.path.exists(RUTA_MODELO):
    print(f"ERROR: No encuentro el modelo en: {RUTA_MODELO}")
    print("   -> Revisa la carpeta 'runs' o 'Frutas_Project' en tu explorador de archivos")
    print("   -> Copia la ruta del archivo 'best.pt' y pégala en RUTA_MODELO")
    exit()
else:
    print(f"Modelo encontrado. Cargando en GPU...")


DISPOSITIVO = 0

model = YOLO(RUTA_MODELO)
cap = cv2.VideoCapture(0) 

while True:
    ret, frame = cap.read()
    if not ret: break

    start = time.time()
    
    
    
    results = model.predict(frame, device=DISPOSITIVO, retina_masks=True, conf=0.5, verbose=False)
    
    end = time.time()
    fps = 1 / (end - start)

    annotated_frame = results[0].plot()

    cv2.putText(annotated_frame, f"YOLOv11 (Frutas) | GPU: {DISPOSITIVO} | FPS: {fps:.1f}", (20, 50), 
                cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)

    cv2.imshow("Parte 1A - Demo Frutas", annotated_frame)
    
    if cv2.waitKey(1) & 0xFF == 27: 
        break

cap.release()
cv2.destroyAllWindows()