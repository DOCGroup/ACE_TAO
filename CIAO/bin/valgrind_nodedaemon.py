#!/usr/bin/python
# $Id$
#
# Runs a NodeManager (optionally) under valgrind with the NodeApplication (optionally) under valgrind.

from optparse import OptionParser
from os import system
from os import environ

def parse_args ():
    
    parser = OptionParser (usage="usage: valgrind_nodemanager [options] <port_number>")

    parser.add_option ("-v", "--verbose", dest="verbose", action="store_true",
                       help="Output the command that is to be executed.",
                       default=False)
    parser.add_option ("-l","--log", dest="log_file", action="store",
                       help="Log all output to a given file.",
                       default="")
    parser.add_option ("-t", "--tool", dest="valgrind_tool", action="store",
                       help="Specify the valgrind tool to run",
                       default="memcheck")
    parser.add_option ("--nm", dest="node_manager", action="store_true",
                       help="Run valgrind on the NodeManager",
                       default=False)
    parser.add_option ("--na", dest="node_application", action="store_true",
                       help="Run valgrind on the NodeApplication",
                       default=False)
    parser.add_option ("--valgrind_args", dest="valgrind_args", action="store",
                       help="Additional arguments to pass to valgrind",
                       default="")
    parser.add_option ("-g", dest="gen_supp", action="store_true",
                       help="Generate suppression lines",
                       default=False)
    parser.add_option ("-s", dest="supp_file", action="store",
                       help="Suppression file for Valgrind to use",
                       default="")
    parser.add_option ("--lc", dest="leak_check", action="store_true",
                       help="Perform a full leak check",
                       default=False)
    
    return parser.parse_args ()

import os

def main ():
    (option, args) = parse_args ()

    ciao_root = environ['CIAO_ROOT']
    
    # Build the valgrind command
    valgrind_command = "valgrind --tool=" + option.valgrind_tool + ' ' +\
                       option.valgrind_args + ' '

    if option.gen_supp:
        valgrind_command += "--gen-suppressions=all "

    if option.supp_file != "":
        valgrind_command += "--suppressions=\"" + options.supp_file + '" '

    if option.leak_check:
        valgrind_command += "--leak-check=full "

    # Build the actual command
    command = ""
    
    if option.node_manager:
        command += valgrind_command

    command += ciao_root + "/DAnCE/NodeManager/NodeManager " +\
               "-ORBEndpoint iiop://localhost:" + args[0] + ' '

    if option.node_application:
        command += "-d 60 -s\"" + valgrind_command
    else:
        command += " -s \""
    
    command += ciao_root + "/DAnCE/NodeApplication/NodeApplication" + '"'

    print command

    system (command)

if __name__ == "__main__":
    main ()
    
    
