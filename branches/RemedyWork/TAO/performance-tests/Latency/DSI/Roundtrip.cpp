//
// $Id$
//
#include "Roundtrip.h"
#include "tao/AnyTypeCode/TypeCode_Constants.h"
#include "tao/AnyTypeCode/NVList.h"
#include "tao/AnyTypeCode/SystemExceptionA.h"
#include "tao/DynamicInterface/Server_Request.h"

Roundtrip::Roundtrip (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Roundtrip::invoke (CORBA::ServerRequest_ptr request)
{
  if (ACE_OS::strcmp ("shutdown", request->operation ()) == 0)
    {
      this->orb_->shutdown (0);

      return;
    }

  else if (ACE_OS::strcmp ("_is_a", request->operation ()) == 0)
    {
      CORBA::NVList_ptr list;
      this->orb_->create_list (0, list);

      CORBA::Any type_id;
      type_id._tao_set_typecode (CORBA::_tc_string);
      list->add_value ("type_id", type_id, CORBA::ARG_IN);

      request->arguments (list);

      CORBA::NamedValue_ptr nv = list->item (0);

      const char *arg;
      *(nv->value ()) >>= arg;

      CORBA::Boolean type_matches = 0;
      if (ACE_OS::strcmp (arg, "IDL:Test/Roundtrip:1.0") == 0
          || ACE_OS::strcmp (arg, "IDL:omg.org/CORBA/Object:1.0") == 0
          || ACE_OS::strcmp (arg, "") == 0)
        type_matches = 1;

      CORBA::Any result;
      result <<= CORBA::Any::from_boolean (type_matches);

      request->set_result (result);

      return;
    }

  else if (ACE_OS::strcmp ("test_method", request->operation ()) == 0)
    {
      CORBA::NVList_ptr list;
      this->orb_->create_list (0, list);

      CORBA::Any send_time;
      send_time._tao_set_typecode (CORBA::_tc_ulonglong);
      list->add_value ("send_time", send_time, CORBA::ARG_IN);

      request->arguments (list);

      CORBA::NamedValue_ptr nv = list->item (0);

      request->set_result (*(nv->value ()));

      return;
    }

  CORBA::Any bad_operation;
  CORBA::BAD_OPERATION exception;
  bad_operation <<= exception;
  request->set_exception (bad_operation);
}

CORBA::RepositoryId
Roundtrip::_primary_interface (const PortableServer::ObjectId &,
                               PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:Test/Roundtrip:1.0");
}
