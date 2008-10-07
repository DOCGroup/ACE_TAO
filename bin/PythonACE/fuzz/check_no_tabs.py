""" Checks that files in ace/tao/ciao do not contain tabs """

import _types
type_list = _types.source_files + _types.header_files + _types.inline_files + _types.idl_files

from sys import stderr
import re

regex = re.compile ("\t")

error_message = ": error: contains tab characters\n"

from _generic_handler import generic_handler

def handler (file_name, file_content):
    return generic_handler_no_exceptions (regex, error_message, file_name, file_content)
