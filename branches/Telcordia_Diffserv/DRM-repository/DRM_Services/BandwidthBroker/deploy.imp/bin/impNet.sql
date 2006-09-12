-- $Id$
CREATE DATABASE IF NOT EXISTS impNet;

use impNet;


DROP TABLE IF EXISTS host, imp_assign, interface;


CREATE TABLE IF NOT EXISTS host (
 hostname		VARCHAR(64),
 type			VARCHAR(16),                	
#-- "router", "imp"
 mib2_host		CHAR(1),		

 snmp_read_community	VARCHAR(32),
 snmp_timeout		INT,
 snmp_retries		INT,

 telnet_username	VARCHAR(32),
 telnet_password	VARCHAR(32),
 enable_password	VARCHAR(32),

 #-- this next is IMP only, but put it here for now

 gateway_device		VARCHAR(16),
 gateway_address	VARCHAR(32),
 is_linux		CHAR(1),
 has_rtc		CHAR(1),
 PRIMARY KEY (hostname)
);

CREATE TABLE IF NOT EXISTS imp_assign (
 hostname		VARCHAR(64),
 imp			VARCHAR(64),
 PRIMARY KEY (hostname)
);

CREATE TABLE IF NOT EXISTS interface (
 i_address		VARCHAR(16) ,
 i_name			VARCHAR(16),           		#-- physical port rep
 i_index		INT,				#-- snmp index
 i_phys_address		VARCHAR(32),			#-- ethernet address
 hostname		VARCHAR(64),
 mgmt_address		CHAR(1)  	NOT NULL, 	#-- network mgt port
 i_speed		INT	,
			#-- bytes per second
 PRIMARY KEY (i_address)
);
