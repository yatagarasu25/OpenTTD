/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file bridge_map.h Map accessor functions for bridges. */

#ifndef BRIDGE_MAP_H
#define BRIDGE_MAP_H

#include "rail_map.h"
#include "road_map.h"
#include "bridge.h"
#include "water_map.h"

/**
 * Checks if this is a bridge, instead of a tunnel
 * @param t The tile to analyze
 * @pre IsTileType(t, MP_TUNNELBRIDGE)
 * @return true if the structure is a bridge one
 */
static inline bool IsBridge(TileIndex t)
{
	return tile_map.bridge(t).is_bridge;
}

/**
 * checks if there is a bridge on this tile
 * @param t The tile to analyze
 * @return true if a bridge is present
 */
static inline bool IsBridgeTile(TileIndex t)
{
	return IsTileType(t, MP_TUNNELBRIDGE) && IsBridge(t);
}

/**
 * checks if a bridge is set above the ground of this tile
 * @param t The tile to analyze
 * @return true if a bridge is detected above
 */
static inline bool IsBridgeAbove(TileIndex t)
{
	return tile_map.get(t).above != 0;
}

/**
 * Determines the type of bridge on a tile
 * @param t The tile to analyze
 * @pre IsBridgeTile(t)
 * @return The bridge type
 */
static inline BridgeType GetBridgeType(TileIndex t)
{
	assert(IsBridgeTile(t));
	return (BridgeType)tile_map.bridge(t).type;
}

/**
 * Get the axis of the bridge that goes over the tile. Not the axis or the ramp.
 * @param t The tile to analyze
 * @pre IsBridgeAbove(t)
 * @return the above mentioned axis
 */
static inline Axis GetBridgeAxis(TileIndex t)
{
	assert(IsBridgeAbove(t));
	return (Axis)(tile_map.get(t).above - 1);
}

TileIndex GetNorthernBridgeEnd(TileIndex t);
TileIndex GetSouthernBridgeEnd(TileIndex t);
TileIndex GetOtherBridgeEnd(TileIndex t);

int GetBridgeHeight(TileIndex tile);
/**
 * Get the height ('z') of a bridge in pixels.
 * @param tile the bridge ramp tile to get the bridge height from
 * @return the height of the bridge in pixels
 */
static inline int GetBridgePixelHeight(TileIndex tile)
{
	return GetBridgeHeight(tile) * TILE_HEIGHT;
}

/**
 * Removes bridges from the given, that is bridges along the X and Y axis.
 * @param t the tile to remove the bridge from
 */
static inline void ClearBridgeMiddle(TileIndex t)
{
	tile_map.get(t).above = 0;
}

/**
 * Set that there is a bridge over the given axis.
 * @param t the tile to add the bridge to
 * @param a the axis of the bridge to add
 */
static inline void SetBridgeMiddle(TileIndex t, Axis a)
{
	if (a == AXIS_X) tile_map.get(t).above_x = 1;
	else if (a == AXIS_Y) tile_map.get(t).above_y = 1;
}

/**
 * Generic part to make a bridge ramp for both roads and rails.
 * @param t          the tile to make a bridge ramp
 * @param o          the new owner of the bridge ramp
 * @param bridgetype the type of bridge this bridge ramp belongs to
 * @param d          the direction this ramp must be facing
 * @param tt         the transport type of the bridge
 * @note this function should not be called directly.
 */
static inline void MakeBridgeRamp(TileIndex t, Owner o, BridgeType bridgetype, DiagDirection d, TransportType tt)
{
	auto& t_ = tile_map.init(t, MP_TUNNELBRIDGE, o).bridge();
	t_.old_road_type = INVALID_ROADTYPE;
	t_.is_bridge = 1;
	t_.direction = d;
	t_.transport_type = tt;
	t_.type = bridgetype;
	t_.rail_type = INVALID_RAILTYPE;
	t_.road_type = INVALID_ROADTYPE;
}

/**
 * Make a bridge ramp for roads.
 * @param t          the tile to make a bridge ramp
 * @param o          the new owner of the bridge ramp
 * @param owner_road the new owner of the road on the bridge
 * @param owner_tram the new owner of the tram on the bridge
 * @param bridgetype the type of bridge this bridge ramp belongs to
 * @param d          the direction this ramp must be facing
 * @param road_rt    the road type of the bridge
 * @param tram_rt    the tram type of the bridge
 */
static inline void MakeRoadBridgeRamp(TileIndex t, Owner o, Owner owner_road, Owner owner_tram, BridgeType bridgetype, DiagDirection d, RoadType road_rt, RoadType tram_rt)
{
	MakeBridgeRamp(t, o, bridgetype, d, TRANSPORT_ROAD);
	SetRoadOwner(t, RTT_ROAD, owner_road);
	if (owner_tram != OWNER_TOWN) SetRoadOwner(t, RTT_TRAM, owner_tram);
	SetRoadTypes(t, road_rt, tram_rt);
}

/**
 * Make a bridge ramp for rails.
 * @param t          the tile to make a bridge ramp
 * @param o          the new owner of the bridge ramp
 * @param bridgetype the type of bridge this bridge ramp belongs to
 * @param d          the direction this ramp must be facing
 * @param rt         the rail type of the bridge
 */
static inline void MakeRailBridgeRamp(TileIndex t, Owner o, BridgeType bridgetype, DiagDirection d, RailType rt)
{
	MakeBridgeRamp(t, o, bridgetype, d, TRANSPORT_RAIL);
	SetRailType(t, rt);
}

/**
 * Make a bridge ramp for aqueducts.
 * @param t          the tile to make a bridge ramp
 * @param o          the new owner of the bridge ramp
 * @param d          the direction this ramp must be facing
 */
static inline void MakeAqueductBridgeRamp(TileIndex t, Owner o, DiagDirection d)
{
	MakeBridgeRamp(t, o, 0, d, TRANSPORT_WATER);
}

#endif /* BRIDGE_MAP_H */
