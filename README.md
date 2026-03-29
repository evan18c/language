# E Programming Language
E is a procedural, statically typed language written by me.

## Compilation
E is currently transpiled into C.

## Example Code
This code prints "hi" 10 times to the console. Alongside a factorial function.
```
map S(x:i32) -> i32 {
    ret x + 1;
}

map main() -> i32 {

    x: i32 = 0;

    while (x < 10) {
        printf("hi");
        x = S(x);
    }

    ret 0;
}

map fact(x:i32) -> i32 {
    if (x == 1) {
        ret 1;
    }
    ret x * fact(x - 1);
}
```

## Types
The language currently supports 10 unique types:
`i64 i32 i16 i8 f64 f32 f16 f8 str bool`

## Keywords
The following keywords are reserved:
`map ret -> if while`