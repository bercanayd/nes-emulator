import os
import re

def find_opcode_functions_in_file(filepath):
    functions = []
    pattern = re.compile(r'\bvoid\s+(opcode_\w+)\s*\(')
    with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
        for line in f:
            match = pattern.search(line)
            if match:
                functions.append(match.group(1))
    return functions

def main():
    opcode_map = {}
    for filename in os.listdir('.'):
        if filename.endswith('.c'):
            funcs = find_opcode_functions_in_file(filename)
            for func in funcs:
                opcode_map[func] = filename

    with open('opcode_functions.txt', 'w') as out_file:
        for func, filename in sorted(opcode_map.items()):
            out_file.write(f"{func}: {filename}\n")

    print(f"Found {len(opcode_map)} opcode functions. Written to opcode_functions.txt")

if __name__ == '__main__':
    main()
