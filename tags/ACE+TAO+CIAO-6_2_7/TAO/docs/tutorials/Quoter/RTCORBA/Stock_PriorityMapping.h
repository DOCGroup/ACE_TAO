// $Id$

/**
 * @file Stock_PriorityMapping.h
 * @author Shanshan Jiang <shanshan.jiang@vanderbilt.edu>
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Douglas C. Schmidt <schmidt@dre.vanderbilt.edu>
 */

#ifndef STOCK_PRIORITYMAPPING_
#define STOCK_PRIORITYMAPPING_

#include "Common_Export.h"

// TAO headers
#include "tao/RTCORBA/Linear_Priority_Mapping.h"

namespace Stock
{
  /**
   * @class Stock_PriorityMapping
   * @brief Defines the runtime priority mappings used in this application.
   *
   * This class defines the priority mappings used by the stock objects. We
   * are using the TAO_Continuous_Priority_Mapping class as a base class
   * because it provides the necessary mappings of CORBA::Priority values
   * to CORBA::NativePriority values when there is only a limited number
   * of CORBA::Priority values.
   */
  class RTCORBA_Quoter_Common_Export Priority_Mapping :
    public TAO_Linear_Priority_Mapping
  {
  public:
    /// Default constructor.
    Priority_Mapping (void);

    /// Default destructor.
    virtual ~Priority_Mapping (void);

    /// Stock priorities.
    enum Priority
      {
        VERY_LOW = 0,
        LOW = 1,
        MEDIUM = 2,
        HIGH = 3,
        VERY_HIGH = 4
      };

    /// Register mapping.
    static void register_mapping (CORBA::ORB_ptr orb);
  };

}

#endif  // !defined STOCK_PRIORITYMAPPING_
