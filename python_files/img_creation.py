from PIL import Image
import numpy as np

for i in range(10):
    Image.fromarray(np.random.randint(low=0, high=256, size=(8,8,3), dtype="uint8")).save("test_img/test_" + str(i) + ".bmp")

