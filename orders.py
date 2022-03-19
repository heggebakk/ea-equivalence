import os
import subprocess

dim = "dim8/classic"
dirs = os.listdir(f"./results/automorphism_group/{dim}/")

if __name__ == '__main__':
    for file in dirs:
        filename = f"./results/automorphism_group/{dim}/{file}"
        dest_name = f"./results/orders/{dim}/{file[:-4]}.txt"
        subprocess.run(["./group.out", '-t', filename, '-f', dest_name])
