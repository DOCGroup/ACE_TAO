// -*- C++ -*-
// $Id$

#include "tao/Messaging/ExceptionHolder_i.h"

#include "tao/Messaging/Messaging.h"
#include "tao/Exception_Data.h"
#include "tao/CDR.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  ExceptionHolder::ExceptionHolder (void) :
    data_ (0),
    count_ (0),
    char_translator_ (0),
    wchar_translator_ (0)
  {
  }

  ExceptionHolder::ExceptionHolder (
      ::CORBA::Boolean is_system_exception,
      ::CORBA::Boolean byte_order,
      const ::CORBA::OctetSeq &marshaled_exception,
      ::TAO::Exception_Data* data,
      ::CORBA::ULong exceptions_count,
      ACE_Char_Codeset_Translator *char_translator,
      ACE_WChar_Codeset_Translator *wchar_translator) :
        ::OBV_Messaging::ExceptionHolder (is_system_exception, byte_order, marshaled_exception),
        data_ (data),
        count_ (exceptions_count),
        char_translator_ (char_translator),
        wchar_translator_ (wchar_translator)
  {
  }

  ExceptionHolder::~ExceptionHolder ()
  {
  }

  void
  ExceptionHolder::set_exception_data (::TAO::Exception_Data* data,
                                       ::CORBA::ULong exceptions_count)
  {
    this->data_ = data;
    this->count_ = exceptions_count;
  }

  void ExceptionHolder::raise_exception (void)
    {
      TAO_InputCDR _tao_in ((const char*) this->marshaled_exception ().get_buffer (),
                            this->marshaled_exception ().length (),
                            this->byte_order ());

      _tao_in.char_translator (this->char_translator_);
      _tao_in.wchar_translator (this->wchar_translator_);

      CORBA::String_var type_id;

      if (!(_tao_in >> type_id.inout ()))
        {
          // Could not demarshal the exception id, raise a local
          // CORBA::MARSHAL
          throw ::CORBA::MARSHAL (TAO::VMCID, CORBA::COMPLETED_YES);
        }

      if (this->is_system_exception ())
        {
          CORBA::ULong minor = 0;
          CORBA::ULong completion = 0;
          if (!(_tao_in >> minor) ||
              !(_tao_in >> completion))
            {
              throw ::CORBA::MARSHAL (TAO::VMCID, CORBA::COMPLETED_MAYBE);
            }

          CORBA::SystemException* exception =
            TAO::create_system_exception (type_id.in ());

          if (!exception)
            {
              // @@ We should raise a CORBA::NO_MEMORY, but we ran out
              //    of memory already. We need a pre-allocated, TSS,
              //    CORBA::NO_MEMORY instance
              ACE_NEW (exception, CORBA::UNKNOWN);
            }
          exception->minor (minor);
          exception->completed (CORBA::CompletionStatus (completion));

          // Raise the exception.
          ACE_Auto_Basic_Ptr<CORBA::SystemException> e_ptr(exception);
          exception->_raise ();

          return;
        }

      // Match the exception interface repository id with the
      // exception in the exception list.
      // This is important to decode the exception.
      for (CORBA::ULong i = 0; i != this->count_; ++i)
        {
          if (ACE_OS::strcmp (type_id.in (), this->data_[i].id) != 0)
            continue;

          CORBA::Exception * const exception = this->data_[i].alloc ();

          if (exception == 0)
            {
              throw ::CORBA::NO_MEMORY (TAO::VMCID, CORBA::COMPLETED_YES);
            }
          else
            {
              exception->_tao_decode (_tao_in);
            }

          // Raise the exception.
          ACE_Auto_Basic_Ptr<CORBA::Exception> e_ptr (exception);
          exception->_raise ();

          return;
        }

      // If we couldn't find the right exception, report it as
      // CORBA::UNKNOWN.

      // @@ It would seem like if the remote exception is a
      //    UserException we can assume that the request was
      //    completed.
      throw ::CORBA::UNKNOWN (TAO::VMCID, CORBA::COMPLETED_YES);
    }

  void ExceptionHolder::raise_exception_with_list (
      const ::Dynamic::ExceptionList &)
    {
      // todo convert exceptionlist to something we can really use.
      this->raise_exception ();
    }

  CORBA::ValueBase*
  ExceptionHolder::_copy_value (void)
  {
    TAO::ExceptionHolder* ret_val = 0;
    ACE_NEW_THROW_EX (ret_val,
                      ExceptionHolder,
                      CORBA::NO_MEMORY ());

    // @todo According to the latest corba spec we should be able to
    // pass this to the ExceptionHolder constructor but the TAO_IDL
    // compiler doesn't seem to generate this.
    ret_val->is_system_exception (this->is_system_exception ());
    ret_val->byte_order (this->byte_order ());
    ret_val->marshaled_exception (this->marshaled_exception ());

    return ret_val;
  }

  CORBA::ValueBase *
  ExceptionHolderFactory::create_for_unmarshal (void)
  {
    TAO::ExceptionHolder* ret_val = 0;
    ACE_NEW_THROW_EX (ret_val,
                      ExceptionHolder,
                      CORBA::NO_MEMORY ());
    return ret_val;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

