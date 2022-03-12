import logging
import os
import subprocess
import threading

dim = "dim8/classic"
dirs = os.listdir(f"./resources/TT_library/{dim}")


def permute(name):
    logging.info("Thread %s: starting", name)
    filename = f"./resources/TT_library/{dim}/{name}"
    dest_name = f"./results/permutations/{dim}/{name[:-3]}.txt"
    subprocess.run(["./main.out", '-t', filename, '-f', dest_name, '-o'])
    logging.info("Thread %s: finishing", name)


if __name__ == '__main__':
    time_format = "%(asctime)s: %(message)s"
    logging.basicConfig(format=time_format, level=logging.INFO, datefmt="%H:%M:%S")
    for file in dirs:
        x = threading.Thread(target=permute, args=(file,))
        x.start()
        logging.info("%s done", file)
