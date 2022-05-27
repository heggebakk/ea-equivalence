import os
import subprocess

dim = "dim10"
dirs = os.listdir(f"./resources/TT_library/{dim}/")

for file in dirs:
    filename = f"./resources/TT_library/{dim}/{file}"
    dest_name = f"./results/partitions/{dim}/{file[:-3]}.txt"
    subprocess.run(["./ea", '-t', filename, '-f', dest_name, '-p'])
