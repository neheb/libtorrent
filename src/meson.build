incdirs = include_directories('..', '.', 'torrent')

libtorrent_files = files(
  'data/chunk.cc',
  'data/chunk_list.cc',
  'data/chunk_part.cc',
  'data/hash_check_queue.cc',
  'data/hash_chunk.cc',
  'data/hash_queue.cc',
  'data/hash_queue_node.cc',
  'data/hash_torrent.cc',
  'data/memory_chunk.cc',
  'data/socket_file.cc',
  'data/thread_disk.cc',
  'dht/dht_bucket.cc',
  'dht/dht_node.cc',
  'dht/dht_router.cc',
  'dht/dht_server.cc',
  'dht/dht_tracker.cc',
  'dht/dht_transaction.cc',
  'download/available_list.cc',
  'download/chunk_selector.cc',
  'download/chunk_statistics.cc',
  'download/delegator.cc',
  'download/download_constructor.cc',
  'download/download_main.cc',
  'download/download_wrapper.cc',
  'net/address_list.cc',
  'net/listen.cc',
  'net/socket_base.cc',
  'net/socket_datagram.cc',
  'net/socket_fd.cc',
  'net/socket_listen.cc',
  'net/socket_set.cc',
  'net/socket_stream.cc',
  'net/thread_net.cc',
  'net/throttle_internal.cc',
  'net/throttle_list.cc',
  'net/udns_resolver.cc',
  'net/udns/udns_dn.c',
  'net/udns/udns_init.c',
  'net/udns/udns_jran.c',
  'net/udns/udns_parse.c',
  'net/udns/udns_resolver.c',
  'net/udns/udns_rr_a.c',
  'net/udns/udns_XtoX.c',
  'protocol/extensions.cc',
  'protocol/handshake.cc',
  'protocol/handshake_encryption.cc',
  'protocol/handshake_manager.cc',
  'protocol/initial_seed.cc',
  'protocol/peer_connection_base.cc',
  'protocol/peer_connection_leech.cc',
  'protocol/peer_connection_metadata.cc',
  'protocol/peer_factory.cc',
  'protocol/request_list.cc',
  'tracker/thread_tracker.cc',
  'tracker/tracker_dht.cc',
  'tracker/tracker_http.cc',
  'tracker/tracker_udp.cc',
  'tracker/tracker_worker.cc',
  'utils/diffie_hellman.cc',
  'utils/instrumentation.cc',
)

subdir('torrent')

libtorrent_other = static_library(
  'torrent_other',
  libtorrent_files,
  link_with: libtorrent_torrent,
  dependencies: [thread_dep, zlib_dep, ssl_dep],
  include_directories: incdirs,
)

depinc = include_directories('.', 'torrent')
libtorrent_other_dep = declare_dependency(
  include_directories: depinc,
  link_with: [libtorrent_torrent, libtorrent_other],
)

libtorrent = library(
  'torrent',
  'globals.cc',
  'manager.cc',
  'thread_main.cc',
  dependencies: libtorrent_other_dep,
  gnu_symbol_visibility: 'hidden',
  include_directories: incdirs,
  install: true,
)

libtorrent_dep = declare_dependency(
  include_directories: depinc,
  link_with: libtorrent,
)
