/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    SUN_Proactor.h
//
// = AUTHOR
//    Alexander Libman <alibman@baltimore.com>
//
// ============================================================================

#ifndef ACE_SUN_PROACTOR_H
#define ACE_SUN_PROACTOR_H

#include "ace/POSIX_Proactor.h"

#if defined (ACE_HAS_AIO_CALLS) && defined (sun)

#include "sys/asynch.h"    // Sun native aio calls

class ACE_Export ACE_SUN_Proactor : public ACE_POSIX_AIOCB_Proactor
{
  // = TITLE
  //     Implementation of the fast and reliable Proactor 
  //     for SunOS 5.6, 5.7, etc.
  //
  // = DESCRIPTION
  //     This proactor, based on <ACE_POSIX_AIOCB_Proactor>,
  //     works with Sun native interface for aio calls.
  //     POSIX_API          Native SUN_API
  //     aio_read           aioread
  //     aio_write          aiowrite
  //     aio_suspend        aiowait
  //     aio_error          aio_result_t.errno
  //     aio_return         aio_result_t.return
  //   
  //     On Solaris, the Sun <aio*()> native implementation is more
  //     reliable and efficient than POSIX <aio_*()> implementation.
  //     There is a problem of lost RT signals with POSIX, if we start
  //     more than SIGQUEUEMAX asynchronous operations at the same
  //     time.
  //
  //     The Sun <aiocb> it is not the standard POSX <aiocb>, instead,
  //     it has the following structure:
  // 
  //     typedef struct aiocb
  //     {
  //  	   int 		aio_fildes;     File descriptor
  //       void		*aio_buf;       buffer location 
  //       size_t       aio_nbytes;     length of transfer 
  //       off_t        aio_offset;     file offset 
  //       int	        aio_reqprio;    request priority offset 
  //       sigevent     aio_sigevent;   signal number and offset
  //       int 		aio_lio_opcode; listio operation 
  //       aio_result_t	aio_resultp;    results 
  //       int 		aio_state;      state flag for List I/O 
  //       int		aio__pad[1];    extension padding 
  //     };

public:
  virtual Proactor_Type get_impl_type (void);

  virtual ~ACE_SUN_Proactor (void);
  // Destructor.
  
  // @@ Alex, this shouldn't be a magic number, i.e., it should be a
  // constant, such as ACE_MAX_AIO_OPERATIONS.
  ACE_SUN_Proactor (size_t max_aio_operations = 512);
  // Constructor defines max number asynchronous operations that can
  // be started at the same time.

protected:
  virtual int handle_events (ACE_Time_Value &wait_time);
  // Dispatch a single set of events.  If <wait_time> elapses before
  // any events occur, return 0.  Return 1 on success i.e., when a
  // completion is dispatched, non-zero (-1) on errors and errno is
  // set accordingly.

  virtual int handle_events (u_long milli_seconds);
  // Dispatch a single set of events.  If <milli_seconds> elapses
  // before any events occur, return 0. Return 1 if a completion is
  // dispatched. Return -1 on errors.
 
  virtual int handle_events (void);
  // Block indefinitely until at least one event is dispatched.
  // Dispatch a single set of events.  If <wait_time> elapses before
  // any events occur, return 0.  Return 1 on success i.e., when a
  // completion is dispatched, non-zero (-1) on errors and errno is
  // set accordingly.

  virtual int start_aio (ACE_POSIX_Asynch_Result *result, int op);
  // From ACE_POSIX_AIOCB_Proactor.

  ACE_POSIX_Asynch_Result *find_completed_aio (aio_result_t *result,
                                               int &error_status,
                                               int &return_status);
  // Extract the results of aio.
};

#if defined (__ACE_INLINE__)
#include "ace/SUN_Proactor.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_AIO_CALLS && sun */
#endif /* ACE_SUN_PROACTOR_H*/

