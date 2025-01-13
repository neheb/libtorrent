project(
  'libtorrent',
  'c',
  'cpp',
  version: '0.14.0',
  default_options: ['cpp_std=c++17', 'warning_level=1'],
)

cc = meson.get_compiler('cpp')

cdata = configuration_data()

cdata.set_quoted('VERSION', meson.project_version())
cdata.set_quoted('PEER_NAME', '-lt0F01-')
cdata.set_quoted('PEER_VERSION', 'lt\x0F\x01')

add_project_arguments('-DHAVE_CONFIG_H', language: 'cpp')
add_project_arguments('-DSUPPORT_ATTRIBUTE_VISIBILITY', language: 'cpp')

if host_machine.endian() == 'little'
  add_project_arguments('-DIS_LITTLE_ENDIAN', language: 'cpp')
endif

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

if cc.has_function('epoll_create')
  cdata.set('USE_EPOLL', 1)
endif

if cc.has_function('fallocate')
  cdata.set('HAVE_FALLOCATE', 1)
endif

if cc.has_function('__builtin_popcount')
  cdata.set('USE_BUILTIN_POPCOUNT', 1)
endif

if cc.has_function('madvise')
  cdata.set('USE_MADVISE', 1)
endif

cdata.set('USE_MINCORE', 1)
if cc.compiles('#include <sys/mman.h>\n int main(){ mincore(nullptr, 0, (unsigned char*)0); }')
  cdata.set('USE_MINCORE_UNSIGNED', 1)
else
  cdata.set('USE_MINCORE_UNSIGNED', 0)
endif

#cdata.set('USE_ALIGNED', 1)

#if cc.has_header_symbol('new', 'std::hardware_constructive_interference_size')
if false
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

if cc.sizeof('void*') == 8
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

kqueue_dep = dependency('libkqueue', required: false)
if cc.has_function('kqueue') or kqueue_dep.found()
  cdata.set('USE_KQUEUE', 1)
endif

thread_dep = dependency('threads')
zlib_dep = dependency('zlib')

cfile = configure_file(
  configuration: cdata,
  output: 'config.h',
)

subdir('src')
subdir('test')
