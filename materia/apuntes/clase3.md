# Operaciones morfologicas

- Para mejorar contraste

- Erosion: eliminando los bordes. Elipse, rectangulo y Cruz
- Dilatacion: duplicar el pixel que tengo a lado, se engrosa el area
- Opening: erosion y lugo dilatacion, para eleminar objetos pequeños como ruido en la imagem
- Closing: dilatar y luego erosionar: tapar huecos internos
- Gradiente: dilatacion - erosion
- Top hat
- Black Hat
- Diferencias entre erosion y dilatacion

original + (top hat - black hat)
porque gradiente de tamaño 1 se detecatn los bordes