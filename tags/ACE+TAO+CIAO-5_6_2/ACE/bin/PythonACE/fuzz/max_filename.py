""" Checks that filenames are the correct length """

type_list = ["*"]

max_filename_length = 50

from os.path import split
from sys import stderr

def handler (file_name, file_content):
    path, filename = split (file_name)
    if len (filename) >= max_filename_length:
        stderr.write (file_name + ":0: error: File name meets or exceeds maximum allowable length ("
                      + str (max_filename_length) + ")\n")
        return 1
    return 0
