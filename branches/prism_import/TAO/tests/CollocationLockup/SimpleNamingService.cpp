// $Id$

/**
 * SimpleNamingService.cpp
 * This is part of the regression test against bug #2130.
 * See CollocationLockup.cpp for a description of the test.
 */

#include "SimpleNamingServiceS.h"

#include "ace/String_Base.h"
#include "ace/streams.h"

namespace
{
  const char* iorFileName = "SimpleNamingService.ior";
}

class SimpleNamingService_i : public virtual POA_SimpleNamingService
{
public:

  //FUZZ: disable check_for_lack_ACE_OS
  virtual void bind (CORBA::Object_ptr obj)
  {
    obj_ = CORBA::Object::_duplicate (obj);
  }
  //FUZZ: enable check_for_lack_ACE_OS


  virtual CORBA::Object_ptr resolve ()
  {
    return CORBA::Object::_duplicate (obj_.in ());
  }

private:

  CORBA::Object_var obj_;

};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var pPoaObj =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (pPoaObj.in ());
      PortableServer::POAManager_var pMgr = poa->the_POAManager ();
      pMgr->activate ();

      SimpleNamingService_i* servant = new SimpleNamingService_i;
      PortableServer::ObjectId_var oid = poa->activate_object (servant);
      CORBA::Object_var obj = poa->id_to_reference (oid.in ());
      CORBA::String_var str = orb->object_to_string (obj.in ());

      ofstream iorFile (iorFileName);
      iorFile << str.in () << endl;
      iorFile.close ();

      orb->run ();
    }
  catch (CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_ERROR, "Corba Exception: %s\n", ex._info ().c_str ()));
      return 1;
    }

  return 0;
}
