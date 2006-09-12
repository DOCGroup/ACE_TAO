# $Id$
use ARMSBB;

## Insert router voice4
INSERT INTO Router (name, equipmentID, vendor, osVersion, adminState,
  operationalState, password, enablePW, managedIPAddr, isProviderEdge,
  lastACL)
VALUES
  ('la', '3620', 'Cisco', '12.0', 'up', 'up', 'ngi', 'password',
#   '192.4.20.67', 1, 0),
   'LA', 1, 0),
  ('bos', '3620', 'Cisco', '12.0', 'up', 'up', 'ngi', 'password',
#  '192.4.20.78', 1, 0);
   'BOS', 1, 0),
  ('dc', '3600', 'Cisco', '12.0', 'up', 'up', 'ngi', 'password',
#   '192.168.10.10', 1, 1)
   'DC', 1, 1);

## Insert PTPs
INSERT INTO PTP (name, adminState, operationalState, rate, phyAddress,
  switchID, routerID)
VALUES
  ('ethernet1/2', 'up', 'up', 10000, 'phy', NULL, 1),
  ('ethernet1/1', 'up', 'up', 10000, 'phy', NULL, 1),
  ('ethernet1/3', 'up', 'up', 10000, 'phy', NULL, 2),
  ('ethernet1/2', 'up', 'up', 10000, 'phy', NULL, 2),
  ('FastEthernet3/0', 'up', 'up', 10000, 'phy', NULL, 3),
  ('FastEthernet2/0', 'up', 'up', 10000, 'phy', NULL, 3);

#  ('ethernet2', 'up', 'up', 10000, 'phy', NULL, 3),
#  ('ethernet3', 'up', 'up', 10000, 'phy', NULL, 3);

## Insert L3Interface
INSERT INTO L3Interface (name, adminState, operationalState, ipAddress, ipMask,
  vlanID, ptpID, routerID)
VALUES
  ('ethernet1/2', 'up', 'up', '192.168.85.2', '255.255.255.0', NULL, 1, 1),
  ('ethernet1/1', 'up', 'up', '192.168.82.1', '255.255.255.0', NULL, 2, 1),
  ('ethernet1/3', 'up', 'up', '192.168.85.1', '255.255.255.0', NULL, 3, 2),
  ('ethernet1/2', 'up', 'up', '192.168.81.1', '255.255.255.0', NULL, 4, 2),
  ('FastEthernet3/0', 'up', 'up', '192.168.85.3', '255.255.255.0', NULL, 5, 3),
  ('FastEthernet2/0', 'up', 'up', '192.168.83.1', '255.255.255.0', NULL, 6, 3);


## Insert ServiceClass

INSERT INTO ServiceClass (name, dscp, dropPrecedence, utilLimit)
VALUES
   ('highPriority', 40, 0, 20),   
   ('highReliability', 32, 0, 30),   
   ('highThroughput', 24, 0, 40),
   ('bestEffort', 0, 0, 10);   
 

#Insert BWPerClass
INSERT INTO BWPerClass (bwAllocated, bwTotal, svcClassID, ptpID)
VALUES
   (0, 2000, 1, 1),
   (0, 3000, 2, 1),	
   (0, 4000, 3, 1),	
   (0, 1000, 4, 1),
   (0, 2000, 1, 3),
   (0, 3000, 2, 3),	
   (0, 4000, 3, 3),	
   (0, 1000, 4, 3),
   (0, 2000, 1, 5),
   (0, 3000, 2, 5),	
   (0, 4000, 3, 5),	
   (0, 1000, 4, 5);

#Insert HostSubnet
INSERT INTO HostSubnet (hostSubnetAddr, hostSubnetMask, l3InterfaceID, poolName)
VALUES
    ('192.168.82.0', '255.255.255.0', 2, 'pool1'),	
    ('192.168.81.0', '255.255.255.0', 4, 'pool1'),			
    ('192.168.83.0', '255.255.255.0', 6, 'pool2');	

#Insert L3Path
INSERT INTO L3Path (ingressID, egressID)
VALUES
	(1,2),
	(2,1),
        (1,3),
        (3,1),
        (2,3),
        (3,2);

#Insert L3Hops
INSERT INTO L3Hops (l3InterfaceID, l3PathID, hopNumber)
VALUES
	(3, 1, 1),
	(1, 2, 1),
	(5, 3, 1),
	(1, 4, 1),
	(5, 5, 1),
	(3, 6, 1);


# update pool names

#UPDATE HostSubnet SET poolName = 'pool1' WHERE hostSubnetAddr = '192.168.82.0';
#UPDATE HostSubnet SET poolName = 'pool2' WHERE hostSubnetAddr = '192.168.83.0';
   


