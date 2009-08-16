// -*- C++ -*-

//=============================================================================
/**
 *  @file    ComponentAttributesSetter.h
 *
 *  $Id$
 *
 * @Brief  Workaround for component attributes setting
 *
 * @author Erwin Gottlieb <eg@prismtech.com>
 */
//=============================================================================

#ifndef COMPONENTATTRIBUTESSETTER_H_
#define COMPONENTATTRIBUTESSETTER_H_

#include "NodeApplication_Export.h"
#include "Deployment/Deployment_BaseC.h"

class NodeApplication_Export ComponentAttributesSetter
  {
  public:
    ComponentAttributesSetter();
    ~ComponentAttributesSetter();

    static void SetComponentAttributes (ACE_CString componentName,
                                        CORBA::Object_ptr,
                                        const Deployment::Properties& prop,
                                        CORBA::ORB_ptr orb);
  };

#endif /*COMPONENTATTRIBUTESSETTER_H_*/
