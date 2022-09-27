/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file tunnel_map.h Map accessors for tunnels. */

#ifndef TUNNEL_MAP_H
#define TUNNEL_MAP_H

#include "rail_map.h"
#include "road_map.h"


/**
 * Is this a tunnel (entrance)?
 * @param t the tile that might be a tunnel
 * @pre IsTileType(t, MP_TUNNELBRIDGE)
 * @return true if and only if this tile is a tunnel (entrance)
 */
static inline bool IsTunnel(TileIndex t)
{
	assert(IsTileType(t, MP_TUNNELBRIDGE));
	return !tile_map.get(t).bridge.is_bridge;
}

/**
 * Is this a tunnel (entrance)?
 * @param t the tile that might be a tunnel
 * @return true if and only if this tile is a tunnel (entrance)
 */
static inline bool IsTunnelTile(TileIndex t)
{
	return IsTileType(t, MP_TUNNELBRIDGE) && IsTunnel(t);
}

TileIndex GetOtherTunnelEnd(TileIndex);
bool IsTunnelInWay(TileIndex, int z);
bool IsTunnelInWayDir(TileIndex tile, int z, DiagDirection dir);

/**
 * Makes a road tunnel entrance
 * @param t the entrance of the tunnel
 * @param o the owner of the entrance
 * @param d the direction facing out of the tunnel
 * @param r the road type used in the tunnel
 */
static inline void MakeRoadTunnel(TileIndex t, Owner o, DiagDirection d, RoadType road_rt, RoadType tram_rt)
{
	Tile& t_ = tile_map.init(t, MP_TUNNELBRIDGE, o);
	t_.bridge.direction = d;
	t_.bridge.transport_type = TRANSPORT_ROAD;
	SetRoadOwner(t, RTT_ROAD, o);
	if (o != OWNER_TOWN) SetRoadOwner(t, RTT_TRAM, o);
	SetRoadTypes(t, road_rt, tram_rt);
}

/**
 * Makes a rail tunnel entrance
 * @param t the entrance of the tunnel
 * @param o the owner of the entrance
 * @param d the direction facing out of the tunnel
 * @param r the rail type used in the tunnel
 */
static inline void MakeRailTunnel(TileIndex t, Owner o, DiagDirection d, RailType r)
{
	Tile& t_ = tile_map.init(t, MP_TUNNELBRIDGE, o);
	t_.bridge.direction = d;
	t_.bridge.transport_type = TRANSPORT_RAIL;
	SetRailType(t, r);
	SetRoadTypes(t, INVALID_ROADTYPE, INVALID_ROADTYPE);
}

#endif /* TUNNEL_MAP_H */
