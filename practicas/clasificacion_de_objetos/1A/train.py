from ultralytics import YOLO

model = YOLO('yolo11n-seg.pt')

if __name__ == '__main__':
    print("Iniciando entrenamiento... ")
    results = model.train(
        data='data/data.yaml', 
        epochs=30, 
        imgsz=640, 
        device=0, 
        batch=8,
        project='Frutas_Project',
        name='modelo_final'
    )