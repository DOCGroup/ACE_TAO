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

# this only works for english
# assumes the date is formatted like this:  Sat Apr 12 18:19:31 UTC 2003
# and outputs this: 2003/04/12-18:19
parse_time ()
{
  # todo: add a format parameter
  local INDEX=0
  local PT_MONTH=""
  local PT_DAY=""
  local PT_YEAR=""
  local PT_HOUR=""
  local PT_MINUTE=""
  local PT_SECOND=""
  local PT_TIMEZONE=""
  
  read -a line
  for token in "${line[@]}"; do
    #echo "$INDEX = $token"
    case $INDEX in
      1 ) case $token in
            Jan ) PT_MONTH="01" ;;
            Feb ) PT_MONTH="02" ;;
            Mar ) PT_MONTH="03" ;;
            Apr ) PT_MONTH="04" ;;
            May ) PT_MONTH="05" ;;
            Jun ) PT_MONTH="06" ;;
            Jul ) PT_MONTH="07" ;;
            Aug ) PT_MONTH="08" ;;
            Sep ) PT_MONTH="09" ;;
            Oct ) PT_MONTH="10" ;;
            Nov ) PT_MONTH="11" ;;
            Dec ) PT_MONTH="12" ;;
          esac ;;
      2 ) PT_DAY="$token" ;;
      3 ) PT_HOUR="${token%%:*}"
          PT_MINUTE="${token%:*}"
          PT_MINUTE="${PT_MINUTE#*:}"
          PT_SECOND="${token##*:}" ;;
      4 ) PT_TIMEZONE="$token" ;;
      5 ) PT_YEAR="$token" ;;
    esac 
    let INDEX=$INDEX+1
  done
  if [ "$1" = "debug" ]; then
    echo "month = $PT_MONTH"
    echo "day   = $PT_DAY"
    echo "year  = $PT_YEAR"
    echo "hour  = $PT_HOUR"
    echo "min   = $PT_MINUTE"
    echo "sec   = $PT_SECOND"
    echo "tz    = $PT_TIMEZONE"
  fi
  echo "$PT_YEAR/$PT_MONTH/$PT_DAY-$PT_HOUR:$PT_MINUTE"
}

# grab date from line with following format: 
#################### End [Fri Apr 11 00:18:31 2003 UTC]
# and return it in this format: Fri Apr 11 00:18:31 UTC 2003 which is
# what parse_time() expects
strip_date ()
{
  local INDEX=0
  local TEMP_DATE=""
  local DATE=""
  read -a line
  for token in "${line[@]}"; do
    #echo "$INDEX = $token"
    case $INDEX in
       2 ) DATE=${token#[} ;;
       7 ) DATE="$DATE ${token%]} $TEMP_DATE" ;;
       # this is a hack since the autobuild scripts don't format the date
       # correctly...  :-(
       6 ) TEMP_DATE=$token ;;
       * ) DATE="$DATE $token" ;;
    esac
    let INDEX=$INDEX+1
  done
  echo $DATE
}

# Parse the commandline and validate the inputs
parse ()
{
  echo "parse()"
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
  local DEST=$2
  # low and high aren't being used right now since we don't have
  # a good idea of what to make them yet.  So, we let gnuplot
  # automatically create them
  #local low=$3
  #local high=$4

  gnuplot <<EOF
    #set grid
    set time
    set xdata time
    set timefmt "%Y/%m/%d-%H:%M"
    set format x "%Y/%m/%d"
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
  echo "create_dirs()"
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
  echo "process_file()"

  CURRENT_TIME=0
  CURRENT_OBJECT=""
  CURRENT_PATH=""

  while read target time; do

    # get path
    CURRENT_PATH=${target%/*}
    
    # strip off the hidden directory if needbe
    CURRENT_PATH=${CURRENT_PATH%/.*}

    # replace all "/" with "___" (so we can keep them all in the same directory)
    CURRENT_PATH=${CURRENT_PATH//\//___}

    # strip path off of target
    CURRENT_OBJECT=${CURRENT_PATH}___${target##*/}
    #echo "target = $target, object = $CURRENT_OBJECT,  path = $CURRENT_PATH"
    
    let "CURRENT_TIME=($time/1000)+$FUDGE_FACTOR"
    make_entry $CURRENT_TIME >> .metrics/data/${CURRENT_OBJECT}.txt

  done # while
}

composite_list ()
{
  local FOUND_OBJ=0
  local BASE_OBJ_FLAG=0
  local BASE_OBJ=""
  local OBJ_LIST=""
  local DIR_LINE=0
  local DIR=""
  local INDEX=0

  while read -a line; do
    DIR_LINE=0
    INDEX=0
    for i in "${line[@]}"; do
      if [ $DIR_LINE -eq 1 ]; then
        if [ $INDEX -eq 3 ]; then
          DIR="${i%?}"                           # strip off last "'"
          DIR="${DIR#*ACE_wrappers/}"            # strip off $ACE_ROOT
          DIR="${DIR//\//___}___"                # replace "/" with "___"
          break
        else
          let INDEX="$INDEX+1"
          continue
        fi
      fi

      if [ "${i%[*}" = "make" ]; then
        let DIR_LINE=1
        let INDEX="$INDEX+1"
        continue
      fi

      if [ $BASE_OBJ_FLAG -eq 1 ]; then
         BASE_OBJ="${DIR}${i##.*/}"
         BASE_OBJ_FLAG=0
      elif [ "$i" = "-o" ]; then
        # found our base object, set flag so we can grab the next one
        BASE_OBJ_FLAG=1
      elif [ "$i" = "${i#-}" -a "$i" = "${i#/}" -a "$i" != "${i%.o}" ]; then
        OBJ_LIST="$OBJ_LIST ${DIR}${i##*/}"
        FOUND_OBJ=1
      fi 
    done # for
    if [ $FOUND_OBJ -eq 1 ]; then
      echo "$BASE_OBJ : $OBJ_LIST"
      FOUND_OBJ=0
      OBJ_LIST=""
      BASE_OBJ=""
    fi
  done # while
}

create_composite_list ()
{
  echo "create_composite_list()"
  local INFILE=$1

  # create a pattern file
  echo "\-L" > .metrics/temp.txt
  echo "Entering directory" >> .metrics/temp.txt

  cat $INFILE | grep -f .metrics/temp.txt | grep -B1 "\-L" | grep -ve "--" \
   | composite_list > .metrics/composites.txt
}

process_composite_objects ()
{
  echo "process_composite_objects()"
  local TOTAL_TIME=0
  local temp
  local tdate=""
  local ttime=0
  local lpath=".metrics/data/"
  while read outfile colon infiles; do
    #echo "$outfile ----- $infiles"
    for i in $infiles; do
      temp=`tail -n 1 ${lpath}${i}.txt`
      tdate=${temp%% *}
      let ttime="${temp##* }"

      if [ "$tdate" = "$DATE" ]; then
        let TOTAL_TIME="$TOTAL_TIME + ${ttime}"
      fi
    done # for
    echo "$DATE $TOTAL_TIME" >> ${lpath}${outfile}.txt
    let TOTAL_TIME=0
  done # while
  
}

create_images ()
{
  echo "create_images()"
 
  local DEST=$1
  local LOW=0
  local HIGH=10000

  while read object; do
    # use a list of objects for various yranges
    # todo: this is currently turned off, but the idea will be to have a list
    #       of objects for each scale, and let all the others default to the
    #       0:10000 (0-10 second) range.
    if [ "$object" != "${object%libACE*}" ]; then
      # libraries can take a lot longer, so increase the range for them
      LOW=0
      HIGH=600000
    else
      # finally, individual object files should be the quickest...
      LOW=0
      HIGH=10000
    fi
    
    object="${object%.txt}"
    gen_chart "${object##*/}" "$DEST" "$LOW" "$HIGH" >/dev/null 2>&1
  done  

}

create_page ()
{
  local BASE=$1
  shift
  #local OBJECTS=$2
  local TITLE="Compilation metrics for ${BASE//___//}"

  # header
  echo "<html>"
  echo "<head><title>$TITLE</title></head>"
  echo '<body text = "#000000" link="#000fff" vlink="#ff0f0f" bgcolor="#ffffff">'
  echo "<br><center>$TITLE</center><br>"
  if [ -e ".metrics/images/$BASE.png" ]; then
    echo '<DIV align="center"><P>'
    echo "<IMG alt=\"$BASE\" border=0 src=\"images/$BASE.png\""
    echo 'width="640" height="480"></P></DIV><HR>'
  fi

  #echo "$OBJECTS<br>"
  # now list all the interal object (if more than one)
  if [ -n "$2" ]; then
    echo "<ul>"
    for i in $@; do
      if [ -e ".metrics/${i}.html" ]; then
        echo "<li><a href=\"$i.html\">${i//___//}</a>"
      elif [ -e ".metrics/images/${i}.png" ]; then
        echo "<li><a href=\"images/$i.png\">${i//___//}</a>"
      fi    
    done # for
    echo '</ul>'
  fi

  # footer
  echo '</body></html>'

}

create_html ()
{
  echo "create_html()"

  local DEST=$1
  local ALL_BASE=""

  while read base colon files; do
    #echo "$base"
    # create individual page for app/lib
    #echo "creating $base.html with $files"
    create_page $base $files > .metrics/$base.html
    cp .metrics/$base.html $DEST/$base.html
    ALL_BASE="$ALL_BASE $base"
  done

  # create main page
  #echo "createing index.html with $ALL_BASE"
  create_page "ACE+TAO" $ALL_BASE > .metrics/index.html
  cp .metrics/index.html $DEST/index.html

}

############ main program

INFILE=""
DEST=""
DATE=""
FUDGE_FACTOR=0

parse $@
create_dirs

DATE=`tail -n 1 $INFILE | strip_date | parse_time`
echo "date = $DATE"

# grab the compile time metrics for objects only and process them
grep "compile time:" $INFILE | grep "\.o" | cut -d' ' -f3,4 | process_file

create_composite_list $INFILE

cat .metrics/composites.txt | process_composite_objects 

ls .metrics/data/*.txt | create_images $DEST

cat .metrics/composites.txt | create_html $DEST


# 0) grab the date off the tail of the input file and use it
#    for the dates in the data files 
# 0.1) validate input file before starting processing, date and compile times
# 1) process the objects first
# 2) then grab all the rollups and add up the objects for each and add
#    and entry for the rolledup object.
# 3) then gen all the graphs at once and move them over...
