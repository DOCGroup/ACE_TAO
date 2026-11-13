/* -*- C++ -*- */
#ifndef _EVENT_ANALYZER_H
#define _EVENT_ANALYZER_H

#include "ace/Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Module.h"
#include "ace/Task.h"

/**
 * @class Event_Analyzer
 * @brief Forwards received ACE_Message_Blocks to the neighboring stream module.
 *
 * In a full event service, application-specific processing would be done in
 * this class's put() or svc() method.
 */
class Event_Analyzer : public ACE_Task<ACE_SYNCH>
{
public:
  Event_Analyzer () = default;
  Event_Analyzer (Event_Analyzer const &) = delete;
  Event_Analyzer (Event_Analyzer &&) = delete;
  Event_Analyzer &operator= (Event_Analyzer const &) = delete;
  Event_Analyzer &operator= (Event_Analyzer &&) = delete;

  int open (void *a = nullptr) override;
  int close (u_long flags = 0) override;

  /// Entry point into this task.
  int put (ACE_Message_Block *msg,
           ACE_Time_Value * = nullptr) override;

  /// @name Dynamic linking hooks
  /// @{

  int init (int argc, ACE_TCHAR *argv[]) override;
  int fini () override;
  int info (ACE_TCHAR **info_string,
            size_t length) const override;

  /// @}

private:
  /// Process watermark control messages.
  virtual int control (ACE_Message_Block *);
};

#endif /* _EVENT_ANALYZER_H */
