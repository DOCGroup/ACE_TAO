""" Catches most files disallowed in CVS """

type_list = ["icc", "ncb", "opt", "zip", "dsw",
             "vcproj", "dsw", "bor", "vcp", "pdb",
             "o", "ilk", "pyc", "so", "dll", "lib" ]

from sys import stderr

def handler (file_name, file_content):
    stderr.write (file_name + ":0: error: This file should not be checked into the repository\n")
    return 1

