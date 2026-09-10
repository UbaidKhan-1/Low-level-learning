# Dict Parser

A custom dictionary-style syntax parser written in C.

The parser takes a string containing key-value pairs, determines the type of each value, and stores the parsed data in a **heterogeneous hashmap**.

This project is part of my low-level C learning work and is an exercise in parsing, string processing, dynamic memory, and working with heterogeneous data in C.

---

## Syntax

The parser uses a simple dictionary-style syntax:

```text
{
    name: 'Carla',
    age: 19,
    occupation: 'teacher',
    isMarried: false
}
```

Each entry consists of a key and a value separated by `:` and entries are separated by commas.

---

## Supported Types

The parser currently supports three types:

- Strings
- Integers
- Booleans

### Strings

Strings are enclosed in single quotes:

```text
name: 'Carla'
occupation: 'teacher'
```

Whitespace inside strings is preserved:

```text
height: '5ft 7'
```

### Integers

Integers can be provided directly as values:

```text
age: 19
salary: 75000
```

### Booleans

Boolean values are represented using `true` and `false`:

```text
isMarried: false
hasCar: true
```

---

## Example

A dictionary can be provided as a C string:

```c
char dict[] = "{"
              "name: 'Carla',"
              "age: 19,"
              "occupation: 'teacher',"
              "salary: 75000,"
              "isMarried: false,"
              "height: '5ft 7',"
              "city: 'Lahore',"
              "languages: 'English',"
              "}";
```

The parser converts this into a heterogeneous hashmap.

Example output:

```text
age: 19
salary: 75000
height: 5ft 7
isMarried: false
name: Carla
occupation: teacher
city: Lahore
languages: English
```

The order of entries depends on the underlying hashmap implementation.

---

## How It Works

The parser processes the input string and extracts its individual key-value pairs.

For each entry it determines:

- The key
- The value
- The value's type

The resulting data is then inserted into the hashmap.

For example:

```text
age: 19
```

is interpreted as:

```text
key   -> age
value -> 19
type  -> int
```

While:

```text
name: 'Carla'
```

becomes:

```text
key   -> name
value -> Carla
type  -> string
```

And:

```text
isMarried: false
```

becomes:

```text
key   -> isMarried
value -> false
type  -> bool
```

---

## Whitespace Handling

Whitespace outside string literals is ignored.

For example:

```text
name : 'Carla'
```

is treated the same as:

```text
name:'Carla'
```

However, whitespace inside strings is preserved:

```text
height: '5ft 7'
```

remains:

```text
height: '5ft 7'
```

The parser keeps track of whether it is currently inside a string while processing the input.

---

## Hashmap

The parsed values are stored using a heterogeneous hashmap implemented separately in the repository.

The parser does not implement its own hashmap. Instead, it uses the existing hashmap implementation from:

```text
../DSA/Linear-Structures/Hash_Map.c
```

This allows the parser to focus on interpreting the input format while the hashmap handles storing the resulting data.

---

## Compilation

From inside the `Dict-Parser` directory, compile the parser with Clang:

```bash
clang DictParser.c ../DSA/Linear-Structures/Hash_Map.c -Wno-int-conversion -o main
```

Then run:

```bash
./main
```

`-Wno-int-conversion` is currently used to suppress an integer to void* conversion warning from the hashmap implementation.

---

## Current Limitations

The parser currently does not support:

- Nested dictionaries
- Arrays
- Floating-point numbers
- `null`

---

## Future Plans

The parser will be expanded as the project develops.

Planned features include:

- [ ] Nested dictionaries
- [ ] Arrays
- [ ] Floating-point numbers
- [ ] `null` values

The long-term goal is to develop this into a more capable custom structured-data format while learning more about parsing and low-level data representation in C.