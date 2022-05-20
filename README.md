# ea-equivalence
This is an efficient implementation of a test for EA-equivalence between two functions $F$ and $G$.

## What you need
- [GCC](https://gcc.gnu.org/) to compile the program, version 10 or above.

## How to build and run project:
To build the program, make `compile.sh` executable and run `./compile.sh` at the command line in the root directory of 
the project, which will generate an executable called `ea`.

For information on how to run the program, you can run `./ea -h` in the command line. Then you will get information on
how to run the program. It will look something like:

```text
EA-equivalence test
Usage: ea [ea_options] [filename_F] [filename_G]
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
```

The program will print the result to the command line. If the two functions $F$ and $G$ are EA-equivalent, the program will 
print out truth tables for $L_1, A_2, A$.

