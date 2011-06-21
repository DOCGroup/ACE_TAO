// author    : Boris Kolpackov <boris@kolpackov.net>
// $Id$

#include "tao/PI/PI.h"
#include "tao/LocalObject.h"
#include "tao/ORBInitializer_Registry.h"

PortableInterceptor::SlotId slot_id;

class ORB_Initializer : public virtual PortableInterceptor::ORBInitializer,
                        public virtual ::CORBA::LocalObject
{
public:
  virtual void
  pre_init (PortableInterceptor::ORBInitInfo_ptr)
  {
  }

  virtual void
  post_init (PortableInterceptor::ORBInitInfo_ptr info)
  {
    slot_id = info->allocate_slot_id ();
    ACE_DEBUG ((LM_DEBUG, "Allocated slot with id %d.\n", slot_id));
  }
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  try
  {
    PortableInterceptor::ORBInitializer_var orb_initializer = new ORB_Initializer ();
    PortableInterceptor::register_orb_initializer (orb_initializer.in ());

    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    CORBA::Object_var pic_obj = orb->resolve_initial_references ("PICurrent");
    PortableInterceptor::Current_var pic (
      PortableInterceptor::Current::_narrow (pic_obj.in ()));

    CORBA::Any in;
    in <<= CORBA::ULong (1);
    pic->set_slot (slot_id, in);

    CORBA::ULong v (0);
    CORBA::Any_var out = pic->get_slot (slot_id);
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
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
