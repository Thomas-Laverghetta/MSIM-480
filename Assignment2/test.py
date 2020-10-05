from PIL import Image

im = Image.open('plane.jpg') # Can be many different formats.
pix = im.load()
print (im.size)  # Get the width and hight of the image for iterating over
for i in range(50):
    print (pix[1,i])  # Get the RGBA Value of the a pixel of an image