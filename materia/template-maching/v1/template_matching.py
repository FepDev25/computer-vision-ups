
import cv2
import numpy as np

def template_matching(main_image_path, template_image_path, output_image_path):
    """
    Realiza el template matching para encontrar una plantilla en una imagen principal.

    Args:
        main_image_path (str): La ruta a la imagen principal.
        template_image_path (str): La ruta a la imagen de la plantilla.
        output_image_path (str): La ruta para guardar la imagen con el resultado.
    """
    # Cargar la imagen principal y la plantilla
    img_rgb = cv2.imread(main_image_path)
    img_gray = cv2.cvtColor(img_rgb, cv2.COLOR_BGR2GRAY)
    template = cv2.imread(template_image_path, 0)

    # Obtener las dimensiones de la plantilla
    w, h = template.shape[::-1]

    # Realizar el template matching
    res = cv2.matchTemplate(img_gray, template, cv2.TM_CCOEFF_NORMED)
    
    # Definir un umbral de confianza para la detección
    threshold = 0.8
    
    # Encontrar las ubicaciones donde la similitud es mayor que el umbral
    loc = np.where(res >= threshold)

    # Dibujar un rectángulo alrededor de las áreas detectadas
    for pt in zip(*loc[::-1]):
        cv2.rectangle(img_rgb, pt, (pt[0] + w, pt[1] + h), (0, 0, 255), 2)

    # Guardar la imagen con los resultados
    cv2.imwrite(output_image_path, img_rgb)
    print(f"Resultado guardado en: {output_image_path}")

if __name__ == '__main__':
    # Rutas a las imágenes (puedes cambiarlas por las tuyas)
    main_image = 'space_invaders.png'
    template_image = 'nave.png'
    output_image = 'resultado.png'

    # Llamar a la función de template matching
    template_matching(main_image, template_image, output_image)
