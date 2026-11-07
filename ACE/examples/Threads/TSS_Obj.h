
//=============================================================================
/**
 *  @file    TSS_Obj.h
 *
 *   This program tests various features of ACE_Thread and the
 *   thread-specific storage variant of <ACE_SingletonEx>.
 *
 *  @author Prashant Jain and Doug Schmidt
 */
//=============================================================================


#include <atomic>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TSS_Obj
 *
 * @brief This object is stored in thread-specific storage.
 */
class TSS_Obj
{
public:
  TSS_Obj ();
  ~TSS_Obj ();

  TSS_Obj (TSS_Obj const &) = delete;
  TSS_Obj (TSS_Obj &&) = delete;
  TSS_Obj &operator= (TSS_Obj const &) = delete;
  TSS_Obj &operator= (TSS_Obj &&) = delete;

private:
  static std::atomic<int> count_;
};

