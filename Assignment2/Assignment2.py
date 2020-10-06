from ImageGeneticModule import Triangle
from PIL import Image
from PIL import ImageDraw
import cv2
import numpy as np

if __name__ == "__main__":
    imgobj = Image.open('plane.jpg')
    pixels = imgobj.convert('RGBA')
    data = imgobj.getdata()
    lofpixels = []
    for pixel in data:
        lofpixels.append(pixel)
    
    print(data[0])
    # image = Image.open('plane.jpg') # Can be many different formats.
    # sourcePix = image.load()         # Load Image
    # ImageSize = image.size     # Get the width and hight of the image for iterating over
    # # print(pix[image.size[0]/2.0,image.size[1]/2.0])

    # # Create empty black canvas
    # im = Image.new('RGB', image.size)
    # draw = ImageDraw.Draw(im)
    
    # triangles = []
    # for _ in range(100):
    #     triangles.append(Triangle(image.size))

    # for tri in triangles:
    #     draw.polygon([(tri.p1.x,tri.p1.y), (tri.p2.x,tri.p2.y),(tri.p3.x,tri.p3.y)], fill = (tri.rgb.R,tri.rgb.G,tri.rgb.B))
    
    # triPix = im.load()
    # sum = 0
    # # im.save('result.png')
    # for rPix in range(image.size[0]):
    #     for cPix in range(image.size[1]):
    #         sum = sum + (triPix[rPix, cPix] == sourcePix[rPix,cPix])

    # print(image.size)
    # print(sum / (image.size[0] * image.size[1]) * 100)