dnl -------------------------------------------------------------------------
dnl       $Id$
dnl 
dnl       subsets.m4
dnl
dnl       ACE M4 include file which contains ACE specific M4 macros
dnl       that set/determine which ACE subsets to build.
dnl 
dnl -------------------------------------------------------------------------

dnl  Copyright (C) 1998, 1999, 2001  Ossama Othman
dnl
dnl  All Rights Reserved
dnl
dnl This library is free software; you can redistribute it and/or
dnl modify it under the current ACE distribution terms.
dnl 
dnl This library is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


dnl Check for thread related libraries and compiler flags
dnl Usage: ACE_CHECK_SUBSETS
AC_DEFUN([ACE_CHECK_SUBSETS],
[
dnl Begin ACE_CHECK_SUBSETS

dnl Assume all subsets will be built, including the full ACE library.
dnl If any of the components is explicitly enabled or disabled by the user
dnl then do NOT build the full ACE library.
AC_ARG_ENABLE([lib-all],
     AC_HELP_STRING([--enable-lib-all],[build all ACE components [[no]]]),
              [
               case "${enableval}" in
                yes)
                  ace_user_enable_lib_all=yes
                  ;;
                no)
                  ace_user_enable_lib_all=no
                  ;;
                *)
                  AC_MSG_ERROR([bad value ${enableval} for --enable-lib-all])
                  ;;
               esac
              ],
              [
               ace_user_enable_lib_all=no
              ])

AC_ARG_ENABLE([lib-full],
     AC_HELP_STRING([--enable-lib-full],[build the full ACE library [[yes]]]),
              [
               case "${enableval}" in
                yes)
                  ace_user_enable_lib_full=yes
                  ;;
                no)
                  ace_user_enable_lib_full=no
                  ;;
                *)
                  AC_MSG_ERROR([bad value ${enableval} for --enable-lib-full])
                  ;;
               esac
              ],
              [
               ace_user_enable_lib_full=yes
              ])

AC_ARG_ENABLE([lib-os],
     AC_HELP_STRING([--enable-lib-os],[build ACE_OS library]),
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_OS
                  ;;
                no)
                  ace_user_enable_lib_os=no
                  ;;
                *)
                  AC_MSG_ERROR([bad value ${enableval} for --enable-lib-os])
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE([lib-utils],
     AC_HELP_STRING([--enable-lib-utils],[build ACE_Utils library]),
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_UTILS
                  ;;
                no)
                  ace_user_enable_lib_utils=no
                  ;;
                *)
                  AC_MSG_ERROR([bad value ${enableval} for --enable-lib-utils])
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE([lib-logging],
     AC_HELP_STRING([--enable-lib-logging],[build ACE_Logging library]),
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_LOGGING
                  ;;
                no)
                  ace_user_enable_lib_logging=no
                  ;;
                *)
                  AC_MSG_ERROR([bad value ${enableval} for --enable-lib-logging])
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE([lib-threads],
     AC_HELP_STRING([--enable-lib-threads],[build ACE_Threads library]),
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_THREADS
                  ;;
                no)
                  ace_user_enable_lib_threads=no
                  ;;
                *)
                  AC_MSG_ERROR([bad value ${enableval} for --enable-lib-threads])
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE([lib-demux],
     AC_HELP_STRING([--enable-lib-demux],[build ACE_Demux library]),
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_DEMUX
                  ;;
                no)
                  ace_user_enable_lib_demux=no
                  ;;
                *)
                  AC_MSG_ERROR([bad value ${enableval} for --enable-lib-demux])
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE([lib-connection],
     AC_HELP_STRING([--enable-lib-connection],[build ACE_Connection library ]),
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_CONNECTION
                  ;;
                no)
                  ace_user_enable_lib_connection=no
                  ;;
                *)
                  AC_MSG_ERROR([bad value ${enableval} for --enable-lib-connection])
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE([lib-sockets],
     AC_HELP_STRING([--enable-lib-sockets],[build ACE_Sockets library]),
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_SOCKETS
                  ;;
                no)
                  ace_user_enable_lib_sockets=no
                  ;;
                *)
                  AC_MSG_ERROR([bad value ${enableval} for --enable-lib-sockets])
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE([lib-ipc],
     AC_HELP_STRING([--enable-lib-ipc],[build ACE_IPC library]),
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_IPC
                  ;;
                no)
                  ace_user_enable_lib_ipc=no
                  ;;
                *)
                  AC_MSG_ERROR([bad value ${enableval} for --enable-lib-ipc])
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE([lib-svcconf],
     AC_HELP_STRING([--enable-lib-svcconf],[build ACE_Svcconf library]),
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_SVCCONF
                  ;;
                no)
                  ace_user_enable_lib_svcconf=no
                  AC_DEFINE([ACE_LACKS_ACE_SVCCONF])
                  ;;
                *)
                  AC_MSG_ERROR([bad value ${enableval} for --enable-lib-svcconf])
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE([lib-streams],
     AC_HELP_STRING([--enable-lib-streams],[build ACE_Streams library]),
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_STREAMS
                  ;;
                no)
                  ace_user_enable_lib_streams=no
                  ;;
                *)
                  AC_MSG_ERROR([bad value ${enableval} for --enable-lib-streams])
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE([lib-memory],
     AC_HELP_STRING([--enable-lib-memory],[build ACE_Memory library]),
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_MEMORY
                  ;;
                no)
                  ace_user_enable_lib_memory=no
                  ;;
                *)
                  AC_MSG_ERROR([bad value ${enableval} for --enable-lib-memory])
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)


AC_ARG_ENABLE([lib-timer],
     AC_HELP_STRING([--enable-lib-timer],[build ACE_Timer library]),
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_TIMER
                  ;;
                no)
                  ace_user_enable_lib_timer=no
                  ;;
                *)
                  AC_MSG_ERROR([bad value ${enableval} for --enable-lib-timer])
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE([lib-token],
     AC_HELP_STRING([--enable-lib-token],[build ACE_Token library]),
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_TOKEN
                  ;;
                no)
                  ace_user_enable_lib_token=no
                  AC_DEFINE([ACE_LACKS_ACE_TOKEN])
                  ;;
                *)
                  AC_MSG_ERROR([bad value ${enableval} for --enable-lib-token])
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE([lib-other],
     AC_HELP_STRING([--enable-lib-other],[build ACE_Other library]),
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_OTHER
                  ;;
                no)
                  ace_user_enable_lib_other=no
                  AC_DEFINE([ACE_LACKS_ACE_OTHER])
                  ;;
                *)
                  AC_MSG_ERROR([bad value ${enableval} for --enable-lib-other])
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)


dnl If no ACE subsets were explicitly enabled or disabled then build
dnl all of them.
if test $ace_user_enable_lib_all = yes; then

  ACE_CREATE_ALL_COMPONENTS

elif test $ace_user_enable_lib_all = no; then

  ACE_DISABLE_ALL_COMPONENTS

fi

if test $ace_user_enable_lib_full = no &&
   test $ace_user_enable_lib_os = no &&
   test $ace_user_enable_lib_utils = no &&
   test $ace_user_enable_lib_logging = no &&
   test $ace_user_enable_lib_threads = no &&
   test $ace_user_enable_lib_demux = no &&
   test $ace_user_enable_lib_connection = no &&
   test $ace_user_enable_lib_sockets = no &&
   test $ace_user_enable_lib_ipc = no &&
   test $ace_user_enable_lib_svcconf = no &&
   test $ace_user_enable_lib_streams = no &&
   test $ace_user_enable_lib_memory = no &&
   test $ace_user_enable_lib_timer = no &&
   test $ace_user_enable_lib_token = no &&
   test $ace_user_enable_lib_other = no; then

  dnl If we get here then no ACE libraries will be built!
  AC_MSG_ERROR([No ACE components will be built.  Specify which components to build.])

fi  dnl No components will be built!

dnl Set which ACE subsets to build
dnl AM_CONDITIONAL(BUILD_OS_FILES,
dnl                test X$ace_user_enable_lib_os = Xyes)

dnl AM_CONDITIONAL(BUILD_UTILS_FILES,
dnl                test X$ace_user_enable_lib_utils = Xyes)

dnl AM_CONDITIONAL(BUILD_LOGGING_FILES,
dnl                test X$ace_user_enable_lib_logging = Xyes)

dnl AM_CONDITIONAL(BUILD_THREADS_FILES,
dnl                test X$ace_user_enable_lib_threads = Xyes)

dnl AM_CONDITIONAL(BUILD_DEMUX_FILES,
dnl                test X$ace_user_enable_lib_demux = Xyes)

dnl AM_CONDITIONAL(BUILD_CONNECTION_FILES,
dnl                test X$ace_user_enable_lib_connection = Xyes)

dnl AM_CONDITIONAL(BUILD_SOCKETS_FILES,
dnl                test X$ace_user_enable_lib_sockets = Xyes)

dnl AM_CONDITIONAL(BUILD_IPC_FILES,
dnl                test X$ace_user_enable_lib_ipc = Xyes)

dnl AM_CONDITIONAL(BUILD_SVCCONF_FILES,
dnl                test X$ace_user_enable_lib_svcconf = Xyes)

dnl AM_CONDITIONAL(BUILD_STREAMS_FILES,
dnl                test X$ace_user_enable_lib_streams = Xyes)

dnl AM_CONDITIONAL(BUILD_MEMORY_FILES,
dnl                test X$ace_user_enable_lib_memory = Xyes)

dnl AM_CONDITIONAL(BUILD_TIMER_FILES,
dnl                test X$ace_user_enable_lib_timer = Xyes)

dnl AM_CONDITIONAL(BUILD_TOKEN_FILES,
dnl                test X$ace_user_enable_lib_token = Xyes)

dnl AM_CONDITIONAL(BUILD_OTHER_FILES,
dnl                test X$ace_user_enable_lib_other = Xyes)

dnl AM_CONDITIONAL(BUILD_FULL_LIBRARY,
dnl                test X$ace_user_enable_lib_full = Xyes)

dnl End ACE_CHECK_SUBSETS
])

dnl Set the component dependencies for the libACE_OS library
dnl Usage: ACE_CREATE_LIBACE_OS
AC_DEFUN([ACE_CREATE_LIBACE_OS],
[
 ace_user_enable_lib_os=yes
])

dnl Set the component dependencies for the libACE_Utils library
dnl Usage: ACE_CREATE_LIBACE_UTILS
AC_DEFUN([ACE_CREATE_LIBACE_UTILS],
[
 ace_user_enable_lib_utils=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
])

dnl Set the component dependencies for the libACE_Logging library
dnl Usage: ACE_CREATE_LIBACE_Logging
AC_DEFUN([ACE_CREATE_LIBACE_LOGGING],
[
 ace_user_enable_lib_logging=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
])

dnl Set the component dependencies for the libACE_Threads library
dnl Usage: ACE_CREATE_LIBACE_THREADS
AC_DEFUN([ACE_CREATE_LIBACE_THREADS],
[
 ace_user_enable_lib_threads=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
])

dnl Set the component dependencies for the libACE_Demux library
dnl Usage: ACE_CREATE_LIBACE_DEMUX
AC_DEFUN([ACE_CREATE_LIBACE_DEMUX],
[
 ace_user_enable_lib_demux=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
 ACE_CREATE_LIBACE_THREADS
])

dnl Set the component dependencies for the libACE_Connection library
dnl Usage: ACE_CREATE_LIBACE_CONNECTION
AC_DEFUN([ACE_CREATE_LIBACE_CONNECTION],
[
 ace_user_enable_lib_connection=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
 ACE_CREATE_LIBACE_THREADS
 ACE_CREATE_LIBACE_DEMUX
])

dnl Set the component dependencies for the libACE_Sockets library
dnl Usage: ACE_CREATE_LIBACE_SOCKETS
AC_DEFUN([ACE_CREATE_LIBACE_SOCKETS],
[
 ace_user_enable_lib_sockets=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
])

dnl Set the component dependencies for the libACE_IPC library
dnl Usage: ACE_CREATE_LIBACE_IPC
AC_DEFUN([ACE_CREATE_LIBACE_IPC],
[
 ace_user_enable_lib_ipc=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
 ACE_CREATE_LIBACE_SOCKETS
])

dnl Set the component dependencies for the libACE_Svcconf library
dnl Usage: ACE_CREATE_LIBACE_SVCCONF
AC_DEFUN([ACE_CREATE_LIBACE_SVCCONF],
[
 ace_user_enable_lib_svcconf=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
 ACE_CREATE_LIBACE_THREADS
 ACE_CREATE_LIBACE_DEMUX
 ACE_CREATE_LIBACE_SOCKETS
])

dnl Set the component dependencies for the libACE_Streams library
dnl Usage: ACE_CREATE_LIBACE_STREAMS
AC_DEFUN([ACE_CREATE_LIBACE_STREAMS],
[
 ace_user_enable_lib_streams=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
 ACE_CREATE_LIBACE_THREADS
 ACE_CREATE_LIBACE_DEMUX
])

dnl Set the component dependencies for the libACE_Memory library
dnl Usage: ACE_CREATE_LIBACE_MEMORY
AC_DEFUN([ACE_CREATE_LIBACE_MEMORY],
[
 ace_user_enable_lib_memory=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
])

dnl Set the component dependencies for the libACE_Timer library
dnl Usage: ACE_CREATE_LIBACE_TIMER
AC_DEFUN([ACE_CREATE_LIBACE_TIMER],
[
 ace_user_enable_lib_timer=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
])

dnl Set the component dependencies for the libACE_Token library
dnl Usage: ACE_CREATE_LIBACE_TOKEN
AC_DEFUN([ACE_CREATE_LIBACE_TOKEN],
[
 ace_user_enable_lib_token=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
 ACE_CREATE_LIBACE_UTILS
 ACE_CREATE_LIBACE_LOGGING
 ACE_CREATE_LIBACE_THREADS
 ACE_CREATE_LIBACE_DEMUX
 ACE_CREATE_LIBACE_CONNECTION
 ACE_CREATE_LIBACE_SOCKETS
 ACE_CREATE_LIBACE_IPC
 ACE_CREATE_LIBACE_SVCCONF
 ACE_CREATE_LIBACE_STREAMS
 ACE_CREATE_LIBACE_MEMORY
 dnl ACE_CREATE_LIBACE_OTHER
])

dnl Set the component dependencies for the libACE_Utils library
dnl Usage: ACE_CREATE_LIBACE_OTHER
AC_DEFUN([ACE_CREATE_LIBACE_OTHER],
[
 ace_user_enable_lib_other=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
 ACE_CREATE_LIBACE_UTILS
 ACE_CREATE_LIBACE_LOGGING
 ACE_CREATE_LIBACE_THREADS
 ACE_CREATE_LIBACE_DEMUX
 ACE_CREATE_LIBACE_CONNECTION
 ACE_CREATE_LIBACE_SOCKETS
 ACE_CREATE_LIBACE_IPC
 ACE_CREATE_LIBACE_SVCCONF
 ACE_CREATE_LIBACE_STREAMS
 ACE_CREATE_LIBACE_MEMORY
 dnl ACE_CREATE_LIBACE_TOKEN
])

dnl Build all ACE component libraries
dnl Usage: ACE_CREATE_ALL_COMPONENTS
AC_DEFUN([ACE_CREATE_ALL_COMPONENTS],
[
 ace_user_enable_lib_os=yes
 ace_user_enable_lib_utils=yes
 ace_user_enable_lib_logging=yes
 ace_user_enable_lib_threads=yes
 ace_user_enable_lib_demux=yes
 ace_user_enable_lib_connection=yes
 ace_user_enable_lib_sockets=yes
 ace_user_enable_lib_ipc=yes
 ace_user_enable_lib_svcconf=yes
 ace_user_enable_lib_streams=yes
 ace_user_enable_lib_memory=yes
 ace_user_enable_lib_timer=yes
 ace_user_enable_lib_token=yes
 ace_user_enable_lib_other=yes
])

dnl Disable all ACE component libraries
dnl Usage: ACE_CREATE_ALL_COMPONENTS
AC_DEFUN([ACE_DISABLE_ALL_COMPONENTS],
[
 ace_user_enable_lib_os=no
 ace_user_enable_lib_utils=no
 ace_user_enable_lib_logging=no
 ace_user_enable_lib_threads=no
 ace_user_enable_lib_demux=no
 ace_user_enable_lib_connection=no
 ace_user_enable_lib_sockets=no
 ace_user_enable_lib_ipc=no
 ace_user_enable_lib_svcconf=no
 ace_user_enable_lib_streams=no
 ace_user_enable_lib_memory=no
 ace_user_enable_lib_timer=no
 ace_user_enable_lib_token=no
 ace_user_enable_lib_other=no
])
