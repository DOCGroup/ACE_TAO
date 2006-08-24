#!/usr/bin/python

##
## Modified version of the Web_Input_Adapter.py script the simply generate
## images of the supplied deployment XML file.  This does not support file
## uploading so whatever XML to be displayed must accessable to the
## webserver's file system.  The location XML can be pulled from directly by
## name is based on the PLANPATH environment variable.
##
## Alternativly the xform_local script could be run to generate the necissary
## html and png files which could then be viewed locally without the hassle
## of starting the web server etc.
##
##
## @author Jonathan Walsh (jwalsh@atl.lmco.com)
##
##

import sys
import os
import cgi
import cgitb
import time
import os.path

#Some fun constants
EXEC_PATH=os.environ['RACE_ROOT'] + "/Input_Adapters/Web_Input_Adapter/new_web_content"
HEADPOPUP = "<SCRIPT TYPE=\"text/javascript\">\nfunction popup(mylink, windowname)\n{\nif (! window.focus)return true;\nvar href;\nif (typeof(mylink) == 'string')\n   href=mylink;\nelse\n   href=mylink.href;\nwindow.open(href, windowname, \'scrollbars=yes\');\nreturn false;\n}\n</SCRIPT>"
host_name=os.environ['HOSTNAME']

#Default value for this needs to be /opt/descriptors
plan_path=os.environ['PLANPATH']

def main(args):
    
    cgitb.enable()
    
    errorText = ""
    headContent=""
    bodycmd=""
    
    form = cgi.FieldStorage()
    fileSelected = form.getvalue("text_field")
    popup = form.getvalue("nopopup")

    #Have the filename to model!
    #check to see if it is value
    if not fileSelected is None:
        theFile = plan_path +"/"+ fileSelected
        if os.path.isfile( theFile ):
            os.system("cp " + theFile + " " + EXEC_PATH + "/image_temp.cdp")
            os.system(". "+EXEC_PATH+"/xform_html " + EXEC_PATH + "/image_temp.cdp " + fileSelected )#+ " > " + EXEC_PATH + "/image.htm")

            #build the page content
            headContent = HEADPOPUP #setup the head for autopopup windows
            webPath = "http://"+host_name+":5440/new_web_content/image.htm"
            bodycmd = "onLoad=\"popup('" + webPath + "', '" + theFile +" Visualization')\""
        
        else:
            errorText = "<BR><BR>ERROR: " + theFile + " is not a valid file!"

    if popup is None:
        #display the pop up window
        print "Content-Type: text/html"
        print ""
        print "<HEAD>"+headContent+"</HEAD>"
        print "<BODY " + bodycmd + ">"
        print "<TITLE>Deployment Plan Visualization</TITLE>"
        print "<h1>Deployment Plan Visualization</h1>"
        print "<form action=\"\">"
        print "Current plan location is: " + plan_path +"<br><br>"
        print "<b>Enter Deployment XML file location to model: </b>"
        print "<INPUT TYPE=\"text\" NAME=\"text_field\">"
        print "<br><input type=\"checkbox\" name=\"nopopup\">Suppress Pop-Up Window"
        print "<br>"
        print "<input type=\"submit\" value=\"Send\">"
        print "<input type=\"reset\" value=\"Reset Form\">"
        print errorText
        print "</form>"
    else:
        #Open the pop up window's content in the current window
        print "Content-Type: text/html"
        print ""
        htmlfile = open(EXEC_PATH+"/image.htm","r")
        for line in htmlfile:
            print line

#Get this to run as a script
if __name__ == "__main__":
    main( sys.argv )
