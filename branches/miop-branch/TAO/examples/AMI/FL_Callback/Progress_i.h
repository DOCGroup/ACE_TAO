// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/examples/AMI/FL_Callback
//
// = FILENAME
//   Progress_i.h
//
// = AUTHOR
//   Carlos O'Ryan
//
// ============================================================================

#ifndef PROGRESS_I_H
#define PROGRESS_I_H

#include "testS.h"

#if defined(ACE_HAS_FL)

#include <FL/Fl_Box.h>

class Progress_Window : public Fl_Box
{
public:
  Progress_Window (int n_peers,
                   int n_iterations,
                   int x, int y,
                   int w, int h,
                   const char* l = 0);
  // Constructor

  void sent_request (CORBA::Long id);
  void recv_reply (CORBA::Long id);
  // Keep track of the progress on each peer

  CORBA::Long bind (Peer_ptr a_peer
                    TAO_ENV_ARG_DECL);
  // One of the peers has bound to the server

  void start (void);
  // Start the peers...

private:
  static void start_callback (Fl_Widget* widget, void *data);
  // The callback method

private:
  int n_peers_;
  // The total number of peers

  PeerSet peers_;
  // The peers

  int n_iterations_;
  // The number of iterations;

  Fl_Widget** request_progress_;
  Fl_Widget** reply_progress_;
  // The set of sliders where we keep track of each client
};

class Progress_i : public POA_Progress
{
  // = TITLE
  //   Progress server implementation
  //
  // = DESCRIPTION
  //   Implements the Progress interface in test.idl, forward the
  //   calls to the Progress_Window class
  //
public:
  Progress_i (Progress_Window* window);
  // ctor

  // = The Progress methods.
  void sent_request (CORBA::Long id
                     TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  void recv_reply (CORBA::Long id
                   TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  CORBA::Long bind (Peer_ptr a_peer
                    TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  Progress_Window* window_;
  // Display the graphics...
};

#if defined(__ACE_INLINE__)
#include "Progress_i.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_FL */

#endif /* PROGRESS_I_H */
