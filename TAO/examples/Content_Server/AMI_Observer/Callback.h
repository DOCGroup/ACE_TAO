// -*- C++ -*-
// $Id$


// ============================================================================
//
// = LIBRARY
//     Content_Server
//
// = FILENAME
//     Callback.h
//
// = DESCRIPTION
//     Header file for the Web_Server::Callback implementation.
//
// = AUTHOR
//     Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef CALLBACK_H
#define CALLBACK_H

#include "ace/pre.h"

#include "ace/FILE_Addr.h"
#include "ace/FILE_IO.h"
#include "Push_Web_ServerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Implement the Web_Server::Callback interface.
class Callback_i :
  public virtual POA_Web_Server::Callback,
  public virtual PortableServer::RefCountServantBase
{
public:

  Callback_i (int * request_count);
  // Constructor

  virtual void next_chunk (const Web_Server::Chunk_Type & chunk,
                           CORBA::Boolean last_chunk,
                           CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // This operation returns the next <chunk> of the file starting at
  // <offset>.  If there are no more bindings, false is returned.

  void metadata (const Web_Server::Metadata_Type & metadata);
  // Set metadata associated with received data.

private:

  ~Callback_i (void);
  // Destructor must be private to ensure that this object is
  // allocated on the heap.

  int metadata_received (void);
  // Returns one if the metadata was received, and zero otherwise.

  int content_received (void);
  // Returns one if the entire content was received, and zero
  // otherwise.

  int get_viewer (char * viewer, size_t length);
  // Get the name of the viewer associated with the file being
  // retrieved.

  int spawn_viewer (void);
  // Spawn an external view to display the retrieved file.

private:

  void deactivate (CORBA::Environment &ACE_TRY_ENV);
  // Deactivate this Callback servant.

private:

  ACE_FILE_Addr file_;
  // The Addr corresponding to the retrieved file.

  ACE_FILE_IO file_io_;
  // The object that provides all file related IO operations
  // (e.g. read, write, etc).

  Web_Server::AMI_CallbackHandler_var ami_handler_;
  // Reference to this Reply Handler's self.

  Web_Server::Metadata_Type metadata_;
  // The metadata associated with the file being retrieved from the
  // web server.

  int last_chunk_;
  // Flag that indicates entire data content has been received.

  ACE_SYNCH_MUTEX lock_;
  // Lock used to prevent race conditions when checking to see if
  // metadata or entire content has been received.

  int * request_count_;
  // Pointer to external status monitoring variable. The contents (not
  // the pointer itself) of the <pending_data> parameter will be
  // decremented when file retrieval has completed.
};

#include "ace/post.h"

#endif  /* CONTENT_ITERATOR_I_H */
