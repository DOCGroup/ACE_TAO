// $Id$

#include "test_dsi.h"
#include "tao/debug.h"

#if !defined(__ACE_INLINE__)
#include "test_dsi.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(DSI_Gateway, test_dsi, "$Id$")

void
DSI_Simple_Server::invoke (CORBA::ServerRequest_ptr request,
                           CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (ACE_OS::strcmp ("_is_a", request->operation ()) == 0)
    {
      this->is_a_impl (request, ACE_TRY_ENV);
    }
  else if (ACE_OS::strcmp ("shutdown", request->operation ()) == 0)
    {
      this->shutdown_impl (request, ACE_TRY_ENV);
    }
  else if (ACE_OS::strcmp ("test_method", request->operation ()) == 0)
    {
      this->test_method_impl (request, ACE_TRY_ENV);
    }
  else
    {
      CORBA::Any any;
      CORBA::BAD_OPERATION bad_op;
      any <<= bad_op;
      request->set_exception (any, ACE_TRY_ENV);
    }
}

void
DSI_Simple_Server::is_a_impl (CORBA::ServerRequest_ptr request,
                              CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::NVList_ptr list;
  this->orb_->create_list (0, list, ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Any any_id (CORBA::_tc_string);

  list->add_value ("id", any_id, CORBA::ARG_IN);

  request->arguments (list, ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Any_ptr id_ptr =
    list->item (0, ACE_TRY_ENV)->value ();
  ACE_CHECK;

  char* id;
  (*id_ptr) >>= id;

  ACE_DEBUG ((LM_DEBUG, "DSI_Simple_Server::is_a - <%s>\n",
              id));
  CORBA::Boolean result = 0;
  if (ACE_OS::strcmp ("IDL:Simple_Server:1.0", id) == 0
      || ACE_OS::strcmp ("IDL:omg.org/CORBA/Object:1.0", id) == 0)
    result = 1;

  CORBA::Any any_result;
  any_result <<= CORBA::Any::from_boolean (result);

  request->set_result (any_result, ACE_TRY_ENV);
}

void
DSI_Simple_Server::shutdown_impl (CORBA::ServerRequest_ptr,
                                  CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->target_->shutdown (ACE_TRY_ENV);
  ACE_CHECK;

  this->orb_->shutdown (0, ACE_TRY_ENV);
}

void
DSI_Simple_Server::test_method_impl (CORBA::ServerRequest_ptr request,
                                     CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::NVList_ptr list;
  this->orb_->create_list (0, list, ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Any x (CORBA::_tc_long);
  CORBA::Any the_in_structure (_tc_Structure);
  CORBA::Any the_out_structure (_tc_Structure);
  CORBA::Any name (CORBA::_tc_string);

  list->add_value ("x"                , x                , CORBA::ARG_IN   );
  list->add_value ("the_in_structure" , the_in_structure , CORBA::ARG_IN   );
  list->add_value ("the_out_structure", the_out_structure, CORBA::ARG_OUT  );
  list->add_value ("name"             , name             , CORBA::ARG_INOUT);

  request->_tao_lazy_evaluation (1);
  request->arguments (list, ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::NamedValue_var nv;
  this->orb_->create_named_value (nv.out (), ACE_TRY_ENV);
  ACE_CHECK;

  *nv->value () = CORBA::Any (CORBA::_tc_long);

  CORBA::Request_var target_request;

  this->target_->_create_request (0, // ctx
                                  "test_method",
                                  list,
                                  nv.in (),
                                  0, // exception_list,
                                  0, // context_list,
                                  target_request.inout (),
                                  0,
                                  ACE_TRY_ENV);
  ACE_CHECK;

  target_request->_tao_lazy_evaluation (1);

  target_request->invoke (ACE_TRY_ENV);
  ACE_CHECK;

  request->set_result (target_request->return_value (),
                       ACE_TRY_ENV);
  ACE_CHECK;

  if (TAO_debug_level > 4)
    {
      CORBA::Long arg_x;
      Structure* arg_the_out_structure;
      char* arg_name;

      target_request->return_value () >>= arg_x;
      CORBA::NamedValue_ptr nv0 = list->item (2, ACE_TRY_ENV);
      ACE_CHECK;
      CORBA::NamedValue_ptr nv1 = list->item (3, ACE_TRY_ENV);
      ACE_CHECK;
      *(nv0->value ()) >>= arg_the_out_structure;
      *(nv1->value ()) >>= arg_name;

      ACE_DEBUG ((LM_DEBUG,
                  "DSI_Simpler_Server ====\n"
                  "    x = %d\n"
                  "    i = %d\n"
                  "    length = %d\n"
                  "    name = <%s>\n",
                  arg_x,
                  arg_the_out_structure->i,
                  arg_the_out_structure->seq.length (),
                  arg_name));
    }
}

CORBA::RepositoryId
DSI_Simple_Server::_primary_interface (const PortableServer::ObjectId &,
                                       PortableServer::POA_ptr,
                                       CORBA::Environment &)
    ACE_THROW_SPEC (())
{
  return CORBA::string_dup ("IDL:Simple_Server:1.0");
}

PortableServer::POA_ptr
DSI_Simple_Server::_default_POA (CORBA::Environment &)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
