import numpy as np
from PIL import Image

def show_blue_component(img):
    a = np.array(img)
    a[:,:,0] *=0    # hiding red component
    a[:,:,1] *=0    # hiding green component
    a = Image.fromarray(a)
    a.show()


def show_green_component(img):
    a = np.array(img)
    a[:,:,0] *=0    # hiding red component
    a[:,:,2] *=0    # hiding blue component
    a = Image.fromarray(a)
    a.show()


def show_red_component(img):
    a = np.array(img)
    a[:,:,1] *=0    # hiding green component
    a[:,:,2] *=0    # hiding blue component
    a = Image.fromarray(a)
    a.show()
    
    
def show_rgb(img):
    show_red_component(img)
    show_green_component(img)
    show_blue_component(img)
    
    
def to_rgb(y, cr, cb):
    red = y + (1.402 * (cr-128))
    green = y - (0.34414 * (cb-128)) - (0.71414 * (cr-128))
    blue = y + (1.772 * (cb-128))
    
    new_a = np.zeros((red.shape[0], red.shape[1], 3))
    new_a[:,:,0] = red
    new_a[:,:,1] = green
    new_a[:,:,2] = blue
    
    return new_a.astype(np.uint8)   # needs to remove floating points


def to_ycbcr(red, green, blue):
    y = (0.299 * red) + (0.587 * green) + (0.114 * blue)
    cb = (-0.1687 * red) + (-0.3313 * green) + (0.5 * blue) + 128
    cr = (0.5 * red) + (-0.4187 * green) + (-0.0813 * blue) + 128

    new_a = np.zeros((red.shape[0], red.shape[1], 3))
    new_a[:,:,0] = y
    new_a[:,:,1] = cb
    new_a[:,:,2] = cr
    
    return new_a.astype(np.uint8)   # needs to remove floating points

# https://stackoverflow.com/questions/74158097/how-to-display-colored-y-cb-cr-channels-in-python-opencv
def show_chroma_components(img):
    a = np.array(img)
    
    red = a[:,:,0]
    green = a[:,:,1]
    blue = a[:,:,2]
    
    y = (0.299 * red) + (0.587 * green) + (0.114 * blue)
    cb = (-0.1687 * red) + (-0.3313 * green) + (0.5 * blue) + 128
    cr = (0.5 * red) + (-0.4187 * green) + (-0.0813 * blue) + 128
    
    chroma_red_img = to_rgb(y, cr, cb)
    chroma_red_img = Image.fromarray(chroma_red_img)
    chroma_red_img.show()
    
    y_img = Image.fromarray(y)
    y_img.show()
    
    neutral_values = np.ones((red.shape[0], red.shape[1])) * 128
    
    chroma_blue_img = to_rgb(neutral_values, neutral_values, cb)
    chroma_blue_img = Image.fromarray(chroma_blue_img)
    chroma_blue_img.show()
    
    chroma_red_img = to_rgb(neutral_values, cr, neutral_values)
    chroma_red_img = Image.fromarray(chroma_red_img)
    chroma_red_img.show()
    


def main():
    img = Image.open("test_img/test_0.bmp")
    img.show()
    
    a = np.array(img)
    
    red = a[:,:,0]
    green = a[:,:,1]
    blue = a[:,:,2]
    
    y = (0.299 * red) + (0.587 * green) + (0.114 * blue)
    cb = (-0.1687 * red) + (-0.3313 * green) + (0.5 * blue) + 128
    cr = (0.5 * red) + (-0.4187 * green) + (-0.0813 * blue) + 128
    
    print(red.astype(np.uint8))
    print(green.astype(np.uint8))
    print(blue.astype(np.uint8))
    print(y.astype(np.uint8))
    print(cb.astype(np.uint8))
    print(cr.astype(np.uint8))
main()