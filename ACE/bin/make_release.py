#!/usr/local/bin/python2.5

# @file make_release.py
# @author William R. Otte <wotte@dre.vanderbilt.edu>
#
# Packaging script for ACE/TAO/CIAO

from __future__ import with_statement
from time import strftime
import pysvn
import re

##################################################
#### Global variables
##################################################
""" Options from the command line """
opts=None

""" Arguments from the command line """
args=None

""" Absolute path from the SVN workspace to be used for the
release"""
doc_root=None

""" Full name of person performing release, obtained from the
environment"""
signature=None

""" Full email address of person performing release. """
mailid = None

""" A dict containing version information used for the release.
This dict contains entries of the form
COMPONENT_version
COMPONENT_beta
COMPONENT_minor
COMPONENT_major """
comp_versions = dict ()

release_date = strftime (# ie: Mon Jan 23 00:35:37 CST 2006
                              "%a %b %d %H:%M:%S %Z %Y")

# Packaging configuration

""" This is a regex that detects files that SHOULD NOT have line endings
converted to CRLF when being put into a ZIP file """
bin_regex = re.compile ("\.(mak|mdp|ide|exe|ico|gz|zip|xls|sxd|gif|vcp|vcproj|vcw|sln|dfm|jpg|png|vsd|bz2)$")


##################################################
#### SVN Client Hooks
##################################################
svn_auth_info = None
def svn_login_callback (realm, username, may_save):
    """ Callback used by the SVN library to obtain login credentials"""
    global svn_auth_info
    if svn_auth_info is None:
        print "Please enter your Subversion login credentials.  They will be saved for the duration of this script."
        username = raw_input ("Username: ")
        password = raw_input ("Password: ")

        svn_auth_info = (True, username, password, False)

    return svn_autn_info

def svn_log_message_callback ():
    """ Callback used by the svn library to generate log messages
    for operations such as copy """
    return (True, "ChangeLogTag:%s  %s  <%s>" % (release_date, signature, mailid))

svn_client = pysvn.Client ()
svn_client.callback_get_login = svn_login_callback
svn_client.callback_get_log_message = svn_log_message_callback

##################################################
#### Utility Methods
##################################################
def parse_args ():
    from optparse import OptionParser

    parser = OptionParser ("usage %prog [options]")

    parser.add_option ("--major", dest="release_type", action="store_const",
                       help="Create a major release.", default=None, const="major")
    parser.add_option ("--minor", dest="release_type", action="store_const",
                       help="Create a minor release.", default=None, const="minor")
    parser.add_option ("--beta", dest="release_type", action="store_const",
                       help="Create a beta release.", default=None, const="beta")


    parser.add_option ("--tag", dest="action", action="store_const",
                       help="Tag the release. DO NOT USE WITH --kit", default=None, const="tag")
    parser.add_option ("--update", dest="update", action="store_true",
                       help="Update the version numbers, only used with --tag", default=False)


    parser.add_option ("--kit", dest="action", action="store_const",
                       help="Create kits. DO NOT USE WITH --tag", default=None, const="kit")
    parser.add_option ("--dest", dest="package_dir", action="store",
                       help="Specify destination for the created packages.", default=None)

    parser.add_option ("--root", dest="repo_root", action="store",
                       help="Specify an alternate repository root",
                       default="https://svn.dre.vanderbilt.edu/DOC/")

    parser.add_option ("-n", dest="take_action", action="store_false",
                       help="Take no action", default=True)
    parser.add_option ("--verbose", dest="verbose", action="store_true",
                       help="Print out actions as they are being performed",
                       default=False)
    parser.add_option ("--override-host", dest="override_host", action="store_true",
                       help="Override the default release host.  Not reccomended", default=False)
    (options, arguments) = parser.parse_args ()

    if options.action is None:
        parser.error ("Must specify an action, ie --tag or --kit")

    if options.action == "tag":
        if options.release_type is None:
            parser.error ("When tagging, must specify a release type")

        if options.update is False:
            print "Warning: You are tagging a release, but not requesting a version increment"

    return (options, arguments)


def ex (command):
    from os import system
    global opts
    vprint ("Executing " + command)

    if not opts.take_action:
        print "Executing " + command
        return

    status = system(command)
    if status != 0:
        print "ERROR: Nonzero retrun value from " + command
        raise Exception

###
# Checks that the users environment is sane.
#
def check_environment ():
    from os import getenv

    global doc_root, signature, mailid, opts

    doc_root = getenv ("DOC_ROOT")
    if (doc_root is None):
        print "ERROR: Environment DOC_ROOT must be defined."
        return False

    signature = getenv ("SIGNATURE")
    if (signature is None):
        print "ERROR: Must define SIGNATURE environment variable to your full name, used in changelogs."
        return False

    mailid = getenv ("MAILID")
    if (mailid is None):
        print "ERROR: Must define MAILID environment to your email address for changelogs."
        return False

    from socket import gethostname

    if ((not opts.override_host) and gethostname () != "anduril.dre.vanderbilt.edu"):
        print "ERROR: Must run script on anduril.dre.vanderbilt.edu"
        return False

    return True

def vprint (string):
    """ Prints the supplied message if verbose is enabled"""
    global opts

    if opts.verbose:
        print string

##################################################
#### Tagging methods
##################################################
def commit (files):
    """ Commits the supplied list of files to the repository. """
    vprint ("Committing the following files: " + " ".join (files))

    rev = svn_client.checkin (files,
                              "ChangeLogTag:%s  %s  <%s>" % (release_date, signature, mailid))

    print "Checked in files, resuling in revision ", rev.number

def check_workspace ():
    """ Checks that the DOC and MPC repositories are up to date.  """
    global opts, doc_root, svn_client
    # @@TODO: Replace with a svn library
    try:
        rev = svn_client.update (doc_root)
        print "Successfully updated ACE/TAO/CIAO working copy to revision "
    except:
        print "Unable to update ACE/TAO/CIAO workspace at " + doc_root
        raise

    try:
        rev = svn_client.update (doc_root + "ACE/MPC")
        print "Successfully updated MPC working copy to revision "
    except:
        print "Unable to update the MPC workspace at " + doc_root + "/MPC"
        raise

def update_version_files (component):
    """ Updates the version files for a given component.  This includes
    Version.h, the PRF, and the VERSION file."""

    global comp_versions, opts, release_date

    vprint ("Updating version files for " + component)

    import re

    retval = list ()

    ## Update component/VERSION
    with open (component + "/VERSION", "r+") as version_file:
        new_version = re.sub (component + " version .*",
                              "%s version %s, released %s" % (component,
                                                              comp_versions[component + "_version"],
                                                              release_date),
                              version_file.read ())
        if opts.take_action:
            version_file.seek (0)
            version_file.truncate (0)
            version_file.write (new_version)
        else:
            print "New version file for " + component
            print new_version

        vprint ("Updating Version.h for " + component)

    retval += [component + "/VERSION"]

    ## Update component/component/Version.h
    version_header = """
// -*- C++ -*-
// $Id$
// This is file was automatically generated by \$ACE_ROOT/bin/make_release.

#define %s_MAJOR_VERSION %s
#define %s_MINOR_VERSION %s
#define %s_BETA_VERSION %s
#define %s_VERSION \"%s\"
""" % (component, comp_versions[component + "_major"],
       component, comp_versions[component + "_minor"],
       component, comp_versions[component + "_beta"],
       component, comp_versions[component + "_version"])

    if opts.take_action:
        with open (component + '/' + component.lower () + "/Version.h", 'r+') as version_h:
            version_h.write (version_header)
    else:
        print "New Version.h for " + component
        print version_header

    retval += [component + '/' + component.lower () + "/Version.h"]

    # Update component/PROBLEM-REPORT-FORM
    vprint ("Updating PRF for " + component)

    version_string = re.compile ("^\s*(\w+) +VERSION ?:")

    with open (component + "/PROBLEM-REPORT-FORM", 'r+') as prf:
        new_prf = ""
        for line in prf.readlines ():
            match = None
            match = version_string.search (line)
            if match is not None:
                vprint ("Found PRF Version for " + match.group (1))
                line = re.sub ("(\d\.)+\d?",
                               comp_versions[match.group(1) + "_version"],
                               line)

            new_prf += line

        if opts.take_action:
            prf.seek (0)
            prf.truncate (0)
            prf.writelines (new_prf)
        else:
            print "New PRF for " + component
            print "".join (new_prf)

    retval += [component + "/PROBLEM-REPORT-FORM"]
    return retval


def get_and_update_versions ():
    """ Gets current version information for each component,
    updates the version files, creates changelog entries,
    and commit the changes into the repository."""

    try:
        get_comp_versions ("ACE")
        get_comp_versions ("TAO")
        get_comp_versions ("CIAO")

        files = list ()
        files += update_version_files ("ACE")
        files += update_version_files ("TAO")
        files += update_version_files ("CIAO")
        files += create_changelog ("ACE")
        files += create_changelog ("TAO")
        files += create_changelog ("CIAO")

        commit (files)
    except:
        print "Fatal error in get_and_update_versions."
        raise

def create_changelog (component):
    """ Creates a changelog entry for the supplied component that includes
    the version number being released"""
    vprint ("Creating ChangeLog entry for " + component)

    global comp_versions, opts

    # generate our changelog entry
    changelog_entry = """%s  %s  <%s>

\t* %s version %s released.

""" % (release_date, signature, mailid,
       component,
       comp_versions[component + "_version"])

    vprint ("Changelog Entry for " + component + "\n" + changelog_entry)

    with open ("%s/ChangeLog" % (component), 'r+') as changelog:
        changelog_entry += changelog.read ()

        if opts.take_action:
            changelog.seek (0)
            changelog.truncate (0)
            changelog.write (changelog_entry)

    return ["%s/ChangeLog" % (component)]

def get_comp_versions (component):
    """ Extracts the current version number from the VERSION
    file and increments it appropriately for the release type
    requested."""
    vprint ("Detecting current version for" + component)

    import re

    global comp_versions, opts

    beta = re.compile ("version (\d+)\.(\d+)\.(\d+)")
    minor = re.compile ("version (\d+)\.(\d+)[^\.]")
    major = re.compile ("version (\d+)[^\.]")

    with open (component + "/VERSION") as version_file:
        for line in version_file:
            match = None

            match = beta.search (line)
            if match is not None:
                vprint ("Detected beta version %s.%s.%s" %
                           (match.group (1), match.group (2), match.group (3)))

                comp_versions[component + "_major"] = int (match.group (1))
                comp_versions[component + "_minor"] = int (match.group (2))
                comp_versions[component + "_beta"] = int (match.group (3))
                break

            match = minor.search (line)
            if match is not None:
                vprint ("Detected minor version %s.%s" %
                            (match.group (1), match.group (2)))

                comp_versions[component + "_major"] = int (match.group (1))
                comp_versions[component + "_minor"] = int (match.group (2))
                comp_versions[component + "_beta"] = 0
                break

            match = major.search (line)
            if match is not None:
                vprint ("Detected major version " + match.group (1) + ".0")

                comp_versions[component + "_major"] = int (match.group (1))
                comp_versions[component + "_minor"] = 0
                comp_versions[component + "_beta"] = 0
                break

            print "FATAL ERROR: Unable to locate current version for " + component
            raise Exception

    if opts.update:
        if opts.release_type == "major":
            comp_versions[component + "_major"] += 1
            comp_versions[component + "_minor"] = 0
            comp_versions[component + "_beta"] = 0
        elif opts.release_type == "minor":
            comp_versions[component + "_minor"] += 1
            comp_versions[component + "_beta"] = 0
        elif opts.release_type == "beta":
            comp_versions[component + "_beta"] += 1

    if opts.release_type == "beta":
        comp_versions [component + "_version"] = \
                      str (comp_versions[component + "_major"])  + '.' + \
                      str (comp_versions[component + "_minor"])  + '.' + \
                      str (comp_versions[component + "_beta"])
    else:
        comp_versions [component + "_version"] = \
                      str (comp_versions[component + "_major"])  + '.' + \
                      str (comp_versions[component + "_minor"])


def tag ():
    """ Tags the DOC and MPC repositories for the version """
    global comp_versions, opts

    branch = "ACE+TAO+CIAO-%d_%d_%d" % (comp_versions["ACE_major"],
                                        comp_versions["ACE_minor"],
                                        comp_versions["ACE_beta"])
    # Tag middleware
    svn_client.copy (opts.repo_root + "/Middleware/trunk",
                     opts.repo_root + "/Middleware/tags/" + branch)

    # Tag MPC
    svn_client.copy (opts.repo_root + "/MPC/trunk",
                     opts.repo_root + "/MPC/tags/" + branch)

##################################################
#### Packaging methods
##################################################
def export_wc (stage_dir):

    global doc_root

    # Export our working copy
    print ("Exporting ACE")
    svn_client.export (doc_root + "/ACE",
                       stage_dir + "/ACE_wrappers")

    print ("Exporting MPC")
    svn_client.export (doc_root + "/ACE/MPC",
                       stage_dir + "/ACE_wrappers/MPC")

    print ("Exporting TAO")
    svn_client.export (doc_root + "/TAO",
                       stage_dir + "/ACE_wrappers/TAO")

    print ("Exporting CIAO")
    svn_client.export (doc_root + "/CIAO",
                       stage_dir + "/ACE_wrappers/TAO/CIAO")


def update_packages (text_files, bin_files, stage_dir, package_dir):
    import os

    print "Updating packages...."
    os.chdir (stage_dir)

    # -g appends, -q for quiet operation
    zip_base_args = " -gqu "
    # -l causes line ending conversion for windows
    zip_text_args = " -l "
    zip_file = stage_dir + "/zip-archive.zip"

    # -r appends, -f specifies file.
    tar_args = "-uf "
    tar_file = stage_dir + "/tar-archive.tar"

    # Zip binary files
    print "\tAdding binary files to zip...."
    instream, outstream = os.popen2 ("xargs zip " + zip_base_args + zip_file)

    instream.write (bin_files)

    instream.close ()
    outstream.close ()

    # Need to wait for zip process spawned by popen2 to complete
    # before proceeding.
    os.wait ()

    print "\tAdding text files to zip....."
    instream, outstream = os.popen2 ("xargs zip " + zip_base_args + zip_text_args + zip_file)

    instream.write (text_files)

    instream.close ()
    outstream.close ()

    # Need to wait for zip process spawned by popen2 to complete
    # before proceeding.
    os.wait ()

    # Tar files
    print "\tAdding to tar file...."
    instream, outstream = os.popen2 ("xargs tar " + tar_args + tar_file)

    instream.write (' ' + bin_files + ' ' + text_files)

    instream.close ()
    outstream.close ()

    os.wait ()

def move_packages (name, stage_dir, package_dir):
    """ Copies the temporary files from the stage_dir to the package_dir.
        Renames them to name.tar and name.zip, respectively, and compresses
        the tarfile with gzip and bzip2. """
    import shutil, os
    from os.path import join

    print "Storing packages for ", name

    # Take care of the zip file
    print "\tZip file..."
    target_file = join (package_dir, name + ".zip")
    shutil.copy (join (stage_dir, "zip-archive.zip"),
                 target_file)
    ex ("md5sum " + target_file + " > " + target_file + ".md5")


    tar_file = join (stage_dir, "tar-archive.tar")
    target_file = join (package_dir, name + ".tar")

    # bzip
    print "\tBzip2 file....."
    shutil.copy (tar_file,
                 target_file)
    ex ("bzip2 " + target_file)
    ex ("md5sum " + target_file + ".bz2 > " + target_file + ".bz2.md5")

    print "\tgzip file....."
    shutil.copy (tar_file,
                 target_file)
    ex ("gzip " + target_file)
    ex ("md5sum " + target_file + ".gz > " + target_file + ".gz.md5")

def create_file_lists (base_dir, prefix, exclude):
    """ Creates two lists of files:  files that need CR->CRLF
    conversions (useful for zip files) and those that don't,
    excluding filies/directories found in exclude. """
    import os

    text_files = list ()
    bin_files = list ()

    for root, dirs, files in os.walk (base_dir, topdown=True):
#        print "root", root

        relroot = root.replace (base_dir, "")

#        print "relroot", relroot

        if len(relroot) and relroot[0] == '/':
            relroot = relroot [1:]

        excluded = False
        for item in exclude:
            dir_item = item + '/'
            if relroot.startswith (dir_item) or relroot.startswith (item):
#                print "excluding", relroot
                excluded = True
#            else:
#                print relroot, "does not start with", dir_item, "or", item

        if excluded:
            continue

        # Remove dirs from our exclude pattern
        for item in dirs:
#            print "item", item
            # Remove our excludes
            if (item) in exclude:
#                print "Removing " + item + " from consideration...."
                dirs.remove (item)

        for item in files:

            fullitem = os.path.join (relroot, item)
            if fullitem in exclude or item in exclude:
#                print "Removing " + fullitem + " from consideration...."
                files.remove (item)
                continue
            else:
                if bin_regex.search (fullitem) is not None:
                    bin_files.append (os.path.join (prefix, fullitem))
                else:
                    text_files.append (os.path.join (prefix, fullitem))

    return (text_files, bin_files)

def write_file_lists (comp, text, bin):
    outfile = open (comp + ".files", 'w')

    outfile.write ("\n".join (text))
    outfile.write (".............\nbin files\n.............\n")
    outfile.write ("\n".join (bin))

    outfile.close ()

def package (stage_dir, package_dir, decorator):
    """ Packages ACE, ACE+TAO, and ACE+TAO+CIAO releases of current
        staged tree, with decorator appended to the name of the archive. """
    from os.path import join
    from os import remove
    from os import chdir

    chdir (stage_dir)

    text_files = list ()
    bin_files = list ()

    # Erase our old temp files
    try:
#        print "removing files", join (stage_dir, "zip-archive.zip"), join (stage_dir, "tar-archive.tar")
        remove (join (stage_dir, "zip-archive.zip"))
        remove (join (stage_dir, "tar-archive.tar"))
    except:
        print "error removing files", join (stage_dir, "zip-archive.zip"), join (stage_dir, "tar-archive.tar")
        pass # swallow any errors

    text_files, bin_files = create_file_lists (join (stage_dir, "ACE_wrappers"),
                                               "ACE_wrappers", ["TAO", "autom4te.cache"])

#    write_file_lists ("fACE" + decorator, text_files, bin_files)
    update_packages ("\n".join (text_files),
                     "\n".join (bin_files),
                     stage_dir,
                     package_dir)


    move_packages ("ACE" + decorator, stage_dir, package_dir)

    text_files = list ()
    bin_files = list ()

    # for TAO:
    text_files, bin_files = create_file_lists (join (stage_dir, "ACE_wrappers/TAO"),
                                                     "ACE_wrappers/TAO", ["CIAO", "autom4te.cache"])

#    write_file_lists ("fTAO" + decorator, text_files, bin_files)
    update_packages ("\n".join (text_files),
                     "\n".join (bin_files),
                     stage_dir,
                     package_dir)

    move_packages ("ACE+TAO" + decorator, stage_dir, package_dir)

    text_files = list ()
    bin_files = list ()
    # for CIAO:
    text_files, bin_files = create_file_lists (join (stage_dir, "ACE_wrappers/TAO/CIAO"),
                                                     "ACE_wrappers/TAO/CIAO", "")

#    write_file_lists ("fCIAO" + decorator, text_files, bin_files)
    update_packages ("\n".join (text_files),
                     "\n".join (bin_files),
                     stage_dir,
                     package_dir)

    move_packages ("ACE+TAO+CIAO" + decorator, stage_dir, package_dir)

def generate_workspaces (stage_dir):
    """ Generates workspaces in the given stage_dir """
    print "Generating workspaces..."
    global opts
    import os

    # Make sure we are in the right directory...
    os.chdir (os.path.join (stage_dir, "ACE_wrappers"))

    # Set up our environment
    os.putenv ("ACE_ROOT", os.path.join (stage_dir, "ACE_wrappers"))
    os.putenv ("MPC_ROOT", os.path.join (stage_dir, "ACE_wrappers", "MPC"))
    os.putenv ("TAO_ROOT", os.path.join (stage_dir, "ACE_wrappers", "TAO"))
    os.putenv ("CIAO_ROOT", os.path.join (stage_dir, "ACE_wrappers", "TAO", "CIAO"))

    # Create option strings
    mpc_command = os.path.join (stage_dir, "ACE_wrappers", "bin", "mwc.pl")
    exclude_option = ' -exclude TAO/TAO_*.mwc,TAO/CIAO/CIAO_*.mwc '
    mpc_option = ' -recurse -hierarchy -relative ACE_ROOT=' + stage_dir + '/ACE_wrappers '
    mpc_option += ' -relative TAO_ROOT=' + stage_dir + '/ACE_wrappers/TAO '
    mpc_option += ' -relative CIAO_ROOT=' + stage_dir + '/ACE_wrappers/TAO/CIAO '

    static_vc71_option = ' -static -name_modifier *_vc71_Static -apply_project -exclude TAO/CIAO '
    static_vc71_option += mpc_option

    static_vc8_option = ' -static -name_modifier *_vc8_Static -apply_project -exclude TAO/CIAO '
    static_vc8_option += mpc_option

    static_vc9_option = ' -static -name_modifier *_vc9_Static -apply_project -exclude TAO/CIAO '
    static_vc9_option += mpc_option

    vc9_option = ' -name_modifier *_vc9 '
    vc8_option = ' -name_modifier *_vc8 '
    vc71_option = ' -name_modifier *_vc71 '

    # Build option string for VC8 platforms
    ce_option = ' -name_modifier *_vc8_WinCE -features "uses_wchar=1,wince=1" '
    ce_option += ' -value_template platforms=\'"Pocket PC 2003 (ARMV4)"\' '
    ce_option += ' -value_template platforms+=\'"Smartphone 2003 (ARMV4)"\' '
    ce_option += ' -value_template platforms+=\'"Windows Mobile 5.0 Pocket PC SDK (ARMV4I)"\' '
    ce_option += ' -value_template platforms+=\'"Windows Mobile 5.0 Smartphone SDK (ARMV4I)"\' '

    redirect_option = str ()
    if not opts.verbose:
        redirect_option = " >> ../mpc.log 2>&1"

    # Generate GNUmakefiles
    print "\tGenerating GNUmakefiles...."
    ex (mpc_command + " -type gnuace " + exclude_option + mpc_option + redirect_option)

    print "\tGenerating VC9 solutions..."
    ex (mpc_command + " -type vc9 " + mpc_option + vc9_option + redirect_option)

    print "\tGenerating VC8 solutions..."
    ex (mpc_command + " -type vc8 " + mpc_option + vc8_option + redirect_option)

    print "\tGenerating VC8 Windows CE solutions..."
    ex (mpc_command + " -type vc8 " + mpc_option + ce_option + redirect_option)

    print "\tGenerating VC71 solutions..."
    ex (mpc_command + " -type vc71 " + mpc_option + vc71_option + redirect_option)

    print "\tGenerating Borland makefiles"
    ex (mpc_command + " -type borland " + mpc_option + exclude_option + redirect_option)

    print "\tGenerating VC71 Static solutions"
    ex (mpc_command + " -type vc71 " + static_vc71_option + redirect_option)

    print "\tGenerating VC8 Static solutions"
    ex (mpc_command + " -type vc8 " + static_vc8_option + redirect_option)

    print "\tGenerating VC9 Static solutions"
    ex (mpc_command + " -type vc9 " + static_vc9_option + redirect_option)

    print "\tBootstrapping autotools support"
    ex ("bin/bootstrap " + redirect_option)

    print "\tCorrecting permissions for all generated files..."
    ex ("find ./ -name '*.vc[p,w]' -or -name '*.bor' -or -name '*.vcproj' -or -name '*.sln' -or -name 'GNUmake*' | xargs chmod 0644")

def create_kit ():
    """ Creates kits """
    import os
    from os.path import join
    # Get version numbers for this working copy, note this will
    # not update the numbers.
    print "Getting current version information...."

    get_comp_versions ("ACE")
    get_comp_versions ("TAO")
    get_comp_versions ("CIAO")

    print "Creating working directories...."
    stage_dir, package_dir = make_working_directories ()

    print "Exporting working copy..."
    export_wc (stage_dir)

    ### make source only packages
    package (stage_dir, package_dir, "-src")

    generate_workspaces (stage_dir)

    ### create standard packages.
    package (stage_dir, package_dir, "")

def make_working_directories ():
    """ Creates directories that we will be working in.
    In particular, we will have DOC_ROOT/stage-PID and
    DOC_ROOT/packages-PID """
    global doc_root
    import os.path, os

    stage_dir = os.path.join (doc_root, "stage-" + str (os.getpid ()))
    package_dir = os.path.join (doc_root, "package-" + str (os.getpid ()))

    os.mkdir (stage_dir)
    os.mkdir (package_dir)

    return (stage_dir, package_dir)

def main ():
    global opts

    if opts.action == "tag":
        print "Tagging a " + opts.release_type + " release."
        raw_input ("Press enter to continue")

        check_workspace ()
        get_and_update_versions ()
        tag ()

    else:
        print "Creating a kit."
        raw_input ("Press enter to continue")

        create_kit ()



if __name__ == "__main__":
    (opts, args) = parse_args ()

    if check_environment() is not True:
        exit (1)

    main ()
