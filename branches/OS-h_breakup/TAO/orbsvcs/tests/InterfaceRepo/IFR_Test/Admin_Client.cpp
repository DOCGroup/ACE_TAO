// -*- C++ -*-
// $Id$

#include "Admin_Client.h"
#include "ace/Get_Opt.h"

ACE_RCSID(IFR_Test, Admin_Client, "$Id$")

Admin_Client::Admin_Client (void)
  : debug_ (0),
    all_tests_ (1),
    which_test_ (0),
    iterations_ (1)
{
  // Initialize the array of pointers to member functions.
  this->test_array_[0] = &Admin_Client::array_test;
  this->test_array_[1] = &Admin_Client::enum_test;
  this->test_array_[2] = &Admin_Client::alias_test;
  this->test_array_[3] = &Admin_Client::native_test;
  this->test_array_[4] = &Admin_Client::struct_test;
  this->test_array_[5] = &Admin_Client::union_test;
  this->test_array_[6] = &Admin_Client::exception_test;
  this->test_array_[7] = &Admin_Client::constant_test;
  this->test_array_[8] = &Admin_Client::interface_test;
  this->test_array_[9] = &Admin_Client::move_test;
  this->test_array_[10] = &Admin_Client::module_test;
}

Admin_Client::~Admin_Client (void)
{
}

// An array of test names, so we can step through them when
// trying to find a match for a test selected on the command line.
const char *Admin_Client::test_names_[] =
{
  "array",
  "enum",
  "alias",
  "native",
  "struct",
  "union",
  "exception",
  "constant",
  "interface",
  "move",
  "module"
};

int
Admin_Client::init (int argc,
                    char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      this->orb_ = CORBA::ORB_init (argc,
                                    argv,
                                    0
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int retval = this->parse_args (argc,
                                     argv);

      if (retval != 0)
        return retval;

      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("InterfaceRepository"
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
        CORBA::Repository::_narrow (object.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (this->repo_.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "CORBA::Repository::_narrow failed\n"),
                            -1);
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Admin_Client::init");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
Admin_Client::run (void)
{
  ACE_TRY_NEW_ENV
    {
      if (this->all_tests_ == 1)
        {
          for (CORBA::ULong i = 0; i < NUMBER_OF_TESTS; ++i)
            {
              // Each test is run twice to make sure everything
              // gets destroyed properly. If not, we get a
              // CORBA::BAD_PARAM exception the second time.
              for (CORBA::ULong j = 0; j < this->iterations_; ++j)
                {
                  (this->*test_array_[i])(ACE_ENV_SINGLE_ARG_PARAMETER);
                  ACE_TRY_CHECK;
                }
            }
        }
      else
        {
          for (CORBA::ULong j = 0; j < this->iterations_; ++j)
            {
              (this->*test_array_[this->which_test_])(ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Admin_Client::run");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
Admin_Client::parse_args (int argc,
                          char *argv[])
{
  ACE_Get_Opt opts (argc, argv, "di:t:");
  int c;
  int result = 0;
  CORBA::ULong i = 0;
  char *name = 0;

  while ((c = opts ()) != -1)
    switch (c)
      {
      case 'd':   // debug flag
        this->debug_ = 1;
        break;
      case 'i':   // iterations
        result = ACE_OS::atoi (opts.opt_arg ());
        if (result > 0)
          this->iterations_ = result;
        break;
      case 't':   // test selection
        this->all_tests_ = 0;
        name = opts.opt_arg ();

        for (i = 0; i < NUMBER_OF_TESTS; ++i)
          {
            if (ACE_OS::strcmp (name, this->test_names_[i]) == 0)
              {
                this->which_test_ = i;
                break;
              }
          }

        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage: %s"
                           " [-d]"
                           " [-i iterations]"
                           " [-t test name]"
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

void
Admin_Client::array_test (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("\n============== ARRAY TEST ==============\n\n")
    ));

  CORBA::IDLType_var atype = this->repo_->create_string (7
                                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ArrayDef_var avar = this->repo_->create_array (5,
                                                        atype.in ()
                                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ULong bound = avar->length (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("ArrayDef::length: %d\n"),
                bound));

  ACE_ASSERT (bound == 5);

  CORBA::TypeCode_var tc = avar->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::TCKind kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("ArrayDef::type::kind: %d\n"),
                kind));

  ACE_ASSERT (kind == CORBA::tk_array);

  tc = avar->element_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("ArrayDef::element_type::kind: %d\n"),
                kind));

  ACE_ASSERT (kind == CORBA::tk_string);

  CORBA::IDLType_var tdef = avar->element_type_def (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  CORBA::DefinitionKind dk = tdef->def_kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((
        LM_DEBUG,
        ACE_TEXT ("ArrayDef::element_type_def::def_kind: %d\n"),
        dk
      ));

  ACE_ASSERT (dk == CORBA::dk_String);

  CORBA::StringDef_var pvar = CORBA::StringDef::_narrow (tdef.in ()
                                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  bound = pvar->bound (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("StringDef::bound: %d\n"),
                bound));

  ACE_ASSERT (bound == 7);

  CORBA::PrimitiveDef_var pdef =
    this->repo_->get_primitive (CORBA::pk_short
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  avar->element_type_def (pdef.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("ArrayDef::element_type_def (set)\n")));

  tc = avar->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("ArrayDef::type::kind: %d\n"),
                kind));

  ACE_ASSERT (kind == CORBA::tk_array);

  tdef = avar->element_type_def (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  dk = tdef->def_kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("ArrayDef::element_type_def::def_kind: %d\n"),
                dk));

  ACE_ASSERT (dk == CORBA::dk_Primitive);

  CORBA::PrimitiveDef_var zvar =
    CORBA::PrimitiveDef::_narrow (tdef.in ()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  CORBA::PrimitiveKind pkind = zvar->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("PrimitiveDef::kind: %d\n"),
                pkind));

  ACE_ASSERT (pkind == CORBA::pk_short);

  avar->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
Admin_Client::enum_test (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("\n============== ENUM TEST ==============\n\n")
    ));

  CORBA::EnumMemberSeq members (4);
  members.length (3);

  members[0] = CORBA::string_dup ("ZERO");
  members[1] = CORBA::string_dup ("ONE");
  members[2] = CORBA::string_dup ("TWO");

  CORBA::EnumDef_var evar = this->repo_->create_enum ("IDL:my_enum:1.0",
                                                      "my_enum",
                                                      "1.0",
                                                      members
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::String_var str = evar->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("EnumDef::id: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "IDL:my_enum:1.0"));

  str = evar->name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("EnumDef::name: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "my_enum"));

  str = evar->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("EnumDef::absolute_name: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "::my_enum"));

  str = evar->version (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("EnumDef::version: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "1.0"));

  CORBA::DefinitionKind dkind = evar->def_kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("EnumDef::def_kind: %d\n\n"),
                dkind));

  ACE_ASSERT (dkind == CORBA::dk_Enum);

  CORBA::Contained::Description_var desc = evar->describe (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  dkind = desc->kind;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("EnumDef::describe::kind: %d\n"),
                dkind));

  ACE_ASSERT (dkind == CORBA::dk_Enum);

  CORBA::TypeDescription *td;
  desc->value >>= td;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("EnumDef::describe::value::name: %s\n"),
                td->name.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (td->name, "my_enum"));

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("EnumDef::describe::value::id: %s\n"),
                td->id.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (td->id, "IDL:my_enum:1.0"));

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("EnumDef::describe::value::defined_in: %s\n"),
                td->defined_in.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (td->defined_in, ""));

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("EnumDef::describe::value::version: %s\n"),
                td->version.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (td->version, "1.0"));

  CORBA::TCKind kind = td->type->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("EnumDef::describe::value::type::kind: %d\n"),
                kind));

  ACE_ASSERT (kind == CORBA::tk_enum);

  str = td->type->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("EnumDef::describe::value::type::id: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "IDL:my_enum:1.0"));

  str = td->type->name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("EnumDef::describe::value::type::name: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "my_enum"));

  CORBA::ULong count = td->type->member_count ();

  if (this->debug_)
    ACE_DEBUG ((
        LM_DEBUG,
        ACE_TEXT ("EnumDef::describe::value::type::member_count: %d\n\n"),
        count
      ));

  ACE_ASSERT (count == 3);

  CORBA::ULong i = 0;

  for (i = 0; i < count; i++)
    {
      str = td->type->member_name (i ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("EnumDef::describe::value::type::")
                    ACE_TEXT ("member_name[%d]: %s\n"),
                    i,
                    str.in ()));

      ACE_ASSERT (!ACE_OS::strcmp (str.in (), members[i]));
    }


  members.length (4);
  members[0] = CORBA::string_dup ("NOTHINGNESS");
  members[1] = CORBA::string_dup ("UNITY");
  members[2] = CORBA::string_dup ("DUALITY");
  members[3] = CORBA::string_dup ("TRINITY");

  evar->members (members
                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nEnumDef::members (set)\n\n")));

  CORBA::EnumMemberSeq_var fellows = evar->members (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  count = fellows->length ();
  const char *tmp = 0;

  for (i = 0; i < count; i++)
    {
      tmp = fellows[i];

      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("EnumDef::")
                    ACE_TEXT ("members[%d]: %s\n"),
                    i,
                    tmp));

      ACE_ASSERT (!ACE_OS::strcmp (fellows[i], members[i]));
    }

  evar->name ("another_enum"
              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  str = evar->name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nEnumDef::name (set)\n")));

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("EnumDef::name: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "another_enum"));

  str = evar->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("EnumDef::absolute_name: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "::another_enum"));

  evar->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
Admin_Client::alias_test (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("\n============== ALIAS TEST ==============\n\n")
    ));

  // This test also tests WstringDef and SequenceDef.

  CORBA::IDLType_var sq_elem = this->repo_->create_wstring (7
                                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::SequenceDef_var sq_var =
    this->repo_->create_sequence (5,
                                  sq_elem.in ()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::AliasDef_var a_var =
    this->repo_->create_alias ("IDL:my_alias:1.0",
                               "my_alias",
                               "1.0",
                               sq_var.in ()
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::String_var str = a_var->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("AliasDef::id: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "IDL:my_alias:1.0"));

  str = a_var->name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("AliasDef::name: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "my_alias"));

  str = a_var->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("AliasDef::absolute_name: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "::my_alias"));

  str = a_var->version (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("AliasDef::version: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "1.0"));

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("AliasDef::version (set)\n")));

  a_var->version ("1.1"
                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  str = a_var->version (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("AliasDef::version: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "1.1"));

  CORBA::TypeCode_var tc = a_var->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  str = tc->name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nAliasDef::type::name: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "my_alias"));

  CORBA::TCKind kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("AliasDef::type::kind: %d\n"),
                kind));

  ACE_ASSERT (kind == CORBA::tk_alias);

  CORBA::TypeCode_var ct = tc->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  kind = ct->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("AliasDef::type::content_type::kind: %d\n"),
                kind));

  ACE_ASSERT (kind == CORBA::tk_sequence);

  CORBA::ULong length = ct->length (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("AliasDef::type::content_type::length: %d\n"),
                length));

  ACE_ASSERT (length == 5);

  CORBA::TypeCode_var ct2 = ct->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  kind = ct2->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("AliasDef::type::content_type::")
                ACE_TEXT ("content_type::kind: %d\n"),
                kind));

  ACE_ASSERT (kind == CORBA::tk_wstring);

  length = ct2->length (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("AliasDef::type::content_type::")
                ACE_TEXT ("content_type::length: %d\n"),
                length));

  ACE_ASSERT (length == 7);

  CORBA::Contained::Description_var desc = a_var->describe (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::TypeDescription *td;
  desc->value >>= td;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nAliasDef::describe::value::name: %s\n"),
                td->name.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (td->name, "my_alias"));

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("AliasDef::describe::value::version: %s\n"),
                td->version.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (td->version, "1.1"));

  kind = td->type->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("AliasDef::describe::value::type::kind: %d\n"),
                kind));

  ACE_ASSERT (kind == CORBA::tk_alias);

  str = td->type->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("AliasDef::describe::value::type::id: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "IDL:my_alias:1.0"));

  str = td->type->name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("AliasDef::describe::value::type::name: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "my_alias"));

  CORBA::IDLType_var i_var = a_var->original_type_def (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::SequenceDef_var seq_var =
    CORBA::SequenceDef::_narrow (i_var.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  length = seq_var->bound (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("AliasDef::original_type_def::bound: %d\n"),
                length));

  ACE_ASSERT (length == 5);

  a_var->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
Admin_Client::native_test (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("\n============== NATIVE TEST ==============\n\n")
    ));

  CORBA::NativeDef_var nvar =
    this->repo_->create_native ("IDL:my_native:1.0",
                                "my_native",
                                "1.0"
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::String_var str = nvar->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("NativeDef::id: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "IDL:my_native:1.0"));

  str = nvar->name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("NativeDef::name: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "my_native"));

  str = nvar->version (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("NativeDef::version: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "1.0"));

  str = nvar->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("NativeDef::absolute_name: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "::my_native"));

  CORBA::DefinitionKind kind = nvar->def_kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("NativeDef::def_kind: %d\n"),
                kind));

  ACE_ASSERT (kind == CORBA::dk_Native);

  nvar->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
Admin_Client::struct_test (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("\n============== STRUCT TEST ==============\n\n")
    ));

  CORBA::StructMemberSeq members (3);
  members.length (3);

  members[0].name = CORBA::string_dup ("ub_string");
  members[0].type_def = this->repo_->get_primitive (CORBA::pk_string
                                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  members[0].type = CORBA::TypeCode::_duplicate (CORBA::_tc_void);

  members[1].name = CORBA::string_dup ("bd_string");
  members[1].type_def = this->repo_->create_string (5
                                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  members[1].type = CORBA::TypeCode::_duplicate (CORBA::_tc_void);

  CORBA::EnumMemberSeq e_members (2);
  e_members.length (2);

  e_members[0] = CORBA::string_dup ("ZERO");
  e_members[1] = CORBA::string_dup ("ONE");

  members[2].name = CORBA::string_dup ("my_struct_enum");

  CORBA::EnumDef_var e_var =
    this->repo_->create_enum ("IDL:my_enum:1.0",
                              "my_enum",
                              "1.0",
                              e_members
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  members[2].type_def = CORBA::EnumDef::_duplicate (e_var.in ());
  members[2].type = CORBA::TypeCode::_duplicate (CORBA::_tc_void);

  CORBA::StructDef_var svar =
    this->repo_->create_struct ("IDL:my_struct:1.0",
                                "my_struct",
                                "1.0",
                                members
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // This also tests the members() function.
  CORBA::TypeCode_var tc = svar->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ULong length = tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("StructDef::type::length: %d\n"),
                length));

  ACE_ASSERT (length == 3);

  CORBA::String_var str;
  CORBA::ULong i = 0;

  for (i = 0; i < length; i++)
    {
      str = tc->member_name (i ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("StructDef::type::member_name[%d]: %s\n"),
                    i,
                    str.in ()));

      ACE_ASSERT (!ACE_OS::strcmp (str.in (), members[i].name));
    }

  CORBA::Contained::Description_var desc = svar->describe (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::TypeDescription *td;
  desc->value >>= td;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nStructDef::describe::value::name: %s\n"),
                td->name.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (td->name, "my_struct"));

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("StructDef::describe::value::id: %s\n"),
                td->id.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (td->id, "IDL:my_struct:1.0"));

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("StructDef::describe::value::version: %s\n"),
                td->version.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (td->version, "1.0"));

  members.length (2);
  members[0].name = CORBA::string_dup ("long_mem");
  members[0].type_def = this->repo_->get_primitive (CORBA::pk_long
                                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  members[0].type = CORBA::TypeCode::_duplicate (CORBA::_tc_void);

  members[1].name = CORBA::string_dup ("array_mem");
  members[1].type_def =
    this->repo_->create_array (5,
                               members[0].type_def.in ()
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  members[1].type = CORBA::TypeCode::_duplicate (CORBA::_tc_void);

  svar->members (members
                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nStructDef::members (set)\n\n")));

  CORBA::EnumMemberSeq def_members (2);
  def_members.length (2);

  def_members[0] = CORBA::string_dup ("FIRST");
  def_members[1] = CORBA::string_dup ("SECOND");

  CORBA::EnumDef_var e_def_var = svar->create_enum ("IDL:my_def_enum:1.0",
                                                    "my_enum",
                                                    "1.0",
                                                    def_members
                                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::StructMemberSeq_var out_members = svar->members (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  length = out_members->length ();

  for (i = 0; i < length; i++)
    {
      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("StructDef::members[%d]::name: %s\n"),
                    i,
                    out_members[i].name.in ()));

      if (i == length - 1)
        ACE_ASSERT (!ACE_OS::strcmp (out_members[i].name, "my_enum"));
      else
        ACE_ASSERT (!ACE_OS::strcmp (out_members[i].name, members[i].name));
    }

  CORBA::Contained_var fox = this->repo_->lookup ("::my_struct::my_enum"
                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  str = fox->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Repository::lookup::absolute_name: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "::my_struct::my_enum"));

  fox = svar->lookup ("my_enum"
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  str = fox->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("StructDef::lookup::absolute_name: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "::my_struct::my_enum"));

  CORBA::Container_var outer = fox->defined_in (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  CORBA::Contained_var schizo = CORBA::Contained::_narrow (outer.in ()
                                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  str = schizo->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("StructDef::lookup::defined_in::")
                ACE_TEXT ("absolute_name: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "::my_struct"));

  CORBA::ContainedSeq_var contents = this->repo_->contents (CORBA::dk_all,
                                                            0
                                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  length = contents->length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nRepository::contents::length: %d\n"),
                length));

  ACE_ASSERT (length == 2);

  contents = svar->contents (CORBA::dk_all,
                             0
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  length = contents->length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("StructDef::contents::length: %d\n"),
                length));

  ACE_ASSERT (length == 1);

  for (i = 0; i < length; ++i)
    {
      str = contents[i]->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (this->debug_)
        ACE_DEBUG ((
            LM_DEBUG,
            ACE_TEXT ("StructDef::contents[%d]::absolute_name: %s\n"),
            i,
            str.in ()
          ));

      if (i == 0)
        {
          ACE_ASSERT (!ACE_OS::strcmp (str.in (), "::my_struct::my_enum"));
        }
      else
        {
          // No more found in lookup.
        }
    }

  contents = this->repo_->lookup_name ("my_enum",
                                       -1,
                                       CORBA::dk_all,
                                       0
                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  length = contents->length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nRepository::lookup_name::length: %d\n"),
                length));

  ACE_ASSERT (length == 2);

  for (i = 0; i < length; i++)
    {
      str = contents[i]->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (this->debug_)
        ACE_DEBUG ((
            LM_DEBUG,
            ACE_TEXT ("Repository::lookup_name[%d]::absolute_name: %s\n"),
            i,
            str.in ()
          ));

      // Can't use ACE_ASSERT here because we don't know the order
      // of the results of the search.
    }

  CORBA::Container::DescriptionSeq_var cont_desc =
    this->repo_->describe_contents (CORBA::dk_all,
                                    0,
                                    -1
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  length = cont_desc->length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nRepository::describe_contents::length: %d\n"),
                length));

  ACE_ASSERT (length == 2);

  for (i = 0; i < length; i++)
    {
      CORBA::TypeDescription *td;
      cont_desc[i].value >>= td;

      str = td->type->id (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (this->debug_)
        ACE_DEBUG ((
            LM_DEBUG,
            ACE_TEXT ("Repository::describe_contents[%d]::id: %s\n"),
            i,
            str.in ()
          ));

      if (i == 0)
        {
          if (ACE_OS::strcmp (str.in (), "IDL:my_enum:1.0"))
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("struct_test::describe_contents -")
                          ACE_TEXT ("incorrect repo id in item %d"),
                          i));

              return;
            }
        }
      else if (i == 1)
        {
          if (ACE_OS::strcmp (str.in (), "IDL:my_struct:1.0"))
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("struct_test::describe_contents -")
                          ACE_TEXT ("incorrect repo id in item %d"),
                          i));

              return;
            }
        }
    }

  svar->name ("your_struct"
              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nStructDef::name (set)\n")));

  contents = svar->contents (CORBA::dk_all,
                             0
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  length = contents->length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("StructDef::contents::length: %d\n"),
                length));

  ACE_ASSERT (length == 1);

  const char *tmp = "::your_struct";

  for (i = 0; i < length; i++)
    {
      str = contents[i]->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (this->debug_)
        ACE_DEBUG ((
            LM_DEBUG,
            ACE_TEXT ("StructDef::contents[%d]::absolute_name: %s\n"),
            i,
            str.in ()
          ));

      // Whatever the scoped name now is, it must begin
      // with "::your_struct".
      ACE_ASSERT (!ACE_OS::strncmp (str.in (),
                                    tmp,
                                    ACE_OS::strlen (tmp)));
    }

#if defined (ACE_NDEBUG)
  // ACE_ASSERT macro expands to nothing, so...
  ACE_UNUSED_ARG (tmp);
#endif /* ACE_NDEBUG */

  e_var->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  svar->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
Admin_Client::union_test (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("\n============== UNION TEST ==============\n\n")
    ));

  CORBA::EnumMemberSeq d_members (4);
  d_members.length (4);

  d_members[0] = CORBA::string_dup ("ZERO");
  d_members[1] = CORBA::string_dup ("ONE");
  d_members[2] = CORBA::string_dup ("TWO");
  d_members[3] = CORBA::string_dup ("THREE");

  CORBA::EnumDef_var d_var =
    this->repo_->create_enum ("IDL:disc_enum:1.0",
                              "disc_enum",
                              "1.0",
                              d_members
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::StructMemberSeq s_members (1);
  s_members.length (1);
  s_members[0].name = CORBA::string_dup ("string_in_struct");
  s_members[0].type_def = this->repo_->create_string (6
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  s_members[0].type = CORBA::TypeCode::_duplicate (CORBA::_tc_void);

  CORBA::StructDef_var s_var =
    this->repo_->create_struct ("IDL:struct_in_union:1.0",
                                "struct_in_union",
                                "1.0",
                                s_members
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::UnionMemberSeq u_members (4);
  u_members.length (4);

  u_members[0].name = CORBA::string_dup ("longval");
  u_members[0].type_def = this->repo_->get_primitive (CORBA::pk_long
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  u_members[0].type = CORBA::TypeCode::_duplicate (CORBA::_tc_void);
  CORBA::TypeCode_var d_type = d_var->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  TAO_OutputCDR maker2;
  maker2.write_ulong (3);  // THREE
  CORBA::Any any2 (d_type.in (),
                   0,
                   TAO_ENCAP_BYTE_ORDER,
                   maker2.begin ());
  u_members[0].label = any2;

  u_members[1].name = CORBA::string_dup ("longval");
  u_members[1].type_def = this->repo_->get_primitive (CORBA::pk_long
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  u_members[1].type = CORBA::TypeCode::_duplicate (CORBA::_tc_void);
  TAO_OutputCDR maker0;
  maker0.write_ulong (2);  // TWO
  CORBA::Any any0 (d_type.in (),
                   0,
                   TAO_ENCAP_BYTE_ORDER,
                   maker0.begin ());
  u_members[1].label = any0;

  u_members[2].name = CORBA::string_dup ("structval");
  u_members[2].type_def = CORBA::StructDef::_duplicate (s_var.in ());
  u_members[2].type = CORBA::TypeCode::_duplicate (CORBA::_tc_void);

  TAO_OutputCDR maker1;
  maker1.write_ulong (0); // ZERO
  CORBA::Any any1 (d_type.in (),
                   0,
                   TAO_ENCAP_BYTE_ORDER,
                   maker1.begin ());
  u_members[2].label = any1;

  u_members[3].name = CORBA::string_dup ("stringval");
  u_members[3].type_def = this->repo_->create_string (17
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  u_members[3].type = CORBA::TypeCode::_duplicate (CORBA::_tc_void);
  u_members[3].label <<= CORBA::Any::from_octet (0);   // default case (ONE)

  CORBA::UnionDef_var u_var =
    this->repo_->create_union ("IDL:my_union:1.0",
                               "my_union",
                               "1.0",
                               d_var.in (),
                               u_members
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::TypeCode_var disc_tc = u_var->discriminator_type (ACE_ENV_SINGLE_ARG_PARAMETER);
   ACE_CHECK;

  CORBA::ULong length = disc_tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((
        LM_DEBUG,
        ACE_TEXT ("UnionDef::discriminator_type::member_count: %d\n"),
        length
      ));

  ACE_ASSERT (length == 4);

  CORBA::TypeCode_var tc = u_var->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  length = tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("UnionDef::type::member_count: %d\n"),
                length));

  ACE_ASSERT (length == 3);

  CORBA::Long slot = tc->default_index (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("UnionDef::type::default_index: %d\n\n"),
                slot));

  ACE_ASSERT (slot == 2);

  CORBA::String_var str;
  const char *tmp = 0;

  for (CORBA::ULong i = 0; i < length; i++)
    {
      str = tc->member_name (i
                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("UnionDef::type::member_name[%d]: %s\n"),
                    i,
                    str.in ()));

      // Multiple labels for first member shifts index by 1.
      ACE_ASSERT (!ACE_OS::strcmp (str.in (), u_members[i + 1].name));

      CORBA::Any_var label = tc->member_label (i
                                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      TAO_InputCDR cdr (label->_tao_get_cdr ());
      CORBA::ULong val;

      // If we're at the default index, it's in the label as octet 0,
      // so just assign the slot value to val.
      if (i != (CORBA::ULong) slot)
        {
          cdr.read_ulong (val);
          tmp = d_members[val];
        }

      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("UnionDef::type::member_label[%d]: %s\n"),
                    i,
                    i == (CORBA::ULong) slot ? "default" : tmp));

      // Labels are not in order.
      switch (i)
      {
        case 0:
          ACE_ASSERT (val == 3 || val == 2);
          break;
        case 1:
          ACE_ASSERT (val == 0);
          break;
        default:
          break;
      }
    }

  u_var->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  s_var->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  d_var->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
Admin_Client::exception_test (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("\n============== EXCEPTION TEST ==============\n\n")
    ));

  CORBA::StructMemberSeq members (3);
  members.length (3);

  members[0].name = CORBA::string_dup ("ub_string");
  members[0].type_def = this->repo_->get_primitive (CORBA::pk_string
                                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  members[0].type = members[0].type_def->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  members[1].name = CORBA::string_dup ("bd_string");
  members[1].type_def = this->repo_->create_string (5
                                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  members[1].type = members[1].type_def->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::EnumMemberSeq e_members (2);
  e_members.length (2);

  e_members[0] = CORBA::string_dup ("ZERO");
  e_members[1] = CORBA::string_dup ("ONE");

  members[2].name = CORBA::string_dup ("my_exception_enum");
  CORBA::EnumDef_var e_var = this->repo_->create_enum ("IDL:my_enum:1.0",
                                                       "my_enum",
                                                       "1.0",
                                                       e_members
                                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  members[2].type_def = CORBA::EnumDef::_duplicate (e_var.in ());

  members[2].type = members[2].type_def->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ExceptionDef_var exvar =
    this->repo_->create_exception ("IDL:my_exception:1.0",
                                   "my_exception",
                                   "1.0",
                                   members
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::TypeCode_var tc = exvar->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ULong length = tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("ExceptionDef::type::member_count: %d\n"),
                length));

  ACE_ASSERT (length == 3);

  CORBA::String_var str;
  CORBA::ULong i = 0;

  for (i = 0; i < length; i++)
    {
      str = tc->member_name (i ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("ExceptionDef::type::member_name[%d]: %s\n"),
                    i,
                    str.in ()));

      ACE_ASSERT (!ACE_OS::strcmp (str.in (), members[i].name));
    }

  CORBA::Contained::Description_var desc = exvar->describe (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ExceptionDescription *ed;
  desc->value >>= ed;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nExceptionDef::describe::value::name: %s\n"),
                ed->name.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (ed->name, "my_exception"));

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("ExceptionDef::describe::value::id: %s\n"),
                ed->id.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (ed->id, "IDL:my_exception:1.0"));

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("ExceptionDef::describe::value::defined_in: %s\n"),
                ed->defined_in.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (ed->defined_in, ""));

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("ExceptionDef::describe::value::version: %s\n"),
                ed->version.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (ed->version, "1.0"));

  members.length (2);
  members[0].name = CORBA::string_dup ("long_mem");
  members[0].type_def = this->repo_->get_primitive (CORBA::pk_long
                                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  members[0].type = members[0].type_def->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  members[1].name = CORBA::string_dup ("array_mem");
  CORBA::ArrayDef_ptr a_ptr =
    this->repo_->create_array (5,
                               members[0].type_def.in ()
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  members[1].type_def = a_ptr;
  members[1].type = members[1].type_def->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  exvar->members (members
                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nExceptionDef::members (set)\n\n")));

  CORBA::EnumMemberSeq def_members (2);
  def_members.length (2);

  def_members[0] = CORBA::string_dup ("FIRST");
  def_members[1] = CORBA::string_dup ("SECOND");

  CORBA::EnumDef_var e_def_var = exvar->create_enum ("IDL:my_def_enum:1.0",
                                                     "my_enum",
                                                     "1.0",
                                                     def_members
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::StructMemberSeq_var out_members = exvar->members (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  length = out_members->length ();

  for (i = 0; i < length; i++)
    {
      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("ExceptionDef::members[%d]::name: %s\n"),
                    i,
                    out_members[i].name.in ()));

      if (i == 2)
        ACE_ASSERT (!ACE_OS::strcmp (out_members[i].name, "my_enum"));
      else
        ACE_ASSERT (!ACE_OS::strcmp (out_members[i].name, members[i].name));
    }

  a_ptr->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nArrayDef::destroy\n\n")));

  out_members = exvar->members (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  length = out_members->length ();

  for (i = 0; i < length; i++)
    {
      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("ExceptionDef::members[%d]::name: %s\n"),
                    i,
                    out_members[i].name.in ()));

      if (i == 1)
        ACE_ASSERT (!ACE_OS::strcmp (out_members[i].name, "my_enum"));
      else
        ACE_ASSERT (!ACE_OS::strcmp (out_members[i].name, members[i].name));
    }

  CORBA::Contained_var fox = this->repo_->lookup ("::my_exception::my_enum"
                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  str = fox->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nRepository::lookup: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "::my_exception::my_enum"));

  fox = exvar->lookup ("my_enum"
                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  str = fox->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("ExceptionDef::lookup: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "::my_exception::my_enum"));

  CORBA::ContainedSeq_var contents = this->repo_->contents (CORBA::dk_all,
                                                            0
                                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  length = contents->length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nRepository::contents::length: %d\n"),
                length));

  ACE_ASSERT (length == 2);

  contents = exvar->contents (CORBA::dk_all,
                              0
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  length = contents->length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("ExceptionDef::contents::length: %d\n"),
                length));

  ACE_ASSERT (length == 1);

  for (i = 0; i < length; i++)
    {
      str = contents[i]->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (this->debug_)
        ACE_DEBUG ((
            LM_DEBUG,
            ACE_TEXT ("ExceptionDef::contents[%d]::absolute_name: %s\n"),
            i,
            str.in ()
          ));

      // Should be only the one member.
      ACE_ASSERT (!ACE_OS::strcmp (str.in (), "::my_exception::my_enum"));
    }

  contents = this->repo_->lookup_name ("my_enum",
                                       -1,
                                       CORBA::dk_all,
                                       0
                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  length = contents->length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nRepository::lookup_name::length: %d\n"),
                length));

  ACE_ASSERT (length == 2);

  for (i = 0; i < length; i++)
    {
      str = contents[i]->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (this->debug_)
        ACE_DEBUG ((
            LM_DEBUG,
            ACE_TEXT ("Repository::lookup_name%d]::absolute_name: %s\n"),
            i,
            str.in ()
          ));

      // Can't use ACE_ASSERT here - order of search results is unknown.
    }

  CORBA::Container::DescriptionSeq_var cont_desc =
    this->repo_->describe_contents (CORBA::dk_all,
                                    0,
                                    -1
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  length = cont_desc->length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nRepository::describe_contents::length: %d\n"),
                length));

  ACE_ASSERT (length == 2);

  CORBA::TypeDescription *td;
  for (i = 0; i < length; i++)
    {
      CORBA::DefinitionKind kind = cont_desc[i].kind;
      if (kind == CORBA::dk_Exception)
        {
          cont_desc[i].value >>= ed;
          CORBA::TypeCode_ptr tc = ed->type.in ();

          length = tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          if (this->debug_)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("Repository::describe_contents[%d]")
                        ACE_TEXT ("::value::type::member_count: %d\n"),
                        i,
                        length));

          ACE_ASSERT (length == 2);

          if (this->debug_)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("Repository::describe_contents[%d]")
                        ACE_TEXT ("::value::name: %s\n"),
                        i,
                        ed->name.in ()));

          ACE_ASSERT (!ACE_OS::strcmp (ed->name, "my_exception"));
        }
      else
        {
          cont_desc[i].value >>= td;
          CORBA::TypeCode_ptr tc = td->type.in ();

          length = tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          if (this->debug_)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("Repository::describe_contents[%d]")
                        ACE_TEXT ("::value::type::member_count: %d\n"),
                        i,
                        length));

          ACE_ASSERT (length == 2);

          if (this->debug_)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("Repository::describe_contents[%d]")
                        ACE_TEXT ("::value::name: %s\n"),
                        i,
                        td->name.in ()));

          ACE_ASSERT (!ACE_OS::strcmp (td->name, "my_enum"));
        }
    }

  exvar->name ("your_exception"
               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nExceptionDef::name (set)\n\n")));

  contents = exvar->contents (CORBA::dk_all,
                              0
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  length = contents->length ();
  const char *tmp = "::your_exception";

  for (i = 0; i < length; i++)
    {
      str = contents[i]->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (this->debug_)
        ACE_DEBUG ((
            LM_DEBUG,
            ACE_TEXT ("ExceptionDef::contents[%d]::absolute_name: %s\n"),
            i,
            str.in ()
          ));

      ACE_ASSERT (!ACE_OS::strncmp (str.in (), tmp, ACE_OS::strlen (tmp)));
    }

#if defined (ACE_NDEBUG)
  // ACE_ASSERT macro expands to nothing, so...
  ACE_UNUSED_ARG (tmp);
#endif /* ACE_NDEBUG */

  e_var->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  exvar->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
Admin_Client::constant_test (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("\n============== CONSTANT TEST ==============\n\n")
    ));

  CORBA::IDLType_var ivar = this->repo_->get_primitive (CORBA::pk_string
                                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Any any;
  const char *s = "hello";
  any <<= s;

  CORBA::ConstantDef_var cvar =
    this->repo_->create_constant ("IDL:my_constant:1.0",
                                  "my_constant",
                                  "1.0",
                                  ivar.in (),
                                  any
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::TypeCode_var tc = cvar->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  CORBA::TCKind kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("ConstantDef::type::kind: %d\n"),
                kind));

  ACE_ASSERT (kind == CORBA::tk_string);

  CORBA::IDLType_var tdef = cvar->type_def (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  CORBA::DefinitionKind def_kind = tdef->def_kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("ConstantDef::type_def::def_kind: %d\n"),
                def_kind));

  ACE_ASSERT (def_kind == CORBA::dk_Primitive);

  CORBA::Any_var out_any;
  out_any = cvar->value (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  const char *out_s;
  out_any >>= out_s;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("ConstantDef::value (string): %s\n"),
                out_s));

  ACE_ASSERT (!ACE_OS::strcmp (out_s, s));

  ivar = this->repo_->get_primitive (CORBA::pk_double
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  cvar->type_def (ivar.in ()
                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Double double_val = -1223.42256;
  any <<= double_val;
  cvar->value (any
               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nConstantDef::type_def (set)\n")
                ACE_TEXT ("ConstantDef::value (set)\n")));

  out_any = cvar->value (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Double out_double_val;
  out_any >>= out_double_val;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("ConstantDef::value (double): %.5f\n"),
                out_double_val));

  ACE_ASSERT (out_double_val == double_val);

  ivar = this->repo_->get_primitive (CORBA::pk_short
                                     ACE_ENV_ARG_PARAMETER);
  cvar->type_def (ivar.in ()
                  ACE_ENV_ARG_PARAMETER);

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nConstantDef::type_def (set)\n")
                ACE_TEXT ("ConstantDef::value (set)\n")));

  CORBA::Short short_val = -65;
  any <<= short_val;

  cvar->value (any
               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  out_any = cvar->value (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Short out_short_val;
  out_any >>= out_short_val;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("ConstantDef::value (short): %hd\n"),
                out_short_val));

  ACE_ASSERT (out_short_val == short_val);

  ivar = this->repo_->get_primitive (CORBA::pk_float
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  cvar->type_def (ivar.in ()
                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nConstantDef::type_def (set)\n")
                ACE_TEXT ("ConstantDef::value (set)\n")));

  CORBA::Float float_val = 2.33f;
  any <<= float_val;

  cvar->value (any
               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  out_any = cvar->value (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Float out_float_val;
  out_any >>= out_float_val;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("ConstantDef::value (float): %.2f\n"),
                out_float_val));

  ACE_ASSERT (out_float_val == float_val);

  ivar = this->repo_->get_primitive (CORBA::pk_ulonglong
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  cvar->type_def (ivar.in ()
                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nConstantDef::type_def (set)\n")
                ACE_TEXT ("ConstantDef::value (set)\n")));

  CORBA::ULongLong ull_val = 1234567890;
  any <<= ull_val;

  cvar->value (any
               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  out_any = cvar->value (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ULongLong out_ull_val;
  out_any >>= out_ull_val;

  if (this->debug_)
    {
#if defined (ACE_LACKS_LONGLONG_T)
      char buffer[32];
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("ConstantDef::value (ulonglong): %s\n"),
                  out_ull_val.as_string (buffer)));
#else
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("ConstantDef::value (ulonglong): %Q\n"),
                  out_ull_val));
#endif
    }
  ACE_ASSERT (out_ull_val == ull_val);

  cvar->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
Admin_Client::interface_test (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("\n============== INTERFACE TEST ==============\n\n")
    ));

  CORBA::ULong i, length;

  CORBA::InterfaceDefSeq in_bases (1);
  in_bases.length (0);

  CORBA::InterfaceDef_var gp_ivar =
    this->repo_->create_interface ("IDL:gp_iface:1.0",
                                   "gp_iface",
                                   "1.0",
                                   in_bases
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::PrimitiveDef_var p_long =
    this->repo_->get_primitive (CORBA::pk_long
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::AttributeDef_var gp_attr =
    gp_ivar->create_attribute ("IDL:gp_iface/gp_attr:1.0",
                               "gp_attr",
                               "1.0",
                               p_long.in (),
                               CORBA::ATTR_NORMAL
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  in_bases.length (1);
  in_bases[0] = CORBA::InterfaceDef::_duplicate (gp_ivar.in ());

  CORBA::InterfaceDef_var p_ivar =
    this->repo_->create_interface ("IDL:p_iface:1.0",
                                   "p_iface",
                                   "1.0",
                                   in_bases
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::StringDef_var p_string = this->repo_->create_string (5
                                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::AttributeDef_var p_attr =
    p_ivar->create_attribute ("IDL:p_iface/p_attr:1.0",
                              "p_attr",
                              "1.0",
                              p_string.in (),
                              CORBA::ATTR_READONLY
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const char *names[] = {"inarg", "inoutarg", "outarg"};

  const CORBA::ParameterMode modes[] =
    {CORBA::PARAM_IN, CORBA::PARAM_INOUT, CORBA::PARAM_OUT};

  const char *contexts[] = {"straw", "sticks", "bricks"};

  length = 3;
  CORBA::ParDescriptionSeq par_seq (3);
  par_seq.length (3);

  CORBA::StructMemberSeq members (0);
  members.length (0);

  CORBA::ExceptionDef_var ex_var =
    this->repo_->create_exception ("IDL:if_exception:1.0",
                                   "if_exception",
                                   "1.0",
                                   members
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ExceptionDefSeq get_seq (1);
  get_seq.length (1);
  get_seq[0] = CORBA::ExceptionDef::_duplicate (ex_var.in ());

  CORBA::ContextIdSeq con_seq (3);
  con_seq.length (3);

  for (i = 0; i < length; ++i)
    {
      par_seq[i].name = names[i];
      par_seq[i].type_def = CORBA::PrimitiveDef::_duplicate (p_long.in ());
      par_seq[i].type = p_long->type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
      par_seq[i].mode = modes[i];
      con_seq[i] = contexts[i];
    }

  CORBA::OperationDef_var p_op =
    p_ivar->create_operation ("IDL:p_iface/p_op:1.0",
                              "p_op",
                              "1.0",
                              p_long.in (),
                              CORBA::OP_NORMAL,
                              par_seq,
                              get_seq,
                              con_seq
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Contained::Description_var desc = p_op->describe (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::OperationDescription *od;
  desc->value >>= od;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("OperationDef::describe::value::")
                ACE_TEXT ("defined_in: %s\n"),
                od->defined_in.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (od->defined_in, "IDL:p_iface:1.0"));

  CORBA::TypeCode_var result = od->result;
  CORBA::TCKind kind = result->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("OperationDef::describe::result::")
                ACE_TEXT ("kind: %d\n"),
                kind));

  ACE_ASSERT (kind == CORBA::tk_long);

  length = od->parameters.length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nOperationDef::describe::parameters::")
                ACE_TEXT ("length: %d\n"),
                length));

  ACE_ASSERT (length == 3);

  const char *tmp = 0;

  for (i = 0; i < length; i++)
    {
      tmp = od->parameters[i].name;

      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("OperationDef::describe::parameters[%d]::")
                    ACE_TEXT ("name: %s\n"),
                    i,
                    tmp));

      ACE_ASSERT (!ACE_OS::strcmp (tmp, names[i]));

      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("OperationDef::describe::parameters[%d]::")
                    ACE_TEXT ("mode: %d\n"),
                    i,
                    od->parameters[i].mode));

      ACE_ASSERT (od->parameters[i].mode == modes[i]);
    }

  CORBA::Boolean is_it = p_ivar->is_a ("IDL:p_iface:1.0"
                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nInterfaceDef::is_a (same class): %hd\n"),
                is_it));

  ACE_ASSERT (is_it == 1);

  is_it = p_ivar->is_a ("IDL:gp_iface:1.0"
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("InterfaceDef::is_a (base class): %hd\n"),
                is_it));

  ACE_ASSERT (is_it == 1);

  is_it = gp_ivar->is_a ("IDL:p_iface:1.0"
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("InterfaceDef::is_a (derived class): %hd\n"),
                is_it));

  ACE_ASSERT (is_it == 0);

  CORBA::InterfaceDef::FullInterfaceDescription_var fifd =
    p_ivar->describe_interface (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::String_var str = fifd->type->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nInterfaceDef::describe_interface::")
                ACE_TEXT ("type::id: %s\n"),
                str.in ()));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (), "IDL:p_iface:1.0"));

  length = fifd->operations.length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nInterfaceDef::describe_interface::")
                ACE_TEXT ("operations::length: %d\n"),
                length));

  ACE_ASSERT (length == 4);

  length = fifd->operations[3].contexts.length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nInterfaceDef::describe_interface::")
                ACE_TEXT ("operations[3]::contexts::length: %d\n"),
                length));

  ACE_ASSERT (length == 3);

  for (i = 0; i < length; i++)
    {
      tmp = fifd->operations[3].contexts[i];

      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("InterfaceDef::describe_interface::")
                    ACE_TEXT ("operations[0]::contexts[%d]: %s\n"),
                    i,
                    tmp));

      ACE_ASSERT (!ACE_OS::strcmp (tmp, contexts[i]));
    }

  length = fifd->operations[3].exceptions.length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nInterfaceDef::describe_interface::")
                ACE_TEXT ("operations[0]::exceptions::length: %d\n"),
                length));

  ACE_ASSERT (length == 1);

  for (i = 0; i < length; i++)
    {
      const char *tmp = fifd->operations[3].exceptions[i].name;

      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("InterfaceDef::describe::operations[3]::")
                    ACE_TEXT ("contexts[%d]: %s\n"),
                    i,
                    tmp));

      ACE_ASSERT (!ACE_OS::strcmp (tmp, "if_exception"));
    }

  tmp = fifd->attributes[0].defined_in;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nInterfaceDef::describe_interface::")
                ACE_TEXT ("attributes::defined_in: %s\n"),
                tmp));

  ACE_ASSERT (!ACE_OS::strcmp (tmp, "IDL:gp_iface:1.0"));

  in_bases[0] = CORBA::InterfaceDef::_duplicate (p_ivar.in ());

  CORBA::InterfaceDef_var ivar =
    this->repo_->create_interface ("IDL:iface:1.0",
                                   "iface",
                                   "1.0",
                                   in_bases
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  desc = ivar->describe (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::InterfaceDescription *ifd;
  desc->value >>= ifd;

  length = ifd->base_interfaces.length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nInterfaceDef::describe::")
                ACE_TEXT ("base_interfaces::length: %d\n"),
                length));

  ACE_ASSERT (length == 2);

  const char *base_iface_id = 0;

  for (i = 0; i < length; i++)
    {
      base_iface_id = ifd->base_interfaces[i].in ();

      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("InterfaceDef::describe::")
                    ACE_TEXT ("base_interfaces[%d]: %s\n"),
                    i,
                    base_iface_id));
    }

  CORBA::InterfaceDefSeq_var out_bases =
    ivar->base_interfaces (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  length = out_bases->length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nInterfaceDef::base_interfaces::length: %d\n"),
                length));

  ACE_ASSERT (length == 2);

  for (i = 0; i < length; i++)
    {
      str = out_bases[i]->name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (this->debug_)
        ACE_DEBUG ((
            LM_DEBUG,
            ACE_TEXT ("InterfaceDef::base_interfaces[%d]::name: %s\n"),
            i,
            str.in ()
          ));
    }

  CORBA::ContainedSeq_var contents = ivar->contents (CORBA::dk_all,
                                                     0
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  length = contents->length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nInterfaceDef::contents::length: %d\n"),
                length));

  ACE_ASSERT (length == 6);

  for (i = 0; i < length; i++)
    {
      str = contents[i]->name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("InterfaceDef::contents[%d]::name: %s\n"),
                    i,
                    str.in ()));

      CORBA::Container_var cr = contents[i]->defined_in (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::Contained_var cd = CORBA::Contained::_narrow (cr.in ()
                                                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      str = cd->name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("InterfaceDef::contents[%d]::")
                    ACE_TEXT ("defined_in::name: %s\n"),
                    i,
                    str.in ()));

      ACE_ASSERT (!ACE_OS::strcmp (str.in (), "p_iface")
                  || !ACE_OS::strcmp (str.in (), "gp_iface"));
    }

  ex_var->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  ivar->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  gp_ivar->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  p_ivar->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
Admin_Client::move_test (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("\n============== MOVE TEST ==============\n\n")
    ));

  CORBA::EnumMemberSeq e_members (2);
  e_members.length (2);

  e_members[0] = CORBA::string_dup ("ZERO");
  e_members[1] = CORBA::string_dup ("ONE");

  CORBA::EnumDef_var e_var = this->repo_->create_enum ("IDL:o_enum:1.0",
                                                       "o_enum",
                                                       "1.0",
                                                       e_members
                                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  e_members[0] = CORBA::string_dup ("TEN");
  e_members[1] = CORBA::string_dup ("ELEVEN");

  CORBA::EnumDef_var e_var2 = this->repo_->create_enum ("IDL:i_enum:1.0",
                                                        "i_enum",
                                                        "1.0",
                                                        e_members
                                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::StructMemberSeq s_members (2);
  s_members.length (2);

  const char *s_names[] = {"s_string", "s_enum"};

  s_members[0].name = s_names[0];
  s_members[0].type_def = this->repo_->get_primitive (CORBA::pk_string
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  s_members[0].type = s_members[0].type_def->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  s_members[1].name = s_names[1];
  s_members[1].type_def = CORBA::EnumDef::_duplicate (e_var2.in ());
  s_members[1].type = s_members[1].type_def->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::StructDef_var s_var =
    this->repo_->create_struct ("IDL:o_struct:1.0",
                                "o_struct",
                                "1.0",
                                s_members
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::UnionMemberSeq u_members (2);
  u_members.length (2);

  const char *u_names[] = {"u_enum", "u_struct"};

  u_members[0].name = u_names[0];
  u_members[0].type_def = CORBA::EnumDef::_duplicate (e_var.in ());
  u_members[0].type = e_var->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  CORBA::Long label = 0;
  CORBA::Any any;
  any <<= label;
  u_members[0].label = any;

  u_members[1].name = u_names[1];
  u_members[1].type_def = CORBA::StructDef::_duplicate (s_var.in ());
  u_members[1].type = s_var->type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  label = 1;
  any <<= label;
  u_members[1].label = any;

  CORBA::PrimitiveDef_var d_var =
    this->repo_->get_primitive (CORBA::pk_long
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::UnionDef_var u_var =
    this->repo_->create_union ("IDL:the_union:1.0",
                               "the_union",
                               "1.0",
                               d_var.in (),
                               u_members
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::UnionMemberSeq_var out_u_members = u_var->members (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ULong length = out_u_members->length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("UnionDef::members::length: %d\n"),
                length));

  ACE_ASSERT (length == 2);

  CORBA::ULong i = 0;
  const char *tmp = 0;

  for (i = 0; i < length; i++)
    {
      tmp = out_u_members[i].name;

      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("UnionDef::members[%d]::name: %s\n"),
                    i,
                    tmp));

      ACE_ASSERT (!ACE_OS::strcmp (tmp, u_members[i].name));
    }

  CORBA::ContainedSeq_var out_contents = u_var->contents (CORBA::dk_all,
                                                          1
                                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  length = out_contents->length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nUnionDef::contents::length: %d\n"),
                length));

  ACE_ASSERT (length == 0);

  e_var->move (u_var.in (),
               u_names[0],
               "1.0"
               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nEnumDef::move (into union)\n")));

  out_u_members = u_var->members (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  length = out_u_members->length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nUnionDef::members::length: %d\n"),
                length));

  ACE_ASSERT (length == 2);

  for (i = 0; i < length; i++)
    {
      tmp = out_u_members[i].name;

      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("UnionDef::members[%d]::name: %s\n"),
                    i,
                    tmp));

      if (i == 0)
        {
          if (ACE_OS::strcmp (tmp, "u_enum"))
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("move_test::name -")
                          ACE_TEXT ("incorrect name in item %d"),
                          i));

              return;
            }
        }
      else
        {
          if (ACE_OS::strcmp (tmp, u_members[i].name))
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("move_test::name -")
                          ACE_TEXT ("incorrect name in item %d"),
                          i));

              return;
            }
        }
    }

  out_contents = u_var->contents (CORBA::dk_all,
                                  1
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  length = out_contents->length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nUnionDef::contents::length: %d\n"),
                length));

  ACE_ASSERT (length == 1);

  CORBA::String_var str;

  for (i = 0; i < length; i++)
    {
      str = out_contents[i]->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("UnionDef::contents[%d]::absolute_name: %s\n"),
                    i,
                    str.in ()));

      if (i == 0)
        {
          if (ACE_OS::strcmp (str.in (), "::the_union::u_enum"))
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("move_test::contents -")
                          ACE_TEXT ("incorrect scoped name in item %d"),
                          i));

              return;
            }
        }
    }

  e_var2->move (s_var.in (),
                s_names[1],
                "1.0"
                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  s_var->move (u_var.in (),
               u_names[1],
               "1.0"
                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nEnumDef::move (into struct)\n")
                ACE_TEXT ("StructDef::move (into union)\n")));

  out_contents = this->repo_->contents (CORBA::dk_all,
                                        1
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  length = out_contents->length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nRepository::contents::length: %d\n"),
                length));

  ACE_ASSERT (length == 1);

  out_u_members = u_var->members (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  length = out_u_members->length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nUnionDef::members::length: %d\n"),
                length));

  ACE_ASSERT (length == 2);

  for (i = 0; i < length; i++)
    {
      tmp = out_u_members[i].name;

      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("UnionDef::members[%d]::name: %s\n"),
                    i,
                    tmp));

      ACE_ASSERT (!ACE_OS::strcmp (tmp, u_names[i]));
    }

  out_contents = u_var->contents (CORBA::dk_all,
                                  1
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  length = out_contents->length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nUnionDef::contents::length: %d\n"),
                length));

  ACE_ASSERT (length == 2);

  ACE_CString scoped ("::the_union::");
  CORBA::StructDef_var s_tmp;

  for (i = 0; i < length; i++)
    {
      str = out_contents[i]->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("UnionDef::contents[%d]::absolute_name: %s\n"),
                    i,
                    str.in ()));

      ACE_ASSERT (!ACE_OS::strcmp (str.in (),
                                   (scoped + u_names[i]).c_str ()));

      if (i == 1)
        {
          s_tmp = CORBA::StructDef::_narrow (out_contents[i]
                                             ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }

  out_contents = s_tmp->contents (CORBA::dk_all,
                                  0
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  length = out_contents->length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nStructDef::contents::length: %d\n"),
                length));

  ACE_ASSERT (length == 1);

  const char *base = "::the_union::u_struct";

  for (i = 0; i < length; i++)
    {
      str = out_contents[i]->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("StructDef::contents[%d]::absolute_name: %s\n"),
                    i,
                    str.in ()));

      ACE_ASSERT (!ACE_OS::strncmp (str.in (),
                                    base,
                                    ACE_OS::strlen (base)));
    }

#if defined (ACE_NDEBUG)
  // ACE_ASSERT macro expands to nothing, so...
  ACE_UNUSED_ARG (base);
#endif /* ACE_NDEBUG */

  CORBA::StructMemberSeq_var out_s_members = s_tmp->members (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  length = out_s_members->length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nStructDef::members::length: %d\n"),
                length));

  ACE_ASSERT (length == 2);

  for (i = 0; i < length; i++)
    {
      tmp = out_s_members[i].name;

      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("StructDef::members[%d]::name: %s\n"),
                    i,
                    tmp));

      ACE_ASSERT (!ACE_OS::strcmp (tmp, s_names[i]));
   }

  u_var->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
Admin_Client::module_test (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("\n============== MODULE TEST ==============\n\n")
    ));

  CORBA::ModuleDef_var outer = this->repo_->create_module ("IDL:outer:1.0",
                                                           "outer",
                                                           "1.0"
                                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ModuleDef_var middle = outer->create_module ("IDL:middle:1.0",
                                                      "middle",
                                                      "1.0"
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ModuleDef_var inner = middle->create_module ("IDL:inner:1.0",
                                                      "inner",
                                                      "1.0"
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::InterfaceDefSeq in_bases (1);
  in_bases.length (0);

  CORBA::InterfaceDef_var p_iface =
    outer->create_interface ("IDL:p_iface:1.0",
                             "p_iface",
                             "1.0",
                             in_bases
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  in_bases.length (1);
  in_bases[0] = CORBA::InterfaceDef::_duplicate (p_iface.in ());

  CORBA::InterfaceDef_var iface =
    inner->create_interface ("IDL:iface:1.0",
                             "iface",
                             "1.0",
                             in_bases
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::PrimitiveDef_var p_void =
    this->repo_->get_primitive (CORBA::pk_void
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  CORBA::PrimitiveDef_var p_long =
    this->repo_->get_primitive (CORBA::pk_long
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::AttributeDef_var attr =
    p_iface->create_attribute ("IDL:iface/attr:1.0",
                               "attr",
                               "1.0",
                               p_void.in (),
                               CORBA::ATTR_NORMAL
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ULong length = 3;
  CORBA::ULong i = 0;

  CORBA::ParDescriptionSeq par_seq (length);
  par_seq.length (length);

  CORBA::ContextIdSeq con_seq (length);
  con_seq.length (length);

  const char *p_names[] = {"inarg", "outarg", "inoutarg"};
  const char *contexts[] = {"straw", "sticks", "bricks"};

  for (i = 0; i < length; ++i)
    {
      par_seq[i].name = p_names[i];
      par_seq[i].type_def = CORBA::PrimitiveDef::_duplicate (p_long.in ());
      par_seq[i].type = p_long->type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
      par_seq[i].mode = ACE_static_cast (CORBA::ParameterMode, i);

      con_seq[i] = contexts[i];
    }

  CORBA::StructMemberSeq members (0);
  members.length (0);

  CORBA::ExceptionDef_var ex_var =
    this->repo_->create_exception ("IDL:if_exception:1.0",
                                   "if_exception",
                                   "1.0",
                                   members
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ExceptionDefSeq get_seq (1);
  get_seq.length (1);
  get_seq[0] = CORBA::ExceptionDef::_duplicate (ex_var.in ());

  CORBA::OperationDef_var op =
    iface->create_operation ("IDL:iface/op:1.0",
                             "op",
                             "1.0",
                             p_void.in (),
                             CORBA::OP_NORMAL,
                             par_seq,
                             get_seq,
                             con_seq
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::String_var str;

  CORBA::Contained_var result =
    inner->lookup ("::outer::middle::inner::iface::op"
                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  str = result->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((
        LM_DEBUG,
        ACE_TEXT ("ModuleDef::lookup (absolute)::absolute_name: %s\n"),
        str.in ()
      ));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (),
                               "::outer::middle::inner::iface::op"));

  result = middle->lookup ("inner::iface::op"
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  str = result->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((
        LM_DEBUG,
        ACE_TEXT ("ModuleDef::lookup (relative)::absolute_name: %s\n"),
        str.in ()
      ));

  ACE_ASSERT (!ACE_OS::strcmp (str.in (),
                               "::outer::middle::inner::iface::op"));

  CORBA::ContainedSeq_var cseq = this->repo_->lookup_name ("op",
                                                           -1,
                                                           CORBA::dk_all,
                                                           0
                                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  length = cseq->length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Repository::lookup_name::length: %d\n"),
                length));

  ACE_ASSERT (length == 1);

  for (i = 0; i < length; i++)
    {
      str = cseq[i]->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (this->debug_)
        ACE_DEBUG ((
            LM_DEBUG,
            ACE_TEXT ("Repository::lookup_name[%d]::absolute_name: %s\n"),
            i,
            str.in ()
          ));

      ACE_ASSERT (!ACE_OS::strcmp (str.in (),
                                   "::outer::middle::inner::iface::op"));
    }

  cseq = middle->lookup_name ("attr",
                              3,
                              CORBA::dk_Attribute,
                              0
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  length = cseq->length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Repository::lookup_name::length: %d\n"),
                length));

  ACE_ASSERT (length == 1);

  for (i = 0; i < length; i++)
    {
      str = cseq[i]->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (this->debug_)
        ACE_DEBUG ((
            LM_DEBUG,
            ACE_TEXT ("Repository::lookup_name[%d]::absolute_name: %s\n"),
            i,
            str.in ()
          ));

      ACE_ASSERT (!ACE_OS::strcmp (str.in (), "::outer::p_iface::attr"));
    }

  iface->move (outer.in (),
               "iface",
               "1.0"
               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nInterfaceDef::move (into outer module)\n")));

  cseq = this->repo_->lookup_name ("op",
                                   -1,
                                   CORBA::dk_all,
                                   0
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  length = cseq->length ();

  if (this->debug_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Repository::lookup_name::length: %d\n"),
                length));

  ACE_ASSERT (length == 1);

  for (i = 0; i < length; i++)
    {
      str = cseq[i]->absolute_name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (this->debug_)
        ACE_DEBUG ((
            LM_DEBUG,
            ACE_TEXT ("Repository::lookup_name[%d]::absolute_name: %s\n"),
            i,
            str.in ()
          ));

      ACE_ASSERT (!ACE_OS::strcmp (str.in (), "::outer::iface::op"));
    }

  outer->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}
