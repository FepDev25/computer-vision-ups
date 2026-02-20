import cv2
import torch
import argparse
import sys
import time
import psutil
import uuid
from ultralytics import YOLO
import numpy as np

try:
    from basicsr.archs.rrdbnet_arch import RRDBNet
    from realesrgan import RealESRGANer
    HAS_REALESRGAN = True
except Exception as e: 
    HAS_REALESRGAN = False
    print(f"ERROR CRÍTICO AL IMPORTAR IA: {e}") 

def get_mac_address():
    mac = uuid.getnode()
    return ':'.join(('%012X' % mac)[i:i+2] for i in range(0, 12, 2))

def parse_args():
    parser = argparse.ArgumentParser(description='YOLO + Super Resolution')
    parser.add_argument('input', nargs='?', default='0', help='Video path or camera index')
    parser.add_argument('--output', '-o', type=str, help='Output video path')
    parser.add_argument('--device', '-d', choices=['cpu', 'cuda', 'auto'], default='auto', help='Device')
    parser.add_argument('--yolo-model', default='yolo11n.pt', help='YOLO model') 
    parser.add_argument('--no-sr', action='store_true', help='Disable Super Resolution')
    return parser.parse_args()

args = parse_args()

if args.device == 'auto':
    DEVICE = 'cuda' if torch.cuda.is_available() else 'cpu'
else:
    DEVICE = args.device

print(f"Corriendo en: {DEVICE.upper()}")

print("Cargando YOLO...")
try:
    model_yolo = YOLO(args.yolo_model)
except:
    print(f"No se encontró {args.yolo_model}, bajando yolov8n.pt...")
    model_yolo = YOLO('yolov8n.pt')

model_yolo.to(DEVICE)

upsampler = None
if not args.no_sr and HAS_REALESRGAN:
    print("Cargando Real-ESRGAN...")
    try:
        model_sr = RRDBNet(num_in_ch=3, num_out_ch=3, num_feat=64, num_block=23, num_grow_ch=32, scale=2)
        upsampler = RealESRGANer(
            scale=2,
            model_path='https://github.com/xinntao/Real-ESRGAN/releases/download/v0.2.1/RealESRGAN_x2plus.pth',
            model=model_sr,
            tile=0, 
            tile_pad=10,
            pre_pad=0,
            half=(DEVICE == 'cuda'),
            device=torch.device(DEVICE)
        )
    except Exception as e:
        print(f"Error cargando SR: {e}")
        upsampler = None

try:
    input_src = int(args.input) if args.input.isdigit() else args.input
except ValueError:
    input_src = args.input

cap = cv2.VideoCapture(input_src)
if not cap.isOpened():
    sys.exit("Error abriendo video/cámara")

cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

writer = None
fps_input = cap.get(cv2.CAP_PROP_FPS) if cap.get(cv2.CAP_PROP_FPS) > 1 else 30.0

window_name = 'YOLO + SR (Monitor de Rendimiento)'
cv2.namedWindow(window_name, cv2.WINDOW_NORMAL) 

print("Presiona 'q' para salir.")

# Variables para métricas
prev_time = 0
mac_address = get_mac_address()
ram_usage_list = [] 

while True:
    start_loop = time.time()
    ret, frame = cap.read()
    if not ret: break

    # detección YOLO
    results = model_yolo(frame, device=DEVICE, verbose=False)
    annotated_frame = results[0].plot()

    # 1.25
    final_frame = annotated_frame
    if not args.no_sr:
        if upsampler:
            try:
                final_frame, _ = upsampler.enhance(annotated_frame, outscale=1.25)
                final_frame = np.ascontiguousarray(final_frame)
            except RuntimeError:
                final_frame = cv2.resize(annotated_frame, None, fx=1.25, fy=1.25, interpolation=cv2.INTER_CUBIC)
        else:
            final_frame = cv2.resize(annotated_frame, None, fx=1.25, fy=1.25, interpolation=cv2.INTER_CUBIC)

    # calcular las métricas
    curr_time = time.time()
    fps = 1 / (curr_time - prev_time) if prev_time != 0 else 0
    prev_time = curr_time
    
    ram_usage = psutil.virtual_memory().percent
    ram_usage_list.append(ram_usage)
    
    vram_info = ""
    if DEVICE == 'cuda':
        vram_mb = torch.cuda.memory_reserved(0) / 1024 / 1024
        vram_info = f"VRAM: {vram_mb:.0f} MB"

    # dibujar información en pantalla
    color = (0, 255, 0) if DEVICE == 'cuda' else (0, 0, 255)
    
    cv2.putText(final_frame, f"FPS: {fps:.2f}", (20, 40), cv2.FONT_HERSHEY_SIMPLEX, 1, color, 2)
    cv2.putText(final_frame, f"Mode: {DEVICE.upper()}", (20, 80), cv2.FONT_HERSHEY_SIMPLEX, 0.7, color, 2)
    cv2.putText(final_frame, f"RAM: {ram_usage}%", (20, 120), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 255), 2)
    if vram_info:
        cv2.putText(final_frame, vram_info, (20, 150), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 255), 2)
    
    cv2.putText(final_frame, f"MAC: {mac_address}", (20, final_frame.shape[0] - 20), 
                cv2.FONT_HERSHEY_SIMPLEX, 0.5, (200, 200, 200), 1)

    if args.output:
        if writer is None:
            h, w = final_frame.shape[:2]
            fourcc = cv2.VideoWriter_fourcc(*'mp4v')
            writer = cv2.VideoWriter(args.output, fourcc, fps_input, (w, h))
            print(f"Grabando en: {args.output} ({w}x{h})")
        writer.write(final_frame)

    cv2.imshow(window_name, final_frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
if writer: writer.release()
cv2.destroyAllWindows()

print("Resumen de Ejecución")
print(f"Dispositivo usado  : {DEVICE.upper()}")
if ram_usage_list:
    avg_ram = sum(ram_usage_list) / len(ram_usage_list)
    max_ram = max(ram_usage_list)
    print(f"RAM Promedio       : {avg_ram:.2f}%")
    print(f"RAM Máxima         : {max_ram:.2f}%")
if DEVICE == 'cuda':
    final_vram = torch.cuda.memory_allocated(0) / 1024 / 1024
    print(f"VRAM Final (Alloc) : {final_vram:.2f} MB")
print(f"MAC Address        : {mac_address}")
print("="*40 + "\n")