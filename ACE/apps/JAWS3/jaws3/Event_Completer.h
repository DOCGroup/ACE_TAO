/* -*- c++ -*- */
#ifndef JAWS_EVENT_COMPLETER_H
#define JAWS_EVENT_COMPLETER_H

#include "jaws3/Export.h"
#include "jaws3/Event_Result.h"

class JAWS_Event_Completer;

/// Event completion callback interface.
class JAWS_Export JAWS_Event_Completer
{
public:
  virtual ~JAWS_Event_Completer ();

  /// Callback invoked when an accept operation completes.
  virtual void accept_complete (JAWS_Event_Result const &r, void *act = nullptr);

  /// Callback invoked when a connect operation completes.
  virtual void connect_complete (JAWS_Event_Result const &r, void *act = nullptr);

  /// Callback invoked when input has been read.
  virtual void input_complete (JAWS_Event_Result const &r, void *act = nullptr);

  /// Callback invoked when output has been completely written.
  virtual void output_complete (JAWS_Event_Result const &r, void *act = nullptr);

  /// Callback invoked when a timer expires.
  virtual void timer_complete (JAWS_Event_Result const &r, void *act = nullptr);

  /// Callback invoked when a lambda event completes.
  virtual void lambda_complete (JAWS_Event_Result const &r, void *act = nullptr);

  /// Default callback invoked when an event completes.
  virtual void default_complete (JAWS_Event_Result const &r, void *act = nullptr);
};

#endif /* JAWS_EVENT_COMPLETER_H */
