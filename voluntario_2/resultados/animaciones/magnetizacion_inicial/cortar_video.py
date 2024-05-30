from moviepy.video.io.VideoFileClip import VideoFileClip

# Ruta del archivo de entrada
input_path = "128_1.mp4"
# Ruta del archivo de salida
output_path = "128_1_2.mp4"

# Duración máxima en segundos (10 minutos = 600 segundos)
max_duration = 9.5 * 60

# Cargar el video
video = VideoFileClip(input_path)

# Cortar el video a la duración máxima
if video.duration > max_duration:
    video = video.subclip(0, max_duration)

# Guardar el video cortado
video.write_videofile(output_path, codec="libx264")

# Cerrar el clip
video.close()
