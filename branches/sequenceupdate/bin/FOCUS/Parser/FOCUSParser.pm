#########################################################################
# A Simple Parser for automating the specializations crated in FOCUS.
#
# @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
#
# $Id$
#
# This parser, parses the specialization file given as an input argument
# and *individually* visits the tags in a pre-determined order to weave
# in the specializations.
# NOTE: This parser will make N passes over the file, where N equals
# to the number of tags defined in the specialization file. This
# approach is intentional as it servers current needs. Future versions
# may enhance this parser and Visit methods to be more intelligent.
###########################################################################
package FOCUSParser;

# for MY own preferences!
use strict;

# XML related operations
use XML::DOM;

# Generic file operations
use FileHandle;

# Creating files and renaming them
use File::Copy;

# Creating directories
use File::Path;

############################################
# GLOBAL CONSTANTS
###########################################
my $FOCUS_PREPEND_TAG = "\/\/@@ ";

####################################################################
# banner: A function that returns the FOCUS banner transformation
# for just clarity purpose only.
###################################################################
sub FOCUS_banner_start
{
  my $banner_str = "// Code woven by FOCUS:\n";
  return $banner_str;
}

sub FOCUS_banner_end
{
  my $banner_str = "// END Code woven by FOCUS\n";
  return $banner_str;
}

#########################################################################
# Visit_ADD: Visit a add element defined in the transform.
# In particular look for the hook defined: search it in the source file
# and add the data in the <data> tags into the file starting from the
# hook, but not including the hook.
##########################################################################
sub Visit_Add
{
  my ($add, $copy_file_name) = @_;

  # Open the copy and transform it
  open (IN, "+<". $copy_file_name) ||
    die "cannot open file: " . $copy_file_name;

  # To update a file in place, we use the temporary
  # file idiom. Perl says this is the best way to
  # do this!
  my $copy_file_tmp = $copy_file_name . "tmp";
  open (OUT, ">". $copy_file_tmp) ||
    die "cannot open temporary file for modying file:" . $copy_file_name;

  # get the hook element defined in the add element
  my $hook = $add->getElementsByTagName ('hook');

  # ensure length of hook == 1;
  if ($hook->getLength != 1)
  {
    print "Assertion Error: An <add> element can have only \
           one <hook> definition";

    # clean up
    close (IN);
    close (OUT);

    # Diagnostic comment
    print " [failure]... Reverting changes \n";

    unlink ($copy_file_name);
    unlink ($copy_file_name . "tmp");
    exit (1);
  }

  # Check if the hook is present in the file at all
  my $hook_str = $hook->item(0)->getFirstChild->getNodeValue;
  chomp ($hook_str);

  #//@@ For now, due to problem with the hook string
  my $search_str = $hook_str;

  while (<IN>)
  {
    if (/$search_str/)
    {
      # Do not remove the hook! It needs to be present
      print OUT $_;

      # FOCUS banner start
      print OUT FOCUS_banner_start;

      # parse <data> ... </data> elements for this add tag
      my @data_list = $add->getElementsByTagName ('data');
      foreach my $data (@data_list)
      {
	my $data_item = $data->getFirstChild->getNodeValue;
	chomp ($data_item);

	# Insert the item
	print OUT "$data_item \n";
      }

      # FOCUS banner end
      print OUT FOCUS_banner_end;
    }
    else
    {  print OUT $_; }
  }

  # Everything went well!
  close (IN);
  close (OUT);

  # replace in place the old file with the new one
  rename ($copy_file_tmp, $copy_file_name);
}

###########################################################################
# Visit_Remove: Visit a <remove> element defined in the transform.
# In particular look for the hook defined: search it in the source file
# and remove the element's value from the source file being searched.
############################################################################
sub Visit_Remove
{
  my ($remove, $copy_file_name) = @_;

  # obtain the data to be removed
  my $search = $remove->getFirstChild->getNodeValue;
  chomp ($search);

  # Open the copy and transform it
  open (IN, "+<" . $copy_file_name) ||
    die "cannot open file: " . $copy_file_name;

  # Update the file in place
  my $copy_file_name_tmp = $copy_file_name . "tmp";
  open (OUT, ">". $copy_file_name_tmp) ||
    die "cannot open temporary file for modying file:" . $copy_file_name;;

  # Removing something is same as search and replace. Replace with ""
  my $replace = "";

  foreach my $line (<IN>)
  {
    if ($line =~/$search/)
    {
      # We do not print the banner information
      # as we have removed something and
      # print the banner will be redundant!

      # replace <search> with <replace>
      $line =~ s/$search/$replace/;

      print OUT $line;
    }
    else { print OUT $line; }
  }

  # Everything went well!
  close (IN);
  close (OUT);

  # replace in place the old file with the new one
  rename ($copy_file_name_tmp, $copy_file_name);
}

#########################################################################
# Visit_Substitute: Visit a <substitute> element defined in the transform.
# In particular look for the <search> element and replace it with the
# <replace> element.
#########################################################################
sub Visit_Substitute
{
  my ($substitute, $copy_file_name) = @_;

  # Open the copy and transform it
  open (IN, "+<". $copy_file_name) ||
    die "cannot open file: " . $copy_file_name;

  # To update a file in place, we use the temporary
  # file idiom. Perl says this is the best way to
  # do this!
  my $copy_file_name_tmp = $copy_file_name . "tmp";
  open (OUT, ">". $copy_file_name . "tmp") ||
    die "cannot open temporary file for modying file:" . $copy_file_name;;

  # check if the match-line keyword is set or not
  my $match_line = $substitute->getAttribute('match-line');

  # <search> .... </search>
  my $search_list = $substitute->getElementsByTagName ('search');

  # ensure length of search == 1;
  if ($search_list->getLength != 1 ||
      $search_list->getLength == 0)
  {
    print "Assertion Error: A <substitute> element can have only \
          one <search> element";
    close (IN);
    close (OUT);

    # Dianostic comment
    print " [failure] reverting changes \n";

    unlink ($copy_file_name);
    unlink ($copy_file_name_tmp);
    exit (1);
  }

  # <replace> .... </replace>
  my $replace_list = $substitute->getElementsByTagName ('replace');
  if ($replace_list->getLength != 1 ||
      $replace_list->getLength == 0)
  {
    print "Assertion Error: A <substitute> element can have only \
           one <replace> element";
    close (IN);
    close (OUT);
    unlink ($copy_file_name);
    unlink ($copy_file_name_tmp);
    exit (1);
  }

  # <search> and <replace> element values
  my $search = $search_list->item(0)->getFirstChild->getNodeValue;
  my $replace = $replace_list->item(0)->getFirstChild->getNodeValue;

  # remove spaces
  chomp ($search);
  chomp ($replace);

  # Search and replace string in the file
  foreach my $line (<IN>)
  {
    # Check if the match line attribute is set. If so then
    # ignore word boundaries. If not, honor word boundaries.
    my $line_matched = 0;
    if (! $match_line)
    {
      if ($line =~/\b$search\b/)
      {
        $line_matched = 1;
      }
    }
    else
    {
      if ($line =~ /$search/)
      {
        $line_matched = 1;
      }
    }

    # Check if the line matched
    if ($line_matched)
    {
      # FOCUS banner start
      print OUT FOCUS_banner_start;

      # replace <search> with <replace>
      # Caveat: What if <search> occures multiple
      # times in the line? Here is how we handle
      # it
      $line =~ s/$search/$replace/g;

      print OUT $line;

      # FOCUS banner end
      print OUT FOCUS_banner_end;
    }
    else { print OUT $line; }
  }

  # everything went well!
  close (IN);
  close (OUT);

  # replace in place the old file with the new one
  rename ($copy_file_name_tmp, $copy_file_name);
}

#########################################################################
# Visit_Comment: Visit the comment-region hooks defined in the
# source code and comment out all code between start and finish of that
# region
#########################################################################
sub Visit_Comment
{
  my ($comment, $copy_file_name) = @_;

  # check for the comment region tags and
  # comment out the region
  my $start_hook_tag = $comment->getElementsByTagName ('start-hook');
  my $end_hook_tag   = $comment->getElementsByTagName ('end-hook');

  if ($start_hook_tag->getLength != 1 ||
      $end_hook_tag->getLength != 1)
  {
    print "Assertion Error: A <comment> element can have only \
           one pair of <start-hook> and <end-hook> tags";
    unlink ($copy_file_name);
    exit (1);
  }

  my $start = $start_hook_tag->item(0)->getFirstChild->getNodeValue;
  my $end =   $end_hook_tag->item(0)->getFirstChild->getNodeValue;

  # What are we looking for:
  # We need to start from "//" . FOCUS_PREPEND_TAG . $hook
  # i.e. //[[@ <blah blah>
  # This will be the format for both start and end
  # //@@ Problems with the hook string
  my $start_hook = $FOCUS_PREPEND_TAG . $start;
  my $end_hook   = $FOCUS_PREPEND_TAG . $end;

  # Open the copy and transform it
  open (IN, "+<". $copy_file_name) ||
    die "cannot open file: " . $copy_file_name;

  my $copy_file_name_tmp = $copy_file_name . "tmp";
  open (OUT, ">". $copy_file_name_tmp) ||
    die "cannot open temporary file for modying file:" . $copy_file_name;

  my $start_commenting = 0;
  while (<IN>)
  {
    if (! /$start_hook/ &&
        ! /$end_hook/)
    {
      if ($start_commenting)
      { print OUT "// " . $_; }
      else
      { print OUT $_; }
    }
    else
    {
      if (/$start_hook/)
      {
        $start_commenting = 1;
        print OUT $_; # print start hook!
      }
      else
      {
        $start_commenting = 0;
        print OUT $_; # print end hook!
      }
    }
  }

  # everything went well!
  close (IN);
  close (OUT);

  rename ($copy_file_name_tmp, $copy_file_name);
}

###############################################################
# Visit_Copy: visit the <copy> tags and weave the code into the
# source file. In particular, open the source file specified
# in the file-source tag. Search for the start hook and
# copy until the end hook is reached.
###############################################################
sub Visit_Copy
{
  my ($copy_tag, $copy_file_name, $default_module_name, $prefix_path) = @_;

  # Check if a file name has been specified
  my $dest_file_tag = $copy_tag->getElementsByTagName ('source');

  if (! $dest_file_tag)
  {
    print "Error: <copy-from-source> does not have the <file> tag..";
    print "aborting \n";
    exit 1;
  }

  if ($dest_file_tag->getLength != 1)
  {
    print "Assertion Error: A <copy-from-source> element can have only \
           one <source> tag from which to copy elements";
    exit (1);
  }

  my $dest_file_name = $dest_file_tag->item(0)->getFirstChild->getNodeValue;

  #Check if the file exists and one is able to access it
  $dest_file_name = $prefix_path . "/" . $default_module_name . "/" . $dest_file_name;

  open (DEST, "<". $dest_file_name) ||
   die "cannot open $dest_file_name \n Wrong <file> tag within <copy-from-source> exiting" ;

  # check for the start and end tags within the target file where
  # one needs to start copying from
  my $start_tag = $copy_tag->getElementsByTagName ('copy-hook-start');
  my $end_tag   = $copy_tag->getElementsByTagName ('copy-hook-end');

  if (! $start_tag || ! $end_tag)
  {
    print "Assertion Error: A <copy> element should have a \
           <copy-hook-start> tag and <copy-hook-end> tag \n";
    exit (1);
  }

  # Get the <dest-hook> tag that indicates the destination where the
  # code between the start and end tags will be placed.
  my $dest_hook_tag   = $copy_tag->getElementsByTagName ('dest-hook');
  if (! $dest_hook_tag)
  {
    print "Assertion Error: <copy-from-source> should have a <dest-hook> \
           tag that dictates where in the source file the code should be \
           placed. \n";
    exit (1);
  }

  # Remove any starting and trailing white spaces
  chomp ($dest_hook_tag);

  # We have everything we need! Do the copy
  my $start_tag_name = $start_tag->item(0)->getFirstChild->getNodeValue;
  my $end_tag_name   = $end_tag->item(0)->getFirstChild->getNodeValue;
  my $dest_tag_name  = $dest_hook_tag->item(0)->getFirstChild->getNodeValue;

  # First we add the FOCUS prepend tags
  $start_tag_name = $FOCUS_PREPEND_TAG . $start_tag_name;
  $end_tag_name   = $FOCUS_PREPEND_TAG . $end_tag_name;
  $dest_tag_name  = $FOCUS_PREPEND_TAG . $dest_tag_name;

  # Step 1: Iterate over the target file till the
  # dest-hook is found in that file
  my $copy_file_name_tmp = $copy_file_name . "tmp";
  open (OUT, ">". $copy_file_name_tmp) ||
    die "cannot open temporary file for modying file:" . $copy_file_name;
  open (IN, "<" . $copy_file_name) ||
    die "cannot open file $copy_file_name specified in the <file> tag \n";

  my $dest_tag_found = 0; #check if tag matched
  foreach my $line (<IN>)
  {
    if ($line =~ /$dest_tag_name/)
    { $dest_tag_found = 1; print OUT $line; last; }

    print OUT $line;
  }
  close (IN);

  # If we reached the end of file before finding the tag!
  if (! $dest_tag_found)
  {
    print "\n Error: <dest-hook> tag missing in file .. aborting \n";
    close (DEST);
    close (IN);
    close (OUT);
    unlink ($copy_file_name_tmp);
    exit (1);
  }

  # Step 2: Now look in the destination file and look for the hooks
  # where one needs to copy. There could be multiple places where the
  # hook can be present. E.g.
  # .......
  # //@@ COPY_START_HOOK
  # ....
  # ....
  # //@@ COPY_END_HOOK
  # ....
  # ....
  # //@@ COPY_START_HOOK
  # ....
  # ....
  # //@@ COPY_END_HOOK
  # Handle this case

  my $line_matched = 0;
  my $start_copying = 0; # initially do not copy
  foreach my $line (<DEST>)
  {
    # Check if the line matches the start tag
    if ($line =~/$start_tag_name/)
    {
      $line_matched += 1;
      $start_copying = 1;
    }
    else
    {
      # Check if the line matches the end tag
      if ($line =~/$end_tag_name/)
      {
        # check if the start tag matched!
        if (! $line_matched)
        {
          print "Assertion error: <copy-hook-end> tag misplaced with \
                 the <copy-hoook-source> \n";
          close (DEST);
          close (IN);
          close (OUT);
          unlink ($copy_file_name_tmp);
          exit (1);
        }

        # decrement the count for nested tags
        $line_matched -= 1;
        if (! $line_matched )
          { $start_copying = 0; }
      }
      else
      {
        # Print out the line
        if ($start_copying)
          { print OUT $line; }
      }
    }
  }

  # At the end of this loop line_matched should be 0
  if ($line_matched)
  {
    print "Error: in $dest_file_name, number of <copy-hook-source> tags \
           did not match the number of <copy-hook-end> tags. Reverting \
           changes. \n";
    close (DEST);
    close (IN);
    close (OUT);
    unlink ($copy_file_name_tmp);
    exit (1);
  }

  # Step 3: Now copy data after the tag in the original file onto the destination
  # file.
  open (IN, "<" . $copy_file_name) ||
    die "cannot open file $copy_file_name specified in the <file> tag \n";
  $dest_tag_found = 0; #used as a flag
  foreach my $line (<IN>)
  {
    if ($dest_tag_found)
    { print OUT $line; }

    # If the hook is found, then don't write the hook onto OUT
    # as it would have been written earlier
    if (! $dest_tag_found &&
        $line =~ /$dest_tag_name/)
      { $dest_tag_found = 1; }
  }

  # Normal exit path
  close (IN);
  close (OUT);
  close (DEST);

  # Rename the tmp file to the file modified
  rename ($copy_file_name_tmp, $copy_file_name);
}

#################################################################
# commit_files: A procedure to commit all the copy files that
# were specialized back to the orginal files.
#################################################################
sub commit_files
{
  my ($path_name, $output_path_name, @files) = @_;

  # iterate over the file_name_list
  foreach my $file (@files)
  {
    # <file name="....">
    my $file_name = $file->getAttribute('name');

    # output_path == input_path then do an in place
    # substitution.
    if ($output_path_name eq $path_name)
    {
      rename ($path_name . "/" . $file_name . "copy",
              $path_name . "/" . $file_name);
    }
    else
    {
      # Check if the path_name exists. The path name
      # corresponds to a directory. So create it if it does
      # not exist.
      if (! -d $output_path_name)
      {
        #@@? Need to revert the *copy files?
        mkpath ($output_path_name, 0, 0744) ||
          die "cannot create $output_path_name: commit files failed! \n";
      }

      # move the specialized file to the output directory
      rename ($path_name . "/" . $file_name . "copy",
              $output_path_name . "/" . $file_name);
    }
  }
}

#### Main ########################################################
# Specialize_Component
# procedure to execute the transformations specified in the
# specialization file
##################################################################
sub Specialize_Components
{
  # Get the command line arguments
  my ($prefix_path, $spl_file, $output_prefix) = @_;

  my $parser = XML::DOM::Parser->new();
  my $doc = $parser->parsefile($spl_file);

  # Check if the prefix path ends with a / or not
  # if it does not then manually add the / to it
  my $last = substr ($prefix_path, -1);
  if ($last ne "/")
  { $prefix_path = $prefix_path . "/"; }

  # Entry Point: <transform> element
  foreach my $transform ($doc->getElementsByTagName('transform'))
  {
    # <module tags>
    foreach my $module ($transform->getElementsByTagName('module'))
    {
      # Complete path name to the module
      my $module_name = $module->getAttribute('name');
      my $path_name = $prefix_path . $module_name;

      # <file tags>
      my @files = $module->getElementsByTagName('file');
      foreach my $file (@files)
      {
	# <file name="....">
	my $file_name = $file->getAttribute('name');

	# Rather than modifying the files directly, make a local
	# copy of the files and then transform them and commit
	# if there is a file called foo we make a file foo_copy
	my $file_path_copy = $path_name . "/" . $file_name . "copy";
	my $file_path_name = $path_name . "/" . $file_name;

	copy ($file_path_name, $file_path_copy);

	# Diagnostic comment
	print "Instrumenting $file_name ..........";

        # <comment> ... </comment>
        my @comment_list = $file->getElementsByTagName ('comment');
        foreach my $comment (@comment_list)
        { Visit_Comment ($comment, $file_path_copy); }

        # <copy-from-source> ... </copy-from-source>
        my @copy_from_source_files =
          $file->getElementsByTagName ('copy-from-source');
        foreach my $copy_from_source (@copy_from_source_files)
        {
          Visit_Copy ($copy_from_source,
                      $file_path_copy,
                      $module_name,
                      $prefix_path);
        }

	# <remove> ... </remove>
	my @remove_list = $file->getElementsByTagName ('remove');
        foreach my $remove (@remove_list)
	{ Visit_Remove ($remove, $file_path_copy); }

	# <substitute ... </substitute>
	my @substitute_list = $file->getElementsByTagName ('substitute');
	foreach my $substitute (@substitute_list)
	{ Visit_Substitute ($substitute, $file_path_copy); }

	# <add> <hook> ...... </hook> <add>
	my @add_list = $file->getElementsByTagName ('add');
	foreach my $add (@add_list)
	{ Visit_Add ($add, $file_path_copy); }

	# Everything went well.. Print success
	print " [done] \n";
      }
    }

    # At this point all the specializations in all the modules have
    # succeeded. It is at this point that we need to commit the
    # specializations in each of the modules. That is move the temporary
    # file that we created to the main file that was specialized.
    # This also means that we need another loop and do the same thing
    # as above....
    # <module tags>
    foreach my $module ($transform->getElementsByTagName('module'))
    {
      # Complete path name to the module
      my $module_name = $module->getAttribute('name');
      my $path_name = $prefix_path . $module_name;

      # Output path name: append output_prefix to the
      # current module name. Append "/" to create a
      # directory like /foo/bar/baz/
      my $output_path = $output_prefix . "/" . $module_name;

      # <file tags>
      my @files = $module->getElementsByTagName('file');

      # commit the files
      commit_files ($path_name, $output_path, @files);
    }
  }
}

####
# Requiured for a module
####
1;
