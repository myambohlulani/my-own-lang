# Custom Language grammar - Reference

## Variables
```
    <- whole numbers ->
    i32 age = 19;
    i64 big_number = 3147483647;

    <- alpha and or alpha-numberic ->
    string name = "Hlulani"

    <- booleans ->
    bool loves_coding = true;
    bool is_raining = false;
    bool isCloudy = true;

    <- floating point precision ->
    f32 PI = 3.1415926;
    f64 pi = 3.141592653589793;
```

## commenting
```
    <-single line comment ->
    # This is a single line comment
    or
    // so is this!

    <- multiple line comment ->
    /*
    * this is a comment
    */

    <-Documentation comment ->
    /**
    *   This comment is for documenting your code.
    */
```

## unary operators
```
    i32 age = 19;
    print(age); # 19

    age++; # post-increment
    print(age); // 20

    age--; # post-decrement
    age--;
    print(age); // 18

    age = 19;
    print(++age); # 20 // pre-increment

    age = 19;
    print(--age); # 18

    print(age++); # 18
    print(age--); #19

    print(age); #18;

```

## If - statements

```
    if (age > 18) : then {
        print("Too old to be joining the club!");
    } elif (age < 18) : then {
        print("Too young to be joining the club!");
    } else {
        printf("Hello {name}, Welcome to the club!");
    } fi;
```

## Arrays

```
    i32[] array = {1, 2, 3, 4};
    // accessing elements
    print(array[0]);

    // changing elements
    array[0] = 10;
    print(array); // [10, 2, 3, 4]

    // length of array
    i32 length = len(array); // 4
```

## forEach - loop
```
    i32[] arr = {10, 20, 30, 40, 50};

    for (i32 number: arr) : do {
        printf("Your number is {number}!");
    } done;
```

## for - loop
```
    i32[] arr = {10, 20, 30, 40, 50};

    for(i32 i = 0; i < len(arr); i++) : do {
        printf("i = {i}");
    } done;
```

## while loop
```
    bool isCold = true;
    i32 count = 0;

    while (isCold) : do {
        if (count == 10) {
            print("Done counting!");
            break;
        }

        counter++;
    } done;

    - while loop supports: break, continue, pass just like any loop
```

## switch statement
```
    string name = "Hlulani";
    switch (name) : do {
        case ("Myambo") {
            print("Hello, Welcome!");
        } case ("Hlulani" | "Hlulie") {
            printf("Hello there {name}");
        } case (*) {
            print("This is the default statement");
        }
    } done;
```

## Functions
```
    public get_name() -> (string | null) {
        return name;
    }

    public set_name(string name) -> void {
        this.name = name; // this keyword is only used in the class.
    }

    public _value(string value) -> string {
        return "Hello, {!value := "World!"};
    }

    private helper() -> void {
        // body
    }

    protected will_be_inherited() -> void {
        // body
    }****
```
