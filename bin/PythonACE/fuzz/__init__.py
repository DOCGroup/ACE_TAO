""" This init script loads all python modules in the current directory that
    do not start with an '_', loads them as a module"""

file_type_handlers = dict ()

def register_handler (module):
     
    for item in module.type_list:
        if file_type_handlers.has_key (item):
            file_type_handlers[item].append (module.handler)
        else:
            handlers = list ()
            handlers.append (module.handler)
            file_type_handlers[item] = handlers

import re

extension_re = re.compile(".+\.([^.]+)$")



# The following is the initialization logic that is executed
# when the fuzz module is loaded
from os import listdir, chdir, getcwd
from sys import stderr, path
oldwd = getcwd ()

try:
    # The following is a trick to get the directory THIS SCRIPT - note, not necessarily the CWD -
    # is located.  We use this path later to load all of the available fuzz checks.
    import _path
    script_path = str (_path).split ()[3][1:-11]
    if script_path == "":
        script_path = "."

    chdir (script_path)

    path.append (getcwd ())

    files = listdir (".")

    modules = list ()

    # We need to import the warning handler here.  If we use a traditional import elsewhere,
    # we get all kinds of problems with the warning_handler being imported twice - once as
    # fuzz._warning_handler and again as _warning_handler - making the singleton instances
    # NOT the same.
    _warning_handler = __import__ ("_warning_handler")
    Warning_Handler = _warning_handler.Warning_Handler
    STDERR = _warning_handler.STDERR
    MAILER = _warning_handler.MAILER
    
    for item in files:
        if (item[0] != '_') and (item[-3:] == ".py"):
            print "Registering " + item [:-3]
            try:
                module = __import__ (item[:-3])
                register_handler (module)
            except:
                stderr.write ("FUZZ ERROR: Unable to load the " + item[:-3] + " module, please notify the build czar\n")

finally:
    chdir (oldwd)


def fuzz_check (file_name, file_content):
    # If the user of the module has not instanciated the warning handler,
    # lets do it here
    if not Warning_Handler._isInstantiated ():
        Warning_Handler.getInstance (STDERR)
    
    # get the file extension
    ext_match = extension_re.search (file_name)
    if ext_match == None:
        # we don't have no stinking file extension!
        ext = ""
    else:
        ext = ext_match.group (1)

    retval = 0

    if file_type_handlers.has_key (ext):
        for handler in file_type_handlers[ext]:
            try: # We don't want one misbehaving handler to screw up the whole sustem
                retval += handler (file_name, file_content)
            except:
                stderr.write ("An unknown exception was thrown while trying to run one of the handlers\n")

    # Run the generic handlers
    for handler in file_type_handlers["*"]:
        try: # We don't want one misbehaving handler to screw up the whole sustem
            retval += handler (file_name, file_content)
        except:
            stderr.write ("An unknown exception was thrown while trying to run one of the handlers\n")
            
        
    return retval
