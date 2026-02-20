# importar los paquetes necesarios
from __future__ import print_function
import argparse
import datetime
import imutils
import cv2
# construir el analizador de argumentos y parsear los argumentos
ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", required=True,
	help="ruta a la imagen de entrada")
ap.add_argument("-w", "--win-stride", type=str, default="(8, 8)",
	help="paso de ventana")
ap.add_argument("-p", "--padding", type=str, default="(16, 16)",
	help="relleno del objeto")
ap.add_argument("-s", "--scale", type=float, default=1.05,
	help="escala de la pirámide de imágenes")
ap.add_argument("-m", "--mean-shift", type=int, default=-1,
	help="si se debe usar agrupamiento mean shift o no")
args = vars(ap.parse_args())

# El paso de la ventana. ¿Cuánto se mueve la ventana verde cada vez?
winStride = eval(args["win_stride"])

# El margen. Cuánto espacio extra añadimos alrededor de la imagen antes de procesarla.
padding = eval(args["padding"])
meanShift = True if args["mean_shift"] > 0 else False

# inicializar el descriptor HOG/detector de personas
hog = cv2.HOGDescriptor()
hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())

# cargar la imagen y redimensionarla
image = cv2.imread(args["image"])
image = imutils.resize(image, width=min(400, image.shape[1]))

# detectar personas en la imagen
start = datetime.datetime.now()
(rects, weights) = hog.detectMultiScale(image, winStride=winStride,
	padding=padding, scale=args["scale"], useMeanshiftGrouping=meanShift)
print("[INFO] la detección tomó: {}s".format(
	(datetime.datetime.now() - start).total_seconds()))

# dibujar los cuadros delimitadores originales
for (x, y, w, h) in rects:
	cv2.rectangle(image, (x, y), (x + w, y + h), (0, 255, 0), 2)

# mostrar la imagen de salida
cv2.imshow("Detecciones", image)
cv2.waitKey(0)
