// $Id$

#include "common.h"

class mySFP_Callback: public SFP_Callback
{
  public:
  // This is called for both active and passive start.
  virtual int start_failed (void) 
    {
      return  0;
    }

  // This is a callback for both active and passive stream
  // establshment.
  virtual int stream_established (void)
    {
      return 0;
    }  

  virtual int receive_frame (ACE_Message_Block *frame)
    {
      ACE_DEBUG ((LM_DEBUG,"mySFP_Callback:receive_frame\n"));
      char *buf =frame->rd_ptr ();
      ACE_DEBUG ((LM_DEBUG,"length of buf = %d\n",frame->length ()));
      for (int i=0;i<frame->length ();i++)
        ACE_DEBUG ((LM_DEBUG,"%c ",buf[i]));
      ACE_DEBUG ((LM_DEBUG,"\n"));
      return 0;
    }
};

int
main (int argc, char **argv)
{
  TAO_ORB_Manager orb_manager;
  
  orb_manager.init (argc,argv);

  ACE_Time_Value timeout1 (5),timeout2 (50);

  mySFP_Callback callback;
  SFP sfp (orb_manager.orb (),
           TAO_ORB_Core_instance ()->reactor (),
           timeout1,
           timeout2,
           &callback);

  int result;
  // passive start.
  result = sfp.start_stream (server_addr,10);
  if (result != 0)
    return 1;
  
  result = orb_manager.run ();
  if (result == 0)
    ACE_DEBUG ((LM_DEBUG,"ORB run timed out\n"));
  else if (result == -1)
    ACE_DEBUG ((LM_DEBUG,"ORB run error\n"));
  return 0;
}
