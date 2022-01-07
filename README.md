# ea-equivalence
ea-equivalence is an implementation from the paper `Deciding EA-equivalence via invariants` by Nikolay Kaleyski.

## Build and run project:
To build the project, run `comp.sh` at the command line in the current directory, which will generate an executable called `main.out` by default.

The program expects two arguments, function f and function g. To run the program with function
f = `resources/TT_library/dim6/q_6_1.tt` and function g = `resources/TT_library/dim6/q_6_1.tt`

```shell
./main.out resources/TT_library/dim6/q_6_1.tt resources/TT_library/dim6/q_6_1.tt
```

The two files f and g expect to be in this current format:

```text
6
 0 1 8 15 27 14 35 48 53 39 43 63 47 41 1 1 41 15 15 47 52 6 34 22 20 33 36 23 8 41 8 47 36 52 35 53 35 39 20 22 33 34 48 53 39 48 6 23 22 33 63 14 23 52 14 43 27 63 36 6 27 43 20 34 
```

The first line is the dimension, and the second is the truth table.