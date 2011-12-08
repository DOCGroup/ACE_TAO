# $Id$
temp_path = ""


# # The following is the initialization logic that is executed
# # when the fuzz module is loaded
# from os import listdir, chdir, getcwd
# from sys import stderr, path
# oldwd = getcwd ()

# try:
#     # The following is a trick to get the directory THIS SCRIPT - note, not necessarily the CWD -
#     # is located.  We use this path later to load all of the available templates
#     import _path
#     script_path = str (_path).split ()[3][1:-11]
#     if script_path == "":
#         script_path = "."

#     chdir (script_path + "/templates")

#     path.append (getcwd ())

#     files = listdir (".")

#     modules = list ()

    
#     for item in files:
#         if (item[0] != '_') and (item[-3:] == ".py"):
#             print "Registering " + item [:-3]
#             try:
#                 __import__ (item[:-3])
#             except:
#                 stderr.write ("ERROR: Unable to load the " + item[:-3] + " template\n")

# finally:
#     chdir (oldwd)

import header
import comp_impl
import comp_inst
import footer
import homed_comp_impl
import homed_comp_inst
import home_impl
import home_inst
import artifact
import connection
import external_reference
import config_prop
import internal_endpoint
import deploy_requirement

