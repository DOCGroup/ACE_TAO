#!/bin/sh
########################## Begin Install Script ##########################
#
# NAME:         ACE-install.sh
#
# PURPOSE:      Bourne shell script to install ACE for UNIX platforms
#
# AUTHOR:       Ajit Sagar <asagar@spdmail.spd.dsccc.com>
#
# HISTORY:      5/20/96     Ajit Sagar          Created
#               x/xx/xx     xxxxxxxxxxxxxx      Modified
#               x/xx/xx     xxxxxxxxxxxxxx      Modified
#
# DESCRIPTION:  This script installs the ACE toolkit for a Unix system.
#               Before using this script, you must do the following:
#
# NOTES:        1) Download the gzipped version of ACE from
#                  http://www.cs.wustl.edu/~schmidt/ (Select "Obtaining ACE")
#		  			OR
#               ftp from wuarchive.wustl.edu in the /languages/c++/ACE directory
#
#                  The local directory to which this will be downloaded is
#                  refered to as the "MY_ACEDIR" in this script.
#
#		2) Make sure there is only one file that matches "ACE*tar*".
#                  This should be the one you just downloaded.
#		   remove older versions of tar files downloaded earlier.
#
#               3) At the end of this script, you should have:
#
#                  a) A compiled toolkit in the $WRAPPER_ROOT directory
#                  b) If you selected to save the existing installation,
#                     a file oldACE.tar.gz will be created in MY_ACEDIR
#                  c) The gzipped ACE source file that was initially downloaded.
#
#               4) This script may be executed without user interaction
#                  by using the "auto" option (see USAGE section below).
#                  It is advisable to execute it without the "auto"  option the
#                  first time to make sure everything works.
#
#               THE FOLLOWING NEED TO BE DONE ONLY THE FIRST TIME THIS
#               SCRIPT IS RUN:
#
#               5) Read the README file in
#                  http://www.cs.wustl.edu/~schmidt/ACE.html directory
#
#               6) Define an environment variable WRAPPER_ROOT the
#                  defines which directory your ACE toolkit is to be
#                  installed in.
#
#                  DO NOT MAKE "WRAPPER_ROOT" THE SAME AS "MY_ACEDIR".
#
#               7) Redefine "MY_ACEDIR" in this script to default
#                  to the directory in which you have downloaded the ACE source.
#
#               8) This script has been tested on Solaris 2.x for the
#                  Sun C++4.0 compiler. To use this on some other UNIX
#                  platform, uncomment and redefine the following
#                  in this script:
#
#                  MY_ACE_CONFIG
#                  MY_ACE_GNU_MACROS
#
#		   Check the README file to define these variables. If
#                  these are not defined, the script will prompt you
#                  help you to change these variables. But in that case
#                  you cannot run with the "auto" option.
#
#
# USAGE:        myacemaker [ auto ]
#
#               auto       automatic make (no user interaction)
#
#                          Under this option, the source will be
#                          extracted and compiled without any user interaction,
#                          i.e., you can kick off this script, go home,
#                          and check it in the morning. The following
#                          assumptions are made:
#
#                            1) MY_ACE_DIR is properly defined in this script.
#                            2) MY_ACE_CONFIG is properlly defined
#                               in this script.
#                            3) MY_ACE_GNU_MACROS is properly defined in this
#                               script.
#                            4) A backup of existing installation
#                               is to be made.
#
# COPYRIGHT INFORMATION:
#
# You are free to do anything you like with this script such as
# including it in commercial software. You may modify it and freely redistribute
# it. The author accepts no responsibility for any bugs or problems that
# arise as a consequence of using this code.
#
# -- Ajit Sagar
#
#########################################################################
#
ACE_ERRFILE=/tmp/myacemaker.err
OLD_ACE_TAR_FILE=oldACE.tar
OLD_ACE_GZIPPED_TAR_FILE=oldACE.tar.gz
USAGE="<Usage> : myacemaker [ auto ]"
#
##############################################################
#
# Platform-specific definitions
#
##############################################################

# Directory where ACE-x.x.xx.tar.gz is located

MY_ACEDIR=${HOME}/aceconfig

#######################################################
# Platform specific config file in ${WRAPPER_ROOT}/ace/
# Uncomment and redefine this
#######################################################

# MY_ACE_CONFIG=config-sunos5.4-sunc++-4.x.h

#######################################################
# Platform specific GNU macros file in
# ${WRAPPER_ROOT}/include/makeinclude/
# Uncomment and redefine this
#######################################################

# MY_ACE_GNU_MACROS=platform_sunos5_sunc++.GNU


##############################################################
#
# Main Script For Installing ACE
#
##############################################################

AUTO="$#"

WRAPPER_ROOT=${WRAPPER_ROOT:?"ERROR: Environment variable WRAPPER_ROOT not set"} 
if [ ${AUTO} -gt 1 ]
then
  echo "$USAGE"
  exit 1
fi

if [ ${AUTO} -eq 1 ]
then
  if [ $1 != "auto" ]
  then
    echo "$USAGE"
    exit 2
  fi
fi

echo ""
echo "ACE source in `ls ${MY_ACEDIR}/ACE*tar.gz`"
echo "ACE will be installed in ${WRAPPER_ROOT}"
echo ""

if [ $AUTO -eq 0 ]
then
  echo "OK to continue? [Y/N] : \c"
  read choice
else
  choice='Y'
fi

if [ ${choice} != 'Y' -a ${choice} != 'y' ]
then
  echo ""
  echo "ACE installation aborted"
  echo ""
  exit 11
fi

cd ${MY_ACEDIR}

echo "Uncomressing ACE archive ..."

gunzip ACE*tar.gz || \
{
  echo "gunzip failed. Aborting script !!"
  echo ""
  exit 22
}

echo ""
echo "Extracting ACE files into `pwd`/ACE_wrappers directory ..."
echo ""

tar xvf ACE*tar || \
{
  echo ""
  echo "tar failed. Aborting script !!"
  echo ""
  exit 33
}

echo ""
echo "Re-compressing ACE source using <gzip -9> ..."
gzip  -9 ACE*tar
echo ""

if [ -d ${WRAPPER_ROOT} ]
then

  if [ $AUTO -eq 0 ]
  then
    echo "Save a copy of existing ACE installation? [Y/N] : \c"
    read choice
    echo ""
  else
    choice='Y'
  fi

  if [ ${choice} = 'Y' -o ${choice} = 'y' ]
  then

   echo "Archiving ${WRAPPER_ROOT} to `pwd`/${OLD_ACE_TAR_FILE} using <tar> ..."
    tar cvpf ${OLD_ACE_TAR_FILE} ${WRAPPER_ROOT}
    echo ""

    if [ -f ${OLD_ACE_GZIPPED_TAR_FILE} ]
    then
      echo "Removing ${OLD_ACE_GZIPPED_TAR_FILE} ..."
      rm ${OLD_ACE_GZIPPED_TAR_FILE}
      echo ""
    fi

    echo "Compressing ${OLD_ACE_TAR_FILE} using <gzip -9> ..."
    gzip  -9 ${OLD_ACE_TAR_FILE}
    echo ""

  fi

  echo "Removing ${WRAPPER_ROOT} ..."
  rm -r ${WRAPPER_ROOT}
  echo ""
fi

echo "Moving `pwd`/ACE_wrappers to ${WRAPPER_ROOT} ..."
mv ./ACE_wrappers ${WRAPPER_ROOT}
echo ""

cd ${WRAPPER_ROOT}/ace

if [ ${MY_ACE_CONFIG:-undefined} = undefined ]
then

  if [ $AUTO -eq 1 ]
  then
    if [ ! -f ${MY_ACE_CONFIG} ]
    then
      echo ""
      echo "${MY_ACE_CONFIG} does not exist. Aborting script ..."
      echo ""
      exit 44
    fi
  fi

  echo "Select one of the following files for linking to config.h"
  echo ""
  echo "`ls config*h`"
  echo ""

  echo "Type the filename for your compiler:  \c"
  read MY_ACE_CONFIG
  echo ""

  if [ ! -f ${MY_ACE_CONFIG} ]
  then
    echo ""
    echo "${MY_ACE_CONFIG} does not exist. Aborting script ..."
    echo ""
    exit 55
  fi

fi


echo "Creating link config.h for ${MY_ACE_CONFIG} in directory `pwd` ..."

rm ./config.h
ln -s ${MY_ACE_CONFIG} config.h

echo ""

cd ${WRAPPER_ROOT}/include/makeinclude

if [ ${MY_ACE_GNU_MACROS:-undefined} = undefined ]
then

  if [ $AUTO -eq 1 ]
  then
    if [ ! -f ${MY_ACE_GNU_MACROS} ]
    then
      echo ""
      echo "${MY_ACE_GNU_MACROS} does not exist. Aborting script ..."
      echo ""
      exit 66
    fi
  fi

  echo "Select one of the following files for linking to config.h"
  echo ""
  echo "`ls platform*GNU`"
  echo ""
  echo "Type the filename for your compiler:  \c"

  read MY_ACE_GNU_MACROS
  echo ""

  if [ ! -f ${MY_ACE_GNU_MACROS} ]
  then
    echo ""
    echo "${MY_ACE_GNU_MACROS} does not exist. Aborting script ..."
    echo ""
    exit 77
  fi

fi

echo "Creating link platform_macros.GNU for ${MY_ACE_GNU_MACROS}"
echo "in directory `pwd` ..."

rm ./platform_macros.GNU 
ln -s ${MY_ACE_GNU_MACROS} platform_macros.GNU 

echo ""

if [ ${AUTO} -eq 0 ]
then
  echo "Make ACE now with default setup? [Y/N] : \c"
  read choice
  echo ""
else
  choice='Y'
fi

if [ ${choice} != 'Y' -a ${choice} != 'y' ]
then
  echo ""
  echo "ACE make skipped. You can use <gmake> to make ACE later"
  echo ""
  exit 0
fi

echo ""
echo "Making ACE now. Examine file ${ACE_ERRFILE} for errors ..."
echo ""

cd ${WRAPPER_ROOT}

echo ""
echo "Executing <gmake> ..."
echo ""
gmake | tee ${ACE_ERRFILE} 2>&1

echo ""
echo "Examine file ${ACE_ERRFILE} for possible errors ..."
echo ""
echo "********** End of myacemaker script !!! ***********"
echo ""

#
#########################################################################
