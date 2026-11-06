
//=============================================================================
/**
 *  @file    TSS_Data.h
 *
 *  @author Prashant Jain and Doug Schmidt
 */
//=============================================================================


#include "ace/Singleton.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch_Traits.h"

/**
 * @class TSS_Data
 *
 * @brief Data that is stored in thread-specific storage.
 */
class TSS_Data
{
public:
  TSS_Data () = default;

  TSS_Data (TSS_Data const &) = delete;
  TSS_Data (TSS_Data &&) = delete;
  TSS_Data &operator= (TSS_Data const &) = delete;
  TSS_Data &operator= (TSS_Data &&) = delete;

  void *data () const { return this->data_; }
  void data (void *v) { this->data_ = v; }

private:
  // = data_ will be thread-specific data so it doesn't need a lock.
  void *data_ {};
};

using TSS_DATA = ACE_TSS_Singleton<TSS_Data, ACE_SYNCH_MUTEX>;

