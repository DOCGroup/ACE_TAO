""" Checks that the file does not contain SVN conflict markers """


type_list = ["*"]

from sys import stderr
import re

regex = re.compile ("^<<<<<<< \.|>>>>>>> \.r|^=======$", re.MULTILINE)
begin_exclude = re.compile ("FUZZ\: disable conflict_marker_check")
end_exclude = re.compile ("FUZZ\: enable conflict_marker_check")

error_message = ": error: contains a SVN conflict marker.  Please resolve the conflict before committing.\n"

from _generic_handler import generic_handler

def handler (file_name, file_content):
    return generic_handler (regex, begin_exclude, end_exclude, error_message, file_name, file_content)


