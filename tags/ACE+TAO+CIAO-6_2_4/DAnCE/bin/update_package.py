#!/usr/bin/env python
# $Id$
""" This script facilitates the update of binaries in a given DAnCE package.
    Note that it DOES NOT modify meta-data, only blindly replacing existing
    binaries that match by name EXACTLY"""

#Globals
verbose = False
take_action = True

def parse_args ():
	from optparse import OptionParser
		
	parser = OptionParser ("usage: %prog [options] package_to_update")
		
	parser.add_option ("-I", "--include-dir", dest="include_dir", action="store",
						help="Specify a path to search for new implementations/descriptors",
						default="./impl/")
	parser.add_option ("-o", "--output-dir", dest="out_dir", action="store",
						help="Specify the directory to write updated packages",
						default="./new_packages/")
	parser.add_option ("-f", "--out-file", dest="out_file", action="store",
						help="Specify the name of the file to output.  Default is same name",
						default=None)
	parser.add_option ("-v", "--verbose", action="store_true", default=False,
						dest="verbose", help="Print verbose debugging output")
	parser.add_option ("-n", "--no-action", action="store_false")
	(opts, args) = parser.parse_args ()
		
	if len(args) != 1:
		parser.error ("Must specify exactly one package to update")
	
	if opts.out_file is None:
		opts.out_file = os.path.basename (args[0])
		
	return (opts, args)

def vprint (string):
	if verbose:
		print string

"""Caches all files in include paths"""
def cache_files (dirs):
	files = dict ()
	vprint ("Building file list")
	for item in dirs:
		for root, dirs, files in os.walk (item, topDown=True):
			for item in files:
				fullpath = os.join (root, item)
				if item not in files:
					vprint ("Adding " + item + " as " + fullpath)
					files[item] = fullpath
				else:
					vprint ("Skipping " + fullpath + " because " + item + " has been found earlier")
	
	return files

""" Update a package from infile, a ZipFile object to outfile, a ZipFile object,
    using files in the dictionary files"""
def update_package (infile, outfile, files):
	orig_list = infile.namelist ()
	
	for item in filelist:
		vprint ("Considering " + item + " from source package")
		
		if item[-1] == '/':
			vprint ("\tSkipping because its a directory")
		else:
			bn_item = os.path.basename (item)
		
			if bn_item in files.keys ():
				vprint ("\tFound replacement: " + files[bn_item])
				vprint ("\tPerforming replacement....")
				
				if take_action:
					ofile.write (files[bn_item], item)
				else:
					print "\tAction: Replace " + item + " with " + item
				continue # next item
				
		if take_action:
			ofile.writestr (item, zfile.read (item))
		else:
			print "\tAction:  Migrate " + item + " unchanged"
			
def main ():
	opts, args = parse_args ()
	
	from zipfile import ZipFile
	
	## Construct output file
	of_name = os.path.join (opts.out_path, opts.out_file)
	vprint ("Chose " + of_name + " as output file")
	
	#make sure out_path exists
	if os.path.exists (os.path.basename (of_name)):
		vprint ("Destination path does not exist, creating....")
		if take_action:
			os.mkdir (os.path.basename (of_name))
		else:
			print "Action: create path " + os.path.basename (of_name)

	# Create zipfile objects
	infile = ZipFile (args[0], 'a')
	outfile = None
	
	if take_action:
		outfile = ZipFile (of_name, 'w')
		
	# cache include directories
	files = cache_files (opts.include_dirs)
	
	# Perform update
	update_package (infile, outfile, files)
	
	print "Package successfully updated"
	
if __name__ == "__main__":
	main (opts, args)
