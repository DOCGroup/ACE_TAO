Welcome to the top-secret Samwise project.

-------------------------------------------------------------------------------
Somewhat of an Overview

Samwise is a Perl-based system for generating project/makefiles for ACE/TAO 
from a common configuration file.

The plan is to use an XML based configuration file and generate MSVC6, MSVC7,
Borland, and GNU Makefiles from this file.  Possibly other targets (MSEVC and
AutoMake) will be added.  

-------------------------------------------------------------------------------
Why?

People are lazy and do not want to hand edit the 5 or so files that may need
to be changed for each additional file change for a project.  And very few
people know how to correctly edit all of them (those that know the Makefiles
well are not good with MSVC6's DSP files, and vice-versa).  Also, MSVC7 
requires a new set of project files, making the problem even worse.

The makefile scheme could do with some enforcement of consistency and I don't
actually trust anybody to keep the DSP files correct (and actually, after 
seeing the inconsistency of them, I guess I shouldn't have trusted myself to 
keep them consistent either).  

A couple of other extra bonuses are that a standard generator of project files
can make the task of system wide changes easier.  If the ace library were 
ever moved into a subdirectory (so ace->ACE/ace, tests->ACE/tests, etc.) it
would require only changing the samwise scripts instead of the individual
project files.  And having a common source for project information allows new
generators to be added to output for new targets.  

And finally, ACE/TAO needed more complex Perl scripts.  

-------------------------------------------------------------------------------
Use

- Run create_sam.pl in a directory to create a sam.xml file.  
- Run sam.pl to generate files

Right now only the msvc6 target is on my default.  To output other targets,
pass "-target foo".  Example: If you wanted to target both Borland and 
GNUMake, use:

sam.pl -target borland -target gnumake

-------------------------------------------------------------------------------
Random Notes

Documentation?  You're looking at it; beautiful, ain't it?  TODO.txt has more
of the current progress.  

Each directory should have one sam.xml file, which specifies one workspace
and one or more projects.  The workspace details which projects should be
included in the default set.  It also details which subdirectories which 
should be recursed into (for those targets that support it).

The sam.pl script contains two main sections, the Parser and the Generator.
The only parser is a light-weight custom parser that can understand a limited
set of XML, which happily coincides with the limited set of XML used by
create_sam.pl to produce sam.xml files.  I wouldn't try to push it that much.
Hand generated sam.xml files should look just like the generated ones:
keep <tag></tag> pairs on separate lines, don't mess too much with spacing,
and so on.

I use a tab size of 4 for perl scripts and sam.xml files.  

The create_sam.pl script is intended to only be needed in the initial 
converstion to sam.xml files.  Hence I don't intend on it surviving the
switchover.  All it does is read the dsp files for project information, and
Makefiles for subdir information.

-------------------------------------------------------------------------------
Closing

So has been written by the self-proclaimed Chief Architect, Darrell Brunsch