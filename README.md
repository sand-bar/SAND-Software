# BAT-Software

Software implementations of BAT block ciphers

## The bitsliced implementations of BAT and SIMON

We provide our bitsliced implementations of BAT and SIMON (may still can be improved), and do the benchmarks on both Linux and Windows platforms. The speeds are tested for 2MB messages and the detail results are listed below.

### The benchmarks on Windows:

**Environments:** Windows 7; i7-6700; DDR4 2400 8G; Devcpp + mingwGCC 4.9.2

| Cipher      | Parallel Way | Speed (cpb) | Cipher        | Parallel Way | Speed (cpb) |
|:-----------:|:-------------:|:-----------:|:-------------:|:-------------:|:-----------:|
| BAT-64/128  | 32            | 2.30        | SIMON-64/128  | 32            | 1.98        |
| BAT-64/128  | 64            | 1.93        | SIMON-64/128  | 64            | 1.79        |
| BAT-128/128 | 16            | 2.46        | SIMON-128/128 | 16            | 2.81        |
| BAT-128/128 | 32            | 2.34        | SIMON-128/128 | 32            | 2.49        |

For the implementations in SIMON-SPECK-SUPERCOP [1], we use the codes and get the results below.

| Cipher        | Speed (cpb) |
|:-------------:|:-----------:|
| SIMON-64/128  |    2.27     |
| SIMON-128/128 |    3.45     |

### The benchmarks on Ubuntu:

**Environments:** Ubuntu 18.04; i7-6700; DDR4 2400 8G; GCC 4.9.2

| Cipher      | Parallel Way | Speed (cpb) | Cipher        | Parallel Way | Speed (cpb) |
|:-----------:|:-------------:|:-----------:|:-------------:|:-------------:|:-----------:|
| BAT-64/128  | 32            | 2.31        | SIMON-64/128  | 32            | 1.94        |
| BAT-64/128  | 64            | 1.97        | SIMON-64/128  | 64            | 1.68        |
| BAT-128/128 | 16            | 2.52        | SIMON-128/128 | 16            | 2.61        |
| BAT-128/128 | 32            | 2.29        | SIMON-128/128 | 32            | 2.41        |


For the implementations in SIMON-SPECK-SUPERCOP [1], we test the codes and get the results below.

| Cipher        | Speed (cpb) |
|:-------------:|:-----------:|
| SIMON-64/128  |   2.29      |
| SIMON-128/128 |   3.40      |


## References

[1] [SIMON-SPECK-SUPERCOP](https://github.com/nsacyber/simon-speck-supercop)
