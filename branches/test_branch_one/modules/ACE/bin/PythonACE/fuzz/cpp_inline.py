""" Checks for ACE_INLINE or and ASYS_INLINE in a .cpp file """
from _types import source_files
type_list = source_files


import re
from sys import stderr
regex = re.compile ("(^\s*ACE_INLINE)|(^\s*ASYS_INLINE)", re.MULTILINE)

error_message = ": error: ACE_INLINE or ASYS_INLINE found in .cpp file\n"

def handler (file_name, file_content):
    if regex.search (file_content) != None:
        # Lets take some time to generate a detailed error report
        # since we appear to have a violation
        lines = file_content.splitlines ()
        for line in range (len (lines)):
            if regex.search (lines[line]) != None:
                stderr.write (file_name + ':' + str (line + 1) + error_message)

        return 1
    else:
        return 0
