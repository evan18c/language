# E Programming Language
E is a procedural, statically typed language written by me.

## Compilation
E is currently transpiled into C.

## Example Code
This is a factorial demo:
```
map fact(x:i32) -> i32 {
    if (x == 1) {
        ret 1;
    }
    ret x * fact(x - 1);
}

map main() -> void {

    x:i32 = fact(5);

    printf("5! = %d\n", x);

}
```

## Types
The language currently supports 11 unique types:
`i64 i32 i16 i8 f64 f32 f16 f8 str bool void`

## Keywords
The following keywords are reserved:
`map ret -> if while else bus stop`