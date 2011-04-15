// -*- C++ -*-
// $Id$

#include "test_i.h"
#include "ace/Auto_Ptr.h"

A_i::A_i (CORBA::ORB_ptr orb,
          CORBA::ValueFactoryBase *vtp_factory)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , vtp_factory_ (vtp_factory)
{
}

char *
A_i::method (::Test::A::FailOn where,
             const char *,
             ::CORBA::String_out arg2,
             char *&arg3)
{
  CORBA::String_var res;
  switch (where)
    {
    case ::Test::A::ARG1:
      // must be never reached.
      break;
    case ::Test::A::ARG2:
      arg2 = CORBA::string_dup (::Test::FailStr);
      break;
    case ::Test::A::ARG3:
      CORBA::string_free (arg3);
      arg3 = CORBA::string_dup (::Test::FailStr);
      break;
    case ::Test::A::RETN:
      res = CORBA::string_dup (::Test::FailStr);
      break;
    }
  return res._retn ();
}

char *
A_i::method_s (::Test::A::FailOn where,
               const char *,
               ::CORBA::String_out arg2,
               char *&arg3)
{
  CORBA::String_var res;
  switch (where)
    {
    case ::Test::A::ARG1:
      // must be never reached.
      break;
    case ::Test::A::ARG2:
      arg2 = CORBA::string_dup (::Test::FailStr);
      break;
    case ::Test::A::ARG3:
      CORBA::string_free (arg3);
      arg3 = CORBA::string_dup (::Test::FailStr);
      break;
    case ::Test::A::RETN:
      res = CORBA::string_dup (::Test::FailStr);
      break;
    }
  return res._retn ();
}

::Test::seq_bd_str *
A_i::seq_method (::Test::A::FailOn where,
                 const ::Test::seq_bd_str &,
                 ::Test::seq_bd_str_out arg2,
                 ::Test::seq_bd_str & arg3)
{
  ::Test::seq_bd_str *arg2_val = 0;
  ACE_NEW_THROW_EX (arg2_val,
                    ::Test::seq_bd_str,
                    CORBA::NO_MEMORY ());
  arg2 = arg2_val;

  ::Test::seq_bd_str *res_val = 0;
  ACE_NEW_THROW_EX (res_val,
                    ::Test::seq_bd_str,
                    CORBA::NO_MEMORY ());
  ::Test::seq_bd_str_var res = res_val;

  switch (where)
    {
    case ::Test::A::ARG1:
      // must be never reached.
      break;
    case ::Test::A::ARG2:
      arg2_val->length (1);
      (*arg2_val)[0] = CORBA::string_dup (::Test::FailStr);
      break;
    case ::Test::A::ARG3:
      arg3.length (1);
      arg3[0] = CORBA::string_dup (::Test::FailStr);
      break;
    case ::Test::A::RETN:
      res->length (1);
      (*res_val)[0] = CORBA::string_dup (::Test::FailStr);
      break;
    }
  return res._retn ();
}

::Test::seq_bds_str *
A_i::seq_method_s (::Test::A::FailOn where,
                   const ::Test::seq_bds_str &,
                   ::Test::seq_bds_str_out arg2,
                   ::Test::seq_bds_str & arg3)
{
  ::Test::seq_bds_str *arg2_val = 0;
  ACE_NEW_THROW_EX (arg2_val,
                    ::Test::seq_bds_str,
                    CORBA::NO_MEMORY ());
  arg2 = arg2_val;

  ::Test::seq_bds_str *res_val = 0;
  ACE_NEW_THROW_EX (res_val,
                    ::Test::seq_bds_str,
                    CORBA::NO_MEMORY ());
  ::Test::seq_bds_str_var res = res_val;
  res->length (1);

  switch (where)
    {
    case ::Test::A::ARG1:
      // must be never reached.
      break;
    case ::Test::A::ARG2:
      arg2_val->length (1);
      (*arg2_val)[0] = CORBA::string_dup (::Test::FailStr);
      break;
    case ::Test::A::ARG3:
      arg3.length (1);
      arg3[0] = CORBA::string_dup (::Test::FailStr);
      break;
    case ::Test::A::RETN:
      (*res_val)[0] = CORBA::string_dup (::Test::FailStr);
      break;
    }
  return res._retn ();
}

::Test::arr_bd_str_slice *
A_i::arr_method (::Test::A::FailOn where,
                 const ::Test::arr_bd_str,
                 ::Test::arr_bd_str_out arg2,
                 ::Test::arr_bd_str arg3)
{
  arg2 = ::Test::arr_bd_str_alloc ();

  ::Test::arr_bd_str_slice *res =
    ::Test::arr_bd_str_alloc ();
  ACE_Auto_Ptr< ::Test::arr_bd_str_slice> safe (res);

  switch (where)
    {
    case ::Test::A::ARG1:
      // must be never reached.
      break;
    case ::Test::A::ARG2:
      {
        ::Test::arr_bd_str_slice *& arg2_slice = arg2;
        arg2_slice[0] = CORBA::string_dup (::Test::FailStr);
      }
      break;
    case ::Test::A::ARG3:
      arg3[0] = CORBA::string_dup (::Test::FailStr);
      break;
    case ::Test::A::RETN:
      res[0] = CORBA::string_dup (::Test::FailStr);
      break;
    }

  safe.release ();
  return res;
}

::Test::arr_bds_str_slice *
A_i::arr_method_s (::Test::A::FailOn where,
                   const ::Test::arr_bds_str,
                   ::Test::arr_bds_str_out arg2,
                   ::Test::arr_bds_str arg3)
{
  arg2 = ::Test::arr_bds_str_alloc ();

  ::Test::arr_bds_str_slice *res =
    ::Test::arr_bds_str_alloc ();
  ACE_Auto_Ptr< ::Test::arr_bds_str_slice > safe (res);

  switch (where)
    {
    case ::Test::A::ARG1:
      // must be never reached.
      break;
    case ::Test::A::ARG2:
      {
        ::Test::arr_bds_str_slice *& arg2_slice = arg2;
        arg2_slice[0] = CORBA::string_dup (::Test::FailStr);
      }
      break;
    case ::Test::A::ARG3:
      arg3[0] = CORBA::string_dup (::Test::FailStr);
      break;
    case ::Test::A::RETN:
      res[0] = CORBA::string_dup (::Test::FailStr);
      break;
    }

  safe.release ();
  return res;
}

::Test::sct *
A_i::sct_method (::Test::A::FailOn where,
                 const ::Test::sct &,
                 ::Test::sct_out arg2,
                 ::Test::sct & arg3)
{
  ::Test::sct *arg2_val = 0;
  ACE_NEW_THROW_EX (arg2_val,
                    ::Test::sct,
                    CORBA::NO_MEMORY ());
  arg2 = arg2_val;

  ::Test::sct *res_val = 0;
  ACE_NEW_THROW_EX (res_val,
                    ::Test::sct,
                    CORBA::NO_MEMORY ());
  ::Test::sct_var res = res_val;

  switch (where)
    {
    case ::Test::A::ARG1:
      // must be never reached.
      break;
    case ::Test::A::ARG2:
      arg2_val->b = CORBA::string_dup (::Test::FailStr);
      arg2_val->c = CORBA::string_dup (::Test::FailStr);
      break;
    case ::Test::A::ARG3:
      arg3.b = CORBA::string_dup (::Test::FailStr);
      arg3.c = CORBA::string_dup (::Test::FailStr);
      break;
    case ::Test::A::RETN:
      res->b = CORBA::string_dup (::Test::FailStr);
      res->c = CORBA::string_dup (::Test::FailStr);
      break;
    }
  return res._retn ();
}

::Test::unn *
A_i::unn_method (::Test::A::FailOn where,
                 const ::Test::unn &,
                 ::Test::unn_out arg2,
                 ::Test::unn & arg3)
{
  ::Test::unn *arg2_val = 0;
  ACE_NEW_THROW_EX (arg2_val,
                    ::Test::unn,
                    CORBA::NO_MEMORY ());
  arg2 = arg2_val;

  ::Test::unn *res_val = 0;
  ACE_NEW_THROW_EX (res_val,
                    ::Test::unn,
                    CORBA::NO_MEMORY ());
  ::Test::unn_var res = res_val;

  switch (where)
    {
    case ::Test::A::ARG1:
      // must be never reached.
      break;
    case ::Test::A::ARG2:
      arg2->b (CORBA::string_dup (::Test::FailStr));
      arg2->c (CORBA::string_dup (::Test::FailStr));
      break;
    case ::Test::A::ARG3:
      arg3.b (CORBA::string_dup (::Test::FailStr));
      arg3.c (CORBA::string_dup (::Test::FailStr));
      break;
    case ::Test::A::RETN:
      res->b (CORBA::string_dup (::Test::FailStr));
      res->c (CORBA::string_dup (::Test::FailStr));
      break;
    }
  return res._retn ();
}

::Test::val_bd_str *
A_i::vbx_method (::Test::A::FailOn where,
                 ::Test::val_bd_str *,
                 ::Test::val_bd_str_out arg2,
                 ::Test::val_bd_str *& arg3)
{
  ::Test::val_bd_str_var res;
  switch (where)
    {
    case ::Test::A::ARG1:
      // must be never reached.
      break;
    case ::Test::A::ARG2:
      {
        ::Test::val_bd_str *arg2_val = 0;
        ACE_NEW_THROW_EX (arg2_val,
                          ::Test::val_bd_str,
                          CORBA::NO_MEMORY ());
        arg2 = arg2_val;
        *arg2 = CORBA::string_dup (::Test::FailStr);
      }
      break;
    case ::Test::A::ARG3:
      {
        ::Test::val_bd_str_var arg3_cleaner = arg3;
        ::Test::val_bd_str *arg3_val = 0;
        ACE_NEW_THROW_EX (arg3_val,
                          ::Test::val_bd_str,
                          CORBA::NO_MEMORY ());
        arg3 = arg3_val;
        *arg3 = CORBA::string_dup (::Test::FailStr);
      }
      break;
    case ::Test::A::RETN:
      {
        ::Test::val_bd_str *res_val = 0;
        ACE_NEW_THROW_EX (res_val,
                          ::Test::val_bd_str,
                          CORBA::NO_MEMORY ());
        res = res_val;
        *res_val = CORBA::string_dup (::Test::FailStr);
      }
      break;
    }
  return res._retn ();
}

::Test::val_bds_str *
A_i::vbx_method_s (::Test::A::FailOn where,
                   ::Test::val_bds_str *,
                   ::Test::val_bds_str_out arg2,
                   ::Test::val_bds_str *& arg3)
{
  ::Test::val_bds_str_var res;
  switch (where)
    {
    case ::Test::A::ARG1:
      // must be never reached.
      break;
    case ::Test::A::ARG2:
      {
        ::Test::val_bds_str *arg2_val = 0;
        ACE_NEW_THROW_EX (arg2_val,
                          ::Test::val_bds_str,
                          CORBA::NO_MEMORY ());
        arg2 = arg2_val;
        *arg2 = CORBA::string_dup (::Test::FailStr);
      }
      break;
    case ::Test::A::ARG3:
      {
        ::Test::val_bds_str_var arg3_cleaner = arg3;
        ::Test::val_bds_str *arg3_val = 0;
        ACE_NEW_THROW_EX (arg3_val,
                          ::Test::val_bds_str,
                          CORBA::NO_MEMORY ());
        arg3 = arg3_val;
        *arg3 = CORBA::string_dup (::Test::FailStr);
      }
      break;
    case ::Test::A::RETN:
      {
        ::Test::val_bds_str *res_val = 0;
        ACE_NEW_THROW_EX (res_val,
                          ::Test::val_bds_str,
                          CORBA::NO_MEMORY ());
        res = res_val;
        *res_val = CORBA::string_dup (::Test::FailStr);
      }
      break;
    }
  return res._retn ();
}

::Test::vtp *
A_i::vtp_method (::Test::A::FailOn where,
                 ::Test::vtp *,
                 ::Test::vtp_out arg2,
                 ::Test::vtp *& arg3)
{
  ::Test::vtp *res = 0;
  switch (where)
    {
    case ::Test::A::ARG1:
      // must be never reached.
      break;
    case ::Test::A::ARG2:
      {
        CORBA::ValueBase_var arg2_base =
          this->vtp_factory_->create_for_unmarshal ();
        arg2 = ::Test::vtp::_downcast (arg2_base.in ());
        arg2->b (CORBA::string_dup (::Test::FailStr));
        arg2->c (CORBA::string_dup (::Test::FailStr));
        arg2_base._retn ();
      }
      break;
    case ::Test::A::ARG3:
      {
        ::Test::vtp_var arg3_cleaner = arg3;
        CORBA::ValueBase_var arg3_base =
          this->vtp_factory_->create_for_unmarshal ();
        arg3 = ::Test::vtp::_downcast (arg3_base.in ());
        arg3->b (CORBA::string_dup (::Test::FailStr));
        arg3->c (CORBA::string_dup (::Test::FailStr));
        arg3_base._retn ();
      }
      break;
    case ::Test::A::RETN:
      {
        CORBA::ValueBase_var res_base =
          this->vtp_factory_->create_for_unmarshal ();
        res = ::Test::vtp::_downcast (res_base.in ());
        res->b (CORBA::string_dup (::Test::FailStr));
        res->c (CORBA::string_dup (::Test::FailStr));
        res_base._retn ();
      }
      break;
    }
  return res;
}

void
A_i::exc_method (::Test::string_field f)
{
  switch (f)
    {
    case ::Test::PLAIN:
      // not really tested.
      break;
    case ::Test::BOUNDED:
      throw ::Test::exc (0,
                         ::Test::FailStr,
                         0);
      break;
    case ::Test::TYPEDEFED:
      throw ::Test::exc (0,
                         0,
                         ::Test::FailStr);
      break;
    }
}

void
A_i::shutdown ()
{
  this->orb_->shutdown (0);
}
