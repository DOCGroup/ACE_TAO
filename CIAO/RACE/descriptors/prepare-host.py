#!/usr/bin/python
#
# Will Otte <wotte@dre.vanderbilt.edu>
#
# Script to prepare a given list of hosts in Emulab.

from optparse import OptionParser
from os import system

def parse_args ():
    default_command = "pwd"
    
    parser = OptionParser (usage="usage: %prog [options] host1 host2 ... hostn")

    parser.add_option ("-v", "--verbose", dest="verbose", action="store_true",
                       help="Output commands that are to be executed.",
                       default=False)
    parser.add_option ("-s", "--silent", dest="silent", action="store_true",
                       help="Send all command output to /dev/null (takes precidence over --file)",
                       default=False)
    parser.add_option ("--file", dest="file", action="store_true",
                       help="Store output to a file named host.log",
                       default=False)
    parser.add_option ("--sequential", dest="sequential", action="store_true",
                       help="Process all hosts sequentially, instead of simultaneously",
                       default=False)
    parser.add_option ("-c", "--command", dest="command", action="store",
                       help="A command (other than the default make command) to run",
                       default=default_command)
    parser.add_option ("-n", "--dry-run", dest="dryrun", action="store_true",
                       help="Don't actually send any commands.",
                       default=False)
    
    return parser.parse_args ()

def main ():
    (option, args) = parse_args ()

    
    for name in args:
        command = "ssh -n " + name + " " + option.command + " 2>&1"

        if option.silent:
            command += " 1>/dev/null"
        elif option.file:
            command += " 1>" + name + ".log"
        
        if not option.sequential:
            command += " && echo \"" + name + " finished.\" &"
        else:
            command += " && echo \"" + name + " finished.\" "
        
        if option.verbose:
            print "Executing: " + command

        if option.dryrun:
            print command
        else:
            system (command)

    print "All commands executed!"

if __name__ == "__main__":
    main ()
