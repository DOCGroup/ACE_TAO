from HTMLParser import HTMLParser
from urllib import urlopen

class MyHTMLParser(HTMLParser):

    def handle_starttag(self, tag, attrs):
        print "Encountered the beginning of a %s tag" % tag

    def handle_endtag(self, tag):
        print "Encountered the end of a %s tag" % tag



parser = MyHTMLParser()
#parser.feed(open ("/tmp/integrated.html").read ())

integrated_url = "http://www.dre.vanderbilt.edu/scoreboard/integrated.html"
parser.feed(urlopen (integrated_url).read ())
