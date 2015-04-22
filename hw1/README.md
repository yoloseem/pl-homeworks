# Programming Languages - Homework 1

Write C/C++ functions that builds and runs a finite state automaton.

- Skeleton codes together with simple test cases are provided with the assignment.
- DO NOT change the `fsa_main.cc` file. It will be the main file for testing and grading.

1.1. Write C/C++ functions : a function that loads the deterministic finite state automaton (DFA) from a file, and a function that runs it on input strings and returns the acceptance (true/false). [60pts]
- The DFA input file structure is the accept states at the first line, followed by (state, input_char, next_state) tuples, for example,

```
1
1 a 2
1 b 1
2 a 1
2 b 2
```

- Design the `FiniteStateAutomaton` structure in `fsa.h`.
- Implement the BuildFSA function that loads the given DFA definition into the `FiniteStateAutomaton` structure.
- Implement the `RunFSA` function so that it returns the acceptance of the given string.

1.2. Write a C/C++ function that builds DFA from a non-deterministic finite state automaton (NFA) definition. [40pts]

- The NFA input file has basically same structure with DFA files, except that the input character can be *epsilon* (`#` in the text file), and there can be more entries for single table cells.

```
3 4
1 # 3
1 a 2
2 b 2
2 b 4
3 # 2
3 a 4
4 a 3
```

- Implement the `BuildFSA` function so that it can process both DFA and NFA definitions.

### Due: Apr 24 (Fri) 11:59 pm

– Zip the source code (ONLY `.h`, `.cc` and `Makefile`; absolutely no executable or object files) and submit it in ezhub (portal).
– The program must run on the Linux server (`csedev.hanyang.ac.kr`).
