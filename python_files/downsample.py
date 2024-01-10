import numpy as np
from PIL import Image

def to_ycbcr(red, green, blue):
    y = np.round((0.299 * red) + (0.587 * green) + (0.114 * blue))
    cb = np.round((-0.1687 * red) + (-0.3313 * green) + (0.5 * blue) + 128)
    cr = np.round((0.5 * red) + (-0.4187 * green) + (-0.0813 * blue) + 128)

    return y, cb, cr   # needs to remove floating points



# downsamples the chromas to 4:2:2
# Does all operations inside matrix to prevent needing additional matrix
def subsample_422_bilinear_interpolation(cb, cr):
    
    # downsampling by pairing and averaging the consecutive columns
    # all odd columns will contain updated values
    # if num cols is odd, leave last column unchanged
    num_cols = cb.shape[1]
    num_rows = cb.shape[0]
    
    for col_idx in range(0, num_cols, 2):
        cb[:, col_idx] = (cb[:, col_idx] + cb[:, col_idx+1]) // 2
        cr[:, col_idx] = (cr[:, col_idx] + cr[:, col_idx+1]) // 2
        
    # resampling back to original image size
    # usinig nearest neighbor on edge pixels which bilinear interpolation is not possible
    
    # ignoring last unknown column if num rows is even
    for col_idx in range(1, num_cols-1, 2):
        
        # updating first row (linear interpolation edge case)
        cb[0, col_idx] = (cb[0, col_idx-1] + cb[0, col_idx+1]) // 2
        cb[num_rows-1, col_idx] = (cb[num_rows-1, col_idx-1] + cb[num_rows-1, col_idx+1]) // 2
        
        # updating last row (linear interpolation edge case)
        cr[0, col_idx] = (cr[0, col_idx-1] + cr[0, col_idx+1]) // 2
        cr[num_rows-1, col_idx] = (cr[num_rows-1, col_idx-1] + cr[num_rows-1, col_idx+1]) // 2
        
        # reduced equation for bilinear interpolation
        # each corner holds equal weight for pixel determination
        for row_idx in range(1,num_rows-1):
            cb[row_idx, col_idx] = (cb[row_idx-1, col_idx-1] + cb[row_idx-1, col_idx+1] + cb[row_idx+1, col_idx+1] + cb[row_idx+1, col_idx-1]) // 4
            cr[row_idx, col_idx] = (cr[row_idx-1, col_idx-1] + cr[row_idx-1, col_idx+1] + cr[row_idx+1, col_idx+1] + cr[row_idx+1, col_idx-1]) // 4
    
    # choosing left pixel for last column (edge case)
    if num_rows % 2 == 0:
        cb[:, num_cols-1] = cb[:, num_cols-2]
        cr[:, num_cols-1] = cr[:, num_cols-2]
        
    # chroma channels 4:2:2 subsambled and resampled
    return cb, cr
    
    
    

def main():
    img = Image.open("maggie.jpg").rotate(-90)
    img_matrix = np.array(img)
    red = img_matrix[:,:,0]
    green = img_matrix[:,:,1]
    blue = img_matrix[:,:,2]
    
    # Step 1: Change color model from RGB --> YCbCr
    y, cb, cr = to_ycbcr(red, green, blue)
    
    # Step 2: Chosse