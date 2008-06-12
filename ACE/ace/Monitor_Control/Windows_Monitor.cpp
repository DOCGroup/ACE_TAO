// $Id$

#include "ace/Monitor_Control/Windows_Monitor.h"

#if defined (ACE_HAS_WIN32_PDH)

#include "ace/Log_Msg.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    Windows_Monitor::Windows_Monitor (const ACE_TCHAR *path)
      : value_ (0.0)
      , query_ (0)
      , counter_ (0)
      , status_ (ERROR_SUCCESS)
    {
      /// Create a query and a counter here so it doesn't have
      /// to be done with each update.

      this->status_ = ACE_TEXT_PdhOpenQuery (0, 0, &this->query_);

      if (ERROR_SUCCESS != this->status_)
        {
          ACE_ERROR ((LM_DEBUG, ACE_TEXT ("PdhOpenQuery failed\n")));
        }

      this->status_ =
        ACE_TEXT_PdhAddCounter (this->query_,
                                path,
                                0,
                                &this->counter_);

      if (ERROR_SUCCESS != this->status_)
        {
          ACE_ERROR ((LM_DEBUG,
                      ACE_TEXT ("PdhAddCounter %s failed\n"),
                      path));
        }
    }

    void
    Windows_Monitor::update_i (void)
    {
      PdhCollectQueryData (this->query_);
      PdhGetFormattedCounterValue (this->counter_,
                                   PDH_FMT_DOUBLE,
                                   0,
                                   &this->pdh_value_);
      this->value_ = this->pdh_value_.doubleValue;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* defined (ACE_HAS_WIN32_PDH) */
