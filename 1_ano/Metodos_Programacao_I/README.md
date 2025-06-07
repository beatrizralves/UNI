# Id Codes
This project implements an algorithm in C that reads a sequence of lowercase letters (up to 50 characters) representing a code.
For each input, it prints the next lexicographical permutation of the code, or "No Successor" if the code is already the last possible permutation for that set of characters.


### How it works

The program reads multiple lines of input (each containing a code) until it encounters the `#` character. For each code, it applies the **next permutation** algorithm, rearranging the letters to generate the next code in lexicographical order if possible. If there is no valid successor, it prints "No Successor".

### Example

**Input**

```
abaacb
cbbaa
#
```

**Output**

```
ababac
No Successor
```

