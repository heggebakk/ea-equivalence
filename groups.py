import os
import subprocess

dim = "dim8/classic"
dirs = os.listdir(f"./results/permutations/{dim}/")

if __name__ == '__main__':
    for file in dirs:
        filename = f"./results/permutations/{dim}/{file}"
        dest_name = f"./results/groups/{dim}/{file[:-4]}.txt"
        subprocess.run(["./group.out", '-t', filename, '-f', dest_name])
