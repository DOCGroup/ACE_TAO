#!/bin/sh
# ====================================================================
#
# OpenCCM: The Open CORBA Component Model Platform
# Copyright (C) 2000-2003 INRIA - USTL - LIFL - GOAL
# Contact: openccm@objectweb.org
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
# USA
#
# Initial developer(s): Romain Rouvoy.
# Contributor(s): Philippe Merle.
#
# ====================================================================
#
# Starting the Ant Compilation Process.
#
# ====================================================================

#
# Set up the environment.
#
if [ -z "$JAVA_HOME" ] ; then
  JAVAC=`which java`
  if [ -z "$JAVAC" ] ; then
    echo "Cannot find JAVA. Please set your PATH."
    exit 1
  fi
  JAVA_BIN=`dirname $JAVAC`
  JAVA_HOME=$JAVA_BIN/..
  export JAVA_HOME
else
  JAVA_BIN=$JAVA_HOME/bin
fi

# Added for Bug #468.
# Contributor: Philippe Merle.
 
if [ -z "$OpenCCM_HOMEDIR" ] ; then
  echo "Please set the OpenCCM_HOMEDIR environment variable."
  exit 1
fi

# Add end.

#
# Determinate what the path separator is:
# - On Unix systems, it is ':'
# - On CygWin systems, it is ';'
#
PS=':'
if [ $OSTYPE = "cygwin32" ] || [ $OSTYPE = "cygwin" ] ; then
    PS=';'
fi
 
#
# Add the path to the 'java' command in the $PATH variable because
# the OpenCCM Ant build.xml could start the 'javacc' task that
# requires that the 'java' command was accessible through the $PATH.
#
PATH=$JAVA_BIN:$PATH
export PATH

# Update for Bug #468.
# Contributor: Philippe Merle.
#
# CLASSPATH=@CLASSPATH@
#
# #$JAVA_BIN/java -cp $CLASSPATH -Dant.home=@ANT_HOME@ org.apache.tools.ant.Main "$@" -buildfile build.xml
# @JAVA@ -cp $CLASSPATH -Dant.home=@ANT_HOME@ org.apache.tools.ant.Main "$@" -quiet -buildfile build.xml

#
# Concatenate all the libraries in the externals/ant directory and
# append them to the classpath environment variable.
#
CLASSPATH=`echo $OpenCCM_HOMEDIR/externals/ant/*.jar | tr ' ' ${PS}`${PS}$CLASSPATH
CLASSPATH=$JAVA_HOME/lib/tools.jar${PS}$CLASSPATH

#
# Execute the build tool passing the build.xml file.
#
$JAVA_BIN/java -cp $CLASSPATH -Dant.home=$OpenCCM_HOMEDIR/externals/ant org.apache.tools.ant.Main "$@" -q -buildfile build.xml

#
# Update end.
