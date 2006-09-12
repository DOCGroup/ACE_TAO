-- $Id$

CREATE DATABASE IF NOT EXISTS imp;

USE imp;


DROP TABLE IF EXISTS client,cli_request,cli_template,snmp_request,snmp_request_oid,dlj_request, packet_request,bw_request;


CREATE TABLE IF NOT EXISTS client (
 foreign_tag		VARCHAR(64),
 tag			VARCHAR(64),
 client_url		VARCHAR(128)			-- a URL
);

CREATE TABLE IF NOT EXISTS cli_request (
 tag			VARCHAR(64),
 imp			VARCHAR(64),
 period			BIGINT,				-- units?
 target			VARCHAR(64),			-- system monitored
 start_time		BIGINT,				-- millis since 1/1/1970
 end_time		BIGINT,				-- ditto

 access_method		VARCHAR(16),
 username		VARCHAR(32),
 password		VARCHAR(32),
 enable_password	VARCHAR(32),
 PRIMARY KEY (tag)
);

CREATE TABLE IF NOT EXISTS cli_template (
 tag			VARCHAR(64),
 name			VARCHAR(128)
);

CREATE TABLE IF NOT EXISTS snmp_request (
 tag			VARCHAR(64)	PRIMARY KEY,
 imp			VARCHAR(64),
 period			BIGINT,				-- units?
 target			VARCHAR(64),			-- system monitored
 start_time		BIGINT,				-- millis since 1/1/1970
 end_time		BIGINT,				-- ditto
 read_community		VARCHAR(32),
 timeout		INT,
 retries		INT

);

CREATE TABLE IF NOT EXISTS snmp_request_oid (
 tag			VARCHAR(64),
 oid			VARCHAR(128)
);

CREATE TABLE IF NOT EXISTS dlj_request (
 tag			VARCHAR(64)	PRIMARY KEY,
 imp			VARCHAR(64),
 period			BIGINT,				-- units?
 target			VARCHAR(64),			-- system monitored
 start_time		BIGINT,				-- millis since 1/1/1970
 end_time		BIGINT,				-- ditto
 pspec			VARCHAR(255),
 emitter_class		VARCHAR(128),			-- emitter class
 collector_class	VARCHAR(128),			-- collector class
 gap			INT,				-- inter-packet gap
 size			INT,				-- packet size
 tos			INT,				-- TOS bits
 num_packets		INT				-- number of packets

);

CREATE TABLE IF NOT EXISTS packet_request (
 tag			VARCHAR(64)	PRIMARY KEY,
 imp			VARCHAR(64),
 period			BIGINT,				-- units?
 target			VARCHAR(64),			-- system monitored
 start_time		BIGINT,				-- millis since 1/1/1970
 end_time		BIGINT,				-- ditto
 pspec			VARCHAR(255),
 emitter_class		VARCHAR(128),			-- emitter class
 collector_class	VARCHAR(128)			-- collector class

);

CREATE TABLE IF NOT EXISTS bw_request (
 tag			VARCHAR(64)	PRIMARY KEY,
 imp			VARCHAR(64),
 period			BIGINT,				-- units?
 target			VARCHAR(64),			-- system monitored
 start_time		BIGINT,				-- millis since 1/1/1970
 end_time		BIGINT,				-- ditto
 pspec			VARCHAR(255),
 emitter_class		VARCHAR(128),			-- emitter class
 collector_class	VARCHAR(128)			-- collector class

);
