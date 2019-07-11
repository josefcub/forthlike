# forthlike

A Forth-like language interpreter written in ANSI C.

This code is what resulted when I wanted to learn to code in C, shortly after the turn of the century in 2002.  Not to be content with merely practicing with "Hello, World!", I decided the best way was to write something I could use.  The result is this CLI version of the "MUF" Forth-like programming language as made famous by TinyMUCK/Fuzzball and other text-based multiplayer games of the era.

This code was known to compile on MS-DOS (using Turbo C++ 3.0), or Linux (GCC, various flavors and versions).  I verified that it still compiles clean (with -pedantic) in 2019, on 64-bit Ubuntu using gcc 7.4.0 but have not run the test programs to verify operation.

I have included the last revisions of the internal text documentation, including the original README.txt.

## Known Issues

This code is known to have broken time and timing primitive issues, due to changes in the C/stdlib data structures around it.  This has not been patched yet.

This code does not represent best practice.  Using tokenization and function pointers would greatly increase the efficiency of the interpreter, something that I did not know how to utilize at the time.

Caveat Emptor and have fun!

