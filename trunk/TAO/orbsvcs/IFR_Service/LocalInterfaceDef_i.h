/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/IFR_Service
//
// = FILENAME
//    LocalInterfaceDef_i.h
//
// = DESCRIPTION
//    LocalInterfaceDef servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_LOCALINTERFACEDEF_I_H
#define TAO_LOCALINTERFACEDEF_I_H

#include "InterfaceDef_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_LocalInterfaceDef_i : public virtual TAO_InterfaceDef_i
{
  // = TITLE
  //    TAO_LocalInterfaceDef_i
  //
  // = DESCRIPTION
  //    Represents an local interface definition.
  //
public:
  TAO_LocalInterfaceDef_i (TAO_Repository_i *repo,
                           ACE_Configuration_Section_Key section_key);
  // Constructor

  virtual ~TAO_LocalInterfaceDef_i (void);
  // Destructor  

  virtual CORBA::DefinitionKind def_kind (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return our definition kind.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_LOCALINTERFACEDEF_I_H */


