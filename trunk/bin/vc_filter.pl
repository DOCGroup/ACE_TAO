# $Id$

$fname = $ARGV[0];

open (fp, "$fname");

while ($line = <fp>)
{
    print $line if ($line =~/^--------------------Configuration:/ ||
                    $line =~/^[A-Z_a-z0-9.]+ - [0-9]+ error\(s\), +[0-9]+ warning\(s\)/ ||
                    $line =~/^[A-Z_a-z0-9.\\:]+\([0-9]+\) : / ||
                    $line =~/^[A-Z_a-z0-9.\\:]+\.obj : / ||
                    $line =~/^fatal error/);
}
