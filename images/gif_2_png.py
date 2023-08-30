from PIL import Image, ImageSequence

imgName = "driveby"
img = Image.open(imgName + ".gif")
for i, frame in enumerate(ImageSequence.Iterator(img)):
    frame.save(f"{imgName}_{i}.png")
