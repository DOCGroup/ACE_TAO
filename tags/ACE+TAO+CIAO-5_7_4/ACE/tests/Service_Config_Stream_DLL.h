// $Id$

#ifndef SERVICE_CONFIG_STREAM_DLL_H
#define SERVICE_CONFIG_STREAM_DLL_H

#include /**/ "ace/pre.h"

#include "Service_Config_Stream_DLL_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "ace/Stream.h"
#include "ace/Module.h"

typedef ACE_Task<ACE_SYNCH> MT_Task;
typedef ACE_Stream<ACE_SYNCH> MT_Stream;
typedef ACE_Module<ACE_SYNCH> MT_Module;

class Service_Config_Stream_DLL_Export Test_Task : public MT_Task
{
public:
  //FUZZ: disable check_for_lack_ACE_OS
  virtual int open (void *);
  virtual int close (u_long);
  //FUZZ: enable check_for_lack_ACE_OS

  virtual int init (int, ACE_TCHAR *[]);
  virtual int fini (void);
};

// Task to verify the order and operation of the stream assembly
// Command line args give the stream name (to look it up) and the names
// of the tasks that should be there, from head to tail.
class Service_Config_Stream_DLL_Export Stream_Order_Test : public MT_Task
{
public:
  virtual int init (int, ACE_TCHAR *[]);
};

#include /**/ "ace/post.h"
#endif /* SERVICE_CONFIG_STREAM_DLL_H */
