// -*- C++ -*-

//=============================================================================
/**
 *  @file     ParameterMode.h
 *
 *  $Id$
 *
 *  This enum is used only in the IDL structs Dynamic::Parameter and 
 *  CORBA::ParameterDescription.
 *
 *  @author  Jeff Parsons
 */
//=============================================================================

#ifndef CORBA_PARAMETERMODE_H
#define CORBA_PARAMETERMODE_H

#include /**/ "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"

class TAO_OutputCDR;
class TAO_InputCDR;

namespace CORBA
{
  enum ParameterMode
    {
      PARAM_IN,
      PARAM_OUT,
      PARAM_INOUT
    };
}

TAO_Export CORBA::Boolean 
operator<< (TAO_OutputCDR &, const CORBA::ParameterMode &);

TAO_Export CORBA::Boolean 
operator>> (TAO_InputCDR &, CORBA::ParameterMode &);

#include /**/ "ace/post.h"

#endif  /* CORBA_PARAMETERMODE_H */
