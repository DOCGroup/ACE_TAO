#$Id$
use ARMSBB;

#update HostSubnetTable with the measurement hosts
use ARMSBB;
INSERT INTO HostSubnet (hostSubnetAddr, hostSubnetMask, l3InterfaceID, poolName, l2InterfaceID, l3Gateway, hostName)
VALUES
    ('192.168.120.0', '255.255.255.0', 1, 'Pool-1C', 1, '192.168.120.1', 'rondomon08'),
    ('192.168.130.0', '255.255.255.0', 2, 'Pool-1D', 2, '192.168.130.1', 'rondomon09'),
    ('192.168.120.0', '255.255.255.0', 3, 'Pool-1C', 1, '192.168.120.1', 'alpha'),
    ('192.168.130.0', '255.255.255.0', 2, 'Pool-1D', 2, '192.168.130.1', 'beta');

#update FlowRecord to test measurement using flow token
INSERT INTO FlowRecord (status, purgeStatus, srcIP, srcMask, srcPort, srcPortHigh, destIP, destMask, destPort, destPortHigh, protocol, dscp, bandwidthMin,extrabudget,bwAllocated,acl,svcClassID, inInterfaceID, outInterfaceID,priority,l2Endpoints,reference)
VALUES
('commited',0,'192.168.120.104', '255.255.255.0',20,20,'192.168.130.104','255.255.255.0',-1, -1, 'tcp',1, 2000, 10, 2000, 0, 2, 1, 2, 1,0,NULL);
