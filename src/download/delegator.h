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

#ifndef LIBTORRENT_DELEGATOR_H
#define LIBTORRENT_DELEGATOR_H

#include <functional>
#include <string>
#include <vector>

#include "torrent/data/transfer_list.h"

namespace torrent {

class Block;
class BlockList;
class BlockTransfer;
class Piece;
class PeerChunks;
class PeerInfo;

class Delegator {
public:
  using slot_peer_chunk = std::function<uint32_t(PeerChunks*, bool)>;
  using slot_size       = std::function<uint32_t(uint32_t)>;

  static constexpr unsigned int block_size = 1 << 14;

  TransferList*       transfer_list()                     { return &m_transfers; }
  const TransferList* transfer_list() const               { return &m_transfers; }

  std::vector<BlockTransfer*> delegate(PeerChunks* peerChunks, uint32_t affinity, uint32_t maxPieces);

  bool               get_aggressive()                     { return m_aggressive; }
  void               set_aggressive(bool a)               { m_aggressive = a; }

  slot_peer_chunk&   slot_chunk_find()                    { return m_slot_chunk_find; }
  slot_size&         slot_chunk_size()                    { return m_slot_chunk_size; }

private:
  void               delegate_from_blocklist(std::vector<BlockTransfer*> &transfers, uint32_t maxPieces, BlockList* c, PeerInfo* peerInfo);
  void               delegate_new_chunks(std::vector<BlockTransfer*> &transfers, uint32_t maxPieces, PeerChunks* pc, bool highPriority);
  Block*             delegate_seeder(PeerChunks* peerChunks);

  TransferList       m_transfers;

  bool               m_aggressive{false};

  // Propably should add a m_slotChunkStart thing, which will take
  // care of enabling etc, and will be possible to listen to.
  slot_peer_chunk    m_slot_chunk_find;
  slot_size          m_slot_chunk_size;
};

}

#endif
