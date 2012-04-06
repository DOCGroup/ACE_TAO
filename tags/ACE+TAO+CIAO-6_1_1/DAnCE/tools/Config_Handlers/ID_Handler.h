
//==============================================================
/**
*  @file  ID_Handler.h
*
*  $Id$
*
*  @author Jules White <jules@dre.vanderbilt.edu>
*/
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_ID_Handler_H
#define CIAO_CONFIG_HANDLERS_ID_Handler_H
#include /**/ "ace/pre.h"

#include "Config_Handlers_Export.h"
#include "ace/config-lite.h"

#include "Utils/Functors.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */



namespace Deployment
{
struct ImplementationDependency;
class ImplementationDependencies;

}


namespace DAnCE
{

namespace Config_Handlers
{

class ImplementationDependency;


/*
* @class ID_Handler
*
* @brief Handler class for <ImplementationDependency> types.
*
* This class defines handler methods to map values from
* XSC ImplementationDependency objects, parsed from the descriptor files, to the
* corresponding CORBA IDL Any type.
*
*/
class Config_Handlers_Export ID_Handler
{
public:

ID_Handler (void);
virtual ~ID_Handler (void);

static void get_ImplementationDependency (
const ImplementationDependency& desc,
Deployment::ImplementationDependency& toconfig);

static ImplementationDependency impl_dependency (
const ::Deployment::ImplementationDependency& src);
};

typedef Sequence_Handler < ImplementationDependency,
::Deployment::ImplementationDependencies,
::Deployment::ImplementationDependency,
ID_Handler::get_ImplementationDependency > ID_Functor;

}
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_ID_Handler_H */
