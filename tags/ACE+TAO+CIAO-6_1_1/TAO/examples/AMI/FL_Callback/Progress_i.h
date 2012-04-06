
//=============================================================================
/**
 *  @file   Progress_i.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef PROGRESS_I_H
#define PROGRESS_I_H

#include "testS.h"

#include <FL/Fl_Box.H>

class Progress_Window : public Fl_Box
{
public:
  /// Constructor
  Progress_Window (int n_peers,
                   int n_iterations,
                   int x, int y,
                   int w, int h,
                   const char* l = 0);

  /// Keep track of the progress on each peer
  void sent_request (CORBA::Long id);
  void recv_reply (CORBA::Long id);

  /// One of the peers has bound to the server
  CORBA::Long bind (Peer_ptr a_peer);

  /// Start the peers...
  void start (void);

private:
  /// The callback method
  static void start_callback (Fl_Widget* widget, void *data);

private:
  /// The total number of peers
  int n_peers_;

  /// The peers
  PeerSet peers_;

  /// The number of iterations;
  int n_iterations_;

  /// The set of sliders where we keep track of each client
  Fl_Widget** request_progress_;
  Fl_Widget** reply_progress_;
};

/**
 * @class Progress_i
 *
 * @brief Progress server implementation
 *
 * Implements the Progress interface in test.idl, forward the
 * calls to the Progress_Window class
 */
class Progress_i : public POA_Progress
{
public:
  /// ctor
  Progress_i (Progress_Window* window);

  // = The Progress methods.
  void sent_request (CORBA::Long id);
  void recv_reply (CORBA::Long id);
  CORBA::Long bind (Peer_ptr a_peer);

private:
  /// Display the graphics...
  Progress_Window* window_;
};

#endif /* PROGRESS_I_H */
