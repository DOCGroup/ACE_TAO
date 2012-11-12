// $Id$

#include "states_test.h"

StatesTest::StatesTest()
{

}

int
StatesTest::run ()
{
  int ret = 0;
  try
    {
      // SAMPLE STATES
      if (DDS_READ_SAMPLE_STATE != ::DDS::READ_SAMPLE_STATE)
        {
          ACE_ERROR ((LM_ERROR, "StatesTest::run - "
                                "ERROR : READ_SAMPLE_STATE states seem "
                                "unequal: DDS4CCM <%u> - RTI <%u>\n",
                                DDS_READ_SAMPLE_STATE,
                                ::DDS::READ_SAMPLE_STATE));
          ++ret;
        }
      if (DDS_NOT_READ_SAMPLE_STATE != ::DDS::NOT_READ_SAMPLE_STATE)
        {
          ACE_ERROR ((LM_ERROR, "StatesTest::run - "
                                "ERROR : NOT_READ_SAMPLE_STATE states seem "
                                "unequal: DDS4CCM <%u> - RTI <%u>\n",
                                DDS_NOT_READ_SAMPLE_STATE,
                                ::DDS::NOT_READ_SAMPLE_STATE));
          ++ret;
        }
      if (DDS_ANY_SAMPLE_STATE != ::DDS::ANY_SAMPLE_STATE)
        {
          ACE_ERROR ((LM_ERROR, "StatesTest::run - "
                                "ERROR : ANY_SAMPLE_STATE states seem "
                                "unequal: DDS4CCM <%u> - RTI <%u>\n",
                                DDS_ANY_SAMPLE_STATE,
                                ::DDS::ANY_SAMPLE_STATE));
          ++ret;
        }
      // VIEW STATES
      if (DDS_NEW_VIEW_STATE != ::DDS::NEW_VIEW_STATE)
        {
          ACE_ERROR ((LM_ERROR, "StatesTest::run - "
                                "ERROR : NEW_VIEW_STATE states seem "
                                "unequal: DDS4CCM <%u> - RTI <%u>\n",
                                DDS_NEW_VIEW_STATE,
                                ::DDS::NEW_VIEW_STATE));
          ++ret;
        }
      if (DDS_NOT_NEW_VIEW_STATE != ::DDS::NOT_NEW_VIEW_STATE)
        {
          ACE_ERROR ((LM_ERROR, "StatesTest::run - "
                                "ERROR : NOT_NEW_VIEW_STATE states seem "
                                "unequal: DDS4CCM <%u> - RTI <%u>\n",
                                DDS_NOT_NEW_VIEW_STATE,
                                ::DDS::NOT_NEW_VIEW_STATE));
          ++ret;
        }
      if (DDS_ANY_VIEW_STATE != ::DDS::ANY_VIEW_STATE)
        {
          ACE_ERROR ((LM_ERROR, "StatesTest::run - "
                                "ERROR : ANY_VIEW_STATE states seem "
                                "unequal: DDS4CCM <%u> - RTI <%u>\n",
                                DDS_ANY_VIEW_STATE,
                                ::DDS::ANY_VIEW_STATE));
          ++ret;
        }
      // VIEW STATES
      if (DDS_ALIVE_INSTANCE_STATE != ::DDS::ALIVE_INSTANCE_STATE)
        {
          ACE_ERROR ((LM_ERROR, "StatesTest::run - "
                                "ERROR : ALIVE_INSTANCE_STATE states seem "
                                "unequal: DDS4CCM <%u> - RTI <%u>\n",
                                DDS_ALIVE_INSTANCE_STATE,
                                ::DDS::ALIVE_INSTANCE_STATE));
          ++ret;
        }
      if (DDS_NOT_ALIVE_DISPOSED_INSTANCE_STATE !=
          ::DDS::NOT_ALIVE_DISPOSED_INSTANCE_STATE)
        {
          ACE_ERROR ((LM_ERROR, "StatesTest::run - "
                                "ERROR : NOT_ALIVE_DISPOSED_INSTANCE_STATE states seem "
                                "unequal: DDS4CCM <%u> - RTI <%u>\n",
                                DDS_NOT_ALIVE_DISPOSED_INSTANCE_STATE,
                                ::DDS::NOT_ALIVE_DISPOSED_INSTANCE_STATE));
          ++ret;
        }
      if (DDS_NOT_ALIVE_NO_WRITERS_INSTANCE_STATE !=
          ::DDS::NOT_ALIVE_NO_WRITERS_INSTANCE_STATE)
        {
          ACE_ERROR ((LM_ERROR, "StatesTest::run - "
                                "ERROR : NOT_ALIVE_NO_WRITERS_INSTANCE_STATE "
                                "states seem "
                                "unequal: DDS4CCM <%u> - RTI <%u>\n",
                                DDS_NOT_ALIVE_NO_WRITERS_INSTANCE_STATE,
                                ::DDS::NOT_ALIVE_NO_WRITERS_INSTANCE_STATE));
          ++ret;
        }
      if (DDS_ANY_INSTANCE_STATE !=
          ::DDS::ANY_INSTANCE_STATE)
        {
          ACE_ERROR ((LM_ERROR, "StatesTest::run - "
                                "ERROR : ANY_INSTANCE_STATE "
                                "states seem "
                                "unequal: DDS4CCM <%u> - RTI <%u>\n",
                                DDS_ANY_INSTANCE_STATE,
                                ::DDS::ANY_INSTANCE_STATE));
          ++ret;
        }
      if (DDS_NOT_ALIVE_INSTANCE_STATE !=
          ::DDS::NOT_ALIVE_INSTANCE_STATE)
        {
          ACE_ERROR ((LM_ERROR, "StatesTest::run - "
                                "ERROR : NOT_ALIVE_INSTANCE_STATE "
                                "states seem "
                                "unequal: DDS4CCM <%u> - RTI <%u>\n",
                                DDS_NOT_ALIVE_INSTANCE_STATE,
                                ::DDS::NOT_ALIVE_INSTANCE_STATE));
          ++ret;
        }
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "ERROR STATES: Unexpected exception caught."));
      return -1;
    }
  return ret;
}
