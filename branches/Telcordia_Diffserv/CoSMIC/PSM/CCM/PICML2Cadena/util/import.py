import os
import sys
import tempfile
from xml.dom import minidom

#fetch needed environment variables
gre_fullpathname = os.environ['GREAT_PATH'].rstrip('\\')+'\\bin\\GRE.exe'  #maybe not necessary because GReAT puts it into PATH
integration_picml2cadena_fullpath = os.environ['PICML2CADENA_PATH']

#everything was set up correctly?
if (integration_picml2cadena_fullpath=='') :
    print 'FATAL ERROR: PICML2CADENA_PATH environment variable not found!\nPlease set it to your PICML2Cadena path.\nExiting.'
    sys.exit(-1)

#let's see if the user is looking for troubles...
if (integration_picml2cadena_fullpath.find(' ')!=-1) : print 'Warning: To avoid troubles, put the PICML2Cadena directory in a path without whitespaces.\n\
Current value is:\n'+integration_picml2cadena_fullpath+'\nGoing ahead...\n'

#let's construct all the other useful paths
integration_picml2cadena_fullpath = integration_picml2cadena_fullpath.rstrip('\\')+'\\' #so that it works regardless if the PICML2CADENA_PATH ends in '\' or not
transformations_fullpath = integration_picml2cadena_fullpath+'Transformations\\'
integration_udm_fullpath = transformations_fullpath+'Udm\\'


#fetching commandline parameters. Expected first the transformation configuration file,
#then the PICML mga file and then the cadena_scenario xml file

#looking for help?
if ('--help' in sys.argv or '-h' in sys.argv) :
    print '''Extensive help:

Installation:
Please set environment variable PICML2CADENA_PATH pointing to your PICML2Cadena path. Whether ends with backslash or not is irrelevant
Putting export.py and import.py somewhere in your PATH could also be handy (these scripts don't need to stay in a specific directory in order to work correctly)

Launching:
import.py CadenaScenarioXMLFile.xml PICMLMgaFile.mga [-d | -dv] : performs import
-d  : output debug information
-dv : outputs more debug information
The filenames can be relative or absolute. GRE will be invoked with absolute filenames in any case, as this avoids problems.

Use ``import.py --help'' to get this extensive help'''
    sys.exit(0)

quotes = '\"'
space = ' '
if ('-dv' in sys.argv) : debugflag = ' -dv'
elif ('-d' in sys.argv) : debugflag = ' -d'
else : debugflag = ''

purifiedparams = []
for param in sys.argv[1:] :
    if (param!='-d' and param!='-dv') : purifiedparams.append(param)

#bad invocation?
if (len(purifiedparams)!=2) :
    print 'Usage: import.py CadenaScenarioXMLFile.xml PICMLMgaFile.mga [-d | -dv]\n\
Or: import.py --help for extensive help'
    sys.exit(0)

picml_file = os.path.abspath(purifiedparams[1])
cadenascenario_file = os.path.abspath(purifiedparams[0])

#checking correct file extension
if (os.path.splitext(picml_file)[1]!='.mga') :
    print 'Fatal: The specified filename for PICML model '+quotes+picml_file+quotes+' does not have .mga extension!\nExiting.'
    sys.exit(-1)
if (os.path.splitext(cadenascenario_file)[1]!='.xml') :
    print 'Fatal: The specified filename for CadenaScenario '+quotes+cadenascenario_file+quotes+' does not have .xml extension!\nExiting.'
    sys.exit(-1)

#checking existence of files
if (False==os.path.isfile(picml_file)) :
    print 'Fatal: File '+quotes+picml_file+quotes+' not found.\nExiting.'
    sys.exit(-1)
if (False==os.path.isfile(cadenascenario_file)) :
    print 'Fatal: File '+quotes+cadenascenario_file+quotes+' not found.\nExiting.'
    sys.exit(-1)

#checking existence of Cadena2PICML_Configuration.mga, Cadena2PICML-gr.xml, Udm\Cadena2PICML.udm
transfname='Cadena2PICML'
for f in [transformations_fullpath+transfname+'_Configuration.mga',
          transformations_fullpath+transfname+'-gr.xml',
          integration_udm_fullpath+transfname+'.udm'] :
    if (False==os.path.isfile(f)) :
        print 'FATAL: File '+quotes+f+quotes+' does not exist.\n\
This is needed for the transformation to run.\n\
Please open GReAT for this '+transfname+' transformation and run the GReAT Master Interpreter.\n\
Then try to run this script again. If it still doesn\'t work, please obtain a clean update\n\
of the whole PICML2Cadena directory from the CVS and then run this script again.'
        sys.exit(-1)

#validating the input xml file for the schema location and namespace
print 'Validating the input xml file for the schema location and namespace.'
xmldoc = minidom.parse(cadenascenario_file)
print 'Before validation:\n', xmldoc.firstChild.cloneNode(0).toxml()
xmldoc.firstChild.attributes['xmlns:xsi'].value='http://www.w3.org/2001/XMLSchema-instance'
xmldoc.firstChild.attributes['xsi:noNamespaceSchemaLocation'].value=integration_udm_fullpath+'cadena_scenario.xsd'
print 'After validation:\n', xmldoc.firstChild.cloneNode(0).toxml()
print 'Saving validated xml file...'
try:
    f = open(cadenascenario_file,'w+')
    f.write(xmldoc.toxml())
except:
    print 'IO Exception caught! Unable to access file '+quotes+cadenascenario_file+quotes+' for writing.\nIt could be in use by another process.\n\Trying to go ahead anyway, without validation (but don\'t be surprised if the GRE.exe crashes)'

try:
    f.close()
except:
    pass
    


greinvocationcommand = 'gre.exe'+ space +quotes+transformations_fullpath+'Cadena2PICML_Configuration.mga'+quotes
greinvocationcommand += space+quotes+'PICML_File='+picml_file+quotes
greinvocationcommand += space+quotes+'CadenaScenario_File='+cadenascenario_file+quotes
greinvocationcommand += space+quotes+'EmbeddedDummy_File='+tempfile.gettempdir().rstrip('\\')+'\\'+'dummy_embedded.xml'+quotes
greinvocationcommand += debugflag

print 'Invoking GRE.exe with the following commandline:\n\
_______________________________________________________________________________\n',\
greinvocationcommand,'\n\
-------------------------------------------------------------------------------\n'
os.system(greinvocationcommand)

