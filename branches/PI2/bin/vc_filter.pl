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

$error_b = "<FONT COLOR=\"FF0000\">";
$error_e = "</FONT>";

$warning_b = "<FONT COLOR=\"FF7700\">";
$warning_e = "</FONT>";

$config_b = "";
$config_e = "";

$new_build_b = "<P>";
$new_build_e = "";
$line_break = "<BR>";
$brief = 0;
$results = 0;

while ( $#ARGV >= 0  &&  $ARGV[0] =~ /^-/ )
{
    if ( $ARGV[0] =~ /-c/) # Text format
    {
        $header = "" ;
        $trailer = "" ;
        $kosher_b = "";
        $kosher_e = "";

        $error_b = "";
        $error_e = "";

        $warning_b = "";
        $warning_e = "";

        $config_b = "";
        $config_e = "";

        $new_build_b = "\n\n";
        $new_build_e = "";
        $line_break = "";
    }
    elsif ( $ARGV[0] =~ /-b/) 
    {
        $brief = 1;
    }
    elsif( $ARGV[0] =~ /-r/)
    {
        $results = 1;
    }
    elsif ( $ARGV[0] =~ /-(\?|h)/)
    {
        print "Options\n";
        print "-b         = Brief output (only errors)\n";
        print "-c         = Text format\n";
        print "-r         = Print only results\n";
        exit;
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
open FP, "$fname";

print $header;

$project = "NULL";
$configuration = "NULL";
$dsp = "NULL";
$first_problem = 1;
$ignored_warnings = 0;

restart: while (<FP>)
{
    if (/^Auto_compiling (.*)/)
    {
        print "$new_build_b$_$new_build_e$line_break"
            if (!$brief && !$results);  
        $dsp = $1;
        $first_problem = 1;
    }
    elsif (/^--------------------Configuration: (.*) - (.*)--------------------/)
    {
        print $config_b.$_.$new_build_e.$line_break
            if (!$brief && !$results);
        $project = $1;
        $configuration = $2;
        $first_problem = 1;
    }
    elsif (/\- (.*) error\(s\)\, (.*) warning\(s\)/)
    {
        print "$_$line_break"
            if (!$brief 
                || ($results && ($1 > 0 || ($2 - $ignored_warnings) > 0)));
        $ignored_warnings = 0;
    }
    elsif (/^LINK : warning LNK4089:/)
    {
        print "$_$line_break"
            if (!$brief);

        ++$ignored_warnings;
    }
    elsif (/warning/i)
    {
        if (!$results)
        {
            if ($first_problem && $brief)
            {
                print "$config_b----- $dsp$config_e$line_break";
                print "$config_b-------------------- $project: $configuration$config_e$line_break";
            }
            $first_problem = 0;
            print "$warning_b$_$warning_e$line_break";
        }
    }
    elsif (/error/i)
    {
        if (!$results)
        {
            if ($first_problem && $brief)
            {
                print "$config_b----- $dsp$config_e$line_break";
                print "$config_b-------------------- $project: $configuration$config_e$line_break";
            }
            $first_problem = 0;
            print "$error_b$_$error_e$line_break";
        }
    }
    else
    {
        print "$_$line_break"
            if (!$brief && !$results);
    }

}

print $trailer;
