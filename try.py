image = [[7, 4, 0, 1],
         [5, 6, 2, 2],
         [6, 10, 7, 8],
         [1, 4, 2, 0]]

def shift_down(image):
    """Shift the image down by one row. << width""" 
    return [[0] * 4] + image[:-1]

def shift_up(image):
    """Shift the image up by one row. >> width""" 
    return image[1:] + [[0] * 4]

def shift_left(image):
    """Shift the image left by one column. >> 1"""
    return [row[1:] + [0] for row in image]

def shift_right(image):
    """Shift the image right by one column. << 1"""
    return [[0] + row[:-1] for row in image]

def matrix_addition(matrix1, matrix2):    
    return [[matrix1[i][j] + (matrix2[i][j]) for j in range(4)] for i in range(4)]

def box_blur(image):
    top_row = shift_up(image)
    bottom_row = shift_down(image)

    top_sum = matrix_addition(matrix_addition(shift_left(top_row), top_row), shift_right(top_row))
    curr_sum = matrix_addition(matrix_addition(shift_left(image), image), shift_right(image))
    bottom_sum = matrix_addition(matrix_addition(shift_left(bottom_row), bottom_row), shift_right(bottom_row))

    result = matrix_addition(matrix_addition(top_sum, curr_sum), bottom_sum)

    return [result[i][j] for i in range(4) for j in range(4)]

def box_blur_2(image):
    result = [[0] * 4 for _ in range(4)]
    
    for i in range(4):
        for j in range(4):
            result[i][j] = (
                (image[i-1][j-1] if i-1 >= 0 and j-1 >= 0 else 0) +
                (image[i-1][j] if i-1 >= 0 else 0) +
                (image[i-1][j+1] if i-1 >= 0 and j+1 < 4 else 0) +
                (image[i][j-1] if j-1 >= 0 else 0) +
                image[i][j] +
                (image[i][j+1] if j+1 < 4 else 0) +
                (image[i+1][j-1] if i+1 < 4 and j-1 >= 0 else 0) +
                (image[i+1][j] if i+1 < 4 else 0) +
                (image[i+1][j+1] if i+1 < 4 and j+1 < 4 else 0)
            )
    return  [
                [
                    (image[i-1][j-1] if i-1 >= 0 and j-1 >= 0 else 0) +
                    (image[i-1][j] if i-1 >= 0 else 0) +
                    (image[i-1][j+1] if i-1 >= 0 and j+1 < 4 else 0) +
                    (image[i][j-1] if j-1 >= 0 else 0) +
                    image[i][j] +
                    (image[i][j+1] if j+1 < 4 else 0) +
                    (image[i+1][j-1] if i+1 < 4 and j-1 >= 0 else 0) +
                    (image[i+1][j] if i+1 < 4 else 0) +
                    (image[i+1][j+1] if i+1 < 4 and j+1 < 4 else 0)
                    for j in range(4)
                ] 
                for i in range(4)
            ]          
                            
                        

print(box_blur(image))