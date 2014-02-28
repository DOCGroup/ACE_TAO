
//=============================================================================
/**
 *  @file    driver.cpp
 *
 *  $Id$
 *
 *  Driver program
 *
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#include "client.h"
#include "driver.h"
#include "results.h"
#include "tests.h"
#include "ace/Get_Opt.h"

// This function runs the test (main program)
int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  // get an instance of the driver object
  try
    {
      Driver drv;

      // initialize the driver
      if (drv.init (argc, argv) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                          "(%N:%l) driver.cpp - "
                          "Driver initialization failed\n"),
                          -1);

      // run various tests
      if (drv.run () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                          "(%N:%l) driver.cpp - "
                          "tests failed\n"),
                          -1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught unexpected CORBA exception:");

      return 1;
    }
  return 0;
}

// constructor
Driver::Driver (void)
{
}

Driver::~Driver (void)
{
}

// initialize the driver
int
Driver::init (int argc, ACE_TCHAR **argv)
{
  // environment to track exceptions

  // retrieve the instance of Options
  Options *opt = OPTIONS::instance ();

  char exception_string[256];

  try
    {
      ACE_OS::strcpy (exception_string, "ORB Initialization");

      // Retrieve the underlying ORB
      this->orb_ = CORBA::ORB_init (argc,
                                    argv,
                                    "internet");

      // Parse command line and verify parameters.
      if (opt->parse_args (argc, argv) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) driver.cpp - "
                           "parse_args failed\n"),
                          -1);
      // Retrieve a Param_Test object reference
      ACE_OS::strcpy (exception_string,"ORB::string_to_object() failed.");

      CORBA::Object_var temp =
        this->orb_->string_to_object (opt->param_test_ior ());


      if (CORBA::is_nil (temp.in()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ORB::string_to_object() returned null object for IOR <%s>\n",
                           opt->param_test_ior ()),
                          -1);

      // Get the object reference
      ACE_OS::strcpy (exception_string,"Param_Test::_narrow () failed.");

      this->objref_ = Param_Test::_narrow (temp.in());

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (exception_string);
      return -1;
    }

  return 0;
}

int
Driver::run (void)
{
  // serves as a factory of Param_Client objects. It is also responsible to
  // start the test

  Options *opt = OPTIONS::instance ();  // get the options object
  int retstatus = -1;

  switch (opt->test_type ())
    {
    case Options::TEST_SHORT:
      {
        Param_Test_Client<Test_Short> *client = new
          Param_Test_Client<Test_Short> (this->orb_.in (),
                                         this->objref_.in(),
                                         new Test_Short);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_ULONGLONG:
      {
        Param_Test_Client<Test_ULongLong> *client = new
          Param_Test_Client<Test_ULongLong> (this->orb_.in (),
                                             this->objref_.in(),
                                             new Test_ULongLong);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_UB_STRING:
      {
        Param_Test_Client<Test_Unbounded_String> *client = new
          Param_Test_Client<Test_Unbounded_String> (this->orb_.in (),
                                                    this->objref_.in(),
                                                    new Test_Unbounded_String);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_BD_STRING:
      {
        Param_Test_Client<Test_Bounded_String> *client = new
          Param_Test_Client<Test_Bounded_String> (this->orb_.in (),
                                                  this->objref_.in(),
                                                  new Test_Bounded_String);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_UB_WSTRING:
      {
        Param_Test_Client<Test_Unbounded_WString> *client = new
          Param_Test_Client<Test_Unbounded_WString> (this->orb_.in (),
                                                     this->objref_.in(),
                                                     new Test_Unbounded_WString);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_BD_WSTRING:
      {
        Param_Test_Client<Test_Bounded_WString> *client = new
          Param_Test_Client<Test_Bounded_WString> (this->orb_.in (),
                                                   this->objref_.in(),
                                                   new Test_Bounded_WString);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_FIXED_STRUCT:
      {
        Param_Test_Client<Test_Fixed_Struct> *client = new
          Param_Test_Client<Test_Fixed_Struct> (this->orb_.in (),
                                                this->objref_.in(),
                                                new Test_Fixed_Struct);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_UB_STRING_SEQUENCE:
      {
        Param_Test_Client<Test_String_Sequence> *client = new
          Param_Test_Client<Test_String_Sequence> (this->orb_.in (),
                                                   this->objref_.in(),
                                                   new Test_String_Sequence);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_BD_STRING_SEQUENCE:
      {
        Param_Test_Client<Test_Bounded_String_Sequence> *client = new
          Param_Test_Client<Test_Bounded_String_Sequence> (this->orb_.in (),
                                                           this->objref_.in(),
                                                           new Test_Bounded_String_Sequence);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_UB_WSTRING_SEQUENCE:
      {
        Param_Test_Client<Test_WString_Sequence> *client = new
          Param_Test_Client<Test_WString_Sequence> (this->orb_.in (),
                                                    this->objref_.in(),
                                                    new Test_WString_Sequence);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_BD_WSTRING_SEQUENCE:
      {
        Param_Test_Client<Test_Bounded_WString_Sequence> *client = new
          Param_Test_Client<Test_Bounded_WString_Sequence> (this->orb_.in (),
                                                            this->objref_.in(),
                                                            new Test_Bounded_WString_Sequence);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_VAR_STRUCT:
      {
        Param_Test_Client<Test_Var_Struct> *client = new
          Param_Test_Client<Test_Var_Struct> (this->orb_.in (),
                                              this->objref_.in(),
                                              new Test_Var_Struct);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_NESTED_STRUCT:
      {
        Param_Test_Client<Test_Nested_Struct> *client = new
          Param_Test_Client<Test_Nested_Struct> (this->orb_.in (),
                                                 this->objref_.in(),
                                                 new Test_Nested_Struct);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_RECURSIVE_STRUCT:
      {
        Param_Test_Client<Test_Recursive_Struct> *client = new
          Param_Test_Client<Test_Recursive_Struct> (this->orb_.in (),
                                                    this->objref_.in(),
                                                    new Test_Recursive_Struct);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_OBJREF_STRUCT:
      {
        Param_Test_Client<Test_Objref_Struct> *client = new
          Param_Test_Client<Test_Objref_Struct> (this->orb_.in (),
                                                 this->objref_.in(),
                                                 new Test_Objref_Struct);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_UB_STRUCT_SEQUENCE:
      {
        Param_Test_Client<Test_Struct_Sequence> *client = new
          Param_Test_Client<Test_Struct_Sequence> (this->orb_.in (),
                                                   this->objref_.in(),
                                                   new Test_Struct_Sequence);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;

        Param_Test_Client<Test_Unbounded_Struct_Sequence> *client2 = new
          Param_Test_Client<Test_Unbounded_Struct_Sequence>
          (this->orb_.in (),
           this->objref_.in(),
           new Test_Unbounded_Struct_Sequence);

        if (opt->invoke_type () == Options::SII)
          retstatus = client2->run_sii_test ();
        else
          retstatus = client2->run_dii_test ();
        delete client2;
      }
      break;
    case Options::TEST_BD_STRUCT_SEQUENCE:
      {
        Param_Test_Client<Test_Bounded_Struct_Sequence> *client = new
          Param_Test_Client<Test_Bounded_Struct_Sequence> (this->orb_.in (),
                                                           this->objref_.in (),
                                                           new Test_Bounded_Struct_Sequence);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_UB_ARRAY_SEQUENCE:
      {
        Param_Test_Client<Test_Array_Sequence> *client = new
          Param_Test_Client<Test_Array_Sequence> (this->orb_.in (),
                                                  this->objref_.in (),
                                                  new Test_Array_Sequence);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_BD_ARRAY_SEQUENCE:
      {
        Param_Test_Client<Test_Bounded_Array_Sequence> *client = new
          Param_Test_Client<Test_Bounded_Array_Sequence> (this->orb_.in (),
                                                          this->objref_.in (),
                                                          new Test_Bounded_Array_Sequence);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_OBJREF:
      {
        Param_Test_Client<Test_ObjRef> *client = new
          Param_Test_Client<Test_ObjRef> (this->orb_.in (),
                                          this->objref_.in (),
                                          new Test_ObjRef);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_TYPECODE:
      {
        Param_Test_Client<Test_TypeCode> *client = new
          Param_Test_Client<Test_TypeCode> (this->orb_.in (),
                                            this->objref_.in(),
                                            new Test_TypeCode);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_ANY:
      {
        Param_Test_Client<Test_Any> *client = new
          Param_Test_Client<Test_Any> (this->orb_.in (),
                                       this->objref_.in(),
                                       new Test_Any);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_OBJREF_SEQUENCE:
      {
        Param_Test_Client<Test_ObjRef_Sequence> *client = new
          Param_Test_Client<Test_ObjRef_Sequence> (this->orb_.in (),
                                                   this->objref_.in(),
                                                   new Test_ObjRef_Sequence);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_ANYSEQ:
      {
        Param_Test_Client<Test_AnySeq> *client = new
          Param_Test_Client<Test_AnySeq> (this->orb_.in (),
                                          this->objref_.in(),
                                          new Test_AnySeq);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_UB_SHORTSEQ:
      {
        Param_Test_Client<Test_Short_Sequence> *client = new
          Param_Test_Client<Test_Short_Sequence> (this->orb_.in (),
                                                  this->objref_.in(),
                                                  new Test_Short_Sequence);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_BD_SHORTSEQ:
      {
        Param_Test_Client<Test_Bounded_Short_Sequence> *client = new
          Param_Test_Client<Test_Bounded_Short_Sequence> (this->orb_.in (),
                                                          this->objref_.in(),
                                                          new Test_Bounded_Short_Sequence);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_UB_LONGSEQ:
      {
        Param_Test_Client<Test_Long_Sequence> *client = new
          Param_Test_Client<Test_Long_Sequence> (this->orb_.in (),
                                                 this->objref_.in(),
                                                 new Test_Long_Sequence);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_BD_LONGSEQ:
      {
        Param_Test_Client<Test_Bounded_Long_Sequence> *client = new
          Param_Test_Client<Test_Bounded_Long_Sequence> (this->orb_.in (),
                                                         this->objref_.in(),
                                                         new Test_Bounded_Long_Sequence);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_FIXED_ARRAY:
      {
        Param_Test_Client<Test_Fixed_Array> *client = new
          Param_Test_Client<Test_Fixed_Array> (this->orb_.in (),
                                               this->objref_.in(),
                                               new Test_Fixed_Array);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_VAR_ARRAY:
      {
        Param_Test_Client<Test_Var_Array> *client = new
          Param_Test_Client<Test_Var_Array> (this->orb_.in (),
                                             this->objref_.in(),
                                             new Test_Var_Array);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_EXCEPTION:
      {
        Param_Test_Client<Test_Exception> *client = new
          Param_Test_Client<Test_Exception> (this->orb_.in (),
                                             this->objref_.in(),
                                             new Test_Exception);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_BIG_UNION:
      {
        Param_Test_Client<Test_Big_Union> *client = new
          Param_Test_Client<Test_Big_Union> (this->orb_.in (),
                                             this->objref_.in(),
                                             new Test_Big_Union);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_SMALL_UNION:
      {
        Param_Test_Client<Test_Small_Union> *client = new
          Param_Test_Client<Test_Small_Union> (this->orb_.in (),
                                             this->objref_.in(),
                                             new Test_Small_Union);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_RECURSIVE_UNION:
      {
        Param_Test_Client<Test_Recursive_Union> *client = new
          Param_Test_Client<Test_Recursive_Union> (this->orb_.in (),
                                                   this->objref_.in(),
                                                   new Test_Recursive_Union);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_COMPLEX_ANY:
      {
        Param_Test_Client<Test_Complex_Any> *client = new
          Param_Test_Client<Test_Complex_Any> (this->orb_.in (),
                                               this->objref_.in(),
                                               new Test_Complex_Any);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_MULTDIM_ARRAY:
      {
        Param_Test_Client<Test_Multdim_Array> *client = new
          Param_Test_Client<Test_Multdim_Array> (this->orb_.in (),
                                                 this->objref_.in(),
                                                 new Test_Multdim_Array);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    default:
      break;
    }

  // Get in a new environment variable
  try
    {
      if (opt->shutdown ())
        {
          this->objref_->shutdown ();
        }

      this->objref_ = Param_Test::_nil ();
      this->orb_->destroy ();

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("during shutdown");
    }

  return retstatus;
}
