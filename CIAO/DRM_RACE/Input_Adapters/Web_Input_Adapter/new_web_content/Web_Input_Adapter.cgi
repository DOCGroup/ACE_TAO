#!/usr/bin/python

##
## A Python CGI script that will allow for interaction with RACE components.
## This program will look for deployment XML first based on the PLANPATH
## environment variable, if this is not set it will use the DEFAULT_PATH
## constant.
##
## This file must be set to executable by all!!! A soft link to this file may
## also be created ie: ln -s Web_Input_Adapter.py Web_Input_Adapter.cgi (see
## setup)
##
## In order to change the port that this cgi script is serving from make sure
## to change the PORT variable.
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

#####Constant Values#####
#Default path to look for XML

#DEFAULT_PATH=os.environ['CIAO_ROOT'] + "/RACE/descriptors"
DEFAULT_PATH="/opt/descriptors"
EXEC_PATH=os.environ['CIAO_ROOT'] + "/RACE/Input_Adapters/Web_Input_Adapter/new_web_content"
CONTENT_PATH=EXEC_PATH #left to support legacy calls...
OUTPUT_FILE='output'#these two files will be placed in the JAWS execution path
DEPLOYED_FILE='deployed'
HOSTNAME=os.environ['SERVER_NAME']
PORT=80
RACE_PLAN="RACE.cdp"

HEADPOPUP = "<SCRIPT TYPE=\"text/javascript\">\nfunction popup(mylink, windowname)\n{\nif (! window.focus)return true;\nvar href;\nif (typeof(mylink) == 'string')\n   href=mylink;\nelse\n   href=mylink.href;\nwindow.open(href, windowname);\nreturn false;\n}\n</SCRIPT>"

##
## Method to convert the cdp XML file into a format that can be browser
## displayed
##
def parse_replace( infile, outfile, titleString ):
    f=open(infile,'r')
    w=open(outfile,'w')
    w.write(titleString)
    for line in f:
        line = line.replace("<","&lt;")
        line = line.replace(">","&gt;")
        line = line.replace(" ","&nbsp;")
        line = line.replace("\n","<BR>")
        w.write(line)
    f.close()
    w.close()

##
##Main method
##
def main(args):
    cgitb.enable() #enable cgi error messaging

    #Get the path to look for deployment xml
    try:
        plan_path = os.environ['PLAN_PATH']
    except:
        plan_path = DEFAULT_PATH

    if not os.path.isdir(plan_path):
        plan_path = DEFAULT_PATH

    #Strings that will be placed in the HTML output
    message = "" #String for the message portion of the screen
    headContent = "" #content for the HEAD portion
    bodycmd = "" #command to insert in the BODY tag
    
    #set up the temporary file directory
    if not os.path.isdir(CONTENT_PATH):
        os.mkdir(CONTENT_PATH)

    #Get the deployment plans!
    #The directory structure is a root plan directory with any
    #   subdirectories containing the necissary plans
    plans = []
    if os.path.isdir(plan_path):
        for file in os.listdir(plan_path):
            if file[-4:] == ".xml" or file[-4:] == ".cdp":
                if len( file ) < 30:
                    plans.append(file)
                        
    deployed_plans = []

    #Figure out which plans are already deployed
    if os.path.isfile(DEPLOYED_FILE):
        f=open(DEPLOYED_FILE,'r')
        contents = f.read()
        split_contents = contents.split(":")
        for plan in split_contents:
            if plan != '':
                deployed_plans.append(plan)
        f.close()

    if not RACE_PLAN in deployed_plans:
        deployed_plans.append( RACE_PLAN )

    #Restore the old output if it exists
    if os.path.isfile(OUTPUT_FILE):
        f=open(OUTPUT_FILE, 'r')
        message = f.read()
        f.close()

    #approximate timestamp for output
    mtime = time.strftime("%d %b %Y %H:%M:%S GMT", time.gmtime())
    
    ######Form handling######
    form = cgi.FieldStorage()
    #deployment interaction
    availFile = form.getvalue("AvailSelect")
    depFile = form.getvalue("DepSelect")
    availXML = form.getvalue("availxml")
    depXML = form.getvalue("depxml")
    availImg = form.getvalue("availimg")
    depImg = form.getvalue("depimg")
    deptearAction = form.getvalue("deployment")
    noRACE = form.getvalue("norace")
    #RACE interaction
    raceSelection = form.getvalue("race")
    #log interaction
    clear = form.getvalue("clear")


    #If we are dealing with a deployed plan we need to get the UUID!!
    uuid="NULL"
    if depFile:
        f=open(plan_path+"/"+depFile)
        for line in f:
            if line.find("<UUID>") != -1:
                line = line.strip()
                #found the uuid tag, get the value
                uuid = line[6:-7]
                break
        f.close()
    

    #######Actually perform the action requested######
    #display XML
    if availXML and availFile:
        headContent = HEADPOPUP #put the content for the popup
        #build the filename
        theFile = plan_path + "/" + availFile
        message = mtime + " Displaying XML for "+theFile+".\n" + message
        #copy the file to a webspace location and display it
        titleString="<center><H2>Deployment XML for: " + availFile + "</H2></center><BR>"
        parse_replace(theFile, CONTENT_PATH+"/xml_page.htm", titleString)
        webPath = "http://"+HOSTNAME+":"+str(PORT)+"/new_web_content/xml_page.htm"
        bodycmd = "onLoad=\"popup('"+webPath+"', 'Deployment XML')\""
        
    elif depXML and depFile:
        headContent = HEADPOPUP #put the content for the popup
        #build the filename
        if os.path.isfile( uuid + ".cdp" ):
            theFile = uuid + ".cdp"
        else:
            theFile = plan_path + "/" + depFile
            
        message = mtime + " Displaying XML for "+theFile+".\n" + message
        #copy the file to a webspace location and display it
        titleString="<center><H2>Deployment XML for: " + depFile + "</H2></center><BR>"
        parse_replace(theFile, CONTENT_PATH+"/xml_page.htm", titleString)
        webPath = "http://"+HOSTNAME+":"+str(PORT)+"/new_web_content/xml_page.htm"
        
        bodycmd = "onLoad=\"popup('"+webPath+"', 'Deployment XML')\""
    elif depXML or availXML:
        message = mtime + " ERROR: Must select a plan to display the XML.\n" + message

    #display image
    if availImg and availFile:
        #get the actual filename
        theFile = plan_path + "/" + availFile
        message = mtime + " Displaying a visualization for "+theFile+".\n" + message
        #copy it over to the temporary webspace
        os.system("cp " + theFile + " " + CONTENT_PATH + "/image_temp.cdp")
        
        #run xform to create the files
        os.system(". "+EXEC_PATH+"/xform_html " + CONTENT_PATH + "/image_temp.cdp " + availFile + " " + str(PORT) )
        
        #add the head concent for the popup
        headContent = HEADPOPUP #setup the head for autopopup windows
        
        #create the webpate and show the popup
        webPath = "http://"+HOSTNAME+":"+str(PORT)+"/new_web_content/image.htm"
        bodycmd = "onLoad=\"popup('" + webPath + "', '" + theFile +" Visualization')\""
    elif depImg and depFile:
        #get the actual filename
        if os.path.isfile( uuid + ".cdp" ):
            theFile = uuid + ".cdp"
        else:
            theFile = plan_path + "/" + depFile
        message = mtime + " Displaying a visualization for "+theFile+".\n" + message
        #copy it over to the temporary webspace
        os.system("cp " + theFile + " " + CONTENT_PATH + "/image_temp.cdp")
        
        #run xform to create the files
        os.system(". "+EXEC_PATH+"/xform_html " + CONTENT_PATH + "/image_temp.cdp " + depFile + " " + str(PORT) )
        
        #add the head concent for the popup
        headContent = HEADPOPUP #setup the head for autopopup windows
        
        #create the webpate and show the popup
        webPath = "http://"+HOSTNAME+":"+str(PORT)+"/new_web_content/image.htm"
        bodycmd = "onLoad=\"popup('" + webPath + "', '" + theFile +" Visualization')\""
    elif depImg or availImg:
        message = mtime + " ERROR: Must select a plan to display a visualization.\n" + message 

    #deploy teardown action
    if deptearAction:
        if deptearAction == "<< Teardown" and depFile:#teardown plan
            if depFile == RACE_PLAN: #NO TEARING RACE DOWN!
                message = mtime + " ERROR: Cannot tear down RACE.\n" + message
            else:
                theFile = plan_path + "/" + depFile
                ###NOTE: switch the commenting on the lines below to send the##
                ###uuid rather than the filename!!###
                os.system( EXEC_PATH + "/WIA_helper teardown " + theFile + " > deploy.out" )
                #os.system( EXEC_PATH + "/WIA_helper teardown " + uuid )
                message = "\tPlan UUID: " + uuid + "\n" + message
                message = mtime + " Tearing down the plan: " + theFile + "\n" + message
                deployed_plans.remove(depFile)
            
            
        elif deptearAction == "Deploy >>" and availFile:#must be a deployment..
            theFile = plan_path + "/" + availFile
            deployed_plans.append( availFile )
            if noRACE:
                message = mtime + " Deploying the plan without RACE: " + theFile + "\n" + message
                os.system(EXEC_PATH+"/WIA_helper deploynorace "+theFile+" > deploy.out" )

            else:
                message = mtime + " Deploying the plan: " + theFile + "\n" + message
                os.system(EXEC_PATH+"/WIA_helper deploy "+theFile+" > deploy.out" )
        else:
            message = mtime + " ERROR: Invalid file selection for deployment interaction.\n" + message
    #RACE start/stop
    if raceSelection:
        if raceSelection == "Start":
            message = mtime + " Starting RACE.\n" + message
        elif raceSelection == "Stop":
            message = mtime + " Stopping RACE.\n" + message
        elif raceSelection == "Plot":
            message = mtime + " Displaying RACE Plot.\n" + message
            headContent = HEADPOPUP
            webPath = "http://"+HOSTNAME+":"+str(PORT)+"/new_web_content/RACE_Plot.cgi"
            bodycmd = "onLoad=\"popup('" + webPath + "', 'Critical Path Execution Time')\""
            

            
    #clear log file
    if clear:
        message = mtime + " Output Log Cleared.\n"

    #######Store all the output and deployed plan state######
    f=open('output','w')
    f.write(message)
    f.close()
    f=open(DEPLOYED_FILE,'w')
    for item in deployed_plans:
        f.write( item +":")
    f.close()

    #####PRINT OUT THE ACTUAL HTML#####

    print "Content-Type: text/html"     # HTML is following
    print ""                            # blank line, end of headers

    print "<HEAD>" + headContent + "</HEAD>"
    print ""
    print "<BODY " + bodycmd + " bgcolor=\"#af0101\">"

    print "<TITLE>RACE - Resource Allocation and Control Engine</TITLE>"

    print "<form action=\"\">"
    print "<table style=\"width: 100%\" border=\"1\" bgcolor=\"white\" >"
    
    print "<TR><td>"#title row
    print "<table style=\"width:100%\" border=\"0\">"
    print "<BASE href=\"http://"+HOSTNAME+":"+str(PORT)+"/new_web_content/\">"
    print "<center>"
    print "<TR>"#top row
    print "<TD style=\"width:35%\" rowspan=\"2\"><img alt=\"\" src=\"arms_logo.png\" style=\"width: 127px; height: 109px;\"></TD><TD style=\"width:25%\"><p align=\"center\"><img alt=\"\" src=\"race.png\" style=\"width: 174px; height: 64px;\"></p></TD><TD style=\"width:40%\"><p align=\"right\"><img alt=\"\" src=\"lockheed.png\" style=\"width: 172px; height: 34px;\"></p></TD>"
    print "</TR>"
    print "<TR>"#middle row
    print "<TD><p align=\"center\"><b><img src=\"RACE_words.png\"></b></p></TD><TD><p align=\"right\"><img alt=\"\" src=\"vandyV.jpg\" style=\"width: 35px; height: 35px;\"><img alt=\"\" src=\"isis.png\" style=\"width: 53px; height: 35px;\"></p</TD>"
    print "</TR>"
    print "</center>"
    print "</base>"
    print "</table>"
    print "</td></TR>"
    
    print "<TR>"#deployment lists
    print "<table style=\"width:100%;\" border\"1\" bgcolor=\"white\">"
    print "<TD style=\"width: 40%\" background=\"greyweave.jpg\">"#available plans
    #The scrolling menu
    print "<center>"
    print "<b>Available Plans:</b><br>"
    print "<select name=\"AvailSelect\" size=7>"
    first = True
    for value in plans:
        if not value in deployed_plans:
            #This is a deployed plan
            if first:
                print "<option selected>" + value + "</option></font>"
                first = False
            else:
                print "<option>" + value + "</option></font>"
    print "</select>"
    print "<br>"
    print "<input type=\"submit\" value=\"View XML\" name=\"availxml\">"
    print "<input type=\"submit\" value=\"View Image\" name=\"availimg\">"
    print "</center>"
    print "</TD>"
    print "<TD style=\"width: 20%\" background=\"greyweave.jpg\">"#buttons
    print "<center>"
    print "<input type=\"submit\" value=\"Deploy >>\" name=\"deployment\" size=25><BR>"
    print "<input type=\"submit\" value=\"<< Teardown\" name=\"deployment\"><BR>"
    print "<input type=\"checkbox\" name=\"norace\">Bypass"
    print "</center>"
    print "</TD>"
    print "<TD style=\"width: 40%\" background=\"greyweave.jpg\">"#deployed plans
    print "<center>"
    print "<b>Deployed Plans:</b><br>"
    print "<select name=\"DepSelect\" size=7>"
    first = True
    for value in deployed_plans:
        #This is a deployed plan
        if first:
            print "<option selected>" + value + "</option></font>"
            first = False
        else:
            print "<option>" + value + "</option></font>"
    print "</select>"
    print "<br>"
    print "<input type=\"submit\" value=\"View XML\" name=\"depxml\">"
    print "<input type=\"submit\" value=\"View Image\" name=\"depimg\">"
    print "</center>"
    print "</TD>"
    print "</table>"
    print "</TR>"

    print "<TR><td><center>"#RACE control
    print "<table style=\"width:100%\" border\"1\" bgcolor=\"white\"><tr><td background=\"greyweave.jpg\">"
    print "<center>"
    print "<h3>RACE Controller</h3>"
    print "<input type=\"submit\" value=\"Start\" name=\"race\">"
    print "<input type=\"submit\" value=\"Stop\" name=\"race\">"
    print "<input type=\"submit\" value=\"Plot\" name=\"race\">"
    print "</center>"
    print "</td></TR></table>"
    print "</td></TR>"

    print "<TR>"#output logs
    print "<td>"
    print "<table style=\"width:100%\" border\"1\" bgcolor=\"white\">"
    print "<tr><td background=\"greyweave.jpg\">"
    print "<center>"
    print "<H3>Output Log File</H3>"
    print "<textarea cols=\"120\" rows=\"15\" readonly>"
    print message
    print "</textarea>"
    print "<br><input type=\"submit\" value=\"Clear Output Log File\" name=\"clear\"><br>"
    print "</center>"
    print "</td></tr>"
    print "</table>"
    print "</td></TR>"
    
    print "</table>"#outer table
        
    print "</body>"

#Get this to run as a script
if __name__ == "__main__":
    main( sys.argv )
