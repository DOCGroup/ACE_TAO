""" Checks that the file has a newline at the end. """

type_list = ["cpp", "h", "inl", "html", "idl", "pl"]

import re
from sys import stderr

regex = re.compile ("\n\Z")

def handler (file_name, file_content):
    if regex.search (file_content) == None:
        stderr.write (file_name + ":0: error: " + file_name + " lacks a newline at the end of the file.\n")
        return 1
    else:
        return 0

    
