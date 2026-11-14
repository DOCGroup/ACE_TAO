#include "test_config.h"

#include "ace/Monitor_Base.h"
#include "ace/Monitor_Point_Registry.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control;

class Test_Monitor : public Monitor_Base
{
public:
  explicit Test_Monitor (char const* name)
    : Monitor_Base (name, Monitor_Control_Types::MC_NUMBER)
  {
  }

  ~Test_Monitor () override
  {
    ++destructor_count_;
  }

  void update () override
  {
  }

  static int destructor_count_;
};

int Test_Monitor::destructor_count_ {};

#endif /* ACE_HAS_MONITOR_FRAMEWORK */

int
run_main (int, ACE_TCHAR* [])
{
  ACE_START_TEST (ACE_TEXT ("Monitor_Point_Registry_Test"));

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)
  char const* const name = "Monitor_Point_Registry_Test_Point";
  Monitor_Point_Registry* const registry = Monitor_Point_Registry::instance ();

  Test_Monitor* registered {};
  ACE_NEW_RETURN (registered, Test_Monitor (name), -1);
  ACE_TEST_ASSERT (registry->add (registered));
  registered->remove_ref ();

  Test_Monitor* duplicate {};
  ACE_NEW_RETURN (duplicate, Test_Monitor (name), -1);
  ACE_TEST_ASSERT (!registry->add (duplicate));
  duplicate->remove_ref ();
  ACE_TEST_ASSERT (Test_Monitor::destructor_count_ == 1);

  ACE_TEST_ASSERT (registry->remove (name));
  ACE_TEST_ASSERT (Test_Monitor::destructor_count_ == 2);
#endif /* ACE_HAS_MONITOR_FRAMEWORK */

  ACE_END_TEST;
  return 0;
}
