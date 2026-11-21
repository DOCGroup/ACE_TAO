#ifndef STATISTICS_HELPER_H
#define STATISTICS_HELPER_H

#include "CSD_TP_Test_Export.h"
#include "tao/Basic_Types.h"
#include "ace/Containers_T.h"
#include "ace/Vector_T.h"
#include "ace/SString.h"

using LongVector = ACE_Vector< CORBA::Long, 1000 >;
using StringVector = ACE_Vector< ACE_CString, 1000 >;
using LongArray = ACE_Array< CORBA::Long >;

extern CSD_TP_Test_Export void swap (CORBA::Long& x, CORBA::Long& y);

extern CSD_TP_Test_Export void sort (LongVector & vector);

#endif
