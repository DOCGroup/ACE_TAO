// -*- C++ -*-
//=============================================================================
/**
 *  @file   Locator.h
 *
 *  $Id$
 *
 *  @brief  This class implements the ImR's INS Locator class
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#ifndef IMR_LOCATOR_H
#define IMR_LOCATOR_H
#include "ace/pre.h"

#include "tao/IORTable/IORTable.h"

class ImplRepo_i;

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/**
 * @class ImR_Locator
 *
 * @brief Implementation Repository INS Locator class
 *
 * This class provides a callback for the IORTable to call when it needs
 * to dynamically receive a IOR to forward in response to an INS request.
 */
class ImR_Locator : public virtual IORTable::Locator,
                    public virtual TAO_Local_RefCounted_Object
{
public:
  /// Constructor
  ImR_Locator (ImplRepo_i *repo);

  /// Locate the appropriate IOR.
  char * locate (const char *object_key
                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException, IORTable::NotFound));

private:
  /// The Implementation Repository implementation
  ImplRepo_i *repo_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* IMR_LOCATOR_H */
