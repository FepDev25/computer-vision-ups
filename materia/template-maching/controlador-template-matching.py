import numpy as np
import cv2
import os

class ControladorTemplateMatching:
    def __init__(self, ruta_template='Nave.png'):
        self.ruta_template = ruta_template

    def get_screenshot(self):
        # usar screenshot manual
        screenshot_path = "Nave.png"
        if os.path.exists(screenshot_path):
            imagen = cv2.imread(screenshot_path)
            return imagen
        else:
            print("guardar 'Nave.png'")
            input("Presiona Enter cuando tengas la imagen...")
            if os.path.exists(screenshot_path):
                imagen = cv2.imread(screenshot_path)
                return imagen
            else:
                return None

    def mostrar_imagen(self, img):
        try:
            import matplotlib.pyplot as plt
            import matplotlib
            matplotlib.use('TkAgg')
            
            img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
            plt.imshow(img_rgb)
            plt.axis('off')
            plt.show()
        except:
            cv2.imshow("Nave.png", img)    
            cv2.waitKey(0)
            cv2.destroyAllWindows()

    def detectar_template(self, imagen, ruta_tamplate = 'Nave.png'):
        templ = cv2.imread(ruta_tamplate, 0)
        gris = cv2.cvtColor(imagen, cv2.COLOR_BGR2GRAY)

        res = cv2.matchTemplate(gris, templ, cv2.TM_CCOEFF_NORMED)
        min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)

        cv2.rectangle(imagen, (max_loc[0], max_loc[1], templ.shape[1], templ.shape[0]), (0, 255, 0), 2)

        cv2.imshow("Gris", gris)
        cv2.imshow("templ", templ)
        cv2.imshow("res", res)
        cv2.imshow("Imagen", imagen)
    
if __name__ == '__main__':
    controlador = ControladorTemplateMatching()
    img = controlador.get_screenshot()
    if img is not None:
        controlador.mostrar_imagen(img)
    else:
        print("No se pudo cargar la imagen Nave.png")