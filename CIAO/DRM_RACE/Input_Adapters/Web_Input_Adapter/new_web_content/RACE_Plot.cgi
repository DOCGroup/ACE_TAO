#!/usr/bin/python

##
## CGI program to generate display a graph of the controller.log file.  This
## page will refresh every REFRESH_RATE seconds.
##
## In order to change the port that this cgi script is serving from make sure
## to change the PORT variable.
##
## @author Jonathan Walsh (jwalsh@atl.lmco.com)
##
##

import sys
import os
import cgi
import cgitb
import time

def main(args):
    
#     cgitb.enable()

    REFRESH_RATE = 10
    PORT = 80

    HOSTNAME = os.environ['SERVER_NAME']
    FILE_PATH= "/opt/descriptors"
    EXEC_PATH = os.environ['CIAO_ROOT'] + "/RACE/Input_Adapters/Web_Input_Adapter/new_web_content"

    os.system( "sudo ln -sf " + FILE_PATH + "/controller.log " + EXEC_PATH + "/controller.log" )

    os.chdir ( EXEC_PATH );

    #####Do work to generate the image here#####
    os.system( "gnuplot " + EXEC_PATH + "/demo.gplot" )
 
    print "Content-Type: text/html"
    print ""
    print "<HTML>"
    print "<BASE href=http://"+HOSTNAME+":"+str(PORT)+"/new_web_content/ />"
    print "<meta http-equiv=\"refresh\" content=\""+str(REFRESH_RATE)+"; url=RACE_Plot.cgi\">"
    print "<body>"
    print "<TITLE>Critical Path Execution Time</TITLE>"
    print "<center>"
    print "<H1>Critical Path Execution Time</H1>"
    #print str(time.gmtime()) + "<br>"
    from time import gmtime, strftime
    print "Last Refresh: " + strftime("%a, %d %b %Y %H:%M:%S GMT", gmtime()) + "<BR>"
    print "<img src=./demo.png>"###Link the image here#####
    
    print "</center>"
    print "</body>"
    print "</HTML>"

#Get this to run as a script
if __name__ == "__main__":
    main( sys.argv )
