// $Id$
#include "TestBasicTypes.h"

#include <orbsvcs/Notify/Name_Value_Pair.h>
#include <orbsvcs/Notify/Property.h>
#include <orbsvcs/Notify/QoSProperties.h>
#include <orbsvcs/Notify/AdminProperties.h>

#include <orbsvcs/CosNotificationC.h>

#include <ace/ARGV.h>

using namespace CORBA;
namespace CN = CosNotification;

using namespace TAO_Notify;

// Tests for simple basic types that don't require the creation
// of a Notify Service, or complex setup.
namespace TestBasicTypes
{
  ORB_var orb;

  class SetupTeardown {
  public:
    SetupTeardown() {
      setUp();
    }
    ~SetupTeardown() {
      try {
        tearDown();
      } catch (...) {
      }
    }
  private:
    void setUp()
    {
      // We have to initialize an orb, or Any will not work.
      ACE_ARGV av;
      int ac = 0;
      orb = ORB_init(ac, av.argv());
      ACE_ASSERT(! is_nil(orb.in()));
    }

    void tearDown()
    {
      orb->shutdown();
      orb->destroy();
      orb = ORB::_nil();
    }
  };

  void testProperty() {
    TAO_NS_Property_Long n("myprop");
    assertTrue(n.name().compare("myprop") == 0);
    assertEquals(0, n.value());
    assertEquals(1, n.is_valid());
    n.invalidate();
    assertEquals(0, n.is_valid());
    n = 42;
    assertEquals(42, n.value());
    assertEquals(1, n.is_valid());

    TAO_NS_Property_Long n2(n);
    assertEquals(42, n2.value());
    assertEquals(1, n2.is_valid());
    assertTrue(n2.name().compare("myprop") == 0);

    TAO_NS_Property_Long n3("myprop2", 1234);
    n.invalidate();
    n3 = n;
    // The assignment operator won't copy if the rhs is invalid
    assertEquals(1234, n3.value());
    assertEquals(1, n3.is_valid());
    assertTrue(n3.name().compare("myprop2") == 0);
    n3 = n2;
    assertEquals(42, n3.value());
    assertEquals(1, n3.is_valid());
    assertTrue(n3.name().compare("myprop") == 0);

    // We should test bool and struct version separately, or figure
    // out a way to safely combine their implementations into the base.
  }

  void testNVP(ACE_ENV_SINGLE_ARG_DECL) {
    SetupTeardown sutd;
    TAO_NS_Property_Long p_long("mylong", 42);
    TAO_NS_Property_Short p_short("myshort", 43);
    TAO_NS_Property_Time p_time("mytime", 44);
    TAO_NS_Property_Boolean p_bool("mybool", 45); // On purpose

    NVPList lst;
    lst.push_back(NVP(p_long));
    assertEquals(1, lst.size());

    // adding a duplicate should just replace the existing value
    lst.push_back(NVP(p_long));
    assertEquals(1, lst.size());

    lst.push_back(NVP(p_short));
    lst.push_back(NVP(p_time));
    lst.push_back(NVP(p_bool));
    lst.push_back(NVP("mystr", "46"));
    lst.push_back(NVP("mylong2", 47));
    assertEquals(6, lst.size());

    p_long = 0;
    p_short = 0;
    p_time = 0;
    p_bool = 0;
    p_long.invalidate();
    p_short.invalidate();
    p_time.invalidate();
    p_bool.invalidate();

    Long n = -1;
    ACE_CString str = "invalid";
    lst.load("mylong2", n);
    assertEquals(47, n);
    lst.load("mylong", n);
    assertEquals(42, n);
    lst.load("myshort", n);
    assertEquals(43, n);
    lst.load("mybool", str);
    assertTrue(str == "true");
    lst.load("mybool", n);
    assertEquals(0, n);

    lst.load(p_long);
    assertTrue(p_long.is_valid());
    assertEquals(42, p_long.value());
    lst.load(p_short);
    assertTrue(p_short.is_valid());
    assertEquals(43, p_short.value());
    lst.load(p_bool);
    assertTrue(p_bool.is_valid());
    assertTrue(p_bool.value());
    lst.load(p_time);
    assertTrue(p_time.is_valid());
    assertEquals(44, p_time.value());

    // Update the number to an invalid value
    lst.push_back(NVP("mylong2", "invalid1234"));
    lst.load("mylong2", n);
    assertEquals(0, n);
    lst.push_back(NVP("mylong2", "1234invalid"));
    lst.load("mylong2", n);
    assertEquals(1234, n);
    // Do the same for time
    lst.push_back(NVP("mytime", "invalid1234"));
    lst.load(p_time);
    p_time.invalidate();
    assertTrue(! p_time.is_valid());
    lst.push_back(NVP("mytime", "1234invalid"));
    lst.load(p_time);
    assertTrue(p_time.is_valid());
    assertEquals(1234, p_time.value());
  }

  void testQoSBasic()
  {
    // There are two ways to init qos and admin. 1) init() from a corba sequence
    // or 2) load manually, and then call init().
    TAO_NS_AdminProperties a;

    a.max_global_queue_length() = 1234;
    a.max_consumers() = 2345;
    a.max_suppliers() = 3456;
    a.reject_new_events() = 1;
    a.init();

    // After setting the properties, we have to init() before they are available
    // as a sequence.

    assertEquals(a.max_global_queue_length().value(), 1234);
    assertEquals(a.max_consumers().value(), 2345);
    assertEquals(a.max_suppliers().value(), 3456);
    assertEquals(a.reject_new_events().value(), 1);

    CN::AdminProperties aseq;
    a.populate(aseq);

    assertEquals(4, aseq.length());
    for (ULong i = 0; i < 4; ++i) {
      ACE_CString last;
      if (a.max_global_queue_length().name() == aseq[i].name.in()) {
        Long tmp = 0;
        assertTrue(aseq[i].value >>= tmp);
        assertEquals(1234, tmp);
      } else if (a.max_consumers().name() == aseq[i].name.in()) {
        Long tmp = 0;
        assertTrue(aseq[i].value >>= tmp);
        assertEquals(2345, tmp);
      } else if (a.max_suppliers().name() == aseq[i].name.in()) {
        Long tmp = 0;
        assertTrue(aseq[i].value >>= tmp);
        assertEquals(3456, tmp);
      } else if (a.reject_new_events().name() == aseq[i].name.in()) {
        Boolean tmp = 0;
        aseq[i].value >>= Any::to_boolean(tmp);
        assertTrue(1);
        assertEquals(1, tmp);
      } else {
        failIt("Unexpected property name");
      }
      assertTrue(last != aseq[i].name.in());
      last = aseq[i].name.in();
    }

    // Populate a new admin props from the seq
    TAO_NS_AdminProperties a2;
    int err = a2.init(aseq);
    assertEquals(0, err);

    assertEquals(a2.max_global_queue_length().value(), 1234);
    assertEquals(a2.max_consumers().value(), 2345);
    assertEquals(a2.max_suppliers().value(), 3456);
    assertEquals(a2.reject_new_events().value(), 1);

    // Finally, show a flaw in the current design where the internal hash
    // map could get out of sync with the properties.
    a2.max_consumers() = 42;

    a2.populate(aseq);
    assertEquals(4, aseq.length());
    err = a.init(aseq);
    assertEquals(0, err);

    assertEquals(a.max_global_queue_length().value(), 1234);
    assertEquals(a.max_consumers().value(), 2345);
    assertEquals(a.max_suppliers().value(), 3456);
    assertEquals(a.reject_new_events().value(), 1);
  }

} // namespace
