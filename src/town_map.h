/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file town_map.h Accessors for towns */

#ifndef TOWN_MAP_H
#define TOWN_MAP_H

#include "road_map.h"
#include "house.h"

/**
 * Get the index of which town this house/street is attached to.
 * @param t the tile
 * @pre IsTileType(t, MP_HOUSE) or IsTileType(t, MP_ROAD) but not a road depot
 * @return TownID
 */
static inline TownID GetTownIndex(TileIndex t)
{
	assert(IsTileType(t, MP_HOUSE) || (IsTileType(t, MP_ROAD) && !IsRoadDepot(t)));
	return tile_map.get(t).town.id;
}

/**
 * Set the town index for a road or house tile.
 * @param t the tile
 * @param index the index of the town
 * @pre IsTileType(t, MP_HOUSE) or IsTileType(t, MP_ROAD) but not a road depot
 */
static inline void SetTownIndex(TileIndex t, TownID index)
{
	assert(IsTileType(t, MP_HOUSE) || (IsTileType(t, MP_ROAD) && !IsRoadDepot(t)));
	tile_map.get(t).town.id = index;
}

/**
 * Get the type of this house, which is an index into the house spec array
 * without doing any NewGRF related translations.
 * @param t the tile
 * @pre IsTileType(t, MP_HOUSE)
 * @return house type
 */
static inline HouseID GetCleanHouseType(TileIndex t)
{
	assert(IsTileType(t, MP_HOUSE));
	return tile_map.get(t).house.house_id | (tile_map.get(t).house.clean_house_flag << 8);
}

/**
 * Get the type of this house, which is an index into the house spec array
 * @param t the tile
 * @pre IsTileType(t, MP_HOUSE)
 * @return house type
 */
static inline HouseID GetHouseType(TileIndex t)
{
	return GetTranslatedHouseID(GetCleanHouseType(t));
}

/**
 * Set the house type.
 * @param t the tile
 * @param house_id the new house type
 * @pre IsTileType(t, MP_HOUSE)
 */
static inline void SetHouseType(TileIndex t, HouseID house_id)
{
	assert(IsTileType(t, MP_HOUSE));
	tile_map.get(t).house.house_id = GB(house_id, 0, 8);
	tile_map.get(t).house.clean_house_flag = GB(house_id, 8, 1);
}

/**
 * Check if the lift of this animated house has a destination
 * @param t the tile
 * @return has destination
 */
static inline bool LiftHasDestination(TileIndex t)
{
	return tile_map.get(t).house.has_destination;
}

/**
 * Set the new destination of the lift for this animated house, and activate
 * the LiftHasDestination bit.
 * @param t the tile
 * @param dest new destination
 */
static inline void SetLiftDestination(TileIndex t, byte dest)
{
	tile_map.get(t).house.has_destination = 1;
	tile_map.get(t).house.destination = dest;
}

/**
 * Get the current destination for this lift
 * @param t the tile
 * @return destination
 */
static inline byte GetLiftDestination(TileIndex t)
{
	return tile_map.get(t).house.destination;
}

/**
 * Stop the lift of this animated house from moving.
 * Clears the first 4 bits of m7 at once, clearing the LiftHasDestination bit
 * and the destination.
 * @param t the tile
 */
static inline void HaltLift(TileIndex t)
{
	tile_map.get(t).house.has_destination = 0;
	tile_map.get(t).house.destination = 0;
}

/**
 * Get the position of the lift on this animated house
 * @param t the tile
 * @return position, from 0 to 36
 */
static inline byte GetLiftPosition(TileIndex t)
{
	return tile_map.get(t).house.lift.position;
}

/**
 * Set the position of the lift on this animated house
 * @param t the tile
 * @param pos position, from 0 to 36
 */
static inline void SetLiftPosition(TileIndex t, byte pos)
{
	tile_map.get(t).house.lift.position = pos;
}

/**
 * Get the completion of this house
 * @param t the tile
 * @return true if it is, false if it is not
 */
static inline bool IsHouseCompleted(TileIndex t)
{
	assert(IsTileType(t, MP_HOUSE));
	return tile_map.get(t).house.is_completed;
}

/**
 * Mark this house as been completed
 * @param t the tile
 * @param status
 */
static inline void SetHouseCompleted(TileIndex t, bool status)
{
	assert(IsTileType(t, MP_HOUSE));
	tile_map.get(t).house.is_completed = !!status;
}

/**
 * House Construction Scheme.
 *  Construction counter, for buildings under construction. Incremented on every
 *  periodic tile processing.
 *  On wraparound, the stage of building in is increased.
 *  GetHouseBuildingStage is taking care of the real stages,
 *  (as the sprite for the next phase of house building)
 *  (Get|Inc)HouseConstructionTick is simply a tick counter between the
 *  different stages
 */

/**
 * Gets the building stage of a house
 * Since the stage is used for determining what sprite to use,
 * if the house is complete (and that stage no longer is available),
 * fool the system by returning the TOWN_HOUSE_COMPLETE (3),
 * thus showing a beautiful complete house.
 * @param t the tile of the house to get the building stage of
 * @pre IsTileType(t, MP_HOUSE)
 * @return the building stage of the house
 */
static inline byte GetHouseBuildingStage(TileIndex t)
{
	assert(IsTileType(t, MP_HOUSE));
	return IsHouseCompleted(t) ? (byte)TOWN_HOUSE_COMPLETED : tile_map.get(t).house.stage;
}

/**
 * Gets the construction stage of a house
 * @param t the tile of the house to get the construction stage of
 * @pre IsTileType(t, MP_HOUSE)
 * @return the construction stage of the house
 */
static inline byte GetHouseConstructionTick(TileIndex t)
{
	assert(IsTileType(t, MP_HOUSE));
	return IsHouseCompleted(t) ? 0 : tile_map.get(t).house.tick;
}

/**
 * Sets the increment stage of a house
 * It is working with the whole counter + stage 5 bits, making it
 * easier to work:  the wraparound is automatic.
 * @param t the tile of the house to increment the construction stage of
 * @pre IsTileType(t, MP_HOUSE)
 */
static inline void IncHouseConstructionTick(TileIndex t)
{
	assert(IsTileType(t, MP_HOUSE));
	tile_map.get(t).house.tick_and_stage += 1;

	if (tile_map.get(t).house.stage == TOWN_HOUSE_COMPLETED) {
		/* House is now completed.
		 * Store the year of construction as well, for newgrf house purpose */
		SetHouseCompleted(t, true);
	}
}

/**
 * Sets the age of the house to zero.
 * Needs to be called after the house is completed. During construction stages the map space is used otherwise.
 * @param t the tile of this house
 * @pre IsTileType(t, MP_HOUSE) && IsHouseCompleted(t)
 */
static inline void ResetHouseAge(TileIndex t)
{
	assert(IsTileType(t, MP_HOUSE) && IsHouseCompleted(t));
	tile_map.get(t).house.age = 0;
}

/**
 * Increments the age of the house.
 * @param t the tile of this house
 * @pre IsTileType(t, MP_HOUSE)
 */
static inline void IncrementHouseAge(TileIndex t)
{
	assert(IsTileType(t, MP_HOUSE));
	if (IsHouseCompleted(t) && tile_map.get(t).house.age < 0xFF) tile_map.get(t).house.age++;
}

/**
 * Get the age of the house
 * @param t the tile of this house
 * @pre IsTileType(t, MP_HOUSE)
 * @return year
 */
static inline Year GetHouseAge(TileIndex t)
{
	assert(IsTileType(t, MP_HOUSE));
	return IsHouseCompleted(t) ? tile_map.get(t).house.age : 0;
}

/**
 * Set the random bits for this house.
 * This is required for newgrf house
 * @param t      the tile of this house
 * @param random the new random bits
 * @pre IsTileType(t, MP_HOUSE)
 */
static inline void SetHouseRandomBits(TileIndex t, byte random)
{
	assert(IsTileType(t, MP_HOUSE));
	tile_map.get(t).house.random = random;
}

/**
 * Get the random bits for this house.
 * This is required for newgrf house
 * @param t the tile of this house
 * @pre IsTileType(t, MP_HOUSE)
 * @return random bits
 */
static inline byte GetHouseRandomBits(TileIndex t)
{
	assert(IsTileType(t, MP_HOUSE));
	return tile_map.get(t).house.random;
}

/**
 * Set the activated triggers bits for this house.
 * This is required for newgrf house
 * @param t        the tile of this house
 * @param triggers the activated triggers
 * @pre IsTileType(t, MP_HOUSE)
 */
static inline void SetHouseTriggers(TileIndex t, byte triggers)
{
	assert(IsTileType(t, MP_HOUSE));
	tile_map.get(t).house.triggers = triggers;
}

/**
 * Get the already activated triggers bits for this house.
 * This is required for newgrf house
 * @param t the tile of this house
 * @pre IsTileType(t, MP_HOUSE)
 * @return triggers
 */
static inline byte GetHouseTriggers(TileIndex t)
{
	assert(IsTileType(t, MP_HOUSE));
	return tile_map.get(t).house.triggers;
}

/**
 * Get the amount of time remaining before the tile loop processes this tile.
 * @param t the house tile
 * @pre IsTileType(t, MP_HOUSE)
 * @return time remaining
 */
static inline byte GetHouseProcessingTime(TileIndex t)
{
	assert(IsTileType(t, MP_HOUSE));
	return tile_map.get(t).house.processing;
}

/**
 * Set the amount of time remaining before the tile loop processes this tile.
 * @param t the house tile
 * @param time the time to be set
 * @pre IsTileType(t, MP_HOUSE)
 */
static inline void SetHouseProcessingTime(TileIndex t, byte time)
{
	assert(IsTileType(t, MP_HOUSE));
	tile_map.get(t).house.processing = time;
}

/**
 * Decrease the amount of time remaining before the tile loop processes this tile.
 * @param t the house tile
 * @pre IsTileType(t, MP_HOUSE)
 */
static inline void DecHouseProcessingTime(TileIndex t)
{
	assert(IsTileType(t, MP_HOUSE));
	tile_map.get(t).house.processing -= 1;
}

/**
 * Make the tile a house.
 * @param t tile index
 * @param tid Town index
 * @param counter of construction step
 * @param stage of construction (used for drawing)
 * @param type of house.  Index into house specs array
 * @param random_bits required for newgrf houses
 * @pre IsTileType(t, MP_CLEAR)
 */
static inline void MakeHouseTile(TileIndex t, TownID tid, byte counter, byte stage, HouseID type, byte random_bits)
{
	assert(IsTileType(t, MP_CLEAR));

	Tile& t_ = tile_map.init(t, MP_HOUSE);
	t_.house.random = random_bits;
	t_.house.town_id = tid;
	SetHouseType(t, type);
	SetHouseCompleted(t, stage == TOWN_HOUSE_COMPLETED);
	if (IsHouseCompleted(t))
		t_.house.age = 0;
	else {
		t_.house.tick = counter;
		t_.house.stage = stage;
	}
	SetAnimationFrame(t, 0);
	SetHouseProcessingTime(t, HouseSpec::Get(type)->processing_time);
}

#endif /* TOWN_MAP_H */
