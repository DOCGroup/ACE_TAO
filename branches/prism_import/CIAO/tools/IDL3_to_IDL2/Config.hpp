/* ===================================================================== */
/*
 * This file is part of CARDAMOM (R) which is jointly developed by THALES
 * and SELEX-SI. It is derivative work based on PERCO Copyright (C) THALES 
 * 2000-2003. All rights reserved.
 * Copyright (C) THALES 2004-2005. All rights reserved.
 * Copyright (C) SELEX-SI and THALES 2006. All rights reserved.
 * 
 * CARDAMOM is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Library General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * CARDAMOM is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Library General Public
 * License for more details.
 * 
 * You should have received a copy of the GNU Library General Public
 * License along with CARDAMOM; see the file COPYING. If not, write to the
 * Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
/* ===================================================================== */

#ifndef CDMW_CODE_GENERATOR_CONFIG_HPP
#define CDMW_CODE_GENERATOR_CONFIG_HPP

#include <iostream>

#define COG_DEBUG(X) std::cout << X << std::endl;
#define COG_INFO(X)  std::cout << X << std::endl;
#define COG_WARN(X)  std::cout << X << std::endl;
#define COG_ERROR(X) std::cerr << X << std::endl;
#define COG_FATAL(X) std::cerr << X << std::endl;

/**
* Root namespace for CDMW runtime.
*/
namespace Cdmw
{
  namespace CodeGenerator
  {
    const char* const BE_VERSION = "V1.0";
       
    const char* const BE_LOGGER_NAME = "CDMW.IDL_BE";
  } // End of TAO_IDL_FE namespace
} // End of Cdmw namespace


#endif // CDMW_CODE_GENERATOR_CONFIG_HPP
