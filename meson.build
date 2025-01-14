project(
  'libtorrent',
  'cpp',
  version: '0.14.0',
  default_options: ['cpp_std=c++17', 'warning_level=1'],
)

cc = meson.get_compiler('cpp')

cdata = configuration_data()

cdata.set_quoted('VERSION', meson.project_version())
cdata.set_quoted('PEER_NAME', '-lt0E00-')
cdata.set_quoted('PEER_VERSION', 'lt\x0E\x00')

headers = [
  'sys/vfs.h',
  'sys/statvfs.h',
  'sys/statfs.h',
  'sys/param.h',
  'sys/mount.h',
]

foreach h : headers
  if cc.has_header(h)
    cdata.set('HAVE_@0@'.format(h.underscorify().to_upper()), 1)
  endif
endforeach

if cc.has_header('backtrace.h')
  cdata.set('HAVE_BACKTRACE', 1)
endif

if cc.has_header('sys/inotify.h')
  cdata.set('HAVE_INOTIFY', 1)
endif

if cc.has_function('fallocate')
  cdata.set('HAVE_FALLOCATE', 1)
endif

if cc.has_function('__builtin_popcount')
  cdata.set('USE_BUILTIN_POPCOUNT', 1)
endif

if cc.has_header_symbol('new', 'std::hardware_constructive_interference_size')
  cdata.set('LT_SMP_CACHE_BYTES', 'std::hardware_constructive_interference_size')
else
  cdata.set('LT_SMP_CACHE_BYTES', '128')
endif

cdata.set('lt_cacheline_aligned', 'alignas(LT_SMP_CACHE_BYTES)')

cdata.set('FS_STAT_FD', 'fstatvfs(fd, &m_stat) == 0')
cdata.set('FS_STAT_FN', 'statvfs(fn, &m_stat) == 0')
cdata.set('FS_STAT_STRUCT', 'struct statvfs')
cdata.set('FS_STAT_SIZE_TYPE', 'unsigned long')
cdata.set('FS_STAT_COUNT_TYPE', 'fsblkcnt_t')
cdata.set('FS_STAT_BLOCK_SIZE', '(m_stat.f_frsize)')

if cc.sizeof('long') == 8
  cdata.set('DEFAULT_ADDRESS_SPACE_SIZE', 4096)
else
  cdata.set('DEFAULT_ADDRESS_SPACE_SIZE', 1024)
endif

ssl_dep = dependency('libcrypto', required: get_option('openssl'))
if ssl_dep.found()
  cdata.set('USE_OPENSSL_SHA', 1)
else
  ssl_dep = [dependency('nss'), dependency('cyrus')]
  cdata.set('USE_CYRUS_RC4', 1)
  cdata.set('USE_NSS_SHA', 1)
endif

thread_dep = dependency('threads')
zlib_dep = dependency('zlib')

cfile = configure_file(
  configuration: cdata,
  output: 'config.h',
)

incdirs = include_directories('.', 'src', 'src/torrent')

libtorrent = library(
  'torrent',
  'src/data/chunk.cc',
  'src/data/chunk_list.cc',
  'src/data/chunk_part.cc',
  'src/data/hash_check_queue.cc',
  'src/data/hash_chunk.cc',
  'src/data/hash_queue.cc',
  'src/data/hash_queue_node.cc',
  'src/data/hash_torrent.cc',
  'src/data/memory_chunk.cc',
  'src/data/socket_file.cc',
  'src/dht/dht_bucket.cc',
  'src/dht/dht_node.cc',
  'src/dht/dht_router.cc',
  'src/dht/dht_server.cc',
  'src/dht/dht_tracker.cc',
  'src/dht/dht_transaction.cc',
  'src/download/available_list.cc',
  'src/download/chunk_selector.cc',
  'src/download/chunk_statistics.cc',
  'src/download/delegator.cc',
  'src/download/download_constructor.cc',
  'src/download/download_main.cc',
  'src/download/download_wrapper.cc',
  'src/globals.cc',
  'src/net/address_list.cc',
  'src/net/listen.cc',
  'src/net/socket_base.cc',
  'src/net/socket_datagram.cc',
  'src/net/socket_fd.cc',
  'src/net/socket_listen.cc',
  'src/net/socket_set.cc',
  'src/net/socket_stream.cc',
  'src/net/throttle_internal.cc',
  'src/net/throttle_list.cc',
  'src/protocol/extensions.cc',
  'src/protocol/handshake.cc',
  'src/protocol/handshake_encryption.cc',
  'src/protocol/handshake_manager.cc',
  'src/protocol/initial_seed.cc',
  'src/protocol/peer_connection_base.cc',
  'src/protocol/peer_connection_leech.cc',
  'src/protocol/peer_connection_metadata.cc',
  'src/protocol/peer_factory.cc',
  'src/protocol/request_list.cc',
  'src/thread_disk.cc',
  'src/thread_main.cc',
  'src/torrent/bitfield.cc',
  'src/torrent/chunk_manager.cc',
  'src/torrent/connection_manager.cc',
  'src/torrent/data/block.cc',
  'src/torrent/data/block_list.cc',
  'src/torrent/data/chunk_utils.cc',
  'src/torrent/data/download_data.cc',
  'src/torrent/data/file.cc',
  'src/torrent/data/file_list.cc',
  'src/torrent/data/file_list_iterator.cc',
  'src/torrent/data/file_manager.cc',
  'src/torrent/data/file_utils.cc',
  'src/torrent/data/transfer_list.cc',
  'src/torrent/dht_manager.cc',
  'src/torrent/download.cc',
  'src/torrent/download/choke_group.cc',
  'src/torrent/download/choke_queue.cc',
  'src/torrent/download/download_manager.cc',
  'src/torrent/download/resource_manager.cc',
  'src/torrent/error.cc',
  'src/torrent/event.cc',
  'src/torrent/exceptions.cc',
  'src/torrent/hash_string.cc',
  'src/torrent/http.cc',
  'src/torrent/net/address_info.cc',
  'src/torrent/net/fd.cc',
  'src/torrent/net/socket_address.cc',
  'src/torrent/net/socket_address_key.cc',
  'src/torrent/net/socket_event.cc',
  'src/torrent/net/utils.cc',
  'src/torrent/object.cc',
  'src/torrent/object_static_map.cc',
  'src/torrent/object_stream.cc',
  'src/torrent/path.cc',
  'src/torrent/peer/client_info.cc',
  'src/torrent/peer/client_list.cc',
  'src/torrent/peer/connection_list.cc',
  'src/torrent/peer/peer.cc',
  'src/torrent/peer/peer_info.cc',
  'src/torrent/peer/peer_list.cc',
  'src/torrent/poll_epoll.cc',
  'src/torrent/poll_kqueue.cc',
  'src/torrent/poll_select.cc',
  'src/torrent/rate.cc',
  'src/torrent/throttle.cc',
  'src/torrent/torrent.cc',
  'src/torrent/tracker.cc',
  'src/torrent/tracker_controller.cc',
  'src/torrent/tracker_list.cc',
  'src/torrent/utils/directory_events.cc',
  'src/torrent/utils/log.cc',
  'src/torrent/utils/log_buffer.cc',
  'src/torrent/utils/option_strings.cc',
  'src/torrent/utils/random.cc',
  'src/torrent/utils/resume.cc',
  'src/torrent/utils/signal_bitfield.cc',
  'src/torrent/utils/thread_base.cc',
  'src/torrent/utils/thread_interrupt.cc',
  'src/torrent/utils/uri_parser.cc',
  'src/tracker/tracker_dht.cc',
  'src/tracker/tracker_http.cc',
  'src/tracker/tracker_udp.cc',
  'src/utils/diffie_hellman.cc',
  'src/utils/instrumentation.cc',
  'src/utils/sha_fast.cc',
  'src/manager.cc',
  dependencies: [ssl_dep, [thread_dep], zlib_dep],
  include_directories: incdirs,
  install: true,
)

depinc = include_directories('src', 'src/torrent')
libtorrent_dep = declare_dependency(
  include_directories: depinc,
  link_with: libtorrent,
)

subdir('test')
