#!/bin/nawk


# defining macros - eat them 
/^\.de.*/ {
        getline
        while ( $0 !~ "^\.\.$" )
        {
            getline
        }
        getline
    }

# eat [nt]roff comments
$0 ~ /['.][\/\\]"/ || $1 == "'"    { next }

# remove sidebar macros
$1 == ".VS" || $1 == ".VE" || $1 == ".AS"  { next }


# handle first .SH as special case - .SH NAME
/^.SH *NAME */ {
        getline
        while ( $0 ~ /\.[a-zA-Z].*/ )   # eat dot-cmd following title 
        {
            getline
        }
        print "<TITLE>" $0 "</TITLE>"
        print "<H1>" $0 "</H1>\n"
        next

#-e 's/^.SH *NAME */{N;s#.*\n\(.*\)#<H1>\1</H1>#;}' \
    }


# Convert .IP Paragraphs upto next .cmd to hanging indents
#       using <DL></DL> pairs without intervening <LI>

/^\.IP */ {
        if ( inIP > 0 )
        {
            print "</DL>"
        }
        inIP = 1 
	startIP = 1
        print "<DL>"
        match($0, /".*"/ )
        if ( RSTART > 0 )
        {
            arg = substr( $0, RSTART+1, RLENGTH-2)
            
            print "<DT> " arg
        }
        else if ( length( $2 ) > 0 )
        {
            print "<DT> " $2
        }
        next
    }

$0 ~ /^\.[a-zA-Z]*/ && inIP > 0 {
        inIP = 0
        print "</DL>"
    }

# Convert 
# .TP
# Line1
# line 2 - n
# .Any              
#
# to
# <DL>
# <DT> Line1
# <DD> lines 2 - n 
# <DT>

/^\.TP */ {
        if ( inTP > 0 )
        {
            print "</DL>"
        }
        inTP = 1 
        print "<DL>"
        next
    }

inTP == 1 && $1 !~ /\.[a-zA-Z]*/ {
        print "<DT> " $0
        inTP = 2
        next
    }

inTP == 2 && $1 !~ /\.[a-zA-Z]*/{
        print "</I></B>"    # Belt and suspenders
        print "<DD> " $0
        inTP = 3
        next
    }

$0 ~ /^\.[a-zA-Z]*/ && inTP > 0 {
        inTP = 0
        print "</DL>"
    }



$1 == ".AP" {
        $1=""
        print "<DL >"
        print "<DT> " $2 "\t\t" $3 "\t\t("$4")"
        inTP = 2
        next
    }

# make a blank line 
$1 == ".sp" {
        print "<BR>"
        next #        print "<BR>"
    }


$1 == ".ta"  { next }

# just pass everything else on

    {
        if ( startIP > 0 )
        {
            print "<DD> " $0
	    startIP = 0
        }
	else
	{
    	    print $0
	}
    }


