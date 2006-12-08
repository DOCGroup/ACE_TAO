// $Id$
//======================================
/**
*   @file    DD_Handler.h
*
*   @brief Contains the Domain Descriptior
*   Handler
*
*   @author Nilabja Roy <nilabjar@dre.vanderbilt.edu>
*
*/
//======================================

#ifndef CIAO_CONFIG_HANDLERS_DD_HANDLER_H
#define CIAO_CONFIG_HANDLERS_DD_HANDLER_H
#include /**/ "ace/pre.h"

#include "Config_Handlers_Export.h"
#include "ace/Auto_Ptr.h"
// #include "Utils/XML_Helper.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
struct Domain;
}

namespace CIAO
{
namespace Config_Handlers
{
class Domain;

/*
* @class DD_Handler
*
* @brief Handler class for <Domain>
*/

class Config_Handlers_Export DD_Handler
{
public:
class NoDomain {};

/**
* @param file The file to be read
*/
DD_Handler (const ACE_TCHAR *file);

/**
* @param dmn The Domain structure
*/
DD_Handler(Domain *dmn);

/**
* @param dmn The Deployment::Domain structure
*/
DD_Handler(::Deployment::Domain *dmn);

/**
*/
~DD_Handler (void);

/**
* @brief Returns the const IDL domain
* @return Domain* the IDL domain pointer
*/
::Deployment::Domain const *domain_idl (void) const
throw (NoDomain);

/**
* @brief Returns the IDL domain
* @return Domain* the IDL domain pointer
*/
::Deployment::Domain *domain_idl (void)
throw (NoDomain);

/**
* @brief Returns the const XSC domain
* @return Domain* the XSC domain pointer
*/
Domain const *domain_xsc (void) const
throw (NoDomain);

/**
* @brief Returns the XSC domain
* @return Domain* the XSC domain pointer
*/
Domain *domain_xsc (void)
throw (NoDomain);

private:
/**
*  @brief builds the domain structure
*/
bool build_domain ();

/**
*  @brief builds the XSC data structure
*/
bool build_xsc ();

/// The Deployment Domain structure
auto_ptr < ::Deployment::Domain > idl_domain_;

/// The XSC Domain structure
auto_ptr <Domain> domain_;

/// The return value used
bool retval_;
};
}
}

#include /**/ "ace/post.h"
#endif /* DD_HANDLER_H */
