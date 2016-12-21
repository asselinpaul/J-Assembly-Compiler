##A simple [J](https://en.wikipedia.org/wiki/J_(programming_language)) to LC4 Assembly compiler

Uses the same calling convention as *lcc*, enabling J programs to call subroutines compiled by *lcc* and vice versa.

Compile using clang 💥 with ```make jc```

Running ```./jc foo.j``` will produce *foo.asm*

###Functionality Implemented:
- Decimal and Hex Literals
- Arithmetic Operations (*+ - * / %*)
- comparisons (*lt, le, eq, ge, gt*  (<, <=, ==, >=, > respectively))
- logical operations (*and, or, not*)
- stack operations (*drop, dup, swap, max*)
- if statements (*if, else, endif*; supports nesting!)
- comments
- functions

```
; a function to compute the square of top of the stack value 

defun square
 arg1 ; fetch input argument (n) from below FP, RA, and RV slots 

dup* ;(n*n)
 return ; copy n*n into RV slot and return 
```
once defined, a function can be called as such:
```
4 square ; stack state after call: 4 16
```

⚠️  No error handling, expects a well-formed J program.