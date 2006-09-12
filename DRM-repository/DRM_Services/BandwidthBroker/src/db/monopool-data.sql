use ARMSBB;

## Insert router Switch1
INSERT INTO Router (name, equipmentID, vendor, osVersion, adminState,
  operationalState, password, enablePW, managedIPAddr, isProviderEdge,
  lastACL)
VALUES
  ('Switch1', '4507', 'Cisco', '12.0', 'up', 'up', '', 'password',
   '192.168.81.1', 1, 0),
  ('Switch2', '4507', 'Cisco', '12.0', 'up', 'up', '', 'password',
   '192.168.82.1', 1, 0);

INSERT INTO Switch (name, equipmentID, vendor, osVersion, adminState,
  operationalState, password, enablePW, managedIPAddr, lastACL)
VALUES
   ('Switch1', '4507', 'Cisco', '12.0', 'up', 'up', '', 'password', '192.168.81.1', 0);


## Insert PTPs
INSERT INTO PTP (name, adminState, operationalState, rate, phyAddress,
  switchID, routerID)
VALUES
  ('vlan61', 'up', 'up', 10000, 'phy', NULL, 1),
  ('vlan62', 'up', 'up', 10000, 'phy', NULL, 1),
  ('vlan63', 'up', 'up', 10000, 'phy', NULL, 2),
  ('FastEthernet5/46', 'up', 'up', 10000, 'phy', NULL, 1),
  ('GigabitEthernet3/46', 'up', 'up', 10000, 'phy', NULL, 2),
  ('FastEthernet5/7', 'up', 'up', 10000, 'phy', 1, NULL),
  ('FastEthernet5/8', 'up', 'up', 10000, 'phy', 1, NULL);


## Insert L3Interface
INSERT INTO L3Interface (name, adminState, operationalState, ipAddress, ipMask,
  vlanID, ptpID, routerID)
VALUES
  ('vlan61', 'up', 'up', '192.168.81.1', '255.255.255.0', NULL, 1, 1),
  ('vlan62', 'up', 'up', '192.168.82.1', '255.255.255.0', NULL, 2, 1),
  ('vlan63', 'up', 'up', '192.168.83.1', '255.255.255.0', NULL, 3, 2),
  ('FastEthernet5/46', 'up', 'up', '192.168.200.1', '255.255.255.0', NULL, 4, 1),
  ('GigabitEthernet3/46', 'up', 'up', '192.168.200.2', '255.255.255.0', NULL, 5, 2);

## Insert L2Interface
INSERT INTO L2Interface (name, adminState, operationalState, vlanID, ptpID, 
                         switchID)
VALUES
  ('FastEthernet5/7', 'up', 'up', 61, 6, 1),
  ('FastEthernet5/8', 'up', 'up', 61, 7, 1);

## Insert ServiceClass

INSERT INTO ServiceClass (name, dscp, dropPrecedence, utilLimit)
VALUES
   ('highPriority', 40, 0, 20),   
   ('highReliability', 32, 0, 30),   
   ('mmVideo', 24, 0, 40),
   ('mmVoice', 24, 0, 40),
   ('bestEffort', 0, 0, 10);   
 

#Insert BWPerClass
INSERT INTO BWPerClass (bwAllocated, bwTotal, svcClassID, ptpID, inbound)
VALUES
   (0, 2000000, 1, 1, 0),
   (0, 3000000, 2, 1, 0),	
   (0, 4000000, 3, 1, 0),	
   (0, 1000000, 4, 1, 0),
   (0, 2000000, 1, 2, 0),
   (0, 3000000, 2, 2, 0),	
   (0, 4000000, 3, 2, 0),	
   (0, 1000000, 4, 2, 0),
   (0, 2000000, 1, 3, 0),
   (0, 3000000, 2, 3, 0),	
   (0, 4000000, 3, 3, 0),	
   (0, 1000000, 4, 3, 0),
   (0, 2000000, 1, 1, 1),
   (0, 3000000, 2, 1, 1),	
   (0, 4000000, 3, 1, 1),	
   (0, 1000000, 4, 1, 1),
   (0, 2000000, 1, 2, 1),
   (0, 3000000, 2, 2, 1),	
   (0, 4000000, 3, 2, 1),	
   (0, 1000000, 4, 2, 1),
   (0, 2000000, 1, 3, 1),
   (0, 3000000, 2, 3, 1),	
   (0, 4000000, 3, 3, 1),	
   (0, 1000000, 4, 3, 1),
   (0, 2000000, 1, 4, 1),
   (0, 3000000, 2, 4, 1),	
   (0, 4000000, 3, 4, 1),	
   (0, 1000000, 4, 4, 1),
   (0, 2000000, 1, 5, 1),
   (0, 3000000, 2, 5, 1),	
   (0, 4000000, 3, 5, 1),	
   (0, 1000000, 4, 5, 1),
   (0, 2000000, 1, 6, 1),
   (0, 3000000, 2, 6, 1),	
   (0, 4000000, 3, 6, 1),	
   (0, 1000000, 4, 6, 1),
   (0, 2000000, 1, 7, 1),
   (0, 3000000, 2, 7, 1),	
   (0, 4000000, 3, 7, 1),	
   (0, 1000000, 4, 7, 1),
   (0, 2000000, 1, 6, 0),
   (0, 3000000, 2, 6, 0),	
   (0, 4000000, 3, 6, 0),	
   (0, 1000000, 4, 6, 0),
   (0, 2000000, 1, 7, 0),
   (0, 3000000, 2, 7, 0),	
   (0, 4000000, 3, 7, 0),	
   (0, 1000000, 4, 7, 0);

#Insert HostSubnet
INSERT INTO HostSubnet (hostSubnetAddr, hostSubnetMask, l3InterfaceID, poolName, l2InterfaceID, l3Gateway, hostName)
VALUES
    ('192.168.81.100', '255.255.255.0', 1, 'Pool-1A', 1, '192.168.81.1', 'mako'),
    ('192.168.82.101', '255.255.255.0', 1, 'Pool-1A', 2, '192.168.81.1', 'javelin'),
    ('192.168.81.0', '255.255.255.0', 1, 'Pool-1A', NULL, NULL, NULL),			
    ('192.168.82.0', '255.255.255.0', 2, 'Pool-1B', NULL, NULL, NULL),	
    ('192.168.83.0', '255.255.255.0', 3, 'Pool-2A', NULL, NULL, NULL);	

#Insert L3Path
INSERT INTO L3Path (ingressID, egressID)
VALUES
	(1,2),
	(2,1);

#Insert L3Hops
INSERT INTO L3Hops (l3InterfaceID, l3PathID, hopNumber)
VALUES
	(5, 1, 1),
	(4, 2, 1);


# update pool names

#UPDATE HostSubnet SET poolName = 'pool1' WHERE hostSubnetAddr = '192.168.82.0';
#UPDATE HostSubnet SET poolName = 'pool2' WHERE hostSubnetAddr = '192.168.83.0';
   


