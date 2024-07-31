import re
from collections import defaultdict
import argparse


def read_cpp_file(benchmark):
    with open(f'bfv_backend/coyote_out/{benchmark}/vector.cpp', 'r') as file:
        return file.read()

def evaluate(benchmark_name):
    cpp_code = read_cpp_file(benchmark_name)

    multiply_pattern = re.compile(r'info\.eval->multiply\(')
    add_pattern = re.compile(r'info\.eval->add\(')
    sub_pattern = re.compile(r'info\.eval->sub\(')
    rotate_pattern = re.compile(r'info\.eval->rotate_rows\(')
    plain_multiply_pattern = re.compile(r'info\.eval->multiply_plain\(')

    # Find all matches
    multiplications = multiply_pattern.findall(cpp_code)
    additions = add_pattern.findall(cpp_code)
    substitution = sub_pattern.findall(cpp_code)
    rotations = rotate_pattern.findall(cpp_code)
    plain_multiplications = plain_multiply_pattern.findall(cpp_code)

    # Count the number of multiplications
    num_multiplications = len(multiplications)
    num_additions = len(additions)
    num_substitutions = len(substitution)
    num_rotations = len(rotations)
    num_plain_multiplications = len(plain_multiplications)

    print(f'Number of multiplications: {num_multiplications}')
    print(f'Number of additions: {num_additions}')
    print(f'Number of substitutions: {num_substitutions}')
    print(f'Number of rotations: {num_rotations}')
    print(f'Number of scalar/plain multiplications: {num_plain_multiplications}')

    multiply_pattern = re.compile(r'info\.eval->multiply\(([^,]+), ([^,]+), ([^)]+)\);')
    add_pattern = re.compile(r'info\.eval->add\(([^,]+), ([^,]+), ([^)]+)\);')
    sub_pattern = re.compile(r'info\.eval->sub\(([^,]+), ([^,]+), ([^)]+)\);')
    rotate_pattern = re.compile(r'info\.eval->rotate_rows\(([^,]+), [^,]+, [^,]+, ([^)]+)\);')

    dependencies = defaultdict(list)

    multiply_results = set()

    # Find all multiply operations
    for match in multiply_pattern.findall(cpp_code):
        var1, var2, result = match
        dependencies[result.strip()].extend([var1.strip(), var2.strip()])
        multiply_results.add(result.strip())
    

    # Find all add operations
    for match in add_pattern.findall(cpp_code):
        var1, var2, result = match
        dependencies[result.strip()].extend([var1.strip(), var2.strip()])

    # Find all sub operations
    for match in sub_pattern.findall(cpp_code):
        var1, var2, result = match
        dependencies[result.strip()].extend([var1.strip(), var2.strip()])

    for match in rotate_pattern.findall(cpp_code):
        var1, result = match
        dependencies[result.strip()].append(var1.strip())   


    # Function to calculate the depth of each variable
    def calculate_depth(var, memo):
        if var not in dependencies:
            return 0
        if var in memo:
            return memo[var]
        depth = 1 + max(calculate_depth(dep, memo) for dep in dependencies[var])
        memo[var] = depth
        return depth

    # Calculate the depth for all variables
    memo = {}
    max_depth = 0
    for var in dependencies:
        depth = calculate_depth(var, memo)
        max_depth = max(max_depth, depth)

    print(f'Depth of the circuit: {max_depth}')

    def calculate_multiplicative_depth(var, memo):
        if var not in dependencies:
            return 0
        if var in memo:
            return memo[var]
        
        # Calculate the depth of the dependencies
        depths = [calculate_multiplicative_depth(dep, memo) for dep in dependencies[var]]
        max_depth = max(depths) if depths else 0
        
        # Increment depth if the current variable is a result of a multiply operation
        if var in multiply_results:
            max_depth += 1
        
        memo[var] = max_depth
        return max_depth



    memo = {}
    max_multiplicative_depth = 0
    for var in dependencies:
        depth = calculate_multiplicative_depth(var, memo)
        max_multiplicative_depth = max(max_multiplicative_depth, depth)

    print(f'Multiplicative depth of the circuit: {max_multiplicative_depth}')

    return num_multiplications, num_additions, num_substitutions, num_rotations, num_plain_multiplications, max_depth, max_multiplicative_depth

# Set up command-line argument parsing
# parser = argparse.ArgumentParser(description='Evaluate the depth of a vector program.')
# parser.add_argument('benchmark', type=str, help='The name of the benchmark to evaluate')

# args = parser.parse_args()
# benchmark = args.benchmark

# evaluate(benchmark)