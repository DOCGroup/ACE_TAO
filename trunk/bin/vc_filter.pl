# $Id$

$header = "<html> <head>
<title>Error/Warning Summary of Daily Build</title>
</head>

<body bgcolor=\"white\">
<h1>Error/Warning Summary of Daily Build</h1><TT>\n";

$trailer = "</TT> </body> </html>";

$fname = $ARGV[0];

open (fp, "$fname");

print $header ;

while ($line = <fp>)
{
    print "<p> $line<BR>" if ($line =~/^--------------------Configuration:/);

    if ($line =~/^[A-Z_a-z0-9.]+ - [0-9]+ error\(s\), +[0-9]+ warning\(s\)/)
    {
        if ($line =~/0 error\(s\), 0 warning\(s\)/)
        {
            print "$line <BR>";
        }
        else
        {
            print "<FONT COLOR=\"RED\"><B> $line </B></FONT><BR>";
        }
    }

    print "<FONT COLOR=\"RED\"><B> $line </B></FONT><BR>"
        if ($line =~/^[A-Z_a-z0-9.\\:]+\([0-9]+\) : / ||
            $line =~/^[A-Z_a-z0-9.\\:]+\.obj : / ||
            $line =~/^fatal error/);

}

print $trailer;
