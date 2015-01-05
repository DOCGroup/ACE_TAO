""" Checks that includes of files in ace/tao/ciao use "" instead of <> """

import _types
type_list = _types.source_files + _types.header_files + _types.inline_files + _types.idl_files

from sys import stderr
import re

regex = re.compile ("\s*#\s*include\s*(\/\*\*\/){0,1}\s*<(ace|tao|ciao|TAO|CIAO).*>")
begin_exclude = re.compile ("FUZZ\: disable check_for_include")
end_exclude = re.compile ("FUZZ\: enable check_for_include")

error_message = ": error: contains an include to ace/tao/ciao code using <>, instead of \"\"\n"

from _generic_handler import generic_handler

def handler (file_name, file_content):
    return generic_handler (regex, begin_exclude, end_exclude, error_message, file_name, file_content)


