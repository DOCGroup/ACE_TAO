$Id$

This test is for ZIOP. Below the output when running this with -debug,
most important, check the hex dumps to see that the application data
is compressed

[build@balrog ZIOP]$ perl run_test.pl -debug
TAO (3199|46912520506160) Completed initializing the process-wide service context
TAO (3199|46912520506160) Default ORB services initialization begins
TAO (3199|46912520506160) - ORBInitializer_Registry::register_orb_initializer 0 @0x17b2fe60
TAO (3199|46912520506160) - ORBInitializer_Registry::register_orb_initializer 1 @0x17b2e660
TAO (3199|46912520506160) Default ORB services initialization completed
TAO (3199|46912520506160) We are the default ORB ...
TAO (3199|46912520506160) Initializing the orb-specific services
TAO (3199|46912520506160) Default_Resource_Factory - codeset manager=0x17b422a0
TAO (3199|46912520506160) - Codeset_Manager_i::init_ccs, Loaded Codeset translator <UTF8_Latin1_Factory>, ncs = 00010001 tcs = 05010001
TAO (3199|46912520506160) - UTF16_BOM_Translator: forceBE 0
TAO (3199|46912520506160) Loaded default protocol <IIOP_Factory>
TAO (3199|46912520506160) created new ORB <>
TAO (3199|46912520506160) - IIOP_Acceptor::open_i, listening on: <balrog.remedy.nl:44773>
TAO (3199|46912520506160) - IIOP_Acceptor::open_i, listening on: <192.168.248.1:44773>
TAO (3199|46912520506160) - IIOP_Acceptor::open_i, listening on: <192.168.238.1:44773>
TAO (3199|46912520506160) - ORB_Core::run, start [run]
TAO (3199|46912520506160) - ORB_Core::run, calling handle_events()
TAO (3201|46912517403856) Completed initializing the process-wide service context
TAO (3201|46912517403856) Default ORB services initialization begins
TAO (3201|46912517403856) - ORBInitializer_Registry::register_orb_initializer 0 @0x8d527f0
TAO (3201|46912517403856) - ORBInitializer_Registry::register_orb_initializer 1 @0x8d50ff0
TAO (3201|46912517403856) Default ORB services initialization completed
TAO (3201|46912517403856) We are the default ORB ...
TAO (3201|46912517403856) Initializing the orb-specific services
TAO (3201|46912517403856) Default_Resource_Factory - codeset manager=0x8d64c20
TAO (3201|46912517403856) - Codeset_Manager_i::init_ccs, Loaded Codeset translator <UTF8_Latin1_Factory>, ncs = 00010001 tcs = 05010001
TAO (3201|46912517403856) - UTF16_BOM_Translator: forceBE 0
TAO (3201|46912517403856) Loaded default protocol <IIOP_Factory>
TAO (3201|46912517403856) created new ORB <>
TAO (3201|46912517403856) - Transport_Cache_Manager::fill_set_i: current_size = 0, cache_maximum = 512
TAO (3201|46912517403856) - Transport_Cache_Manager::purge: Cache size after purging is [0]
TAO (3201|46912517403856) - IIOP_Connector::begin_connection, to <balrog.remedy.nl:44773> which should block
TAO (3201|46912517403856) - Transport_Connector::wait_for_connection_completion, going to wait for connection completion on transport[148297632]
TAO (3201|46912517403856) - Leader_Follower[148297632]::wait_for_event, (leader) enter reactor event loop
TAO (3199|46912520506160) - Transport_Cache_Manager::fill_set_i: current_size = 0, cache_maximum = 512
TAO (3201|46912517403856) - IIOP_Connection_Handler::open, The local addr is <127.0.0.1:53936>
TAO (3199|46912520506160) - Transport_Cache_Manager::purge: Cache size after purging is [0]
TAO (3201|46912517403856) - IIOP_Connection_Handler::open, IIOP connection to peer <127.0.0.1:44773> on 5
TAO (3201|46912517403856) - Leader_Follower[148297632]::wait_for_event, (leader) exit reactor event loop
TAO (3201|46912517403856) - Transport_Connector::wait_for_connection_completion, transport [5], wait done result = 1
TAO (3201|46912517403856) - IIOP_Connector::make_connection, new connected connection to <balrog.remedy.nl:44773> on Transport[5]
TAO (3201|46912517403856) - Transport_Cache_Manager::bind_i: Transport[5] @ hash:index{2130751206:0}
TAO (3201|46912517403856) - Transport_Cache_Manager::bind_i: Success Transport[5] @ hash:index{2130751206:0}. Cache size is [1]
TAO (3201|46912517403856) - Transport[5]::register_handler
TAO (3201|46912517403856) - Transport_Connector::connect, opening Transport[5] in TAO_CLIENT_ROLE
TAO (3199|46912520506160) - Concurrency_Strategy::activate_svc_handler, opened as TAO_SERVER_ROLE
TAO (3201|46912517403856) - Transport_Cache_Manager::is_entry_available:returns true state is [1]
TAO (3201|46912517403856) - Transport_Cache_Manager::find_i: Found available Transport[5] @hash:index {2130751206:0}
TAO (3201|46912517403856) - Transport_Connector::connect, got an existing connected Transport[5] in role TAO_CLIENT_ROLE
TAO (3201|46912517403856) - Codeset_Manager_i::set_tcs, setting char translator (00010001)
TAO (3199|46912520506160) - IIOP_Connection_Handler::open, The local addr is <127.0.0.1:44773>
TAO (3201|46912517403856) - Codeset_Manager_i::set_tcs, setting wchar translator (00010109)
TAO (3199|46912520506160) - IIOP_Connection_Handler::open, IIOP connection to peer <127.0.0.1:53936> on 8
TAO (3201|46912517403856) - Muxed_TMS[5]::request_id, <1>
TAO (3201|46912517403856) - Codeset_Manager_i::generate_service_context, using tcs_c = 00010001, tcs_w = 00010109
TAO (3199|46912520506160) - Transport_Cache_Manager::bind_i: Transport[8] @ hash:index{2130760369:0}
TAO (3199|46912520506160) - Transport_Cache_Manager::bind_i: Success Transport[8] @ hash:index{2130760369:0}. Cache size is [1]
TAO (3199|46912520506160) - Transport[8]::register_handler
TAO (3199|46912520506160) - ORB_Core::run, handle_events() returns 1
TAO (3199|46912520506160) - ORB_Core::run, calling handle_events()
TAO (3201|46912517403856) - GIOP_Message_Base::dump_msg, send GIOP v1.2 msg, 140 data bytes, my endian, is compressed, Type Request[1]
GIOP message - HEXDUMP 152 bytes
47 49 4f 50 01 02 05 00  8c 00 00 00 01 00 00 00   GIOP............
03 00 00 00 00 00 00 00  23 00 00 00 14 01 0f 00   ........#.......
4e 53 54 66 db 85 48 3c  ff 0a 00 01 00 00 00 01   NSTf..H<........
00 00 00 00 00 00 00 01  00 00 00 01 00 00 00 00   ................
0b 00 00 00 67 65 74 5f  73 74 72 69 6e 67 00 00   ....get_string..
01 00 00 00 01 00 00 00  0c 00 00 00 01 74 63 73   .............tcs
01 00 01 00 09 01 01 00  04 00 00 00 c3 00 00 00   ................
24 00 00 00 78 da db cf  c0 c0 10 92 91 59 ac 00   $...x........Y..
44 89 0a 25 a9 c5 25 0a  c5 25 45 99 79 e9 43 45   D..%..%..%E.y.CE
90 8b 01 00 a0 b6 45 d6                            ......E.
TAO (3199|46912520506160) - Connection_Handler[8]::handle_input, handle = 8/8
TAO (3201|46912517403856) - Transport[5]::cleanup_queue, byte_count = 152
TAO (3199|46912520506160) - Transport[8]::handle_input
TAO (3201|46912517403856) - Transport[5]::cleanup_queue, after transfer, bc = 0, all_sent = 1, ml = 0
TAO (3199|46912520506160) - Transport[8]::process_queue_head, 0 enqueued
TAO (3201|46912517403856) - Transport[5]::drain_queue_helper, byte_count = 152, head_is_empty = 1
TAO (3199|46912520506160) - Transport[8]::handle_input_parse_data, enter
TAO (3201|46912517403856) - Transport[5]::drain_queue_i, helper retval = 1
TAO (3201|46912517403856) - Transport[5]::make_idle
TAO (3201|46912517403856) - Leader_Follower[5]::wait_for_event, (leader) enter reactor event loop
TAO (3199|46912520506160) - Transport[8]::handle_input_parse_data, read 152 bytes
TAO (3199|46912520506160) - GIOP_Message_State::parse_message_header_i
TAO (3199|46912520506160) - GIOP_Message_State::get_version_info
TAO (3199|46912520506160) - GIOP_Message_State::get_byte_order_info
TAO (3199|46912520506160) - Transport[8]::process_parsed_messages, entering (missing data == 0)
TAO (3199|46912520506160) - GIOP_Message_Base::dump_msg, recv GIOP v1.2 msg, 140 data bytes, my endian, is compressed, Type Request[1]
GIOP message - HEXDUMP 152 bytes
47 49 4f 50 01 02 05 00  8c 00 00 00 01 00 00 00   GIOP............
03 00 00 00 00 00 00 00  23 00 00 00 14 01 0f 00   ........#.......
4e 53 54 66 db 85 48 3c  ff 0a 00 01 00 00 00 01   NSTf..H<........
00 00 00 00 00 00 00 01  00 00 00 01 00 00 00 00   ................
0b 00 00 00 67 65 74 5f  73 74 72 69 6e 67 00 00   ....get_string..
01 00 00 00 01 00 00 00  0c 00 00 00 01 74 63 73   .............tcs
01 00 01 00 09 01 01 00  04 00 00 00 c3 00 00 00   ................
24 00 00 00 78 da db cf  c0 c0 10 92 91 59 ac 00   $...x........Y..
44 89 0a 25 a9 c5 25 0a  c5 25 45 99 79 e9 43 45   D..%..%..%E.y.CE
90 8b 01 00 a0 b6 45 d6                            ......E.
TAO (3199|46912520506160) - Codeset_Manager_i::process_service_context, using tcsc = 00010001, tcsw = 00010109
Received <This is a test stringThis is a test stringThis is a test stringThis is a test stringThis is a test stringThis is a test stringThis is a test stringThis is a test stringThis is a test string
>
TAO (3199|46912520506160) - GIOP_Message_Base::dump_msg, send GIOP v1.2 msg, 52 data bytes, my endian, is compressed, Type Reply[1]
GIOP message - HEXDUMP 64 bytes
47 49 4f 50 01 02 05 01  34 00 00 00 01 00 00 00   GIOP....4.......
00 00 00 00 00 00 00 00  04 00 00 00 72 00 00 00   ............r...
1c 00 00 00 78 9c cb 63  60 60 f0 48 cd c9 c9 57   ....x..c``.H...W
28 c9 48 2d 4a 55 a4 35  9b 8b 01 00 c3 3c 27 2e   (.H-JU.5.....<'.
TAO (3199|46912520506160) - Transport[8]::cleanup_queue, byte_count = 64
TAO (3199|46912520506160) - Transport[8]::cleanup_queue, after transfer, bc = 0, all_sent = 1, ml = 0
TAO (3201|46912517403856) - Connection_Handler[5]::handle_input, handle = 5/5
TAO (3201|46912517403856) - Transport[5]::handle_input
TAO (3201|46912517403856) - Transport[5]::process_queue_head, 0 enqueued
TAO (3201|46912517403856) - Transport[5]::handle_input_parse_data, enter
TAO (3201|46912517403856) - Transport[5]::handle_input_parse_data, read 64 bytes
TAO (3201|46912517403856) - GIOP_Message_State::parse_message_header_i
TAO (3201|46912517403856) - GIOP_Message_State::get_version_info
TAO (3201|46912517403856) - GIOP_Message_State::get_byte_order_info
TAO (3201|46912517403856) - Transport[5]::process_parsed_messages, entering (missing data == 0)
TAO (3201|46912517403856) - GIOP_Message_Base::dump_msg, recv GIOP v1.2 msg, 52 data bytes, my endian, is compressed, Type Reply[1]
GIOP message - HEXDUMP 64 bytes
47 49 4f 50 01 02 05 01  34 00 00 00 01 00 00 00   GIOP....4.......
00 00 00 00 00 00 00 00  04 00 00 00 72 00 00 00   ............r...
1c 00 00 00 78 9c cb 63  60 60 f0 48 cd c9 c9 57   ....x..c``.H...W
28 c9 48 2d 4a 55 a4 35  9b 8b 01 00 c3 3c 27 2e   (.H-JU.5.....<'.
TAO (3201|46912517403856) - TAO_Muxed_TMS::dispatch_reply, id = 1
TAO (3201|46912517403856) - Connection_Handler[5]::handle_input, handle = 5/5, retval = 0
TAO (3201|46912517403856) - Leader_Follower[5]::wait_for_event, (leader) exit reactor event loop
(3201|46912517403856) - string returned <r>
TAO (3201|46912517403856) - Transport_Cache_Manager::is_entry_available:returns true state is [0]
TAO (3201|46912517403856) - Transport_Cache_Manager::find_i: Found available Transport[5] @hash:index {2130751206:0}
TAO (3201|46912517403856) - Transport_Connector::connect, got an existing connected Transport[5] in role TAO_CLIENT_ROLE
TAO (3201|46912517403856) - Muxed_TMS[5]::request_id, <2>
TAO (3201|46912517403856) - GIOP_Message_Base::dump_msg, send GIOP v1.2 msg, 72 data bytes, my endian, not compressed, Type Request[2]
GIOP message - HEXDUMP 84 bytes
47 49 4f 50 01 02 01 00  48 00 00 00 02 00 00 00   GIOP....H.......
00 00 00 00 00 00 00 00  23 00 00 00 14 01 0f 00   ........#.......
4e 53 54 66 db 85 48 3c  ff 0a 00 01 00 00 00 01   NSTf..H<........
00 00 00 00 00 00 00 01  00 00 00 01 00 00 00 00   ................
09 00 00 00 73 68 75 74  64 6f 77 6e 00 67 00 00   ....shutdown.g..
00 00 00 00                                        ....
TAO (3201|46912517403856) - Transport[5]::send_asynchronous_message_i, trying to send the message (ml = 84)
TAO (3201|46912517403856) - Transport[5]::cleanup_queue, byte_count = 84
TAO (3201|46912517403856) - Transport[5]::cleanup_queue, after transfer, bc = 0, all_sent = 1, ml = 0
TAO (3201|46912517403856) - Transport[5]::drain_queue_helper, byte_count = 84, head_is_empty = 1
TAO (3201|46912517403856) - Transport[5]::drain_queue_i, helper retval = 1
TAO (3201|46912517403856) - Transport[5]::make_idle
CORBA::ORB::destroy() called on ORB <>.
TAO (3199|46912520506160) - Transport[8]::drain_queue_helper, byte_count = 64, head_is_empty = 1
TAO (3199|46912520506160) - Transport[8]::drain_queue_i, helper retval = 1
TAO (3199|46912520506160) - Connection_Handler[8]::handle_input, handle = 8/8, retval = 0
Destroying ORB <>
TAO (3199|46912520506160) - ORB_Core::run, handle_events() returns 1
TAO (3201|46912517403856) - Connection_Handler[5]::close_connection_eh, purging entry from cache
TAO (3199|46912520506160) - ORB_Core::run, calling handle_events()
TAO (3201|46912517403856) - Transport[5]::cleanup_queue_i, cleaning up complete queue
TAO (3201|46912517403856) - Transport[5]::cleanup_queue_i, discarded 0 messages, 0 bytes.
TAO (3199|46912520506160) - Connection_Handler[8]::handle_input, handle = 8/8
TAO (3201|46912517403856) - Connection_Handler[5]::close_connection_eh, removing from the reactor
TAO (3199|46912520506160) - Transport[8]::handle_input
TAO (3201|46912517403856) - Connection_Handler[5]::close_connection_eh, cancel all timers
TAO (3201|46912517403856) - Transport[5]::cleanup_queue_i, cleaning up complete queue
TAO (3201|46912517403856) - Transport[5]::cleanup_queue_i, discarded 0 messages, 0 bytes.
TAO (3201|46912517403856) - Connection_Handler[5]::close_connection_eh
TAO (3201|46912517403856) - Transport[5]::cleanup_queue_i, cleaning up complete queue
TAO (3201|46912517403856) - Transport[5]::cleanup_queue_i, discarded 0 messages, 0 bytes.
TAO (3199|46912520506160) - Transport[8]::process_queue_head, 0 enqueued
TAO (3199|46912520506160) - Transport[8]::handle_input_parse_data, enter
TAO (3199|46912520506160) - Transport[8]::handle_input_parse_data, read 84 bytes
TAO (3199|46912520506160) - GIOP_Message_State::parse_message_header_i
TAO (3199|46912520506160) - GIOP_Message_State::get_version_info
TAO (3199|46912520506160) - GIOP_Message_State::get_byte_order_info
TAO (3199|46912520506160) - Transport[8]::process_parsed_messages, entering (missing data == 0)
TAO (3199|46912520506160) - GIOP_Message_Base::dump_msg, recv GIOP v1.2 msg, 72 data bytes, my endian, not compressed, Type Request[2]
GIOP message - HEXDUMP 84 bytes
47 49 4f 50 01 02 01 00  48 00 00 00 02 00 00 00   GIOP....H.......
00 00 00 00 00 00 00 00  23 00 00 00 14 01 0f 00   ........#.......
4e 53 54 66 db 85 48 3c  ff 0a 00 01 00 00 00 01   NSTf..H<........
00 00 00 00 00 00 00 01  00 00 00 01 00 00 00 00   ................
09 00 00 00 73 68 75 74  64 6f 77 6e 00 67 00 00   ....shutdown.g..
00 00 00 00                                        ....
TAO (3201|46912517403856) - ORBInitializer_Registry::fini clearing 1 @0x8d50ff0
TAO (3201|46912517403856) - ORBInitializer_Registry::fini clearing 0 @0x8d527f0
TAO (3199|46912520506160) - Connection_Handler[8]::handle_input, handle = 8/8, retval = 0
TAO (3199|46912520506160) - ORB_Core::run, handle_events() returns 1
TAO (3199|46912520506160) - ORB_Core::run, ends with result = 1
(3199|46912520506160) server - event loop finished
CORBA::ORB::destroy() called on ORB <>.
Destroying ORB <>
TAO (3199|46912520506160) - Connection_Handler[8]::close_connection_eh, purging entry from cache
TAO (3199|46912520506160) - Transport[8]::cleanup_queue_i, cleaning up complete queue
TAO (3199|46912520506160) - Transport[8]::cleanup_queue_i, discarded 0 messages, 0 bytes.
TAO (3199|46912520506160) - Connection_Handler[8]::close_connection_eh, removing from the reactor
TAO (3199|46912520506160) - Connection_Handler[8]::close_connection_eh, cancel all timers
TAO (3199|46912520506160) - Transport[8]::cleanup_queue_i, cleaning up complete queue
TAO (3199|46912520506160) - Transport[8]::cleanup_queue_i, discarded 0 messages, 0 bytes.
TAO (3199|46912520506160) - Connection_Handler[8]::close_connection_eh
TAO (3199|46912520506160) - Transport[8]::cleanup_queue_i, cleaning up complete queue
TAO (3199|46912520506160) - Transport[8]::cleanup_queue_i, discarded 0 messages, 0 bytes.
TAO (3199|46912520506160) - ORBInitializer_Registry::fini clearing 1 @0x17b2e660
TAO (3199|46912520506160) - ORBInitializer_Registry::fini clearing 0 @0x17b2fe60

