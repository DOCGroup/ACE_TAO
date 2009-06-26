// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  SANet_Exceptions.cpp
 *
 * This file contains the exception implementations for spreading activation
 * networks.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#include "SANet_Exceptions.h"

using namespace SANet;

Invalid_Step::Invalid_Step (void)
: std::invalid_argument ("Invalid step argument")
{
  // Nothing to do.
};

Update_Error::Update_Error (void)
: std::logic_error ("Update error")
{
  // Nothing to do.
};

Duplicate_ID::Duplicate_ID (void)
: std::invalid_argument ("Duplicate ID provided for node creation")
{
  // Nothing to do.
};

Insertion_Error::Insertion_Error (void)
: std::logic_error ("Error while attempting to insert a value in map or set")
{
  // Nothing to do.
};

UnknownNode::UnknownNode (void)
: std::invalid_argument ("Unknown node lookup")
{
  // Nothing to do.
};
