// $Id$

//=============================================================================
/**
 *  @file $Name$
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef CIAO_XML_UTILS_H
#define CIAO_XML_UTILS_H

#include "Softpkg_Handlers.h"

namespace CIAO
{
  /**
   * @class Utils
   *
   * @brief Utils contains a collection of utility functions
   */
  class XML_Utils
  {
  public:
    /**
     * @brief Parse a set of component softpkg and return the important info
     *
     * This function parses a set of component softpkg files and
     * returns the important information required to intall the
     * component in a component server.  In CIAO, two different
     * DLL are needed for a component installation.  One for the
     * actual executor implementation and one for the container
     * servant implementation.
     *
     * When calling this function, info->csd_path_ must holds the
     * pathname to the component software descriptor the function is
     * going to parse.
     *
     * @retval 0 if all infor are parsed successfully.
     * @retval -1 if error occured
     */
    static int parse_softpkg (Softpkg_Handler::Softpkg_Info *info);


  };

}

#if defined (__ACE_INLINE__)
# include "XML_Utils.inl"
#endif /* __ACE_INLINE__ */
#endif /* CIAO_XML_UTILS_H */
