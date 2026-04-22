import cv2
import numpy as np
from pathlib import Path

ROOT_DIR = Path(__file__).resolve().parents[2]
INPUT_IMAGE = ROOT_DIR / "python" / "imagens" / "mapa1.png"
OUTPUT_DIR = ROOT_DIR / "dados"
OUTPUT_DIR.mkdir(exist_ok=True)

# Leitura da imagem em uma variavel (escala de cinza).
img = cv2.imread(str(INPUT_IMAGE), cv2.IMREAD_GRAYSCALE)

# Verifica se a imagem foi carregada corretamente
if img is None:
	raise FileNotFoundError(f"Imagem nao encontrada: {INPUT_IMAGE}")

# Obtém as dimensões da imagem
altura, largura = img.shape[:2]

# Define as coordenadas do retângulo de recorte (x_start, y_start, x_end, y_end)
x_start, y_start, x_end, y_end = 50, 170, 100, 256

# Verifica se as coordenadas estão dentro dos limites da imagem
if not (0 <= x_start < x_end <= largura and 0 <= y_start < y_end <= altura):
	raise ValueError(
		f"ROI invalida. Use 0 <= x_start < x_end <= {largura} e 0 <= y_start < y_end <= {altura}."
	)
 
# Recorta a imagem usando as coordenadas definidas.
cropped_img = img[y_start:y_end, x_start:x_end]
 
# Usa min e max do recorte como limiar para classificar toda a imagem.
vmin = int(cropped_img.min())
vmax = int(cropped_img.max())
mapa = np.where((img >= vmin) & (img <= vmax), 0, -1).astype(np.int8)

# Salva mapa numerico e preview visual (livre=branco, obstaculo=preto).
np.savetxt(OUTPUT_DIR / "mapa_binario.txt", mapa, fmt="%d")
preview = np.where(mapa == 0, 255, 0).astype(np.uint8)
cv2.imwrite(str(OUTPUT_DIR / "mapa_binario_preview.png"), preview)

print(f"Limiar do terreno: min={vmin}, max={vmax}")
print(f"Mapa salvo em {OUTPUT_DIR / 'mapa_binario.txt'}")
print(f"Preview salvo em {OUTPUT_DIR / 'mapa_binario_preview.png'}")