#Grammar - Reference

## Variables
### int, long, byte
1. int age{19};
2. int age = 19;

### floating point - single & double
1. float pi{3.141f}; // single precision
2. float pi{3.141d}; // double precision

- single precision has f at the end and double precision has d at the end.

## Comments
### Single-line comments
1. -- This is a single line comment
2. `#` This is a comment too // starts with hashtag
3. // This is a comment too

### Multi-line comments
1. /* This is a multi Line comment */

### Documentation comment - Similar to java
1. /** This is a docs comment */

## Data-Types
1. Integers:
    - type-keyword: `int`
    - maximum-value: self.MAX
           - usage: `int maximumValue{self.MAX};`
    - minimum-value: self.MIN
           - usage: `int variable-name{self.MIN};`

2. Floating points:
    - type-keyword: `float`
    - maximum-value: self.MAX.f for 32 bit and self.MAX.d for 64 bit
       - usage: `float max{self.MAX.f};` or `float max{self.MAX.d}`;
    - minimum-value: self.MIN.f for 32 bits and self.MAX.d for 64 bits
       - usage: `float min{self.MIN.f};` or `float min{self.MIN.d};`

    - NOTE: There is no restriction for choosing to use small letter d or caps D for floating point - both will yield the same results and goes the same for f and F.

3. char* / strings
   - type-keyword: `char*`
   - usage: `char* name{};` // value store will be null or `char* name{"Hlulani"};` // value is Hlulani
   - disclaimer: unfortunately it supports double quotes only.


i prefer using resolution operator `::` to access stuff like function calls from a certain library
i.e accessing stuff within system use `sys::<name>` this goes the same with accessing stuff within a class ..etc

## importing
```hlx
import::<name>
```

with aliasing

```hlx
import::<name> as <your aliasing here but with no space>
```
