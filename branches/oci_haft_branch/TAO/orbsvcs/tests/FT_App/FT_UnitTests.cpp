// -*- C++ -*-
//
// $Id$

#include "ace/pre.h"
#include "FT_UnitTests.h"
#include "ace/Get_Opt.h"
#include "tao/PortableServer/ORB_Manager.h"
#include "orbsvcs/PortableGroup/PG_Properties_Encoder.h"
#include "orbsvcs/PortableGroup/PG_Properties_Decoder.h"
#include "orbsvcs/orbsvcs/PortableGroup/PG_Operators.h"

#include "tao/corbafwd.h"
#include "tao/ORB.h"
#include "orbsvcs/FT_CORBA_ORBC.h"

#include <iostream>
#include <fstream>


/////////////////////////////////////////////////////
// Test default properties
//
int FT_UnitTests::test_001 (int run_test,
                            CORBA::String_var & desc)
{
  int result = 0;

  ACE_TRY_NEW_ENV
  {
    desc = CORBA::string_dup(
      "FT UnitTest[001]: default properties.");
    if (run_test)
    {
      // create a property set
      TAO_PG::Properties_Encoder encoder;
      PortableGroup::Value value;

      value <<= ::FT::SEMI_ACTIVE;
      encoder.add(::FT::FT_REPLICATION_STYLE, value);

      value <<= ::FT::MEMB_INF_CTRL;
      encoder.add(::FT::FT_MEMBERSHIP_STYLE, value);

      value <<= ::FT::CONS_APP_CTRL;
      encoder.add(::FT::FT_CONSISTENCY_STYLE, value);

      value <<= ::FT::PULL;
      encoder.add(::FT::FT_FAULT_MONITORING_STYLE, value);

      value <<= ::FT::MEMB;
      encoder.add(::FT::FT_FAULT_MONITORING_GRANULARITY, value);

      value <<= 2;
      encoder.add(::FT::FT_INITIAL_NUMBER_REPLICAS, value);

      value <<= 2;
      encoder.add(::FT::FT_MINIMUM_NUMBER_REPLICAS, value);

      // allocate and populate the criteria
      FT::Properties_var props_in;
      ACE_NEW_NORETURN (props_in, FT::Properties);
      if (props_in.ptr() == 0)
      {
        ACE_ERROR((LM_ERROR,
          "Error cannot allocate properties.\n"
          ));
          result = 1;
      }
      else
      {
        encoder.encode(props_in);

        rm_->set_default_properties (props_in
                                    ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        //TODO: do a get props and check the return value
        FT::Properties_var props_out;
        props_out = rm_->get_default_properties (
          ACE_ENV_SINGLE_ARG_PARAMETER);

        const FT::Properties & p_in = props_in;
        ::TAO_PG::Properties_Decoder decoder_in(p_in);
        int len_in = p_in.length ();

        const FT::Properties & p_out = props_out;
        ::TAO_PG::Properties_Decoder decoder_out(p_out);
        int len_out = p_out.length ();

        if (len_out != len_in)
        {
          std::cout << "FT UnitTest[001]: Wrong number of properties." << std::endl;
          result = 1;
        }

        for (int i = 0; i < len_out; ++i)
        {
          const PortableGroup::Property & property1 = p_out[i];
          ACE_CString prop_name = property1.nam[0].id;

          CORBA::Long value_in;
          CORBA::Long value_out;

          if(TAO_PG::find (decoder_in, prop_name, value_in) )
          {
            if(TAO_PG::find (decoder_out, prop_name, value_out) )
            {
              if (value_in != value_out)
              {
                std::cout << "FT UnitTest[001]: Property '" << prop_name << "' did not have the same value." << std::endl;
                result = 1;
              }
            }
            else
            {
              std::cout << "FT UnitTest[001]: Property '" << prop_name << "' was not in the output set." << std::endl;
              result = 1;
            }
          }
          else
          {
            std::cout << "FT UnitTest[001]: Property '" << prop_name << "' was not in the input set." << std::endl;
            result = 1;
          }
        }
      }
    }
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
    result = 1;
  }
  ACE_ENDTRY;

  return result;
}

/////////////////////////////////////////////////////
// Test type properties
//
int FT_UnitTests::test_002 (int run_test,
                            CORBA::String_var & desc)
{
  int result = 0;

  ACE_TRY_NEW_ENV
  {
    desc = CORBA::string_dup(
      "FT UnitTest[002]: type properties.");
    if (run_test)
    {
      // create a test type-id
      const char * type_id = "IDL:FT_TEST/TestReplica:1.0";

      // create a property set
      TAO_PG::Properties_Encoder encoder;
      PortableGroup::Value value;

      value <<= ::FT::MEMB_APP_CTRL;
      encoder.add(::FT::FT_MEMBERSHIP_STYLE, value);

      value <<= ::FT::CONS_INF_CTRL;
      encoder.add(::FT::FT_CONSISTENCY_STYLE, value);

      value <<= 10;
      encoder.add(::FT::FT_INITIAL_NUMBER_REPLICAS, value);

      value <<= 3;
      encoder.add(::FT::FT_MINIMUM_NUMBER_REPLICAS, value);

      // allocate and populate the criteria
      FT::Properties_var props_in;
      ACE_NEW_NORETURN (props_in, FT::Properties);
      if (props_in.ptr() == 0)
      {
        ACE_ERROR((LM_ERROR,
          "Error cannot allocate properties.\n"
          ));
          result = 1;
      }
      else
      {
        encoder.encode(props_in);

        rm_->set_type_properties (type_id, props_in
                                  ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        FT::Properties_var props_out;
        props_out = rm_->get_type_properties (type_id
                                              ACE_ENV_ARG_PARAMETER);

        const FT::Properties & p_in = props_in;
        ::TAO_PG::Properties_Decoder decoder_in(p_in);
        int len_in = p_in.length ();

        const FT::Properties & p_out = props_out;
        ::TAO_PG::Properties_Decoder decoder_out(p_out);
        int len_out = p_out.length ();

        // Note: the output properties is the merged set of the
        // default properties and the type properties. Therefore,
        // we *can* get back more properties than we set. However,
        // the ones that we did set should come back with the same
        // values.

        for (int i = 0; i < len_in; ++i)
        {
          const PortableGroup::Property & property1 = p_in[i];
          ACE_CString prop_name = property1.nam[0].id;

          CORBA::Long value_in;
          CORBA::Long value_out;

          if(TAO_PG::find (decoder_in, prop_name, value_in) )
          {
            if(TAO_PG::find (decoder_out, prop_name, value_out) )
            {
              if (value_in != value_out)
              {
                std::cout << "FT UnitTest[002]: Property '" << prop_name << "' did not have the same value." << std::endl;
                result = 1;
              }
            }
            else
            {
              std::cout << "FT UnitTest[002]: Property '" << prop_name << "' was not in the output set." << std::endl;
              result = 1;
            }
          }
          else
          {
            std::cout << "FT UnitTest[002]: Property '" << prop_name << "' was not in the input set." << std::endl;
            result = 1;
          }
        }
      }
    }
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
    result = 1;
  }
  ACE_ENDTRY;

  return result;
}

/////////////////////////////////////////////////////
// Test object group properties
//
int FT_UnitTests::test_003 (int run_test,
                            CORBA::String_var & desc)
{
  int result = 0;

  ACE_TRY_NEW_ENV
  {
    desc = CORBA::string_dup(
      "FT UnitTest[003]: object group properties.");
    if (run_test)
    {
      // create a test type-id
      const char * type_id = "IDL:FT_TEST/TestReplica:1.0";

      // create a property set
      TAO_PG::Properties_Encoder encoder;
      PortableGroup::Value value;

      value <<= 7;
      encoder.add(::FT::FT_MINIMUM_NUMBER_REPLICAS, value);

      // allocate and populate the criteria
      FT::Properties_var props_in;
      ACE_NEW_NORETURN (props_in, FT::Properties);
      if (props_in.ptr() == 0)
      {
        ACE_ERROR((LM_ERROR,
          "Error cannot allocate properties.\n"
          ));
          result = 1;
      }
      else
      {
        encoder.encode(props_in);

        rm_->set_properties_dynamically (test_iogr_, props_in
                                         ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        //TODO: do a get props and check the return value
        FT::Properties_var props_out;
        props_out = rm_->get_properties (test_iogr_
                                         ACE_ENV_ARG_PARAMETER);

        const FT::Properties & p_in = props_in;
        ::TAO_PG::Properties_Decoder decoder_in(p_in);
        int len_in = p_in.length ();

        const FT::Properties & p_out = props_out;
        ::TAO_PG::Properties_Decoder decoder_out(p_out);
        int len_out = p_out.length ();

        // Note: the output properties is the merged set of the
        // default properties and the type properties. Therefore,
        // we *can* get back more properties than we set. However,
        // the ones that we did set should come back with the same
        // values.

        for (int i = 0; i < len_in; ++i)
        {
          const PortableGroup::Property & property1 = p_in[i];
          ACE_CString prop_name = property1.nam[0].id;

          CORBA::Long value_in;
          CORBA::Long value_out;

          if(TAO_PG::find (decoder_in, prop_name, value_in) )
          {
            if(TAO_PG::find (decoder_out, prop_name, value_out) )
            {
              if (value_in != value_out)
              {
                std::cout << "FT UnitTest[003]: Property '" << prop_name << "' did not have the same value." << std::endl;
                result = 1;
              }
            }
            else
            {
              std::cout << "FT UnitTest[003]: Property '" << prop_name << "' was not in the output set." << std::endl;
              result = 1;
            }
          }
          else
          {
            std::cout << "FT UnitTest[003]: Property '" << prop_name << "' was not in the input set." << std::endl;
            result = 1;
          }
        }
      }
    }
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
    result = 1;
  }
  ACE_ENDTRY;

  return result;
}

/////////////////////////////////////////////////////
// Test create_object
//
int FT_UnitTests::test_004 (int run_test,
                            CORBA::String_var & desc)
{
  int result = 0;

  ACE_TRY_NEW_ENV
  {
    desc = CORBA::string_dup(
      "FT UnitTest[002]: create_object.");
    if (run_test)
    {
      // create a test type-id
      const char * type_id = "IDL:FT_TEST/TestReplica:1.0";

      // create a property set
      TAO_PG::Properties_Encoder encoder;
      PortableGroup::Value value;

      value <<= ::FT::MEMB_APP_CTRL;
      encoder.add(::FT::FT_MEMBERSHIP_STYLE, value);

      value <<= ::FT::CONS_INF_CTRL;
      encoder.add(::FT::FT_CONSISTENCY_STYLE, value);

      value <<= 3;
      encoder.add(::FT::FT_INITIAL_NUMBER_REPLICAS, value);

      value <<= 2;
      encoder.add(::FT::FT_MINIMUM_NUMBER_REPLICAS, value);

      // allocate and populate the criteria
      FT::Properties_var props_in;
      ACE_NEW_NORETURN (props_in, FT::Properties);
      if (props_in.ptr() == 0)
      {
        ACE_ERROR((LM_ERROR,
          "Error cannot allocate properties.\n"
          ));
          result = 1;
      }
      else
      {
        encoder.encode(props_in);

        PortableGroup::GenericFactory::FactoryCreationId_var fcid;
        CORBA::Object_var my_object_group =
        rm_->create_object (type_id,
                            props_in,
                            fcid.out()
                            ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

      }
    }
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
    result = 1;
  }
  ACE_ENDTRY;

  return result;
}

/////////////////////////////////////////////////////
// Test ???
//
int FT_UnitTests::test_005 (int run_test,
                            CORBA::String_var & desc)
{
  int result = 0;
  desc = CORBA::string_dup(
    "This test has not been implemented.");
  if (run_test)
  {
    // put the test code here
  }
  return result;
}

/////////////////////////////////////////////////////
// Test ???
//
int FT_UnitTests::test_006 (int run_test,
                            CORBA::String_var & desc)
{
  int result = 0;
  desc = CORBA::string_dup(
    "This test has not been implemented.");
  if (run_test)
  {
    // put the test code here
  }
  return result;
}

/////////////////////////////////////////////////////
// Test ???
//
int FT_UnitTests::test_007 (int run_test,
                            CORBA::String_var & desc)
{
  int result = 0;
  desc = CORBA::string_dup(
    "This test has not been implemented.");
  if (run_test)
  {
    // put the test code here
  }
  return result;
}

/////////////////////////////////////////////////////
// Test ???
//
int FT_UnitTests::test_008 (int run_test,
                            CORBA::String_var & desc)
{
  int result = 0;
  desc = CORBA::string_dup(
    "This test has not been implemented.");
  if (run_test)
  {
    // put the test code here
  }
  return result;
}

/////////////////////////////////////////////////////
// Test ???
//
int FT_UnitTests::test_009 (int run_test,
                            CORBA::String_var & desc)
{
  int result = 0;
  desc = CORBA::string_dup(
    "This test has not been implemented.");
  if (run_test)
  {
    // put the test code here
  }
  return result;
}

/////////////////////////////////////////////////////
// Test ???
//
int FT_UnitTests::test_010 (int run_test,
                            CORBA::String_var & desc)
{
  int result = 0;
  desc = CORBA::string_dup(
    "This test has not been implemented.");
  if (run_test)
  {
    // put the test code here
  }
  return result;
}

/////////////////////////////////////////////////////
// Test ???
//
int FT_UnitTests::test_011 (int run_test,
                            CORBA::String_var & desc)
{
  int result = 0;
  desc = CORBA::string_dup(
    "This test has not been implemented.");
  if (run_test)
  {
    // put the test code here
  }
  return result;
}

//////////////////////////
// The Scafolding

FT_UnitTests::FT_UnitTests (CORBA::ORB_var orb)
  : orb_(CORBA::ORB::_duplicate (orb))
{
}


FT_UnitTests::~FT_UnitTests ()
{
}


int FT_UnitTests::parse_args (int argc, char * argv[])
{
  int result = 0;
  char * rm_ior = 0;

  ACE_Get_Opt get_opts (argc, argv, "r:");
  int c;
  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'r':
      {
        const char * rm_ior = get_opts.opt_arg ();
        CORBA::String_var ior;
        if (readIORFile(rm_ior, ior))
        {
          CORBA::Object_var obj = orb_->string_to_object(ior);
          rm_ = FT::ReplicationManager::_narrow(obj);
          if (CORBA::is_nil(rm_))
          {
            std::cerr << "Can't resolve ReplicationManager IOR in " << rm_ior << std::endl;
            result = -1;
          }
        }
        else
        {
          std::cerr << "Can't read file " << rm_ior << std::endl;
          result = -1;
        }
        break;
      }
    }
  }
  return result;
}

int FT_UnitTests::readIORFile(const char * fileName, CORBA::String_var & ior)
{
  int result = 0;
  FILE *in = ACE_OS::fopen (fileName, "r");
  ACE_OS::fseek(in, 0, SEEK_END);
  size_t fileSize = ACE_OS::ftell(in);
  ACE_OS::fseek(in, 0, SEEK_SET);
  char * buffer;
  ACE_NEW_NORETURN (buffer,
    char[fileSize+1]);
  if (buffer != 0)
  {
    if( fileSize == ACE_OS::fread(buffer, 1, fileSize, in))
    {
      buffer[fileSize] = '\0';
      ior = CORBA::string_dup(buffer);
      ACE_TRY_CHECK;
      result = 1; // success
    }
    delete[] buffer;
  }
  return result;
}

  int
  FT_UnitTests::init ()
  {
    ACE_TRY
    {
      test_iogr_ = rm_->create_test_iogr();
    }
    ACE_CATCHANY
      {
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                            "Unexpected ACE_CATCHANY Exception!\n");
        return -1;
      }
    ACE_ENDTRY;

    return 0;
  }

  int
  FT_UnitTests::test_count ()
  {
    return 0;
  }

  int
  FT_UnitTests::run_all_tests ()
  {
    return 0;
  }

  const char *
  FT_UnitTests::test_desc (int test_number)
  {
    return "not implemented";
  }

  int
  FT_UnitTests::run_test (int test_number)
  {
    int result = 1;
    CORBA::String_var desc;

    switch (test_number)
    {
      case 1:  result =  test_001 (0, desc); break;
      case 2:  result =  test_002 (0, desc); break;
      case 3:  result =  test_003 (0, desc); break;
      case 4:  result =  test_004 (0, desc); break;
      case 5:  result =  test_005 (0, desc); break;
      case 6:  result =  test_006 (0, desc); break;
      case 7:  result =  test_007 (0, desc); break;
      case 8:  result =  test_008 (0, desc); break;
      case 9:  result =  test_009 (0, desc); break;
      case 10: result =  test_010 (0, desc); break;
      case 11: result =  test_011 (0, desc); break;
    }

    std::cout << "*** BEGIN " << desc << std::endl;

    switch (test_number)
    {
      case 1:  result =  test_001 (1, desc); break;
      case 2:  result =  test_002 (1, desc); break;
      case 3:  result =  test_003 (1, desc); break;
      case 4:  result =  test_004 (1, desc); break;
      case 5:  result =  test_005 (1, desc); break;
      case 6:  result =  test_006 (1, desc); break;
      case 7:  result =  test_007 (1, desc); break;
      case 8:  result =  test_008 (1, desc); break;
      case 9:  result =  test_009 (1, desc); break;
      case 10: result =  test_010 (1, desc); break;
      case 11: result =  test_011 (1, desc); break;
    }

    if (result == 0)
      std::cout << "*** SUCCESS " << desc << std::endl;
    else
      std::cout << "*** FAIL " << desc << std::endl;

    return result;
  }

#include "ace/post.h"
