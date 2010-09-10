// $Id$

#include <iostream>

#include "Deployment.hpp"
#include "DP_Handler.h"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "ace/Get_Opt.h"
#include "XML_Typedefs.h"
#include "tao/ORB.h"
#include "Common.h"
#include "dynany_testC.h"
#include "XML_File_Intf.h"

static const char *input_file = "BasicSP.cdp";


static int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "i:");

  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        input_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-i <input file> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command-line
  return 0;
}

using namespace CIAO::Config_Handlers;

bool check_doubleseq (CORBA::Any &any)
{
  ACE_DEBUG ((LM_DEBUG, "Checking a double sequence\n"));

  dynany_test::DoubleSequence *seq;

  if (!(any >>= seq))
    {
      ACE_ERROR ((LM_ERROR, "failed to extract double sequence\n"));
      return false;
    }

  static const CORBA::Double ref_seq[3] = {1.0, 2.0, 2.5};

  if (seq->length () != 3)
    {
      ACE_ERROR ((LM_ERROR, "Unexpected length of double sequence\n"));
      return false;
    }

  for (CORBA::ULong i = 0; i < seq->length (); ++i)
    {
      if ((*seq)[i] != ref_seq[i])
        {
          ACE_ERROR ((LM_ERROR, "Expected %ith element of double sequence "
                      " to be %d, got %d instead.\n", i, (*seq)[i], ref_seq[i]));
          return false;
        }
    }

  return true;
}

bool check_longseq (dynany_test::LongSequence *seq);

bool check_longseq (CORBA::Any &any)
{
  dynany_test::LongSequence *seq;

  if (!(any >>= seq))
    {
      ACE_ERROR ((LM_ERROR, "failed to extract long sequence\n"));
      return false;
    }

  return check_longseq (seq);
}

bool check_longseq (dynany_test::LongSequence *seq)
{
  ACE_DEBUG ((LM_DEBUG, "Checking a long sequence\n"));

  static const CORBA::Long ref_seq[3] = {1, 5, 6};

  if (seq->length () != 3)
    {
      ACE_ERROR ((LM_ERROR, "Unexpected length of long sequence\n"));
      return false;
    }

  for (CORBA::ULong i = 0; i < seq->length (); ++i)
    {
      if ((*seq)[i] != ref_seq[i])
        {
          ACE_ERROR ((LM_ERROR, "Expected %ith element of long sequence "
                      " to be %i, got %i instead.\n", i, (*seq)[i], ref_seq[i]));
          return false;
        }
    }

  return true;
}


bool check_stringseq (dynany_test::StringSequence *seq);

bool check_stringseq (CORBA::Any &any)
{
  dynany_test::StringSequence *seq;

  if (!(any >>= seq))
    {
      ACE_ERROR ((LM_ERROR, "failed to extract string sequence\n"));
      return false;
    }

  return check_stringseq (seq);
}

bool check_stringseq (dynany_test::StringSequence *seq)
{
  ACE_DEBUG ((LM_DEBUG, "Checking a string sequence\n"));

  static const std::string ref_seq[3] = {"StringOne",
                                         "StringTwo",
                                         "StringThree"};

  if (seq->length () != 3)
    {
      ACE_ERROR ((LM_ERROR, "Unexpected length of string sequence\n"));
      return false;
    }

  for (CORBA::ULong i = 0; i < seq->length (); ++i)
    {
      std::string val ((*seq)[i].in ());

      if (val != ref_seq[i])
        {
          ACE_ERROR ((LM_ERROR, "Expected %ith element of string sequence "
                      " to be %i, got %i instead.\n", i, val.c_str (), ref_seq[i].c_str ()));
          return false;
        }
    }

  return true;
}

bool check_colorseq (dynany_test::ColorSequence *seq);

bool check_colorseq (CORBA::Any &any)
{
  dynany_test::ColorSequence *seq;

  if (!(any >>= seq))
    {
      ACE_ERROR ((LM_ERROR, "failed to extract color sequence\n"));
      return false;
    }

  return check_colorseq (seq);
}

bool check_colorseq (dynany_test::ColorSequence *seq)
{
  ACE_DEBUG ((LM_DEBUG, "Checking a color sequence\n"));

  static const dynany_test::COLOR ref_seq[5] = {dynany_test::yellow, dynany_test::grey, dynany_test::red,
                                                dynany_test::blue, dynany_test::black};

  if (seq->length () != 5)
    {
      ACE_ERROR ((LM_ERROR, "Unexpected length of color sequence\n"));
      return false;
    }

  for (CORBA::ULong i = 0; i < seq->length (); ++i)
    {
      if ((*seq)[i] != ref_seq[i])
        {
          ACE_ERROR ((LM_ERROR, "Expected %ith element of color sequence "
                      " to be %i, got %i instead.\n", i, (*seq)[i], ref_seq[i]));
          return false;
        }
    }

  return true;
}

bool check_foo_struct (CORBA::Any &any)
{
  ACE_DEBUG ((LM_DEBUG, "Checking a foo struct\n"));

  dynany_test::Foo *foo;

  if (!(any >>= foo))
    {
      ACE_ERROR ((LM_ERROR, "failed to extract foo struct\n"));
      return false;
    }

  // Check contents of foo struct
  if (foo->db != 5.5)
    {
      ACE_ERROR ((LM_ERROR, "expected value of db field to be 5.5, for %d\n",
                  foo->db));
      return false;
    }

  return check_colorseq (&foo->color_sequence) &&
    check_longseq (&foo->long_sequence) &&
    check_stringseq (&foo->string_sequence);
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      if (parse_args (argc, argv) != 0)
    return 1;

  // Initialize an ORB so Any will work
  CORBA::ORB_ptr orb = CORBA::ORB_init (argc, argv);
  ACE_UNUSED_ARG (orb);

  //Create an XML_Helper for all the file work

  bool success (true);

  XML_File_Intf intf (input_file);
  intf.add_search_path ("DANCE_ROOT", "/docs/schema/");
  intf.add_search_path ("CIAO_ROOT", "/docs/schema/");

  if (intf.get_plan () != 0)
    {
      std::cout << "Instance document import succeeded.  Checking contents.\n";

      //Retrieve the newly created IDL structure
      Deployment::DeploymentPlan *idl = intf.release_plan();

      for (CORBA::ULong i = 0; i < idl->infoProperty.length (); ++i)
        {
          std::string name (idl->infoProperty[i].name.in ());

          if (name == "double_sequence")
            success = success && check_doubleseq (idl->infoProperty[i].value);
          if (name == "long_sequence")
            success = success && check_longseq (idl->infoProperty[i].value);
          if (name == "string_sequence")
            success = success && check_stringseq (idl->infoProperty[i].value);
          if (name == "color_sequence")
            success = success && check_colorseq (idl->infoProperty[i].value);
          if (name == "foo_struct")
            success = success && check_foo_struct (idl->infoProperty[i].value);
        }
      delete idl;


    }

  if (success)
    return 0;
  else
    {
      ACE_ERROR((LM_ERROR, "Some test failures occurred, bailing out."));
      return -1;
    }
    }
  catch (CIAO::Config_Handlers::Config_Error &ex)
    {
      ACE_ERROR ((LM_ERROR, "Caught config_error: %C:%C\n",
                  ex.name_.c_str (), ex.error_.c_str ()));
      return -1;
    }
}


