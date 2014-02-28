// $Id$
#ifndef ALLOC_TRACKER_H
#define ALLOC_TRACKER_H
#include /**/"ace/pre.h"

#include "orbsvcs/Notify/notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//#define DEBUG_MEMORY_USE
#if defined(_MSC_VER) && defined (_DEBUG) && defined (DEBUG_MEMORY_USE)
namespace CRT{

/// \brief dump heap allocation stats
///
/// This works only on microsoft/windows compilers
/// but it's handy to diagnose memory allocation problems.
/// To use, add the following macro in the scope where you
/// want to check memory usage.
///   ACE_WIN32_HEAP_MONITOR(name);
///     where name is a quoted string to label the stats.
/// When the object created by this macro goes out of scope
/// it will write a log message like:
/// (10056|6396) name: New heap blocks: 39; bytes: 19550
///
/// For non-debug, or non-vc builds, the macro expands to nothing
class CrtHeapDumper
{
public:
  CrtHeapDumper(const char * name, bool verbose = false)
    : name_ (name)
    , verbose_ (verbose)
  {
    _CrtMemCheckpoint (&before_);
  }

  ~CrtHeapDumper()
  {
    dump();
  }
  void dump()
  {
    _CrtMemState after;
    _CrtMemCheckpoint (&after);
    _CrtMemState diff;
    _CrtMemDifference (&diff, &before_, &after);

    ORBSVCS_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) %s: New heap blocks: %d; bytes: %d\n"),
      name_.c_str (),
      static_cast<int> (diff.lCounts[_NORMAL_BLOCK]),
      static_cast<int> (diff.lSizes[_NORMAL_BLOCK])
      ));
    if (this->verbose_)
    {
     _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
     _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
     _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
     _CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT );
     _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
     _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT );
     _CrtMemDumpAllObjectsSince (&this->before_);
    }
  }

private:
  ACE_CString name_;
  bool verbose_;
  _CrtMemState before_;
};
} //namespace
#define ACE_WIN32_HEAP_MONITOR(name) \
  CRT::CrtHeapDumper heap_check___(name); \
  ACE_UNUSED_ARG (heap_check___)
#else // _MSC_VER etc
 #define ACE_WIN32_HEAP_MONITOR(name)
#endif // _MSC_VER etc
#include /**/"ace/post.h"
#endif // ALLOC_TRACKER_H
