""" Checks that the C++ inline keyword is not used """

from _types import inline_files
type_list = inline_files

from sys import stderr
import re

regex = re.compile ("(\s|^)+inline\s+")
begin_exclude = re.compile ("FUZZ\: disable check_for_inline")
end_exclude = re.compile ("FUZZ\: enable check_for_inline")

error_message = ": error: contains a C++ inline keyword, instead of ACE_INLINE\n"

from _generic_handler import generic_handler

def handler (file_name, file_content):
    return generic_handler (regex, begin_exclude, end_exclude, error_message, file_name, file_content)

