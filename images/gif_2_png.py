from PIL import Image, ImageSequence

img = Image.open("shuffleboard.gif")
for i, frame in enumerate(ImageSequence.Iterator(img)):
    frame.save(f"shuffleboard_{i}.png")
