// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  TaskMapFileIn.h
 *
 * This file contains the TaskMapFileIn class definition for the input adapter
 * that initializes a TaskMap object using an XML TaskMap file.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_TASK_MAP_FILE_IN_H_
#define SA_POP_TASK_MAP_FILE_IN_H_

#include "SA_POP_Types.h"
#include "Builder.h"


namespace SA_POP {

  /**
   * @class TaskMapFileIn
   *
   * @brief Input adapter that initializes a TaskMap object using an XML
   *        TaskMap file.
   */
  class TaskMapFileIn {
  public:
    /// Constructor.
    TaskMapFileIn (void);

    /// Destructor.
    virtual ~TaskMapFileIn (void);

    /// Buffer size for string conversion.
    static const size_t STR_BUF_SIZE = 129;

    /// Build task map from XML file.
    /**
     * @param filename  Name of XML task map file.
     *
     * @param builder  SA-POP builder to use in building task map.
     */
    virtual void build_task_map (std::string filename, Builder *builder);
  };

};  /* SA_POP namespace */

#endif /* SA_POP_TASK_MAP_FILE_IN_H_ */
