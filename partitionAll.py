import os
import subprocess

dirs = os.listdir("./resources/TT_library/dim6/")

for file in dirs:
    filename = "./resources/TT_library/dim6/" + file
    dest_name = "./results/partitions/dim6/" + file[:-3] + ".txt"
    subprocess.run(["./main.out", '-t', filename, '-f', dest_name, '-p'])
