# $Id$
from templet import stringfunction

@stringfunction
def template (name, deployrequirement, endpoint_one, endpoint_two) :
    """
    <connection>
      <name>${name}</name>
      ${deployrequirement}
      ${endpoint_one}
      ${endpoint_two}
    </connection>
    """
