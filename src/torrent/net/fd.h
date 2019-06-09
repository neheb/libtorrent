#ifndef LIBTORRENT_NET_FD_H
#define LIBTORRENT_NET_FD_H

#include <string>
#include <sys/socket.h>
#include <torrent/common.h>

namespace torrent {

enum fd_flags : int {
  fd_flag_stream = 0x1,
  fd_flag_nonblock = 0x10,
  fd_flag_reuse_address = 0x20,
  fd_flag_v4only = 0x40,
  fd_flag_v6only = 0x80
};

int  fd_open(fd_flags flags) LIBTORRENT_EXPORT;
void fd_close(int fd) LIBTORRENT_EXPORT;

bool fd_set_nonblock(int fd) LIBTORRENT_EXPORT;
bool fd_set_reuse_address(int fd, bool state) LIBTORRENT_EXPORT;
bool fd_set_v6only(int fd, bool state) LIBTORRENT_EXPORT;

bool fd_connect(int fd, const sockaddr* sa) LIBTORRENT_EXPORT;
bool fd_bind(int fd, const sockaddr* sa) LIBTORRENT_EXPORT;
bool fd_listen(int fd, int backlog) LIBTORRENT_EXPORT;

extern "C" {
  [[gnu::weak]] int fd__bind(int socket, const sockaddr *address, socklen_t address_len) LIBTORRENT_EXPORT;
  [[gnu::weak]] int fd__close(int fildes) LIBTORRENT_EXPORT;
  [[gnu::weak]] int fd__connect(int socket, const sockaddr *address, socklen_t address_len) LIBTORRENT_EXPORT;
  [[gnu::weak]] int fd__fcntl_int(int fildes, int cmd, int arg) LIBTORRENT_EXPORT;
  [[gnu::weak]] int fd__listen(int socket, int backlog) LIBTORRENT_EXPORT;
  [[gnu::weak]] int fd__setsockopt_int(int socket, int level, int option_name, int option_value) LIBTORRENT_EXPORT;
  [[gnu::weak]] int fd__socket(int domain, int type, int protocol) LIBTORRENT_EXPORT;
}

inline fd_flags
operator |(fd_flags lhs, fd_flags rhs) {
  return static_cast<fd_flags>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

}

#endif
