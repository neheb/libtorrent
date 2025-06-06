#ifndef LIBTORRENT_NET_THREAD_NET_H
#define LIBTORRENT_NET_THREAD_NET_H

#include "torrent/common.h"
#include "torrent/utils/thread.h"

namespace torrent {

class UdnsResolver;

class LIBTORRENT_EXPORT ThreadNet : public utils::Thread {
public:
  ~ThreadNet() override;

  static void         create_thread();
  static ThreadNet*   thread_net();

  const char*         name() const override { return "rtorrent net"; }

  void                init_thread() override;

protected:
  friend class torrent::net::Resolver;

  ThreadNet() = default;

  void                      call_events() override;
  std::chrono::microseconds next_timeout() override;

  UdnsResolver*             udns() const    { return m_udns.get(); }

private:
  static ThreadNet*             m_thread_net;

  std::unique_ptr<UdnsResolver> m_udns;
};

inline ThreadNet* thread_net() {
  return ThreadNet::thread_net();
}

} // namespace torrent

#endif // LIBTORRENT_NET_THREAD_NET_H
