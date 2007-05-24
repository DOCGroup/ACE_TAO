""" Defines a generic handler that tests against a given regex, and allows for exclusions. """

from sys import stderr
import _warning_handler

def generic_handler (regex, begin_exclude, end_exclude, error_message, file_name, file_content, warn = False):
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
                msg = file_name + ':' + str (line + 1) + error_message
                if not warn:
                    stderr.write (msg)
                    retval = 1
                else:
                    handler = _warning_handler.Warning_Handler.getInstance ()
                    handler.add_warning (msg)
    return retval

def generic_handler_no_exceptions (regex, error_message, file_name, file_content, warn = False):
    retval = 0
    
    if regex.search (file_content) != None:
        # We have a potential violation, lets check
        lines = file_content.splitlines ()
        for line in range (len (lines)):
            if regex.search (lines[line]) != None:
                msg = file_name + ':' + str (line + 1) + error_message
                # Violation!
                if not warn:
                    stderr.write (msg)
                    retval = 1
                else:
                    Warning_Handler.getInstance ().add_warning (msg)
    return retval
