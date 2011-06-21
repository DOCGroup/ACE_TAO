// -*- C++ -*-
// $Id$

#include "Ptest.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_string.h"



Ptest::Ptest (void)
  : debug_ (0),
    query_ (0)
{
}

Ptest::~Ptest (void)
{
}

int
Ptest::init (int argc, ACE_TCHAR *argv[])
{
  try
    {
      this->orb_ = CORBA::ORB_init (argc, argv);

      int retval = this->parse_args (argc, argv);

      if (retval != 0)
        return retval;

      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("InterfaceRepository");

      if (CORBA::is_nil (object.in ()))
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              "Null objref from resolve_initial_references\n"
            ),
            -1
          );
        }

      this->repo_ =
        CORBA::Repository::_narrow (object.in ());

      if (CORBA::is_nil (this->repo_.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "CORBA::Repository::_narrow failed\n"),
                            -1);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Ptest::init");
      return -1;
    }

  return 0;
}

int
Ptest::run (void)
{
  try
    {
      if (this->query_ == 1)
        {
          this->query ();
        }
      else
        {
          this->populate ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Ptest::run");
      return -1;
    }

  return 0;
}

int
Ptest::parse_args (int argc,
                   ACE_TCHAR *argv[])
{
  ACE_Get_Opt opts (argc, argv, ACE_TEXT("dq"));
  int c;

  while ((c = opts ()) != -1)
    switch (c)
      {
      case 'd':   // debug flag
        this->debug_ = 1;
        break;
      case 'q':   // query toggle
        this->query_ = 1;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage: %s"
                           " [-d]"
                           " [-q]"
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

void
Ptest::populate (void)
{
  if (this->debug_)
    {
      ACE_DEBUG ((
          LM_DEBUG,
          ACE_TEXT ("\n============== POPULATE ==============\n\n")
        ));
    }

  CORBA::StructMemberSeq members (2);
  members.length (2);
  members[0].name = CORBA::string_dup ("long_mem");
  members[0].type_def = this->repo_->get_primitive (CORBA::pk_long);
  members[0].type = members[0].type_def->type ();

  members[1].name = CORBA::string_dup ("array_mem");
  members[1].type_def = this->repo_->create_array (5,
                                                   members[0].type_def.in ());
  members[1].type = members[1].type_def->type ();


  CORBA::StructDef_var svar = this->repo_->create_struct ("IDL:my_struct:1.0",
                                                          "my_struct",
                                                          "1.0",
                                                          members);

  CORBA::EnumMemberSeq def_members (2);
  def_members.length (2);

  def_members[0] = CORBA::string_dup ("ZERO");
  def_members[1] = CORBA::string_dup ("ONE");

  CORBA::EnumDef_var e_def_var = svar->create_enum ("IDL:my_def_enum:1.0",
                                                    "my_enum",
                                                    "1.0",
                                                    def_members);
}

int
Ptest::query (void)
{
  if (this->debug_)
    {
      ACE_DEBUG ((
          LM_DEBUG,
          ACE_TEXT ("\n============== QUERY ==============\n\n")
        ));
    }

  const char *members[] =
  {
    "long_mem",
    "array_mem",
    "my_enum"
  };

  CORBA::ContainedSeq_var contents =
    this->repo_->contents (CORBA::dk_all,
                           0);

  CORBA::ULong length = contents->length ();

  if (length != 1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ERROR: Repository::contents::length: %d")
                         ACE_TEXT (" while it's expected to be 1\n"),
                         length),
                        -1);
    }

  CORBA::ULong i = 0;

  CORBA::StructDef_var svar =
    CORBA::StructDef::_narrow (contents[i]);

  if (CORBA::is_nil (svar.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ERROR: Struct from repo is nil\n")),
                        -1);
    }

  CORBA::StructMemberSeq_var out_members =
    svar->members ();

  length = out_members->length ();

  if (length != 3)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ERROR: StructDef::members::length: %d")
                         ACE_TEXT (" while it's expected to be 3\n"),
                         length),
                        -1);
    }

  for (i = 0; i < length; ++i)
    {
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("StructDef::members[%d]::name: %C\n"),
                      i,
                      out_members[i].name.in ()));
        }

      if (i == length - 1)
        {
          if (ACE_OS::strcmp (out_members[i].name, "my_enum") != 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("ERROR: Incorrect struct member\n")),
                                -1);
            }
        }
      else
        {
          if (ACE_OS::strcmp (out_members[i].name, members[i]) != 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("ERROR: Incorrect struct member\n")),
                                -1);
            }
        }
    }

#if defined (ACE_NDEBUG)
  // ACE_ASSERT macro expands to nothing, so...
  ACE_UNUSED_ARG (members);
#endif /* ACE_NDEBUG */

  svar->destroy ();

  return 0;
}
