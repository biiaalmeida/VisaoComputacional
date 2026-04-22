import numpy as np
import cv2
from pathlib import Path


size = 256

# Criando uma imagem branca (255) de 256x256 pixels.
img = np.full((size, size), 255, dtype=np.uint8)

# Preto = obstáculo, branco = espaço livre.
# A imagem é gerada diretamente, sem depender de matriz de entrada.
obstacles = [
	(24, 24, 56, 156),
	(102, 10, 120, 94),
	(102, 162, 120, 244),
	(156, 48, 196, 70),
	(168, 126, 224, 146),
	(188, 190, 228, 210),
]

# Desenha os obstáculos como retângulos pretos na imagem.
for x1, y1, x2, y2 in obstacles:
	cv2.rectangle(img, (x1, y1), (x2, y2), 0, thickness=-1)

# Garante um caminho livre contínuo entre início e fim.
start = (12, 12)
end = (244, 244)
path_points = np.array([
	start,
	(12, 220),
	(220, 220),
	(end),
], dtype=np.int32)

cv2.polylines(img, [path_points], isClosed=False, color=255, thickness=22)
cv2.circle(img, start, 14, 255, thickness=-1)
cv2.circle(img, end, 14, 255, thickness=-1)

output_dir = Path(__file__).resolve().parent.parent / "imagens"
output_dir.mkdir(exist_ok=True)

output_file = output_dir / "mapa1.png"
cv2.imwrite(str(output_file), img)
print(f"Imagem gerada: {output_file}")