dnl -------------------------------------------------------------------------
dnl       $Id$
dnl
dnl       config_h.m4
dnl
dnl       ACE M4 include file which contains preprocessor constants
dnl       and other items to be place in the generated ace/config.h
dnl       header.
dnl
dnl -------------------------------------------------------------------------

dnl  Copyright (C) 2002, 2003  Ossama Othman
dnl
dnl  All Rights Reserved
dnl
dnl This library is free software; you can redistribute it and/or
dnl modify it under the current ACE distribution terms.
dnl
dnl This library is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

dnl Prepare the ace/config.h.in template.
dnl
dnl Usage: ACE_PREP_CONFIG_HEADER
AC_DEFUN([ACE_PREP_CONFIG_HEADER],
[
dnl Text to be placed at the top of the `ace/config.h' header.
AH_TOP([
// -*- C++ -*-

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H

// ACE configuration header file


])

dnl Text to be placed at the bottom of the `ace/config.h' header.
AH_BOTTOM([


#endif  /* ACE_CONFIG_H */
])


dnl ACE currently doesn't use these; however the configure script does
AH_TEMPLATE([ACE_LACKS_SYSTIMES_H],[The `sys/times.h' header is unavailable])
AH_TEMPLATE([ACE_LACKS_UNBUFFERED_STREAMBUF],[])
AH_TEMPLATE([ACE_HAS_STDCPP_STL_INCLUDES],[])

AH_TEMPLATE([ACE_HAS_LIBC_H],[])
AH_TEMPLATE([ACE_HAS_OSFCN_H],[])
AH_TEMPLATE([ACE_HAS_NEW_NO_H],[Platform provides new style C++ <new> header])
AH_TEMPLATE([ACE_HAS_NEW_H],[Platform provides C++ <new.h> header])
AH_TEMPLATE([ACE_HAS_STDEXCEPT_NO_H],[Platform provides C++ <stdexcept> header])
AH_TEMPLATE([ACE_HAS_EXCEPTION_H],[Platform provides C++ <exception.h> header])

AH_TEMPLATE([ACE_HAS_SYS_IOCTL_H],[Platform provides <sys/ioctl.h> header.])

dnl Deprecated! (or soon to be?)
AH_TEMPLATE([ACE_HAS_OSF1_GETTIMEOFDAY],
[timezone* 2nd parameter & no prototype])
AH_TEMPLATE([ACE_HAS_LYNXOS_SIGNALS],[])
AH_TEMPLATE([ACE_HAS_TANDEM_SIGNALS],[])
AH_TEMPLATE([ACE_HAS_IRIX_53_SIGNALS],[])

AH_TEMPLATE([PTHREAD_STACK_MIN],[])
AH_TEMPLATE([PTHREAD_STACK_MAX],[])

dnl ///////////////////// OSSAMA'S NEW STUFF ////////////////// */
dnl THIS STUFF WILL REPLACE THE ABOVE OLDER STUFF AND/OR WILL BE MERGED INTO IT


dnl /* results from checks for programs */
dnl /* results from checks for libraries */
dnl /* results from checks for header files */
dnl /* results from checks for typedefs */
dnl /* results from checks for structures */
dnl /* results from checks for variables */
dnl /* results from checks for compiler characteristics */
dnl /* results from checks for library functions */
dnl /* results from checks for system services */

dnl AIX specific configuration parameters
AH_TEMPLATE([AIX],[Configure for use on AIX])

dnl DG/UX specific configuration parameters
AH_TEMPLATE([ACE_DGUX],[])

dnl FreeBSD specific configuration parameters
dnl Nothing yet

dnl HP/UX specific configuration parameters
AH_TEMPLATE([HPUX],[Configure for use on HP-UX])
AH_TEMPLATE([HPUX_10],[Configure for use on HP-UX 10])
AH_TEMPLATE([HPUX_11],[Configure for use on HP-UX 11])
AH_TEMPLATE([ACE_HAS_BROKEN_HPUX_TEMPLATES],
[Earlier versions of HP/UX C++ are damned...])

dnl Irix specific configuration parameters
AH_TEMPLATE([IRIX5],[Configure for use on Irix 5])
AH_TEMPLATE([IRIX6],[Configure for use on Irix 6])
AH_TEMPLATE([ACE_HAS_IRIX62_THREADS],
[Platform supports the very odd IRIX 6.2 threads...])

dnl Linux specific configuration parameters
dnl Nothing yet */

dnl LynxOS specific configuration parameters
AH_TEMPLATE([__NO_INCLUDE_WARN__],[])

dnl M88K specific configuration parameters
AH_TEMPLATE([m88k],[])
AH_TEMPLATE([__m88k__],[])

dnl MVS specific configuration parameters
dnl Nothing yet

dnl NetBSD specific configuration parameters
AH_TEMPLATE([ACE_NETBSD],[Configure for use on NetBSD])

dnl OSF/1 and Digital Unix specific configuration parameters
AH_TEMPLATE([DEC_CXX],[])
AH_TEMPLATE([DIGITAL_UNIX],[Configure for use on Digital Unix])

dnl pSOS specific configuration parameters
AH_TEMPLATE([ACE_PSOS],[Configure for use on pSoS])
AH_TEMPLATE([ACE_PSOSIM],[])
AH_TEMPLATE([ACE_PSOSTBD],[])

dnl SCO specific configuration parameters
AH_TEMPLATE([SCO],[])

dnl Tandem specific configuration parameters
dnl Nothing yet

dnl UnixWare specific configuration parameters
AH_TEMPLATE([UNIXWARE],[Configure for use on UnixWare])
AH_TEMPLATE([UNIXWARE_2_0],[])
AH_TEMPLATE([UNIXWARE_2_1],[])
AH_TEMPLATE([UNIXWARE_7_1],[])
AH_TEMPLATE([__IOCTL_VERSIONED__],[])

dnl VXWorks specific configuration parameters
AH_TEMPLATE([VXWORKS],[Configure for use on VxWorks])

dnl Win32 specific configuration parameters
AH_TEMPLATE([ACE_WIN32],[Configure for use on Win32])

AH_TEMPLATE([ACE_DISABLE_DEBUG_DLL_CHECK],
[Define this if you don't want debug version ACE search for debug version
DLLs first before looking for the DLL names specified.])

AH_TEMPLATE([ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS],
[Platform/compiler supports Win32 structural exceptions.])

AH_TEMPLATE([ACE_WSOCK_VERSION],
[A parameter list indicating the version of WinSock (e.g., "1, 1" is
version 1.1).])

AH_TEMPLATE([ACE_HAS_MFC],[Platform supports Microsoft Foundation Classes])

AH_TEMPLATE([ACE_USES_STATIC_MFC],
[When linking MFC as a static library is desired])

AH_TEMPLATE([ACE_HAS_CANCEL_IO],
[Platform supports the Win32 CancelIO() function. (WinNT 4.0 and beyond)])

AH_TEMPLATE([ACE_HAS_WIN32_TRYLOCK],
[The Win32 platform support TryEnterCriticalSection(). (WinNT 4.0 and
beyond)])

AH_TEMPLATE([ACE_HAS_WINSOCK2],[The Win32 platform supports WinSock 2.0.])

AH_TEMPLATE([ACE_USE_SELECT_REACTOR_FOR_REACTOR_IMPL],
[For Win32: Use Select_Reactor as default implementation of Reactor
   instead of WFMO_Reactor.])

AH_TEMPLATE([ACE_HAS_DLL],[Build ACE using the frigging PC DLL nonsense...])

AH_TEMPLATE([ACE_HAS_STRICT],[Use the STRICT compilation mode on Win32.])

AH_TEMPLATE([CYGWIN32], [GNU Win32 environement])
AH_TEMPLATE([ACE_HAS_CYGWIN32_SOCKET_H],[Platform has cygwin32 socket.h.])


dnl ACE internals
AH_TEMPLATE([ACE_DEFAULT_BASE_ADDR],[])
AH_TEMPLATE([ACE_DEFAULT_BASE_ADDRL],[])
AH_TEMPLATE([ACE_DEFAULT_CLOSE_ALL_HANDLES],[])
AH_TEMPLATE([ACE_DEFAULT_MAX_SOCKET_BUFSIZ],[])
AH_TEMPLATE([ACE_DEFAULT_SELECT_REACTOR_SIZE],[The default number of handles the select()-based reactor should handle])
AH_TEMPLATE([ACE_MALLOC_ALIGN],[])
AH_TEMPLATE([ACE_MAP_PRIVATE],[])
AH_TEMPLATE([ACE_THR_PRI_FIFO_DEF],[])
AH_TEMPLATE([ACE_TIMER_SKEW],[])
AH_TEMPLATE([ACE_UINT64_FORMAT_SPECIFIER],[The format specifier (e.g. "%Lu") for the 64 bit unsigned integer type])
AH_TEMPLATE([ACE_USE_RCSID],[Enable embedding of global RCS ID strings into compiled object file])
AH_TEMPLATE([IP_ADD_MEMBERSHIP],[])
AH_TEMPLATE([IP_DROP_MEMBERSHIP],[])


dnl Specify sizes of given built-in types.  If a size isn't defined here,
dnl then ace/Basic_Types.h will attempt to deduce the size.
dnl AH_TEMPLATE([ACE_SIZEOF_CHAR],[Size of native "char" type])
AH_TEMPLATE([ACE_SIZEOF_SHORT],[Size of the native "short" type])
AH_TEMPLATE([ACE_SIZEOF_INT],[Size of the native "int" type])
AH_TEMPLATE([ACE_SIZEOF_LONG],[Size of the native "long" type])
AH_TEMPLATE([ACE_SIZEOF_LONG_LONG],[Size of the native "long long" type])
AH_TEMPLATE([ACE_SIZEOF_VOID_P],[Size of the native "pointer to void" type])
AH_TEMPLATE([ACE_SIZEOF_FLOAT],[Size of the native "float" type])
AH_TEMPLATE([ACE_SIZEOF_DOUBLE],[Size of the native "double" type])
AH_TEMPLATE([ACE_SIZEOF_LONG_DOUBLE],[Size of the native "long double" type])


AH_VERBATIM([ACE_UINT64_TYPEDEF],
[
/*
   typedef for ACE_UINT64

   We only make the typedef if ACE_UINT64_TYPEDEF is defined.  Otherwise,
   let ace/Basic_Types.h do the work for us.
*/
#undef ACE_UINT64_TYPEDEF
#ifdef ACE_UINT64_TYPEDEF
   typedef ACE_UINT64_TYPEDEF ACE_UINT64;
#endif /* ACE_UINT64_TYPEDEF */
])

AH_TEMPLATE([ACE_LOFF_T_TYPEDEF],[typedef for ACE_LOFF_T])

AH_TEMPLATE([__ACE_INLINE__],[Enable ACE inlining])

AH_TEMPLATE([ACE_NO_INLINE],[Explicitly disable ACE inlining])

AH_TEMPLATE([ACE_COMPILE_TIMEPROBES],[Enable ACE_Timeprobes])

AH_TEMPLATE([ACE_HAS_GNU_REPO],
[Enable use of GNU template repositories.  GNU C++ w/repo patch
and EGCS only])

AH_TEMPLATE([ACE_HAS_AIO_CALLS],[Platform supports Asynchronous IO calls])

AH_TEMPLATE([ACE_HAS_ALT_CUSERID],
[Use ACE's alternate cuserid() implementation since a system
cuserid() may not exist, or it is not desirable to use it.  The
implementation requires ACE_LACKS_PWD_FUNCTIONS to be undefined and
that the geteuid() system call exists.])

AH_TEMPLATE([ACE_HAS_ANSI_CASTS],
[Platform supports new C++ style casts (dynamic_cast, static_cast,
reinterpret_cast and const_cast)])

AH_TEMPLATE([ACE_DEFAULT_THREAD_KEYS],
[Number of TSS keys, with ACE_HAS_TSS_EMULATION _only_.  Defaults to 64.])

AH_TEMPLATE([ACE_THREADS_DONT_INHERIT_LOG_MSG],
[Specify this if you don't want threads to inherit parent thread's
ACE_Log_Msg properties.])

AH_TEMPLATE([ACE_HAS_ONE_DEFINITION_RULE],
[Compiler enforces C++ One Definition Rule])

AH_TEMPLATE([ACE_HAS_PRIOCNTL],[OS has priocntl (2)])

dnl Platform has the MIT pthreads APIs for timed send/recv operations
AH_TEMPLATE([ACE_HAS_RECV_TIMEDWAIT],[])
AH_TEMPLATE([ACE_HAS_RECVFROM_TIMEDWAIT],[])
AH_TEMPLATE([ACE_HAS_RECVMSG_TIMEDWAIT],[])
AH_TEMPLATE([ACE_HAS_SEND_TIMEDWAIT],[])
AH_TEMPLATE([ACE_HAS_SENDTO_TIMEDWAIT],[])
AH_TEMPLATE([ACE_HAS_SENDMSG_TIMEDWAIT],[])
AH_TEMPLATE([ACE_HAS_READ_TIMEDWAIT],[])
AH_TEMPLATE([ACE_HAS_READV_TIMEDWAIT],[])
AH_TEMPLATE([ACE_HAS_WRITE_TIMEDWAIT],[])
AH_TEMPLATE([ACE_HAS_WRITEV_TIMEDWAIT],[])

AH_TEMPLATE([ACE_HAS_RLIMIT_RESOURCE_ENUM],
[Platform has enum instead of int for first argument to ::{get,set}rlimit ().
The value of this macro is the enum definition, e.g.,
enum __rlimit_resource, for Linux glibc 2.0.])

AH_TEMPLATE([ACE_HAS_RUSAGE_WHO_ENUM],
[Platform has enum instead of int for first argument to ::getrusage ().  The
value of this macro is the enum definition, e.g., enum __rusage_who, for
Linux glibc 2.0.])

AH_TEMPLATE([ACE_HAS_STDARG_THR_DEST],
[Platform has void (*)(...) prototype for pthread_key_create()
destructor (e.g., LynxOS).])

AH_TEMPLATE([ACE_HAS_STL_MAP_CONFLICT],
[Used when users want to compile ACE with STL and STL map class
conflicts with <net/if.h> map struct.])

AH_TEMPLATE([ACE_HAS_STL_QUEUE_CONFLICT],
[Used when users want to compile ACE with STL and STL queue class
conflicts with <netinet/in.h> queue struct.])

AH_TEMPLATE([ACE_HAS_4_4BSD_SENDMSG_RECVMSG],
[Platform has BSD 4.4 sendmsg()/recvmsg() APIs.])

AH_TEMPLATE([ACE_HAS_P_READ_WRITE],
[Platform has pread() and pwrite() support.])

AH_TEMPLATE([ACE_HAS_AIX_BROKEN_SOCKET_HEADER],
[Platform, such as AIX4, needs to wrap #include of sys/socket.h with
#undef/#define of __cplusplus.
])

AH_TEMPLATE([ACE_HAS_AIX_HI_RES_TIMER],
[Platform has AIX4 ::read_real_time()])

AH_TEMPLATE([ACE_HAS_ALLOCA],[Compiler/platform supports alloca().])

AH_TEMPLATE([ACE_HAS_ALLOCA_H],[Compiler/platform has <alloca.h>])

AH_TEMPLATE([ACE_HAS_ALPHA_TIMER],
[CPU is an Alpha, with the rpcc instruction to read the tick timer.])

AH_TEMPLATE([ACE_HAS_AUTOMATIC_INIT_FINI],
[Compiler/platform correctly calls init()/fini() for shared libraries.])

AH_TEMPLATE([ACE_HAS_WORKING_EXPLICIT_TEMPLATE_DESTRUCTOR],
[Compiler handles explicit calling of template destructor correctly.
See `ace/OS.h' for details.])

AH_TEMPLATE([ACE_HAS_BROKEN_MAP_FAILED],
[Platform doesn't cast MAP_FAILED to a (void *).])

AH_TEMPLATE([ACE_HAS_BROKEN_NAMESPACES],
[Compiler/platform doesn't support namespaces (or the support is not
fully implemented.)])

AH_TEMPLATE([ACE_HAS_BROKEN_BITSHIFT],
[Compiler has integer overflow problem with bit-shift operations.])

AH_TEMPLATE([ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS],
[Compiler can't handle const char * as rvalue in conditional operator.])

AH_TEMPLATE([ACE_HAS_BROKEN_CONVERSIONS],
[Compiler can't handle calls like foo->operator T *()])

AH_TEMPLATE([ACE_HAS_BROKEN_CTIME],
[Compiler/platform uses macro for ctime (e.g., MVS)])

AH_TEMPLATE([ACE_HAS_BROKEN_DGRAM_SENDV],
[Platform sendv() does not work properly with datagrams, i.e. it
fails when the iovec size is IOV_MAX.])

AH_TEMPLATE([ACE_HAS_BROKEN_MSG_H],
[Platform headers don't support <msg.h> prototypes])

AH_TEMPLATE([ACE_HAS_BROKEN_MMAP_H],
[HP/UX does not wrap the mmap(2) header files with extern "C".])

AH_TEMPLATE([ACE_HAS_BROKEN_NESTED_TEMPLATES],
[MSVC has trouble with defining STL containers for nested structs and
classes.])

AH_TEMPLATE([ACE_HAS_BROKEN_NON_BLOCKING_CONNECTS],
[Platform has a bug with non-blocking connects (e.g., WinNT 4.0)])

AH_TEMPLATE([ACE_HAS_BROKEN_POSIX_TIME],
[Platform defines struct timespec in <sys/timers.h>])

AH_TEMPLATE([ACE_HAS_BROKEN_R_ROUTINES],
[Platform defines ctime_r, asctime_r, rand_r and getpwnam_r as macros])

AH_TEMPLATE([ACE_HAS_BROKEN_RANDR],
[OS/compiler's header files are inconsistent with libC definition of
rand_r().])

AH_TEMPLATE([ACE_HAS_BROKEN_READV],
[OS/Compiler's header files are not consistent with readv() definition.])

AH_TEMPLATE([ACE_HAS_BROKEN_SAP_ANY],
[Compiler can't handle the static ACE_Addr::sap_any construct.])

AH_TEMPLATE([ACE_HAS_BROKEN_SENDMSG],
[OS/compiler omits the const from the sendmsg() prototype.])

AH_TEMPLATE([ACE_HAS_BROKEN_SETRLIMIT],
[OS/compiler omits the const from the rlimit parameter in the
setrlimit() prototype.])

AH_TEMPLATE([ACE_HAS_BROKEN_T_ERROR],
[Compiler/platform has the wrong prototype for t_error(), i.e.,
t_error(char *) rather than t_error(const char *).])

AH_TEMPLATE([ACE_HAS_BROKEN_TIMESPEC_MEMBERS],
[Platform defines struct timespec members as ts_sec and ts_nsec
instead of tv_sec and tv_nsec.  This is highly non-portable.
Currently only FreeBSD 2.1.x uses it.])

AH_TEMPLATE([ACE_HAS_BROKEN_WRITEV],
[OS/compiler omits the const from the iovec parameter in the
writev() prototype.])

AH_TEMPLATE([ACE_HAS_BROKEN_XTI_MACROS],
[OS header files have some problems with XTI (HP/UX 11).])

AH_TEMPLATE([ACE_HAS_BSTRING],
[Platform has <bstring.h> (which contains bzero() prototype)])

AH_TEMPLATE([ACE_HAS_BYTESEX_H],[Platform has <bytesex.h>.])

AH_TEMPLATE([ACE_HAS_CHARPTR_DL],
[OS/platform uses char * for dlopen/dlsym args, rather than const char *.])

AH_TEMPLATE([ACE_HAS_CHARPTR_SOCKOPT],
[OS/platform uses char * for sockopt, rather than const char *])

AH_TEMPLATE([ACE_HAS_CHARPTR_SPRINTF],
[sprintf() returns char * rather than int (e.g., SunOS 4.x)])

AH_TEMPLATE([ACE_HAS_CLOCK_GETTIME],
[Platform supports POSIX 1.b clock_gettime()])

AH_TEMPLATE([ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES],
[Prototypes for both signal() and struct sigaction are consistent.])

AH_TEMPLATE([ACE_HAS_CPLUSPLUS_HEADERS],
[Compiler/platform has correctly prototyped header files.])

AH_TEMPLATE([ACE_HAS_DIRENT],
[Platform supports operations on directories via struct dirent,
readdir_r, etc.])

AH_TEMPLATE([ACE_HAS_DLFCN_H_BROKEN_EXTERN_C],
[For platforms, e.g., RedHat 4.2/Linux 2.0.30/Alpha, that don't
declare dl* functions as extern "C" in dlfcn.h.])

AH_TEMPLATE([ACE_HAS_EXCEPTIONS],[Compiler supports C++ exception handling.])

AH_TEMPLATE([ACE_HAS_FL],[Platform has Fast-Light (FL) toolkit installed.])

AH_TEMPLATE([ACE_HAS_GETPAGESIZE],
[Platform supports getpagesize() call (otherwise, ACE_PAGE_SIZE must
be defined, except on Win32).])

AH_TEMPLATE([ACE_HAS_GETRUSAGE],
[Platform supports the getrusage() system call.])

AH_TEMPLATE([ACE_HAS_GETRUSAGE_PROTO],
[Platform has a getrusage () prototype in sys/resource.h that
   differs from the one in ace/OS.i.])

AH_TEMPLATE([ACE_HAS_GNUC_BROKEN_TEMPLATE_INLINE_FUNCTIONS],
[GNUC 2.7.3 mistakenly takes the template definition as the place
   where an inline function of an argument class is first used.])

AH_TEMPLATE([ACE_HAS_GNU_CSTRING_H],
[Denotes that GNU has cstring.h as standard which redefines memchr()])

AH_TEMPLATE([ACE_HAS_GPERF],
[The GPERF utility is compiled for this platform])

AH_TEMPLATE([ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT],
[Optimize ACE_Handle_Set::count_bits for select() operations (common case)])

AH_TEMPLATE([ACE_HAS_LSEEK64],
[Platform supports lseek64().  This should not be defined if
   ACE_HAS_LLSEEK is defined.])

AH_TEMPLATE([ACE_HAS_LLSEEK],
[Platform supports llseek(). This should not be defined if
   ACE_HAS_LSEEK64 is defined.])

AH_TEMPLATE([ACE_HAS_HI_RES_TIMER],
[Compiler/platform supports SunOS high resolution timers])

AH_TEMPLATE([ACE_HAS_IDTYPE_T],[Compiler/platform supports idtype_t.])

AH_TEMPLATE([ACE_HAS_INLINED_OSCALLS],[
Inline all the static class OS methods to remove call overhead
Note: This gets defined by OS.h if __ACE_INLINE__ is defined])

AH_TEMPLATE([ACE_HAS_IP_MULTICAST],[Platform supports IP multicast])

AH_TEMPLATE([ACE_HAS_IPV6],[Platform supports IPv6])

AH_TEMPLATE([ACE_HAS_NONSTATIC_OBJECT_MANAGER],
[Causes the ACE_Object_Manager instance to be created in
   main (int, char *[]), instead of as a static (global) instance.])

AH_TEMPLATE([ACE_HAS_THR_KEYDELETE],
[Platform supports thr_keydelete (e.g,. UNIXWARE)])

AH_TEMPLATE([ACE_HAS_THR_MINSTACK],
[Platform calls thr_minstack() rather than thr_min_stack() (e.g., Tandem).])

AH_TEMPLATE([ACE_HAS_LIMITED_RUSAGE_T],
[The rusage_t structure has only two fields.])

AH_TEMPLATE([ACE_HAS_BIG_FD_SET],
[Compiler/platform has "big" fd_set, i.e. large number of bits set
   in fd_set passed back from select().])

AH_TEMPLATE([ACE_HAS_LONG_MAP_FAILED],
[Platform defines MAP_FAILED as a long constant.])

AH_TEMPLATE([ACE_HAS_MALLOC_STATS], [Enabled malloc statistics collection.])

AH_TEMPLATE([ACE_HAS_MEMCHR],[Use native implementation of memchr().])

AH_TEMPLATE([ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION],
[Avoid #including ace/streams.h in OS.h.  Users must include
   ace/streams.h, <iostream>, or <iostream.h> explicitly in their code.
   Some platforms, such as g++/VxWorks, have trouble compiling templates
   and iostreams header because of static variables in the stream
   headers.  This flag will also avoid extra compilation and runtime
   overheads on some platforms.])

AH_TEMPLATE([ACE_USES_OLD_IOSTREAMS],
[Some files, such as ace/streams.h, want to include new style C++
   stream headers.  These headers are iomanip, ios, iostream, istream,
   ostream, fstream and streambuf.  If _all_ of these headers aren't
   available, then assume that only iostream.h and fstream.h are
   available.])

AH_TEMPLATE([ACE_HAS_MSG],[Platform supports recvmsg and sendmsg])

AH_TEMPLATE([ACE_HAS_MT_SAFE_MKTIME],
[Platform supports MT safe mktime() call (do any of them?)])

AH_TEMPLATE([ACE_HAS_MT_SAFE_SOCKETS],
[Sockets may be called in multi-threaded programs])

AH_TEMPLATE([ACE_HAS_NONCONST_GETBY],
[Platform uses non-const char * in calls to gethostbyaddr,
   gethostbyname, getservbyname])

AH_TEMPLATE([ACE_HAS_NONCONST_MSGSND],
[Platform has a non-const parameter to msgsnd() (e.g., SCO).])

AH_TEMPLATE([ACE_HAS_NONCONST_SELECT_TIMEVAL],
[Platform's select() uses non-const timeval* (only found on Linux
   right now)])

AH_TEMPLATE([ACE_HAS_GNUG_PRE_2_8],
[Platform has "old" GNU compiler,  i.e. does not completely support
   standard C++. (compiling with g++ prior to version 2.8.0)])

AH_TEMPLATE([ACE_HAS_OLD_MALLOC],
[Compiler/platform uses old malloc()/free() prototypes (ugh)])

AH_TEMPLATE([ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R],
[Uses ctime_r & asctime_r with only two parameters vs. three.])

AH_TEMPLATE([ACE_HAS_ONLY_SCHED_OTHER],
[Platform, e.g., Solaris 2.5, only supports SCHED_OTHER POSIX
   scheduling policy.])

AH_TEMPLATE([ACE_HAS_OPTIMIZED_MESSAGE_QUEUE],
[Use the semaphore implementation of ACE_Message_Queue rather than
   the emulated condition variable (NT and VxWorks).])

AH_TEMPLATE([ACE_HAS_OSF_TIMOD_H],
[Platform supports the OSF TLI timod STREAMS module])

AH_TEMPLATE([ACE_HAS_PENTIUM],[Platform is an Intel Pentium microprocessor.])

AH_TEMPLATE([ACE_HAS_POLL],[Platform contains <poll.h>])

AH_TEMPLATE([ACE_HAS_POSITION_INDEPENDENT_POINTERS],
[Platform supports "position-independent" features provided by
   ACE_Based_Pointer<>.])

AH_TEMPLATE([ACE_HAS_POSIX_GETPWNAM_R],
[Platform supports POSIX getpwnam_r() function])

AH_TEMPLATE([ACE_HAS_POSIX_NONBLOCK],
[Platform supports POSIX O_NONBLOCK semantics])

AH_TEMPLATE([ACE_HAS_POSIX_SEM],
[Platform supports POSIX real-time semaphores (e.g., VxWorks and
   Solaris)])

AH_TEMPLATE([ACE_HAS_POSIX_TIME],
[Platform supports the POSIX struct timespec type])

AH_TEMPLATE([ACE_HAS_PROC_FS],
[Platform supports the /proc file system and defines tid_t
   in <sys/procfs.h>])

AH_TEMPLATE([ACE_HAS_POWERPC_TIMER],
[Platform supports PowerPC time-base register.])

AH_TEMPLATE([ACE_HAS_PRUSAGE_T],[Platform supports the prusage_t struct])

AH_TEMPLATE([ACE_HAS_PTHREADS],[Platform supports POSIX Threads])

AH_TEMPLATE([ACE_HAS_PTHREADS_DRAFT4],
[Platform supports POSIX Threads .4a Draft 4])

AH_TEMPLATE([ACE_HAS_PTHREADS_DRAFT6],
[Platform supports POSIX Threads .4a Draft 6])

AH_TEMPLATE([ACE_HAS_PTHREADS_DRAFT7],a
[Platform supports POSIX Threads .1c Draft 7])

AH_TEMPLATE([ACE_HAS_PTHREADS_STD],[Platform supports POSIX.1c-1995 threads])

AH_TEMPLATE([ACE_HAS_PTHREADS_UNIX98_EXT],
[Platform has the UNIX98 extensions to Pthreads (suspend, continue,
   rwlocks)])

AH_TEMPLATE([ACE_HAS_PTHREAD_CONDATTR_SETKIND_NP],
[Platform has pthread_condattr_setkind_np().])

AH_TEMPLATE([ACE_HAS_PTHREAD_MUTEXATTR_SETKIND_NP],
[Platform has pthread_mutexattr_setkind_np().])

AH_TEMPLATE([ACE_HAS_PTHREAD_PROCESS_ENUM],
[pthread.h declares an enum with PTHREAD_PROCESS_PRIVATE and
   PTHREAD_PROCESS_SHARED values])

AH_TEMPLATE([ACE_HAS_PURIFY],[Purify'ing.  Defined on command line.])

AH_TEMPLATE([ACE_HAS_QUANTIFY],[Quantify'ing.  Defined on command line.])

AH_TEMPLATE([ACE_HAS_RECURSIVE_MUTEXES],
[Mutexes are inherently recursive (e.g., Win32) ])

AH_TEMPLATE([ACE_HAS_RECURSIVE_THR_EXIT_SEMANTICS],
[Platform will recurse infinitely on thread exits from TSS cleanup
   routines (e.g., AIX)])

AH_TEMPLATE([ACE_HAS_REENTRANT_FUNCTIONS],
[Platform supports reentrant functions (i.e., all the POSIX *_r
   functions).])

AH_TEMPLATE([ACE_HAS_X86_STAT_MACROS],
[Solaris for intel uses macros for fstat() and stat(), these are
   wrappers for _fxstat() and _xstat() uses of the macros.  Causes
   compile and runtime problems.])

AH_TEMPLATE([ACE_HAS_XPG4_MULTIBYTE_CHAR],
[Platform has support for multi-byte character support compliant
   with the XPG4 Worldwide Portability Interface wide-character
   classification.])

AH_TEMPLATE([ACE_LACKS_AUTO_MMAP_REPLACEMENT],
[No system support for replacing any previous mappings.])

AH_TEMPLATE([ACE_LACKS_AUTO_PTR],
[Platform lacks support for the standard C++ auto_ptr class])

AH_TEMPLATE([ACE_AUTO_PTR_LACKS_RESET],
[Compiler/platform standard C++ auto_ptr implementation lacks
   reset() method])

AH_TEMPLATE([ACE_LACKS_BSEARCH],
[Compiler/platform lacks the standard C library bsearch() function])

AH_TEMPLATE([ACE_LACKS_CMSG_DATA_MACRO],
[Platform has ACE_HAS_4_4BSD_SENDMSG_RECVMSG but does not define
   CMSG_DATA (cmsg) macro.])

AH_TEMPLATE([ACE_LACKS_CMSG_DATA_MEMBER],
[Platform has ACE_HAS_4_4BSD_SENDMSG_RECVMSG but its cmsghdr
   structure does not contain an 'unsigned char cmsg_data[0]' member.
   (This may be 'unsigned char __cmsg_data[0]' on some platforms, in
   which case we need another macro.)])

AH_TEMPLATE([ACE_LACKS_FGETWC],
[Compiler/platform lacks the fgetwc() function])

AH_TEMPLATE([ACE_LACKS_QSORT],
[Compiler/platform lacks the standard C library qsort() function])

AH_TEMPLATE([ACE_LACKS_FCNTL],[Platform lacks POSIX-style fcntl ()])

AH_TEMPLATE([ACE_LACKS_FSYNC],[Platform lacks fsync()])

AH_TEMPLATE([ACE_LACKS_RTTI],
[Compiler does not support dynamic_cast.  Usually used with
   ACE_HAS_ANSI_CASTS])

AH_TEMPLATE([ACE_LACKS_READDIR_R],[Platform lacks readdir_r()])

AH_TEMPLATE([ACE_LACKS_SEEKDIR],[Platform lacks seekdir()])

AH_TEMPLATE([ACE_LACKS_TELLDIR],[Platform lacks telldir()])

AH_TEMPLATE([ACE_LACKS_INLINE_FUNCTIONS],
[Platform can't handle "inline" keyword correctly.])

AH_TEMPLATE([ACE_LACKS_IOSTREAM_TOTALLY],
[iostreams are not supported adequately on the given platform.])

AH_TEMPLATE([ACE_LACKS_NETDB_REENTRANT_FUNCTIONS],
[Platform does not support reentrant netdb functions
   (getprotobyname_r, getprotobynumber_r, gethostbyaddr_r,
   gethostbyname_r, getservbyname_r).])

AH_TEMPLATE([ACE_HAS_REGEX],
[Platform supports the POSIX regular expression library])

AH_TEMPLATE([ACE_HAS_SCANDIR],
[Platform supports the scandir() function.])

AH_TEMPLATE([ACE_HAS_SELECT_H],
[Platform has special header for select().])

AH_TEMPLATE([ACE_HAS_SEMUN],
[Compiler/platform defines a union semun for SysV shared memory ])

AH_TEMPLATE([ACE_HAS_SET_T_ERRNO],
[Platform has a function to set t_errno (e.g., Tandem).])

AH_TEMPLATE([ACE_HAS_SHM_OPEN],[Platform has shm_open()])

AH_TEMPLATE([ACE_HAS_SIGINFO_T],[Platform supports SVR4 extended signals])

AH_TEMPLATE([ACE_HAS_SIGISMEMBER_BUG],
[Platform has bug with sigismember() (HP/UX 11).])

AH_TEMPLATE([ACE_HAS_SIG_MACROS],
[Platform/compiler has macros for sig{empty,fill,add,del}set (e.g.,
   SCO and FreeBSD)])

AH_TEMPLATE([ACE_HAS_SIGNAL_OBJECT_AND_WAIT],
[Platform supports the Win32 SignalObjectAndWait() function (WinNT
   4.0 and beyond).])

AH_TEMPLATE([ACE_HAS_SIGNAL_SAFE_OS_CALLS],
[Automatically restart OS system calls when EINTR occurs])

AH_TEMPLATE([ACE_HAS_SIGSUSPEND],[Platform has the sigsuspend() system call])

AH_TEMPLATE([ACE_HAS_SIGTIMEDWAIT],
[Platform has the sigtimedwait() system call])

AH_TEMPLATE([ACE_HAS_SIGWAIT],[Platform/compiler has the sigwait(2) prototype])

AH_TEMPLATE([ACE_HAS_SIG_ATOMIC_T],
[Compiler/platform defines the sig_atomic_t typedef])

AH_TEMPLATE([ACE_HAS_SIG_C_FUNC],
[Compiler requires extern "C" functions for signals.])

AH_TEMPLATE([ACE_HAS_SIN_LEN],[Platform supports new BSD inet_addr len field.])

AH_TEMPLATE([ACE_HAS_SIZET_SOCKET_LEN],
[OS/compiler uses size_t * rather than int * for socket lengths])

AH_TEMPLATE([ACE_HAS_SNPRINTF],[Platform offers snprintf().])

AH_TEMPLATE([ACE_HAS_SOCKADDR_MSG_NAME],
[Platform requires (struct sockaddr *) for msg_name field of
   struct msghdr.])

AH_TEMPLATE([ACE_HAS_SOCKIO_H],
[Compiler/platform provides the sys/sockio.h file])

AH_TEMPLATE([ACE_HAS_SOCKLEN_T],
[Platform provides socklen_t type, such as Linux with glibc2.])

AH_TEMPLATE([ACE_HAS_SPARCWORKS_401_SIGNALS],
[Compiler has brain-damaged SPARCwork SunOS 4.x signal prototype...])

AH_TEMPLATE([ACE_HAS_SSIZE_T],[Compiler supports the ssize_t typedef])

AH_TEMPLATE([ACE_HAS_STHREADS],[Platform supports UNIX International Threads])

AH_TEMPLATE([ACE_HAS_THR_YIELD],[Platform has thr_yield()])

AH_TEMPLATE([ACE_HAS_STANDARD_CPP_LIBRARY],
[Platform/compiler supports Standard C++ Library])

AH_TEMPLATE([ACE_HAS_STRBUF_T],[Compiler/platform supports struct strbuf])

AH_TEMPLATE([ACE_HAS_STRDUP_EMULATION],
[Platform/compiler lacks strdup() (e.g., VxWorks, Chorus, WinCE)])

AH_TEMPLATE([ACE_HAS_STRPTIME],[Compile ACE_OS::strptime() wrapper into ACE.])

AH_TEMPLATE([ACE_HAS_STREAMS],[Platform supports STREAMS])

AH_TEMPLATE([ACE_HAS_STREAM_PIPES],[Platform supports STREAM pipes])

AH_TEMPLATE([ACE_HAS_STRERROR],[Compiler/platform supports strerror()])

AH_TEMPLATE([ACE_HAS_STRING_CLASS],
[Platform/Compiler supports a String class (e.g., GNU or Win32).])

AH_TEMPLATE([ACE_HAS_STRINGS],
[Platform has <strings.h> (which contains bzero() prototype)])

AH_TEMPLATE([ACE_HAS_STRUCT_NETDB_DATA],
[Compiler/platform has strange hostent API for socket *_r() calls])

AH_TEMPLATE([ACE_HAS_SUNOS4_GETTIMEOFDAY],
[Platform has void * as second parameter to gettimeofday and a has a
   prototype])

AH_TEMPLATE([ACE_HAS_SUNOS4_SIGNAL_T],
[Compiler has horrible SunOS 4.x signal handlers...])

AH_TEMPLATE([ACE_HAS_SVR4_DYNAMIC_LINKING],
[Compiler/platform supports SVR4 dynamic linking semantics])

AH_TEMPLATE([ACE_HAS_SVR4_GETTIMEOFDAY],
[Compiler/platform supports SVR4 gettimeofday() prototype but
   doesn't have a prototype])

AH_TEMPLATE([ACE_HAS_SVR4_SIGNAL_T],
[Compiler/platform supports SVR4 signal typedef.])

AH_TEMPLATE([ACE_HAS_SVR4_TLI],
[Compiler/platform supports SVR4 TLI (in particular, T_GETNAME stuff).])

AH_TEMPLATE([ACE_HAS_SYSCALL_GETRUSAGE],
[HP/UX has an undefined syscall for GETRUSAGE.])

AH_TEMPLATE([ACE_HAS_SYSCALL_H],
[Compiler/platform contains the <sys/syscall.h> file.])

AH_TEMPLATE([ACE_HAS_SYSENT_H],[Platform provides <sysent.h> header])

AH_TEMPLATE([ACE_HAS_SYSINFO],
[Platform supports system configuration information.])

AH_TEMPLATE([ACE_HAS_SYSV_IPC],
[Platform supports System V IPC (most versions of UNIX, but not Win32)])

AH_TEMPLATE([ACE_HAS_SYS_ERRLIST],
[Platform/compiler supports _sys_errlist symbol])

AH_TEMPLATE([ACE_HAS_SYS_FILIO_H],[Platform provides <sys/filio.h> header])

AH_TEMPLATE([ACE_HAS_SYS_SIGLIST],
[Compiler/platform supports _sys_siglist array])

AH_TEMPLATE([ACE_HAS_SYS_XTI_H],[Platform provides <sys/xti.h> header])

AH_TEMPLATE([ACE_HAS_TEMPLATE_SPECIALIZATION],
[Compiler implements template specialization])

AH_TEMPLATE([ACE_HAS_STD_TEMPLATE_SPECIALIZATION],
[Compiler supports standard C++ template specializations
   (e.g. "template <>" syntax.)])

AH_TEMPLATE([ACE_HAS_STD_TEMPLATE_METHOD_SPECIALIZATION],
[Compiler supports standard C++ template method specializations
   (e.g. "template <>" syntax.)])

AH_TEMPLATE([ACE_HAS_TEMPLATE_TYPEDEFS],
[Compiler implements templates that support typedefs inside of
   classes used as formal arguments to a template class.])

AH_TEMPLATE([ACE_HAS_TERM_IOCTLS],
[Platform has terminal ioctl flags like TCGETS and TCSETS.])

AH_TEMPLATE([ACE_HAS_THREADS],[Platform supports threads.])

AH_TEMPLATE([ACE_HAS_THREAD_SAFE_ACCEPT],
[Platform allows multiple threads to call accept() on the same port
   (e.g., WinNT).])

AH_TEMPLATE([ACE_HAS_THREAD_SELF],
[Platform has thread_self() rather than pthread_self() (e.g.,
   DCETHREADS and AIX)])

AH_TEMPLATE([ACE_HAS_THREAD_SPECIFIC_STORAGE],
[Compiler/platform has thread-specific storage])

AH_TEMPLATE([ACE_HAS_THR_C_DEST],
[The pthread_keycreate() routine *must* take extern C functions.])

AH_TEMPLATE([ACE_HAS_THR_C_FUNC],
[The pthread_create() routine *must* take extern C functions.])

AH_TEMPLATE([ACE_HAS_TIMEZONE_GETTIMEOFDAY],
[Platform/compiler supports timezone * as second parameter to
   gettimeofday() and has a prototype.])

AH_TEMPLATE([ACE_HAS_TIMOD_H],
[Platform supports TLI timod STREAMS module])

AH_TEMPLATE([ACE_HAS_TIUSER_H],[Platform supports TLI tiuser header])

AH_TEMPLATE([ACE_HAS_TIUSER_H_BROKEN_EXTERN_C],
[Platform does not protect <tiuser.h> with extern "C"])

AH_TEMPLATE([ACE_HAS_TLI],
[Platform supports TLI.  Also see ACE_TLI_TCP_DEVICE.])

AH_TEMPLATE([ACE_HAS_TLI_PROTOTYPES],
[Platform provides TLI function prototypes])

AH_TEMPLATE([ACE_HAS_TSS_EMULATION],
[ACE provides TSS emulation.  See also ACE_DEFAULT_THREAD_KEYS.])

AH_TEMPLATE([ACE_HAS_UALARM],[Platform supports ualarm()])

AH_TEMPLATE([ACE_HAS_UCONTEXT_T],
[Platform supports ucontext_t (which is used in the extended signal API).])

AH_TEMPLATE([ACE_HAS_UNION_WAIT],
[The wait() system call takes a (union wait *) rather than int *])

AH_TEMPLATE([ACE_HAS_UNIXWARE_SVR4_SIGNAL_T],
[Has inconsistent SVR4 signal stuff, but not the same as the other
   platforms])

AH_TEMPLATE([ACE_HAS_WCHAR],[Platform/compiler supports wchar_t])

AH_TEMPLATE([ACE_HAS_UTIME],[Platform has <utime.h> header file])

AH_TEMPLATE([ACE_HAS_EXPLICIT_KEYWORD],
[Compiler supports explicit constructors.])

AH_TEMPLATE([ACE_HAS_MUTABLE_KEYWORD],
[Compiler supports the C++ `mutable' keyword.])

AH_TEMPLATE([ACE_HAS_TYPENAME_KEYWORD],
[Compiler supports the C++ typename keyword])

AH_TEMPLATE([ACE_HAS_USING_KEYWORD],
[Compiler supports the new using keyword for C++ namespaces.])

AH_TEMPLATE([ACE_HAS_VERBOSE_NOTSUP],
[Prints out console message in ACE_NOTSUP.  Useful for tracking down
   origin of ACE_NOTSUP.])

AH_TEMPLATE([ACE_HAS_VOIDPTR_MMAP],[Platform requires void * for mmap().])

AH_TEMPLATE([ACE_HAS_VOIDPTR_SOCKOPT],
[OS/compiler uses void * arg 4 setsockopt() rather than const char *])

AH_TEMPLATE([ACE_HAS_XLI],[Platform has the XLI version of TLI])

AH_TEMPLATE([ACE_HAS_XT],[Platform has Xt Intrinsics Toolkit])

AH_TEMPLATE([ACE_LACKS_MOTIF],
[Platform does not have Motif X toolkit available])

AH_TEMPLATE([ACE_HAS_XTI],
[Platform has XTI (X/Open-standardized superset of TLI).  Implies
   ACE_HAS_TLI but uses a different header file.])

AH_TEMPLATE([ACE_LACKS_ACCESS],
[Platform lacks access() (e.g., VxWorks and Chorus)])

AH_TEMPLATE([ACE_LACKS_ACE_IOSTREAM],
[Platform can not build ace/IOStream{,_T}.cpp.  This does not
   necessarily mean that the platform does not support iostreams.])

AH_TEMPLATE([ACE_LACKS_ACE_CODECS],
[Do not compile support for the "Codecs" ACE features.])

AH_TEMPLATE([ACE_LACKS_ACE_OTHER],
[Do not compile support for the "other" ACE features, such as CORBA
   handling, name services, and QoS.])

AH_TEMPLATE([ACE_LACKS_ACE_SVCCONF],
[Do not compile support for the ACE Service Configurator.])

AH_TEMPLATE([ACE_LACKS_ACE_TOKEN],
[Do not compile support for the ACE Token feature.])

AH_TEMPLATE([ACE_LACKS_COND_T],
[Platform lacks condition variables (e.g., Win32 and VxWorks)])

AH_TEMPLATE([ACE_LACKS_COND_TIMEDWAIT_RESET],
[pthread_cond_timedwait does *not* reset the time argument when
   the lock is acquired.])

AH_TEMPLATE([ACE_LACKS_CONDATTR_PSHARED],
[Platform has no implementation of pthread_condattr_setpshared(), even
   though it supports pthreads!])

AH_TEMPLATE([ACE_LACKS_CONST_STRBUF_PTR],
[Platform uses struct strbuf * rather than const struct strbuf *
   (e.g., HP/UX 10.x)])

AH_TEMPLATE([ACE_LACKS_CONST_TIMESPEC_PTR],
[Platform forgot const in cond_timewait (e.g., HP/UX).])

AH_TEMPLATE([ACE_LACKS_DIFFTIME],[Platform lacks difftime() implementation])

AH_TEMPLATE([ACE_LACKS_EXEC],
[Platform lacks the exec() family of system calls (e.g., Win32,
   VxWorks, Chorus)])

AH_TEMPLATE([ACE_LACKS_FILELOCKS],[Platform lacks file locking mechanism])

AH_TEMPLATE([ACE_LACKS_FLOATING_POINT],
[Platform does not support floating point operations])

AH_TEMPLATE([ACE_LACKS_FORK],
[Platform lacks the fork() system call (e.g., Win32, VxWorks, Chorus)])

AH_TEMPLATE([ACE_LACKS_GETOPT_PROTO],
[Platform lacks the getopt() prototype (e.g., LynxOS)])

AH_TEMPLATE([ACE_LACKS_GETPGID],
[Platform lacks getpgid() call (e.g., Win32, Chorus, and FreeBSD).])

AH_TEMPLATE([ACE_LACKS_GETPPID],[Platform lacks getppid() call.])

AH_TEMPLATE([ACE_LACKS_SETREGID],[Platform lacks setregid() call.])

AH_TEMPLATE([ACE_LACKS_SETREUID],[Platform lacks setreuid() call.])

AH_TEMPLATE([ACE_LACKS_GETSERVBYNAME],
[Platforms lacks getservbyname() (e.g., VxWorks and Chorus).])

AH_TEMPLATE([ACE_LACKS_IOSTREAM_FX],
[iostream header does not declare ipfx (), opfx (), etc.])

AH_TEMPLATE([ACE_LACKS_LINEBUFFERED_STREAMBUF],
[Platform lacks streambuf "linebuffered ()".])

AH_TEMPLATE([ACE_LACKS_LONGLONG_T],
[Compiler/platform does not support the unsigned long long datatype.])

AH_TEMPLATE([ACE_LACKS_LSTAT],[Platform lacks the lstat() function.])

AH_TEMPLATE([ACE_LACKS_U_LONGLONG_T],
[Platform does not have u_longlong_t typedef])

AH_TEMPLATE([ACE_LACKS_MADVISE],
[Platform lacks madvise() (e.g., Linux)])

AH_TEMPLATE([ACE_LACKS_MALLOC_H],[Platform lacks malloc.h])

AH_TEMPLATE([ACE_LACKS_MEMORY_H],
[Platform lacks memory.h (e.g., VxWorks and Chorus)])

AH_TEMPLATE([ACE_LACKS_STDINT_H],
[Platform lacks stdint.h (e.g., UnixWare)])

AH_TEMPLATE([ACE_LACKS_INTTYPES_H],
[Platform lacks inittypes.h (e.g. Unixware)])

AH_TEMPLATE([ACE_LACKS_MKFIFO],
[Platform lacks mkfifo(), e.g. VxWorks, Chorus, pSoS, and WinNT.])

AH_TEMPLATE([ACE_LACKS_MKTEMP],[ACE has no mktemp()])

AH_TEMPLATE([ACE_LACKS_MMAP],
[The platform doesn't have mmap(2) (e.g., SCO UNIX).])

AH_TEMPLATE([ACE_LACKS_MODE_MASKS],
[Platform/compiler doesn't have open() mode masks.])

AH_TEMPLATE([ACE_LACKS_MPROTECT],
[The platform doesn't have mprotect(2) (e.g., EPLX real time OS from CDC
   (based on LYNX))])

AH_TEMPLATE([ACE_LACKS_MSG_ACCRIGHTS],
[Platform defines ACE_HAS_MSG, but lacks msg_accrights{len}.])

AH_TEMPLATE([ACE_LACKS_MSGBUF_T],
[Platform lacks struct msgbuf (e.g., NT and MSV).])

AH_TEMPLATE([ACE_LACKS_MSYNC],[Platform lacks msync()])

AH_TEMPLATE([ACE_HAS_BROKEN_NETBSD_MSYNC],
[Platform doesn't support "sync" third argument to msync (?)])

AH_TEMPLATE([ACE_LACKS_TEMPNAM],[Platform doesn't support tempnam])

AH_TEMPLATE([ACE_LACKS_MUTEXATTR_PSHARED],
[Platform lacks pthread_mutexattr_setpshared().])

AH_TEMPLATE([ACE_LACKS_NULL_PTHREAD_STATUS],
[OS requires non-null status pointer for pthread_join ()])

AH_TEMPLATE([ACE_HAS_MUTEX_TIMEOUTS],
[Compiler supports timed mutex acquisitions (e.g. pthread_mutex_timedlock()).])

AH_TEMPLATE([ACE_LACKS_PARAM_H],[Platform lacks <sys/param.h> (e.g., MVS)])

AH_TEMPLATE([ACE_LACKS_NAMED_POSIX_SEM],
[Platform lacks named POSIX semaphores (e.g., Chorus)])

AH_TEMPLATE([ACE_LACKS_NATIVE_STRPTIME],
[Platform lacks native strptime() implementation.])

AH_TEMPLATE([ACE_LACKS_RLIMIT],
[Platform/compiler lacks {get,set}rlimit() function (e.g., VxWorks,
   Chorus, and SCO UNIX)])

AH_TEMPLATE([ACE_LACKS_RLIMIT_PROTOTYPE],
[Platform/compiler lacks {get,set}rlimit() prototypes (e.g., Tandem)])

AH_TEMPLATE([ACE_LACKS_POSIX_PROTOTYPES],
[Platform lacks POSIX prototypes for certain System V functions like
   shared memory and message queues.])

AH_TEMPLATE([ACE_LACKS_SOME_POSIX_PROTOTYPES],
[Platform lacks POSIX prototypes for certain System V functions like
   shared memory and message queues.])

AH_TEMPLATE([ACE_LACKS_PLACEMENT_OPERATOR_NEW],
[Compiler doesn't support placement operator new(size_t, void *).])

AH_TEMPLATE([ACE_LACKS_PLACEMENT_OPERATOR_DELETE],
[Compiler doesn't support placement operator delete(void *, void *).])

AH_TEMPLATE([ACE_LACKS_PRAGMA_ONCE],
[Compiler complains about use of obsolete "pragma once"])

AH_TEMPLATE([ACE_LACKS_PRI_T],
[Platform lacks pri_t (e.g., Tandem NonStop UNIX).])

AH_TEMPLATE([ACE_LACKS_THREAD_STACK_ADDR],
[Platform lack pthread_attr_setstackaddr()])

AH_TEMPLATE([ACE_LACKS_PTHREAD_CANCEL],[Platform lacks pthread_cancel()])

AH_TEMPLATE([ACE_LACKS_PTHREAD_SIGMASK],[Platform lacks pthread_sigmask()])

AH_TEMPLATE([ACE_LACKS_PTHREAD_THR_SIGSETMASK],
[Platform lacks pthread_thr_sigsetmask (e.g., MVS, HP/UX, and OSF/1 3.2)])

AH_TEMPLATE([ACE_LACKS_PTHREAD_YIELD],
[Platfrom lack pthread_yield() support.])

AH_TEMPLATE([ACE_LACKS_PWD_FUNCTIONS],
[Platform lacks, getpwnam(), etc.])

AH_TEMPLATE([ACE_LACKS_PWD_REENTRANT_FUNCTIONS],
[Platform lacks getpwnam_r() methods (e.g., SGI 6.2).])

AH_TEMPLATE([ACE_LACKS_READLINK],[Platform lacks the readlink() function.])

AH_TEMPLATE([ACE_LACKS_RENAME],[Platform lacks the rename() function.])

AH_TEMPLATE([ACE_LACKS_RECVMSG],[Platform lacks recvmsg()])

AH_TEMPLATE([ACE_LACKS_RWLOCK_T],[Platform lacks readers/writer locks.])

AH_TEMPLATE([ACE_LACKS_SBRK],
[Platform lacks a working sbrk() (e.g., Win32 and VxWorks)])

AH_TEMPLATE([ACE_LACKS_SEMBUF_T],
[Platform lacks struct sembuf (e.g., Win32 and VxWorks)])

AH_TEMPLATE([ACE_LACKS_SETDETACH],
[Platform lacks pthread_attr_setdetachstate() (e.g., HP/UX 10.x)])

AH_TEMPLATE([ACE_LACKS_SETSCHED],
[Platform lacks pthread_attr_setsched() (e.g. MVS)])

AH_TEMPLATE([ACE_LACKS_SIGACTION],
[Platform lacks struct sigaction (e.g., Win32 and Chorus)])

AH_TEMPLATE([ACE_LACKS_SIGNED_CHAR],
[Platform lacks "signed char" type (broken!)])

AH_TEMPLATE([ACE_LACKS_SIGSET],
[Platform lacks signal sets (e.g., Chorus and Win32)])

AH_TEMPLATE([ACE_LACKS_STRRCHR],
[Platform/compiler lacks strrchr () function.])

AH_TEMPLATE([ACE_LACKS_STRUCT_DIR],
[Platform lacks dirent structure.])

AH_TEMPLATE([ACE_LACKS_SYS_NERR],
[Platforms/compiler lacks the sys_nerr variable (e.g., VxWorks and MVS).])

AH_TEMPLATE([ACE_LACKS_SYSV_MSG_H],
[Platform lacks sys/msg.h (e.g., Chorus and VxWorks)])

AH_TEMPLATE([ACE_LACKS_SYSV_MSQ_PROTOS],
[Platform lacks SYSV message queue prototypes])

AH_TEMPLATE([ACE_LACKS_KEY_T],
[Platform lacks key_t (e.g., Chorus, VxWorks, Win32)])

AH_TEMPLATE([ACE_LACKS_SENDMSG],[Platform lacks sendmsg()])

AH_TEMPLATE([ACE_LACKS_SI_ADDR],
[Platform lacks the si_addr field of siginfo_t (e.g., VxWorks and
   HP/UX 10.x)])

AH_TEMPLATE([ACE_LACKS_SYSV_SHMEM],
[Platform lacks System V shared memory (e.g., Win32 and VxWorks)])

AH_TEMPLATE([ACE_LACKS_SIGINFO_H],
[Platform lacks the siginfo.h include file (e.g., MVS)])

AH_TEMPLATE([ACE_LACKS_SOCKET_BUFSIZ],
[Platform doesn't support SO_SNDBUF/SO_RCVBUF (used in TAO)])

AH_TEMPLATE([ACE_LACKS_SOCKETPAIR],
[Platform lacks the socketpair() call (e.g., SCO UNIX)])

AH_TEMPLATE([ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES],
[Compiler doesn't support static data member templates])

AH_TEMPLATE([ACE_LACKS_STRCASECMP],
[Compiler/platform lacks strcasecmp() (e.g., DG/UX, UNIXWARE, VXWORKS)])

AH_TEMPLATE([ACE_LACKS_STRRECVFD],[Platform doesn't define struct strrecvfd.])

AH_TEMPLATE([ACE_LACKS_SYSCALL],[Platform doesn't have syscall() prototype])

AH_TEMPLATE([ACE_LACKS_SYS_TYPES_H],[Platform lacks <sys/types.h> header file])

AH_TEMPLATE([ACE_LACKS_T_ERRNO],[Header files lack t_errno for TLI])

AH_TEMPLATE([ACE_LACKS_TCP_H],[Platform doesn't have netinet/tcp.h])

AH_TEMPLATE([ACE_LACKS_TCP_NODELAY],[OS does not support TCP_NODELAY])

AH_TEMPLATE([ACE_LACKS_THREAD_PROCESS_SCOPING],
[Platform lacks pthread_attr_setscope()])

AH_TEMPLATE([ACE_LACKS_THREAD_STACK_SIZE],
[Platform lacks pthread_attr_setstacksize() (e.g., Linux pthreads)])

AH_TEMPLATE([ACE_LACKS_TIMEDWAIT_PROTOTYPES],
[MIT pthreads platform lacks the timedwait prototypes])

AH_TEMPLATE([ACE_LACKS_TIMESPEC_T],
[Platform does not define timepec_t as a typedef for struct timespec.])

AH_TEMPLATE([ACE_LACKS_TRUNCATE],
[Platform doesn't have truncate() (e.g., vxworks)])

AH_TEMPLATE([ACE_LACKS_GETPGID_PROTOTYPE],
[Platform/compiler lacks the getpgid() prototype])

AH_TEMPLATE([ACE_LACKS_SETREGID_PROTOTYPE],
[Platform/compiler lacks the setregid() prototype])

AH_TEMPLATE([ACE_LACKS_SETREUID_PROTOTYPE],
[Platform/compiler lacks the setreuid() prototype])

AH_TEMPLATE([ACE_LACKS_STRPTIME_PROTOTYPE],
[Platform/compiler lacks the strptime() prototype])

AH_TEMPLATE([ACE_LACKS_STRTOK_R_PROTOTYPE],
[Platform/compiler lacks the strtok_r() prototype])

AH_TEMPLATE([ACE_LACKS_LSEEK64_PROTOTYPE],
[Platform/compiler lacks the lseek64() prototype.  This should not
   be defined if ACE_LACKS_LLSEEK_PROTOTYPE is defined.])

AH_TEMPLATE([ACE_LACKS_LLSEEK_PROTOTYPE],
[Platform/compiler lacks the llseek() prototype.  This should not
   be defined if ACE_LACKS_LSEEK64_PROTOTYPE is defined.])

AH_TEMPLATE([ACE_LACKS_PREAD_PROTOTYPE],
[Platform/compiler lacks the pread() and pwrite() prototypes])

AH_TEMPLATE([ACE_LACKS_UALARM_PROTOTYPE],
[Platform/compiler lacks the ualarm() prototype (e.g., Solaris)])

AH_TEMPLATE([ACE_LACKS_CHAR_RIGHT_SHIFTS],
[Compiler does not have any istream operator>> for chars, u_chars, or
   signed chars.])

AH_TEMPLATE([ACE_LACKS_CHAR_STAR_RIGHT_SHIFTS],
[Compiler does not have operator>> (istream &, u_char *) or
   operator>> (istream &, signed char *)])

AH_TEMPLATE([ACE_LACKS_UCONTEXT_H],[Platform lacks the ucontext.h file])

AH_TEMPLATE([ACE_LACKS_UNISTD_H],
[Platform lacks the unistd.h file (e.g., VxWorks and Win32) ])

AH_TEMPLATE([ACE_LACKS_UNIX_DOMAIN_SOCKETS],
[ACE platform has no UNIX domain sockets])

AH_TEMPLATE([ACE_LACKS_UNIX_SIGNALS],
[Platform lacks full signal support (e.g., Win32 and Chorus).])

AH_TEMPLATE([ACE_LACKS_UTSNAME_T],
[Platform lacks struct utsname (e.g., Win32 and VxWorks)])

AH_TEMPLATE([ACE_LACKS_WCHAR_T],[Platform lacks wchar_t typedef])

AH_TEMPLATE([ACE_LACKS_WILDCARD_BIND],
[The bind() call will not select the port if it's 0.])

AH_TEMPLATE([ACE_MAIN],
[Renames "main (int, char *[])", for platforms such as g++/VxWorks
   that don't allow main.  Requires the use of
   ACE_HAS_NONSTATIC_OBJECT_MANAGER.])

AH_TEMPLATE([ACE_MT_SAFE],[Compile using multi-thread libraries])

AH_TEMPLATE([ACE_NDEBUG],[Turns off debugging features])

AH_TEMPLATE([ACE_NEEDS_DEV_IO_CONVERSION],
[Necessary with some compilers to pass ACE_TTY_IO as parameter to
   DEV_Connector.])

AH_TEMPLATE([ACE_NEEDS_HUGE_THREAD_STACKSIZE],
[Required by platforms with small default stacks.])

AH_TEMPLATE([ACE_NEEDS_LWP_PRIO_SET],
[OS has LWPs, and when the priority of a bound thread is set, then
   the LWP priority must be set also.])

AH_TEMPLATE([ACE_LACKS_READV],
[Platform doesn't define readv, so use our own])

AH_TEMPLATE([ACE_LACKS_WRITEV],
[Platform doesn't define writev, so use our own])

AH_TEMPLATE([ACE_NEEDS_REGEXPR_H],
[Platform needs regexpr.h for regular expression support])

AH_TEMPLATE([ACE_NEEDS_SCHED_H],
[Platform needs to #include <sched.h> to get thread scheduling defs.])

AH_TEMPLATE([ACE_LACKS_SYSTIME_H],
[<time.h> doesn't automatically #include <sys/time.h>])

AH_TEMPLATE([ACE_NEW_THROWS_EXCEPTIONS],
[Compiler's 'new' throws exception on failure (ANSI C++ behavior).])

AH_TEMPLATE([ACE_NLOGGING],
[Turns off the LM_DEBUG and LM_ERROR logging macros...])

AH_TEMPLATE([ACE_NTRACE],[Turns off the tracing feature.])

AH_TEMPLATE([ACE_PAGE_SIZE],
[Defines the page size of the system (not used on Win32 or with
   ACE_HAS_GETPAGESIZE).])

AH_TEMPLATE([ACE_REDEFINES_XTI_FUNCTIONS],
[Platform redefines the t_... names (UnixWare)])

AH_TEMPLATE([ACE_SELECT_USES_INT],
[Platform uses int for select() rather than fd_set])

AH_TEMPLATE([ACE_TEMPLATES_REQUIRE_PRAGMA],
[Compiler's template mechanism must use a pragma.  This is used for
   AIX's C++ compiler.])

AH_TEMPLATE([ACE_TEMPLATES_REQUIRE_SOURCE],
[Compiler's template mechanim must see source code (i.e., .cpp
   files).  This is used for GNU G++.])

AH_TEMPLATE([ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION],
[Compiler's template instantiation mechanism supports the use of
   explicit C++ specializations for all used templates. This is also
   used for GNU G++ if you don't use the "repo" patches.])

AH_TEMPLATE([ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA],
[Compiler's template instantiation mechanism supports the use of
   "#pragma instantiate".  Edison Design Group compilers, e.g., SGI
   C++ and Green Hills 1.8.8 and later, support this.])

AH_TEMPLATE([ACE_NEEDS_FUNC_DEFINITIONS],
[Compiler requires a definition for a "hidden" function, e.g., a
   private, unimplemented copy constructor or assignment operator.
   The SGI C++ compiler needs this, in template classes, with
   ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA.])

AH_TEMPLATE([ACE_TLI_TCP_DEVICE],
[Device the platform uses for TCP on TLI.  Only needed if not
   /dev/tcp.])

AH_TEMPLATE([ACE_USE_POLL],
[The OS/platform supports the poll() event demultiplexor])

AH_TEMPLATE([ACE_POLL_IS_BROKEN],[Platform has broken poll()])

AH_TEMPLATE([ACE_USES_ASM_SYMBOL_IN_DLSYM],
[Platform uses assembly symbols instead of C symbols in dlsym()])

AH_TEMPLATE([ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB],
[Platform has its standard c++ library in the namespace std.])

AH_TEMPLATE([HAVE_RESTARTABLE_SYSCALLS],[])

])  dnl End ACE_PREP_CONFIG_HEADER
