/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file object_map.h Map accessors for object tiles. */

#ifndef OBJECT_MAP_H
#define OBJECT_MAP_H

#include "water_map.h"
#include "object_type.h"

ObjectType GetObjectType(TileIndex t);

/**
 * Check whether the object on a tile is of a specific type.
 * @param t Tile to test.
 * @param type Type to test.
 * @pre IsTileType(t, MP_OBJECT)
 * @return True if type matches.
 */
static inline bool IsObjectType(TileIndex t, ObjectType type)
{
	return GetObjectType(t) == type;
}

/**
 * Check whether a tile is a object tile of a specific type.
 * @param t Tile to test.
 * @param type Type to test.
 * @return True if type matches.
 */
static inline bool IsObjectTypeTile(TileIndex t, ObjectType type)
{
	return IsTileType(t, MP_OBJECT) && GetObjectType(t) == type;
}

/**
 * Get the index of which object this tile is attached to.
 * @param t the tile
 * @pre IsTileType(t, MP_OBJECT)
 * @return The ObjectID of the object.
 */
static inline ObjectID GetObjectIndex(TileIndex t)
{
	assert(IsTileType(t, MP_OBJECT));
	return tile_map.get(t).object.index | tile_map.get(t).object.index2 << 16;
}

/**
 * Get the random bits of this tile.
 * @param t The tile to get the bits for.
 * @pre IsTileType(t, MP_OBJECT)
 * @return The random bits.
 */
static inline byte GetObjectRandomBits(TileIndex t)
{
	assert(IsTileType(t, MP_OBJECT));
	return tile_map.get(t).object.bits;
}


/**
 * Make an Object tile.
 * @param t      The tile to make and object tile.
 * @param o      The new owner of the tile.
 * @param index  Index to the object.
 * @param wc     Water class for this object.
 * @param random Random data to store on the tile
 */
static inline void MakeObject(TileIndex t, Owner o, ObjectID index, WaterClass wc, byte random)
{
	Tile& t_ = tile_map.init(t, MP_OBJECT);
	SetTileOwner(t, o);
	SetWaterClass(t, wc);
	t_.object.index = index;
	t_.object.bits = random;
	t_.object.index2 = index >> 16;
	SB(tile_map.get_e(t).m6, 2, 4, 0);
	tile_map.get_e(t).m7 = 0;
}

#endif /* OBJECT_MAP_H */
