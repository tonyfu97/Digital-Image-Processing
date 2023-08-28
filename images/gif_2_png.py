from PIL import Image, ImageSequence

img = Image.open("driveby.gif")
for i, frame in enumerate(ImageSequence.Iterator(img)):
    frame.save(f"driveby_{i}.png")
