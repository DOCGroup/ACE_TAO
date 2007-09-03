// $Id$

#include "CNPM_Handler.h"
#include "CIAOServerResources.hpp"
#include "ace/Auto_Ptr.h"

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

static int const dscp[] =
{
  IPDSFIELD_DSCP_DEFAULT ,
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


namespace CIAO
{
  namespace Config_Handlers
  {

    bool
    CNPM_Handler::cnw_priority_pd (const CNWPriorityModelPolicyDef &src,
      ::CIAO::DAnCE::CNWPriorityModelPolicyDef &dest)
    {
      int request_array_slot = 7;
      long d = dscp[request_array_slot];

      int a = static_cast <ACE_INT16> (d);
      long b = static_cast <ACE_INT32> (a);
      long c = static_cast <ACE_INT32> (a);

      ACE_DEBUG ((LM_DEBUG, "numbers are %d %X %X\n", a, b, c));

      dest.request_dscp = 
        static_cast <ACE_INT32> (src.request_dscp ());
      dest.reply_dscp = 
        static_cast <ACE_INT32> (src.reply_dscp ());

      return true;
    }

    CNWPriorityModelPolicyDef
    CNPM_Handler::cnw_priority_pd (
      const ::CIAO::DAnCE::CNWPriorityModelPolicyDef&)
    {
      auto_ptr <CNWPriorityModelPolicyDef> pmd;
      pmd.reset (new CNWPriorityModelPolicyDef (0, 0));
      return *pmd;
    }
  }
}

