// $Id$

#include "TestS.h"
#include "tao/IORManipulation/IORManip_IIOP_Filter.h"
#include "tao/Stub.h"

class Filter_Localhost: public TAO_IORManip_IIOP_Filter
{
public:
  virtual CORBA::Boolean profile_info_matches (
                         const TAO_IORManip_IIOP_Filter::Profile_Info& pinfo);

};

CORBA::Boolean
Filter_Localhost::profile_info_matches (
                         const TAO_IORManip_IIOP_Filter::Profile_Info& pinfo)
{
  static const ACE_CString localhost ("localhost");
  static const ACE_CString localip ("127.0.0.1");
  return (pinfo.host_name_ != localhost && pinfo.host_name_ != localip);
}

class Hello: public POA_Test::Hello
{
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int status = 0;
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      Hello *hello_impl;
      ACE_NEW_RETURN (hello_impl,
                      Hello,
                      1);
      PortableServer::ServantBase_var owner_transfer(hello_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (hello_impl);

      CORBA::Object_var obj = root_poa->id_to_reference (id.in ());

      Test::Hello_var hello = Test::Hello::_narrow (obj.in ());

      Filter_Localhost filter;
      obj = filter.sanitize_profiles (hello.in ());
      Test::Hello_var after = Test::Hello::_narrow(obj.in());

      if (hello->_stubobj ()->base_profiles ().profile_count () ==
          after->_stubobj ()->base_profiles ().profile_count ())
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: profiles were not correctly filtered\n"));
          status++;
        }

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      status++;
    }

  return status;
}
