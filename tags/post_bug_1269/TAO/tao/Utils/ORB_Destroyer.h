/**
 * @file ORB_Destroyer.h
 *
 * $Id$
 *
 * @author Marina Spivak <marina@atdesk.com>
 */
#ifndef ORB_DESTROYER__H_
#define ORB_DESTROYER__H_

#include "Auto_Functor.h"

#include "tao/ORB.h"

namespace TAO
{
namespace Utils
{

/**
 * @struct ORB_Destroyer_Functor
 *
 * @brief Implements a functor for the ORB_Destroyer class.
 */
struct ORB_Destroyer_Functor
{
  typedef CORBA::ORB_ptr argument;

  /// Destroy the ORB
  void operator() (CORBA::ORB_ptr orb)
    ACE_THROW_SPEC (());
};

/**
 * @class ORB_Destroyer
 *
 * @brief Helper class to destroy an ORB.
 *
 */
typedef Auto_Functor<
               CORBA::ORB,
               ORB_Destroyer_Functor>
        ORB_Destroyer;

} // namespace Utils
} // namespace TAO

#endif // ORB_DESTROYER__H_
