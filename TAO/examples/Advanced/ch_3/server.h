
//=============================================================================
/**
 *  @file    server.h
 *
 *  @author Source code used in TAO has been modified and adapted from the codeprovided in the book
 *  @author "Advanced CORBA Programming with C++" by MichiHenning and Steve Vinoski. Copyright 1999. Addison-Wesley
 *  @author Reading
 *  @author MA.  Used with permission of Addison-Wesley.Modified for TAO by Mike Moran <mm4@cs.wustl.edu>
 */
//=============================================================================


#ifndef server_HH_
#define server_HH_

#include "timeS.h"

class Time_impl : public virtual POA_Time
{
public:
  Time_impl () = default;

  Time_impl (Time_impl const&) = delete;
  Time_impl (Time_impl&&) = delete;
  Time_impl& operator= (Time_impl const&) = delete;
  Time_impl& operator= (Time_impl&&) = delete;

  TimeOfDay get_gmt () override;
};

#endif /* server_HH_ */
