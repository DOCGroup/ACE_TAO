// $Id$
// Allows application to configure a variable number of rate groups.  (Boeing Extension)
#ifndef TAO_Rate_Group_Info_h
#define TAO_Rate_Group_Info_h 1

#include "orbsvcs/orbsvcs_export.h"
#include "orbsvcs/RtecEventCommC.h"
#include "orbsvcs/RtecSchedulerC.h"

class TAO_ORBSVCS_Export TAO_Rate_Group_Info
{
public:
   TAO_Rate_Group_Info();
   TAO_Rate_Group_Info(RtecScheduler::Period_t rate_in_usec, RtecScheduler::OS_Priority rate_group_priority);
   virtual ~TAO_Rate_Group_Info();
   
   void set_rate(RtecScheduler::Period_t rate_in_usec);
   RtecScheduler::Period_t get_rate() const;

   void set_rate_group_priority(RtecScheduler::OS_Priority rate_group_priority);
   RtecScheduler::OS_Priority get_rate_group_priority() const;

   TAO_Rate_Group_Info(const TAO_Rate_Group_Info &right);
   TAO_Rate_Group_Info & operator=(const TAO_Rate_Group_Info &rate_group_info);

   static void set_num_rate_groups(int num_rate_groups);
   static int get_num_rate_groups();

   static void set_corba_base_priority(int base_corba_priority);
   static int get_corba_base_priority();

   static void set_priority_spacing(int priority_spacing);
   static int get_priority_spacing();
   
   static void set_priorities_contiguous(int priorities_contiguous);
   static int get_priorities_contiguous();

   // Number of rate groups allowed in system. This determines how many rate groups, 
   // queues, etc ... will be configured.
//   enum { MAX_RATE_GROUP = 6 };

private:
   
   RtecScheduler::OS_Priority rate_group_priority_;
   RtecScheduler::Period_t rate_in_usec_;

   static int num_rate_groups_;
   static int base_corba_priority_;
   static int priority_spacing_;
   static int priorities_contiguous_;
};

#if defined (__ACE_INLINE__)
#include "Rate_Group_Info.i"
#endif /* __ACE_INLINE__ */

#endif
