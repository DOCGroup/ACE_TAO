#$Id$
#
# Drop all tables
#

CREATE DATABASE IF NOT EXISTS ARMSBB;

use ARMSBB;


DROP TABLE IF EXISTS L2Hops, L3Hops, L2Path, L3Path, activeVLAN, Link;

DROP TABLE IF EXISTS HostSubnet, FlowRecord, ServiceClass, BWPerClass;

DROP TABLE IF EXISTS Router, Switch, L3Interface, L2Interface,PTP, FaultSubscriptions;

/**
 *
 * Convention:
 *   Logical Name     Data Type            Default Value
 *   name             VARCHAR(30)
 *   code             CHAR(30)
 *   ip               CHAR(20)
 *   ID               INT UNSIGNED    auto_increment
 *   number           INT(11)              NULL
 *          <- probably use mediumint unsigned ?
 *   bw/speed         INT UNSIGNED         
 *          <- in kbps (in 1000 bps)
 *   long name        VARCHAR(50)
 *          <- a set? but the set is yet defined
 *   isXXX            BOOL
 *   priority         SMALLINT UNSIGNED    
 * configState     ENUM("NEW", "CONFIGURED", "CONFIG", "DELETE")
 *                                     default 'CONFIG',
 * status         BOOL             NOT NULL default '0', ## down
 */



#
# Table structure for 'Switch'
#

CREATE TABLE IF NOT EXISTS Switch (
  id             INT UNSIGNED   NOT NULL auto_increment,
  name           VARCHAR(30)    NOT NULL default 'switchName',
  adminState     VARCHAR(30)    default 'up',
  operationalState VARCHAR(30)  NOT NULL default 'OPERATIONAL',
  equipmentID    CHAR(30)            NOT NULL default '',
  vendor         VARCHAR(50)         default NULL,
  osVersion      VARCHAR(30)         default NULL,
  password       CHAR(30)            NOT NULL,
  enablePW       CHAR(30)            NOT NULL,
# Ravi - added read Community attribute - for SNMP
  readCommunity   CHAR(20)            NOT NULL default 'public',
  managedIPAddr   CHAR(20)            NOT NULL,
#RV - added attribute for FaultMonitoring
  isMonitored    BOOL	default '0',
  lastACL        SMALLINT UNSIGNED   default '1',
  PRIMARY KEY (id),
  UNIQUE KEY switch_name_index (name)
) ENGINE = INNODB;


#
# Table structure for 'Router'
#

CREATE TABLE IF NOT EXISTS Router (
  id             INT UNSIGNED   NOT NULL auto_increment,
  name           VARCHAR(30)         NOT NULL default 'routerName',
  equipmentID    CHAR(30)            NOT NULL default '',
  vendor         VARCHAR(50)         default NULL,
  osVersion      VARCHAR(30)         default NULL,
  adminState    VARCHAR(30)         default 'up',
  operationalState VARCHAR(30)      default 'up',
  password       CHAR(30)            NOT NULL,
  enablePW       CHAR(30)            NOT NULL,
  managedIPAddr   CHAR(20)            NOT NULL,
  isProviderEdge  BOOL               NOT NULL default '0',
  lastACL        SMALLINT UNSIGNED   default '1',
  PRIMARY KEY (id),
  UNIQUE KEY router_name_index (name)
) ENGINE = INNODB;

#
# Table structure for 'PTP'
#
CREATE TABLE IF NOT EXISTS PTP (
  id             INT UNSIGNED   NOT NULL auto_increment,
  name           VARCHAR(30)         NOT NULL default 'l2name',
  adminState     VARCHAR(30)         default 'up',
  operationalState VARCHAR(30)      default 'up',
  rate           INT                 NOT NULL default '0',
  phyAddress     VARCHAR(30)         default 'NULL',
  ifIndex	 INT UNSIGNED,
  switchID       INT UNSIGNED   NULL REFERENCES Switch (id),
  routerID       INT UNSIGNED   NULL REFERENCES Router (id),
  lastEvent	 VARCHAR (30),
  PRIMARY KEY (id),
  UNIQUE KEY (switchID, routerID, name)
) ENGINE = INNODB;


#
# Table structure for 'L2Interface'
#
CREATE TABLE IF NOT EXISTS L2Interface (
  id             INT UNSIGNED   NOT NULL auto_increment,
  name           VARCHAR(30)         NOT NULL default 'l2name',
  adminState     VARCHAR(30)         default 'up',
  operationalState VARCHAR(30)      default 'up',
  vlanID         INT UNSIGNED        default 0,
  ptpID         INT UNSIGNED        default 0,	
  switchID       INT UNSIGNED   NOT NULL REFERENCES Switch (id),
  PRIMARY KEY (id),
  UNIQUE KEY (switchID, name)
) ENGINE = INNODB;

#
# Ravi - added
# Table structure for 'activeVLAN'
#
CREATE TABLE IF NOT EXISTS activeVLAN (
  id             INT UNSIGNED   NOT NULL auto_increment,
  vlanID         INT UNSIGNED   NOT NULL default '0', 
  ptpID          INT UNSIGNED   NOT NULL default '0',
  switchID       INT UNSIGNED   NOT NULL REFERENCES Switch (id),
  PRIMARY KEY (id),
  UNIQUE KEY (switchID, ptpID, vlanID)
) ENGINE = INNODB;

#
# Table structure for 'Link'
# refers to a dual or bi-directional "link"
# Ravi - added
#
CREATE TABLE IF NOT EXISTS Link (
  id             INT UNSIGNED   NOT NULL auto_increment,
  aPtpID          INT UNSIGNED   NOT NULL default '0',
  zPtpID          INT UNSIGNED   NOT NULL default '0',
  PRIMARY KEY (id),
  UNIQUE KEY (aPtpID, zPtpID)
) ENGINE = INNODB;


#
# Table structure for 'L3Interface'
#
CREATE TABLE IF NOT EXISTS L3Interface (
  id             INT UNSIGNED   NOT NULL auto_increment,
  name           VARCHAR(30)         NOT NULL default 'l2name',
  adminState     VARCHAR(30)         default 'up',
  operationalState VARCHAR(30)      default 'up',
  ipAddress      VARCHAR(30)         default 'NULL',
  ipMask         VARCHAR(30)         default 'NULL',
  vlanID         INT UNSIGNED        default 0,	
  ptpID         INT UNSIGNED        default 0,	
  routerID       INT UNSIGNED   NOT NULL REFERENCES Router (id),
  PRIMARY KEY (id),
  UNIQUE KEY (routerID, name)
) ENGINE = INNODB;


#
# Table structure for 'ServiceClass'
#
CREATE TABLE IF NOT EXISTS ServiceClass (
  id             INT UNSIGNED   NOT NULL auto_increment,
  name           VARCHAR(30)    NOT NULL default 'l2name',
  dscp           INT UNSIGNED   default '0',
  dropPrecedence INT UNSIGNED   default '0',
  utilLimit	 INT UNSIGNED   NOT NULL,
  PRIMARY KEY (id),
  UNIQUE KEY (name)
) ENGINE = INNODB;


#
# Table structure for 'BWPerClass'
#
CREATE TABLE IF NOT EXISTS BWPerClass (
  id             INT UNSIGNED   NOT NULL auto_increment,
  bwAllocated    INT UNSIGNED        default '0',
  bwTotal        INT UNSIGNED        default '0',
  svcClassID     INT UNSIGNED   NOT NULL REFERENCES ServiceClass (id),
  ptpID  INT UNSIGNED   REFERENCES PTP (id),
  inbound        BOOL default '1',
  PRIMARY KEY (id),
  UNIQUE KEY (svcClassID, ptpID, inbound)
) ENGINE = INNODB;


#
# Table structure for 'FlowRecord'
#
CREATE TABLE IF NOT EXISTS FlowRecord (
  id             INT UNSIGNED   NOT NULL auto_increment,
  status         VARCHAR (30)        NOT NULL default 'reserved',
  purgeStatus    BOOL                NOT NULL default '0',
  srcIP          VARCHAR (30)        NOT NULL,
  srcMask        VARCHAR (30)        NOT NULL,
  srcPort        VARCHAR (30)        NOT NULL,
  srcPortHigh    VARCHAR (30)        NOT NULL,
  destIP         VARCHAR (30)        NOT NULL,
  destMask       VARCHAR (30)        NOT NULL,
  destPort       VARCHAR (30)        NOT NULL,
  destPortHigh   VARCHAR (30)        NOT NULL default '',
  protocol	 VARCHAR (30)        NOT NULL default 'tcp',
  dscp           INT UNSIGNED        default 0,
  bandwidthMin   INT UNSIGNED        default '0',
  extraBudget    INT UNSIGNED        default '0',
  bwAllocated    INT UNSIGNED        default '0',
  acl            SMALLINT UNSIGNED   default '0',
  svcClassID     INT UNSIGNED   NOT NULL REFERENCES ServiceClass (id),
  inInterfaceID  INT UNSIGNED   NOT NULL,
  outInterfaceID INT UNSIGNED   NOT NULL,
  priority	 INT UNSIGNED        default 1,
  l2Endpoints    BOOL                NOT NULL default '0',
  reference	 VARCHAR (32)	     default NULL,
  UNIQUE KEY (srcIP, destIP, dscp),
  PRIMARY KEY (id)
) ENGINE = INNODB;


#
# Table structure for 'HostSubnet'
#
CREATE TABLE IF NOT EXISTS HostSubnet (
  id             INT UNSIGNED   NOT NULL auto_increment,
  hostSubnetAddr VARCHAR (30)        NOT NULL,
  hostSubnetMask VARCHAR (30)        NOT NULL,
  l3InterfaceID  INT UNSIGNED   REFERENCES L3Interface (id),
  poolName       VARCHAR (30)   default NULL,
  l2InterfaceID  INT UNSIGNED   default NULL REFERENCES L2Interface (id),
  l3Gateway      VARCHAR (30)   default NULL,
  hostName       VARCHAR (30)   default NULL,
  PRIMARY KEY (id)
) ENGINE = INNODB;



#
# Table structure for 'L3Path'
#

CREATE TABLE IF NOT EXISTS L3Path (
  id             INT UNSIGNED   NOT NULL auto_increment,
  ingressID      INT UNSIGNED   NOT NULL REFERENCES Router (id),
  egressID       INT UNSIGNED   NOT NULL REFERENCES Router (id),
  PRIMARY KEY (id)
) ENGINE = INNODB;



#
# Table structure for 'L2Path'
#

CREATE TABLE IF NOT EXISTS L2Path (
  id             INT UNSIGNED   NOT NULL auto_increment,
  ingressIPHop   VARCHAR (30)        NOT NULL,
  egressIPHop    VARCHAR (30)        NOT NULL,
  PRIMARY KEY (id)
) ENGINE = INNODB;


#
# Table structure for 'L3Hops'
#

CREATE TABLE IF NOT EXISTS L3Hops (
  id             INT UNSIGNED   NOT NULL auto_increment,
  l3InterfaceID  INT UNSIGNED   REFERENCES L3Interface (id),
  l3PathID       INT UNSIGNED   REFERENCES L3Path (id),
  hopNumber      SMALLINT UNSIGNED   NOT NULL,
  UNIQUE KEY (id, hopNumber, l3PathID),
  UNIQUE KEY (id, l3InterfaceID, l3PathID),
  PRIMARY KEY (id)
) ENGINE = INNODB;


#
# Table structure for 'L2Hops'
#

CREATE TABLE IF NOT EXISTS L2Hops (
  id             INT UNSIGNED   NOT NULL auto_increment,
  l2InterfaceID  INT UNSIGNED   REFERENCES L3Interface (id),
  l2PathID       INT UNSIGNED   REFERENCES L3Path (id),
  hopNumber      SMALLINT UNSIGNED   NOT NULL,
  UNIQUE KEY (id, hopNumber, l2PathID),
  UNIQUE KEY (id, l2InterfaceID, l2PathID),
  PRIMARY KEY (id)
) ENGINE = INNODB;

#
# Table structure for 'FaultSubscriptions'
#

CREATE TABLE IF NOT EXISTS FaultSubscriptions (
  id             INT UNSIGNED   NOT NULL auto_increment,
  eventSinkName  VARCHAR(30)    NOT NULL, 
  eventSinkType  INT UNSIGNED   NOT NULL default '0',
  cookie         VARCHAR(30)    NOT NULL,
  switchID       INT UNSIGNED   NOT NULL REFERENCES Switch (id),
  eventType	 INT UNSIGNED 	default '8',
  UNIQUE KEY (eventSinkName, eventSinkType, cookie, switchID, eventType),
  PRIMARY KEY (id)
) ENGINE = INNODB;

#
# Table structure for 'NetworkMonitorEventSubscriptions'
#

CREATE TABLE IF NOT EXISTS NetworkMonitorEventSubscriptions (
  id             	INT UNSIGNED   NOT NULL auto_increment,
  eventConsumer  	VARCHAR(100)    NOT NULL, 
  cookie         	VARCHAR(100)    NOT NULL,
  probeID        	VARCHAR(100)    NOT NULL,
  eventSupplierMonitor  VARCHAR(255)    NOT NULL,
  UNIQUE KEY (eventConsumer, cookie, probeID, eventSupplierMonitor),
  PRIMARY KEY (id)
) ENGINE = INNODB;

