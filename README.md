# ExpressionsErrorFinder



## Description:

ExpressionsErrorFinder reads in a text file that consists of a token (token name and respective regular expression), as well as an input string that is checked whether or not it is part of the language.

The following grammar was used to construct the parser:

input → tokens_section INPUT_TEXT
tokens_section → token_list HASH
token_list → token
token_list → token COMMA token_list
token → ID expr
expr → CHAR
expr → LPAREN expr RPAREN DOT LPAREN expr RPAREN
expr → LPAREN expr RPAREN OR LPAREN expr RPAREN
expr → LPAREN expr RPAREN STAR
expr → UNDERSCORE

The program checks for syntax errors (both in the RE and everywhere else), semantic errors, as well as whether any of the tokens can generate an empty string when passing in Epsilon.

## Approach

First, the parser had to work for this project as it would allow the construction of the graphs that represent the regular expressions. This project uses multiple different complex data structures that range from sets, vectors, and custom structs to the data structure containing the graphs representing a regular expression.
