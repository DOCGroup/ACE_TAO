// $Id$

#include "DII_Arguments.h"

#include "tao/NVList.h"
#include "tao/Any_Impl.h"
#include "tao/Exception.h"
#include "tao/CDR.h"
#include "tao/Typecode.h"
#include "tao/DynamicC.h"

ACE_RCSID (DynamicInterface,
           DII_Arguments,
           "$Id$")

#if !defined (__ACE_INLINE__)
# include "DII_Arguments.inl"
#endif /* ! __ACE_INLINE__ */

namespace TAO
{
  CORBA::Boolean
  NamedValue_Argument::demarshal (TAO_InputCDR &cdr)
  {
    ACE_TRY_NEW_ENV
      {
        if (this->x_ !=0 && this->x_->value ()->impl ())
          {
            this->x_->value ()->impl ()->_tao_decode (cdr
                                                      ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
      }
    ACE_CATCHANY
      {
        return 0;
      }
    ACE_ENDTRY;

    this->byte_order_ = cdr.byte_order ();

    return 1;
  }

  void
  NamedValue_Argument::interceptor_result (CORBA::Any *any)
  {
    (*any) <<= *this->x_->value ();
  }

  // ========================================================================

  CORBA::Boolean
  NVList_Argument::marshal (TAO_OutputCDR &cdr)
  {
    ACE_TRY_NEW_ENV
      {
        this->x_->_tao_encode (cdr,
                               CORBA::ARG_IN | CORBA::ARG_INOUT
                               ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
    ACE_CATCHANY
      {
        return 0;
      }
    ACE_ENDTRY;

    return 1;
  }

  CORBA::Boolean
  NVList_Argument::demarshal (TAO_InputCDR &cdr)
  {


    ACE_TRY_NEW_ENV
      {
        // Now, get all the "return", "out", and "inout" parameters
        // from the response message body ... return parameter is
        // first, the rest are in the order defined in the IDL spec
        // (which is also the order that DII users are required to
        // use).

        this->x_->_tao_incoming_cdr (
            cdr,
            CORBA::ARG_OUT | CORBA::ARG_INOUT,
            this->lazy_evaluation_
            ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
    ACE_CATCHANY
      {
        return 0;
      }
    ACE_ENDTRY;

    return 1;
  }

  void
  NVList_Argument::interceptor_paramlist (Dynamic::ParameterList *list)
  {
    const CORBA::ULong len = this->x_->count ();
    list->length (len);

    for (CORBA::ULong i = 0; i < len; ++i)
      {
        (*list)[i].argument <<= *this->x_->item (i)->value ();

        switch (this->x_->item (i)->flags ())
          {
          case CORBA::ARG_IN:
            {
              (*list)[i].mode = CORBA::PARAM_IN;
              break;
            }
          case CORBA::ARG_INOUT:
            {
              (*list)[i].mode = CORBA::PARAM_INOUT;
              break;
            }
          case CORBA::ARG_OUT:
            {
              (*list)[i].mode = CORBA::PARAM_OUT;
              break;
            }
          default:
            break;
          }
      }
  }
}
