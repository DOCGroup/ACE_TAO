// $Id$

#define ACE_BUILD_SVC_DLL

#include "ace/Stream.h"
#include "ace/Task.h"
#include "ace/Module.h"
#include "ace/svc_export.h"

ACE_RCSID(CCM_App, CCM_App, "$Id$")

typedef ACE_Task<ACE_SYNCH> MT_Task;
typedef ACE_Stream<ACE_SYNCH> MT_Stream;
typedef ACE_Module<ACE_SYNCH> MT_Module;

class ACE_Svc_Export Test_Task : public MT_Task
{
public:
  virtual int open (void *);
  virtual int close (u_long);
  virtual int init (int, char *[]);
  virtual int fini (void);
  virtual int suspend (void);
  virtual int resume (void);
};

int 
Test_Task::open (void *)
{ 
  ACE_DEBUG ((LM_DEBUG,
              "opening %s\n",
              this->name () ? this->name () : "task"));
  return 0;
}

int 
Test_Task::close (u_long) 
{
  ACE_DEBUG ((LM_DEBUG,
              "closing %s\n",
              this->name () ? this->name () : "task"));
  return 0;
}

int 
Test_Task::suspend (void) 
{ 
  ACE_DEBUG ((LM_DEBUG,
              "suspending in %s\n",
              this->name () ? this->name () : "task")); 
  return 0; 
}

int 
Test_Task::resume (void) 
{
  ACE_DEBUG ((LM_DEBUG,
              "resuming in %s\n",
              this->name () ? this->name () : "task"));
  return 0;
}

int 
Test_Task::init (int, char *[])
{
  ACE_DEBUG ((LM_DEBUG,
              "initializing %s\n",
              this->name () ? this->name () : "task"));

  return 0; 
}

int 
Test_Task::fini (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "finalizing %s\n",
              this->name () ? this->name () : "task"));
  return 0; 
}

// Factories used to control configuration.

ACE_SVC_FACTORY_DECLARE (Test_Task)
ACE_SVC_FACTORY_DEFINE (Test_Task)

// Dynamically linked functions used to control configuration.

extern "C" ACE_Svc_Export MT_Stream *make_stream (void);
extern "C" ACE_Svc_Export MT_Module *make_da (void);
extern "C" ACE_Svc_Export MT_Module *make_ea (void);
extern "C" ACE_Svc_Export MT_Module *make_mr (void);

MT_Stream *
make_stream (void)
{
  return new MT_Stream;
}

MT_Module *
make_da (void)
{
  return new MT_Module ("Device_Adapter", new Test_Task, new Test_Task);
}

MT_Module *
make_ea (void)
{
  return new MT_Module ("Event_Analyzer", new Test_Task, new Test_Task);
}

MT_Module *
make_mr (void)
{
  return new MT_Module ("Multicast_Router", new Test_Task, new Test_Task);
}
