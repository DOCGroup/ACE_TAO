# $Id$
#
# A perl script that filers length VC output into a concise html report.

$header = "<html><head>
<title>Error/Warning Summary of Daily Build</title>
</head>

<body bgcolor=\"white\">
<h1>Error/Warning Summary of Daily Build</h1><TT>\n";
$trailer = "</TT></body></html>";

$kosher_b = "";
$kosher_e = "";

$in_sin_b = "<FONT COLOR=\"RED\">";
$in_sin_e = "</FONT>";

$new_build_b = "<P>";
$new_build_e = "";

$fname = $ARGV[0];

open (fp, "$fname");

print $header ;

while ($line = <fp>)
{
    print "$new_build_b $line $new_build_e<BR>" if ($line =~/^--------------------Configuration:/);

    if ($line =~/^[A-Z_a-z0-9.]+ - [0-9]+ error\(s\), +[0-9]+ warning\(s\)/)
    {
        if ($line =~/0 error\(s\), 0 warning\(s\)/)
        {
            print "$kosher_b $line $kosher_e<BR>";
        }
        else
        {
            print "$in_sin_b $line $in_sin_e<BR>";
        }
    }

    print "$in_sin_b $line $in_sin_e<BR>"
        if ($line =~/^[A-Z_a-z0-9.\/\\:]+\([0-9]+\) : / ||
            $line =~/^[A-Z_a-z0-9.\\:]+\.obj : / ||
            $line =~/^fatal error/ ||
            $line =~/^LINK : /);

}

print $trailer;
