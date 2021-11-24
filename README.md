# ea-equivalence
This is an implementation from the paper `Deciding EA-equivalence via invariants` by Nikolay Kaleyski.

## Build and run project:
To build the project, simply run `comp.sh` at the commandline in the current directory.

By default, this will generate an executable called `main.out`. 

The program expects two arguments, function f and function g. To run the program with function 
f = `resources/TT_library/dim6/q_6_1.tt` and function g = `resources/TT_library/dim6/q_6_1.tt`

```shell
./main.out resources/TT_library/dim6/q_6_1.tt resources/TT_library/dim6/q_6_1.tt
```