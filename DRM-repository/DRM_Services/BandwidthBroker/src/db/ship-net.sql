use ARMSBB;

## Insert routers
INSERT INTO Router (name, equipmentID, vendor, osVersion, adminState,
  operationalState, password, enablePW, managedIPAddr, isProviderEdge,
  lastACL)
VALUES
  ('access1', 'pc', 'Linux', '12.1', 'up', 'up', '', 'password',
   '192.168.1.1', 0, 0),
  ('access2', 'pc', 'Linux', '12.1', 'up', 'up', '', 'password',
   '192.168.2.2', 0, 0),
  ('access3', 'pc', 'Linux', '12.1', 'up', 'up', '', 'password',
   '192.168.3.3', 0, 0),
  ('access4', 'pc', 'Linux', '12.1', 'up', 'up', '', 'password',
   '192.168.4.4', 0, 0),
  ('access5', 'pc', 'Linux', '12.1', 'up', 'up', '', 'password',
   '192.168.5.5', 0, 0),
  ('access6', 'pc', 'Linux', '12.1', 'up', 'up', '', 'password',
   '192.168.6.6', 0, 0),
  ('access7', 'pc', 'Linux', '12.1', 'up', 'up', '', 'password',
   '192.168.7.7', 0, 0),
  ('access8', 'pc', 'Linux', '12.1', 'up', 'up', '', 'password',
   '192.168.8.8', 0, 0),
  ('access9', 'pc', 'Linux', '12.1', 'up', 'up', '', 'password',
   '192.168.9.9', 0, 0);

## Insert PTPs
INSERT INTO PTP (name, adminState, operationalState, rate, phyAddress,
  switchID, routerID)
VALUES
# Interfaces on access1
  ('ethA', 'up', 'up', 100000, 'phy', NULL, 1),
  ('ethB', 'up', 'up', 100000, 'phy', NULL, 1),
  ('ethC', 'up', 'up', 100000, 'phy', NULL, 1),
  ('ethD', 'up', 'up', 100000, 'phy', NULL, 1),

# Interfaces on access2
  ('ethA', 'up', 'up', 100000, 'phy', NULL, 2),
  ('ethB', 'up', 'up', 100000, 'phy', NULL, 2),
  ('ethC', 'up', 'up', 100000, 'phy', NULL, 2),
  ('ethD', 'up', 'up', 100000, 'phy', NULL, 2),

# Interfaces on access3
  ('ethA', 'up', 'up', 100000, 'phy', NULL, 3),
  ('ethB', 'up', 'up', 100000, 'phy', NULL, 3),
  ('ethC', 'up', 'up', 100000, 'phy', NULL, 3),
  ('ethD', 'up', 'up', 100000, 'phy', NULL, 3),

# Interfaces on access4
  ('ethA', 'up', 'up', 100000, 'phy', NULL, 4),
  ('ethB', 'up', 'up', 100000, 'phy', NULL, 4),
  ('ethC', 'up', 'up', 100000, 'phy', NULL, 4),
  ('ethD', 'up', 'up', 100000, 'phy', NULL, 4),

# Interfaces on access5
  ('ethA', 'up', 'up', 100000, 'phy', NULL, 5),
  ('ethB', 'up', 'up', 100000, 'phy', NULL, 5),
  ('ethC', 'up', 'up', 100000, 'phy', NULL, 5),
  ('ethD', 'up', 'up', 100000, 'phy', NULL, 5),

# Interfaces on access6
  ('ethA', 'up', 'up', 100000, 'phy', NULL, 6),
  ('ethB', 'up', 'up', 100000, 'phy', NULL, 6),
  ('ethC', 'up', 'up', 100000, 'phy', NULL, 6),
  ('ethD', 'up', 'up', 100000, 'phy', NULL, 6),

# Interfaces on access7
  ('ethA', 'up', 'up', 100000, 'phy', NULL, 7),
  ('ethB', 'up', 'up', 100000, 'phy', NULL, 7),
  ('ethC', 'up', 'up', 100000, 'phy', NULL, 7),
  ('ethD', 'up', 'up', 100000, 'phy', NULL, 7),

# Interfaces on access8
  ('ethA', 'up', 'up', 100000, 'phy', NULL, 8),
  ('ethB', 'up', 'up', 100000, 'phy', NULL, 8),
  ('ethC', 'up', 'up', 100000, 'phy', NULL, 8),
  ('ethD', 'up', 'up', 100000, 'phy', NULL, 8),

# Interfaces on access9
  ('ethA', 'up', 'up', 100000, 'phy', NULL, 9),
  ('ethB', 'up', 'up', 100000, 'phy', NULL, 9),
  ('ethC', 'up', 'up', 100000, 'phy', NULL, 9),
  ('ethD', 'up', 'up', 100000, 'phy', NULL, 9);

   
INSERT INTO Link (aPtpID, zPtpID)
VALUES
  (1, 5),
  (2, 9),
  (3, 13),
  (4, 25),

  (6, 10),
  (7, 33),
  (11, 17),
  (14, 18),

  (15, 21),
  (16, 26),
  (19, 22),
  (23, 29),

  (27, 30),
  (28, 34),
  (31, 35);

## Insert L3Interface
INSERT INTO L3Interface (name, adminState, operationalState, ipAddress, ipMask,
  vlanID, ptpID, linkID, routerID)
VALUES
  ('ethA', 'up', 'up', '192.168.12.1', '255.255.255.0', NULL, 1, 1, 1),
  ('ethB', 'up', 'up', '192.168.13.1', '255.255.255.0', NULL, 2, 2, 1),
  ('ethC', 'up', 'up', '192.168.14.1', '255.255.255.0', NULL, 3, 3, 1),
  ('ethD', 'up', 'up', '192.168.17.1', '255.255.255.0', NULL, 4, 4, 1),

  ('ethA', 'up', 'up', '192.168.12.2', '255.255.255.0', NULL, 5, 1, 2),
  ('ethB', 'up', 'up', '192.168.23.2', '255.255.255.0', NULL, 6, 5, 2),
  ('ethC', 'up', 'up', '192.168.29.2', '255.255.255.0', NULL, 7, 6, 2),
  ('ethD', 'up', 'up', '192.168.120.1', '255.255.255.0', NULL, 8, null, 2),

  ('ethA', 'up', 'up', '192.168.13.3', '255.255.255.0', NULL, 9, 2, 3),
  ('ethB', 'up', 'up', '192.168.23.3', '255.255.255.0', NULL, 10, 5, 3),
  ('ethC', 'up', 'up', '192.168.35.3', '255.255.255.0', NULL, 11, 7, 3),
  ('ethD', 'up', 'up', '192.168.130.1', '255.255.255.0', NULL, 12, null, 3),

  ('ethA', 'up', 'up', '192.168.14.4', '255.255.255.0', NULL, 13, 3, 4),
  ('ethB', 'up', 'up', '192.168.45.4', '255.255.255.0', NULL, 14, 8, 4),
  ('ethC', 'up', 'up', '192.168.46.4', '255.255.255.0', NULL, 15, 9, 4),
  ('ethD', 'up', 'up', '192.168.47.4', '255.255.255.0', NULL, 16, 10, 4),

  ('ethA', 'up', 'up', '192.168.35.5', '255.255.255.0', NULL, 17, 7, 5),
  ('ethB', 'up', 'up', '192.168.45.5', '255.255.255.0', NULL, 18, 8, 5),
  ('ethC', 'up', 'up', '192.168.56.5', '255.255.255.0', NULL, 19, 11, 5),
  ('ethD', 'up', 'up', '192.168.150.1', '255.255.255.0', NULL, 20, null, 5),

  ('ethA', 'up', 'up', '192.168.46.6', '255.255.255.0', NULL, 21, 9, 6),
  ('ethB', 'up', 'up', '192.168.56.6', '255.255.255.0', NULL, 22, 11, 6),
  ('ethC', 'up', 'up', '192.168.68.6', '255.255.255.0', NULL, 23, 12, 6),
  ('ethD', 'up', 'up', '192.168.160.1', '255.255.255.0', NULL, 24, null, 6),

  ('ethA', 'up', 'up', '192.168.17.7', '255.255.255.0', NULL, 25, 4, 7),
  ('ethB', 'up', 'up', '192.168.47.7', '255.255.255.0', NULL, 26, 10, 7),
  ('ethC', 'up', 'up', '192.168.78.7', '255.255.255.0', NULL, 27, 13, 7),
  ('ethD', 'up', 'up', '192.168.79.7', '255.255.255.0', NULL, 28, 14, 7),

  ('ethA', 'up', 'up', '192.168.68.8', '255.255.255.0', NULL, 29, 12, 8),
  ('ethB', 'up', 'up', '192.168.78.8', '255.255.255.0', NULL, 30, 13, 8),
  ('ethC', 'up', 'up', '192.168.89.8', '255.255.255.0', NULL, 31, 15, 8),
  ('ethD', 'up', 'up', '192.168.180.1', '255.255.255.0', NULL, 32, null, 8),

  ('ethA', 'up', 'up', '192.168.29.9', '255.255.255.0', NULL, 33, 6, 9),
  ('ethB', 'up', 'up', '192.168.79.9', '255.255.255.0', NULL, 34, 14, 9),
  ('ethC', 'up', 'up', '192.168.89.9', '255.255.255.0', NULL, 35, 15, 9),
  ('ethD', 'up', 'up', '192.168.190.1', '255.255.255.0', NULL, 36, null, 9);



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
    ('192.168.120.2', '255.255.255.255', 8, 'Pool-1A', NULL, '192.168.120.1', 'alpha'),
    ('192.168.120.3', '255.255.255.255', 8, 'Pool-1B', NULL, '192.168.120.1', 'bravo'),
    ('192.168.130.2', '255.255.255.255', 12, 'Pool-1B', NULL, '192.168.130.1', 'charlie'),
    ('192.168.130.3', '255.255.255.255', 12, 'Pool-1B', NULL, '192.168.130.1', 'delta'),
    ('192.168.150.2', '255.255.255.255', 20, 'Pool-1B', NULL, '192.168.150.1', 'echo'),
    ('192.168.150.3', '255.255.255.255', 20, 'Pool-1B', NULL, '192.168.150.1', 'foxtrot'),
    ('192.168.160.2', '255.255.255.255', 24, 'Pool-1B', NULL, '192.168.160.1', 'golf'),
    ('192.168.160.3', '255.255.255.255', 24, 'Pool-1B', NULL, '192.168.160.1', 'hotel'),
    ('192.168.180.2', '255.255.255.255', 32, 'Pool-1B', NULL, '192.168.180.1', 'india'),
    ('192.168.180.3', '255.255.255.255', 32, 'Pool-1B', NULL, '192.168.180.1', 'juliet'),
    ('192.168.190.2', '255.255.255.255', 36, 'Pool-1B', NULL, '192.168.190.1', 'kilo'),
    ('192.168.190.2', '255.255.255.255', 36, 'Pool-1B', NULL, '192.168.190.1', 'lima');



#Insert L3Path
INSERT INTO L3Path (ingressID, egressID)
VALUES
	(1,2), (1,3), (1,4), (1,5),(1,6), (1,7), (1,8), (1,9),
	(2,1), (2,3), (2,4), (2,5),(2,6), (2,7), (2,8), (2,9),
	(3,1), (3,2), (3,4), (3,5),(3,6), (3,7), (3,8), (3,9),
	(4,1), (4,2), (4,3), (4,5),(4,6), (4,7), (4,8), (4,9),
	(5,1), (5,2), (5,3), (5,4),(5,6), (5,7), (5,8), (5,9),
	(6,1), (6,2), (6,3), (6,4),(6,5), (6,7), (6,8), (6,9),
	(7,1), (7,2), (7,3), (7,4),(7,5), (7,6), (7,8), (7,9),
	(8,1), (8,2), (8,3), (8,4),(8,5), (8,6), (8,7), (8,9),
	(9,1), (9,2), (9,3), (9,4),(9,5), (9,6), (9,7), (9,8);



# update pool names

#UPDATE HostSubnet SET poolName = 'pool1' WHERE hostSubnetAddr = '192.168.82.0';
#UPDATE HostSubnet SET poolName = 'pool2' WHERE hostSubnetAddr = '192.168.83.0';
