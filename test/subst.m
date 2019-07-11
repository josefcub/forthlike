( --[ subst.m ]---------------------------[ 10/10/2006 ]-- )
( subst [ s1 s2 s3 -- s ]                                  )
(                                                          )
(       Change all occurrences of s3 into s2 in string s1, )
(       and returns the changed string, s.  Implemented in )
(       MUF instead of C because it was easier.            )
( -----------------------------------------[ BlueDevil ]-- )

( --[ TinyMUCK Compatibility ]---------------------------- )
$def notify me @ swap \notify 

( --[ subst ]--------------------------------------------- )
: subst ( s1 s2 s3 -- s )

  ( Stack Depth and Item Type Check )
  depth 3 < if "SUBST: Stack underflow" abort then
  3 pick string? not if 
    "SUBST: Non-string argument (1)" abort then
  over string? not if 
    "SUBST: Non-string argument (2)" abort then
  dup string? not if 
    "SUBST: Non-string argument (3)" abort then
  
  ( Step 1 - remove s3 and segment the string )
  rot swap explode

  ( No segments, no match.  Clean up and exit. )
  dup 1 = if 
    pop pop pop exit 
  then

  ( Step 2 - Loop through and join all segments ) 
  (          with s3.                           )
  begin dup 1 > while 
    dup 2 + pick
    rot swap strcat rot strcat 
    swap 1 -
  repeat 

  ( Step 3 - Clean up after ourselves.          )
  rot pop pop 

;

( Tests the above. )
: test_subst
  "This_used_to_not_have_any_spaces_at_all." " " "_" 
  subst notify 
;
