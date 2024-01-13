from PIL import Image
import numpy as np

for i in range(10, 12):
    Image.fromarray(np.random.randint(low=0, high=256, size=(14,14,3), dtype="uint8")).save("test_" + str(i) + ".bmp")

