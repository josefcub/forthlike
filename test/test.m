( -------------------------------------------------[03-10-2002]- )
(   TEST.F - Tests functions of the interpreter for proper op-   )
(            eration.  The date shown above each word is the     )
(            date the routine was last modified.                 )
( ------------------------------------------------[ BlueDevil ]- )

$include include.m       ( Include things useful to FLL programs. )

( -[Support Routine]-------------------------------------------- )
( newline - Prints out a blank space, creating a blank line.     )
(           It's also a good test of the notify primitive.       )
( -------------------------------------------------------------- )
: newline ( -- )
  " " notify
;

( -[Miscellaneous]---------------------------------------------- )
( test_misc - Tests the various primitives that don't fit into   )
(             any other category.                                )
( -------------------------------------------------------------- )
: test_misc ( -- )

  ( greeting )
  newline
  "-[ Miscellaneous ]----------------------------------" notify

  ( $include )
  includedword

  ( version )
  "VERSION: The interpreter is version " 
  version strcat notify

  ( time and date )
  time "TIME: It is now " 
  swap intostr strcat ":" strcat
  swap intostr strcat ":" strcat
  swap intostr strcat "." strcat notify

  date "DATE: Today is " 
  swap intostr strcat "/" strcat
  swap intostr strcat "/" strcat
  swap intostr strcat "." strcat notify

  ( sleep )
  "SLEEP: Sleeping for 3 seconds." notify
  3 sleep

;

( -[String Manipulation]---------------------------------------- )
( test_strings - Tests the string manipulation primitives curr-  )
(                ently supported.                                )
( -------------------------------------------------------------- )
: test_strings ( -- )

  ( greeting and information request )
  newline
  "-[ String Manipulation ]----------------------------" notify
  "What is your name?" notify
  read

  ( strcat )
  dup "STRCAT: Pleased to meet you, " "." rot swap
  strcat strcat notify
  
  ( strlen and intostr )
  dup strlen intostr
  " characters long." strcat
  "STRLEN: Your name is " swap strcat notify
  
  ( toupper )
  dup toupper "TOUPPER: All uppercase: " swap strcat notify
  
  ( tolower )
  dup tolower "TOLOWER: All lowercase: " swap strcat notify
  
  ( string? )
  dup string? intostr
  "STRING?: Is it a string? " swap strcat notify
  
  ( stringcmp )
  dup dup stringcmp intostr
  "STRINGCMP: Compared against itself: " swap strcat notify
 
  dup "Bob" stringcmp intostr
  "STRINGCMP: Compared against 'Bob': " swap strcat notify
  
  ( strcmp )
  dup dup strcmp intostr
  "STRCMP: Case-sensitive compared against itself: " swap strcat notify
 
  dup "Bob" strcmp intostr
  "STRCMP: Case-sensitive compared against 'Bob': " swap strcat notify
  
  ( strncmp )
  dup dup 2 strncmp intostr
  "STRNCMP: First two letters compared against itself: " swap strcat notify
 
  dup "Bob" 2 strncmp intostr
  "STRNCMP: First two letters compared against 'Bob': " swap strcat notify
  
  ( striplead )
  dup "STRIPLEAD: Before striplead: '" swap "'" strcat strcat notify
  dup striplead
  "STRIPLEAD: Stripped of leading spaces: '" swap "'" strcat strcat notify

  ( striptail )
  dup "STRIPTAIL: Before striptail: '" swap "'" strcat strcat notify
  dup striptail
  "STRIPTAIL: Stripped of tailing spaces: '" swap "'" strcat strcat notify

  ( strip )
  dup "STRIP: Before strip: '" swap "'" strcat strcat notify
  dup strip
  "STRIP: Stripped of lead and tail spaces: '" swap "'" strcat strcat notify

  ( strcut ) 
  dup 3 strcut
  "STRCUT: Your name cut at position 3: '" rot strcat "' and '" 
  rot strcat strcat "'" strcat notify
  
  ( stringpfx )
  dup "Bob" stringpfx intostr
  "STRINGPFX: Is your first name 'Bob': " swap strcat notify

  ( explode )
  " " explode
  intostr "EXPLODE: How many names you gave me: " swap strcat notify

  ( leave the stack as we found it )
  begin depth while pop repeat

;

( -[Integer Manipulation]--------------------------------------- )
( test_ints - Tests the integer manipulation primitives current- )
(             ly supported.                                      )
( -------------------------------------------------------------- )
: test_ints ( -- )

  ( greeting and information request )
  newline
  "-[ Integer Manipulation ]---------------------------" notify
  "Enter a number:" notify
  read

  ( number? )
  dup number? intostr
  "NUMBER?: Is the entered string a number?: " swap strcat notify

  ( atoi and int? )
  dup atoi int? intostr
  "ATOI/INT: Is the atoi'd result an integer?: " swap strcat notify

  ( Arithmetic operators )
  atoi
  dup 1 + intostr
  "+: One added: " swap strcat notify
  dup 1 - intostr
  "-: One subtracted: " swap strcat notify
  dup 2 * intostr
  "*: Times two: " swap strcat notify
  dup 2 / intostr
  "/: Divided by two: " swap strcat notify
  dup 2 % intostr
  "%: Remainder of above: " swap strcat notify

  ( Relational operators )
  dup 10 > intostr
  ">: Is it greater than 10?: " swap strcat notify
  dup 10 >= intostr
  ">=: Is it greater than or equal to 10?: " swap strcat notify
  dup 10 < intostr
  "<: Is it less than 10?: " swap strcat notify
  dup 10 <= intostr
  "<=: Is it less than or equal to 10?: " swap strcat notify
  dup 10 = intostr
  "=: Is it equal to 10?: " swap strcat notify

  ( Logic operators: AND )
  dup 10 and intostr
  "AND: Your number AND 10: " swap strcat notify
  dup 0 and intostr
  "AND: Your number AND 0: " swap strcat notify
  
  ( Logic operators: OR )
  dup 10 or intostr
  "OR: Your number OR 10: " swap strcat notify
  dup 0 or intostr
  "OR: Your number OR 0: " swap strcat notify

  ( Logic operators: NOT )
  dup not intostr
  "NOT: NOT your number: " swap strcat notify
  0 not intostr
  "NOT: NOT 0: " swap strcat notify

  ( random )
  "RANDOM: A random number: " 
  random intostr strcat notify

  ( Clean up after ourselves )
  pop
;

( -[Flow Control]----------------------------------------------- )
( test_flow - Tests flow control primitives.                     )
( -------------------------------------------------------------- )
: test_flow ( -- )
  ( greeting and information request )
  newline
  "-[ Flow Control ]-----------------------------------" notify
  "Please enter a number." notify
  read
  
  ( if / else / then )
  dup number? if
    "IF: That is indeed a number." notify
  else
    "IF-ELSE:" notify
    ( abort )
    "We can't finish the test without a number" abort
  then

  ( begin / while / repeat )
  "BEGIN: Counting from your number down to zero." notify
  atoi begin
    dup intostr "WHILE: " swap strcat notify
    1 - dup 0 >= while
  repeat
  
  ( don't need the user's number anymore )
  pop

  ( begin / until - nested )
  "Nested BEGINs: Five sets of ten." notify
  5 begin          
    10 " " begin   
      swap dup intostr      
      rot strcat
      swap 1 - swap 
    over not until
    "2nd UNTIL: " swap strcat notify
    pop 1 -                        ( remove the extra int first )
  dup not until
  
  ( exit )
  "EXIT:" notify
  exit
  "There is a problem with the interpreter." notify
;


( -------------------------------------------------------------- )
( main - Write the support code here.                            )
( -------------------------------------------------------------- )
: main ( -- )
  "----------------------------------------------------" notify
  "  TEST.F - Interpreter testing routines." notify
  "----------------------------------------------------" notify
  test_misc 
  test_strings 
  test_ints 
  test_flow 
  "----------------------------------------------------" notify
  "Done." notify
;
