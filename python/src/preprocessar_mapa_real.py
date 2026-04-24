import cv2
import numpy as np
from pathlib import Path

ROOT_DIR = Path(__file__).resolve().parents[2]
INPUT_IMAGE = ROOT_DIR / "python" / "imagens" / "mapa5.jpeg"
OUTPUT_DIR = ROOT_DIR / "dados"
OUTPUT_DIR.mkdir(exist_ok=True)
CROP_DIR = OUTPUT_DIR / "crop"
MAPA_BINARIO_DIR = OUTPUT_DIR / "mapabinario"
PREVIEW_DIR = OUTPUT_DIR / "preview"
CROP_DIR.mkdir(exist_ok=True)
MAPA_BINARIO_DIR.mkdir(exist_ok=True)
PREVIEW_DIR.mkdir(exist_ok=True)

ROI = (420, 40, 820, 260)
PERCENTIL_CHAO_BAIXO = 8 #Cor mais escura do chao
PERCENTIL_CHAO_ALTO = 92 #Cor mais clara do chao
MARGEM_SOMBRA = 18 #Margem para incluir sombras no limiar inferior 
MARGEM_BRILHO = 8 #Margem para incluir reflexos no limiar superior 
KERNEL_LIMPEZA = 5 #Tamanho do kernel para fechar falhas
MAX_BURACO_INTERNO = 2200 #Para remover buracos pretos internos no chão livre
MAX_ILHA_BRANCA_INTERNA = 3000 #Para remover ilhas brancas (chão falso) dentro de obstáculos
MAX_COMPONENTE_PRETO_NA_BORDA = 6000 #Para remover obstáculos pretos que tocam a borda

# Carrega a imagem em escala de cinza
img = cv2.imread(str(INPUT_IMAGE), cv2.IMREAD_GRAYSCALE)

# Verifica se a imagem foi carregada corretamente
if img is None:
	raise FileNotFoundError(f"Imagem nao encontrada: {INPUT_IMAGE}")

# Verifica se a ROI é válida para as dimensões da imagem
altura, largura = img.shape[:2]
x_start, y_start, x_end, y_end = ROI

# Verifica se a ROI é válida para as dimensões da imagem
if not (0 <= x_start < x_end <= largura and 0 <= y_start < y_end <= altura):
	raise ValueError(
		f"ROI invalida. Use 0 <= x_start < x_end <= {largura} e 0 <= y_start < y_end <= {altura}."
	)

# Corta a ROI da imagem para calcular os percentis do chao apenas nessa regiao
cropped_img = img[y_start:y_end, x_start:x_end]

# Calcula os percentis do chao na ROI e define o limiar com margem para incluir sombras e reflexos.
vmin_roi = int(np.percentile(cropped_img, PERCENTIL_CHAO_BAIXO))
vmax_roi = int(np.percentile(cropped_img, PERCENTIL_CHAO_ALTO))
vmin = max(0, vmin_roi - MARGEM_SOMBRA)
vmax = min(255, vmax_roi + MARGEM_BRILHO)

# Cria mascara binaria onde 255 = livre e 0 = ocupado, usando o limiar definido.
free_mask = np.where((img >= vmin) & (img <= vmax), 255, 0).astype(np.uint8)

# Fecha falhas pequenas no piso (manchas de sombra/reflexo) sem mexer em obstaculos grandes.
kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (KERNEL_LIMPEZA, KERNEL_LIMPEZA))
free_mask = cv2.morphologyEx(free_mask, cv2.MORPH_CLOSE, kernel, iterations=1)

# Remove buracos internos pequenos em areas ocupadas, mantendo apenas os que tocam a borda (que provavelmente sao entradas).
black_mask = 255 - free_mask
num_labels, labels, stats, _ = cv2.connectedComponentsWithStats(black_mask, connectivity=8)
for label in range(1, num_labels):
	x = stats[label, cv2.CC_STAT_LEFT]
	y = stats[label, cv2.CC_STAT_TOP]
	w = stats[label, cv2.CC_STAT_WIDTH]
	h = stats[label, cv2.CC_STAT_HEIGHT]
	area = stats[label, cv2.CC_STAT_AREA]
	touches_border = (
		x == 0 or
		y == 0 or
		x + w >= largura or
		y + h >= altura
	)
	if not touches_border and area <= MAX_BURACO_INTERNO:
		free_mask[labels == label] = 255
	elif touches_border and area <= MAX_COMPONENTE_PRETO_NA_BORDA:
		free_mask[labels == label] = 255

	# Remove ilhas brancas pequenas dentro de obstaculos para deixar contornos mais solidos.
	num_labels_free, labels_free, stats_free, _ = cv2.connectedComponentsWithStats(free_mask, connectivity=8)
	for label in range(1, num_labels_free):
		x = stats_free[label, cv2.CC_STAT_LEFT]
		y = stats_free[label, cv2.CC_STAT_TOP]
		w = stats_free[label, cv2.CC_STAT_WIDTH]
		h = stats_free[label, cv2.CC_STAT_HEIGHT]
		area = stats_free[label, cv2.CC_STAT_AREA]
		touches_border = (
			x == 0 or
			y == 0 or
			x + w >= largura or
			y + h >= altura
		)
		if not touches_border and area <= MAX_ILHA_BRANCA_INTERNA:
			free_mask[labels_free == label] = 0

mapa = np.where(free_mask == 255, 0, -1).astype(np.int8)

# Salva mapa numerico e preview visual (livre=branco, obstaculo=preto).
np.savetxt(MAPA_BINARIO_DIR / "mapa5_binario.txt", mapa, fmt="%d")
preview = np.where(mapa == 0, 255, 0).astype(np.uint8)
cv2.imwrite(str(PREVIEW_DIR / "mapa5_binario_preview.png"), preview)
cv2.imwrite(str(CROP_DIR / "mapa5_roi_processada.png"), cropped_img)

print(f"Imagem: {INPUT_IMAGE}")
print(f"Resolucao: {largura}x{altura}")
print(f"ROI em pixels: x={x_start}:{x_end}, y={y_start}:{y_end}")
print(f"Percentis do chao na ROI: p{PERCENTIL_CHAO_BAIXO}={vmin_roi}, p{PERCENTIL_CHAO_ALTO}={vmax_roi}")
print(f"Limiar ROI original: min={vmin_roi}, max={vmax_roi}")
print(f"Limiar final com margem: min={vmin}, max={vmax}")
print(f"Percentual de area livre: {float((mapa == 0).mean() * 100):.2f}%")
print(f"Mapa salvo em {MAPA_BINARIO_DIR / 'mapa5_binario.txt'}")
print(f"Preview salvo em {PREVIEW_DIR / 'mapa5_binario_preview.png'}")
print(f"ROI processada salva em {CROP_DIR / 'mapa5_roi_processada.png'}")
