""" Checks that ptr_arith_t usage in source code.  ptr_arithh_t is non-portable
    use ptrdiff_t instead. """

import _types
type_list = _types.source_files + _types.header_files + _types.inline_files

from sys import stderr
import re

regex = re.compile ("(ptr_arith_t )|(ptr_arith_t,)")
error_message = ": error: contains a non portable reference to ptr_arith_t, use ptrdiff_t instead.\n"

from _generic_handler import generic_handler_no_exceptions

def handler (file_name, file_content):
    return generic_handler_no_exceptions (regex, error_message, file_name, file_content)

