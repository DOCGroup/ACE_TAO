#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for the pace/pace C library release
#----------------------------------------------------------------------------

ifndef PACE_ROOT
  PACE_ROOT = $(ACE_ROOT)/pace
endif # ! PACE_ROOT

ifndef static_libs_only
  ifndef shared_libs_only
    static_libs_only = 1
  endif # ! shared_libs_only
endif # ! static_libs_only

# If we are inlining the PACE functions then we need to define
# PACE_HAS_INLINE. Inlining is the default.
ifndef inline
  CFLAGS += -DPACE_HAS_INLINE
endif # ! inline

ifeq (1,$(inline))
  CFLAGS += -DPACE_HAS_INLINE
endif # inline

MAKEFILE = Makefile
LIB      = libPACE.a
SHLIB    = libPACE.$(SOEXT)

ifeq ($(PACE_SYSNAME),)
  PACE_SYSNAME = $(shell uname -s)
endif # ! PACE_SYSNAME

VPATH:=common:arpa:config:netinet:sys

# List of platforms and uname -s values
# =====================================
# Solaris           SunOS
# NT                "NT" (uname not supported)
# LynxOS            LynxOS
# VxWorks           "VxWorks" (strictly cross-compiled)
# Linux             Linux
# pSOS              "pSOS" (strictly cross-compiled)
# Chorus            "Chorus" (strictly cross-compiled)
# QNX Neutrino      "Neutrino" (strictly cross-compiled)
# OSF/1             OSF1
# IRIX (64 bit)     IRIX64
# IRIX (32 bit)     IRIX (unconfirmed)
# HPUX              HP-UX

# Solaris is POSIX - change this when the platform directories get
# squared away
#ifeq ($(PACE_SYSNAME),SunOS || $(PACE_SYSNAME),Linux)

ifeq ($(PACE_SYSNAME),SunOS)
VPATH :=.:posix:$(VPATH)
CFLAGS += -DPACE_SUNOS
endif

ifeq ($(PACE_SYSNAME),NT)
VPATH :=.:win32:$(VPATH)
CFLAGS += -DPACE_WIN32
endif

ifeq ($(PACE_SYSNAME),LynxOS)
VPATH :=.:posix:$(VPATH)
CFLAGS += -DPACE_LYNXOS
endif

ifeq ($(PACE_SYSNAME),VxWorks)
VPATH :=.:vxworks:$(VPATH)
CFLAGS += -DPACE_VXWORKS
endif

ifeq ($(PACE_SYSNAME),Linux)
VPATH :=.:posix:$(VPATH)
CFLAGS += -DPACE_LINUX
endif

PACE_FILES = \
  aio \
  assert \
  ctype \
  dirent \
  fcntl \
  grp \
  locale \
  math \
  mqueue \
  pthread \
  pwd \
  sched \
  semaphore \
  setjmp \
  signal \
  stdio \
  stdlib \
  string \
  time \
  unistd \
  utime \
  wait
CONFIG_FILES =
SYS_FILES = \
  sys/mman \
  sys/stat \
  sys/termios \
  sys/times \
  sys/utsname
# POSIX_SINGLE_PROCESS Unit of Functionality (POSIX.1)
# pace_sysconf is in unistd.h;
# pace_time is in time.h;
# pace_uname is in sys/ustname.h
POSIX_SP_FILES = \
  unistd \
  time \
  sys/utsname
# POSIX_MULTI_PROCESS Unit of Functionality (POSIX.1)
# pace__exit is in unistd.h
# pace_assert is in assert.h
# pace_execl, pace_execle, pace_execlp, pace_execv, pace_execve, pace_execvp
#  are in unistd.h;
# pace_exit is in stdlib.h;
# pace_fork is in unistd.h;
# pace_getenv is in stdlib.h;
# pace_getpid, pace_getppid are in unistd.h;
# pace_setlocale is in locale.h;
# pace_sleep is in unistd.h;
# pace_times is in sys/times.h;
# pace_wait, pace_waitpid are in wait.h;
POSIX_MP_FILES = \
  unistd \
  assert \
  stdlib \
  locale \
  sys/times \
  wait
# POSIX_SIGNALS Unit of Functionality (POSIX.1)
# pace_abort is in stdlib.h;
# pace_alarm is in unistd.h;
# pace_kill is in signal.h;
# pace_pause is in unistd.h;
# pace_sigaction, pace_sigaddset, pace_sigdelset, pace_sigemptyset,
#  pace_sigfillset, pace_sigismember are in signal.h;
# pace_siglongjmp is in setjmp.h;
# pace_sigpending, pace_sigprocmask are in signal.h;
# pace_sigsetjmp is in setjmp.h;
# pace_sigsuspend is in signal.h;
POSIX_SIG_FILES = \
  stdlib \
  unistd \
  signal \
  setjmp
# POSIX_USER_GROUPS Unit of Functionality (POSIX.1)
# pace_getegid, pace_geteuid, pace_getgid, pace_getgroups, pace_getlogin,
#  pace_getpgrp, pace_getuid, pace_setgid, pace_setsid, pace_setuid are in
#  unistd.h;
POSIX_UG_FILES = \
  unistd
# POSIX_FILE_SYSTEM Unit of Functionality (POSIX.1)
# pace_access, pace_chdir are in unistd.h;
# pace_closedir is in dirent.h;
# pace_creat is in fcntl.h;
# pace_fpathconf is in unistd.h;
# pace_fstat is in sys/stat.h
# pace_getcwd, pace_link are in unistd.h
# pace_mkdir is in sys/stat.h
# pace_opendir is in dirent.h;
# pace_pathconf is in unistd.h;
# pace_readdir is in dirent.h;
# pace_remove, pace_rename are in stdio.h;
# pace_rewinddir is in dirent.h;
# pace_rmdir is in unistd.h;
# pace_stat is in sys/stat.h;
# pace_tmpfile, pace_tmpnam are in stdio.h;
# pace_unlink is in unistd.h;
# pace_utime is in utime.h;
POSIX_FS_FILES = \
  unistd \
  dirent \
  fcntl \
  sys/stat \
  stdio \
  utime
# POSIX_FILE_ATTRIBUTES Unit of Functionality (POSIX.1)
# pace_chmod is in sys/stat.h;
# pace_chown is in unistd.h;
# pace_umask is in sys/stat.h;
POSIX_FA_FILES = \
  sys/stat \
  unistd
# POSIX_FIFO Unit of Functionality (POSIX.1)
# pace_mkfifo is in sys/stat.h;
POSIX_F_FILES = \
  sys/stat
# POSIX_DEVICE_IO Unit of Functionality (POSIX.1)
# pace_clearerr is in stdio.h;
# pace_close is in unistd.h;
# pace_fclose, pace_fdopen, pace_feof, pace_ferror are in stdio.h;
# pace_fflush, pace_fgetc, pace_fgets, pace_fileno are in stdio.h;
# pace_fopen, pace_fprintf, pace_fputc, pace_fputs, pace_fread, pace_freopen,
#  pace_fscanf, pace_fwrite, pace_getc, pace_getchar, pace_gets are in stdio.h;
# pace_open is in fcntl.h;
# pace_perror, pace_printf, pace_putc, pace_putchar, pace_puts are in stdio.h;
# pace_read is in unistd.h;
# pace_scanf is in stdio.h;
# pace_setbuf, pace_sprintf, pace_sscanf, pace_ungetc are in stdio.h;
# pace_write are in unistd.h;
POSIX_DI_FILES = \
  stdio \
  unistd \
  fcntl
# POSIX_FD_MGMT Unit of Functionality (POSIX.1)
# pace_dup, pace_dup2 are in unistd.h;
# pace_fcntl is in fcntl.h;
# pace_fseek, pace_ftell are in stdio.h;
# pace_lseek in unistd.h;
# pace_rewind is in stdio.h;
POSIX_FM_FILES = \
  unistd \
  fcntl \
  stdio
# POSIX_PIPE Unit of Functionality (POSIX.1)
# pace_pipe is in unistd.h;
POSIX_P_FILES = \
  unistd
# POSIX_DEVICE_SPECIFIC Unit of Functionality (POSIX.1)
# pace_cfgetispeed, pace_cfgetospeed, pace_cfsetispeed, pace_cfsetospeed are
#  in sys/termios.h;
# pace_ctermid is in stdio.h;
# pace_isatty is in unistd.h;
# pace_tcdrain, pace_tcflow, pace_tcflush, pace_tcgetattr, pace_tcsendbreak,
#  pace_tcsetattr are in sys/termios.h;
# pace_ttyname is in unistd.h;
POSIX_DS_FILES = \
  sys/termios \
  stdio \
  unistd
# POSIX_SYSTEM_DATABASE Unit of Functionality (POSIX.1)
# pace_getgrgid, pace_getgrnam, pace_getpwnam, pace_getpwuid are in grp.h;
POSIX_SD_FILES = \
  grp
# POSIX_C_LANG_SUPPORT Unit of Functionality (POSIX.1)
# pace_isalnum is in ??.h;
POSIX_CLS_FILES = \
  ??
# _POSIX_JOB_CONTROL Unit of Functionality (POSIX.1)
# pace_setpgid is in unistd.h;
# pace_tcgetpgrp, pace_tcsetpgrp are in sys/termios.h
_POSIX_JC_FILES = \
  unistd \
  sys/termios

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

INCLDIRS += -I$(PACE_ROOT)
#INCLDIRS = -I$(PACE_ROOT)

####
#### Build customization.
####
ifndef PACE_COMPONENTS
  #### Do we want PACE subsets as below? If so, we need to add the
  #### documentation referenced below as well.
  #### Please see docs/PACE-subsets.html for documentation. ??
  PACE_COMPONENTS = \
    Pace \
    Config \
    Sys
endif # PACE_COMPONENTS

ifeq (FOR_ACE,$(PACE_COMPONENTS))
  #### These PACE components are necessary to support ACE.
  override PACE_COMPONENTS = \
    Pace \
    Config \
    Sys
else  # ! FOR_ACE
  #### Is there anything in PACE that ACE doesn't need? Probably not
endif # ! FOR_ACE

####
#### PACE_COMPONENTS support.
####
ifneq (,$(findstring Pace,$(PACE_COMPONENTS)))
  FILES += $(PACE_FILES)
endif # Pace

ifneq (,$(findstring Config,$(PACE_COMPONENTS)))
  FILES += $(CONFIG_FILES)
endif # Config

ifneq (,$(findstring Sys,$(PACE_COMPONENTS)))
  FILES += $(SYS_FILES)
endif # Sys

ifneq (,$(findstring Posix_sp,$(PACE_COMPONENTS)))
  FILES = $(POSIX_SP_FILES)
  LIB   = libPACE_SP.a
  SHLIB = libPACE_SP.$(SOEXT)
endif # Single Process

ifneq (,$(findstring Posix_mp,$(PACE_COMPONENTS)))
  FILES = $(POSIX_MP_FILES)
  LIB   = libPACE_MP.a
  SHLIB = libPACE_MP.$(SOEXT)
endif # Multi Process

ifneq (,$(findstring Posix_sig,$(PACE_COMPONENTS)))
  FILES = $(POSIX_SIG_FILES)
  LIB   = libPACE_S.a
  SHLIB = libPACE_S.$(SOEXT)
endif # Signals

ifneq (,$(findstring Posix_ug,$(PACE_COMPONENTS)))
  FILES = $(POSIX_UG_FILES)
  LIB   = libPACE_UG.a
  SHLIB = libPACE_UG.$(SOEXT)
endif # User Groups

ifneq (,$(findstring Posix_fs,$(PACE_COMPONENTS)))
  FILES = $(POSIX_FS_FILES)
  LIB   = libPACE_FS.a
  SHLIB = libPACE_FS.$(SOEXT)
endif # File System

ifneq (,$(findstring Posix_fa,$(PACE_COMPONENTS)))
  FILES = $(POSIX_FA_FILES)
  LIB   = libPACE_FA.a
  SHLIB = libPACE_FA.$(SOEXT)
endif # File Attributes

ifneq (,$(findstring Posix_fif,$(PACE_COMPONENTS)))
  FILES = $(POSIX_F_FILES)
  LIB   = libPACE_F.a
  SHLIB = libPACE_F.$(SOEXT)
endif # Fifo

ifneq (,$(findstring Posix_di,$(PACE_COMPONENTS)))
  FILES = $(POSIX_DI_FILES)
  LIB   = libPACE_DI.a
  SHLIB = libPACE_DI.$(SOEXT)
endif # Device Input/Output

ifneq (,$(findstring Posix_fm,$(PACE_COMPONENTS)))
  FILES = $(POSIX_FM_FILES)
  LIB   = libPACE_FM.a
  SHLIB = libPACE_FM.$(SOEXT)
endif # File Descriptor Management

ifneq (,$(findstring Posix_p,$(PACE_COMPONENTS)))
  FILES = $(POSIX_P_FILES)
  LIB   = libPACE_P.a
  SHLIB = libPACE_P.$(SOEXT)
endif # Pipe

ifneq (,$(findstring Posix_ds,$(PACE_COMPONENTS)))
  FILES = $(POSIX_DS_FILES)
  LIB   = libPACE_DS.a
  SHLIB = libPACE_DS.$(SOEXT)
endif # Device Specific

ifneq (,$(findstring Posix_sd,$(PACE_COMPONENTS)))
  FILES = $(POSIX_SD_FILES)
  LIB   = libPACE_SD.a
  SHLIB = libPACE_SD.$(SOEXT)
endif # System Database

ifneq (,$(findstring Posix_cls,$(PACE_COMPONENTS)))
  FILES = $(POSIX_CLS_FILES)
  LIB   = libPACE_CLS.a
  SHLIB = libPACE_CLS.$(SOEXT)
endif # C Language Support

ifneq (,$(findstring Posix_jc,$(PACE_COMPONENTS)))
  FILES = $(_POSIX_JC_FILES)
  LIB   = libPACE_JC.a
  SHLIB = libPACE_JC.$(SOEXT)
endif # Job Control

LSRC  = $(addsuffix .cpp,$(FILES))

BUILD += PACE_COMPONENTS

#### Setting PACELIB to null allows it to be used for building shared
#### libraries, including libPACE.$(SOEXT,) on certain platforms.
PACELIB =

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
#### Disable installs in this ($(ACE_ROOT)/pace/pace) directory, because this
#### is the (default) destination of installs anyways.  This line prevents
#### creation of a link from the PACE library to itself when the build of
#### the PACE library fails.
INSTALL =
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

.PHONY: PACE_COMPONENTS
PACE_COMPONENTS:
	@sh $(ACE_ROOT)/bin/pace_components --pace --set ' $(PACE_COMPONENTS) '

realclean:
	@sh $(ACE_ROOT)/bin/pace_components --pace --remove

# AIX template compilation speedup hack
# Not added for PACE - can always be added later if needed

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
