//
// $Id$
//

#include "tao/DynamicInterface/Request.h"  /* This must come first for
                                              G++ 3.4 or better */
#include "Client_Task.h"
#include "TestC.h"
#include "tao/Object.h"

char TEST_STR[] = "This is a DII collocation test.";
CORBA::Long TEST_BASIC_VALUE = 12345;
CORBA::Char TEST_SPECIAL_VALUE = 'x';
CORBA::Short TEST_HOUR = 12;
CORBA::Short TEST_MINUTE = 59;
CORBA::Short TEST_SECOND = 59;

Client_Task::Client_Task (const ACE_TCHAR *ior,
                          const ACE_TCHAR *simple_test_ior,
                          CORBA::ORB_ptr corb,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
    , input_ (ior)
    , simple_test_input_ (simple_test_ior)
    , corb_ (CORBA::ORB::_duplicate (corb))
    , error_count_ (0)
{
}

int
Client_Task::svc (void)
{
  try
    {
      CORBA::Object_var obj =
        this->corb_->string_to_object (input_);

      if (CORBA::is_nil (obj.in ()))
      {
        ACE_ERROR_RETURN ((LM_DEBUG,
          "Nil Test::Hello reference <%s>\n",
          input_),
          1);
      }

      CORBA::Object_var simple_test_obj =
        this->corb_->string_to_object (simple_test_input_);

      if (CORBA::is_nil (simple_test_obj.in ()))
      {
        ACE_ERROR_RETURN ((LM_DEBUG,
          "Nil Test::Simple_Test reference <%s>\n",
          simple_test_input_),
          1);
      }

      Test::Simple_Test_var simple_test
        = Test::Simple_Test::_narrow (simple_test_obj.in ());

      CORBA::Request_var req (obj->_request ("test_basic_arg"));
      req->add_in_arg ("basic") <<= TEST_BASIC_VALUE;
      req->send_oneway ();


      req = obj->_request ("test_unbounded_string_arg");

      req->add_in_arg ("ub_string") <<= TEST_STR;
      req->send_oneway ();


      req = obj->_request ("test_bounded_string_arg");

      ACE_CString bs (TEST_STR);
      bs = bs.substr (0, ::Test::BOUNDED_STRING_SIZE);

      req->add_in_arg ("bd_string")
        <<= CORBA::Any::from_string (bs.c_str (), ::Test::BOUNDED_STRING_SIZE);
      req->send_oneway ();


      req = obj->_request ("test_fixed_array_arg");

      ::Test::Fixed_Array fixed_array;

      for (CORBA::ULong m = 0 ; m < ::Test::FIX_ARRAY_SIZE; m ++)
      {
        fixed_array[m] = m + 999;
      }

      req->add_in_arg ("fixed_array") <<= ::Test::Fixed_Array_forany (fixed_array);
      req->send_oneway ();


      req = obj->_request ("test_var_array_arg");

      ::Test::Var_Array var_array;

      for (CORBA::ULong i = 0; i < ::Test::VAR_ARRAY_SIZE; ++i)
      {
        var_array[i] = CORBA::string_dup(TEST_STR);
      }

      req->add_in_arg ("var_array") <<= var_array;
      req->send_oneway ();


      req = obj->_request ("test_bounded_var_size_arg");

      ACE_CString bvs (TEST_STR);
      bvs = bvs.substr (0, ::Test::BOUNDED_VAR_SIZE);

      ::Test::Bounded_Var_Size_Arg* bd_var_size
        = new ::Test::Bounded_Var_Size_Arg();

      bd_var_size->replace (::Test::BOUNDED_VAR_SIZE,
        const_cast <CORBA::Char*> (bvs.c_str ()));

      req->add_in_arg ("bd_var_size") <<= bd_var_size;
      req->send_oneway ();


      req = obj->_request ("test_unbounded_var_size_arg");

      ::Test::Unbounded_Var_Size_Arg* ub_var_size
        = new ::Test::Unbounded_Var_Size_Arg();
      ub_var_size->replace (ub_var_size->maximum (),
        ACE_OS::strlen (TEST_STR),
        TEST_STR);

      req->add_in_arg ("ub_var_size") <<= ub_var_size;
      req->send_oneway ();


      req = obj->_request ("test_fixed_size_arg");

      ::Test::TimeOfDay t;
      t.hour = TEST_HOUR;
      t.minute = TEST_MINUTE;
      t.second = TEST_SECOND;

      req->add_in_arg ("fixed_size") <<= t;
      req->send_oneway ();


      req = obj->_request ("test_special_basic_arg");

      req->add_in_arg ("special") <<= CORBA::Any::from_char (TEST_SPECIAL_VALUE);
      req->send_oneway ();


      req = obj->_request ("test_object_arg");

      req->add_in_arg ("object") <<= simple_test_obj.in ();
      req->send_oneway ();


      req = obj->_request ("test_objref_arg");

      req->add_in_arg ("objref") <<= simple_test.in ();
      req->send_oneway ();


      req = obj->_request ("test_args_1");

      req->add_in_arg ("arg1") <<= simple_test_obj.in ();
      req->add_in_arg ("arg2") <<= CORBA::Any::from_char (TEST_SPECIAL_VALUE);
      req->add_in_arg ("arg3") <<= simple_test.in ();
      req->send_oneway ();


      req = obj->_request ("test_args_2");

      req->add_in_arg ("arg1") <<= TEST_STR;
      req->add_in_arg ("arg2") <<= ::Test::Fixed_Array_forany (fixed_array);
      req->add_in_arg ("arg3") <<= t;
      req->send_oneway ();


      req = obj->_request ("test_args_3");

      bd_var_size
        = new ::Test::Bounded_Var_Size_Arg();
      bd_var_size->replace (::Test::BOUNDED_VAR_SIZE,
        const_cast <CORBA::Char*> (bvs.c_str ()));

      ub_var_size
        = new ::Test::Unbounded_Var_Size_Arg();
      ub_var_size->replace (ub_var_size->maximum (),
        ACE_OS::strlen (TEST_STR),
        TEST_STR);

      for (CORBA::ULong j = 0; j < ::Test::VAR_ARRAY_SIZE; ++j)
      {
        var_array[j] = CORBA::string_dup(TEST_STR);
      }

      req->add_in_arg ("arg1")
        <<= CORBA::Any::from_string (bs.c_str (), ::Test::BOUNDED_STRING_SIZE);

      req->add_in_arg ("arg2") <<= bd_var_size;
      req->add_in_arg ("arg3") <<= ub_var_size;
      req->add_in_arg ("arg4") <<= var_array;
      req->send_oneway ();


      req = obj->_request ("shutdown");

      req->send_oneway ();


    }
  catch (const CORBA::Exception& ex)
    {
      error_count_ ++;
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }
  catch (...)
    {
      error_count_ ++;
      ACE_ERROR ((LM_ERROR, "(%P|%t)Client_Task::svc - caught unknown exception\n"));
      return 1;
    }

  return 0;

}

CORBA::ULong
Client_Task::error_count () const
{
  return error_count_;
}

