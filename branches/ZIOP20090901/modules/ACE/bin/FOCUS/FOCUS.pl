eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;
#########################################################################
# FOCUS: A Feature Oriented Customization tool for ACE+TAO.
#
# Usage:
# ======
# --prefix-path - gives the prefix to the place where ACE+TAO is installed
# --reactor-spl - provide the component that needs to be specialized
# --protocol-spl - provide the concrete protocol to specialize
# --output-prefix-path -
#                  (Optional) if specified, copies all the files to the
#                  output prefix. The directory structure is maintained.
#                  That is if foo/bar.h file was specialized. Then the
#                  output will be <prefix_output>/foo/bar.h
#
# Adding New Specializations
# ===========================
# (1) Add a new command line option if the specialization belongs to a
#     new family of components. For example, Concurrency strategy
# (1.b) If not, then enhance exisiting option with a new value
# (2) Associate a specialization file for the concrete component being
#     specialized.
# (3) Update the specializations directory appropriately to add the
#     specializations file.
#
# @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
#
# $Id$
#
########################################################################
use Getopt::Long;

# This script can be invoked from anywhere. We need to add this to the
# path
use lib "$ENV{ACE_ROOT}/bin";

use FOCUS::Parser::FOCUSParser;

sub usage
{

  # How to use FOCUS
  # Currently FOCUS supports the specialization of reactor and protocol
  # families
  my $usage = "usage: \n ./FOCUS "
              . " \n --prefix-path=<path to module root>"
              . " [\n --reactor-spl=<select-mt, select-st, tp-reactor>"
              . " \n --protocol-spl=<iiop>"
              . " \n --wait-strategy-spl=<rw, lf>"
              . " \n --output-prefix=<output path>"
              . " \n --context-specific-optimizations=<dispatch>"
              . " \n ]"
              . "\n";
  print "$usage \n";
}

# Global Table that maps the specialization name to the file
# select => "../specializations/Reactor_Family/Select_Reactor.spl
# For each component that one needs to specialize add an entry in
# this simple global table to execute the specializations.

# Get the ACE_ROOT ENV variable. Then the specializations
# can be accessed via $ACE_ROOT/bin/FOCUS/.....
my $spl_prefix_path = "";
if (defined $ENV{"ACE_ROOT"})
{
  $spl_prefix_path = $ENV{"ACE_ROOT"};

  # Check if the prefix path ends with a / or not
  # if it does not then manually add the / to it
  my $last = substr ($spl_prefix_path, -1);
  if ($last ne "/")
  {
    $spl_prefix_path = $spl_prefix_path . "/";
  }

  # Add the bin/FOCUS directory to the prefix path
  $spl_prefix_path = $spl_prefix_path . "bin/FOCUS/";

}
else
{
  print "FOCUS: ACE_ROOT environment variable not defined. Exiting... \n";
  exit 1;
}

my %specialization_table  = (
 "select-mt", $spl_prefix_path . "specializations/Reactor_Family/Select_Reactor_MT.spl",
 "select-st", $spl_prefix_path . "specializations/Reactor_Family/Select_Reactor_ST.spl",
 "tp-reactor",$spl_prefix_path . "specializations/Reactor_Family/TP_Reactor.spl",
 "iiop", $spl_prefix_path . "specializations/Protocol_Family/IIOP/iiop.spl",
 "rw",   $spl_prefix_path . "specializations/Wait_Strategy/Wait_On_Read.spl",
 "lf",   $spl_prefix_path . "specializations/Wait_Strategy/Wait_On_Leader_Follower.spl",
 "dispatch", $spl_prefix_path . "specializations/Context-Specific-Optimizations/Dispatch-Resolution-Optimization.spl");

# Get the command line options
my $ret = GetOptions ("prefix-path=s"   => \$prefix_path,
                      "reactor-spl=s"   => \$reactor_type,
                      "protocol-spl=s"  => \$protocol_type,
                      "wait-strategy-spl=s" => \$wait_strat_type,
                      "output-prefix:s" => \$output_prefix,
                      "context-specific-optimizations=s" => \$context_opt);

# Check if the prefix path is specified
if (!$prefix_path)
{
  usage;
  exit 1;
}

#
# Check if the optional outputpath is specified
# if it not specified then the output_prefix will
# be same as the prefix_path.
# So if the prefix is something like:
# /build/arvindk/ACE_wrappers, then the output_prefix
# will be the same.
#
# Upshot: The source files will be directly replaced.
#
if (! $output_prefix)
{
  $output_prefix = $prefix_path;
}

# Check if the type of each component specialized is valid!
if ($reactor_type)
{
  my $spl_file = $specialization_table {$reactor_type};
  if (! length ($spl_file))
  {
    print "Invalid reactor type specified \n";
    usage;
    exit 1;
  }

  # specialize the Reactor component
  FOCUSParser::Specialize_Components ($prefix_path, $spl_file, $output_prefix);
}

# protocol specialization
if ($protocol_type)
{
  my $spl_file = $specialization_table {$protocol_type};
  if (! spl_file)
  {
    print "Invalid reactor type specified \n";
    usage;
    exit 1;
  }

  # specialize the Protocol component
  FOCUSParser::Specialize_Components ($prefix_path, $spl_file, $output_prefix);
}

# Wait Strategy specialization
if ($wait_strat_type)
{
  my $spl_file = $specialization_table {$wait_strat_type};
  if (! spl_file)
  {
    print "Invalid specialization for the Wait strategy \n";
    usage;
    exit 1;
  }

  # specialize the wait strategy
  FOCUSParser::Specialize_Components ($prefix_path, $spl_file, $output_prefix);
}

# Context specific optimizations present
if ($context_opt)
{
  my $spl_file = $specialization_table {$context_opt};
  if (! spl_file)
  {
    print "Invalid Context specialization optimization specified\n";
    usage;
    exit 1;
  }

  # specialize the wait strategy
  FOCUSParser::Specialize_Components ($prefix_path, $spl_file, $output_prefix);
}
