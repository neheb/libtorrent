// libTorrent - BitTorrent library
// Copyright (C) 2005-2011, Jari Sundell
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// In addition, as a special exception, the copyright holders give
// permission to link the code of portions of this program with the
// OpenSSL library under certain conditions as described in each
// individual source file, and distribute linked combinations
// including the two.
//
// You must obey the GNU General Public License in all respects for
// all of the code used other than OpenSSL.  If you modify file(s)
// with this exception, you may extend this exception to your version
// of the file(s), but you are not obligated to do so.  If you do not
// wish to do so, delete this exception statement from your version.
// If you delete this exception statement from all source files in the
// program, then also delete it here.
//
// Contact:  Jari Sundell <jaris@ifi.uio.no>
//
//           Skomakerveien 33
//           3185 Skoppum, NORWAY

#ifndef LIBTORRENT_PROTOCOL_EXTENSIONS_H
#define LIBTORRENT_PROTOCOL_EXTENSIONS_H

#include <string>
#include <vector>

#include "torrent/exceptions.h"
#include "torrent/object.h"
#include "torrent/object_static_map.h"

#include "torrent/download_info.h"
#include "net/address_list.h"
#include "net/data_buffer.h"

// Not really important, so no need to make this a configure check.
#ifdef __GNUC__
#define ATTRIBUTE_PRINTF(num) __attribute__ ((format (printf, num, num+1)))
#else
#define ATTRIBUTE_PRINTF(num)
#endif

namespace torrent {

class ProtocolExtension {
public:
  typedef enum {
    HANDSHAKE = 0,
    UT_PEX,
    UT_METADATA,

    FIRST_INVALID,    // first invalid message ID

    SKIP_EXTENSION,
  } MessageType;

  typedef std::vector<SocketAddressCompact> PEXList;

  static const int    flag_default           = 1<<0;
  static const int    flag_initial_handshake = 1<<1;
  static const int    flag_initial_pex       = 1<<2;
  static const int    flag_received_ext      = 1<<3;

  // The base bit to shift by MessageType to check if the extension is
  // enabled locally or supported by the peer.
  static const int    flag_local_enabled_base    = 1<<8;
  static const int    flag_remote_supported_base = 1<<16;

  // Number of extensions we support, not counting handshake.
  static const int    extension_count = FIRST_INVALID - HANDSHAKE - 1;

  // Fixed size of a metadata piece (16 KB).
  static const size_t metadata_piece_shift = 14;
  static const size_t metadata_piece_size  = 1 << metadata_piece_shift;

  ProtocolExtension();
  ~ProtocolExtension() { delete [] m_read; }

  void                cleanup();

  // Create default extension object, with all extensions disabled.
  // Useful for eliminating checks whether peer supports extensions at all.
  static ProtocolExtension make_default();

  void                set_info(PeerInfo* peerInfo, DownloadMain* download) { m_peerInfo = peerInfo; m_download = download; }
  void                set_connection(PeerConnectionBase* c)                { m_connection = c; }

  DataBuffer          generate_handshake_message();
  static DataBuffer   generate_toggle_message(MessageType t, bool on);
  static DataBuffer   generate_ut_pex_message(const PEXList& added, const PEXList& removed);

  // Return peer's extension ID for the given extension type, or 0 if
  // disabled by peer.
  uint8_t             id(int t) const;

  bool                is_local_enabled(int t) const    { return m_flags & flag_local_enabled_base << t; }
  bool                is_remote_supported(int t) const { return m_flags & flag_remote_supported_base << t; }

  void                set_local_enabled(int t);
  void                unset_local_enabled(int t);
  void                set_remote_supported(int t)      { m_flags |= flag_remote_supported_base << t; }

  // General information about peer from extension handshake.
  uint32_t            max_queue_length() const         { return m_maxQueueLength; }

  // Handle reading extension data from peer.
  void                read_start(int type, uint32_t length, bool skip);
  bool                read_done();

  char*               read_position()                  { return m_readPos; }
  bool                read_move(uint32_t v)            { m_readPos += v; return (m_readLeft -= v) == 0; }
  uint32_t            read_need() const                { return m_readLeft; }

  bool                is_complete() const              { return m_readLeft == 0; }
  bool                is_invalid() const               { return m_readType == FIRST_INVALID; }

  bool                is_default() const               { return m_flags & flag_default; }

  // Initial PEX message after peer enables PEX needs to send full list
  // of peers instead of the delta list, so keep track of that.
  bool                is_initial_handshake() const     { return m_flags & flag_initial_handshake; }
  bool                is_initial_pex() const           { return m_flags & flag_initial_pex; }
  bool                is_received_ext() const          { return m_flags & flag_received_ext; }

  void                clear_initial_pex()              { m_flags &= ~flag_initial_pex; }
  void                reset()                          { std::memset(&m_idMap, 0, sizeof(m_idMap)); }

  bool                request_metadata_piece(const Piece* p);

  // To handle cases where the extension protocol needs to send a reply.
  bool                has_pending_message() const      { return m_pendingType != HANDSHAKE; }
  MessageType         pending_message_type() const     { return m_pendingType; }
  DataBuffer          pending_message_data()           { return m_pending.release(); }
  void                clear_pending_message()          { if (m_pending.empty()) m_pendingType = HANDSHAKE; }

private:
  bool                parse_handshake();
  bool                parse_ut_pex();
  bool                parse_ut_metadata();

  static DataBuffer   build_bencode(size_t maxLength, const char* format, ...) ATTRIBUTE_PRINTF(2);

  void                peer_toggle_remote(int type, bool active);
  void                send_metadata_piece(size_t piece);

  // Map of IDs peer uses for each extension message type, excluding
  // HANDSHAKE. 
  uint8_t             m_idMap[extension_count];

  uint32_t            m_maxQueueLength;

  int                 m_flags;
  PeerInfo*           m_peerInfo;
  DownloadMain*       m_download;
  PeerConnectionBase* m_connection;

  uint8_t             m_readType;
  uint32_t            m_readLeft;
  char*               m_read;
  char*               m_readPos;

  MessageType         m_pendingType;
  DataBuffer          m_pending;
};

//
// 
//

enum ext_handshake_keys {
  key_e,
  key_m_utMetadata,
  key_m_utPex,
  key_metadataSize,
  key_p,
  key_reqq,
  key_v,
  key_handshake_LAST
};

enum ext_pex_keys {
  key_pex_added,
  key_pex_LAST
};

enum ext_metadata_keys {
  key_msgType,
  key_piece,
  key_totalSize,
  key_metadata_LAST
};

typedef static_map_type<ext_handshake_keys, key_handshake_LAST> ExtHandshakeMessage;
typedef static_map_type<ext_pex_keys, key_pex_LAST> ExtPEXMessage;
typedef static_map_type<ext_metadata_keys, key_metadata_LAST> ExtMetadataMessage;

//
//
//

inline
ProtocolExtension::ProtocolExtension() :
  // Set HANDSHAKE as enabled and supported. Those bits should not be
  // touched.
  m_flags(flag_local_enabled_base | flag_remote_supported_base | flag_initial_handshake),
  m_peerInfo(nullptr),
  m_download(nullptr),
  m_connection(nullptr),
  m_readType(FIRST_INVALID),
  m_read(nullptr),
  m_pendingType(HANDSHAKE) {

  reset();
  set_local_enabled(UT_METADATA);
}

inline ProtocolExtension
ProtocolExtension::make_default() {
  ProtocolExtension extension;
  extension.m_flags |= flag_default;
  return extension;
}

inline uint8_t
ProtocolExtension::id(int t) const {
  if (t == HANDSHAKE)
    return 0;

  if (t - 1 >= extension_count)
    throw internal_error("ProtocolExtension::id message type out of range.");

  return m_idMap[t - 1];
}

}

#endif
