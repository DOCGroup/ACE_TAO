#!/bin/bash
# $Id$

function strip-binary {
        mkdir -p cut_cidlc/$1
        cd cut_cidlc/$1
        cp /web/users/isisbuilds/CIDLC/$1/cidlc .
        strip cidlc
        bzip2 cidlc
        mv cidlc.bz2 cidlc-$2.bz2
}

case "$1" in
    gnu-linux)
        strip-binary $1 $2
        ;;

    solaris-sparc)
        strip-binary "Solaris/SPARC" $2
        ;;

    solaris-x86)
        strip-binary "Solaris/x86" $2
        ;;

    darwin)
        strip-binary "Darwin/PowerPC" $2
        ;;

    *)
        ssh tango.dre.vanderbilt.edu ~/cut_cidlc.sh gnu-linux $1
        ssh sparc.dre.vanderbilt.edu ~/cut_cidlc.sh solaris-sparc $1
        ssh corona.dre.vanderbilt.edu ~/cut_cidlc.sh solaris-x86 $1
        ssh abbarach.dre.vanderbilt.edu ~/cut_cidlc.sh darwin $1

        cd cut_cidlc
        cp gnu-linux/cidlc-$1.bz2 /web/www/cidlc/binary/gnu-linux/i386/
        cp Solaris/SPARC/cidlc-$1.bz2 /web/www/cidlc/binary/solaris/SPARC/
        cp Solaris/x86/cidlc-$1.bz2 /web/www/cidlc/binary/solaris/x86/
        cp Darwin/PowerPC/cidlc-$1.bz2 /web/www/cidlc/binary/darwin/powerpc/

        #Grab windows
	cp /web/users/isisbuilds/CIDLC/Win32/cidlc.exe .
        zip ./cidlc-$1.zip ./cidlc.exe
	rm ./cidlc.exe
        cp ./cidlc-$1.zip /web/www/cidlc/binary/windows/i386/

	cd ..
	rm -rf cut_cidlc
        ;;
esac

exit 0


