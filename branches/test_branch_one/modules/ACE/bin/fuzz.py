#!/usr/bin/env python
""" Implements a command line client for the Python Fuzz module, much like fuzz.pl """

def parse_args ():
    from optparse import OptionParser

    parser = OptionParser ("usage %prog [options] <files or directories to check>")

    parser.add_option ("--exclude-dirs", dest="exclude_dir", action="append", default=list (),
                       help="A regular expression that when matched, will cause directories to be skipped.")
    parser.add_option ("--exclude-files", dest="exclude_file", action="append", default=list (),
                       help="A regular expression that when matched, will cause files to be skipped.")
    
    return parser.parse_args ()


import PythonACE.fuzz

class Fuzz_Client:
    def __init__ (self, opts, args):
        import re
        
        self.ex_dirs = map (re.compile, opts.exclude_dir)
        self.ex_files = map (re.compile, opts.exclude_file)
        self.args = args

    def walk_dir (self, directory):
        import os
        
        for root, dirs, files in os.walk (directory):
            # Prune out .svn directories
            for item in dirs:
                if item == ".svn":
                    dirs.remove (item)

                # Prune out exclusions
                for regex in self.ex_dirs:
                    if regex.search (item) != None:
                        dirs.remove (item)
            
            for item in files:
                for regex in self.ex_files:
                    if regex.serch (item) != None:
                        continue
                self.check_file (os.path.join (root, item))
                
    def check_file (self, the_file):
        f = open (the_file, 'r')
        PythonACE.fuzz.fuzz_check (the_file, f.read ())
        f.close ()
        
    def main (self):
        for item in self.args:
            import os.path
            if os.path.isfile (item):
                self.check_file (item)
            elif os.path.isdir (item):
                self.walk_dir (item)
            else:
                print item + " is not a file or directory."

if __name__ == "__main__":
    opts, args = parse_args ()
    Fuzz = Fuzz_Client(opts, args)
    Fuzz.main ()
    
