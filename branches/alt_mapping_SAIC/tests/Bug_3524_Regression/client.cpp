// -*- C++ -*-
// $Id$

#include "ace/Get_Opt.h"
#include "ace/Synch.h"
#include "testC.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-v ")
                           ACE_TEXT ("\n"),
                           argv [0]),
                          -1);
      }
  return 0;
}

int
call_method (::Test::A_ptr server,
             ::Test::A::FailOn where,
             const char * arg1,
             ::CORBA::String_out & arg2,
             char *& arg3,
             const char * error)
{
  try
    {
      CORBA::String_var res = server->method (where,
                                              arg1,
                                              arg2,
                                              arg3);

      ACE_ERROR ((LM_ERROR,
                  "ERROR: No %C",
                  error));
      return 1;
    }
  catch (const CORBA::BAD_PARAM &)
    {
      // it's ok
    }
  catch (const CORBA::Exception &)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Wrong %C",
                  error));
      return 1;
    }

  return 0;
}

int
call_method_s (::Test::A_ptr server,
               ::Test::A::FailOn where,
               const char * arg1,
               ::CORBA::String_out & arg2,
               char *& arg3,
               const char * error)
{
  try
    {
      CORBA::String_var res = server->method_s (where,
                                                arg1,
                                                arg2,
                                                arg3);

      ACE_ERROR ((LM_ERROR,
                  "ERROR: No %C",
                  error));
      return 1;
    }
  catch (const CORBA::BAD_PARAM &)
    {
      // it's ok
    }
  catch (const CORBA::Exception &)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Wrong %C",
                  error));
      return 1;
    }

  return 0;
}

int
call_seq_method (::Test::A_ptr server,
                 ::Test::A::FailOn where,
                 const ::Test::seq_bd_str & arg1,
                 ::Test::seq_bd_str_out & arg2,
                 ::Test::seq_bd_str & arg3,
                 const char * error)
{
  try
    {
      ::Test::seq_bd_str_var res =
        server->seq_method (where,
                            arg1,
                            arg2,
                            arg3);

      ACE_ERROR ((LM_ERROR,
                  "ERROR: No %C",
                  error));
      return 1;
    }
  catch (const CORBA::BAD_PARAM &)
    {
      // it's ok
    }
  catch (const CORBA::Exception &)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Wrong %C",
                  error));
      return 1;
    }

  return 0;
}

int
call_seq_method_s (::Test::A_ptr server,
                   ::Test::A::FailOn where,
                   const ::Test::seq_bds_str & arg1,
                   ::Test::seq_bds_str_out & arg2,
                   ::Test::seq_bds_str & arg3,
                   const char * error)
{
  try
    {
      ::Test::seq_bds_str_var res =
        server->seq_method_s (where,
                              arg1,
                              arg2,
                              arg3);

      ACE_ERROR ((LM_ERROR,
                  "ERROR: No %C",
                  error));
      return 1;
    }
  catch (const CORBA::BAD_PARAM &)
    {
      // it's ok
    }
  catch (const CORBA::Exception &)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Wrong %C",
                  error));
      return 1;
    }

  return 0;
}

int
call_arr_method (::Test::A_ptr server,
                 ::Test::A::FailOn where,
                 const ::Test::arr_bd_str & arg1,
                 ::Test::arr_bd_str_out & arg2,
                 ::Test::arr_bd_str & arg3,
                 const char * error)
{
  try
    {
      ::Test::arr_bd_str_var res =
        server->arr_method (where,
                            arg1,
                            arg2,
                            arg3);

      ACE_ERROR ((LM_ERROR,
                  "ERROR: No %C",
                  error));
      return 1;
    }
  catch (const CORBA::BAD_PARAM &)
    {
      // it's ok
    }
  catch (const CORBA::Exception &)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Wrong %C",
                  error));
      return 1;
    }

  return 0;
}

int
call_arr_method_s (::Test::A_ptr server,
                   ::Test::A::FailOn where,
                   const ::Test::arr_bds_str & arg1,
                   ::Test::arr_bds_str_out & arg2,
                   ::Test::arr_bds_str & arg3,
                   const char * error)
{
  try
    {
      ::Test::arr_bds_str_var res =
        server->arr_method_s (where,
                              arg1,
                              arg2,
                              arg3);

      ACE_ERROR ((LM_ERROR,
                  "ERROR: No %C",
                  error));
      return 1;
    }
  catch (const CORBA::BAD_PARAM &)
    {
      // it's ok
    }
  catch (const CORBA::Exception &)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Wrong %C",
                  error));
      return 1;
    }

  return 0;
}

int
call_sct_method (::Test::A_ptr server,
                 ::Test::A::FailOn where,
                 const ::Test::sct & arg1,
                 ::Test::sct_out & arg2,
                 ::Test::sct & arg3,
                 const char * error)
{
  try
    {
      ::Test::sct_var res =
        server->sct_method (where,
                            arg1,
                            arg2,
                            arg3);

      ACE_ERROR ((LM_ERROR,
                  "ERROR: No %C",
                  error));
      return 1;
    }
  catch (const CORBA::BAD_PARAM &)
    {
      // it's ok
    }
  catch (const CORBA::Exception &)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Wrong %C",
                  error));
      return 1;
    }

  return 0;
}

int
call_unn_method (::Test::A_ptr server,
                 ::Test::A::FailOn where,
                 const ::Test::unn & arg1,
                 ::Test::unn_out & arg2,
                 ::Test::unn & arg3,
                 const char * error)
{
  try
    {
      ::Test::unn_var res =
        server->unn_method (where,
                            arg1,
                            arg2,
                            arg3);

      ACE_ERROR ((LM_ERROR,
                  "ERROR: No %C",
                  error));
      return 1;
    }
  catch (const CORBA::BAD_PARAM &)
    {
      // it's ok
    }
  catch (const CORBA::Exception &)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Wrong %C",
                  error));
      return 1;
    }

  return 0;
}

int
call_vbx_method (::Test::A_ptr server,
                 ::Test::A::FailOn where,
                 ::Test::val_bd_str * arg1,
                 ::Test::val_bd_str_out arg2,
                 ::Test::val_bd_str *& arg3,
                 const char * error)
{
  try
    {
      ::Test::val_bd_str_var res =
        server->vbx_method (where,
                            arg1,
                            arg2,
                            arg3);

      ACE_ERROR ((LM_ERROR,
                  "ERROR: No %C",
                  error));
      return 1;
    }
  catch (const CORBA::BAD_PARAM &)
    {
      // it's ok
    }
  catch (const CORBA::Exception &)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Wrong %C",
                  error));
      return 1;
    }

  return 0;
}

int
call_vbx_method_s (::Test::A_ptr server,
                   ::Test::A::FailOn where,
                   ::Test::val_bds_str * arg1,
                   ::Test::val_bds_str_out arg2,
                   ::Test::val_bds_str *& arg3,
                   const char * error)
{
  try
    {
      ::Test::val_bds_str_var res =
        server->vbx_method_s (where,
                              arg1,
                              arg2,
                              arg3);

      ACE_ERROR ((LM_ERROR,
                  "ERROR: No %C",
                  error));
      return 1;
    }
  catch (const CORBA::BAD_PARAM &)
    {
      // it's ok
    }
  catch (const CORBA::Exception &)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Wrong %C",
                  error));
      return 1;
    }

  return 0;
}

int
call_vtp_method (::Test::A_ptr server,
                 ::Test::A::FailOn where,
                 ::Test::vtp * arg1,
                 ::Test::vtp_out arg2,
                 ::Test::vtp *& arg3,
                 const char * error)
{
  try
    {
      ::Test::vtp_var res =
        server->vtp_method (where,
                            arg1,
                            arg2,
                            arg3);

      ACE_ERROR ((LM_ERROR,
                  "ERROR: No %C",
                  error));
      return 1;
    }
  catch (const CORBA::BAD_PARAM &)
    {
      // it's ok
    }
  catch (const CORBA::Exception &)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Wrong %C",
                  error));
      return 1;
    }

  return 0;
}

int test_strings (::Test::A_ptr server)
{
  int status = 0;

  {
    CORBA::String_var arg2, arg3;
    CORBA::String_out arg2_out = arg2.out ();
    status |=
      call_method (server,
                   ::Test::A::ARG1,
                   ::Test::FailStr,
                   arg2_out,
                   arg3,
                   "exception is thrown in a call "
                   "for `in bounded_string`.\n");
    status |=
      call_method_s (server,
                     ::Test::A::ARG1,
                     ::Test::FailStr,
                     arg2_out,
                     arg3,
                     "exception is thrown in a call "
                     "for `in string<BOUND>`.\n");
  }

  {
    CORBA::String_var arg2, arg3;
    CORBA::String_out arg2_out = arg2.out ();
    status |=
      call_method (server,
                   ::Test::A::ARG2,
                   0,
                   arg2_out,
                   arg3,
                   "exception is thrown in a call "
                   "for `out bounded_string`.\n");
    status |=
      call_method_s (server,
                     ::Test::A::ARG2,
                     0,
                     arg2_out,
                     arg3,
                     "exception is thrown in a call "
                     "for `out string<BOUND>`.\n");
  }

  {
    CORBA::String_var arg2, arg3;
    CORBA::String_out arg2_out = arg2.out ();
    status |=
      call_method (server,
                   ::Test::A::ARG3,
                   0,
                   arg2_out,
                   arg3,
                   "exception is thrown in a call "
                   "for `inout bounded_string` (server side).\n");
    status |=
      call_method_s (server,
                     ::Test::A::ARG3,
                     0,
                     arg2_out,
                     arg3,
                     "exception is thrown in a call "
                     "for `inout string<BOUND>` (server side).\n");
  }

  {
    CORBA::String_var arg2;
    CORBA::String_var arg3 = CORBA::string_dup (::Test::FailStr);
    CORBA::String_out arg2_out = arg2.out ();
    status |=
      call_method (server,
                   ::Test::A::ARG3,
                   0,
                   arg2_out,
                   arg3,
                   "exception is thrown in a call "
                   "for `inout bounded_string`.\n");
    status |=
      call_method_s (server,
                     ::Test::A::ARG3,
                     0,
                     arg2_out,
                     arg3,
                     "exception is thrown in a call "
                     "for `inout string<BOUND>`.\n");
  }

  {
    CORBA::String_var arg2, arg3;
    CORBA::String_out arg2_out = arg2.out ();
    status |=
      call_method (server,
                   ::Test::A::RETN,
                   0,
                   arg2_out,
                   arg3,
                   "exception is thrown in a call "
                   "when server returns `bounded_string`.\n");
    status |=
      call_method_s (server,
                     ::Test::A::RETN,
                     0,
                     arg2_out,
                     arg3,
                     "exception is thrown in a call "
                     "when server returns `string<BOUND>`.\n");
  }

  return status;
}

int test_sequences (::Test::A_ptr server)
{
  int status = 0;

  {
    ::Test::seq_bd_str arg1;
    arg1.length (1);
    arg1[0] = CORBA::string_dup (::Test::FailStr);
    ::Test::seq_bd_str_var arg2;
    ::Test::seq_bd_str_out arg2_out = arg2.out ();
    ::Test::seq_bd_str arg3;
    status |=
      call_seq_method (server,
                       ::Test::A::ARG1,
                       arg1,
                       arg2_out,
                       arg3,
                       "exception is thrown in a call "
                       "for `in sequence<bounded_string>`.\n");
  }

  {
    ::Test::seq_bds_str arg1;
    arg1.length (1);
    arg1[0] = CORBA::string_dup (::Test::FailStr);
    ::Test::seq_bds_str_var arg2;
    ::Test::seq_bds_str_out arg2_out = arg2.out ();
    ::Test::seq_bds_str arg3;
    arg3.length (1);
    status |=
      call_seq_method_s (server,
                         ::Test::A::ARG1,
                         arg1,
                         arg2_out,
                         arg3,
                         "exception is thrown in a call "
                         "for `in sequence<string<BOUND>>`.\n");
  }

  {
    ::Test::seq_bd_str arg1;
    arg1.length (1);
    ::Test::seq_bd_str_var arg2;
    ::Test::seq_bd_str_out arg2_out = arg2.out ();
    ::Test::seq_bd_str arg3;
    arg3.length (1);
    status |=
      call_seq_method (server,
                       ::Test::A::ARG2,
                       arg1,
                       arg2_out,
                       arg3,
                       "exception is thrown in a call "
                       "for `out sequence<bounded_string>`.\n");
  }

  {
    ::Test::seq_bds_str arg1;
    arg1.length (1);
    ::Test::seq_bds_str_var arg2;
    ::Test::seq_bds_str_out arg2_out = arg2.out ();
    ::Test::seq_bds_str arg3;
    arg3.length (1);
    status |=
      call_seq_method_s (server,
                         ::Test::A::ARG2,
                         arg1,
                         arg2_out,
                         arg3,
                         "exception is thrown in a call "
                         "for `out sequence<string<BOUND>>`.\n");
  }

  {
    ::Test::seq_bd_str arg1;
    arg1.length (1);
    ::Test::seq_bd_str_var arg2;
    ::Test::seq_bd_str_out arg2_out = arg2.out ();
    ::Test::seq_bd_str arg3;
    arg3.length (1);
    status |=
      call_seq_method (server,
                       ::Test::A::ARG3,
                       arg1,
                       arg2_out,
                       arg3,
                       "exception is thrown in a call "
                       "for `inout sequence<bounded_string>` (server side).\n");
  }

  {
    ::Test::seq_bds_str arg1;
    arg1.length (1);
    ::Test::seq_bds_str_var arg2;
    ::Test::seq_bds_str_out arg2_out = arg2.out ();
    ::Test::seq_bds_str arg3;
    arg3.length (1);
    status |=
      call_seq_method_s (server,
                         ::Test::A::ARG3,
                         arg1,
                         arg2_out,
                         arg3,
                         "exception is thrown in a call "
                         "for `inout sequence<string<BOUND>>` (server side).\n");
  }

  {
    ::Test::seq_bd_str arg1;
    arg1.length (1);
    ::Test::seq_bd_str_var arg2;
    ::Test::seq_bd_str_out arg2_out = arg2.out ();
    ::Test::seq_bd_str arg3;
    arg3.length (1);
    arg3[0] = CORBA::string_dup (::Test::FailStr);
    status |=
      call_seq_method (server,
                       ::Test::A::ARG3,
                       arg1,
                       arg2_out,
                       arg3,
                       "exception is thrown in a call "
                       "for `inout sequence<bounded_string>`.\n");
  }

  {
    ::Test::seq_bds_str arg1;
    arg1.length (1);
    ::Test::seq_bds_str_var arg2;
    ::Test::seq_bds_str_out arg2_out = arg2.out ();
    ::Test::seq_bds_str arg3;
    arg3.length (1);
    arg3[0] = CORBA::string_dup (::Test::FailStr);
    status |=
      call_seq_method_s (server,
                         ::Test::A::ARG3,
                         arg1,
                         arg2_out,
                         arg3,
                         "exception is thrown in a call "
                         "for `inout sequence<string<BOUND>>`.\n");
  }

  {
    ::Test::seq_bd_str arg1;
    arg1.length (1);
    ::Test::seq_bd_str_var arg2;
    ::Test::seq_bd_str_out arg2_out = arg2.out ();
    ::Test::seq_bd_str arg3;
    arg3.length (1);
    status |=
      call_seq_method (server,
                       ::Test::A::RETN,
                       arg1,
                       arg2_out,
                       arg3,
                       "exception is thrown in a call "
                       "when server returns `sequence<bounded_string>`.\n");
  }

  {
    ::Test::seq_bds_str arg1;
    arg1.length (1);
    ::Test::seq_bds_str_var arg2;
    ::Test::seq_bds_str_out arg2_out = arg2.out ();
    ::Test::seq_bds_str arg3;
    arg3.length (1);
    status |=
      call_seq_method_s (server,
                         ::Test::A::RETN,
                         arg1,
                         arg2_out,
                         arg3,
                         "exception is thrown in a call "
                         "when server returns `sequence<string<BOUND>>`.\n");
  }

  return status;
}

int test_arrays (::Test::A_ptr server)
{
  int status = 0;

  {
    ::Test::arr_bd_str arg1;
    arg1[0] = CORBA::string_dup (::Test::FailStr);
    ::Test::arr_bd_str_var arg2;
    ::Test::arr_bd_str_out arg2_out = arg2.out ();
    ::Test::arr_bd_str arg3;
    status |=
      call_arr_method (server,
                       ::Test::A::ARG1,
                       arg1,
                       arg2_out,
                       arg3,
                       "exception is thrown in a call "
                       "for `in bounded_string array[]`.\n");
  }

  {
    ::Test::arr_bds_str arg1;
    arg1[0] = CORBA::string_dup (::Test::FailStr);
    ::Test::arr_bds_str_var arg2;
    ::Test::arr_bds_str_out arg2_out = arg2.out ();
    ::Test::arr_bds_str arg3;
    status |=
      call_arr_method_s (server,
                         ::Test::A::ARG1,
                         arg1,
                         arg2_out,
                         arg3,
                         "exception is thrown in a call "
                         "for `in string<BOUND> array[]`.\n");
  }

  {
    ::Test::arr_bd_str arg1;
    ::Test::arr_bd_str_var arg2;
    ::Test::arr_bd_str_out arg2_out = arg2.out ();
    ::Test::arr_bd_str arg3;
    status |=
      call_arr_method (server,
                       ::Test::A::ARG2,
                       arg1,
                       arg2_out,
                       arg3,
                       "exception is thrown in a call "
                       "for `out bounded_string array[]`.\n");
  }

  {
    ::Test::arr_bds_str arg1;
    ::Test::arr_bds_str_var arg2;
    ::Test::arr_bds_str_out arg2_out = arg2.out ();
    ::Test::arr_bds_str arg3;
    status |=
      call_arr_method_s (server,
                         ::Test::A::ARG2,
                         arg1,
                         arg2_out,
                         arg3,
                         "exception is thrown in a call "
                         "for `out string<BOUND> array[]`.\n");
  }

  {
    ::Test::arr_bd_str arg1;
    ::Test::arr_bd_str_var arg2;
    ::Test::arr_bd_str_out arg2_out = arg2.out ();
    ::Test::arr_bd_str arg3;
    status |=
      call_arr_method (server,
                       ::Test::A::ARG3,
                       arg1,
                       arg2_out,
                       arg3,
                       "exception is thrown in a call "
                       "for `inout bounded_string array[]` (server side).\n");
  }

  {
    ::Test::arr_bds_str arg1;
    ::Test::arr_bds_str_var arg2;
    ::Test::arr_bds_str_out arg2_out = arg2.out ();
    ::Test::arr_bds_str arg3;
    status |=
      call_arr_method_s (server,
                         ::Test::A::ARG3,
                         arg1,
                         arg2_out,
                         arg3,
                         "exception is thrown in a call "
                         "for `inout string<BOUND> array[]` (server side).\n");
  }

  {
    ::Test::arr_bd_str arg1;
    ::Test::arr_bd_str_var arg2;
    ::Test::arr_bd_str_out arg2_out = arg2.out ();
    ::Test::arr_bd_str arg3;
    arg3[0] = CORBA::string_dup (::Test::FailStr);
    status |=
      call_arr_method (server,
                       ::Test::A::ARG3,
                       arg1,
                       arg2_out,
                       arg3,
                       "exception is thrown in a call "
                       "for `inout bounded_string array[]`.\n");
  }

  {
    ::Test::arr_bds_str arg1;
    ::Test::arr_bds_str_var arg2;
    ::Test::arr_bds_str_out arg2_out = arg2.out ();
    ::Test::arr_bds_str arg3;
    arg3[0] = CORBA::string_dup (::Test::FailStr);
    status |=
      call_arr_method_s (server,
                         ::Test::A::ARG3,
                         arg1,
                         arg2_out,
                         arg3,
                         "exception is thrown in a call "
                         "for `inout string<BOUND> array[]`.\n");
  }

  {
    ::Test::arr_bd_str arg1;
    ::Test::arr_bd_str_var arg2;
    ::Test::arr_bd_str_out arg2_out = arg2.out ();
    ::Test::arr_bd_str arg3;
    status |=
      call_arr_method (server,
                       ::Test::A::RETN,
                       arg1,
                       arg2_out,
                       arg3,
                       "exception is thrown in a call "
                       "when server returns `bounded_string array[]`.\n");
  }

  {
    ::Test::arr_bds_str arg1;
    ::Test::arr_bds_str_var arg2;
    ::Test::arr_bds_str_out arg2_out = arg2.out ();
    ::Test::arr_bds_str arg3;
    status |=
      call_arr_method_s (server,
                         ::Test::A::RETN,
                         arg1,
                         arg2_out,
                         arg3,
                         "exception is thrown in a call "
                         "when server returns `string<BOUND> array[]`.\n");
  }

  return status;
}

int test_structs (::Test::A_ptr server)
{
  int status = 0;

  {
    ::Test::sct arg1;
    arg1.b = CORBA::string_dup (::Test::FailStr);
    arg1.c = CORBA::string_dup (::Test::FailStr);
    ::Test::sct_var arg2;
    ::Test::sct_out arg2_out = arg2.out ();
    ::Test::sct arg3;
    status |=
      call_sct_method (server,
                       ::Test::A::ARG1,
                       arg1,
                       arg2_out,
                       arg3,
                       "exception is thrown in a call "
                       "for `in struct {}`.\n");
  }

  {
    ::Test::sct arg1;
    ::Test::sct_var arg2;
    ::Test::sct_out arg2_out = arg2.out ();
    ::Test::sct arg3;
    status |=
      call_sct_method (server,
                       ::Test::A::ARG2,
                       arg1,
                       arg2_out,
                       arg3,
                       "exception is thrown in a call "
                       "for `out struct {}`.\n");
  }

  {
    ::Test::sct arg1;
    ::Test::sct_var arg2;
    ::Test::sct_out arg2_out = arg2.out ();
    ::Test::sct arg3;
    status |=
      call_sct_method (server,
                       ::Test::A::ARG3,
                       arg1,
                       arg2_out,
                       arg3,
                       "exception is thrown in a call "
                       "for `inout struct {}` (server side).\n");
  }

  {
    ::Test::sct arg1;
    ::Test::sct_var arg2;
    ::Test::sct_out arg2_out = arg2.out ();
    ::Test::sct arg3;
    arg3.b = CORBA::string_dup (::Test::FailStr);
    arg3.c = CORBA::string_dup (::Test::FailStr);
    status |=
      call_sct_method (server,
                       ::Test::A::ARG3,
                       arg1,
                       arg2_out,
                       arg3,
                       "exception is thrown in a call "
                       "for `inout struct {}`.\n");
  }

  {
    ::Test::sct arg1;
    ::Test::sct_var arg2;
    ::Test::sct_out arg2_out = arg2.out ();
    ::Test::sct arg3;
    status |=
      call_sct_method (server,
                       ::Test::A::RETN,
                       arg1,
                       arg2_out,
                       arg3,
                       "exception is thrown in a call "
                       "when server returns `struct {}`.\n");
  }

  return status;
}

int test_unions (::Test::A_ptr server)
{
  int status = 0;

  {
    ::Test::unn arg1;
    arg1.b (CORBA::string_dup (::Test::FailStr));
    arg1.c (CORBA::string_dup (::Test::FailStr));
    ::Test::unn_var arg2;
    ::Test::unn_out arg2_out = arg2.out ();
    ::Test::unn arg3;
    status |=
      call_unn_method (server,
                       ::Test::A::ARG1,
                       arg1,
                       arg2_out,
                       arg3,
                       "exception is thrown in a call "
                       "for `in union {}`.\n");
  }

  {
    ::Test::unn arg1;
    ::Test::unn_var arg2;
    ::Test::unn_out arg2_out = arg2.out ();
    ::Test::unn arg3;
    status |=
      call_unn_method (server,
                       ::Test::A::ARG2,
                       arg1,
                       arg2_out,
                       arg3,
                       "exception is thrown in a call "
                       "for `out union {}`.\n");
  }

  {
    ::Test::unn arg1;
    ::Test::unn_var arg2;
    ::Test::unn_out arg2_out = arg2.out ();
    ::Test::unn arg3;
    status |=
      call_unn_method (server,
                       ::Test::A::ARG3,
                       arg1,
                       arg2_out,
                       arg3,
                       "exception is thrown in a call "
                       "for `inout union {}` (server side).\n");
  }

  {
    ::Test::unn arg1;
    ::Test::unn_var arg2;
    ::Test::unn_out arg2_out = arg2.out ();
    ::Test::unn arg3;
    arg3.b (CORBA::string_dup (::Test::FailStr));
    arg3.c (CORBA::string_dup (::Test::FailStr));
    status |=
      call_unn_method (server,
                       ::Test::A::ARG3,
                       arg1,
                       arg2_out,
                       arg3,
                       "exception is thrown in a call "
                       "for `inout union {}`.\n");
  }

  {
    ::Test::unn arg1;
    ::Test::unn_var arg2;
    ::Test::unn_out arg2_out = arg2.out ();
    ::Test::unn arg3;
    status |=
      call_unn_method (server,
                       ::Test::A::RETN,
                       arg1,
                       arg2_out,
                       arg3,
                       "exception is thrown in a call when "
                       "when server returns `union {}`.\n");
  }

  return status;
}

int test_valueboxes (::Test::A_ptr server)
{
  int status = 0;

  {
    ::Test::val_bd_str_var arg1;
    ACE_NEW_THROW_EX (arg1.inout (),
                      ::Test::val_bd_str (::Test::FailStr),
                      CORBA::NO_MEMORY ());
    ::Test::val_bd_str_var arg2;
    ::Test::val_bd_str_out arg2_out = arg2;
    ::Test::val_bd_str_var arg3;
    status |=
      call_vbx_method (server,
                       ::Test::A::ARG1,
                       arg1.in (),
                       arg2_out,
                       arg3.inout (),
                       "exception is thrown in a call "
                       "for `in valuebox {bounded_string}`.\n");
  }

  {
    ::Test::val_bds_str_var arg1;
    ACE_NEW_THROW_EX (arg1.inout (),
                      ::Test::val_bds_str (::Test::FailStr),
                      CORBA::NO_MEMORY ());
    ::Test::val_bds_str_var arg2;
    ::Test::val_bds_str_out arg2_out = arg2;
    ::Test::val_bds_str_var arg3;
    status |=
      call_vbx_method_s (server,
                         ::Test::A::ARG1,
                         arg1.in (),
                         arg2_out,
                         arg3.inout (),
                         "exception is thrown in a call "
                         "for `in valuebox {string<BOUND>}`.\n");
  }

  {
    ::Test::val_bd_str_var arg1;
    ::Test::val_bd_str_var arg2;
    ::Test::val_bd_str_out arg2_out = arg2;
    ::Test::val_bd_str_var arg3;
    status |=
      call_vbx_method (server,
                       ::Test::A::ARG2,
                       arg1.in (),
                       arg2_out,
                       arg3.inout (),
                       "exception is thrown in a call "
                       "for `out valuebox {bounded_string}`.\n");
  }

  {
    ::Test::val_bds_str_var arg1;
    ::Test::val_bds_str_var arg2;
    ::Test::val_bds_str_out arg2_out = arg2;
    ::Test::val_bds_str_var arg3;
    status |=
      call_vbx_method_s (server,
                         ::Test::A::ARG2,
                         arg1.in (),
                         arg2_out,
                         arg3.inout (),
                         "exception is thrown in a call "
                         "for `out valuebox {string<BOUND>}`.\n");
  }

  {
    ::Test::val_bd_str_var arg1;
    ::Test::val_bd_str_var arg2;
    ::Test::val_bd_str_out arg2_out = arg2;
    ::Test::val_bd_str_var arg3;
    status |=
      call_vbx_method (server,
                       ::Test::A::ARG3,
                       arg1.in (),
                       arg2_out,
                       arg3.inout (),
                       "exception is thrown in a call "
                       "for `inout valuebox {bounded_string}` (server side).\n");
  }

  {
    ::Test::val_bds_str_var arg1;
    ::Test::val_bds_str_var arg2;
    ::Test::val_bds_str_out arg2_out = arg2;
    ::Test::val_bds_str_var arg3;
    status |=
      call_vbx_method_s (server,
                         ::Test::A::ARG3,
                         arg1.in (),
                         arg2_out,
                         arg3.inout (),
                         "exception is thrown in a call "
                         "for `inout valuebox {string<BOUND>}` (server side).\n");
  }

  {
    ::Test::val_bd_str_var arg1;
    ::Test::val_bd_str_var arg2;
    ::Test::val_bd_str_out arg2_out = arg2;
    ::Test::val_bd_str_var arg3;
    ACE_NEW_THROW_EX (arg3.inout (),
                      ::Test::val_bd_str (::Test::FailStr),
                      CORBA::NO_MEMORY ());
    status |=
      call_vbx_method (server,
                       ::Test::A::ARG3,
                       arg1.in (),
                       arg2_out,
                       arg3.inout (),
                       "exception is thrown in a call "
                       "for `inout valuebox {bounded_string}`.\n");
  }

  {
    ::Test::val_bds_str_var arg1;
    ::Test::val_bds_str_var arg2;
    ::Test::val_bds_str_out arg2_out = arg2;
    ::Test::val_bds_str_var arg3;
    ACE_NEW_THROW_EX (arg3.inout (),
                      ::Test::val_bds_str (::Test::FailStr),
                      CORBA::NO_MEMORY ());
    status |=
      call_vbx_method_s (server,
                         ::Test::A::ARG3,
                         arg1.in (),
                         arg2_out,
                         arg3.inout (),
                         "exception is thrown in a call "
                         "for `inout valuebox {string<BOUND>}`.\n");
  }

  {
    ::Test::val_bd_str_var arg1;
    ::Test::val_bd_str_var arg2;
    ::Test::val_bd_str_out arg2_out = arg2;
    ::Test::val_bd_str_var arg3;
    status |=
      call_vbx_method (server,
                       ::Test::A::RETN,
                       arg1.in (),
                       arg2_out,
                       arg3.inout (),
                       "exception is thrown in a call "
                       "when server returns `valuebox {bounded_string}`.\n");
  }

  {
    ::Test::val_bds_str_var arg1;
    ::Test::val_bds_str_var arg2;
    ::Test::val_bds_str_out arg2_out = arg2;
    ::Test::val_bds_str_var arg3;
    status |=
      call_vbx_method_s (server,
                         ::Test::A::RETN,
                         arg1.in (),
                         arg2_out,
                         arg3.inout (),
                         "exception is thrown in a call "
                         "when server returns `valuebox {string<BOUND>}`.\n");
  }

  return status;
}

int test_valuetypes (::Test::A_ptr server, ::Test::vtp_init *vtp_factory)
{
  int status = 0;

  {
    ::Test::vtp_var arg1 =
      ::Test::vtp::_downcast (vtp_factory->create_for_unmarshal ());
    arg1->b (CORBA::string_dup (::Test::FailStr));
    arg1->c (CORBA::string_dup (::Test::FailStr));
    ::Test::vtp_var arg2;
    ::Test::vtp_out arg2_out = arg2;
    ::Test::vtp_var arg3 =
      ::Test::vtp::_downcast (vtp_factory->create_for_unmarshal ());
    status |=
      call_vtp_method (server,
                       ::Test::A::ARG1,
                       arg1.in (),
                       arg2_out,
                       arg3.inout (),
                       "exception is thrown in a call "
                       "for `in valuetype {}`.\n");
  }

  {
    ::Test::vtp_var arg1;
    ::Test::vtp_var arg2;
    ::Test::vtp_out arg2_out = arg2;
    ::Test::vtp_var arg3;
    status |=
      call_vtp_method (server,
                       ::Test::A::ARG2,
                       arg1.in (),
                       arg2_out,
                       arg3.inout (),
                       "exception is thrown in a call "
                       "for `out valuetype {}`.\n");
  }

  {
    ::Test::vtp_var arg1;
    ::Test::vtp_var arg2;
    ::Test::vtp_out arg2_out = arg2;
    ::Test::vtp_var arg3;
    status |=
      call_vtp_method (server,
                       ::Test::A::ARG3,
                       arg1.in (),
                       arg2_out,
                       arg3.inout (),
                       "exception is thrown in a call "
                       "for `inout valuetype {}` (server side).\n");
  }

  {
    ::Test::vtp_var arg1;
    ::Test::vtp_var arg2;
    ::Test::vtp_out arg2_out = arg2;
    ::Test::vtp_var arg3 =
      ::Test::vtp::_downcast (vtp_factory->create_for_unmarshal ());
    arg3->b (CORBA::string_dup (::Test::FailStr));
    arg3->c (CORBA::string_dup (::Test::FailStr));
    status |=
      call_vtp_method (server,
                       ::Test::A::ARG3,
                       arg1.in (),
                       arg2_out,
                       arg3.inout (),
                       "exception is thrown in a call "
                       "for `inout valuetype {}`.\n");
  }

  {
    ::Test::vtp_var arg1;
    ::Test::vtp_var arg2;
    ::Test::vtp_out arg2_out = arg2;
    ::Test::vtp_var arg3;
    status |=
      call_vtp_method (server,
                       ::Test::A::RETN,
                       arg1.in (),
                       arg2_out,
                       arg3.inout (),
                       "exception is thrown in a call "
                       "when server returns `valuetype {}`.\n");
  }

  return status;
}

int test_exceptions (::Test::A_ptr server)
{
  int status = 0;

  ::Test::string_field all[] = {
    ::Test::BOUNDED,
    ::Test::TYPEDEFED
  };

  for (unsigned int i = 0;
       i < sizeof (all) / sizeof (::Test::string_field);
       ++i)
    {
      try
        {
          server->exc_method (all[i]);
        }
      catch (const ::Test::exc &)
        {
          status |= 1;
          ACE_ERROR ((LM_ERROR,
                      "ERROR: Illegal exception is thrown.\n"));
        }
      catch (const CORBA::BAD_PARAM &)
        {
          // it's ok
        }
      catch (const CORBA::Exception &)
        {
          status |= 1;
          ACE_ERROR ((LM_ERROR,
                      "ERROR: Wrong exception is thrown.\n"));
        }
    }

  return status;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int status = 0;

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      ::Test::vtp_init *vtp_factory = 0;
      ACE_NEW_THROW_EX (vtp_factory,
                        ::Test::vtp_init,
                        CORBA::NO_MEMORY ());
      CORBA::ValueFactoryBase_var vtp_factory_owner = vtp_factory;

      orb->register_value_factory (vtp_factory->tao_repository_id (),
                                   vtp_factory);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior);

      Test::A_var server =
        Test::A::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference <%s> is nil\n"),
                             ior),
                            1);
        }

      status |= test_strings (server.in ());

      status |= test_sequences (server.in ());

      status |= test_arrays (server.in ());

      status |= test_structs (server.in ());

      status |= test_unions (server.in ());

      status |= test_valueboxes (server.in ());

      status |= test_valuetypes (server.in (), vtp_factory);

      status |= test_exceptions (server.in ());

      server->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception in client:");
      return 1;
    }

  return status;
}
