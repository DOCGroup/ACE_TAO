use ARMSBB;

## Insert router Switch1
INSERT INTO Router (name, equipmentID, vendor, osVersion, adminState,
  operationalState, password, enablePW, managedIPAddr, isProviderEdge,
  lastACL)
VALUES
  ('dandelion', '6500', 'Cisco', '12.1', 'up', 'up', '', 'password',
   '192.4.20.146', 1, 0),
  ('dahlia', '6500', 'Cisco', '12.1', 'up', 'up', '', 'password',
   '192.4.20.145', 1, 0),
  ('daisy', '5000', 'Cisco', '12.1', 'up', 'up', '', 'password',
   '192.4.20.62', 1, 0),
  ('daffodil', '5000', 'Cisco', '12.1', 'up', 'up', '', 'password',
   '192.4.20.63', 1, 0);

INSERT INTO Switch (name, equipmentID, vendor, osVersion, adminState,
  operationalState, password, enablePW, managedIPAddr, lastACL)
VALUES
  ('dahlia', '6500', 'Cisco', '12.1', 'up', 'up', '', 'password',
   '192.4.20.145', 0),
  ('daffodil', '5000', 'Cisco', '12.1', 'up', 'up', '', 'password',
   '192.4.20.63', 0),
  ('daisy', '5000', 'Cisco', '12.1', 'up', 'up', '', 'password',
   '192.4.20.62', 0),
  ('dandelion', '6500', 'Cisco', '12.1', 'up', 'up', '', 'password',
   '192.4.20.146', 0);


## Insert PTPs
INSERT INTO PTP (name, adminState, operationalState, rate, phyAddress,
  switchID, routerID)
VALUES
# Interfaces on dahalia
  ('FastEthernet1/2', 'up', 'up', 10000, 'phy', 1, NULL),
  ('FastEthernet1/3', 'up', 'up', 10000, 'phy', 1, NULL),
  ('FastEthernet1/4', 'up', 'up', 10000, 'phy', 1, NULL),
  ('FastEthernet1/5', 'up', 'up', 10000, 'phy', 1, NULL),

  ('FastEthernet1/21', 'up', 'up', 10000, 'phy', 1, NULL),
  ('FastEthernet1/22', 'up', 'up', 10000, 'phy', 1, NULL),
# Interfaces on daffodil
  ('FastEthernet2/2', 'up', 'up', 10000, 'phy', 2, NULL),
  ('FastEthernet2/3', 'up', 'up', 10000, 'phy', 2, NULL),

  ('FastEthernet2/21', 'up', 'up', 10000, 'phy', 1, NULL),
  ('FastEthernet2/22', 'up', 'up', 10000, 'phy', 1, NULL),
# Interfaces on daisy
  ('FastEthernet3/2', 'up', 'up', 10000, 'phy', 3, NULL),
  ('FastEthernet3/3', 'up', 'up', 10000, 'phy', 3, NULL),
  ('FastEthernet3/4', 'up', 'up', 10000, 'phy', 3, NULL),

  ('FastEthernet3/21', 'up', 'up', 10000, 'phy', 1, NULL),
  ('FastEthernet3/22', 'up', 'up', 10000, 'phy', 1, NULL),
# Interfaces on dandelion
  ('FastEthernet4/2', 'up', 'up', 10000, 'phy', 4, NULL),
  ('FastEthernet4/3', 'up', 'up', 10000, 'phy', 4, NULL),
  ('FastEthernet4/4', 'up', 'up', 10000, 'phy', 4, NULL),

  ('FastEthernet4/21', 'up', 'up', 10000, 'phy', 1, NULL),
  ('FastEthernet4/22', 'up', 'up', 10000, 'phy', 1, NULL);


## Insert L3Interface
INSERT INTO L3Interface (name, adminState, operationalState, ipAddress, ipMask,
  vlanID, ptpID, routerID)
VALUES
  ('FastEthernet4/1', 'up', 'up', '192.168.200.1', '255.255.255.0', NULL, 1, 1),
  ('FastEthernet4/1', 'up', 'up', '192.168.200.2', '255.255.255.0', NULL, 9, 2);

## Insert L2Interface
INSERT INTO L2Interface (name, adminState, operationalState, vlanID, ptpID, 
                         switchID)
VALUES
  ('FastEthernet1/2', 'up', 'up', 100, 1, 1),
  ('FastEthernet1/3', 'up', 'up', 61, 2, 1),
  ('FastEthernet1/4', 'up', 'up', 61, 3, 1),
  ('FastEthernet1/5', 'up', 'up', 61, 4, 1),

  ('FastEthernet1/21', 'up', 'up', 100, 5, 1),
  ('FastEthernet1/22', 'up', 'up', 101, 6, 1),
  
  ('FastEthernet2/2', 'up', 'up', 61, 7, 2),
  ('FastEthernet2/3', 'up', 'up', 61, 8, 2),

  ('FastEthernet2/21', 'up', 'up', 100, 9, 2),
  ('FastEthernet2/22', 'up', 'up', 101, 10, 2),

  ('FastEthernet3/2', 'up', 'up', 61, 11, 3),
  ('FastEthernet3/3', 'up', 'up', 61, 12, 3),
  ('FastEthernet3/4', 'up', 'up', 61, 13, 3),

  ('FastEthernet3/21', 'up', 'up', 100, 14, 3),
  ('FastEthernet3/22', 'up', 'up', 101, 15, 3),

  ('FastEthernet4/2', 'up', 'up', 61, 16, 4),
  ('FastEthernet4/3', 'up', 'up', 61, 17, 4),
  ('FastEthernet4/4', 'up', 'up', 100, 18, 4),

  ('FastEthernet4/21', 'up', 'up', 100, 19, 4),
  ('FastEthernet4/22', 'up', 'up', 101, 20, 4);

## Insert ServiceClass

INSERT INTO ServiceClass (name, dscp, dropPrecedence, utilLimit)
VALUES
   ('highPriority', 40, 0, 20),   
   ('highReliability', 32, 0, 30),   
   ('mmVideo', 24, 0, 20),
   ('mmVoice', 24, 0, 20),
   ('bestEffort', 0, 0, 10);   
 

INSERT INTO BWPerClass (ptpID) SELECT id from PTP;
UPDATE BWPerClass set svcClassID=1 where svcClassID=0;
INSERT INTO BWPerClass (ptpID) SELECT id from PTP;
UPDATE BWPerClass set svcClassID=2 where svcClassID=0;
INSERT INTO BWPerClass (ptpID) SELECT id from PTP;
UPDATE BWPerClass set svcClassID=3 where svcClassID=0;
INSERT INTO BWPerClass (ptpID) SELECT id from PTP;
UPDATE BWPerClass set svcClassID=4 where svcClassID=0;
UPDATE BWPerClass set inBound=0;
INSERT INTO BWPerClass (ptpID, svcClassID) SELECT ptpID,svcClassID from BWPerClass where inbound=0;
UPDATE BWPerClass set bwTotal=20000 where svcClassID=1;
UPDATE BWPerClass set bwTotal=30000 where svcClassID=2;
UPDATE BWPerClass set bwTotal=20000 where svcClassID=3;
UPDATE BWPerClass set bwTotal=20000 where svcClassID=4;

#Insert HostSubnet
INSERT INTO HostSubnet (hostSubnetAddr, hostSubnetMask, l3InterfaceID, poolName, l2InterfaceID, l3Gateway, hostName)
VALUES
    ('192.168.81.100', '255.255.255.0', NULL, 'Pool-1A', 19, '192.168.81.1', 'alpha'),
    ('192.168.81.101', '255.255.255.0', NULL, 'Pool-1B', 5, '192.168.81.1', 'bravo'),
    ('192.168.82.101', '255.255.255.0', NULL, 'Pool-1B', 20, '192.168.82.1', 'charlie'),
    ('192.168.82.101', '255.255.255.0', NULL, 'Pool-1B', 6, '192.168.82.1', 'delta'),
    ('192.168.81.102', '255.255.255.0', NULL, 'Pool-1B', 14, '192.168.81.1', 'echo'),
    ('192.168.82.102', '255.255.255.0', NULL, 'Pool-1B', 15, '192.168.82.1', 'foxtrot'),
    ('192.168.81.103', '255.255.255.0', NULL, 'Pool-1B', 9, '192.168.81.1', 'golf'),
    ('192.168.82.103', '255.255.255.0', NULL, 'Pool-1B', 10, '192.168.82.1', 'hotel'),
    ('192.168.81.0', '255.255.255.0', 1, 'Pool-1A', NULL, NULL, NULL),			
    ('192.168.82.0', '255.255.255.0', 2, 'Pool-1B', NULL, NULL, NULL),	
    ('192.168.83.0', '255.255.255.0', 3, 'Pool-2A', NULL, NULL, NULL);	

# update pool names

#UPDATE HostSubnet SET poolName = 'pool1' WHERE hostSubnetAddr = '192.168.82.0';
#UPDATE HostSubnet SET poolName = 'pool2' WHERE hostSubnetAddr = '192.168.83.0';
   
INSERT INTO Link (aPtpID, zPtpID)
VALUES
  (2, 8),
  (3, 12),
  (4, 16),
  (7, 13),
  (11, 17);

## Insert appropriate state into the activeVLAN table
INSERT INTO activeVLAN (vlanID, ptpID, switchID)
VALUES
  (100, 1, 1),
  (100, 2, 1),
  (100, 3, 1),
  (101, 3, 1),
  (100, 4, 1),
  (100, 5, 1),
  (101, 6, 1),

  (101, 7, 2),
  (100, 8, 2),
  (100, 9, 2),
  (101, 10, 2),

  (101, 11, 3),
  (100, 12, 3),
  (101, 12, 3),
  (101, 13, 3),
  (100, 14, 3),
  (101, 15, 3),

  (100, 16, 4),
  (101, 17, 4),
  (100, 18, 4),
  (100, 19, 4),
  (101, 20, 4);

