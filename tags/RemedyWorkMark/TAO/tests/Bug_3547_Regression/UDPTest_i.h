// $Id$

#ifndef UDPTest_i_h
#define UDPTest_i_h

#include "UDPTestS.h"

// This class provides the implementation for Stock_Quoter interface.
class UDPTest_i : public virtual POA_UDPTestI
{
public:
   UDPTest_i ();
   virtual ~UDPTest_i ();

   virtual void send( const ::UDPTestI::BufferT Msg );

   bool received_send_;
};

#endif /* Stock_Quoter_i_h */
