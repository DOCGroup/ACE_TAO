""" Checks that an obsolete PortableServer::RefCountServantBase is not used """

from _types import header_files
type_list = header_files

from sys import stderr
import re

regex = re.compile ("RefCountServantBase")

error_message = ": error: reference to deprecated PortableServer::RefCountServantBase\n"

from _generic_handler import generic_handler_no_exceptions

def handler (file_name, file_content):
    return generic_handler_no_exceptions (regex, error_message, file_name, file_content)

