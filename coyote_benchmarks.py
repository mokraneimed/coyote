import os
from coyote import *
import time
import csv
import subprocess
from evaluate import *

def shift_down(image):
    return [[Var('0')] * 4] + image[:-1]

def shift_up(image):
    return image[1:] + [[Var('0')] * 4]

def shift_left(image):
    return [row[1:] + [Var('0')] for row in image]

def shift_right(image):
    return [[Var('0')] + row[:-1] for row in image]

def matrix_addition(matrix1, matrix2):    
    return [[matrix1[i][j] + (matrix2[i][j]) for j in range(4)] for i in range(4)]

coyote = coyote_compiler()

@coyote.define_circuit(a=matrix(3, 3), b=matrix(3, 3))
def matmul_3x3_un(a, b):
    return [recursive_sum([a[i][k] * b[k][j] for k in range(len(a))]) for i in range(len(a)) for j in range(len(a))]

@coyote.define_circuit(a=vector(4), b=vector(4))
def l2_distance_4(a, b):
    return recursive_sum([(x - y) * (x - y) for x, y in zip(a, b)])

@coyote.define_circuit(c0=vector(1), c1=vector(1), c2=vector(1), c3=vector(1), c4=vector(1))
def poly_reg(c0, c1, c2, c3, c4):
    return [c1[i] - (c0[i] * c0[i] * c4[i] + c0[i] * c3[i] + c2[i]) for i in range(1)]

@coyote.define_circuit(c0=vector(1), c1=vector(1), c2=vector(1), c3=vector(1))
def linear_reg(c0, c1, c2, c3):
    return [c1[i] - (c2[i] * c0[i]) - c3[i] for i in range(1)]

@coyote.define_circuit(image = matrix(4,4))
def box_blur(image):
    top_row = shift_up(image)
    bottom_row = shift_down(image)

    top_sum = matrix_addition(matrix_addition(shift_left(top_row), top_row), shift_right(top_row))
    curr_sum = matrix_addition(matrix_addition(shift_left(image), image), shift_right(image))
    bottom_sum = matrix_addition(matrix_addition(shift_left(bottom_row), bottom_row), shift_right(bottom_row))

    result = matrix_addition(matrix_addition(top_sum, curr_sum), bottom_sum)

    return [result[i][j] for i in range(4) for j in range(4)]

with open('benchmarks_evaluation.csv', mode='w', newline='') as csvfile:
    csvwriter = csv.writer(csvfile)
    # Write the header
    csvwriter.writerow(['Benchmark name', 'Compile time',
                        'multiplications',
                        'additions',
                        'substitutions',
                        'rotations',
                        'scalar/plain multiplications',
                        'Depth',
                        'Multiplicative depth'
                        ])

for func in coyote.func_signatures:
    print(func.__name__)
    benchmark_name = func.__name__

    start_time = time.time()
    scalar_code = coyote.instantiate(benchmark_name)
    vector_code = list(map(str, coyote.vectorize()))

    end_time = time.time()
    compilation_time = end_time - start_time
    print(f'Compilation time: {compilation_time:.2f} seconds')
    try:
        os.mkdir('outputs')
    except FileExistsError:
        pass

    try:
        os.mkdir(f'outputs/{benchmark_name}')
    except FileExistsError:
        pass

    open(f'outputs/{benchmark_name}/scal', 'w').write('\n'.join(scalar_code))
    open(f'outputs/{benchmark_name}/vec', 'w').write('\n'.join(vector_code))
    print(f'Successfully compiled benchmark {benchmark_name}; outputs placed in outputs/{benchmark_name}!')

    subprocess.run(['python3', 'compile_to_bfv.py', benchmark_name])
    
    num_multiplications, num_additions, num_substitutions, num_rotations, num_plain_multiplications, max_depth, max_multiplicative_depth = evaluate(benchmark_name)
    with open('benchmarks_evaluation.csv', mode='a', newline='') as csvfile:
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow([benchmark_name, compilation_time,
                        num_multiplications, num_additions, num_substitutions, 
                        num_rotations, num_plain_multiplications, 
                        max_depth, max_multiplicative_depth])

subprocess.run(['python3', 'build_and_run_all.py', '--iters=1', '--runs=1'])

csv_directory = 'bfv_backend/csvs'

run_times = {}
for filename in os.listdir(csv_directory):
    if filename.endswith('.csv'):
        benchmark_name = filename[:-4]  # Remove the '.csv' extension to get the benchmark name
        file_path = os.path.join(csv_directory, filename)

        # Read the second row and fourth column from the CSV file
        with open(file_path, mode='r', newline='') as csvfile:
            reader = csv.reader(csvfile)
            rows = list(reader)
            if len(rows) > 1 and len(rows[1]) > 3:  # Ensure there is a second row and fourth column
                run_time = rows[1][3]  # Get the value from the second row and fourth column
                run_times[benchmark_name] = run_time

with open('benchmarks_evaluation.csv', mode='r', newline='') as infile:
    reader = csv.reader(infile)
    headers = next(reader)
    rows = list(reader)

if 'Run time' not in headers:
    headers.append('Run time')

# Update the benchmark evaluation CSV with the run times
with open('benchmarks_evaluation.csv', mode='w', newline='') as outfile:
    writer = csv.writer(outfile)
    writer.writerow(headers)
    for row in rows:
        benchmark_name = row[0]
        if benchmark_name in run_times:
            row.append(run_times[benchmark_name])
        else:
            row.append('')
        writer.writerow(row)


