#!/pkg/gnu/bin/perl -i
# This perl script re-arrange the macro indentation so it's easier to
# see the layering relationship.
# $Id$

$indent = 0;

sub inc_indent
{
    $indent += 2;
}

sub dec_indent
{
    $indent -= 2;
}

sub get_indent
{
    $retv = 0;
    die ("Unbalanced macro pairs\n") if ($indent < 0);
    $retv = $indent - 1 if ($indent > 0);
    $retv;
}

while (<>) {
    if (/[ \t]*\#[ \t]*((if|el|en|).*)/)
    {
        $cont = $1;
        $temp = $2;
        if ($temp =~ /if/) {
            print "#", " " x &get_indent (), $cont,"\n";
            inc_indent ();
        }
        elsif ($temp =~ /el/) {
            dec_indent ();
            print "#", " " x &get_indent (), $cont,"\n";
            inc_indent ();
        }
        elsif ($temp =~ /en/) {
            dec_indent ();
            print "#", " " x &get_indent (), $cont,"\n";
        }
        else {
            print "#", " " x &get_indent (), $cont,"\n";
        }
    }
    else {
        print $_;
    }
}

die ("Unbalanced macro pairs\n") if ($indent < 0);
