# Tree Expression Calculator (C)

A small C project that parses arithmetic expressions into an Abstract Syntax Tree (AST), evaluates the result, and can print the tree structure.

## Features

- Parses infix expressions with:
  - Numbers (integer/decimal)
  - `+`, `-`, `*`, `/`
  - Parentheses `(` `)`
- Builds an AST for the expression
- Evaluates the AST recursively
- Prints the AST in a readable tree form using branch characters (`├──`, `└──`)
- Simple interactive CLI flow:
  - Prompt for expression
  - Show result
  - Ask whether to print the expression tree (`y/n`)

## Project Structure

- `main.c` — CLI interaction (input, result display, optional tree printing)
- `calc.c` — Lexer, parser, AST evaluation, AST printing, memory cleanup
- `calc.h` — Public types and function declarations
- `Makefile` — Build rules

## clone the repository

```bash
git clone https://github.com/yourusername/ex_calc_C.git
cd ex_calc_C
```

## Build

From the project root:

```bash
make
```

This produces the executable:

- `calculator`

## Run

```bash
./calculator
```

You will be prompted for an expression, then asked whether to print the tree.

Then enter an expression such as:

- `2+3*4`
- `(10-4)/3`
- `7*(2+5)-1`

After the result is shown, answer `y` to print the expression tree or `n` to skip.

## Example (interactive)

```text
Expression Tree Calculator
Enter expression: 2+3*4
Result: 14
Print expression tree? (y/n): y
└── +
    ├── 2
    └── *
        ├── 3
        └── 4
```

## Clean Build Artifacts

```bash
make clean
```

## Notes / Current Limitations

- Parser does not explicitly reject every trailing invalid sequence after a valid prefix (for example, `2+3abc` is currently accepted and evaluated as `2+3`).
- Division-by-zero behavior follows C floating-point rules (no custom error handling yet).
- Unary operators (for example `-5+2`) are not implemented as a dedicated grammar rule and currently produce a parse error.

## License

MIT
