/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    FILE.h
//
// = AUTHOR
//    Gerhard Lenzer
// 
// ============================================================================

#if !defined (ACE_FILE_H)
#define ACE_FILE_H

#include "ace/IO_SAP.h"
#include "ace/FILE_Addr.h"

// The following is necessary since many C++ compilers don't support
// typedef'd types inside of classes used as formal template
// arguments... ;-(.  Luckily, using the C++ preprocessor I can hide
// most of this nastiness!

#if defined (ACE_HAS_TEMPLATE_TYPEDEFS)
#define ACE_FILE_CONNECTOR ACE_FILE_Connector
#define ACE_FILE_STREAM ACE_FILE_IO
#else /* TEMPLATES are broken (must be a cfront-based compiler...) */
#define ACE_FILE_CONNECTOR ACE_FILE_Connector, ACE_FILE_Addr
#define ACE_FILE_STREAM ACE_FILE_IO, ACE_FILE_Addr
#endif /* ACE_TEMPLATE_TYPEDEFS */

struct ACE_FILE_Info
{
  mode_t mode_;	
  // mode of file

  nlink_t nlink_;
  // no of links

  off_t size_;
  // size of file
};

class ACE_Export ACE_FILE : public ACE_IO_SAP
  // = TITLE
  //     Defines the member functions for the base class of the
  //     ACE_FILE abstraction. 
{
public:
  int close (void);
  // Close down the ACE_FILE

  int get_info (ACE_FILE_Info *finfo);
  // get informations on the ACE_FILE

  int truncate (off_t length);
  // set filesize to length byte

  off_t position (long offset, int startpos);
  // set the filepointer to the specified position

  off_t position (void);
  // get current filepointer

  void dump (void) const;
  // Dump the state of an object.

  int disable (int signum) const ;
  // Disable signal <signum>
  // This is here to prevent Win32 from
  // disabling SPIPE using socket calls

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_FILE (void);
  // Ensure that this class is an abstract base class 
};

#include "ace/FILE.i"

#endif /* ACE_FILE_H */
