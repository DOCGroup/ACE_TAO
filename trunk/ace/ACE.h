/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    ACE.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

// This #include must come first to avoid recursive include problems.
#include "ace/OS.h"

#ifndef ACE_ACE_H
#define ACE_ACE_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declarations.
class ACE_Time_Value;
class ACE_INET_Addr;
class ACE_Message_Block;

class ACE_Export ACE
{
  // = TITLE
  //     Contains value added ACE methods that extend the behavior
  //     of the UNIX and Win32 OS calls.
  //
  // = DESCRIPTION
  //     This class consolidates all these ACE static methods in a
  //     single place in order to manage the namespace better.  These
  //     methods are put here rather than in ACE_OS in order to
  //     separate concerns.

  ACE_CLASS_IS_NAMESPACE (ACE);
public:
  // Initialize ACE library services.
  static int init (void);
  // Returns 0 on success, -1 on failure, and 1 if it had already been called.

  // Shut down ACE library services.
  static int fini (void);
  // Returns 0 on success, -1 on failure, and 1 if it had already been called.

  // = ACE version information.
  static u_int major_version (void);
  // E.g., the "4" in ACE 4.3.19.

  static u_int minor_version (void);
  // E.g., the "3" in ACE 4.3.19.

  static u_int beta_version (void);
  // E.g., the "19" in ACE 4.3.19.  Returns 0 for "stable" (non-beta) releases.

  // = C++ compiler version information.
  static const char* compiler_name (void);
  // E.g., the "SunPro C++" in SunPro C++ 4.32.0

  static u_int compiler_major_version (void);
  // E.g., the "4" in SunPro C++ 4.32.0

  static u_int compiler_minor_version (void);
  // E.g., the "32" in SunPro C++ 4.32.0

  static u_int compiler_beta_version (void);
  // E.g., the "0" in SunPro C++ 4.32.0

  static int out_of_handles (int error);
  // Check if error indicates the process being out of handles (file
  // descriptors).

  // = I/O operations.

  // = Notes on common parameters:
  //
  // <handle> is the connected endpoint that will be used for I/O.
  //
  // <buf> is the buffer to write from or receive into.
  //
  // <len> is the number of bytes to transfer.
  //
  // The <timeout> parameter in the following methods indicates how
  // long to blocking trying to transfer data.  If <timeout> == 0,
  // then the call behaves as a normal send/recv call, i.e., for
  // blocking sockets, the call will block until action is possible;
  // for non-blocking sockets, EWOULDBLOCK will be returned if no
  // action is immediately possible.
  //
  // If <timeout> != 0, the call will wait until the relative time
  // specified in *<timeout> elapses.
  //
  // Errors are reported by -1 and 0 return values.  If the operation
  // times out, -1 is returned with <errno == ETIME>.  If it succeeds
  // the number of bytes transferred is returned.
  //
  // The "_n" I/O methods keep looping until all the data has been
  // transferred.  These methods also work for sockets in non-blocking
  // mode i.e., they keep looping on EWOULDBLOCK.  <timeout> is used
  // to make sure we keep making progress, i.e., the same timeout
  // value is used for every I/O operation in the loop and the timeout
  // is not counted down.  If the transfer times out, the number of
  // bytes transferred so far are returned.
  //
  // Methods with <iovec> parameter are I/O vector variants of the I/O
  // operations.

  static ssize_t recv (ACE_HANDLE handle,
                       void *buf,
                       size_t len,
                       int flags,
                       const ACE_Time_Value *timeout = 0);

  static ssize_t recv (ACE_HANDLE handle,
                       void *buf,
                       size_t len,
                       const ACE_Time_Value *timeout = 0);

  static ssize_t recvmsg (ACE_HANDLE handle,
                          struct msghdr *msg,
                          int flags,
                          const ACE_Time_Value *timeout = 0);

  static ssize_t recvfrom (ACE_HANDLE handle,
                           char *buf,
                           int len,
                           int flags,
                           struct sockaddr *addr,
                           int *addrlen,
                           const ACE_Time_Value *timeout = 0);

  static ssize_t recv_n (ACE_HANDLE handle,
                         void *buf,
                         size_t len,
                         int flags,
                         const ACE_Time_Value *timeout = 0);

  static ssize_t recv_n (ACE_HANDLE handle,
                         void *buf,
                         size_t len,
                         const ACE_Time_Value *timeout = 0);

  static ssize_t recv (ACE_HANDLE handle, size_t n, ...);
  // Varargs variant.

  static ssize_t recvv (ACE_HANDLE handle,
                        iovec *iov,
                        int iovcnt,
                        const ACE_Time_Value *timeout = 0);

  static ssize_t recvv_n (ACE_HANDLE handle,
                          iovec *iov,
                          int iovcnt,
                          const ACE_Time_Value *timeout = 0);

  static ssize_t recv_n (ACE_HANDLE handle,
                         ACE_Message_Block *message_block,
                         const ACE_Time_Value *timeout = 0);

  static ssize_t send (ACE_HANDLE handle,
                       const void *buf,
                       size_t len,
                       const ACE_Time_Value *timeout = 0);

  static ssize_t send (ACE_HANDLE handle,
                       const void *buf,
                       size_t len,
                       int flags,
                       const ACE_Time_Value *timeout = 0);

  static ssize_t sendmsg (ACE_HANDLE handle,
                          const struct msghdr *msg,
                          int flags,
                          const ACE_Time_Value *timeout = 0);

  static ssize_t sendto (ACE_HANDLE handle,
                         const char *buf,
                         int len,
                         int flags,
                         const struct sockaddr *addr,
                         int addrlen,
                         const ACE_Time_Value *timeout = 0);

  static ssize_t send_n (ACE_HANDLE handle,
                         const void *buf,
                         size_t len,
                         int flags,
                         const ACE_Time_Value *timeout = 0);

  static ssize_t send_n (ACE_HANDLE handle,
                         const void *buf,
                         size_t len,
                         const ACE_Time_Value *timeout = 0);

  static ssize_t send (ACE_HANDLE handle, size_t n, ...);
  // Varargs variant.

  static ssize_t sendv (ACE_HANDLE handle,
                        const iovec *iov,
                        int iovcnt,
                        const ACE_Time_Value *timeout = 0);

  static ssize_t sendv_n (ACE_HANDLE handle,
                          const iovec *iov,
                          int iovcnt,
                          const ACE_Time_Value *timeout = 0);

  static ssize_t send_n (ACE_HANDLE handle,
                         const ACE_Message_Block *message_block,
                         const ACE_Time_Value *timeout = 0);

  // = File system I/O functions (these don't support timeouts).

  static ssize_t read_n (ACE_HANDLE handle,
                         void *buf,
                         size_t len);

  static ssize_t write_n (ACE_HANDLE handle,
                          const void *buf,
                          size_t len);

  static ssize_t readv_n (ACE_HANDLE handle,
                          iovec *iov,
                          int iovcnt);

  static ssize_t writev_n (ACE_HANDLE handle,
                           const iovec *iov,
                           int iovcnt);

  // = Socket connection establishment calls.

  static int bind_port (ACE_HANDLE handle,
                        ACE_UINT32 ip_addr = INADDR_ANY);
  // Bind a new unused port to <handle>.

  static int get_bcast_addr (ACE_UINT32 &bcast_addr,
                             const char *hostname = 0,
                             ACE_UINT32 host_addr = 0,
                             ACE_HANDLE handle = ACE_INVALID_HANDLE);
  // Get our broadcast address based on our <host_addr>.  If
  // <hostname> is non-0 we'll use it to determine our IP address.  If
  // <handle> is not <ACE_INVALID_HANDLE> then we'll use this to
  // determine our broadcast address, otherwise we'll have to create a
  // socket internally (and free it).  Returns -1 on failure and 0 on
  // success.

  static int get_ip_interfaces (size_t &count,
                                ACE_INET_Addr *&addr_array);
  // Return count and array of all configured IP interfaces on this
  // host, rc = 0 on success (count == number of interfaces else -1).
  // Caller is responsible for calling delete [] on <addr_array>.

  static int count_interfaces (ACE_HANDLE handle,
                               size_t &how_many);
  // Helper routine for get_ip_interfaces, differs by UNIX platform so
  // put into own subroutine.  perform some ioctls to retrieve ifconf
  // list of ifreq structs.

  static ACE_HANDLE get_handle (void);
  // Routine to return a handle from which <ioctl> requests can be
  // made.  Caller must <close> the handle.

  static int handle_timed_accept (ACE_HANDLE listener,
                                  ACE_Time_Value *timeout,
                                  int restart);
  // Wait up to <timeout> amount of time to passively establish a
  // connection.  This method doesn't perform the <accept>, it just
  // does the timed wait...

  static ACE_HANDLE handle_timed_complete (ACE_HANDLE listener,
                                           ACE_Time_Value *timeout,
                                           int is_tli = 0);
  // Wait up to <timeout> amount of time to complete an actively
  // established non-blocking connection.  If <is_tli> is non-0 then
  // we are being called by a TLI wrapper (which behaves slightly
  // differently from a socket wrapper).

  // = Operations on HANDLEs.

  static ACE_HANDLE handle_timed_open (ACE_Time_Value *timeout,
                                       LPCTSTR name,
                                       int flags,
                                       int perms);
  // Wait up to <timeout> amount of time to actively open a device.
  // This method doesn't perform the <connect>, it just does the timed
  // wait...

  // = Set/get/clear various flags related to I/O HANDLE.
  static int set_flags (ACE_HANDLE handle,
                        int flags);
  // Set flags associated with <handle>.

  static int clr_flags (ACE_HANDLE handle,
                        int flags);
  // Clear flags associated with <handle>.

  static int get_flags (ACE_HANDLE handle);
  // Return the current setting of flags associated with <handle>.

  static int set_handle_limit (int new_limit = -1);
  // Reset the limit on the number of open handles.  If <new_limit> ==
  // -1 set the limit to the maximum allowable.  Otherwise, set it to
  // be the value of <new_limit>.

  static int max_handles (void);
  // Returns the maximum number of open handles currently permitted in
  // this process.  This maximum may be extended using
  // <ACE::set_handle_limit>.

  // = String functions
#if !defined (ACE_HAS_WINCE)
  static ASYS_TCHAR *strenvdup (const ASYS_TCHAR *str);
  // Return a dynamically allocated duplicate of <str>, substituting
  // the environment variable if <str[0] == '$'>.  Note that the
  // pointer is allocated with <ACE_OS::malloc> and must be freed by
  // <ACE_OS::free>.
#endif /* ACE_HAS_WINCE */

  static char *strecpy (char *des, const char *src);
  // Copies <src> to <des>, returning a pointer to the end of the
  // copied region, rather than the beginning, as <strcpy> does.

  static const char *strend (const char *);
  // Returns a pointer to the "end" of the string, i.e., the character
  // past the '\0'.

  static char *strnew (const char *s);
  // This method is just like <strdup>, except that it uses <operator
  // new> rather than <malloc>.

#   if defined (ACE_WIN32) && defined (ACE_HAS_UNICODE)
  static wchar_t *strnew (const wchar_t *s);
  // This method is just like <strdup>, except that it uses <operator
  // new> rather than <malloc>.
#endif /* ACE_WIN32 && ACE_HAS_UNICODE */

  static char *strndup (const char *str, size_t n);
  // Create a fresh new copy of <str>, up to <n> chars long.  Uses
  // <ACE_OS::malloc> to allocate the new string.

  static char *strnnew (const char *str, size_t n);
  // Create a fresh new copy of <str>, up to <n> chars long.  Uses
  // <ACE_OS::malloc> to allocate the new string.

  static char *strsplit_r (char *s,
                           const char *token,
                           char *&next_start);
  // Splits string <s> into pieces separated by the string <token>.
  // <next_start> is an opaque cookie handed back by the call to store
  // its state for the next invocation, thus making it re-entrant.
  // This operates very similar to Perl's <split> function except that
  // it returns pieces one at a time instead of into an array.

  static size_t strrepl (char *s, char search, char replace);
  // Replace all instances of <search> in <s> with <replace>.  Returns
  // the number of replacements made.

  static const char *execname (const char *pathname);
  // On Win32 returns <pathname> if it already ends in ".exe,"
  // otherwise returns a dynamically allocated buffer containing
  // "<pathname>.exe".  Always returns <pathname> on UNIX.

  static const char *basename (const char *pathname,
                               char delim = ACE_DIRECTORY_SEPARATOR_CHAR_A);
  // Returns the "basename" of a <pathname> separated by <delim>.  For
  // instance, the basename of "/tmp/foo.cpp" is "foo.cpp" when
  // <delim> is '/'.

  static const char *dirname (const char *pathname,
                              char delim = ACE_DIRECTORY_SEPARATOR_CHAR_A);
  // Returns the "dirname" of a <pathname>.  For instance, the dirname
  // of "/tmp/foo.cpp" is "/tmp" when <delim> is '/'.  If <pathname>
  // has no <delim> ".\0" is returned.  This method does not modify
  // <pathname> and is not reentrant.

#if defined (ACE_HAS_UNICODE)
  // A collection of wide string functions.  See above for details.

  static wchar_t *strecpy (wchar_t *s, const wchar_t *t);

  static wchar_t *strsplit_r (wchar_t *s,
                              const wchar_t *token,
                              wchar_t *&next_start);

  static size_t strrepl (wchar_t *s, wchar_t search, wchar_t replace);

  static const wchar_t *execname (const wchar_t *pathname);

  static const wchar_t *basename (const wchar_t *pathname,
                                  wchar_t delim = ACE_DIRECTORY_SEPARATOR_CHAR_W);
  // Returns the "basename" of a <pathname>.
#endif /* ACE_HAS_UNICODE */

  static ASYS_TCHAR *timestamp (ASYS_TCHAR date_and_time[],
                                int time_len);
  // Returns the current timestamp in the form
  // "hour:minute:second:microsecond."  The month, day, and year are
  // also stored in the beginning of the date_and_time array.  Returns
  // 0 if unsuccessful, else returns pointer to beginning of the
  // "time" portion of <day_and_time>.

  static pid_t fork (const char *program_name = "<unknown>",
                     int avoid_zombies = 0);
  // if <avoid_zombies> == 0 call <ACE_OS::fork> directly, else create
  // an orphan process that's inherited by the init process; init
  // cleans up when the orphan process terminates so we don't create
  // zombies.

  static int daemonize (const char pathname[] = "/",
                        int close_all_handles = ACE_DEFAULT_CLOSE_ALL_HANDLES,
                        const char program_name[] = "<unknown>");
  // Become a daemon process using the algorithm in Richard Stevens
  // "Advanced Programming in the UNIX Environment."  If
  // <close_all_handles> is non-zero then all open file handles are
  // closed.

  // = Methods for searching and opening shared libraries.

  static int ldfind (const ASYS_TCHAR *filename,
                     ASYS_TCHAR *pathname,
                     size_t maxlen);
  // Finds the file <filename> either using an absolute path or using
  // a relative path in conjunction with ACE_LD_SEARCH_PATH (e.g.,
  // $LD_LIBRARY_PATH on UNIX or $PATH on Win32).  This function will
  // add appropriate suffix (e.g., .dll on Win32 or .so on UNIX)
  // according to the OS platform.  In addition, this function will
  // apply the appropriate prefix (e.g., "lib" on UNIX and "" on
  // Win32) if the <filename> doesn't match directly.

  static FILE *ldopen (const ASYS_TCHAR *filename,
                       const ASYS_TCHAR *type);
  // Uses <ldopen> to locate and open the appropriate <filename> and
  // returns a pointer to the file, else it returns a NULL
  // pointer. <type> specifies how the file should be open.

  static ACE_HANDLE open_temp_file (const char *name,
                                    int mode,
                                    int perm = 0);
  // Opening the temp file.  File is automagically unlinked when it is
  // closed.  This is useful for have temp files.

  // = Shield us from Win32's inability to select on STDIN.

  // = Miscelleous functions.
  static size_t round_to_pagesize (off_t length);
  // Rounds the request to a multiple of the page size.

  static size_t round_to_allocation_granularity (off_t len);
  // Rounds the request to a multiple of the allocation granularity.

  static int format_hexdump (const char *buffer, int size,
                             ASYS_TCHAR *obuf, int obuf_sz);
  // Format buffer into printable format.  This is useful for
  // debugging.

  static u_long hash_pjw (const char *str);
  // Computes the hash value of <str> using the ``Hash PJW'' routine.

  static u_long hash_pjw (const char *str, size_t len);
  // Computes the hash value of <str> using the ``Hash PJW'' routine.

#if !defined (ACE_HAS_WCHAR_TYPEDEFS_CHAR)
  static u_long hash_pjw (const wchar_t *str);
  // Computes the hash value of <str> using the ``Hash PJW'' routine.

  static u_long hash_pjw (const wchar_t *str, size_t len);
  // Computes the hash value of <str> using the ``Hash PJW'' routine.
#endif /* ! ACE_HAS_WCHAR_TYPEDEFS_CHAR */

#if !defined (ACE_HAS_WCHAR_TYPEDEFS_USHORT)
  static u_long hash_pjw (const ACE_USHORT16 *str);
  // Computes the hash value of <str> using the ``Hash PJW'' routine.

  static u_long hash_pjw (const ACE_USHORT16 *str, size_t len);
  // Computes the hash value of <str> using the ``Hash PJW'' routine.
#endif /* ! ACE_HAS_WCHAR_TYPEDEFS_USHORT */

  static u_long crc32 (const char *str);
  // Computes the ISO 8802-3 standard 32 bits CRC for the string
  // (not for a file).

  static u_long crc32 (const char *buf, ACE_UINT32 len);
  // Computes the ISO 8802-3 standard 32 bits CRC for the given
  // buffer (the length is included in the CRC).

  static u_long gcd (u_long x, u_long y);
  // Euclid's greatest common divisor algorithm.

  static u_long minimum_frame_size (u_long period1, u_long period2);
  // Calculates the minimum enclosing frame size for the given values.

  static u_long is_prime (const u_long n,
                          const u_long min_factor,
                          const u_long max_factor);
  // Function that can burn up noticeable CPU time:  brute-force
  // determination of whether number "n" is prime.  Returns 0 if
  // it is prime, or the smallest factor if it is not prime.  min_factor
  // and max_factor can be used to partition the work among threads.
  // For just one thread, typical values are 2 and n/2.

  static int map_errno (int error);
  // Map troublesome win32 errno values to values that standard C
  // strerr function understands.  Thank you Microsoft.

  static const ASYS_TCHAR *sock_error (int error);
  // Returns a string containing the error message corresponding to a
  // WinSock error.  This works around an omission in the Win32 API...

  static int process_active (pid_t pid);
  // Checks if process with <pid> is still alive.  Returns 1 if it is
  // still alive, 0 if it isn't alive, and -1 if something weird
  // happened.

  static int terminate_process (pid_t pid);
  // Terminate the process abruptly with id <pid>.  On Win32 platforms
  // this uses <TerminateProcess> and on POSIX platforms is uses
  // <kill> with the -9 (SIGKILL) signal, which cannot be caught or
  // ignored.  Note that this call is potentially dangerous to use
  // since the process being terminated may not have a chance to
  // cleanup before it shuts down.

  static void unique_name (const void *object,
                           LPTSTR name,
                           size_t length);
  // This method uses process id and object pointer to come up with a
  // machine wide unique name.  The process ID will provide uniqueness
  // between processes on the same machine. The "this" pointer of the
  // <object> will provide uniqueness between other "live" objects in
  // the same process. The uniqueness of this name is therefore only
  // valid for the life of <object>.

  static u_long log2 (u_long num);
  // Computes the base 2 logarithm of <num>.

  static char nibble2hex (u_int n);
  // Hex conversion utility.

  static u_char hex2byte (char c);
  // Convert a hex character to its byte representation.

  // = Set/get the debug level.
  static char debug (void);
  static void debug (char d);

  static int handle_read_ready (ACE_HANDLE handle,
                                const ACE_Time_Value *timeout);
  // Timed wait for handle to get read ready.

  static int handle_write_ready (ACE_HANDLE handle,
                                const ACE_Time_Value *timeout);
  // Timed wait for handle to get write ready.

  static int handle_exception_ready (ACE_HANDLE handle,
                                     const ACE_Time_Value *timeout);
  // Timed wait for handle to get exception ready.

  static int handle_ready (ACE_HANDLE handle,
                           const ACE_Time_Value *timeout,
                           int read_ready,
                           int write_ready,
                           int exception_ready);
  // Timed wait for handle to get read, write, or exception ready.

  static int enter_recv_timedwait (ACE_HANDLE handle,
                                   const ACE_Time_Value *timeout,
                                   int &val);
  // Wait for <timeout> before proceeding to a <recv> operation.
  // <val> keeps track of whether we're in non-blocking mode or not.

  static int enter_send_timedwait (ACE_HANDLE handle,
                                   const ACE_Time_Value* timeout,
                                   int &val);
  // Wait for <timeout> before proceeding to a <send> operation.
  // <val> keeps track of whether we're in non-blocking mode or not.

  static void record_and_set_non_blocking_mode (ACE_HANDLE handle,
                                                int &val);
  // This makes sure that <handle> is set into non-blocking mode.
  // <val> keeps track of whether were in non-blocking mode or not.

  static void restore_non_blocking_mode (ACE_HANDLE handle,
                                         int val);
  // Cleanup after a timed operation, restore the appropriate
  // non-blocking status of <handle>.

private:

  //
  // = Recv_n helpers
  //

  static ssize_t recv_i (ACE_HANDLE handle,
                         void *buf,
                         size_t len);

  static ssize_t recv_n_i (ACE_HANDLE handle,
                           void *buf,
                           size_t len,
                           int flags);

  static ssize_t recv_n_i (ACE_HANDLE handle,
                           void *buf,
                           size_t len,
                           int flags,
                           const ACE_Time_Value *timeout);

  static ssize_t recv_n_i (ACE_HANDLE handle,
                           void *buf,
                           size_t len);

  static ssize_t recv_n_i (ACE_HANDLE handle,
                           void *buf,
                           size_t len,
                           const ACE_Time_Value *timeout);

  static ssize_t recvv_n_i (ACE_HANDLE handle,
                            iovec *iov,
                            int iovcnt);

  static ssize_t recvv_n_i (ACE_HANDLE handle,
                            iovec *iov,
                            int iovcnt,
                            const ACE_Time_Value *timeout);

  //
  // = Send_n helpers
  //

  static ssize_t send_i (ACE_HANDLE handle,
                         const void *buf,
                         size_t len);

  static ssize_t send_n_i (ACE_HANDLE handle,
                           const void *buf,
                           size_t len,
                           int flags);

  static ssize_t send_n_i (ACE_HANDLE handle,
                           const void *buf,
                           size_t len,
                           int flags,
                           const ACE_Time_Value *timeout);

  static ssize_t send_n_i (ACE_HANDLE handle,
                           const void *buf,
                           size_t len);

  static ssize_t send_n_i (ACE_HANDLE handle,
                           const void *buf,
                           size_t len,
                           const ACE_Time_Value *timeout);

  static ssize_t sendv_n_i (ACE_HANDLE handle,
                            const iovec *iov,
                            int iovcnt);

  static ssize_t sendv_n_i (ACE_HANDLE handle,
                            const iovec *iov,
                            int iovcnt,
                            const ACE_Time_Value *timeout);

  static u_int init_fini_count_;
  // Counter to match <init>/<fini> calls.  <init> must increment it;
  // <fini> must decrement it.  <fini> then does nothing until it
  // reaches 0.

  static size_t pagesize_;
  // Size of a VM page.

  static size_t allocation_granularity_;
  // Size of allocation granularity.

  static u_long crc_table_[];
  // CRC table.

  static const char hex_chars_[];
  // Hex characters.

  static char debug_;
  // Are we debugging ACE?
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/ACE.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#endif  /* ACE_ACE_H */
