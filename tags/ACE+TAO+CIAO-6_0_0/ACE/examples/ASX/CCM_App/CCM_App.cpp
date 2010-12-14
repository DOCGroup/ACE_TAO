// $Id$

#define ACE_BUILD_SVC_DLL

#include "ace/Stream.h"
#include "ace/Task.h"
#include "ace/Module.h"
#include "ace/svc_export.h"



typedef ACE_Task<ACE_SYNCH> MT_Task;
typedef ACE_Stream<ACE_SYNCH> MT_Stream;
typedef ACE_Module<ACE_SYNCH> MT_Module;

class ACE_Svc_Export Test_Task : public MT_Task
{
public:
  //FUZZ: disable check_for_lack_ACE_OS
  virtual int open (void *);
  virtual int close (u_long);
  //FUZZ: enable check_for_lack_ACE_OS

  virtual int init (int, ACE_TCHAR *[]);
  virtual int fini (void);
  virtual int suspend (void);
  virtual int resume (void);
};

int
Test_Task::open (void *)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("opening %s\n"),
              this->name () ? this->name () : ACE_TEXT ("task")));
  return 0;
}

int
Test_Task::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("closing %s\n"),
              this->name () ? this->name () : ACE_TEXT ("task")));
  return 0;
}

int
Test_Task::suspend (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("suspending in %s\n"),
              this->name () ? this->name () : ACE_TEXT ("task")));
  return 0;
}

int
Test_Task::resume (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("resuming in %s\n"),
              this->name () ? this->name () : ACE_TEXT ("task")));
  return 0;
}

int
Test_Task::init (int, ACE_TCHAR *[])
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("initializing %s\n"),
              this->name () ? this->name () : ACE_TEXT ("task")));

  return 0;
}

int
Test_Task::fini (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("finalizing %s (%@)\n"),
              this->name () ? this->name () : ACE_TEXT ("task"),
              this));
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
  return new MT_Module (ACE_TEXT ("Device_Adapter"),
                        new Test_Task, new Test_Task);
}

MT_Module *
make_ea (void)
{
  return new MT_Module (ACE_TEXT ("Event_Analyzer"),
                        new Test_Task, new Test_Task);
}

MT_Module *
make_mr (void)
{
  return new MT_Module (ACE_TEXT ("Multicast_Router"),
                        new Test_Task, new Test_Task);
}
