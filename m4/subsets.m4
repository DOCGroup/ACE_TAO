dnl -------------------------------------------------------------------------
dnl       $Id$
dnl 
dnl       subsets.m4
dnl
dnl       ACE M4 include file which contains ACE specific M4 macros
dnl       that set/determine which ACE subsets to build.
dnl 
dnl -------------------------------------------------------------------------

dnl  Copyright (C) 1998, 1999  Ossama Othman
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
AC_DEFUN(ACE_CHECK_SUBSETS, dnl
[

dnl Begin ACE_CHECK_SUBSETS

dnl Assume all subsets will be built, including the full ACE library.
dnl If any of the components is explicitly enabled or disabled by the user
dnl then do NOT build the full ACE library.
AC_ARG_ENABLE(lib-all,
              [  --enable-lib-all        build all ACE components        [default=no]],
              [
               case "${enableval}" in
                yes)
                  ace_user_enable_lib_all=yes
                  ;;
                no)
                  ace_user_enable_lib_all=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-all)
                  ;;
               esac
              ],
              [
               ace_user_enable_lib_all=no
              ])

AC_ARG_ENABLE(lib-full,
              [  --enable-lib-full       build the full ACE library      [default=yes]],
              [
               case "${enableval}" in
                yes)
                  ace_user_enable_lib_full=yes
                  ;;
                no)
                  ace_user_enable_lib_full=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-full)
                  ;;
               esac
              ],
              [
               ace_user_enable_lib_full=yes
              ])

AC_ARG_ENABLE(lib-os,
              [  --enable-lib-os         build libACE_OS library         ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_OS
                  ;;
                no)
                  ace_user_enable_lib_os=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-os)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-utils,
              [  --enable-lib-utils      build libACE_Utils library      ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_UTILS
                  ;;
                no)
                  ace_user_enable_lib_utils=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-utils)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-logging,
              [  --enable-lib-logging    build libACE_Logging library    ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_LOGGING
                  ;;
                no)
                  ace_user_enable_lib_logging=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-logging)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-threads,
              [  --enable-lib-threads    build libACE_Threads library    ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_THREADS
                  ;;
                no)
                  ace_user_enable_lib_threads=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-threads)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-demux,
              [  --enable-lib-demux      build libACE_Demux library      ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_DEMUX
                  ;;
                no)
                  ace_user_enable_lib_demux=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-demux)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-connection,
              [  --enable-lib-connection build libACE_Connection library ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_CONNECTION
                  ;;
                no)
                  ace_user_enable_lib_connection=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-connection)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-sockets,
              [  --enable-lib-sockets    build libACE_Sockets library    ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_SOCKETS
                  ;;
                no)
                  ace_user_enable_lib_sockets=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-sockets)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-ipc,
              [  --enable-lib-ipc        build libACE_IPC library        ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_IPC
                  ;;
                no)
                  ace_user_enable_lib_ipc=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-ipc)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-svcconf,
              [  --enable-lib-svcconf    build libACE_Svcconf library    ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_SVCCONF
                  ;;
                no)
                  ace_user_enable_lib_svcconf=no
                  AC_DEFINE(ACE_LACKS_ACE_SVCCONF)
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-svcconf)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-streams,
              [  --enable-lib-streams    build libACE_Streams library    ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_STREAMS
                  ;;
                no)
                  ace_user_enable_lib_streams=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-streams)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-memory,
              [  --enable-lib-memory     build libACE_Memory library     ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_MEMORY
                  ;;
                no)
                  ace_user_enable_lib_memory=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-memory)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-token,
              [  --enable-lib-token      build libACE_Token library      ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_TOKEN
                  ;;
                no)
                  ace_user_enable_lib_token=no
                  AC_DEFINE(ACE_LACKS_ACE_TOKEN)
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-token)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-other,
              [  --enable-lib-other      build libACE_Other library      ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_OTHER
                  ;;
                no)
                  ace_user_enable_lib_other=no
                  AC_DEFINE(ACE_LACKS_ACE_OTHER)
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-other)
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
   test $ace_user_enable_lib_token = no &&
   test $ace_user_enable_lib_other = no; then

  dnl If we get here then no ACE libraries will be built!
  AC_MSG_ERROR(no ACE components will be built.  Specify which components to build)

fi  dnl No components will be built!

dnl Set which ACE subsets to build
AM_CONDITIONAL(BUILD_OS_FILES,
               test X$ace_user_enable_lib_os = Xyes)

AM_CONDITIONAL(BUILD_UTILS_FILES,
               test X$ace_user_enable_lib_utils = Xyes)

AM_CONDITIONAL(BUILD_LOGGING_FILES,
               test X$ace_user_enable_lib_logging = Xyes)

AM_CONDITIONAL(BUILD_THREADS_FILES,
               test X$ace_user_enable_lib_threads = Xyes)

AM_CONDITIONAL(BUILD_DEMUX_FILES,
               test X$ace_user_enable_lib_demux = Xyes)

AM_CONDITIONAL(BUILD_CONNECTION_FILES,
               test X$ace_user_enable_lib_connection = Xyes)

AM_CONDITIONAL(BUILD_SOCKETS_FILES,
               test X$ace_user_enable_lib_sockets = Xyes)

AM_CONDITIONAL(BUILD_IPC_FILES,
               test X$ace_user_enable_lib_ipc = Xyes)

AM_CONDITIONAL(BUILD_SVCCONF_FILES,
               test X$ace_user_enable_lib_svcconf = Xyes)

AM_CONDITIONAL(BUILD_STREAMS_FILES,
               test X$ace_user_enable_lib_streams = Xyes)

AM_CONDITIONAL(BUILD_MEMORY_FILES,
               test X$ace_user_enable_lib_memory = Xyes)

AM_CONDITIONAL(BUILD_TIMER_FILES,
		test X$ace_user_enable_lib_timer = Xyes)

AM_CONDITIONAL(BUILD_TOKEN_FILES,
               test X$ace_user_enable_lib_token = Xyes)

AM_CONDITIONAL(BUILD_OTHER_FILES,
               test X$ace_user_enable_lib_other = Xyes)

AM_CONDITIONAL(BUILD_FULL_LIBRARY,
               test X$ace_user_enable_lib_full = Xyes)

dnl End ACE_CHECK_SUBSETS
])

dnl Set the component dependencies for the libACE_OS library
dnl Usage: ACE_CREATE_LIBACE_OS
AC_DEFUN(ACE_CREATE_LIBACE_OS,
[
 ace_user_enable_lib_os=yes
])

dnl Set the component dependencies for the libACE_Utils library
dnl Usage: ACE_CREATE_LIBACE_UTILS
AC_DEFUN(ACE_CREATE_LIBACE_UTILS,
[
 ace_user_enable_lib_utils=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
])

dnl Set the component dependencies for the libACE_Logging library
dnl Usage: ACE_CREATE_LIBACE_Logging
AC_DEFUN(ACE_CREATE_LIBACE_LOGGING,
[
 ace_user_enable_lib_logging=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
])

dnl Set the component dependencies for the libACE_Threads library
dnl Usage: ACE_CREATE_LIBACE_THREADS
AC_DEFUN(ACE_CREATE_LIBACE_THREADS,
[
 ace_user_enable_lib_threads=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
])

dnl Set the component dependencies for the libACE_Demux library
dnl Usage: ACE_CREATE_LIBACE_DEMUX
AC_DEFUN(ACE_CREATE_LIBACE_DEMUX,
[
 ace_user_enable_lib_demux=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
 ACE_CREATE_LIBACE_THREADS
])

dnl Set the component dependencies for the libACE_Connection library
dnl Usage: ACE_CREATE_LIBACE_CONNECTION
AC_DEFUN(ACE_CREATE_LIBACE_CONNECTION,
[
 ace_user_enable_lib_connection=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
 ACE_CREATE_LIBACE_THREADS
 ACE_CREATE_LIBACE_DEMUX
])

dnl Set the component dependencies for the libACE_Sockets library
dnl Usage: ACE_CREATE_LIBACE_SOCKETS
AC_DEFUN(ACE_CREATE_LIBACE_SOCKETS,
[
 ace_user_enable_lib_sockets=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
])

dnl Set the component dependencies for the libACE_IPC library
dnl Usage: ACE_CREATE_LIBACE_IPC
AC_DEFUN(ACE_CREATE_LIBACE_IPC,
[
 ace_user_enable_lib_ipc=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
 ACE_CREATE_LIBACE_SOCKETS
])

dnl Set the component dependencies for the libACE_Svcconf library
dnl Usage: ACE_CREATE_LIBACE_SVCCONF
AC_DEFUN(ACE_CREATE_LIBACE_SVCCONF,
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
AC_DEFUN(ACE_CREATE_LIBACE_STREAMS,
[
 ace_user_enable_lib_streams=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
 ACE_CREATE_LIBACE_THREADS
 ACE_CREATE_LIBACE_DEMUX
])

dnl Set the component dependencies for the libACE_Memory library
dnl Usage: ACE_CREATE_LIBACE_MEMORY
AC_DEFUN(ACE_CREATE_LIBACE_MEMORY,
[
 ace_user_enable_lib_memory=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
])

dnl Set the component dependencies for the libACE_Token library
dnl Usage: ACE_CREATE_LIBACE_TOKEN
AC_DEFUN(ACE_CREATE_LIBACE_TOKEN,
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
AC_DEFUN(ACE_CREATE_LIBACE_OTHER,
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
AC_DEFUN(ACE_CREATE_ALL_COMPONENTS,
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
 ace_user_enable_lib_token=yes
 ace_user_enable_lib_other=yes
])

dnl Disable all ACE component libraries
dnl Usage: ACE_CREATE_ALL_COMPONENTS
AC_DEFUN(ACE_DISABLE_ALL_COMPONENTS,
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
 ace_user_enable_lib_token=no
 ace_user_enable_lib_other=no
])
