# A test for EA-equivalence
Testing if two functions $F$ and $G$ are EA-equivalent through the relation $L_1 \circ F \circ A2 + A = G$. If such a 
relation is found, returns $L_1, A2, A$.

## What you need
- [GCC](https://gcc.gnu.org/) to compile the program, version 10 or above.

## How to build and run project:
To build the program, make `compile.sh` executable and run `./compile.sh` at the command line in the root directory of 
the project, which will generate an executable called `ea`.

For information on how to run the program, you can run `./ea -h` in the command line. Then you will get information on
how to run the program. It will look something like:

```text
EA-equivalence test
Usage: ea[ea_options] [filename_F] [filename_G]
Ea options:
	-h 	 - Print help
	-k 	 - Size of k
	-t 	 - Add this for printing running times for different functions.

	filename_F: path to the file of a function F
	filename_G: path to the file of a function G
```
The truth table file is expected to be of the format:

```text
6
0 1 8 15 27 14 35 48 53 39 43 63 47 41 1 1 41 15 15 47 52 6 34 22 20 33 36 23 8 41 8 47 36 52 35 53 35 39 20 22 33 34 48 53 39 48 6 23 22 33 63 14 23 52 14 43 27 63 36 6 27 43 20 34 
```

Example for testing for EA-equivalence between two functions $F$ and $G$.
```shell
./ea path/to/functionF path/to/functionG
  L1:
  6
  0 3 1 2 38 37 39 36 32 35 33 34 6 5 7 4 9 10 8 11 47 44 46 45 41 42 40 43 15 12 14 13 58 57 59 56 28 31 29 30 26 25 27 24 60 63 61 62 51 48 50 49 21 22 20 23 19 16 18 17 53 54 52 55

  A2:
  6
  0 52 50 6 40 28 26 46 4 48 54 2 44 24 30 42 43 31 25 45 3 55 49 5 47 27 29 41 7 51 53 1 39 19 21 33 15 59 61 9 35 23 17 37 11 63 57 13 12 56 62 10 36 16 22 34 8 60 58 14 32 20 18 38

  A:
  6
  0 50 59 9 35 17 24 42 6 52 61 15 37 23 30 44 60 14 7 53 31 45 36 22 58 8 1 51 25 43 34 16 2 48 57 11 33 19 26 40 4 54 63 13 39 21 28 46 62 12 5 55 29 47 38 20 56 10 3 49 27 41 32 18

```

The program will print the result to the command line. If the two functions $F$ and $G$ are EA-equivalent, the program will 
print out truth tables for $L_1, A_2, A$.

### Other executables 
When compiling `compile.sh` some other executables is created other than the `ea`. These can also be used:

- `eaFull` Runs the program as above, but first computes all possible $L_1$ before trying to find an inner permutation.
- `automorphism` Computes the automorphism group of a function F
- `order` Computes the order of a function F
- `partition` Partition a function F

All of these expect one argument, the function $F$.
```shell
EA-equivalence test - full version
Finds all outer permutations L1 first

Usage: eaFull [eaFull_options] [filename_F] [filename_G]
EaFull options:
	-h 	 - Print help
	-k 	 - Size of k
	-t 	 - Add this for printing running times for different functions.

	filename_F: path to the file of a function F
	filename_G: path to the file of a function G
```
```shell
Automorphism
Usage: automorphism [filename]

filename = the path to the file of a function F.
```
```shell
Order
Usage: order [filename]
filename = Path to file of partition of a function F.
```
```shell
Partition
Usage: partition [filename]
filename = Path to file of a function F.
```