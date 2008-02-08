#!/usr/bin/python 

import sys
import pdb
from HTMLParser import HTMLParser
from urllib import urlopen
from urlparse import urljoin
from time import gmtime, strptime, strftime
from optparse import OptionParser
from difflib import unified_diff

## Abstract base class implementing basic bookkeeping tasks
## like maintaing context information about the tags we care
## about, etc.

class AbstractParser (HTMLParser):

    def __init__ (self, baseurl=None):
        HTMLParser.__init__ (self)
        self.base_url = baseurl
        self.context = []
#        print "(debug) Parser: %s ..." % baseurl

    def parse (self, url=None):
        try:
            if not url:
                url = self.base_url
                
            f = urlopen(url)
            try:
 #               print "(debug) Opening %s ..." % url
                self.feed(f.read())
            finally:
 #               print "(debug) Closing %s ..." % url
                f.close()
        finally:
            self.close()

    def prefix (self, name, a):
        return

    def data (self, content):
        return

    def postfix (self, name):
        return

    def istag (self, tagname): 
        return True

    def handle_starttag (self,name,a):
        if self.istag (name):
            self.context.append (name)
            self.prefix (name, a)


    def handle_data (self,content):
        if len (self.context) >= 1:
            content = content.strip ()
            if len (content) > 0:
                self.data (content)

        
    def handle_endtag (self,name):
        if self.istag (name):
            try:
                self.postfix (name)
            finally:
                self.context.pop()


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

class Build:
    def __init__ (self, group, historyurl):
        self.group = group
        self.historyurl = historyurl
        self.name = None
        self.briefurl = None
        self.filtered = False

class IndexParser (AbstractParser):

    def __init__ (self, baseurl, filter_group, filter_build):
        AbstractParser.__init__ (self, baseurl)
        self.group_name = []
        self.build = []
        self.column = -1
        self.current_group = None
        self.filter_group = filter_group
        self.filter_build = filter_build
        
    def istag (self, tagname):
        return (['h2', 'a', 'td', 'tr'].count (tagname) > 0)
        
    def prefix (self,name,a):
        if name == 'tr':
            self.column = -1
        elif name == 'td':
            self.column += 1
        elif name == 'a':
            if len (self.context) > 1 and self.context[-2] == 'td':
                attrs = dict (a)
                href = attrs['href']
                url = urljoin(self.base_url, href)
                if self.column == 0:
  #                  print "\n(debug) adding build for: %s" % url
                    self.build.append (Build (self.current_group, url))
                elif self.column == 5:
                    if href.find ('Brief'):
                        self.build[-1].briefurl = url

    def postfix (self,name):
        if name == 'tr':
            self.column = -1
            if len (self.build) > 0 and self.build[-1].filtered:
                self.build.pop ()

    def data (self,content):
        if len (self.context) >= 2:
            if self.context[-1] == 'a':
                if self.context[-2] == 'h2':
                    self.addGroup (content)
                elif self.context[-2] == 'td' and self.column == 0:
                    self.addBuild (content)


    def addGroup (self, g):
 #       print "(debug) group: %s" % g
        self.current_group = g
        if len (self.filter_group) > 0 and not g in self.filter_group:
            pass
        else:
            self.group_name.append (g)


    def addBuild (self, b):
        filtered = ((len (self.filter_build) > 0 and 
                     not b in self.filter_build) 
                    or (len (self.filter_group) > 0 and 
                        not self.current_group in self.filter_group))
        self.build[-1].filtered = filtered
        self.build[-1].name = b

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

class DatesParser (AbstractParser):

    def __init__ (self, baseurl):
        AbstractParser.__init__ (self, baseurl)
        self.timestamp = []
        self.timestamp_url = []
        self.column = -1
        
    def istag (self, tagname):
        return (['a', 'td', 'tr'].count (tagname) > 0)

    def prefix (self,name,a):
        if name == 'tr':
            self.column = -1
            return

        if name == 'td':
            self.column += 1
            return

        if name == 'a' and len (self.context) > 1 and self.context[-2] == 'td' and self.column == 0:
            attrs = dict (a)
            url = urljoin(self.base_url, attrs['href'])
            self.timestamp_url.append (url)


    def data (self,content):
        if len (self.context) > 1 and \
            self.context[-1] == 'a' and \
            self.context[-2] == 'td' and \
            self.column == 0:

            t = strptime (content, "%Y_%m_%d_%H_%M")
            self.timestamp.append (t)

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

class HistoryParser (AbstractParser):

    def __init__ (self, baseurl):
        AbstractParser.__init__ (self, baseurl)
        self.timestamp = []
        self.build_log_index = []
        self.column = -1
        
    def istag (self, tagname):
        return (['a', 'td', 'tr'].count (tagname) > 0)

    def prefix (self,name,a):
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


    def data (self,content):
        if len (self.context) > 1 and \
                self.context[-1] == 'a' and \
                self.context[-2] == 'td' and \
                self.column == 0:
            
            t = strptime (content, "%Y_%m_%d_%H_%M")
            self.timestamp.append (t)


## Searches the index page of a build for a specific day
## to extract information about each section: begin, setup, 
## config, compile, test, and end. The sections are denoted by:
##
## <table border="1">
##   <tbody>
##     <tr>HEADER</tr>
##     <tr>SECTION0</tr>
##     <tr>SECTION1</tr>
##   </tbody>
## <table>
##
## Where SECTIONn is:
## ...
## <td>NAME</td>
## <td><a href="FULLURL">Full</a><a href="BRIEFURL">Brief</a></td>
## <td>TOTALSUBSECTIONS</td>
## <td>TOTALERRORS</td>
## <td>TOTALWARNINGS</td>
## ...

class DailyParser (AbstractParser):

    def __init__ (self, baseurl):
        AbstractParser.__init__ (self, baseurl)
        self.section = []
        self.errors = []
        self.warnings = []
        self.brief_url = []
        self.full_url = []
        self.column = -1
        
    def istag (self, tagname):
        return (['a', 'td', 'tr'].count (tagname) > 0)

    def prefix (self,name,a):
        if name == 'tr':
            self.column = -1
            return

        if name == 'td':
            self.column += 1
            return

        if len (self.context) > 1 and self.context[-2] == 'td' :
            if self.context[-1] == 'a' and self.column == 1:
                attrs = dict (a)
                url = urljoin(self.base_url, attrs['href'])
                
                b = f = None
                if len (self.section) == len (self.brief_url):
                    b = self.brief_url.pop ()
                    f = self.full_url.pop ()
                    
                if url.find ('_Brief.html') == -1:
                    f = url
                else:
                    b = url

                self.brief_url.append (b)
                self.full_url.append (f)


    def data (self,content):
        if self.context[-1] == 'td' :
            if self.column == 0:
                self.section.append (content.lower())
            elif self.column == 3:
                self.errors.append (content)
            elif self.column == 4:
                self.warnings.append (content)

## Parses the 'brief' tests failure file to extract information about the failing
## tests. The failures are denoted by:
##
##  <h2>Test</h2>
##      <a name="subsection_2098"></a><hr>
##      <h3>tests/Bug_2820_Regression_Test</h3>
## ...

class BriefParser (AbstractParser):

    def __init__ (self, baseurl):
        AbstractParser.__init__ (self, baseurl)
        self.failed = []
        self.ready = False

    def istag (self, tagname):
        return (['h2', 'h3'].count (tagname) > 0)

    def data (self,content):
        if self.context[-1] == 'h2' and content.lower() == 'test':
            self.ready = True

        if self.context[-1] == 'h3' and self.ready:
            self.failed.append (content)

integrated_url = "http://www.dre.vanderbilt.edu/scoreboard/integrated.html"
#dates_url = "http://acebuilds.riverace.com/cuddy/dev/va6-64_debug/index.html"
#    brieflog = "http://acebuilds.riverace.com/cuddy/dev/va6-64_debug/2008_01_25_10_31_Brief.html#section_6"
    
def list_timestamps_for_build (b):
    ip = IndexParser (integrated_url)
    ip.parse ()
    ix = ip.build_name.find (b)
    if ix == -1:
        return -1

    hp = HistoryParser (ip.build_history_url[ix])
    hp.parse()
    print hp.timestamp
    
def format_date (t):
    if t == None or len (t) != 9:
        return '(unknown)'
    else:
        return strftime("%Y-%m-%d %H:%M", t)

def parse_date (s):
    timeformats = [ 
        '%Y_%m_%d %H:%M',
        '%Y-%m-%d %H:%M',
        '%Y%m%d%H%M',
        '%Y_%m_%d %H',
        '%Y-%m-%d %H',
        '%Y%m%d%H',
        '%Y_%m_%d',
        '%Y-%m-%d',
        '%Y%m%d',
        '%Y_%m',
        '%Y-%m',
        '%Y%m',
        '%Y',
        ]

    for tf in timeformats:
        try:
            return strptime(s, tf)
        except ValueError:
            pass

    return None


def nearest_after (ts, available_sorted):
    """ 
    Returns the closest date, that's not before ts, or None
    """
    for (i,a) in enumerate (available_sorted):
        if a >= ts:
            return available_sorted[i]

    return None


def find_index (array, what):
    for i in range (len(array)):
        if what == array[i]:
            return i
    return -1

def diff_dates (timestamps, available_unsorted):
    # available is a list of timestamps that are available.
    # timestamps contains up to two timestamps to filter with.
    available = sorted (available_unsorted)
    if len (timestamps) == 0 and len (available) >= 2:
        # if none specified - compare, using the one before last with the last
        return [available[-2], available[-1]]
    elif len (timestamps) == 1 and len (available) >= 1:
        return [nearest_after (timestamps[0], available), 
                available[-1]]
    elif len (timestamps) >= 2 and len (available) >= 1:
        return [nearest_after (timestamps[0], available), 
                nearest_after (timestamps[1], available)]

    return [None, None]


def parse_with_retry (parser, what):
    return parser.parse()
#    while True:
#        try:
#            return parser.parse()
#        except:
#            print "Cant access %s. Retry/Stop/Continue [r/s/c]?" % (what)
    

def main ():
    parser = OptionParser()
    parser.add_option("-D", "--date", action="append", dest="dates",
                      help="date to use in comparisons  [default=today]")
    parser.add_option("-g", "--group", action="append",  dest="groups",
                      default=[],
                      help="consider only builds from specific group(s) [all]")
    parser.add_option("-b", "--build", action="append",  dest="builds",
                      default=[],
                      help="builds to compare [all]")
    parser.add_option("-v", action="store_true", dest="verbose",
                      help="make lots of noise [default=true]")

    (options, args) = parser.parse_args()
    
    if options.dates == None or len (options.dates) == 0:
        options.dates = []
    else:
        options.dates = [parse_date (d) for d in options.dates]

    # what do we need to do?
    if len (args) > 0 and (args[0] == 'list' or args[0] == 'l'):
        ip = IndexParser (integrated_url, options.groups, options.builds)
        ip.parse ()
        
        for i in range (len (ip.build)):
            print "%s [%s]" % (ip.build[i].name, ip.build[i].group)
 
            hp = HistoryParser (ip.build[i].historyurl)
            parse_with_retry (hp, ip.build[i].historyurl)


    elif len (args) > 0 and (args[0] == 'diff' or args[0] == 'd'):
        ip = IndexParser (integrated_url, options.groups, options.builds)
        ip.parse ()
        
        logs = []
        for i in range (len (ip.build)):
            print "%s [%s]" % (ip.build[i].name, ip.build[i].group)
 
            hp = HistoryParser (ip.build[i].historyurl) 
            parse_with_retry (hp, ip.build[i].historyurl) 
            
            print "options.dates %s" % options.dates

            times = diff_dates (options.dates, hp.timestamp)
            print "%s: %s" % (ip.build[i].name, 
                              [format_date(t) for t in times])
                                   
            if None in times:
                print "error: %s dates %s not in %s" % (ip.build[i].name, 
                                                        [format_date(t) for t in times],
                                                        [format_date(t) for t in hp.timestamp])
            else:
                log_abbrev_index = [find_index (hp.timestamp, times[0]),
                                    find_index (hp.timestamp, times[1])]

                print "info: indexes [%d, %d]" % (log_abbrev_index[0],
                                                  log_abbrev_index[1])

                dp0 = DailyParser (hp.build_log_index[log_abbrev_index[0]])
                dp0.parse()
                print "info: %s -> %s" % (format_date (times[0]),
                                          dp0.brief_url[5])

                bp0 = BriefParser (dp0.brief_url[5])
                bp0.parse()

#                print "info: %s -> %s" % (format_date (times[0]),
#                                          bp0.failed)

                dp1 = DailyParser (hp.build_log_index[log_abbrev_index[1]])
                dp1.parse()
                print "info: %s -> %s" % (format_date (times[1]),
                                               dp1.brief_url[5])

                bp1 = BriefParser (dp1.brief_url[5])
                bp1.parse()

#                print "info: %s -> %s" % (format_date (times[1]),
#                                          bp1.failed)

                for l in unified_diff (bp0.failed, bp1.failed):
                    print l


def sample():
    ip = IndexParser (integrated_url, [], [])
    ip.parse ()

    hp = HistoryParser (ip.build_history_url[0])
    hp.parse()

#    print hp.timestamp
#    print hp.build_log_index

    dp = DailyParser (hp.build_log_index[0])
    dp.parse()

#   print dp.section
#   print dp.errors
#   print dp.warnings
    print dp.brief_url
    print dp.full_url
    
    bp = BriefParser (dp.brief_url[5])
    bp.parse()

    print bp.failed
    return 0


if __name__ == '__main__':
#    sys.exit (sample())
#    sys.exit (main())
    pdb.run ("main()");
