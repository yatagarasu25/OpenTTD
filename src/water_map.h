/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file water_map.h Map accessors for water tiles. */

#ifndef WATER_MAP_H
#define WATER_MAP_H

#include "depot_type.h"
#include "tile_map.h"

/**
 * Bit field layout of m5 for water tiles.
 */
enum WaterTileTypeBitLayout {
	WBL_TYPE_BEGIN        = 4,   ///< Start of the 'type' bitfield.
	WBL_TYPE_COUNT        = 4,   ///< Length of the 'type' bitfield.

	WBL_TYPE_NORMAL       = 0x0, ///< Clear water or coast ('type' bitfield).
	WBL_TYPE_LOCK         = 0x1, ///< Lock ('type' bitfield).
	WBL_TYPE_DEPOT        = 0x8, ///< Depot ('type' bitfield).

	WBL_COAST_FLAG        = 0,   ///< Flag for coast.

	WBL_LOCK_ORIENT_BEGIN = 0,   ///< Start of lock orientation bitfield.
	WBL_LOCK_ORIENT_COUNT = 2,   ///< Length of lock orientation bitfield.
	WBL_LOCK_PART_BEGIN   = 2,   ///< Start of lock part bitfield.
	WBL_LOCK_PART_COUNT   = 2,   ///< Length of lock part bitfield.

	WBL_DEPOT_PART        = 0,   ///< Depot part flag.
	WBL_DEPOT_AXIS        = 1,   ///< Depot axis flag.
};

/** Available water tile types. */
enum WaterTileType {
	WATER_TILE_CLEAR, ///< Plain water.
	WATER_TILE_COAST, ///< Coast.
	WATER_TILE_LOCK,  ///< Water lock.
	WATER_TILE_DEPOT, ///< Water Depot.
};

/** classes of water (for #WATER_TILE_CLEAR water tile type). */
enum WaterClass : byte {
	WATER_CLASS_SEA,     ///< Sea.
	WATER_CLASS_CANAL,   ///< Canal.
	WATER_CLASS_RIVER,   ///< River.
	WATER_CLASS_INVALID, ///< Used for industry tiles on land (also for oilrig if newgrf says so).
};
/** Helper information for extract tool. */
template <> struct EnumPropsT<WaterClass> : MakeEnumPropsT<WaterClass, byte, WATER_CLASS_SEA, WATER_CLASS_INVALID, WATER_CLASS_INVALID, 2> {};

/**
 * Checks if a water class is valid.
 *
 * @param wc The value to check
 * @return true if the given value is a valid water class.
 */
static inline bool IsValidWaterClass(WaterClass wc)
{
	return wc < WATER_CLASS_INVALID;
}

/** Sections of the water depot. */
enum DepotPart {
	DEPOT_PART_NORTH = 0, ///< Northern part of a depot.
	DEPOT_PART_SOUTH = 1, ///< Southern part of a depot.
	DEPOT_PART_END
};

/** Sections of the water lock. */
enum LockPart {
	LOCK_PART_MIDDLE = 0, ///< Middle part of a lock.
	LOCK_PART_LOWER  = 1, ///< Lower part of a lock.
	LOCK_PART_UPPER  = 2, ///< Upper part of a lock.
};

bool IsPossibleDockingTile(TileIndex t);

/**
 * Get the water tile type at a tile.
 * @param t Water tile to query.
 * @return Water tile type at the tile.
 */
static inline WaterTileType GetWaterTileType(TileIndex t)
{
	assert(IsTileType(t, MP_WATER));

	switch (tile_map.get(t).water.type) {
		case WBL_TYPE_NORMAL: return tile_map.get(t).water.is_coast ? WATER_TILE_COAST : WATER_TILE_CLEAR;
		case WBL_TYPE_LOCK:   return WATER_TILE_LOCK;
		case WBL_TYPE_DEPOT:  return WATER_TILE_DEPOT;
		default: NOT_REACHED();
	}
}

/**
 * Checks whether the tile has an waterclass associated.
 * You can then subsequently call GetWaterClass().
 * @param t Tile to query.
 * @return True if the tiletype has a waterclass.
 */
static inline bool HasTileWaterClass(TileIndex t)
{
	return IsTileType(t, MP_WATER) || IsTileType(t, MP_STATION) || IsTileType(t, MP_INDUSTRY) || IsTileType(t, MP_OBJECT) || IsTileType(t, MP_TREES);
}

/**
 * Get the water class at a tile.
 * @param t Water tile to query.
 * @pre IsTileType(t, MP_WATER) || IsTileType(t, MP_STATION) || IsTileType(t, MP_INDUSTRY) || IsTileType(t, MP_OBJECT)
 * @return Water class at the tile.
 */
static inline WaterClass GetWaterClass(TileIndex t)
{
	assert(HasTileWaterClass(t));
	return (WaterClass)tile_map.get(t).water.wc;
}

/**
 * Set the water class at a tile.
 * @param t  Water tile to change.
 * @param wc New water class.
 * @pre IsTileType(t, MP_WATER) || IsTileType(t, MP_STATION) || IsTileType(t, MP_INDUSTRY) || IsTileType(t, MP_OBJECT)
 */
static inline void SetWaterClass(TileIndex t, WaterClass wc)
{
	assert(HasTileWaterClass(t));
	tile_map.get(t).water.wc = wc;
}

/**
 * Tests if the tile was built on water.
 * @param t the tile to check
 * @pre IsTileType(t, MP_WATER) || IsTileType(t, MP_STATION) || IsTileType(t, MP_INDUSTRY) || IsTileType(t, MP_OBJECT)
 * @return true iff on water
 */
static inline bool IsTileOnWater(TileIndex t)
{
	return (GetWaterClass(t) != WATER_CLASS_INVALID);
}

/**
 * Is it a plain water tile?
 * @param t Water tile to query.
 * @return \c true if any type of clear water like ocean, river, or canal.
 * @pre IsTileType(t, MP_WATER)
 */
static inline bool IsWater(TileIndex t)
{
	return GetWaterTileType(t) == WATER_TILE_CLEAR;
}

/**
 * Is it a sea water tile?
 * @param t Water tile to query.
 * @return \c true if it is a sea water tile.
 * @pre IsTileType(t, MP_WATER)
 */
static inline bool IsSea(TileIndex t)
{
	return IsWater(t) && GetWaterClass(t) == WATER_CLASS_SEA;
}

/**
 * Is it a canal tile?
 * @param t Water tile to query.
 * @return \c true if it is a canal tile.
 * @pre IsTileType(t, MP_WATER)
 */
static inline bool IsCanal(TileIndex t)
{
	return IsWater(t) && GetWaterClass(t) == WATER_CLASS_CANAL;
}

/**
 * Is it a river water tile?
 * @param t Water tile to query.
 * @return \c true if it is a river water tile.
 * @pre IsTileType(t, MP_WATER)
 */
static inline bool IsRiver(TileIndex t)
{
	return IsWater(t) && GetWaterClass(t) == WATER_CLASS_RIVER;
}

/**
 * Is it a water tile with plain water?
 * @param t Tile to query.
 * @return \c true if it is a plain water tile.
 */
static inline bool IsWaterTile(TileIndex t)
{
	return IsTileType(t, MP_WATER) && IsWater(t);
}

/**
 * Is it a coast tile?
 * @param t Water tile to query.
 * @return \c true if it is a sea water tile.
 * @pre IsTileType(t, MP_WATER)
 */
static inline bool IsCoast(TileIndex t)
{
	return GetWaterTileType(t) == WATER_TILE_COAST;
}

/**
 * Is it a coast tile
 * @param t Tile to query.
 * @return \c true if it is a coast.
 */
static inline bool IsCoastTile(TileIndex t)
{
	return (IsTileType(t, MP_WATER) && IsCoast(t)) || (IsTileType(t, MP_TREES) && GetWaterClass(t) != WATER_CLASS_INVALID);
}

/**
 * Is it a water tile with a ship depot on it?
 * @param t Water tile to query.
 * @return \c true if it is a ship depot tile.
 * @pre IsTileType(t, MP_WATER)
 */
static inline bool IsShipDepot(TileIndex t)
{
	return GetWaterTileType(t) == WATER_TILE_DEPOT;
}

/**
 * Is it a ship depot tile?
 * @param t Tile to query.
 * @return \c true if it is a ship depot tile.
 */
static inline bool IsShipDepotTile(TileIndex t)
{
	return IsTileType(t, MP_WATER) && IsShipDepot(t);
}

/**
 * Get the axis of the ship depot.
 * @param t Water tile to query.
 * @return Axis of the depot.
 * @pre IsShipDepotTile(t)
 */
static inline Axis GetShipDepotAxis(TileIndex t)
{
	assert(IsShipDepotTile(t));
	return (Axis)tile_map.get(t).depot.ship.axis;
}

/**
 * Get the part of a ship depot.
 * @param t Water tile to query.
 * @return Part of the depot.
 * @pre IsShipDepotTile(t)
 */
static inline DepotPart GetShipDepotPart(TileIndex t)
{
	assert(IsShipDepotTile(t));
	return (DepotPart)tile_map.get(t).depot.ship.part;
}

/**
 * Get the direction of the ship depot.
 * @param t Water tile to query.
 * @return Direction of the depot.
 * @pre IsShipDepotTile(t)
 */
static inline DiagDirection GetShipDepotDirection(TileIndex t)
{
	return XYNSToDiagDir(GetShipDepotAxis(t), GetShipDepotPart(t));
}

/**
 * Get the other tile of the ship depot.
 * @param t Tile to query, containing one section of a ship depot.
 * @return Tile containing the other section of the depot.
 * @pre IsShipDepotTile(t)
 */
static inline TileIndex GetOtherShipDepotTile(TileIndex t)
{
	return t + (GetShipDepotPart(t) != DEPOT_PART_NORTH ? -1 : 1) * (GetShipDepotAxis(t) != AXIS_X ? TileDiffXY(0, 1) : TileDiffXY(1, 0));
}

/**
 * Get the most northern tile of a ship depot.
 * @param t One of the tiles of the ship depot.
 * @return The northern tile of the depot.
 * @pre IsShipDepotTile(t)
 */
static inline TileIndex GetShipDepotNorthTile(TileIndex t)
{
	assert(IsShipDepot(t));
	TileIndex tile2 = GetOtherShipDepotTile(t);

	return t < tile2 ? t : tile2;
}

/**
 * Is there a lock on a given water tile?
 * @param t Water tile to query.
 * @return \c true if it is a water lock tile.
 * @pre IsTileType(t, MP_WATER)
 */
static inline bool IsLock(TileIndex t)
{
	return GetWaterTileType(t) == WATER_TILE_LOCK;
}

/**
 * Get the direction of the water lock.
 * @param t Water tile to query.
 * @return Direction of the lock.
 * @pre IsTileType(t, MP_WATER) && IsLock(t)
 */
static inline DiagDirection GetLockDirection(TileIndex t)
{
	assert(IsLock(t));
	return (DiagDirection)tile_map.get(t).lock.direction;
}

/**
 * Get the part of a lock.
 * @param t Water tile to query.
 * @return The part.
 * @pre IsTileType(t, MP_WATER) && IsLock(t)
 */
static inline byte GetLockPart(TileIndex t)
{
	assert(IsLock(t));
	return tile_map.get(t).lock.part;
}

/**
 * Get the random bits of the water tile.
 * @param t Water tile to query.
 * @return Random bits of the tile.
 * @pre IsTileType(t, MP_WATER)
 */
static inline byte GetWaterTileRandomBits(TileIndex t)
{
	assert(IsTileType(t, MP_WATER));
	return tile_map.get(t).water.bits;
}

/**
 * Checks whether the tile has water at the ground.
 * That is, it is either some plain water tile, or a object/industry/station/... with water under it.
 * @return true iff the tile has water at the ground.
 * @note Coast tiles are not considered waterish, even if there is water on a halftile.
 */
static inline bool HasTileWaterGround(TileIndex t)
{
	return HasTileWaterClass(t) && IsTileOnWater(t) && !IsCoastTile(t);
}

/**
 * Set the docking tile state of a tile. This is used by pathfinders to reach their destination.
 * As well as water tiles, half-rail tiles, buoys and aqueduct ends can also be docking tiles.
 * @param t the tile
 * @param b the docking tile state
 */
static inline void SetDockingTile(TileIndex t, bool b)
{
	assert(IsTileType(t, MP_WATER) || IsTileType(t, MP_RAILWAY) || IsTileType(t, MP_STATION) || IsTileType(t, MP_TUNNELBRIDGE));
	tile_map.get(t).water.is_docking = b ? 1 : 0;
}

/**
 * Checks whether the tile is marked as a dockling tile.
 * @return true iff the tile is marked as a docking tile.
 */
static inline bool IsDockingTile(TileIndex t)
{
	return (IsTileType(t, MP_WATER) || IsTileType(t, MP_RAILWAY) || IsTileType(t, MP_STATION) || IsTileType(t, MP_TUNNELBRIDGE))
		&& tile_map.get(t).water.is_docking;
}


/**
 * Helper function to make a coast tile.
 * @param t The tile to change into water
 */
static inline void MakeShore(TileIndex t)
{
	Tile& t_ = tile_map.init(t, MP_WATER, OWNER_WATER);
	SetWaterClass(t, WATER_CLASS_SEA);
	SetDockingTile(t, false);
	t_.water.is_coast = 1;
	t_.water.type = WBL_TYPE_NORMAL;
	SB(tile_map.get_e(t).water.m6, 2, 4, 0);
	tile_map.get_e(t).water.m7 = 0;
}

/**
 * Helper function for making a watery tile.
 * @param t The tile to change into water
 * @param o The owner of the water
 * @param wc The class of water the tile has to be
 * @param random_bits Eventual random bits to be set for this tile
 */
static inline void MakeWater(TileIndex t, Owner o, WaterClass wc, uint8 random_bits)
{
	Tile& t_ = tile_map.init(t, MP_WATER, o);
	SetWaterClass(t, wc);
	SetDockingTile(t, false);
	t_.water.bits = random_bits;
	t_.water.type = WBL_TYPE_NORMAL;
	SB(tile_map.get_e(t).water.m6, 2, 4, 0);
	tile_map.get_e(t).water.m7 = 0;
}

/**
 * Make a sea tile.
 * @param t The tile to change into sea
 */
static inline void MakeSea(TileIndex t)
{
	MakeWater(t, OWNER_WATER, WATER_CLASS_SEA, 0);
}

/**
 * Make a river tile
 * @param t The tile to change into river
 * @param random_bits Random bits to be set for this tile
 */
static inline void MakeRiver(TileIndex t, uint8 random_bits)
{
	MakeWater(t, OWNER_WATER, WATER_CLASS_RIVER, random_bits);
}

/**
 * Make a canal tile
 * @param t The tile to change into canal
 * @param o The owner of the canal
 * @param random_bits Random bits to be set for this tile
 */
static inline void MakeCanal(TileIndex t, Owner o, uint8 random_bits)
{
	assert(o != OWNER_WATER);
	MakeWater(t, o, WATER_CLASS_CANAL, random_bits);
}

/**
 * Make a ship depot section.
 * @param t    Tile to place the ship depot section.
 * @param o    Owner of the depot.
 * @param did  Depot ID.
 * @param part Depot part (either #DEPOT_PART_NORTH or #DEPOT_PART_SOUTH).
 * @param a    Axis of the depot.
 * @param original_water_class Original water class.
 */
static inline void MakeShipDepot(TileIndex t, Owner o, DepotID did, DepotPart part, Axis a, WaterClass original_water_class)
{
	Tile& t_ = tile_map.init(t, MP_WATER, o);
	SetWaterClass(t, original_water_class);
	SetDockingTile(t, false);
	t_.depot.id = did;
	t_.depot.ship.part = part;
	t_.depot.ship.axis = a;
	t_.depot.ship.type = WBL_TYPE_DEPOT;
	SB(tile_map.get_e(t).water.m6, 2, 4, 0);
	tile_map.get_e(t).water.m7 = 0;
}

/**
 * Make a lock section.
 * @param t Tile to place the water lock section.
 * @param o Owner of the lock.
 * @param part Part to place.
 * @param dir Lock orientation
 * @param original_water_class Original water class.
 * @see MakeLock
 */
static inline void MakeLockTile(TileIndex t, Owner o, LockPart part, DiagDirection dir, WaterClass original_water_class)
{
	Tile& t_ = tile_map.init(t, MP_WATER, o);
	SetWaterClass(t, original_water_class);
	SetDockingTile(t, false);
	t_.lock.direction = dir;
	t_.lock.part = part;
	t_.lock.type = WBL_TYPE_LOCK;
	SB(tile_map.get_e(t).water.m6, 2, 4, 0);
	tile_map.get_e(t).water.m7 = 0;
}

/**
 * Make a water lock.
 * @param t Tile to place the water lock section.
 * @param o Owner of the lock.
 * @param d Direction of the water lock.
 * @param wc_lower Original water class of the lower part.
 * @param wc_upper Original water class of the upper part.
 * @param wc_middle Original water class of the middle part.
 */
static inline void MakeLock(TileIndex t, Owner o, DiagDirection d, WaterClass wc_lower, WaterClass wc_upper, WaterClass wc_middle)
{
	TileIndexDiff delta = TileOffsByDiagDir(d);

	/* Keep the current waterclass and owner for the tiles.
	 * It allows to restore them after the lock is deleted */
	MakeLockTile(t, o, LOCK_PART_MIDDLE, d, wc_middle);
	MakeLockTile(t - delta, IsWaterTile(t - delta) ? GetTileOwner(t - delta) : o, LOCK_PART_LOWER, d, wc_lower);
	MakeLockTile(t + delta, IsWaterTile(t + delta) ? GetTileOwner(t + delta) : o, LOCK_PART_UPPER, d, wc_upper);
}

#endif /* WATER_MAP_H */
