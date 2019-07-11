Basic Instructions:

     Check the files dos/makefile (if under DOS) or src/makefile (if 
 under Linux) to change anything that you may need to in order for 
 the interpreter to compile.  The interpreter has been successfully
 compiled under MS-DOS with Borland Turbo C++ 3.0, and under Linux
 with egcs-2.95.x and GCC 4.0.

     The code is (hopefully) 100% ANSI C code, and the interpreter is 
 only limited by the amount of RAM of the target computer.  In Linux, 
 this is more especially true.  Under MS-(PC- or DR-)DOS, the default 
 memory model is huge, to provide the maximum space for operation.

     To compile the interpreter, first check the makefile for your
 system, and change what may need to be changed.  Second, in the same
 directory as your necessary makefile, type 'make install' and after
 the compilation, check in the ../bin directory for the executables.

 MINIMUM REQUIREMENTS - MS-DOS: 256k RAM, monochrome display, one
                        disk drive of any sort.  The more RAM, the
                        bigger that programs can be and still run.

                        Linux: If it'll run Linux, it oughta run FLL
                        just fine.

 Directories:

     ${ROOT}/bin - Where the finished binaries go with 'make install'.
    ${ROOT}/docs - Supplementary documentation for the interpreter.
     ${ROOT}/dos - DOS compilation and configuration files.
 ${ROOT}/include - The include files for the source code.
     ${ROOT}/src - The actual source code for the interpreter.
    ${ROOT}/test - Test scripts for the interpreter.

 Documentation:
     
         ./README - This file.
    docs/bugs.txt - If present, any currently known bugs in the source.
 docs/differs.txt - Differences between FLL and MUCKer Forth (the 
                    inspiration for FLL).
   docs/goals.txt - Current and past goals and milestones in development.
docs/overview.txt - An overview on how the interpreter and its particular
                    functions works.
   docs/prims.txt - List of currently supported primitives.

