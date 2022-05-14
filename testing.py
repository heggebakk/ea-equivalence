# We want to test the computational results for Kaleyski's algorithm.
# By running the program 10 times, we get the average running times.
import logging
import subprocess
import time
import threading

filenames = ["q_10_5.tt", "q_10_6.tt"]
root = "../TT_library/dim10/"
count = 10


def ea(name, i):
    destination = f"results/computationally_testing/dim10/{name}"
    start = time.perf_counter()
    subprocess.run(["./ea", "-t", f"{root}{name}", "-f", f"{destination}_{i}"])
    end = time.perf_counter() - start
    wf = open(f"results/computationally_testing/runtime_dim10_{name}_{i}.txt", "w+")
    wf.write(f"{end}")
    wf.close()


if __name__ == '__main__':
    time_format = "%(asctime)s: %(message)s"
    logging.basicConfig(format=time_format, level=logging.INFO, datefmt="%H:%M:%S")
    threads = list()
    for f in filenames:
        for i in range(count):
            x = threading.Thread(target=ea, args=(f, i,))
            threads.append(x)
            x.start()

    for thread in threads:
        thread.join()
