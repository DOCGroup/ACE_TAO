// $Id$

#include "tao/orbconf.h"
#include "Custom_Network_Priority_Mapping.h"
#include "tao/debug.h"

ACE_RCSID(Strategies, Custom_Network_Priority_Mapping, "$Id$")

#define IPDSFIELD_DSCP_DEFAULT  0x00
#define IPDSFIELD_DSCP_CS1      0x08
#define IPDSFIELD_DSCP_CS2      0x10
#define IPDSFIELD_DSCP_CS3      0x18
#define IPDSFIELD_DSCP_CS4      0x20
#define IPDSFIELD_DSCP_CS5      0x28
#define IPDSFIELD_DSCP_CS6      0x30
#define IPDSFIELD_DSCP_CS7      0x38
#define IPDSFIELD_DSCP_AF11     0x0A
#define IPDSFIELD_DSCP_AF12     0x0C
#define IPDSFIELD_DSCP_AF13     0x0E
#define IPDSFIELD_DSCP_AF21     0x12
#define IPDSFIELD_DSCP_AF22     0x14
#define IPDSFIELD_DSCP_AF23     0x16
#define IPDSFIELD_DSCP_AF31     0x1A
#define IPDSFIELD_DSCP_AF32     0x1C
#define IPDSFIELD_DSCP_AF33     0x1E
#define IPDSFIELD_DSCP_AF41     0x22
#define IPDSFIELD_DSCP_AF42     0x24
#define IPDSFIELD_DSCP_AF43     0x26
#define IPDSFIELD_ECT_MASK      0x02
#define IPDSFIELD_CE_MASK       0x01
#define IPDSFIELD_DSCP_EF       0x2E

static int dscp [] =
  {
    // IPDSFIELD_DSCP_DEFAULT ,
    IPDSFIELD_DSCP_CS1     ,
    IPDSFIELD_DSCP_CS2     ,
    IPDSFIELD_DSCP_CS3     ,
    IPDSFIELD_DSCP_CS4     ,
    IPDSFIELD_DSCP_CS5     ,
    IPDSFIELD_DSCP_CS6     ,
    IPDSFIELD_DSCP_CS7     ,
    IPDSFIELD_DSCP_AF11    ,
    IPDSFIELD_DSCP_AF12    ,
    IPDSFIELD_DSCP_AF13    ,
    IPDSFIELD_DSCP_AF21    ,
    IPDSFIELD_DSCP_AF22    ,
    IPDSFIELD_DSCP_AF23    ,
    IPDSFIELD_DSCP_AF31    ,
    IPDSFIELD_DSCP_AF32    ,
    IPDSFIELD_DSCP_AF33    ,
    IPDSFIELD_DSCP_AF41    ,
    IPDSFIELD_DSCP_AF42    ,
    IPDSFIELD_DSCP_AF43    ,
    IPDSFIELD_DSCP_EF      
  };

/*
static const char *dscp_char[]=
  {
    //"Normal",
    "CS1",
    "CS2",
    "CS3",
    "CS4",
    "CS5",
    "CS6",
    "CS7",
    "Assured Forwarding 11",
    "Assured Forwarding 12",
    "Assured Forwarding 13",
    "Assured Forwarding 21",
    "Assured Forwarding 22",
    "Assured Forwarding 23",
    "Assured Forwarding 31",
    "Assured Forwarding 32",
    "Assured Forwarding 33",
    "Assured Forwarding 41",
    "Assured Forwarding 42",
    "Assured Forwarding 43",
    "Expedited Forwarding"
  };
*/

TAO_Custom_Network_Priority_Mapping::TAO_Custom_Network_Priority_Mapping (void)
  : corba_priority_ (0)
{
  this->min_ = 0; //ACE_Sched_Params::priority_min (this->policy_);
  this->max_ = 32767; //ACE_Sched_Params::priority_max (this->policy_);
  
  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG,
		"Min = %d\n Max = %d\n",
		this->min_,
		this->max_));
  this->increment_ = (((this->max_ - this->min_) + 1) / 20) + 1;
}

TAO_Custom_Network_Priority_Mapping::~TAO_Custom_Network_Priority_Mapping (void)
{
}

void
TAO_Custom_Network_Priority_Mapping::desired_priority (int priority)
{
  this->corba_priority_ = priority;
}

CORBA::Boolean
TAO_Custom_Network_Priority_Mapping::to_network (RTCORBA::Priority /*corba_priority*/,
					 RTCORBA::NetworkPriority &network_priority)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
		"TAO_Custom_Network_Priority_Mapping::to_network corba_priority %d\n",
		this->corba_priority_));

  network_priority = dscp [(this->corba_priority_ - this->min_) / this->increment_];
  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG,
		"TAO_Linear_Network_Priority_Mapping::to_network = %x increment = %d\n",
		network_priority,
		this->increment_));
  return 1;
}

CORBA::Boolean
TAO_Custom_Network_Priority_Mapping::to_CORBA (RTCORBA::NetworkPriority /*network_priority*/,
                                       RTCORBA::Priority &/*corba_priority*/)
{
  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG,
		"TAO_Custom_Network_Priority_Mapping::to_corba\n"));
  return 1;
}



