#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for the PACE/pace C library release
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIB      = libPACE.a
SHLIB    = libPACE.$(SOEXT)

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
  termios \
  time \
  unistd \
  utime
CONFIG_FILES =
SYS_FILES = \
  sys/mman \
  sys/socket \
  sys/stat \
  sys/times \
  sys/utsname \
  sys/wait

# POSIX_SOCKET Unit of Functionality
# pace_accept      is in sys/socket.h
# pace_bind        is in sys/socket.h
# pace_connect     is in sys/socket.h
# pace_getpeername is in sys/socket.h
# pace_getsockname is in sys/socket.h
# pace_getsockopt  is in sys/socket.h
# pace_setsockopt  is in sys/socket.h
# pace_isfdtype    is in sys/socket.h
# pace_listen      is in sys/socket.h
# pace_recv        is in sys/socket.h
# pace_recvfrom    is in sys/socket.h
# pace_rcvmsg      is in sys/socket.h
# pace_send        is in sys/socket.h
# pace_sendto      is in sys/socket.h
# pace_sendmsg     is in sys/socket.h
# pace_shutdown    is in sys/socket.h
# pace_socket      is in sys/socket.h
# pace_socketpair  is in sys/socket.h
POSIX_SOCK_FILES = \
  sys/socket	

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
  sys/wait

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
#  in termios.h;
# pace_ctermid is in stdio.h;
# pace_isatty is in unistd.h;
# pace_tcdrain, pace_tcflow, pace_tcflush, pace_tcgetattr, pace_tcsendbreak,
#  pace_tcsetattr are in termios.h;
# pace_ttyname is in unistd.h;
POSIX_DS_FILES = \
  termios \
  stdio \
  unistd

# POSIX_SYSTEM_DATABASE Unit of Functionality (POSIX.1)
# pace_getgrgid, pace_getgrnam, pace_getpwnam, pace_getpwuid are in grp.h;
POSIX_SD_FILES = \
  grp

# POSIX_C_LANG_SUPPORT Unit of Functionality (POSIX.1)
# Character handling functions (e.g., isalnum, toupper) are in ctype.h;
# Mathematics functions (e.g., acos, tanh) are in math.h;
# Non-local jump functions (i.e., longjmp, setjmp) are in setjmp.h;
# General utility functions (i.e., abs, srand) are in stdlib.h;
# String handling functions (i.e., strcat, strtok) are in string.h;
# Date and Time functions (i.e., asctime, tzset) are in time.h;
POSIX_CLS_FILES = \
  ctype \
  math \
  setjmp \
  stdlib \
  time

# _POSIX_JOB_CONTROL Unit of Functionality (POSIX.1)
# pace_setpgid is in unistd.h;
# pace_tcgetpgrp, pace_tcsetpgrp are in termios.h
_POSIX_JC_FILES = \
  unistd \
  termios

# POSIX_USER_GROUPS_R Unit of Functionality (POSIX.1)
# pace_getlogin_r is in unistd.h;
POSIX_UGR_FILES = \
  unistd

# POSIX_FILE_LOCKING Unit of Functionality (POSIX.1)
# pace_flockfile, pace_ftrylockfile, pace_funlockfile, pace_getc_unlocked,
# pace_getchar_unlocked, pace_putc_unlocked, pace_putchar_unlocked are in
# stdio.h;
POSIX_FL_FILES = \
  stdio

# POSIX_C_LANG_SUPPORT_R Unit of Functionality (POSIX.1)
# pace_asctime_r, pace_ctime_r, pace_gmtime_r, pace_localtime_r are in time.h;
# pace_rand_r is in stdlib.h;
# pace_strtok_r is in string.h;
# pace_readdir_r is in dirent.h;
POSIX_CLSR_FILES = \
  time \
  stdlib \
  string \
  dirent

# POSIX_SYSTEM_DATABASE_R Unit of Functionality (POSIX.1)
# pace_getgrgid_r, pace_getgrnam_r are in grp.h;
# pace_getpwnam_r, pace_getpwuid_r are in pwd.h;
POSIX_SDR_FILES = \
  grp \
  pwd

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

INCLDIRS += -I$(PACE_ROOT)

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
  CFLAGS += -DPACE_HAS_ALL_POSIX_FUNCS
endif # PACE_COMPONENTS

ifeq (FOR_ACE,$(PACE_COMPONENTS))
  #### These PACE components are necessary to support ACE.
  override PACE_COMPONENTS = \
    Pace \
    Config \
    Sys
  CFLAGS += -DPACE_HAS_ALL_POSIX_FUNCS
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
  CFLAGS += -DPACE_HAS_POSIX_SP_UOF
endif # Single Process

ifneq (,$(findstring Posix_mp,$(PACE_COMPONENTS)))
  FILES = $(POSIX_MP_FILES)
  LIB   = libPACE_MP.a
  SHLIB = libPACE_MP.$(SOEXT)
  CFLAGS += -DPACE_HAS_POSIX_MP_UOF
endif # Multi Process

ifneq (,$(findstring Posix_sig,$(PACE_COMPONENTS)))
  FILES = $(POSIX_SIG_FILES)
  LIB   = libPACE_S.a
  SHLIB = libPACE_S.$(SOEXT)
  CFLAGS += -DPACE_HAS_POSIX_SIG_UOF
endif # Signals

ifneq (,$(findstring Posix_usg,$(PACE_COMPONENTS)))
  FILES = $(POSIX_UG_FILES)
  LIB   = libPACE_UG.a
  SHLIB = libPACE_UG.$(SOEXT)
  CFLAGS += -DPACE_HAS_POSIX_UG_UOF
endif # User Groups

ifneq (,$(findstring Posix_fs,$(PACE_COMPONENTS)))
  FILES = $(POSIX_FS_FILES)
  LIB   = libPACE_FS.a
  SHLIB = libPACE_FS.$(SOEXT)
  CFLAGS += -DPACE_HAS_POSIX_FS_UOF
endif # File System

ifneq (,$(findstring Posix_fa,$(PACE_COMPONENTS)))
  FILES = $(POSIX_FA_FILES)
  LIB   = libPACE_FA.a
  SHLIB = libPACE_FA.$(SOEXT)
  CFLAGS += -DPACE_HAS_POSIX_FA_UOF
endif # File Attributes

ifneq (,$(findstring Posix_fif,$(PACE_COMPONENTS)))
  FILES = $(POSIX_F_FILES)
  LIB   = libPACE_F.a
  SHLIB = libPACE_F.$(SOEXT)
  CFLAGS += -DPACE_HAS_POSIX_F_UOF
endif # Fifo

ifneq (,$(findstring Posix_di,$(PACE_COMPONENTS)))
  FILES = $(POSIX_DI_FILES)
  LIB   = libPACE_DI.a
  SHLIB = libPACE_DI.$(SOEXT)
  CFLAGS += -DPACE_HAS_POSIX_DI_UOF
endif # Device Input/Output

ifneq (,$(findstring Posix_fm,$(PACE_COMPONENTS)))
  FILES = $(POSIX_FM_FILES)
  LIB   = libPACE_FM.a
  SHLIB = libPACE_FM.$(SOEXT)
  CFLAGS += -DPACE_HAS_POSIX_FM_UOF
endif # File Descriptor Management

ifneq (,$(findstring Posix_p,$(PACE_COMPONENTS)))
  FILES = $(POSIX_P_FILES)
  LIB   = libPACE_P.a
  SHLIB = libPACE_P.$(SOEXT)
  CFLAGS += -DPACE_HAS_POSIX_P_UOF
endif # Pipe

ifneq (,$(findstring Posix_ds,$(PACE_COMPONENTS)))
  FILES = $(POSIX_DS_FILES)
  LIB   = libPACE_DS.a
  SHLIB = libPACE_DS.$(SOEXT)
  CFLAGS += -DPACE_HAS_POSIX_DS_UOF
endif # Device Specific

ifneq (,$(findstring Posix_sd,$(PACE_COMPONENTS)))
  FILES = $(POSIX_SD_FILES)
  LIB   = libPACE_SD.a
  SHLIB = libPACE_SD.$(SOEXT)
  CFLAGS += -DPACE_HAS_POSIX_SD_UOF
endif # System Database

ifneq (,$(findstring Posix_cls,$(PACE_COMPONENTS)))
  FILES = $(POSIX_CLS_FILES)
  LIB   = libPACE_CLS.a
  SHLIB = libPACE_CLS.$(SOEXT)
  CFLAGS += -DPACE_HAS_POSIX_CLS_UOF
endif # C Language Support

ifneq (,$(findstring Posix_jc,$(PACE_COMPONENTS)))
  FILES = $(_POSIX_JC_FILES)
  LIB   = libPACE_JC.a
  SHLIB = libPACE_JC.$(SOEXT)
  CFLAGS += -DPACE_HAS_POSIX_JC_UOF
endif # Job Control

ifneq (,$(findstring Posix_ugr,$(PACE_COMPONENTS)))
  FILES = $(POSIX_UGR_FILES)
  LIB   = libPACE_UGR.a
  SHLIB = libPACE_UGR.$(SOEXT)
  CFLAGS += -DPACE_HAS_POSIX_UGR_UOF
endif # User Groups - Reentrant

ifneq (,$(findstring Posix_fl,$(PACE_COMPONENTS)))
  FILES = $(POSIX_FL_FILES)
  LIB   = libPACE_FL.a
  SHLIB = libPACE_FL.$(SOEXT)
  CFLAGS += -DPACE_HAS_POSIX_FL_UOF
endif # File Locking

ifneq (,$(findstring Posix_clr,$(PACE_COMPONENTS)))
  FILES = $(POSIX_CLSR_FILES)
  LIB   = libPACE_CLSR.a
  SHLIB = libPACE_CLSR.$(SOEXT)
  CFLAGS += -DPACE_HAS_POSIX_CLSR_UOF
endif # C Language Support - Reentrant

ifneq (,$(findstring Posix_sr,$(PACE_COMPONENTS)))
  FILES = $(POSIX_SDR_FILES)
  LIB   = libPACE_SDR.a
  SHLIB = libPACE_SDR.$(SOEXT)
  CFLAGS += -DPACE_HAS_POSIX_SDR_UOF
endif # System Database - Reentrant

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

ifndef PACE_ROOT
  PACE_ROOT = $(ACE_ROOT)/PACE
endif # ! PACE_ROOT

include $(PACE_ROOT)/include/makeinclude/rules.common.GNU

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

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

.PHONY: PACE_COMPONENTS
PACE_COMPONENTS:
	@sh $(ACE_ROOT)/bin/ace_components --pace --set ' $(PACE_COMPONENTS) '

realclean:
	@sh $(ACE_ROOT)/bin/ace_components --pace --remove

# AIX template compilation speedup hack
# Not added for PACE - can always be added later if needed

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
