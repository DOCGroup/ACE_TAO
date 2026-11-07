#define ACE_BUILD_SVC_DLL

#include "ace/Stream.h"
#include "ace/Task.h"
#include "ace/Module.h"
#include "ace/svc_export.h"
#include <memory>


using MT_Task = ACE_Task<ACE_SYNCH>;
using MT_Stream = ACE_Stream<ACE_SYNCH>;
using MT_Module = ACE_Module<ACE_SYNCH>;

class ACE_Svc_Export Test_Task : public MT_Task
{
public:
  Test_Task () = default;

  //FUZZ: disable check_for_lack_ACE_OS
  int open (void *) override;
  int close (u_long) override;
  //FUZZ: enable check_for_lack_ACE_OS

  int init (int, ACE_TCHAR *[]) override;
  int fini () override;
  int suspend () override;
  int resume () override;

  Test_Task (Test_Task const &) = delete;
  Test_Task (Test_Task &&) = delete;
  Test_Task &operator= (Test_Task const &) = delete;
  Test_Task &operator= (Test_Task &&) = delete;
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
Test_Task::suspend ()
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("suspending in %s\n"),
              this->name () ? this->name () : ACE_TEXT ("task")));
  return 0;
}

int
Test_Task::resume ()
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
Test_Task::fini ()
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

static MT_Module *
make_module (ACE_TCHAR const *name)
{
  std::unique_ptr<Test_Task> writer = std::make_unique<Test_Task> ();
  std::unique_ptr<Test_Task> reader = std::make_unique<Test_Task> ();
  std::unique_ptr<MT_Module> module =
    std::make_unique<MT_Module> (name, writer.get (), reader.get ());
  // ACE_Module takes ownership of both tasks.
  writer.release ();
  reader.release ();
  return module.release ();
}

// Dynamically linked functions used to control configuration.

extern "C" ACE_Svc_Export MT_Stream *make_stream ();
extern "C" ACE_Svc_Export MT_Module *make_da ();
extern "C" ACE_Svc_Export MT_Module *make_ea ();
extern "C" ACE_Svc_Export MT_Module *make_mr ();

// These factories transfer ownership of their raw pointers to ACE's service
// configuration and stream/module lifecycle.
MT_Stream *
make_stream ()
{
  return std::make_unique<MT_Stream> ().release ();
}

MT_Module *
make_da ()
{
  return make_module (ACE_TEXT ("Device_Adapter"));
}

MT_Module *
make_ea ()
{
  return make_module (ACE_TEXT ("Event_Analyzer"));
}

MT_Module *
make_mr ()
{
  return make_module (ACE_TEXT ("Multicast_Router"));
}
