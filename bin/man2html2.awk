#!/usr/bin/gawk

$0 ~ "</pre><H2>SEE ALSO</H2>" {
  print $0
  getline
  while ( $0 !~ "^<P>$" && $0 !~ "^[ \t]*$" ) {
    gsub("OTC[_a-zA-Z0-9]*_[_a-zA-Z0-9]*","<A HREF=\"&.html\">&</A>",$0)
    gsub("OUX[_a-zA-Z0-9]*_[_a-zA-Z0-9]*","<A HREF=\"&.html\">&</A>",$0)
    gsub("OTK[_a-zA-Z0-9]*_[_a-zA-Z0-9]*","<A HREF=\"&.html\">&</A>",$0)
    print $0
    if ( getline <= 0 )
      $0 = ""
  }
  if ( $0 !~ "^[ \t]*$" )
    print $0
}

{ print $0 }
