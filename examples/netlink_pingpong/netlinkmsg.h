// $Id$
/*! \file netlinmmsg.h
 * 
 *  \brief a message struct which is passed to the and from the driver
 */
#ifndef NETLINKMSG_H
#define NETLINKMSG_H

 /*! \class NetlinkMsgHeader
    \brief  a simple netlink message header
*/
typedef struct _NetlinkMsgHeader_{

      int dw_pid; //!< message agent's pid
      int dw_num; //!< message number
}NetlinkMsgHeader;


/*! \class NetlinkMsg
    \brief  a simple netlink message 
*/
typedef struct _NetlinkMsg_{

    NetlinkMsgHeader h;
    char data[512];
}NetlinkMsg;

#endif
