// $Id$

//=============================================================================
/**
 *  @file XML_Utils.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef CIAO_XML_UTILS_H
#define CIAO_XML_UTILS_H

#include "XML_Helpers_Export.h"
#include "Softpkg_Handlers.h"
#include "Assembly_Spec.h"

namespace CIAO
{
  /**
   * @class Utils
   *
   * @brief Utils contains a collection of utility functions
   */
  class CIAO_XML_HELPERS_Export XML_Utils
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

    static int parse_componentassembly (const char *filename,
                                        Assembly_Spec *spec);

    static long get_id_and_cardinality (const char *&id,
                                        ACEXML_Attributes *atts
                                        ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException)) ;

    static long get_single_attribute (const char *attname,
                                      const char *&id,
                                      ACEXML_Attributes *atts
                                      ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException)) ;

  };

}

#if defined (__ACE_INLINE__)
# include "XML_Utils.inl"
#endif /* __ACE_INLINE__ */
#endif /* CIAO_XML_UTILS_H */
