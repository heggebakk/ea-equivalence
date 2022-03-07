import os
import subprocess

dirs = os.listdir("./resources/TT_library/dim8/")

for file in dirs:
    filename = f"./resources/TT_library/dim8/{file}"
    dest_name = f"./results/partitions/dim8/{file[:-3]}.txt"
    subprocess.run(["./main.out", '-t', filename, '-f', dest_name, '-p'])
