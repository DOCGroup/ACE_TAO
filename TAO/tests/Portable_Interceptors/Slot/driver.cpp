// author    : Boris Kolpackov <boris@kolpackov.net>
#include "tao/PI/PI.h"
#include "tao/LocalObject.h"
#include "tao/ORBInitializer_Registry.h"

PortableInterceptor::SlotId slot_id {};

class ORB_Initializer : public virtual PortableInterceptor::ORBInitializer,
                        public virtual ::CORBA::LocalObject
{
public:
  ORB_Initializer () = default;

  void pre_init (PortableInterceptor::ORBInitInfo_ptr) override
  {
  }

  void post_init (PortableInterceptor::ORBInitInfo_ptr info) override
  {
    slot_id = info->allocate_slot_id ();
    ACE_DEBUG ((LM_DEBUG, "Allocated slot with id %d.\n", slot_id));
  }

  ORB_Initializer (ORB_Initializer const &) = delete;
  ORB_Initializer (ORB_Initializer &&) = delete;
  ORB_Initializer & operator = (ORB_Initializer const &) = delete;
  ORB_Initializer & operator = (ORB_Initializer &&) = delete;
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
  {
    PortableInterceptor::ORBInitializer_var const orb_initializer (new ORB_Initializer ());
    PortableInterceptor::register_orb_initializer (orb_initializer.in ());

    CORBA::ORB_var const orb (CORBA::ORB_init (argc, argv));

    CORBA::Object_var const pic_obj (orb->resolve_initial_references ("PICurrent"));
    PortableInterceptor::Current_var const pic (
      PortableInterceptor::Current::_narrow (pic_obj.in ()));

    CORBA::Any in {};
    CORBA::ULong constexpr slot_value {1};
    in <<= slot_value;
    pic->set_slot (slot_id, in);

    CORBA::ULong v {};
    CORBA::Any_var const out (pic->get_slot (slot_id));
    out >>= v;

    if (v != 1)
    {
      ACE_ERROR ((LM_ERROR, "ERROR: Slot value was not preserved.\n"));
      return 1;
    }

    orb->destroy ();
  }
  catch (PortableInterceptor::InvalidSlot const&)
    {
      ACE_ERROR ((LM_ERROR, "ERROR: Caught InvalidSlot exception.\n"));
      return 1;
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
