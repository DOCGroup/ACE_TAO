#! /bin/sh
#
# $Id$
#
# Parse the build.txt files from an autobuild that was generated with the 
# g++_metric.sh script, e.g., with CXX=g++_metric.sh which outputs
# compile times on a per object basis, and use the data to generate time 
# series graphs with gnuplot.
#
# For use with an autobuild, place a line something like this in the xml file, 
# after the log file is closed, but before it's moved.
#
#  <command name="shell"  options="$ACE_ROOT/bin/generate_compile_stats.sh <path>/build.txt <destination>" />
#

usage ()
{
  echo "usage: $0 <input_file> <destination_directory> [<date> [<fudge_factor>]]"
  exit
}

# Parse the commandline and validate the inputs
parse ()
{
  #echo "parse $@"
  # set input file and destination (required) 
  if [ $# -gt 1 ]; then
    INFILE=$1
    DEST=$2

    if ! [ -e "$INFILE" ]; then
      echo "input_file $INFILE does not exist."
      usage
    fi
    if ! [ -d "$DEST" ]; then
      echo "destination_directory $DEST does not exist."
      usage
    fi
    if ! [ -w "$DEST" ]; then
      echo "destination_directory $DEST is not writable."
      usage
    fi
  else
    usage
  fi

  # set the date from command line
  if [ $# -gt 2 ]; then
    DATE=$3
  else
    DATE=`date +%Y/%m/%d-%H:%M`
  fi

  # set fudge factor from commandline (for testing)
  if [ $# -gt 3 ]; then
    FUDGE_FACTOR=$4
  else
    FUDGE_FACTOR=0
  fi
}

# Generate the actual charts and move them to $DEST
gen_chart ()
{
  local object=$1

  # low and high aren't being used right now since we don't have
  # a good idea of what to make them yet.  So, we let gnuplot
  # automatically create them
  #local low=$2
  #local high=$3

  gnuplot <<EOF
    #set grid
    set time
    set xdata time
    set timefmt "%Y/%m/%d-%H:%M"
    set format x "%Y/%m/%d-%H:%M"
    set xtics rotate
    set xlabel 'Date (YYYY/MM/DD)' 0,-3
    set ylabel 'Compile Time (milliseconds)'
    set terminal png small color
    set yrange [$low:$high]
    set output ".metrics/images/${object}.png"
    set title "${object//___//}" 
    plot '.metrics/data/${object}.txt' using 1:2 notitle w l
    exit
EOF

  # copy the data and images to $DEST
  /bin/cp .metrics/images/${object}.png $DEST/images/${object}.png
  /usr/bin/tac .metrics/data/${object}.txt > $DEST/data/${object}.txt
  /usr/bin/tail -5 .metrics/data/${object}.txt > $DEST/data/LAST_${object}.txt
}

# Make a datafile entry, <date> <milliseconds>
make_entry ()
{
  echo -n $DATE " ";
  echo $1
}

# Make sure hidden directory tree exists, and create it if it doesn't
create_dirs ()
{
  if ! [ -d ".metrics" ]; then
    mkdir .metrics
  fi
  if ! [ -d ".metrics/data" ]; then
    mkdir .metrics/data
  fi
  if ! [ -d ".metrics/images" ]; then
    mkdir .metrics/images
  fi
}


# Process the the $INPUT file
process_file ()
{
  #echo "process_file: $@"

  OBJECTS=""
  TOTAL_TIME=0
  CURRENT_TIME=0
  CURRENT_OBJECT=""
  #LAST_OBJECT=""
  CURRENT_PATH=""

  # read from $INFILE (std input)
  while read unused1 unused2 target time; do

    # keep track of the last read object (needed to determine if you need to 
    # rollup a lib or executable.
    LAST_OBJECT=$CURRENT_OBJECT

    # get path
    CURRENT_PATH=${target%/*}
    
    # strip off the hidden directory if needbe
    CURRENT_PATH=${CURRENT_PATH%/.*}

    # replace all "/" with "___" (so we can keep them all in the same directory)
    CURRENT_PATH=${CURRENT_PATH//\//___}

    # strip path off of target
    CURRENT_OBJECT=${CURRENT_PATH}___${target##*/}
    #echo "target = $target, object = $CURRENT_OBJECT,  path = $CURRENT_PATH"
    
    if [ "$CURRENT_OBJECT" != "${CURRENT_OBJECT%.o}" ]; then
      # this is a .o file, so send it to it's own file and roll it
      # up into the larger lib or executable

      let "CURRENT_TIME=($time/1000)+$FUDGE_FACTOR"
      make_entry $CURRENT_TIME >> .metrics/data/${CURRENT_OBJECT}.txt
      let "TOTAL_TIME+=$CURRENT_TIME"
      OBJECTS="$OBJECTS $CURRENT_OBJECT"
    else
      if [ "${CURRENT_OBJECT}" != "${LAST_OBJECT%.o}" ]; then
        # only rollup if you have multiple objects (this isn't always true
        # for some executables)

        make_entry $TOTAL_TIME >> .metrics/data/${CURRENT_OBJECT}.txt
        OBJECTS="$OBJECTS $CURRENT_OBJECT"
      fi
      
      # reset the total time even if you didn't rollup the object, e.g. 
      # executables with a single object file. 
      let "TOTAL_TIME=0"

    fi

  done

  for i in $OBJECTS; do
    # use a list of objects for various yranges
    # todo: this is currently turned off, but the idea will be to have a list
    #       of objects for each scale, and let all the others default to the
    #       0:10000 (0-10 second) range.
    if [ "$i" != "${i%libACE*}" ]; then
      # libraries can take a lot longer, so increase the range for them
      LOW=0
      HIGH=600000
    else
      # finally, individual object files should be the quickest...
      LOW=0
      HIGH=10000
    fi

    gen_chart "${i}" "$LOW" "$HIGH" >/dev/null 2>&1
  done

}

INFILE=""
DEST=""
DATE=""
FUDGE_FACTOR=0

parse $@
create_dirs

#echo "infile=$INFILE, dest=$DEST, date=$DATE, ffactor=$FUDGE_FACTOR"
grep "compile time" $INFILE | process_file

