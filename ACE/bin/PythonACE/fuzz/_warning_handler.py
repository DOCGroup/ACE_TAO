""" Implements a warning handler base class and a simple handler that simply
    outputs warnings to stderr. """

import _singleton

# Constants
STDERR = 1
MAILER = 2

from sys import stderr

class Warning_Handler (_singleton.Singleton):
    def __init__(self, handlertype=STDERR):
        """ Constructor. Type should be either STDERR or MAILER.
            There are probably better ways to do this, but it is implemented
            this way because I only wanted to have a dependancy on ldap if someone
            actually wanted to use the mailer. """
        super (Warning_Handler, self).__init__ ()
        self.messages = ""
        self.add_warning = self.default_add_warning
        self.close = self.default_close
        self.open = self.default_open
        
        if handlertype is STDERR:
            self.add_warning = self.stderr_add_warning
        elif handlertype is MAILER:
            from _mailer import Mailer
            self.handler = Mailer ()
            self.add_warning = self.handler.add_warning
            self.open = self.handler.open
            self.close = self.handler.close
            self.get_messages = self.handler.get_messages
        else:
            self.add_warning = self.stderr_add_warning

    def default_add_warning (self, warning_text):
        pass

    def default_open (self, arg = ""):
        pass

    def default_close (self, arg = ""):
        pass

    def get_messages (self):
        return self.messages
    
    def stderr_add_warning (self, warning_text):
        stderr.write (warning_text)
        return
    

    
