# ea-equivalence
ea-equivalence is an implementation from the paper `Deciding EA-equivalence via invariants` by Nikolay Kaleyski.

## Build and run project:
To build the project, run `comp.sh` at the command line in the current directory, which will generate an executable called `main.out` by default.

The program uses flags to run the program. Use `-h` for help.

Flags:
```text
-h Help
-f Filename for writing to a file. Default value: result.txt
-k Size of the tuple T. Default value: 4
-t Truth table file
-p Use for finding the partitions and write them to a file.
```

Example:
```shell
./main.out -f ./results/dim6/q_6_1.txt -k 6 -t resources/TT_library/dim6/q_6_1.tt 6
```

The truth table file is expected to be in the format:

```text
6
0 1 8 15 27 14 35 48 53 39 43 63 47 41 1 1 41 15 15 47 52 6 34 22 20 33 36 23 8 41 8 47 36 52 35 53 35 39 20 22 33 34 48 53 39 48 6 23 22 33 63 14 23 52 14 43 27 63 36 6 27 43 20 34 
```

The first line is the dimension, and the second is the truth table.