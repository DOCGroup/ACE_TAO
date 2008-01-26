#!/usr/bin/python 

import sys
import pdb
from HTMLParser import HTMLParser
from urllib import urlopen
from urlparse import urljoin
from time import strptime

## Searches the build index for groups
## of builds denoted by:
##
## <h2>
##   <a name="GROUP">GROUP</a>
## </h2>
## BUILDTABLE
##
## Groups are folowed by a BUILDTABLE denoted by:
##
## <table border="1">
##   <tbody>
##     <tr>HEADER</tr>
##     <tr>BUILD0</tr>
##     <tr>BUILD1</tr>
##   </tbody>
## <table>
##
## Where BUILDn is:
##
## <td><a href="BUILDHISTORYURL">NAME</a></td> ...

class IndexParser (HTMLParser):

    def __init__ (self, baseurl=None):

        HTMLParser.__init__ (self)
        self.tags = ['h2', 'a', 'td', 'tr']
        self.group_name = []
        self.build_history_url = []
        self.build_name = []
        self.build_group = []
        self.build_brief = []
        self.context = []
        self.column = -1
        self.base_url = baseurl
        
    def handle_starttag (self,name,a):

        if self.tags.count (name) == 0:
            return

        self.context.append (name)
        
        if name == 'tr':
            self.column = -1
            return

        if name == 'td':
            self.column += 1
            return

        if name == 'a' and \
           len (self.context) > 1 and \
           self.context[-2] == 'td':

            attrs = dict (a)
            href = attrs['href']
            url = urljoin(self.base_url, href)
            if self.column == 0:
                self.build_history_url.append (url)
            elif self.column == 5:
                if href.find ('Brief'):
                    self.build_brief.append (url)

        return


    def handle_data (self,content):

        if len (self.context) < 2:
            return

        content = content.strip ()
        if len (content) == 0:
            return
        
        if len (self.context) > 1 and \
            self.context[-1] == 'a' and \
            self.context[-2] == 'h2':

            self.group_name.append (content)
            return
        
        if len (self.context) > 1 and \
            self.context[-1] == 'a' and \
            self.context[-2] == 'td':
            
            if self.column == 0:
                self.build_name.append (content)
                self.build_group.append (self.group_name[-1])

            return
        
        return

    def handle_endtag (self,name):

        if self.tags.count (name) == 0:
            return
        
        self.context.pop()
        



## Searches a specific build for available results
## denoted by:
##
## <table border="1">
##   <tbody>
##     <tr>HEADER</tr>
##     <tr>TIMESTAMP0</tr>
##     <tr>TIMESTAMP1</tr>
##   </tbody>
## <table>
##
## Where TIMESTAMPn is:
##
## <td><a href="TIMESTAMPURL">TIMESTAMP</a></td> ...

class DatesParser (HTMLParser):

    def __init__ (self, baseurl=None):

        HTMLParser.__init__ (self)
        self.tags = ['a', 'td', 'tr']
        self.timestamp = []
        self.timestamp_url = []
        self.context = []
        self.column = -1
        self.base_url = baseurl
        
    def handle_starttag (self,name,a):

        if self.tags.count (name) == 0:
            return

        self.context.append (name)
        
        if name == 'tr':
            self.column = -1
            return

        if name == 'td':
            self.column += 1
            return

        if name == 'a' and \
           len (self.context) > 1 and \
           self.context[-2] == 'td' and \
           self.column == 0:

            attrs = dict (a)
            url = urljoin(self.base_url, attrs['href'])
            self.timestamp_url.append (url)

        return


    def handle_data (self,content):

        if len (self.context) < 2:
            return

        content = content.strip ()
        if len (content) == 0:
            return
        
        if len (self.context) > 1 and \
            self.context[-1] == 'a' and \
            self.context[-2] == 'td' and \
            self.column == 0:

            t = strptime (content, "%Y_%m_%d_%H_%M")
            self.timestamp.append (t)

        return

    def handle_endtag (self,name):

        if self.tags.count (name) == 0:
            return
        
        self.context.pop()
        

## Searches a specific (brief) build log for test results
## denoted by:
##
## <table border="1">
##   <tbody>
##     <tr>HEADER</tr>
##     <tr>TIMESTAMP0</tr>
##     <tr>TIMESTAMP1</tr>
##   </tbody>
## <table>
##
## Where TIMESTAMPn is:
##
## <td><a href="TIMESTAMPURL">TIMESTAMP</a></td> ...

class HistoryParser (HTMLParser):

    def __init__ (self, baseurl=None):

        HTMLParser.__init__ (self)
        self.tags = ['a', 'td', 'tr']
        self.timestamp = []
        self.build_log_index = []
        self.context = []
        self.column = -1
        self.base_url = baseurl
        
    def handle_starttag (self,name,a):

        if self.tags.count (name) == 0:
            return

        self.context.append (name)
        
        if name == 'tr':
            self.column = -1
            return

        if name == 'td':
            self.column += 1
            return

        if name == 'a' and \
           len (self.context) > 1 and \
           self.context[-2] == 'td' and \
           self.column == 0:

            attrs = dict (a)
            url = urljoin(self.base_url, attrs['href'])
            self.build_log_index.append (url)

        return


    def handle_data (self,content):

        if len (self.context) < 2:
            return

        content = content.strip ()
        if len (content) == 0:
            return
        
        if len (self.context) > 1 and \
            self.context[-1] == 'a' and \
            self.context[-2] == 'td' and \
            self.column == 0:

            t = strptime (content, "%Y_%m_%d_%H_%M")
            self.timestamp.append (t)

        return

    def handle_endtag (self,name):

        if self.tags.count (name) == 0:
            return
        
        self.context.pop()
        



integrated_url = "http://www.dre.vanderbilt.edu/scoreboard/integrated.html"

dates_url = "http://acebuilds.riverace.com/cuddy/dev/va6-64_debug/index.html"

def main():
    ip = IndexParser ()
    try:
        f = urlopen(integrated_url)
        try:
            ip.feed(f.read())
        finally:
            f.close()
    finally:
        ip.close()
    
    dp = HistoryParser (dates_url)
    try:
        f = urlopen(ip.build_history_url[0])
        try:
            dp.feed(f.read())
        finally:
            f.close()
    finally:
        dp.close()

    
    print dp.timestamp
    print dp.build_log_index
    return 0


if __name__ == '__main__':
#  sys.exit (main())
  pdb.run ("main()");
