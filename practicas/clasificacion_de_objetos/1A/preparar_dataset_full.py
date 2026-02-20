import os
import shutil
import random

ORIGEN = "data"
DESTINO_ROOT = "dataset_colab"

# Límites de imágenes por clase
LIMITES = {
    "train": 400,  
    "valid": 50,   
    "test": 50     
}

CLASES_KEYWORD = {
    "bitter": "bitter_melon", 
    "cucumber": "cucumber",
    "fig": "fig",
    "jujube": "jujube",
    "boyang": "melon_boyang", 
    "musk": "muskmelon"       
}

def preparar_dataset():
    if os.path.exists(DESTINO_ROOT):
        shutil.rmtree(DESTINO_ROOT)
    
    for split, limite in LIMITES.items():
        print(f"\nProcesando {split.upper()}...")
        
        img_orig = os.path.join(ORIGEN, split, "images")
        lbl_orig = os.path.join(ORIGEN, split, "labels")
        
        if not os.path.exists(img_orig):
            print(f"saltando {split}, no existe la carpeta {img_orig}")
            continue

        img_dest = os.path.join(DESTINO_ROOT, split, "images")
        lbl_dest = os.path.join(DESTINO_ROOT, split, "labels")
        os.makedirs(img_dest, exist_ok=True)
        os.makedirs(lbl_dest, exist_ok=True)

        todos_los_archivos = [f for f in os.listdir(img_orig) if f.lower().endswith(('.jpg', '.png', '.jpeg'))]

        for key, nombre_clase in CLASES_KEYWORD.items():
            archivos_clase = [f for f in todos_los_archivos if key in f.lower()]
            if not archivos_clase:
                archivos_clase = [f for f in todos_los_archivos if f.lower().startswith(key[0])]
            seleccionados = random.sample(archivos_clase, min(len(archivos_clase), limite))
            print(f"{nombre_clase}: {len(seleccionados)} imágenes copiadas.")

            for img_name in seleccionados:
                shutil.copy(os.path.join(img_orig, img_name), os.path.join(img_dest, img_name))
                lbl_name = os.path.splitext(img_name)[0] + ".txt"
                if os.path.exists(os.path.join(lbl_orig, lbl_name)):
                    shutil.copy(os.path.join(lbl_orig, lbl_name), os.path.join(lbl_dest, lbl_name))

    yaml_content = f"""
path: /content/dataset_colab
train: train/images
val: valid/images
test: test/images

nc: 6
names: ['bitter_melon', 'cucumber', 'fig', 'jujube', 'melon_boyang', 'muskmelon']
"""
    with open(os.path.join(DESTINO_ROOT, "data.yaml"), "w") as f:
        f.write(yaml_content.strip())
    print(f"\nProceso completado. Carpeta '{DESTINO_ROOT}' lista para comprimir.")

if __name__ == "__main__":
    preparar_dataset()