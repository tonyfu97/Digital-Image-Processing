from PIL import Image
import matplotlib.pyplot as plt

imgName = "shuffleboard"

# Initialize 3x5 subplot grid
fig, axs = plt.subplots(3, 5, figsize=(15, 9))

# set background color
# fig.patch.set_facecolor('#E2D282')  # yellow
fig.patch.set_facecolor('#EE7272')  # red

# Remove axis labels and tickmarks
for ax in axs.flatten():
    ax.axis('off')

# Loop through images and plot them
for i in range(1, 16):
    img_path = f"kalman_{imgName}_{i}.png"
    img = Image.open(img_path)
    
    row = (i - 1) // 5
    col = (i - 1) % 5
    
    axs[row, col].imshow(img)
    axs[row, col].set_title(f"Frame {i}")

plt.tight_layout()
plt.savefig(f"kalman_{imgName}_subplots.png")
