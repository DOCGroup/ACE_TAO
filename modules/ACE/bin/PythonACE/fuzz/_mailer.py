""" This module implements a mailer to mail a user about fuzz warnings """

import _singleton

def ldap_lookup (username):
    """ Performs a ldap lookup to find the email address associated with
        username.  If none exists, it returns the empty string."""
    import ldap

    try:
        conn = ldap.open ("ldap.dre.vanderbilt.edu")
        conn.protocol_version = ldap.VERSION3

        baseDN = "dc=dre,dc=vanderbilt,dc=edu"
        scope = ldap.SCOPE_SUBTREE
        attrFilter = None
        searchFilter = "uid=" + username

        result = conn.search (baseDN, scope, searchFilter, attrFilter)

        result_type, result_data = conn.result (result, 0)
        email = ""
        if (result_data != []) and (result_type == ldap.RES_SEARCH_ENTRY):
            # we have a valid result!
            if (result_data[0][1].has_key ('mail')):
                email = result_data[0][1]['mail'][0]
            elif (result_data[0][1].has_key ('svnmail')):
                email = result_data[0][1]['svnmail'][0]
            else:
                email = ""

        conn.unbind ()

        return email
    except:
        # Some error occurred when looking this guy up.
        return ""

            
                    
class Mailer:
    def __init__ (self):
        self.recipient = ""
        self.body = """\
This is an automatically generated message from the fuzz check system
in the subversion repository.

Your recent commit to the ACE/TAO/CIAO repository had a number of warnings
which should be addressed.

"""
        self.warnings  = ""
        self.subject = "Your recent commit to the DOC group repository."
        self.sent = False

    def get_messages (self):
        return self.warnings
    def open (self, ldap_user_name):
        from sys import stderr
        stderr.write ("LDAP Name: " + ldap_user_name.rstrip () + "\n")
        self.recipient = ldap_lookup (ldap_user_name.rstrip ())

    def add_warning (self, warning_text):
        self.warnings += warning_text

    def close (self):
        try:
            message = """\
From: %s
To: %s
Subject: %s
\r\n
%s
"""         % ("bczar@dre.vanderbilt.edu",
               self.recipient,
               self.subject,
               self.body + self.warnings)
            
            print message
            
            import smtplib
            server = smtplib.SMTP('discovery.isis.vanderbilt.edu')
            server.sendmail ("bczar@dre.vanderbilt.edu",
                             [self.recipient],
                             message)
        except smtplib.SMTPRecipientsRefused:
            print "Recipients refused exception"
            server.close ()
        except smtplib.SMTPHeloError:
            print "Helo error"
            server.close ()
        except smtplib.SMTPSenderRefused:
            print "Sender refused"
            server.close ()
        except smtplib.SMTPDataError:
            print "Data error"
            server.close ()
        except:
            from sys import stderr
            stderr.write ("Caught exception while sending email\n")
            server.close ()




    
