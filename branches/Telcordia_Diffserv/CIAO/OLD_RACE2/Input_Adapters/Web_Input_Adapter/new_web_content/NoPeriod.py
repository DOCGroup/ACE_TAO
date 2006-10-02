#!/usr/bin/python

##
## Script to replace periods in connection and id tags within a deployment XML
## file.
##
## Usage: NoPeriod.py XMLFILE.cdp
##
## @author Jonathan Walsh (jwalsh@atl.lmco.com)
##

 
import sys
import os
import os.path

def usage():
    print "Usage: NoPeriod.py [deployment_plan.cdp]"
    sys.exit( 1 )

def main(args):
    if args[1] is None:
        usage()
        
    thefile = args[1]
    
    if not os.path.isfile(thefile):
        usage()

    #the file is ok, start the read/replace
    f=open(thefile,'r')
    out=""
    firstConn = False
    secondConn = False
    for line in f:
        #handle the instance ids
        if line.find("<instance id=\"") != -1:
            line = line.replace(".", "_")

        #handle the connection instance ids
        if line.find("<connection>"):
            firstConn = True
            secondConn = True
        if line.find("<instance>") != -1 and firstConn:
            line = line.replace(".", "_")
            firstConn = False
        elif line.find("<instance>") != -1 and secondConn:
            line = line.replace(".", "_")
            secondConn = False
        out=out+line

    f.close()
    f=open(thefile, 'w')
    f.write(out)
    f.close()


#Get this to run as a script
if __name__ == "__main__":
    main( sys.argv )
