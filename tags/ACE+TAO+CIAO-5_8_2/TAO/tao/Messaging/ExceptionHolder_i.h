// -*- C++ -*-

//=============================================================================
/**
 *  @file    ExceptionHolder_i.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_MESSAGING_EXCEPTIONHOLDER_I_H
#define TAO_MESSAGING_EXCEPTIONHOLDER_I_H
#include /**/ "ace/pre.h"

#include "tao/Messaging/messaging_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#include "tao/Messaging/ExceptionHolderA.h"
#include "tao/Messaging/ExceptionHolderC.h"
#include "tao/Valuetype/ValueFactory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace Dynamic
{
  class ExceptionList;
}

namespace Messaging
{
  class ReplyHandler;
}

namespace TAO
{
  struct Exception_Data;

  class TAO_Messaging_Export ExceptionHolder
    : public virtual ::OBV_Messaging::ExceptionHolder,
      public virtual ::CORBA::DefaultValueRefCountBase
  {
  public:
    ExceptionHolder (void);

    ExceptionHolder (
      ::CORBA::Boolean is_system_exception,
      ::CORBA::Boolean byte_order,
      const ::CORBA::OctetSeq &marshaled_exception,
      ::TAO::Exception_Data* data,
      ::CORBA::ULong exceptions_count,
      ACE_Char_Codeset_Translator *char_translator,
      ACE_WChar_Codeset_Translator *wchar_translator);

    void set_exception_data (::TAO::Exception_Data* data,
                             ::CORBA::ULong exceptions_count);

    virtual void raise_exception (void);

    virtual void raise_exception_with_list (
        const ::Dynamic::ExceptionList & exc_list);

    virtual CORBA::ValueBase* _copy_value (void);

  protected:

    virtual ~ExceptionHolder (void);

  private:
    TAO::Exception_Data* data_;
    CORBA::ULong count_;
    ACE_Char_Codeset_Translator *char_translator_;
    ACE_WChar_Codeset_Translator *wchar_translator_;
  };

  /**
   * @class ExceptionHolderFactory
   *
   * @brief OBV factory implementation.
   *
   * Factory for ExceptionHolder
   */
  class TAO_Messaging_Export ExceptionHolderFactory :
    public virtual CORBA::ValueFactoryBase
  {
  public:
    virtual CORBA::ValueBase * create_for_unmarshal (void);
  };

}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_MESSAGING_EXCEPTIONHOLDER_I_H */

