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

#if !defined (ACE_ACE_H)
#define ACE_ACE_H

#include "ace/OS.h"
#include "ace/Version.h"

// Forward declarations.
class ACE_Time_Value;
class ACE_Thread_Manager;
class ACE_Reactor;
class ACE_Event_Handler;
class ACE_INET_Addr;

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
public:
  // = ACE version information.
  static u_int major_version ();
  // E.g., the "4" in ACE 4.3.18.

  static u_int minor_version ();
  // E.g., the "3" in ACE 4.3.18.

  static u_int beta_version ();
  // E.g., the "18" in ACE 4.3.18.

  // = Recv operations that factor out differences between Win32 and UNIX.
  static ssize_t recv (ACE_HANDLE handle, 
		       void *buf, 
		       size_t len, 
		       int flags);
  // Receive up to <len> bytes into <buf> from <handle> (uses the
  // <ACE_OS::recv> call).

  static ssize_t recv (ACE_HANDLE handle, 
		       void *buf, 
		       size_t len);
  // Receive up to <len> bytes into <buf> from <handle> (uses the
  // <ACE_OS::read> system call on UNIX and the <ACE_OS::recv> call on
  // Win32).

  // = Recv operations that receive exactly n bytes.
  static ssize_t recv_n (ACE_HANDLE handle, 
			 void *buf, 
			 size_t len, 
			 int flags);
  // Receive <len> bytes into <buf> from <handle> (uses the
  // <ACE_OS::recv> call).  If <handle> is set to non-blocking mode
  // this call will poll until all <len> bytes are received.

  static ssize_t recv_n (ACE_HANDLE handle, 
			 void *buf, 
			 size_t len);
  // Receive <len> bytes into <buf> from <handle> (uses the
  // <ACE_OS::read> system call on UNIX and the <ACE_OS::recv> call on
  // Win32).  If <handle> is set to non-blocking mode this call will
  // poll until all <len> bytes are received.

  // = Timed <recv> operations.
  static ssize_t recv (ACE_HANDLE handle, 
		       void *buf, 
		       size_t len, 
		       int flags,
		       const ACE_Time_Value *timeout);
  // Wait up to <timeout> amount of time to receive up to <len> bytes
  // into <buf> from <handle> (uses the <ACE_OS::recv> call).  The
  // <timeout> indicates how long to blocking trying to receive.  If
  // <timeout> == 0, the caller will block until action is possible,
  // else will wait until the relative time specified in *<timeout>
  // elapses).  If <recv> times out a -1 is returned with <errno ==
  // ETIME>.  If it succeeds the number of bytes received is returned.

  static ssize_t recv (ACE_HANDLE handle, 
		       void *buf, 
		       size_t len, 
		       const ACE_Time_Value *timeout);
  // Wait up to <timeout> amount of time to receive up to <len> bytes
  // into <buf> from <handle> (uses the <ACE_OS::read> call).  The
  // <timeout> indicates how long to blocking trying to receive.  If
  // <timeout> == 0, the caller will block until action is possible,
  // else will wait until the relative time specified in *<timeout>
  // elapses).  If <recv> times out a -1 is returned with <errno ==
  // ETIME>.  If it succeeds the number of bytes received is returned.

  static ssize_t recvmsg (ACE_HANDLE handle,
			  struct msghdr *msg,
			  int flags,
			  const ACE_Time_Value *timeout);
  // Wait up to <timeout> amount of time to receive <msg> from
  // <handle> (uses the <ACE_OS::recvmsg> call).  The <timeout>
  // indicates how long to blocking trying to receive.  If <timeout>
  // == 0, the caller will block until action is possible, else will
  // wait until the relative time specified in *<timeout> elapses).
  // If <recvmsg> times out a -1 is returned with <errno == ETIME>.
  // If it succeeds the number of bytes received is returned.

  static ssize_t recvfrom (ACE_HANDLE handle,
			   char *buf,
			   int len, 
			   int flags,
			   struct sockaddr *addr,
			   int *addrlen,
			   const ACE_Time_Value *timeout);
  // Wait up to <timeout> amount of time to recv up to <len> bytes
  // into <buf> from <handle> (uses the <ACE_OS::recvfrom> call).  The
  // <timeout> indicates how long to blocking trying to recv.  If
  // <timeout> == 0, the caller will block until action is possible,
  // else will wait until the relative time specified in *<timeout>
  // elapses).  If <recvfrom> times out a -1 is returned with <errno
  // == ETIME>.  If it succeeds the number of bytes received is
  // returned.

  static ssize_t recv_n (ACE_HANDLE handle, 
			 void *buf, 
			 size_t len, 
			 int flags,
			 const ACE_Time_Value *timeout);
  // Try to recv exactly <len> bytes into <buf> from <handle> (uses
  // the <ACE_OS::recv> call).  The <timeout> indicates how long to
  // blocking trying to receive.  If <timeout> == 0, the caller will
  // block until action is possible, else will wait until the relative
  // time specified in *<timeout> elapses).  If <recv> blocks for
  // longer than <timeout> the number of bytes actually read is
  // returned with <errno == ETIME>.  If a timeout does not occur,
  // <recv_n> return <len> (i.e., the number of bytes requested to be
  // read).

  // = Send operations that factor out differences between Win32 and UNIX.
  static ssize_t send (ACE_HANDLE handle, 
		       const void *buf, 
		       size_t len, 
		       int flags);
  // Send up to <len> bytes into <buf> from <handle> (uses the
  // <ACE_OS::send> call).

  static ssize_t send (ACE_HANDLE handle, 
		       const void *buf, 
		       size_t len);
  // Send up to <len> bytes into <buf> from <handle> (uses the
  // <ACE_OS::write> system call on UNIX and the <ACE_OS::send> call
  // on Win32).

  // = Send operations that send exactly n bytes.
  static ssize_t send_n (ACE_HANDLE handle, 
			 const void *buf, 
			 size_t len, 
			 int flags);
  // Send <len> bytes from <buf> to <handle> (uses the <ACE_OS::send>
  // system call).  If <handle> is set to non-blocking mode this call
  // will poll until all <len> bytes are sent.

  static ssize_t send_n (ACE_HANDLE handle, 
			 const void *buf, 
			 size_t len);
  // Send <len> bytes from <buf> to <handle> (uses the <ACE_OS::write>
  // system call on UNIX and the <ACE_OS::recv> call on Win32).  If
  // <handle> is set to non-blocking mode this call will poll until
  // all <len> bytes are sent.

  // = Timed <send> operations.
  static ssize_t send (ACE_HANDLE handle, 
		       const void *buf, 
		       size_t len, 
		       const ACE_Time_Value *timeout);
  // Wait up to <timeout> amount of time to send up to <len> bytes
  // into <buf> from <handle> (uses the <ACE_OS::write> system call on
  // UNIX and the <ACE_OS::send> call on Win32).  The <timeout>
  // indicates how long to blocking trying to send.  If <timeout> ==
  // 0, the caller will block until action is possible, else will wait
  // until the relative time specified in *<timeout> elapses).  If
  // <send> times out a -1 is returned with <errno == ETIME>.  If it
  // succeeds the number of bytes sent is returned.

  static ssize_t send (ACE_HANDLE handle, 
		       const void *buf, 
		       size_t len, 
		       int flags,
		       const ACE_Time_Value *timeout);
  // Wait up to <timeout> amount of time to send up to <len> bytes
  // into <buf> from <handle> (uses the <ACE_OS::send> call).  The
  // <timeout> indicates how long to blocking trying to send.  If
  // <timeout> == 0, the caller will block until action is possible,
  // else will wait until the relative time specified in *<timeout>
  // elapses).  If <send> times out a -1 is returned with <errno ==
  // ETIME>.  If it succeeds the number of bytes sent is returned.

  static ssize_t sendmsg (ACE_HANDLE handle, 
			  const struct msghdr *msg, 
			  int flags,
			  const ACE_Time_Value *timeout);
  // Wait up to <timeout> amount of time to send the <msg> to <handle>
  // (uses the <ACE_OS::sendmsg> call).  The <timeout> indicates how
  // long to blocking trying to send.  If <timeout> == 0, the caller
  // will block until action is possible, else will wait until the
  // relative time specified in *<timeout> elapses).  If <sendmsg>
  // times out a -1 is returned with <errno == ETIME>.  If it succeeds
  // the number of bytes sent is returned.

  static ssize_t sendto (ACE_HANDLE handle,
			 const char *buf,
			 int len, 
			 int flags,
			 const struct sockaddr *addr,
			 int addrlen,
			 const ACE_Time_Value *timeout);
  // Wait up to <timeout> amount of time to send up to <len> bytes
  // into <buf> from <handle> (uses the <ACE_OS::sendto> call).  The
  // <timeout> indicates how long to blocking trying to send.  If
  // <timeout> == 0, the caller will block until action is possible,
  // else will wait until the relative time specified in *<timeout>
  // elapses).  If <sendto> times out a -1 is returned with <errno ==
  // ETIME>.  If it succeeds the number of bytes sent is returned.

  static ssize_t send_n (ACE_HANDLE handle, 
			 const void *buf, 
			 size_t len, 
			 int flags,
			 const ACE_Time_Value *timeout);
  // Try to send exactly <len> bytes into <buf> from <handle> (uses
  // the <ACE_OS::send> call).  The <timeout> indicates how long to
  // blocking trying to send.  If <timeout> == 0, the caller will
  // block until action is possible, else will wait until the relative
  // time specified in *<timeout> elapses).  If <send> blocks for
  // longer than <timeout> the number of bytes actually sent is
  // returned with <errno == ETIME>.  If a timeout does not occur,
  // <send_n> return <len> (i.e., the number of bytes requested to be
  // sent).

  // = Timed Scatter-read and gather-write functions.

  static ssize_t writev (ACE_HANDLE handle, 
			 const struct iovec* iov,
			 int iovcnt,
			 const ACE_Time_Value *timeout);
  // Send <iovcnt> <iovec> structs to <handle> (uses the
  // <ACE_OS::writev> call).  If <timeout> == 0, the caller will block
  // until action is possible, else will wait until the relative time
  // specified in *<timeout> elapses).  If <writev> times out a -1 is
  // returned with <errno == ETIME>.  If it succeeds the number of
  // bytes written is returned.

  static ssize_t readv (ACE_HANDLE handle,
			struct iovec *iov,
			int iovcnt,
			const ACE_Time_Value *timeout);
  // Read <iovcnt> <iovec> structs from <handle> (uses the
  // <ACE_OS::readv> call).  If <timeout> == 0, the caller will block
  // until action is possible, else will wait until the relative time
  // specified in *<timeout> elapses).  If <readv> times out a -1 is
  // returned with <errno == ETIME>.  If it succeeds the number of
  // bytes receieved is returned.

  static ssize_t send (ACE_HANDLE handle, size_t n, ...);
  // Send varargs messages to the <handle> using <writev>.

  static ssize_t recv (ACE_HANDLE handle, size_t n, ...);
  // Recv varargs messages to the <handle> using <readv>.

  // = File system I/O functions.

  // These encapsulate differences between UNIX and Win32 and also
  // send and recv exactly n bytes.

  static ssize_t read_n (ACE_HANDLE handle, 
			 void *buf, 
			 size_t len);
  // Receive <len> bytes into <buf> from <handle> (uses the
  // <ACE_OS::read> call, which uses the <read> system call on UNIX
  // and the <ReadFile> call on Win32).  If <handle> is set to
  // non-blocking mode this call will poll until all <len> bytes are
  // received.

  static ssize_t write_n (ACE_HANDLE handle, 
			  const void *buf, 
			  size_t len);
  // Send <len> bytes from <buf> to <handle> (uses the <ACE_OS::write>
  // calls, which is uses the <write> system call on UNIX and the
  // <WriteFile> call on Win32).  If <handle> is set to non-blocking
  // mode this call will poll until all <len> bytes are sent.

  // = Socket connection establishment calls.

  static int bind_port (ACE_HANDLE handle);
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
  static char *strenvdup (const char *str);
  // Return a dynamically allocated duplicate of <str>, substituting
  // the environment variable if <str[0] == '$'>.  Note that the
  // pointer is allocated with <ACE_OS::malloc> and must be freed by
  // <ACE_OS::free>

  static char *strecpy (char *s, const char *t);
  // Copies <t> to <s>, returning a pointer to the end of the copied
  // region (rather than the beginning, a la <strcpy>.

  static size_t strrepl (char *s, char search, char replace);
  // Replace all instances of <search> in <s> with <replace>.  Returns
  // the number of replacements made.

  static const char *execname (const char *pathname);
  // On Win32 returns <pathname> if it already ends in ".exe,"
  // otherwise returns a dynamically allocated buffer containing
  // "<pathname>.exe".  Always returns <pathname> on UNIX.

  static const char *basename (const char *pathname, char delim);
  // Returns the "basename" of a <pathname>.

#if defined (ACE_HAS_UNICODE)
  static const wchar_t *basename (const wchar_t *pathname, wchar_t delim);
  // Returns the "basename" of a <pathname>.
#endif /* ACE_HAS_UNICODE */

  static char *timestamp (char date_and_time[], int time_len);
  // Returns the current timestamp in the form
  // "hour:minute:second:microsecond."  The month, day, and year are
  // also stored in the beginning of the date_and_time array.  Returns
  // 0 if unsuccessful, else returns pointer to beginning of the
  // "time" portion of <day_and_time>.

  static int daemonize (const char pathname[] = "/");
  // Become a daemon process.

  // = Methods for searching and opening shared libraries.

  static int ldfind (const char *filename, 
		     char *pathname, 
		     size_t maxlen);
  // Finds the file <filename> either using an absolute path or using
  // a relative path in conjunction with ACE_LD_SEARCH_PATH (e.g.,
  // $LD_LIBRARY_PATH on UNIX or $PATH on Win32).  This function will
  // add appropriate suffix (e.g., .dll on Win32 or .so on UNIX)
  // according to the OS platform.  In addition, this function will
  // apply the appropriate prefix (e.g., "lib" on UNIX and "" on
  // Win32) if the <filename> doesn't match directly.

  static FILE *ldopen (const char *filename, const char *type);
  // Uses <ldopen> to locate and open the appropriate <filename> and
  // returns a pointer to the file, else it returns a NULL
  // pointer. <type> specifies how the file should be open.

  // = Shield us from Win32's inability to select on STDIN.

  static void *read_adapter (void *event_handler);
  // Used to read from non-socket ACE_HANDLEs in our own thread to
  // work around Win32 limitations that don't allow us to select() on
  // non-sockets (such as ACE_STDIN).  This is commonly used in
  // situations where the Reactor is used to demultiplex read events
  // on ACE_STDIN on UNIX.  Note that <event_handler> must be a
  // subclass of <ACE_Event_Handler>.  If the <get_handle> method of
  // this event handler returns <ACE_INVALID_HANDLE> we default to
  // reading from ACE_STDIN.

  static int register_stdin_handler (ACE_Event_Handler *eh,
				     ACE_Reactor *reactor,
				     ACE_Thread_Manager *thr_mgr,
				     int flags = THR_DETACHED);
  // Abstracts away from the differences between Win32 and ACE with
  // respect to reading from ACE_STDIN (which is non-select()'able on
  // Win32.

  // = Miscelleous functions.
  static size_t	round_to_pagesize (off_t length);
  // Rounds the request to a multiple of the page size.

  static int format_hexdump (const char *buffer, int size, char *obuf,
                             int obuf_sz);
  // Format buffer into printable format.  This is useful for
  // debugging.

  static u_long hash_pjw (const char *str);
  // Computes the hash value of <str> using the ``Hash PJW'' routine.

  static u_long hash_pjw (const ACE_USHORT16 *str);
  // Computes the hash value of <str> using the ``Hash PJW'' routine
  // (works for UNICODE strings).

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

  static const char *sock_error (int error);
  // Returns a string containing the error message corresponding to a
  // WinSock error.  This works around an omission in the Win32 API...

  static int process_active (pid_t pid);
  // Checks if process with <pid> is still alive.  Returns 1 if it is
  // still alive, 0 if it isn't alive, and -1 if something weird
  // happened.

  static int terminate_process (pid_t pid);
  // Terminate the process with id <pid>.  Note that this call is
  // potentially dangerous to use since the process being terminated
  // may not have a chance to cleanup before it shuts down.

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

private:
  ACE (void);
  // Ensure we can't define an instance of this class...

  static int enter_recv_timedwait (ACE_HANDLE handle,
				   const ACE_Time_Value *timeout,
				   int &val);
  // Wait for <timeout> before proceeding to a <recv> operation.
  // <val> keeps track of whether we're in non-blocking mode or not.

  static void leave_recv_timedwait (ACE_HANDLE handle,
				    const ACE_Time_Value *timeout,
				    int val);
  // Cleanup after a <recv> operation (e.g., restore the appropriate
  // non-blocking status of <handle>).

  static int enter_send_timedwait (ACE_HANDLE handle,
				   const ACE_Time_Value* timeout,
				   int &val);
  // Wait for <timeout> before proceeding to a <send> operation.
  // <val> keeps track of whether we're in non-blocking mode or not.

  static void leave_send_timedwait (ACE_HANDLE handle,
				    const ACE_Time_Value *timeout,
				    int val);
  // Cleanup after the <send> operation (e.g., restore the appropriate
  // non-blocking status of <handle>).

  static size_t pagesize_;
  // Size of a VM page.
};

#include "ace/ACE.i"

#endif  /* ACE_ACE_H */
