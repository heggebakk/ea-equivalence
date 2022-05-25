import time
import logging
import os
import subprocess
import threading

dim = "dim6"
dirs = os.listdir(f"../TT_library/{dim}")


def ea(name):
    filename = f"../TT_library/{dim}/{name}"
#    dest_name = f"results/hybrid/{dim}/{name}"
    subprocess.run(["./hybrid", filename, "-t"])


if __name__ == '__main__':
    time_format = "%(asctime)s: %(message)s"
    logging.basicConfig(format=time_format, level=logging.INFO, datefmt="%H:%M:%S")
    start = time.perf_counter()
    threads = list()
    for file in dirs:
        for i in range(5):
            x = threading.Thread(target=ea, args=(file,))
            threads.append(x)
            x.start()
        for thread in threads:
            thread.join()
    end = time.perf_counter()
    print(end-start)
