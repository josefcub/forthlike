( -------------------------------------------------[10-08-2006]- )
( INCLUDE.M - Tests the $include directive and contains handy    )
(             things every FLL program should have.              )
( ------------------------------------------------[ BlueDevil ]- )

$def notify me @ swap \notify      ( These are for TinyMUCK com- )
$def dump                          ( patibility.                 )

( -[ Included Word ]---------------------------------------------)
(     This is just a test to ensure that $includes actually work )
(     as they should.                                            )
( -------------------------------------------------------------- )

: includedword
  "$INCLUDE: The $include directive is working properly." notify
;
