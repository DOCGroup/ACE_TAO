// $Id$

#include "common.h"
#include "ace/Event_Handler.h"

char *data = "Hello how are you";

class frame_handler: public ACE_Event_Handler
{
  public:

  frame_handler (ACE_Reactor* reactor,SFP *sfp)
    :sfp_ (sfp)
    {
      reactor->schedule_timer (this,
                               0,
                               20);
    }

  // Called when timer expires.  
  int handle_timeout (const ACE_Time_Value &tv,
                      const void *arg = 0)
    {
       ACE_DEBUG ((LM_DEBUG,"frame_handler:handle_timeout\n"));
       ACE_Message_Block mb (data,
                              ACE_OS::strlen (data),
                              -1);
       mb.wr_ptr (ACE_OS::strlen (data));
       int result = this->sfp_->send_simple_frame (&mb);
       if (result != -1)
         ACE_DEBUG ((LM_DEBUG,"simple_frame sent\n"));
       else
         ACE_DEBUG ((LM_DEBUG,"simple_frame send failed\n"));
       return 0;
//        ACE_DEBUG ((LM_DEBUG,"frame_handler:handle_timeout\n"));
//        char *buf;
//        ACE_NEW_RETURN (buf,
//                        char [4*ACE_MAX_DGRAM_SIZE],
//                        -1);
//        ACE_Message_Block mb (buf,
//                              4*ACE_MAX_DGRAM_SIZE);
//        //       mb.wr_ptr (ACE_OS::strlen (data));
//        mb.wr_ptr (4*ACE_MAX_DGRAM_SIZE);
//        //       int result = this->sfp_->send_simple_frame (&mb);
//        int result = this->sfp_->send_frame (&mb);
//        if (result != -1)
//          ACE_DEBUG ((LM_DEBUG,"simple_frame sent\n"));
//        else
//          ACE_DEBUG ((LM_DEBUG,"simple_frame send failed\n"));
//        return 0;

    }

  private:
  
  SFP *sfp_;
};

int
main (int argc, char **argv)
{
  TAO_ORB_Manager orb_manager;
  
  orb_manager.init (argc,argv);

  ACE_Time_Value timeout1 (5),timeout2 (50);

  SFP sfp (orb_manager.orb (),
           TAO_ORB_Core_instance ()->reactor (),
           timeout1,
           timeout2,
           0);

  int result;

  result = sfp.start_stream (server_addr);
  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR,"sfp start failed\n"),1);

  frame_handler handler (TAO_ORB_Core_instance ()->reactor (),
                         &sfp);

  result = orb_manager.run ();
  if (result == 0)
    ACE_DEBUG ((LM_DEBUG,"ORB run timed out\n"));
  else if (result == -1)
    ACE_DEBUG ((LM_DEBUG,"ORB run error\n"));
  return 0;
}
