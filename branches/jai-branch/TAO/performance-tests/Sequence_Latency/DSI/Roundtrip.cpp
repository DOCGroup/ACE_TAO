//
// $Id$
//

#include "TestC.h"
#include "Roundtrip.h"
#include "tao/DynamicInterface/Server_Request.h"

ACE_RCSID(DSI_Latency, Roundtrip, "$Id$")

Roundtrip::Roundtrip (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Roundtrip::invoke (CORBA::ServerRequest_ptr request
                   ACE_ENV_ARG_DECL)
{
  if (ACE_OS::strcmp ("shutdown", request->operation ()) == 0)
    {
      this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      return;
    }

  else if (ACE_OS::strcmp ("_is_a", request->operation ()) == 0)
    {
      CORBA::NVList_ptr list;
      this->orb_->create_list (0, list ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::Any type_id (CORBA::_tc_string,
                          0);
      list->add_value ("type_id", type_id, CORBA::ARG_IN
                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      request->arguments (list ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::NamedValue_ptr nv = list->item (0 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      const char *arg;
      *(nv->value ()) >>= arg;

      CORBA::Boolean type_matches = 0;
      if (ACE_OS::strcmp (arg, "IDL:Test/Roundtrip:1.0") == 0
          || ACE_OS::strcmp (arg, "IDL:omg.org/CORBA/Object:1.0") == 0
          || ACE_OS::strcmp (arg, "") == 0)
        type_matches = 1;

      CORBA::Any result;
      result <<= CORBA::Any::from_boolean (type_matches);

      request->set_result (result ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      return;
    }

  else if (ACE_OS::strcmp ("test_octet_method", request->operation ()) == 0)
    {
      CORBA::NVList_ptr list;
      this->orb_->create_list (0, list ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
		
		// Set up the argument list

	  CORBA::Any octet_load (Test::_tc_octet_load,
	  					     0);
	  list->add_value ("octet_load", octet_load, CORBA::ARG_IN
					   ACE_ENV_ARG_PARAMETER);
		
	  CORBA::Any send_time (CORBA::_tc_ulonglong,
						    0);
      list->add_value ("send_time", send_time, CORBA::ARG_IN
                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      request->arguments (list ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

	    // Set up the return value

      CORBA::NamedValue_ptr nv = list->item (1 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      request->set_result (*(nv->value ()) ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      return;
    }

  else if (ACE_OS::strcmp ("test_long_method", request->operation ()) == 0)
    {
      CORBA::NVList_ptr list;
      this->orb_->create_list (0, list ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
		
		// Set up the argument list

	  CORBA::Any long_load (Test::_tc_long_load,
	  					    0);
	  list->add_value ("long_load", long_load, CORBA::ARG_IN
					   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
		
	  CORBA::Any send_time (CORBA::_tc_ulonglong,
						    0);
      list->add_value ("send_time", send_time, CORBA::ARG_IN
                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      request->arguments (list ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

	    // Set up the return value

      CORBA::NamedValue_ptr nv = list->item (1 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      request->set_result (*(nv->value ()) ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      return;
    }

  else if (ACE_OS::strcmp ("test_short_method", request->operation ()) == 0)
    {
      CORBA::NVList_ptr list;
      this->orb_->create_list (0, list ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
		
		// Set up the argument list

	  CORBA::Any short_load (Test::_tc_short_load,
	  					     0);
	  list->add_value ("short_load", short_load, CORBA::ARG_IN
					   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
		
	  CORBA::Any send_time (CORBA::_tc_ulonglong,
						    0);
      list->add_value ("send_time", send_time, CORBA::ARG_IN
                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      request->arguments (list ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

	    // Set up the return value

      CORBA::NamedValue_ptr nv = list->item (1 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      request->set_result (*(nv->value ()) ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      return;
    }

  else if (ACE_OS::strcmp ("test_char_method", request->operation ()) == 0)
    {
      CORBA::NVList_ptr list;
      this->orb_->create_list (0, list ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
		
		// Set up the argument list

	  CORBA::Any char_load (Test::_tc_char_load,
	  					    0);
	  list->add_value ("char_load", char_load, CORBA::ARG_IN
					   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
		
	  CORBA::Any send_time (CORBA::_tc_ulonglong,
						    0);
      list->add_value ("send_time", send_time, CORBA::ARG_IN
                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      request->arguments (list ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

	    // Set up the return value

      CORBA::NamedValue_ptr nv = list->item (1 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      request->set_result (*(nv->value ()) ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      return;
    }

  else if (ACE_OS::strcmp ("test_longlong_method", request->operation ()) == 0)
    {
      CORBA::NVList_ptr list;
      this->orb_->create_list (0, list ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
		
		// Set up the argument list

	  CORBA::Any longlong_load (Test::_tc_longlong_load,
	  					     0);
	  list->add_value ("longlong_load", longlong_load, CORBA::ARG_IN
					   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
		
	  CORBA::Any send_time (CORBA::_tc_ulonglong,
						    0);
      list->add_value ("send_time", send_time, CORBA::ARG_IN
                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      request->arguments (list ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

	    // Set up the return value

      CORBA::NamedValue_ptr nv = list->item (1 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      request->set_result (*(nv->value ()) ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      return;
    }

  else if (ACE_OS::strcmp ("test_double_method", request->operation ()) == 0)
    {
      CORBA::NVList_ptr list;
      this->orb_->create_list (0, list ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
		
		// Set up the argument list

	  CORBA::Any double_load (Test::_tc_double_load,
	  					     0);
	  list->add_value ("double_load", double_load, CORBA::ARG_IN
					   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
		
	  CORBA::Any send_time (CORBA::_tc_ulonglong,
						    0);
      list->add_value ("send_time", send_time, CORBA::ARG_IN
                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      request->arguments (list ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

	    // Set up the return value

      CORBA::NamedValue_ptr nv = list->item (1 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      request->set_result (*(nv->value ()) ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      return;
    }

  CORBA::Any bad_operation;
  CORBA::BAD_OPERATION exception;
  bad_operation <<= exception;
  request->set_exception (bad_operation ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

CORBA::RepositoryId
Roundtrip::_primary_interface (const PortableServer::ObjectId &,
                               PortableServer::POA_ptr
                               ACE_ENV_ARG_DECL_NOT_USED)
{
  return CORBA::string_dup ("IDL:Test/Roundtrip:1.0");
}
