import time
import logging
import os
import subprocess
import threading

dim = "dim10"
dirs = os.listdir(f"../TT_library/{dim}")


def ea(name):
    filename = f"../TT_library/{dim}/{name}"
    subprocess.run(["./ea", filename, "-t"])


if __name__ == '__main__':
    time_format = "%(asctime)s: %(message)s"
    logging.basicConfig(format=time_format, level=logging.INFO, datefmt="%H:%M:%S")
    start = time.perf_counter()
    threads = list()
    for file in dirs[0]:
        for i in range(10):
            x = threading.Thread(target=ea, args=(file,))
            threads.append(x)
            x.start()
        for thread in threads:
            thread.join()
    end = time.perf_counter()
    print(end-start)
