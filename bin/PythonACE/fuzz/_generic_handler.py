""" Defines a generic handler that tests against a given regex, and allows for exclusions. """

from sys import stderr

def generic_handler (regex, begin_exclude, end_exclude, error_message, file_name, file_content):
    retval = 0
    
    if regex.search (file_content) != None:
        # We have a potential violation, lets check
        lines = file_content.splitlines ()
        exclusion = False
        for line in range (len (lines)):
            if begin_exclude.search (lines[line]) != None:
                exclusion = True
            elif end_exclude.search (lines[line]) != None:
                exclusion = False
            elif (exclusion == False) and (regex.search (lines[line]) != None):
                # Violation!
                stderr.write (file_name + ':' + str (line + 1) + error_message)
                retval = 1
    return retval

def generic_handler_no_exceptions (regex, error_message, file_name, file_content):
    retval = 0
    
    if regex.search (file_content) != None:
        # We have a potential violation, lets check
        lines = file_content.splitlines ()
        for line in range (len (lines)):
            if regex.search (lines[line]) != None:
                # Violation!
                stderr.write (file_name + ':' + str (line + 1) + error_message)
                retval = 1
    return retval
