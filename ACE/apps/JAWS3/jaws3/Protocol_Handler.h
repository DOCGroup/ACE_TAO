#ifndef JAWS_PROTOCOL_HANDLER_H
#define JAWS_PROTOCOL_HANDLER_H

#include "ace/Message_Block.h"

#include "jaws3/Export.h"
#include "jaws3/Event_Completer.h"

class JAWS_Export JAWS_Protocol_State
{
  friend class JAWS_Protocol_Handler;

public:
  virtual ~JAWS_Protocol_State ();

protected:
  virtual int service (JAWS_Event_Completer *, void *data) = 0;

  virtual JAWS_Protocol_State * transition ( const JAWS_Event_Result &
                                           , void *data
                                           , void *act) = 0;
};

/// Abstraction that interacts with concurrency dispatching.
class JAWS_Export JAWS_Protocol_Handler
  : public JAWS_Event_Completer
{
  friend class JAWS_TPOOL_Concurrency;
  friend class JAWS_TPR_Concurrency;
  friend class JAWS_THYBRID_Concurrency;

public:
  explicit JAWS_Protocol_Handler (JAWS_Protocol_State *state = nullptr,
                                  void *data = nullptr);
  JAWS_Protocol_Handler (JAWS_Protocol_Handler const &) = delete;
  JAWS_Protocol_Handler (JAWS_Protocol_Handler &&) = delete;
  JAWS_Protocol_Handler &operator= (JAWS_Protocol_Handler const &) = delete;
  JAWS_Protocol_Handler &operator= (JAWS_Protocol_Handler &&) = delete;

  virtual int service ();

  virtual void dismiss ()
  {
    delete this;
  }

protected:
  /// Destructor is protected to ensure instances are created dynamically.
  virtual ~JAWS_Protocol_Handler ();

protected:
  /// Handles event completion and transitions to the next protocol state.
  void event_complete (const JAWS_Event_Result &result, void *act);

  void default_complete (const JAWS_Event_Result &result, void *act) override
  {
    this->event_complete (result, act);
  }

private:
  JAWS_Protocol_State *state_;

  void *data_;

  ACE_Data_Block db_;
  ACE_Message_Block mb_;
};

#endif /* JAWS_PROTOCOL_HANDLER_H */
