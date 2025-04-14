project(
  'libtorrent',
  'c',
  'cpp',
  version: '0.15.2',
  default_options: ['cpp_std=c++20', 'warning_level=1'],
)

if get_option('b_ndebug') not in ['true', 'if-release']
  add_project_arguments('-DDEBUG', language: 'cpp')
endif

cdata = configuration_data()

cdata.set_quoted('VERSION', meson.project_version())
cdata.set_quoted('PEER_NAME', '-lt0F02-')
cdata.set_quoted('PEER_VERSION', 'lt\\x0F\\x02')

cc = meson.get_compiler('cpp')
if cc.has_function_attribute('visibility:default')
  cdata.set('SUPPORT_ATTRIBUTE_VISIBILITY', 1)
endif

if host_machine.endian() == 'little'
  cdata.set('IS_LITTLE_ENDIAN', 1)
else
  cdata.set('IS_BIG_ENDIAN', 1)
endif

cdata.set('HAVE_CONFIG_H', 1)
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

foreach f : ['backtrace', 'fallocate']
  if cc.has_function(f)
    cdata.set('HAVE_@0@'.format(f.to_upper()), 1)
  endif
endforeach

foreach f, d : {'madvise': [], 'mincore': [], 'posix_fadvise': []}
  if cc.has_function(f, dependencies: d)
    cdata.set('USE_@0@'.format(f.to_upper()), 1)
  endif
endforeach

if cc.has_function('inotify_init')
  cdata.set('HAVE_INOTIFY', 1)
endif

if cc.has_function('epoll_create')
  cdata.set('USE_EPOLL', 1)
endif

if cc.has_function('__builtin_popcount')
  cdata.set('USE_BUILTIN_POPCOUNT', 1)
endif

if cc.compiles('#include <sys/mman.h>\n int main(){mincore(nullptr,0,(unsigned char*)0);}')
  cdata.set('USE_MINCORE_UNSIGNED', 1)
else
  cdata.set('USE_MINCORE_UNSIGNED', 0)
endif

if cc.compiles('#include <pthread.h>\nint main(){pthread_setname_np("foo");}')
  cdata.set('HAS_PTHREAD_SETNAME_NP_DARWIN', 1)
elif cc.compiles('#include <pthread.h>\nint main(){pthread_t t;pthread_setname_np(t,"foo");}')
  cdata.set('HAS_PTHREAD_SETNAME_NP_GENERIC', 1)
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

if cc.sizeof('void*') == 8
  cdata.set('DEFAULT_ADDRESS_SPACE_SIZE', 4096)
else
  cdata.set('DEFAULT_ADDRESS_SPACE_SIZE', 1024)
endif

cdata.set('LT_INSTRUMENTATION', 1)

kqueue_dep = dependency('libkqueue', required: false)
if cc.has_function('kqueue') or kqueue_dep.found()
  cdata.set('USE_KQUEUE', 1)
endif

ssl_dep = dependency('libcrypto')
thread_dep = dependency('threads')
zlib_dep = dependency('zlib')

cfile = configure_file(
  configuration: cdata,
  output: 'config.h',
)

subdir('src')
subdir('test')
