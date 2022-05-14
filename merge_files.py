import os

files = os.listdir("results/computationally_testing/dim10_5/")

count = 0
for f in files:
    with open(f"results/computationally_testing/dim10_5/{f}", "r") as file:
        count += float(file.readline()) * 10
print(f"10.5 : {count / 10}")


files = os.listdir("results/computationally_testing/dim10_6/")

count = 0
for f in files:
    with open(f"results/computationally_testing/dim10_6/{f}", "r") as file:
        count += float(file.readline()) * 10
print(f"10.6 : {count / 10}")
