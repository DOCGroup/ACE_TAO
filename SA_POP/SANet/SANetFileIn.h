// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  SANetFileIn.h
 *
 * This file contains the SANetFileIn class definition for the input adapter
 * that initializes a SANet object using an XML SANet file.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SANET_SANET_FILE_IN_H_
#define SANET_SANET_FILE_IN_H_

#include "SANet_Types.h"
#include "SANet.h"

#if !defined (SANET_STANDALONE)
#include "SA_POP/Builder.h"
#include "SA_POP/SANet/SANetFileIn_Export.h"
#endif

namespace SANet {

  /**
   * @class SANetFileIn
   *
   * @brief Input adapter that initializes a SANet object using an XML
   *        SANet file.
   */
#if !defined (SANET_STANDALONE)
  class SANETFILEIN_Export SANetFileIn {
#else
  class SANetFileIn {
#endif /* SANET_STANDALONE */
  public:
    /// Constructor.
    SANetFileIn (void);

    /// Destructor.
    virtual ~SANetFileIn (void);

    /// Buffer size for string conversion.
    static const size_t STR_BUF_SIZE = 129;

    /// Create network from XML file.
    /**
     * @param filename  Name of XML SANet network file.
     *
     * @return  New SANet network.
     */
    virtual Network *build_net (std::string filename);

#if !defined (SANET_STANDALONE)
    /// Build network from XML file.
    /**
     * @param filename  Name of XML SANet network file.
     *
     * @param builder  SA-POP builder to use in building the SANet network.
     */
    virtual void build_net (std::string filename, SA_POP::Builder *builder);
#endif /* SANET_STANDALONE not defined */
  };
};  /* SANet namespace */

#endif /* SANET_SANET_FILE_IN_H_ */
