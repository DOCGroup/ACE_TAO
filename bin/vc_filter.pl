# $Id$
#
# A perl script that filers length VC output into a concise html report.

$usage = "vc_filter.pl [-c] <source>";

# Define html tags.
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
$line_break = "<BR>";

while ( $#ARGV >= 0  &&  $ARGV[0] =~ /^-/ )
{
    if ( $ARGV[0] eq '-c') # Text format
    {
        $header = "" ;
        $trailer = "" ;
        $kosher_b = "";
        $kosher_e = "";

        $in_sin_b = "";
        $in_sin_e = "";

        $new_build_b = "\n\n";
        $new_build_e = "";
        $line_break = "";
    }
    else
    {
        warn "$0:  unknown option $ARGV[0]\n";
        die $usage;
    }
    shift;
}

# Get filename.
$fname = $ARGV[0];
open (fp, "$fname");

print $header ;

restart: while ($line = <fp>)
{
    print "$new_build_b $line $new_build_e $line_break" if ($line =~/^--------------------Configuration:/);

    if ($line =~ /(^[A-Z_a-z0-9.]+ - [0-9]+ error\(s\), +[0-9]+ warning\(s\))|(LNK4089)/)
    {
        if ($line =~ /(0 error\(s\), 0 warning\(s\))|(LNK4089)/)
        {
            print "$kosher_b $line $kosher_e $line_break";
        }
        else
        {
            print "$in_sin_b $line $in_sin_e $line_break";
        }
        next restart;
    }

    print "$in_sin_b $line $in_sin_e $line_break"
        if ($line =~/^[-A-Z_a-z0-9.\/\\:]+\([0-9]+\) : / ||
            $line =~/^[-A-Z_a-z0-9.\/\\:]+.*, line [0-9]+: / ||
            $line =~/^[-A-Z_a-z0-9.\\:]+\.obj : / ||
            $line =~/^fatal error/ ||
            $line =~/^Error executing/ ||
            $line =~/^4NT: / ||
            $line =~/^LINK : /);

}

print $trailer;
