#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# @file make_release.py
# @author William R. Otte <wotte@dre.vanderbilt.edu>
#
# Packaging script for ACE/TAO
#
# Requires Python 3.4+

from time import strftime
import re
import subprocess
import shlex
import multiprocessing
import sys
import enum
import os
from os.path import join
import shutil

##################################################
#### Global variables
##################################################
""" Options from the command line """
opts=None

""" Absolute path from the git workspace to be used for the
release"""
doc_root=None

""" A dict containing version information used for the release.
This dict contains entries of the form
COMPONENT_version
COMPONENT_micro
COMPONENT_minor
COMPONENT_major
COMPONENT_code """
comp_versions = {}
old_comp_versions = {}

release_date = strftime (# ie: Mon Jan 23 00:35:37 CST 2006
                              "%a %b %d %H:%M:%S %Z %Y")
cpu_count = multiprocessing.cpu_count()

# Packaging configuration

""" This is a regex that detects files that SHOULD NOT have line endings
converted to CRLF when being put into a ZIP file """
bin_regex = re.compile ("\.(mak|mdp|ide|exe|ico|gz|zip|xls|sxd|gif|vcp|vcproj|vcw|sln|dfm|jpg|png|vsd|bz2|pdf|ppt|graffle|pptx|odt|sh)$")
version_restr = r'(\d+)(?:\.(\d+)(?:\.(\d+))?)?'
version_re = re.compile(version_restr)

##################################################
#### Utility Methods
##################################################

class ReleaseType(enum.Enum):
    major = enum.auto()
    minor = enum.auto()
    micro = enum.auto()


def parse_args ():
    from argparse import ArgumentParser
    parser = ArgumentParser ()

    mutex_args = parser.add_mutually_exclusive_group(required=True)
    for rt in ReleaseType:
        mutex_args.add_argument ('--' + rt.name,
            dest="release_type", default=None, action="store_const", const=rt,
            help="Create a " + rt.name + " release.")
    mutex_args.add_argument ("--kit",
        dest="action", default=None, action="store_const", const="kit",
        help="Create kits.")

    parser.add_argument ("--tag", action="store_true",
        help="Update tags and branches of the repositories", default=False)
    parser.add_argument ("--update", action="store_true",
        help="Update the version numbers", default=False)
    parser.add_argument ("--push", action="store_true",
        help="Push all changes to remote", default=False)

    parser.add_argument ("--dest", dest="package_dir",
        help="Specify destination for the created packages.", default=None)

    parser.add_argument ("--root", dest="repo_root",
        help="Specify an alternate repository root",
        default="https://github.com/DOCGroup/ACE_TAO.git")
    parser.add_argument ("--ace-tao-branch",
        help="ACE/TAO branch to update", default="master")

    parser.add_argument ("--mpc_root",
        help="Specify an alternate MPC repository root",
        default="https://github.com/DOCGroup/MPC.git")
    parser.add_argument ("--mpc-branch",
        help="MPC branch to update", default="master")

    parser.add_argument ("-n", dest="take_action", action="store_false",
        help="Take no action", default=True)
    parser.add_argument ("--verbose", action="store_true",
        help="Print out actions as they are being performed",
        default=False)

    options = parser.parse_args ()

    if options.tag:
        if not options.update:
            print ("Warning: You are tagging a release, but not requesting a version increment")

        if not options.push:
            print ("Warning: You are tagging a release, but not requesting a push to remote")

    return options


def ex (command, allow_fail=False):
    if vprint ("Executing", command, take_action=True):
        return

    status = os.system(command)
    if status != 0:
        print (("ERROR" if allow_fail else "WARNING") +
          ": Nonzero return value from " + command, file=sys.stderr)
        if not allow_fail:
            raise Exception


def vprint (*args, take_action=False, **kwargs):
    """Prints the supplied message if verbose is enabled or this is a dry-run
    print statement. Return a bool of the latter case, so the caller can
    act differently.
    """

    take_action = take_action and not opts.take_action

    if opts.verbose or take_action:
        print (*args, **kwargs)

    return take_action


def get_tag (verdict, component):
    return "ACE+TAO-%d_%d_%d" % (
        verdict[component + '_major'], verdict[component + '_minor'], verdict[component + '_micro'])


def get_path (*args):
    if not args:
        args = ('',)
    return join (doc_root, 'ACE_TAO', *args)

##################################################
#### Tagging methods
##################################################
def commit (files):
    """ Commits the supplied list of files to the repository. """

    version = get_tag(comp_versions, 'ACE')
    root_path = get_path()
    files = [i[len(root_path):] if i.startswith(root_path) else i for i in files]

    print ("Committing the following files for " + version + ':', " ".join (files))

    if opts.take_action:
        for file in files:
            print ("Adding file " + file + " to commit")
            ex ("cd $DOC_ROOT/ACE_TAO && git add " + file)

        ex ("cd $DOC_ROOT/ACE_TAO && git commit -m\"" + version + "\"")

def check_workspace ():
    """ Checks that the DOC and MPC repositories are up to date.  """

    try:
        ex ("cd $DOC_ROOT/ACE_TAO && git pull -p")
        print ("Successfully updated ACE/TAO working copy")
    except:
        print ("Unable to update ACE/TAO workspace at " + doc_root)
        raise

    try:
        ex ("cd $DOC_ROOT/MPC && git pull -p")
        print ("Successfully updated MPC working copy to revision ")
    except:
        print ("Unable to update the MPC workspace at " + doc_root + "/ACE/MPC")
        raise

    vprint ("Repos root URL = " + opts.repo_root + "\n")
    vprint ("Repos MPC root URL = " + opts.mpc_root + "\n")

def update_version_files (component):
    """ Updates the version files for a given component.  This includes
    Version.h, the PRF, and the VERSION.txt file."""

    vprint ("Updating version files for " + component)

    retval = []

    ## Update component/VERSION.txt
    path = get_path(component, "VERSION.txt")
    with open (path, "r+") as version_file:
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
            print ("New version file for " + component)
            print (new_version)

        vprint ("Updating Version.h for " + component)

    retval.append(path)

    ## Update COMPONENT/component/Version.h
    comp_l = len(component + "_")
    parts = {k[comp_l:]:v for (k, v) in comp_versions.items() if k.startswith(component)}
    parts["comp"]  = component
    version_header = """
// -*- C++ -*-
// This is file was automatically generated by $ACE_ROOT/bin/make_release.py

#define {comp}_MAJOR_VERSION {major}
#define {comp}_MINOR_VERSION {minor}
#define {comp}_MICRO_VERSION {micro}
#define {comp}_VERSION \"{version}\"
#define {comp}_VERSION_CODE 0x{code:x}
#define {comp}_MAKE_VERSION_CODE(a,b,c) (((a) << 16) + ((b) << 8) + (c))
""".format(**parts)

    path = get_path(component, component.lower (), "Version.h")
    if opts.take_action:
        with open (path, 'w+') as version_h:
            version_h.write (version_header)
    else:
        print ("New Version.h for " + component)
        print (version_header)

    retval.append(path)

    # Update component/PROBLEM-REPORT-FORM
    vprint ("Updating PRF for " + component)

    version_line_re = re.compile (r"^\s*(\w+) +VERSION ?:")
    path = get_path(component, "PROBLEM-REPORT-FORM")

    with open (path, 'r+') as prf:
        new_prf = ""
        for line in prf.readlines ():
            match = version_line_re.search (line)
            if match is not None:
                vprint ("Found PRF Version for " + match.group (1))
                new_version = comp_versions[match.group(1) + "_version"]
                line = version_re.sub (new_version, line)

            new_prf += line

        if opts.take_action:
            prf.seek (0)
            prf.truncate (0)
            prf.writelines (new_prf)
        else:
            print ("New PRF for " + component)
            print ("".join (new_prf))

    retval.append(path)

    return retval


def update_spec_file ():
    path = get_path('ACE', "rpmbuild", "ace-tao.spec")
    with open (path, 'r+') as spec_file:
        new_spec = ""
        for line in spec_file.readlines ():
            if line.find ("define ACEVER ") != -1:
                line = "%define ACEVER  " + comp_versions["ACE_version"] + "\n"
            if line.find ("define TAOVER ") != -1:
                line = "%define TAOVER  " + comp_versions["TAO_version"] + "\n"
            if line.find ("define is_major_ver") != -1:
                line = "%define is_major_ver {}\n".format(
                    int(opts.release_type != ReleaseType.micro))

            new_spec += line

        if opts.take_action:
            spec_file.seek (0)
            spec_file.truncate (0)
            spec_file.writelines (new_spec)
        else:
            print ("New spec file:")
            print ("".join (new_spec))

    return [path]

def update_debianbuild ():
    """ Updates ACE_ROOT/debian directory.
    - renames all files with version numbers in name; if file contains
      lintian overrides, update version numbers inside file
    - updates version numbers inside file debian/control
    Currently ONLY ACE is handled here """

    prev_ace_ver = None

    path = get_path('ACE', 'debian', 'control')

    mask = re.compile ("(libace|libACE|libkokyu|libKokyu|libnetsvcs)([^\s,:]*-)(\d+\.\d+\.\d+)([^\s,:]*)")

    def update_ver (match):
        return match.group (1) + match.group (2) + comp_versions["ACE_version"] + match.group (4)

    # update debian/control
    with open (path, 'r+') as control_file:
        new_ctrl = ""
        for line in control_file.readlines ():
            if re.search ("^(Package|Depends|Suggests):", line) is not None:
                line = mask.sub (update_ver, line)
            elif re.search ('^Replaces:', line) is not None:
                line = line.replace (old_comp_versions["ACE_version"], comp_versions["ACE_version"])

            new_ctrl += line

        if opts.take_action:
            control_file.seek (0)
            control_file.truncate (0)
            control_file.writelines (new_ctrl)
        else:
            print ("New control file:")
            print ("".join (new_ctrl))

    return [path]

def get_and_update_versions ():
    """ Gets current version information for each component,
    updates the version files, creates changelog entries,
    and commit the changes into the repository."""

    try:
        get_comp_versions ("ACE")
        get_comp_versions ("TAO")

        if opts.update:
            files = []
            files += update_version_files ("ACE")
            files += update_version_files ("TAO")
            files += create_changelog ("ACE")
            files += create_changelog ("TAO")
            files += update_spec_file ()
            files += update_debianbuild ()

            commit (files)

    except:
        print ("Fatal error in get_and_update_versions.")
        raise

def create_changelog (component):
    """ Creates a changelog entry for the supplied component that includes
    the version number being released"""
    vprint ("Creating ChangeLog entry for " + component)

    old_tag = get_tag (old_comp_versions, 'ACE')

    # Generate changelogs per component
    path = get_path(component, "ChangeLogs", component + "-" + comp_versions[component + "_version_"])
    ex ("cd $DOC_ROOT/ACE_TAO && git log " + old_tag + "..HEAD " + component + " > " + path)

    return [path]

def get_comp_versions (component):
    """ Extracts the current version number from the VERSION.txt
    file and increments it appropriately for the release type
    requested."""
    vprint ("Detecting current version for " + component)

    regex = re.compile (r"version " + version_restr)
    major = component + "_major"
    minor = component + "_minor"
    micro = component + "_micro"


    version = (None, None, None)
    with open (doc_root + "/ACE_TAO/" + component + "/VERSION.txt") as version_file:
        for line in version_file:
            match = regex.search (line)
            if match is not None:
                version = match.groups(default=0)

                vprint ("Detected version %s.%s.%s" % version)

                comp_versions[major] = int (version[0])
                comp_versions[minor] = int (version[1])
                comp_versions[micro] = int (version[2])

                break

            print ("FATAL ERROR: Unable to locate current version for " + component)
            raise Exception

    # Also store the current release (old from now)
    old_comp_versions[major] = comp_versions[major]
    old_comp_versions[minor] = comp_versions[minor]
    old_comp_versions[micro] = comp_versions[micro]

    if opts.update:
        if opts.release_type == ReleaseType.major:
            comp_versions[major] += 1
            comp_versions[minor] = 0
            comp_versions[micro] = 0
        elif opts.release_type == ReleaseType.minor:
            comp_versions[minor] += 1
            comp_versions[micro] = 0
        elif opts.release_type == ReleaseType.micro:
            comp_versions[micro] += 1

    def make_version (versions, joiner):
        return joiner.join ([
          str (versions[component + '_' + x]) for x in ReleaseType.__members__.keys ()
        ])

    comp_versions [component + "_version"] = make_version (comp_versions, '.')
    comp_versions [component + "_version_"] = make_version (comp_versions, '_')

    comp_versions [component + "_code"] = \
        (comp_versions[major] << 16) + \
        (comp_versions[minor] << 8) + \
        comp_versions[micro]

    old_comp_versions [component + "_version"] = make_version (old_comp_versions, '.')
    old_comp_versions [component + "_version_"] = make_version (old_comp_versions, '_')

    if opts.update:
      vprint ("Updating from version %s to version %s" %
                  (old_comp_versions [component + "_version"], comp_versions [component + "_version"]))
    else:
      vprint ("Found version %s" %
                  (comp_versions [component + "_version"]))

    # else:
    #     comp_versions [component + "_version"] = \
    #                   str (comp_versions[major])  + '.' + \
    #                   str (comp_versions[minor])


def update_latest_branch (product, which, main_branch):
    """Update one of the Latest_ACE7TAO3_* branches to point to the new release.
    """

    name = "Latest_ACE7TAO3_" + which

    vprint ('Fast-forwarding', name, 'to', main_branch)
    ex ("cd $DOC_ROOT/" + product + " && git fetch . " + main_branch +  ":" + name)


def push_latest_branch (product, which, main_branch):
    """Update one of the remote Latest_ACE7TAO3_* branches to point to the new release.
    """

    name = "Latest_ACE7TAO3_" + which

    if opts.push:
        vprint ("Pushing branch", name)
        ex ("cd $DOC_ROOT/" + product + " && git push origin refs/heads/" + name,
            allow_fail=True)


def latest_branch_helper (fn, release_type):
    release_types = tuple(ReleaseType.__members__.values())
    do = release_types[release_types.index(release_type):]
    if ReleaseType.micro in do:
        fn ("ACE_TAO", "Micro", opts.ace_tao_branch)
        fn ("MPC", "Micro", opts.mpc_branch)
    if ReleaseType.minor in do:
        fn ("ACE_TAO", "Minor", opts.ace_tao_branch)
        fn ("MPC", "Minor", opts.mpc_branch)
    if ReleaseType.major in do:
        fn ("ACE_TAO", "Major", opts.ace_tao_branch)
        fn ("MPC", "Major", opts.mpc_branch)


def tag ():
    """Add the release tag and fast-forward the release branches on DOC and MPC
    repositories.
    """

    tagname = get_tag(comp_versions, 'ACE')

    if opts.tag:
        if opts.take_action:
            vprint ("Placing tag %s on ACE_TAO" % (tagname))
            ex ("cd $DOC_ROOT/ACE_TAO && git tag -a " + tagname + " -m\"" + tagname + "\"")

            vprint ("Placing tag %s on MPC" % (tagname))
            ex ("cd $DOC_ROOT/MPC && git tag -a " + tagname + " -m\"" + tagname + "\"")

            # Update release branches
            latest_branch_helper (update_latest_branch, opts.release_type)
        else:
            vprint ("Placing tag %s on ACE_TAO" % (tagname))
            vprint ("Placing tag %s on MPC" % (tagname))
            print ("Creating tags:\n")
            print ("Placing tag " + tagname + "\n")


def push ():
    """Push the release tag and the fast-forwarded release branches on DOC and
    MPC repositories.
    """

    tagname = get_tag (comp_versions, 'ACE')

    if opts.push:
        if opts.take_action:
            vprint ("Pushing ACE_TAO", opts.ace_tao_branch, "to origin")
            ex ("cd $DOC_ROOT/ACE_TAO && git push origin " + opts.ace_tao_branch)

            vprint ("Pushing tag %s on ACE_TAO" % (tagname))
            ex ("cd $DOC_ROOT/ACE_TAO && git push origin tag " + tagname)

            vprint ("Pushing tag %s on MPC" % (tagname))
            ex ("cd $DOC_ROOT/MPC && git push origin tag " + tagname)

            # Push release branches
            latest_branch_helper (push_latest_branch, opts.release_type)
        else:
            vprint ("Pushing tag %s on ACE_TAO" % (tagname))
            vprint ("Pushing tag %s on MPC" % (tagname))
            print ("Pushing tags:\n")
            print ("Pushing tag " + tagname + "\n")


##################################################
#### Packaging methods
##################################################
def export_wc (stage_dir):

    tag = get_tag (comp_versions, 'ACE')

    # Clone the ACE repository with the needed tag
    print ("Retrieving ACE with tag " + tag)
    ex ("git clone --depth 1 --branch " + tag + " " + opts.repo_root + " " + stage_dir + "/ACE_TAO")

    # Clone the MPC repository with the needed tag
    print ("Retrieving MPC with tag " + tag)
    ex ("git clone --depth 1 --branch " + tag + " " + opts.mpc_root + " " + stage_dir + "/MPC")

    # Setting up stage_dir
    print ("Moving ACE")
    ex ("mv " + stage_dir + "/ACE_TAO/ACE " + stage_dir + "/ACE_wrappers")
    print ("Moving TAO")
    ex ("mv " + stage_dir + "/ACE_TAO/TAO " + stage_dir + "/ACE_wrappers/TAO")
    print ("Moving MPC")
    ex ("mv " + stage_dir + "/MPC " + stage_dir + "/ACE_wrappers/MPC")

def update_packages (text_files_list, bin_files_list, stage_dir, package_dir):
    stream_encoding = 'utf-8'
    list_to_bytes = lambda l: ('\n'.join (l)).encode (stream_encoding)
    text_files = list_to_bytes (text_files_list)
    bin_files = list_to_bytes (bin_files_list)

    print ("Updating packages....")
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
    print ("\tAdding binary files to zip....")
    p = subprocess.Popen (
        shlex.split ("xargs zip " + zip_base_args + zip_file),
        stdin=subprocess.PIPE, stdout=subprocess.PIPE, close_fds=True)
    instream, outstream = (p.stdin, p.stdout)

    instream.write (bin_files)

    instream.close ()
    outstream.close ()

    # Need to wait for zip process spawned by popen2 to complete
    # before proceeding.
    os.wait ()

    print ("\tAdding text files to zip.....")
    p = subprocess.Popen (
        shlex.split ("xargs zip " + zip_base_args + zip_text_args + zip_file),
        stdin=subprocess.PIPE, stdout=subprocess.PIPE, close_fds=True)
    instream, outstream = (p.stdin, p.stdout)

    instream.write (text_files)

    instream.close ()
    outstream.close ()

    # Need to wait for zip process spawned by popen2 to complete
    # before proceeding.
    os.wait ()

    # Tar files
    print ("\tAdding to tar file....")
    if not os.path.exists (tar_file):
        open(tar_file, 'w').close ()

    p = subprocess.Popen (
        shlex.split ("xargs tar " + tar_args + tar_file),
        stdin=subprocess.PIPE, stdout=subprocess.PIPE, close_fds=True)
    instream, outstream = (p.stdin, p.stdout)

    instream.write (b' ' + bin_files + b' ' + text_files)

    instream.close ()

    print (outstream.read ().decode(stream_encoding))
    outstream.close ()

    os.wait ()

def move_packages (name, stage_dir, package_dir):
    """ Copies the temporary files from the stage_dir to the package_dir.
        Renames them to name.tar and name.zip, respectively, and compresses
        the tarfile with gzip and bzip2. """

    print ("Storing packages for ", name)

    # Take care of the zip file
    print ("\tZip file...")
    target_file = join (package_dir, name + ".zip")
    shutil.copy (join (stage_dir, "zip-archive.zip"), target_file)
    ex ("md5sum " + target_file + " > " + target_file + ".md5")


    tar_file = join (stage_dir, "tar-archive.tar")
    target_file = join (package_dir, name + ".tar")

    # bzip
    print ("\tBzip2 file.....")
    shutil.copy (tar_file, target_file)
    ex ("bzip2 " + target_file)
    ex ("md5sum " + target_file + ".bz2 > " + target_file + ".bz2.md5")

    print ("\tgzip file.....")
    shutil.copy (tar_file, target_file)
    ex ("gzip " + target_file)
    ex ("md5sum " + target_file + ".gz > " + target_file + ".gz.md5")

def create_file_lists (base_dir, prefix, exclude):
    """ Creates two lists of files:  files that need CR->CRLF
    conversions (useful for zip files) and those that don't,
    excluding files/directories found in exclude. """

    text_files = list ()
    bin_files = list ()

    for root, dirs, files in os.walk (base_dir, topdown=True):
#        print "root", root

        relroot = root.replace (base_dir, "")

#        print "relroot", relroot

        if relroot and relroot[0] == '/':
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

        # Remove dirs that are listed in our exclude pattern
        for item in dirs:
#            print "item", item
            # Remove our excludes
            if (item) in exclude:
#                print "Removing " + item + " from consideration...."
                dirs.remove (item)

        # Remove files that are listed in our exclude pattern
        for item in files:
            fullitem = os.path.join (relroot, item)
            if fullitem in exclude or item in exclude:
#                print "Removing " + fullitem + " from consideration...."
                continue
            else:
                if bin_regex.search (fullitem) is not None:
                    bin_files.append ('"' + os.path.join (prefix, fullitem) + '"')
                else:
                    text_files.append ('"' + os.path.join (prefix, fullitem) + '"')

    return (text_files, bin_files)

def write_file_lists (comp, text, bin):
    with open (comp + ".files", 'w') as outfile:
        outfile.write ("\n".join (text))
        outfile.write (".............\nbin files\n.............\n")
        outfile.write ("\n".join (bin))

def package (stage_dir, package_dir, decorator):
    """ Packages ACE, ACE+TAO releases of current
        staged tree, with decorator appended to the name of the archive. """

    os.chdir (stage_dir)

    # Erase our old temp files
    try:
#        print "removing files", join (stage_dir, "zip-archive.zip"), join (stage_dir, "tar-archive.tar")
        os.remove (join (stage_dir, "zip-archive.zip"))
        os.remove (join (stage_dir, "tar-archive.tar"))
    except:
        print ("error removing files", join (stage_dir, "zip-archive.zip"), join (stage_dir, "tar-archive.tar"))
        pass # swallow any errors

    text_files, bin_files = create_file_lists (join (stage_dir, "ACE_wrappers"),
                                               "ACE_wrappers", ["TAO", ".gitignore", ".git"])

#    write_file_lists ("fACE" + decorator, text_files, bin_files)
    update_packages (text_files, bin_files, stage_dir, package_dir)

    move_packages ("ACE" + decorator, stage_dir, package_dir)

    # for TAO:
    text_files, bin_files = create_file_lists (join (stage_dir, "ACE_wrappers/TAO"),
                                                     "ACE_wrappers/TAO", [".gitignore", ".git"])

#    write_file_lists ("fTAO" + decorator, text_files, bin_files)
    update_packages (text_files, bin_files, stage_dir, package_dir)

    move_packages ("ACE+TAO" + decorator, stage_dir, package_dir)

def generate_workspaces (stage_dir):
    """ Generates workspaces in the given stage_dir """
    print ("Generating workspaces...")

    # Make sure we are in the right directory...
    os.chdir (os.path.join (stage_dir, "ACE_wrappers"))

    # Set up our environment
    os.putenv ("ACE_ROOT", os.path.join (stage_dir, "ACE_wrappers"))
    os.putenv ("MPC_ROOT", os.path.join (stage_dir, "ACE_wrappers", "MPC"))
    os.putenv ("TAO_ROOT", os.path.join (stage_dir, "ACE_wrappers", "TAO"))
    os.putenv ("DDS_ROOT", "")

    # Create option strings
    mpc_command = os.path.join (stage_dir, "ACE_wrappers", "bin", "mwc.pl")
    exclude_option = ' -exclude TAO/TAO_*.mwc '
    workers_option = ' -workers ' + str(cpu_count)
    mpc_option = ' -recurse -hierarchy -relative ACE_ROOT=' + stage_dir + '/ACE_wrappers '
    mpc_option += ' -relative TAO_ROOT=' + stage_dir + '/ACE_wrappers/TAO '
    msvc_exclude_option = ' '
    vs2017_option = ' -name_modifier *_vs2017 '
    vs2019_option = ' -name_modifier *_vs2019 '

    redirect_option = str ()
    if not opts.verbose:
        redirect_option = " >> ../mpc.log 2>&1"

    print ("\tGenerating GNUmakefiles....")
    ex (mpc_command + " -type gnuace " + \
        exclude_option + workers_option + mpc_option + redirect_option)

    print ("\tGenerating VS2017 solutions...")
    ex (mpc_command + " -type vs2017 "  + \
        msvc_exclude_option + mpc_option + workers_option + vs2017_option + redirect_option)

    print ("\tGenerating VS2019 solutions...")
    ex (mpc_command + " -type vs2019 " + \
        msvc_exclude_option + mpc_option + workers_option + vs2019_option + redirect_option)

    print ("\tCorrecting permissions for all generated files...")
    regex = [
        '*.vc[p,w]',
        '*.bmak',
        '*.vcproj',
        '*.sln',
        '*.vcxproj',
        '*.filters',
        'GNUmake*',
    ]
    ex ("find ./ " + ' -or '.join(["-name '%s'" % (i,) for i in regex]) + " | xargs chmod 0644")

def create_kit ():
    """ Creates kits """

    # Get version numbers for this working copy, note this will
    # not update the numbers.
    print ("Getting current version information....")

    get_comp_versions ("ACE")
    get_comp_versions ("TAO")

    print ("Creating working directories....")
    stage_dir, package_dir = make_working_directories ()

    print ("Exporting working copy...")
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

    stage_dir = join (doc_root, "stage-" + str (os.getpid ()))
    package_dir = join (doc_root, "package-" + str (os.getpid ()))

    os.mkdir (stage_dir)
    os.mkdir (package_dir)

    return (stage_dir, package_dir)

def main ():

    if opts.action == "kit":
        print ("Creating a kit.")
        input ("Press enter to continue")

        create_kit ()

    else:
        print ("Making a " + opts.release_type.name + " release.")
        input ("Press enter to continue")

        get_and_update_versions ()
        tag ()
        push ()

if __name__ == "__main__":
    opts = parse_args ()

    doc_root = os.getenv ("DOC_ROOT")
    if doc_root is None:
        sys.exit ("ERROR: Environment DOC_ROOT must be defined.")

    main ()
