#!/usr/bin/python 

import sys
import pdb
from HTMLParser import HTMLParser
from urllib import urlopen
from urlparse import urljoin, urlparse
from time import gmtime, strptime, strftime
from optparse import OptionParser
from difflib import unified_diff
from os import stat, access, F_OK, makedirs, remove
from os.path import split, expanduser
from shutil import copyfileobj

class Logger:
    """
    Simple logging facility
    """
    def __init__ (self, verbosity):
        self.verbosity = verbosity

    def debug (self, message):
        if self.verbosity > 1:
            print message

    def info (self, message):
        if self.verbosity > 0:
            print message

    def warn (self, message):
        if self.verbosity > 0:
            print message

    def error (self, message):
        print message


## Abstract base class implementing basic bookkeeping tasks
## like maintaing context information about the tags we care
## about, etc.

class AbstractParser (HTMLParser):

    def __init__ (self, baseurl=None, logger=None):
        HTMLParser.__init__ (self)
        self.base_url = baseurl
        self.context = []
        if logger == None:
            self.log = Logger (0)
        else:
            self.log = logger

    def opencached (self, url, c):
        (cachepath, cachefile) = split (c)
        
        iscached = True
        if not access (cachepath, F_OK):
            self.log.info ("Create dirs=%s" % (cachepath))
            makedirs (cachepath)
            iscached = False

        if not access (c, F_OK):
            self.log.info ("Create cache file=%s" % (c))
            open (c, 'a').close()
            iscached = False

        # Determine the time last modified either from the HTTP
        # headers or from the last modified date of the cache file (if
        # no connection is available)
        try:
            f = urlopen (url)
            lastmod = parse_date (f.info().get ('last-modified'))
            return (f, lastmod)
        except IOError:
            if self.verbose_ > 1:
                print "(warning) Failed to retrieve headers, url=%s" % (url)

            if not iscached:
                if self.verbose_ > 0:
                    print "(error) No cache found, url=%s" % (url)
                raise
        
        return (open (c), gmtime (stat (c).st_mtime))

        
    def cached (self, url):
        """
        Returns the file object representing the URL from the
        cache. If not cached yet (or stale) the contents is
        downloaded.
        """
        parts = urlparse (url)
        p = parts.path.replace ('~', '')
        c = expanduser("~") + '/.diff-builds' + p

        (f, lastmod) = self.opencached (url, c)

        # Determine the last cached time stamp
        try:
            st = stat (c)
            lastcached = gmtime (st.st_mtime)

            if lastmod > lastcached or st.st_size < 1: 
                self.log.debug ("Caching, url=%s" % (url))

                tgt = open (c, 'w+')
                try:
                    copyfileobj(f, tgt)
                finally:
                    tgt.close ()
            else:
                self.log.debug ("Using cached url=%s" % (url))
        finally:
            f.close()

        return open (c)

        
    def parse (self, url=None):
        try:
            if not url:
                url = self.base_url
                
            f = self.cached (url)
            try:
                self.feed(f.read())
            finally:
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

    def __init__ (self, baseurl, filter_group, filter_build, logger=None):
        AbstractParser.__init__ (self, baseurl, logger)
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

    def __init__ (self, baseurl, logger=None):
        AbstractParser.__init__ (self, baseurl, logger)
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

    def __init__ (self, baseurl, logger=None):
        AbstractParser.__init__ (self, baseurl, logger)
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

    def __init__ (self, baseurl, logger=None):
        AbstractParser.__init__ (self, baseurl, logger)
        self.section = []
        self.errors = []
        self.warnings = []
        self.brief_url = []
        self.full_url = []
        self.column = -1

    def briefurl (self, section_name):
        for (i,s) in enumerate (self.section):
            if s == section_name:
                return self.brief_url[i]
        return None

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

    def __init__ (self, baseurl, logger=None):
        AbstractParser.__init__ (self, baseurl, logger)
        self.failed = []
        self.ready = False

    def istag (self, tagname):
        return (['h2', 'h3'].count (tagname) > 0)

    def data (self,content):
        if self.context[-1] == 'h2' and content.lower() == 'test':
            self.ready = True

        if self.context[-1] == 'h3' and self.ready:
            self.failed.append (content)



def format_date (t):
    if t == None or len (t) != 9:
        return '(unknown)'
    else:
        return strftime("%Y-%m-%d %H:%M", t)

def parse_date (s):

    if s == None:
        return None

    timeformats = [ 
        '%a, %d %b %Y %H:%M:%S %Z',
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
    


scoreboards = {"oci":"http://scoreboard.ociweb.com/oci-all.html", 
               "doc":"http://www.dre.vanderbilt.edu/scoreboard/integrated.html"}

def main ():
    parser = OptionParser()
    parser.add_option("-D", "--date", action="append", dest="dates",
                      help="Date to use in comparisons. One can be specified "
                      + "for each build provided on the command line  [default=today]")
    parser.add_option("-g", "--group", action="append",  dest="groups",
                      default=[],
                      help="Filter builds by their scoreboard group. Can be used "
                      + " multiple times [default=all]")
    parser.add_option("-d", "--diff", action="store_true",  dest="isdiff",
                      default=True, help="Compare tests failures [default=true]")
    parser.add_option("-l", "--list", action="store_true",  dest="islist",
                      help="Compare tests failures [default=false]")
    parser.add_option("-v", action="store_true", dest="verbose",
                      help="Make a bit more noise [default=true]")
    parser.add_option("-S", "--scoreboard", action="store", dest="scoreboard",
                      default="doc",
                      help="Specify the scoreboard URL to use. Can be either a "
                      + "proper URL, or one of the shortcuts (oci, doc), which "
                      + "designates respectively, the OCI's or the DOC's scoreboard "
                      + "[default=doc]")

    (options, builds) = parser.parse_args()
    log = Logger(options.verbose)

    log.debug ("Builds=%s" % (builds))
    
    for key in scoreboards.keys():
        if options.scoreboard.startswith (key):
            options.scoreboard = scoreboards[options.scoreboard]
            break
        
    if options.dates == None or len (options.dates) == 0:
        options.dates = []
    else:
        options.dates = [parse_date (d) for d in options.dates]

    # what do we need to do?
    if options.islist:
        ip = IndexParser (options.scoreboard, options.groups, builds, log)
        ip.parse ()
        
        for i in range (len (ip.build)):
            log.info ("%s [%s]" % (ip.build[i].name, ip.build[i].group))
 
            hp = HistoryParser (ip.build[i].historyurl, log)
            parse_with_retry (hp, ip.build[i].historyurl)


    elif options.isdiff:
        ip = IndexParser (options.scoreboard, options.groups, builds, log)
        ip.parse ()
        
        ## Determine build times and brief test error's URL for each build
        order = []
        testlogs = []
        times = []
        for i in range (len (ip.build)):
            log.info ("(%d) build=%s, group=%s" % (i, ip.build[i].name, ip.build[i].group))

            ## index page may have the builds in different order
            order.append (find_index (builds, ip.build[i].name))

            hp = HistoryParser (ip.build[i].historyurl, log) 
            parse_with_retry (hp, ip.build[i].historyurl) 
            
            log.debug ("options.dates %s" % options.dates)

            btimes = diff_dates (options.dates, hp.timestamp)
            log.debug ("build: %s, times:  %s" % (ip.build[i].name, 
                                                  [format_date(t) for t in btimes]))
                                   
            if None in btimes:
                log.error ("%s dates %s not in %s" % (ip.build[i].name, 
                                                      [format_date(t) for t in btimes],
                                                      [format_date(t) for t in hp.timestamp]))
            else:
                times.append (btimes)

                log_abbrev_index = [find_index (hp.timestamp, btimes[0]),
                                    find_index (hp.timestamp, btimes[1])]

                log.info ("timestamp indexes [%d, %d]" % (log_abbrev_index[0],
                                                          log_abbrev_index[1]))

                dp0 = DailyParser (hp.build_log_index[log_abbrev_index[0]], log)
                dp0.parse()
                
#                print "info: %s -> %s" % (format_date (btimes[0]),
#                                          dp0.brief_url[5])

                dp1 = DailyParser (hp.build_log_index[log_abbrev_index[1]], log)
                dp1.parse()
#                print "info: %s -> %s" % (format_date (btimes[1]),
#                                               dp1.brief_url[5])

                testlogs.append ([dp0.briefurl('test'), dp1.briefurl('test')])
                
        log.debug ("testlogs=%s\n" % testlogs)
        log.debug ("times=%s\n" % times)
        log.debug ("order=%s\n" % order)

        if len (ip.build) > 1:
            (first, second) = order
            l = [testlogs[first][1], testlogs[second][1]]
            t = [times[first][1], times[second][1]]

            log.debug ("l=%s\n" % l)
            log.debug ("t=%s\n" % t)

            bp0 = BriefParser (l[0], log)
            bp0.parse()

            log.info ("%s -> %s" % (format_date (t[0]),
                                    bp0.failed))

            bp1 = BriefParser (l[1], log)
            bp1.parse()

            log.info ("%s -> %s" % (format_date (t[1]),
                                    bp1.failed))

            for l in unified_diff (bp0.failed, bp1.failed, 
                                   l[0], l[1], 
                                   format_date (t[0]), format_date (t[1])):
                print l


def timestamps_for_build (build_name):
    ip = IndexParser (integrated_url)
    ip.parse ()
    ix = ip.build_name.find (build_name)
    if ix == -1:
        return -1

    hp = HistoryParser (ip.build_history_url[ix])
    hp.parse()
    print hp.timestamp
    
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

def testDaylyLog():
    url ="http://scoreboard.ociweb.com/doc_iliyan-gestalt_mosquito_linux_gcc_d1o0/2008_02_10_12_25_Totals.html"

    dp0 = DailyParser (url)
    dp0.parse()
    print "info: %s -> %s" % (url, dp0.briefurl('test'))
    print "info: section=%s" % dp0.section
    print "info: briefs=%s" % dp0.brief_url
    

if __name__ == '__main__':
#    sys.exit (sample())
    sys.exit (main())
#    pdb.run ("main()");
#    pdb.run ("testDaylyLog()")
