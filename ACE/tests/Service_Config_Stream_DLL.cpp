// $Id$

#include "Service_Config_Stream_DLL.h"
#include "ace/Service_Repository.h"
#include "ace/Service_Types.h"



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
              ACE_TEXT ("finalizing %s\n"),
              this->name () ? this->name () : ACE_TEXT ("task")));
  return 0;
}

// Factories used to control configuration.

ACE_FACTORY_DECLARE (Service_Config_Stream_DLL, Test_Task)
ACE_FACTORY_DEFINE (Service_Config_Stream_DLL, Test_Task)

// Dynamically linked functions used to control configuration.

extern "C" Service_Config_Stream_DLL_Export MT_Stream *make_stream (void);
extern "C" Service_Config_Stream_DLL_Export MT_Module *make_da (void);
extern "C" Service_Config_Stream_DLL_Export MT_Module *make_ea (void);
extern "C" Service_Config_Stream_DLL_Export MT_Module *make_mr (void);
extern "C" Service_Config_Stream_DLL_Export MT_Module *make_close (void);

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

MT_Module *
make_close (void)
{
  return new MT_Module (ACE_TEXT ("Close_Test_Module"),
                        new Test_Task, new Test_Task);
}


// Task to verify the order and operation of the stream assembly
// Command line args give the stream name (to look it up) and the names
// of the tasks that should be there, from head to tail.

ACE_FACTORY_DECLARE (Service_Config_Stream_DLL, Stream_Order_Test)
ACE_FACTORY_DEFINE (Service_Config_Stream_DLL, Stream_Order_Test)

int
Stream_Order_Test::init (int argc, ACE_TCHAR *argv[])
{
  if (argc < 1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Stream_Order_Test needs at least 1 arg\n")),
                      -1);
  const ACE_Service_Type *st = 0;
  if (ACE_Service_Repository::instance ()->find (argv[0], &st, false) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Cannot find %s\n"), argv[0]), -1);
  const ACE_Service_Type_Impl *st_impl = st->type ();
  MT_Stream *str = reinterpret_cast<MT_Stream *>(st_impl->object ());
  MT_Module *m = 0;
  if (-1 == str->top (m))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Cannot get module %p\n"),
                       ACE_TEXT ("top")),
                      -1);
  // Walk down the stream and compare module names. Note we start from the
  // top, i.e., the last module pushed.
  bool error = false;
  for (int i = 1; i < argc; ++i)
    {
      if (m == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Ran out of modules at layer %d\n"),
                      i));
          continue;
        }
      if (ACE_OS::strcmp (argv[i], m->name ()) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Layer %d: expected module %s, found %s\n"),
                      i,
                      argv[i], m->name ()));
          error = true;
        }
      else
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("Layer %d: found module %s, correct\n"),
                    i,
                    m->name ()));
      m = m->next ();
    }
  return error ? -1 : 0;
}
