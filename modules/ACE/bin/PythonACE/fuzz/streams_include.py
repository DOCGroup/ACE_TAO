""" Checks for inclusion of a non efficient streams include """

import _types
type_list = _types.source_files +_types. header_files + _types.inline_files

import re

regex = re.compile ("^\s*#\s*include\s*(\/\*\*\/){0,1}\s*\"ace\/streams\.h\"")
begin_exclude = re.compile ("FUZZ\: disable check_for_streams_include")
end_exclude = re.compile ("FUZZ\: enable check_for_streams_include")

error_message = ": warning: expensive ace/streams.h included; consider ace/iosfwd.h\n"

from _generic_handler import generic_handler
def handler (file_name, file_content):
    return generic_handler (regex, begin_exclude,
                            end_exclude, error_message,
                            file_name, file_content, True)

