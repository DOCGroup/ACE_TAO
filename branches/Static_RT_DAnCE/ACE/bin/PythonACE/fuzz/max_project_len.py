""" Checks that project names are not too long """

from max_filename import max_filename_length

type_list = ["mpc"]

max_proj_len = max_filename_length - 12 #  GNUmakefile.

from sys import stderr
import re

regex = re.compile ("\s*project\s*\((\w+)\)")

def handler (file_name, file_content):
    match = regex.search (file_content)
    if match == None:
        return 0
    else:
        for group in match.groups ():
            if (len (group) >= max_proj_len):
                stderr.write (file_name + ":0: error: Project named " + group +
                              " meets or exceeds the maximum project name length of " +
                              str (max_proj_len) + " characters\n")

        return 1

        
