# ADA Challenge !

### File & Directory

* **public-test-cases**

  A place for public test cases

* **validator**

  A place for TA's checker

* **best-answer**

  A place to store the best output so far ?

* **Others (just-random, ...)**

  "Maybe" each for a different kind of method to solve the problem

### checker

A program that checks both your program output and the testcases you generate is provided together with the public testcases. Note that for the public testcases, you may have to use the flag `--public` to loosen some restrictions.

``` sh
# Build the validator
make
# Verifies if the testcase 00.in satisfies the private contraints
./checker 00.in
# Verifies if the testcase 00.in satisfies the private contraints
# and calculate the metric for the output 00.out
./checker 00.in 00.out
# Verifies if the testcase 00.in satisfies the public contraints
# and calculate the metric for the output 00.out
./checker --public 00.in 00.out
```

### Goal

Just enjoy(?