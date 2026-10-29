
//=============================================================================
/**
 *  @file   Progress_i.h
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef PROGRESS_I_H
#define PROGRESS_I_H

#include "testS.h"

#include <vector>

#include <FL/Fl_Box.H>

class Progress_Window : public Fl_Box
{
public:
  /// Constructor
  Progress_Window (int n_peers,
                   int n_iterations,
                   int x, int y,
                   int w, int h,
                   char const *l = nullptr);

  ~Progress_Window () override = default;

  Progress_Window (Progress_Window const &) = delete;
  Progress_Window &operator= (Progress_Window const &) = delete;
  Progress_Window (Progress_Window &&) = delete;
  Progress_Window &operator= (Progress_Window &&) = delete;

  /// Keep track of the progress on each peer
  void sent_request (CORBA::Long id);
  void recv_reply (CORBA::Long id);

  /// One of the peers has bound to the server
  CORBA::Long bind (Peer_ptr a_peer);

  /// Start the peers...
  void start ();

private:
  /// The callback method
  static void start_callback (Fl_Widget *widget, void *data);

private:
  /// The total number of peers
  int n_peers_;

  /// The peers
  PeerSet peers_;

  /// The number of iterations;
  int n_iterations_;

  /// The set of sliders where we keep track of each client
  std::vector<Fl_Widget *> request_progress_;
  std::vector<Fl_Widget *> reply_progress_;
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
  explicit Progress_i (Progress_Window *window);

  Progress_i (Progress_i const &) = delete;
  Progress_i &operator= (Progress_i const &) = delete;
  Progress_i (Progress_i &&) = delete;
  Progress_i &operator= (Progress_i &&) = delete;

  // = The Progress methods.
  void sent_request (CORBA::Long id) override;
  void recv_reply (CORBA::Long id) override;
  CORBA::Long bind (Peer_ptr a_peer) override;

private:
  /// Display the graphics...
  Progress_Window *window_ {};
};

#endif /* PROGRESS_I_H */
