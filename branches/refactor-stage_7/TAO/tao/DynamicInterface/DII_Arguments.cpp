// $Id$

#include "DII_Arguments.h"

#include "tao/NVList.h"
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
        this->x_->value ()->impl ()->_tao_decode (cdr
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
        this->x_->_tao_decode (cdr,
                               CORBA::ARG_INOUT | CORBA::ARG_OUT
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
        (*list)[len].argument <<= *this->x_->item (len)->value ();

        switch (this->x_->item (len)->flags ())
          {
          case CORBA::ARG_IN:
            {
              (*list)[len].mode = CORBA::PARAM_IN;
              break;
            }
          case CORBA::ARG_INOUT:
            {
              (*list)[len].mode = CORBA::PARAM_INOUT;
              break;
            }
          case CORBA::ARG_OUT:
            {
              (*list)[len].mode = CORBA::PARAM_OUT;
              break;
            }
          default:
            break;
          }
      }
  }
}

