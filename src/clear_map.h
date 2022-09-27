/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file clear_map.h Map accessors for 'clear' tiles */

#ifndef CLEAR_MAP_H
#define CLEAR_MAP_H

#include "bridge_map.h"
#include "industry_type.h"

/**
 * Ground types. Valid densities in comments after the enum.
 */
enum ClearGround {
	CLEAR_GRASS  = 0, ///< 0-3
	CLEAR_ROUGH  = 1, ///< 3
	CLEAR_ROCKS  = 2, ///< 3
	CLEAR_FIELDS = 3, ///< 3
	CLEAR_SNOW   = 4, ///< 0-3
	CLEAR_DESERT = 5, ///< 1,3
};


/**
 * Test if a tile is covered with snow.
 * @param t the tile to check
 * @pre IsTileType(t, MP_CLEAR)
 * @return whether the tile is covered with snow.
 */
static inline bool IsSnowTile(TileIndex t)
{
	return tile_map.clear(t).is_snow;
}

/**
 * Get the type of clear tile but never return CLEAR_SNOW.
 * @param t the tile to get the clear ground type of
 * @pre IsTileType(t, MP_CLEAR)
 * @return the ground type
 */
static inline ClearGround GetRawClearGround(TileIndex t)
{
	return (ClearGround)tile_map.clear(t).ground_type;
}

/**
 * Get the type of clear tile.
 * @param t the tile to get the clear ground type of
 * @pre IsTileType(t, MP_CLEAR)
 * @return the ground type
 */
static inline ClearGround GetClearGround(TileIndex t)
{
	if (IsSnowTile(t)) return CLEAR_SNOW;
	return GetRawClearGround(t);
}

/**
 * Set the type of clear tile.
 * @param t  the tile to set the clear ground type of
 * @param ct the ground type
 * @pre IsTileType(t, MP_CLEAR)
 */
static inline bool IsClearGround(TileIndex t, ClearGround ct)
{
	return GetClearGround(t) == ct;
}


/**
 * Get the density of a non-field clear tile.
 * @param t the tile to get the density of
 * @pre IsTileType(t, MP_CLEAR)
 * @return the density
 */
static inline uint GetClearDensity(TileIndex t)
{
	return tile_map.clear(t).density;
}

/**
 * Increment the density of a non-field clear tile.
 * @param t the tile to increment the density of
 * @param d the amount to increment the density with
 * @pre IsTileType(t, MP_CLEAR)
 */
static inline void AddClearDensity(TileIndex t, int d)
{
	tile_map.clear(t).density += d;
}

/**
 * Set the density of a non-field clear tile.
 * @param t the tile to set the density of
 * @param d the new density
 * @pre IsTileType(t, MP_CLEAR)
 */
static inline void SetClearDensity(TileIndex t, uint d)
{
	tile_map.clear(t).density = d;
}


/**
 * Get the counter used to advance to the next clear density/field type.
 * @param t the tile to get the counter of
 * @pre IsTileType(t, MP_CLEAR)
 * @return the value of the counter
 */
static inline uint GetClearCounter(TileIndex t)
{
	return tile_map.clear(t).counter;
}

/**
 * Increments the counter used to advance to the next clear density/field type.
 * @param t the tile to increment the counter of
 * @param c the amount to increment the counter with
 * @pre IsTileType(t, MP_CLEAR)
 */
static inline void AddClearCounter(TileIndex t, int c)
{
	tile_map.clear(t).counter += c;
}

/**
 * Sets the counter used to advance to the next clear density/field type.
 * @param t the tile to set the counter of
 * @param c the amount to set the counter to
 * @pre IsTileType(t, MP_CLEAR)
 */
static inline void SetClearCounter(TileIndex t, uint c)
{
	tile_map.clear(t).counter = c;
}


/**
 * Sets ground type and density in one go, also sets the counter to 0
 * @param t       the tile to set the ground type and density for
 * @param type    the new ground type of the tile
 * @param density the density of the ground tile
 * @pre IsTileType(t, MP_CLEAR)
 */
static inline void SetClearGroundDensity(TileIndex t, ClearGround type, uint density)
{
	tile_map.clear(t).density = density;
	tile_map.clear(t).ground_type = type;
	tile_map.clear(t).counter = 0;
}


/**
 * Get the field type (production stage) of the field
 * @param t the field to get the type of
 * @pre GetClearGround(t) == CLEAR_FIELDS
 * @return the field type
 */
static inline uint GetFieldType(TileIndex t)
{
	assert(GetClearGround(t) == CLEAR_FIELDS);
	return tile_map.get(t).field.type;
}

/**
 * Set the field type (production stage) of the field
 * @param t the field to get the type of
 * @param f the field type
 * @pre GetClearGround(t) == CLEAR_FIELDS
 */
static inline void SetFieldType(TileIndex t, uint f)
{
	assert(GetClearGround(t) == CLEAR_FIELDS); // XXX incomplete
	tile_map.get(t).field.type = f;
}

/**
 * Get the industry (farm) that made the field
 * @param t the field to get creating industry of
 * @pre GetClearGround(t) == CLEAR_FIELDS
 * @return the industry that made the field
 */
static inline IndustryID GetIndustryIndexOfField(TileIndex t)
{
	assert(GetClearGround(t) == CLEAR_FIELDS);
	return(IndustryID) tile_map.get(t).field.industry_id;
}

/**
 * Set the industry (farm) that made the field
 * @param t the field to get creating industry of
 * @param i the industry that made the field
 * @pre GetClearGround(t) == CLEAR_FIELDS
 */
static inline void SetIndustryIndexOfField(TileIndex t, IndustryID i)
{
	assert(GetClearGround(t) == CLEAR_FIELDS);
	tile_map.get(t).field.industry_id = i;
}


/**
 * Is there a fence at the given border?
 * @param t the tile to check for fences
 * @param side the border to check
 * @pre IsClearGround(t, CLEAR_FIELDS)
 * @return 0 if there is no fence, otherwise the fence type
 */
static inline uint GetFence(TileIndex t, DiagDirection side)
{
	assert(IsClearGround(t, CLEAR_FIELDS));
	switch (side) {
		default: NOT_REACHED();
		case DIAGDIR_SE: return tile_map.get(t).field.fence_se;
		case DIAGDIR_SW: return tile_map.get(t).field.fence_sw;
		case DIAGDIR_NE: return tile_map.get(t).field.fence_ne;
		case DIAGDIR_NW: return GB(tile_map.get_e(t).m6, 2, 3);
	}
}

/**
 * Sets the type of fence (and whether there is one) for the given border.
 * @param t the tile to check for fences
 * @param side the border to check
 * @param h 0 if there is no fence, otherwise the fence type
 * @pre IsClearGround(t, CLEAR_FIELDS)
 */
static inline void SetFence(TileIndex t, DiagDirection side, uint h)
{
	assert(IsClearGround(t, CLEAR_FIELDS));
	switch (side) {
		default: NOT_REACHED();
		case DIAGDIR_SE: tile_map.get(t).field.fence_se = h; break;
		case DIAGDIR_SW: tile_map.get(t).field.fence_sw = h; break;
		case DIAGDIR_NE: tile_map.get(t).field.fence_ne = h; break;
		case DIAGDIR_NW: SB(tile_map.get_e(t).m6, 2, 3, h); break;
	}
}


/**
 * Make a clear tile.
 * @param t       the tile to make a clear tile
 * @param g       the type of ground
 * @param density the density of the grass/snow/desert etc
 */
static inline void MakeClear(TileIndex t, ClearGround g, uint density)
{
	Tile& t_ = tile_map.init(t, MP_CLEAR, OWNER_NONE);
	SetClearGroundDensity(t, g, density); // Sets m5
	tile_map.get_e(t).m6 = 0;
	tile_map.get_e(t).m7 = 0;
	tile_map.get_e(t).m8 = 0;
}


/**
 * Make a (farm) field tile.
 * @param t          the tile to make a farm field
 * @param field_type the 'growth' level of the field
 * @param industry   the industry this tile belongs to
 */
static inline void MakeField(TileIndex t, uint field_type, IndustryID industry)
{
	Tile& t_ = tile_map.init(t, MP_CLEAR, OWNER_NONE);
	t_.field.industry_id = industry; // TODO ?????
	t_.field.type = field_type;
	SetClearGroundDensity(t, CLEAR_FIELDS, 3);
	SB(tile_map.get_e(t).m6, 2, 4, 0);
	tile_map.get_e(t).m7 = 0;
	tile_map.get_e(t).m8 = 0;
}

/**
 * Make a snow tile.
 * @param t the tile to make snowy
 * @param density The density of snowiness.
 * @pre GetClearGround(t) != CLEAR_SNOW
 */
static inline void MakeSnow(TileIndex t, uint density = 0)
{
	assert(GetClearGround(t) != CLEAR_SNOW);
	tile_map.clear(t).is_snow = 1;
	if (GetRawClearGround(t) == CLEAR_FIELDS) {
		SetClearGroundDensity(t, CLEAR_GRASS, density);
	} else {
		SetClearDensity(t, density);
	}
}

/**
 * Clear the snow from a tile and return it to its previous type.
 * @param t the tile to clear of snow
 * @pre GetClearGround(t) == CLEAR_SNOW
 */
static inline void ClearSnow(TileIndex t)
{
	assert(GetClearGround(t) == CLEAR_SNOW);
	tile_map.clear(t).is_snow = 0;
	SetClearDensity(t, 3);
}

#endif /* CLEAR_MAP_H */
