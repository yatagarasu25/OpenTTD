/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file map_type.h Types related to maps. */

#ifndef MAP_TYPE_H
#define MAP_TYPE_H

#include "rail_type.h"
#include "road_type.h"
#include "water_type.h"

struct TileCore {
	union {
		struct {
			byte _type;
		};
		struct {
			byte zone : 2;      ///< rainforest/desert (0..1)
			byte above_x : 1;   ///< bridges (2..3)
			byte above_y : 1;
			byte type : 4;     ///< The type (bits 4..7)
		};
		struct {
			byte zone : 2;      ///< rainforest/desert (0..1)
			byte above : 2;     ///< rainforest/desert (0..1)
			byte type : 4;     ///< The type (bits 4..7)
		};
	};
	byte   height;      ///< The height of the northern corner.

	void SetType(TileType type)
	{
		this->type = type;
	}
};
static_assert(sizeof(TileCore) == 2);

/**
 * Data that is stored per tile. Also used TileExtended for this.
 * Look at docs/landscape.html for the exact meaning of the members.
 */
struct Tile {
	struct Raw {
		byte type;          ///< rainforest/desert (0..1), bridges (2..3), The type (bits 4..7)
		byte height;        ///< The height of the northern corner.
		uint16 m2;          ///< Primarily used for indices to towns, industries and stations
		byte   m1;          ///< Primarily used for ownership information
		byte   m3;          ///< General purpose
		byte   m4;          ///< General purpose
		byte   m5;          ///< General purpose
		byte   m6;          ///< General purpose
		byte   m7;          ///< Primarily used for newgrf support
		uint16 m8;          ///< General purpose
	};

	struct Owned {
		uint16 m2;
		byte owner : 5;
	};

	struct Animated {
		uint16 m2;
		byte   m1;
		byte   m3;
		byte   m4;
		byte   m5;
		byte   m6;
		byte   frame;
		uint16 m8;
	};

	struct WaterClass {
		uint16 m2;
		byte owner : 5;
		byte wc : 2;
	};

	struct Docking {
		uint16 m2;
		byte owner : 5;
		byte wc : 2;
		byte is_docking : 1;
	};


	struct Clear {
		uint16 industry_id;
		byte m1;
		byte field_type : 4;
		byte is_snow : 1;
		byte m3_57 : 3;
		byte m4_12 : 2;
		byte fence_se : 3;
		byte fence_sw : 3;
		byte density : 2;
		byte ground_type : 3;
		byte counter : 3;
		byte m6;
		byte m7;
		uint16 m8;
	};

	struct Field {
		uint16 industry_id;
		byte m1;
		byte type : 4;
		byte is_snow : 1;
		byte fence_ne : 3;
		byte m4_12 : 2;
		byte fence_se : 3;
		byte fence_sw : 3;
		byte density : 2;
		byte ground_type : 3;
		byte counter : 3;
		byte m6_01 : 2;
		byte fence_nw : 3;
		byte m6_57 : 3;
		byte m7;
		uint16 m8;
	};

	struct Water {
		uint16 m2;
		byte owner : 5;
		byte wc : 2;
		byte is_docking : 1;
		byte m3;
		byte bits;
		byte is_coast : 1;
		byte m5_1 : 1;
		byte lock : 2;
		byte type : 4;
		byte m6;
		byte m7;
		uint16 m8;

		inline WaterTileType GetWaterTileType()
		{
			switch (type) {
				case WBL_TYPE_NORMAL: return is_coast ? WATER_TILE_COAST : WATER_TILE_CLEAR;
				case WBL_TYPE_LOCK:   return WATER_TILE_LOCK;
				case WBL_TYPE_DEPOT:  return WATER_TILE_DEPOT;
				default: NOT_REACHED();
			}
		}
	};

	struct Lock {
		uint16 m2;
		byte owner : 5;
		byte wc : 2;
		byte is_docking : 1;
		byte m3;
		byte bits;
		byte direction : 2;
		byte part : 2;
		byte type : 4;
		byte m6;
		byte m7;
		uint16 m8;
	};

	struct Tree {
		byte counter : 4;
		byte density : 2;
		byte ground : 3;
		byte m2_1415 : 5;
		byte owner : 5;
		byte wc : 2;
		byte m1_7 : 1;
		byte type;
		byte m4_12 : 2;
		byte fence_se : 3;
		byte fence_sw : 3;
		byte growth : 6;
		byte count : 2;
		byte m6;
		byte m7;
		uint16 m8;
	};

	struct Town {
		uint16 id;
		byte m1;
		byte m3;
		byte m4;
		byte m5;
		byte m6;
		byte m7;
		uint16 m8;
	};

	struct House {
		uint16 town_id;
		byte random;
		byte triggers : 5;
		byte m3_5 : 1;
		byte clean_house_flag : 1;
		byte is_completed : 1;
		union {
			byte house_id;
			byte old_town_id;
		};
		union {
			byte age;
			struct {
				byte tick : 3;
				byte stage : 2;
				byte m5_57 : 3;
			};
			byte tick_and_stage : 5;
		};
		union {
			struct {
				byte m6_01 : 2;
				byte position : 6;
			} lift;
			struct {
				byte m6_01 : 2;
				byte processing : 6;
			};
		};
		byte has_destination : 1;
		byte destination : 3;
		byte m7_17 : 4;
		uint16 m8;
	};

	struct Industry {
		uint16 id;
		byte construction_stage : 2;
		byte construction_counter : 2;
		byte m1_5 : 1;
		byte wc : 2;
		byte is_completed : 1;
		byte bits;
		byte animation_loop;
		byte gfx;
		byte m6_01 : 2;
		byte gfx_bit : 1;
		byte triggers : 3;
		byte m6_67 : 2;
		byte m7;
		uint16 m8;
	};

	struct Bridge {
		uint16 m2;
		byte owner : 5;
		byte wc : 2;
		byte is_docking : 1;
		byte m3_18;
		byte old_road_type : 6;
		byte m4_67 : 2;
		byte direction : 2;
		byte transport_type : 2;
		byte reserved : 1;
		byte m5_67 : 2;
		byte is_bridge : 1;
		byte m6_01 : 2;
		byte type : 4;
		byte m6_67 : 2;
		byte m7_04 : 4;
		byte snow_or_desert : 1;
		byte m7_57 : 3;
		byte rail_type : 6;
		byte road_type : 6;
		uint16 m8_1215 : 4;
	};

	struct Road {
		uint16 town_id;
		byte owner : 5;
		byte wc : 2;
		byte m1_7 : 1;
		byte tram_bits : 4;
		byte tram_owner : 4;
		byte type : 6;
		byte m4_67 : 2;
		union {
			struct {
				byte road_bits : 4;
				byte disalowed_direction : 2;
				byte tile_type : 2;
				byte m6_02 : 3;
				byte roadside : 3;
				byte m6_67 : 2;
				byte road_works_counter : 4;
				byte m7_4 : 1;
				byte on_snow : 1;
				byte m7_67 : 2;
			};
			struct {
				byte axis : 1;
				byte m5_13 : 3;
				byte reserved : 1;
				byte barred : 1;
				byte m5_67 : 2;
				byte m6_02 : 3;
				byte roadside : 3;
				byte m6_67 : 2;
				byte road_type;
			} crossing;
		};
		byte rail_type : 6;
		byte road_type : 6;
		byte m8_1215 : 4;
	};

	struct Rail {
		struct {
			byte signal_ul : 3;
			byte signal_ul_variant : 1;
			byte signal_lr : 3;
			byte signal_lr_variant : 1;
			byte bits : 3;
			byte m2_11 : 1;
			byte m2_1215 : 4;
		};
		byte owner : 5;
		byte wc : 2;
		byte is_docking : 1;
		union {
			byte m3;
			struct {
				byte m3 : 4;
				byte signal_side_lr : 2;
				byte signal_side_ul : 2;
			};
			struct {
				byte m3 : 4;
				byte signals_present : 4;
			};
		};
		byte ground_type : 4;
		byte signal_states : 4;
		byte track_bits : 6;
		byte tile_type : 2;
		byte m6;
		byte m7;
		byte rail_type : 6;
		uint16 m8_615 : 10;
	};

	struct Station {
		uint16 id;
		byte owner : 5;
		byte wc : 2;
		byte is_docking : 1;
		byte m3 : 4;
		byte random : 4;
		byte spec_index;
		byte gfx;
		byte m6_01 : 2;
		byte resereved : 1;
		byte type : 3;
		byte m6_67 : 2;
		byte m7;
		byte rail_type : 6;
		uint16 m8_615 : 10;
	};

	struct Depot {
		uint16 id;
	};

	struct DepotRoad : public Depot {
		byte m1;
		byte m3;
		byte type : 6;
		byte m4 : 2;
		byte direction : 2;
		byte m5_23 : 2;
		byte reserved : 1;
		byte m5_5 : 1;
		byte tile_type : 2;
		byte m6;
		byte owner : 5;
		byte on_snow : 1;
		byte m7_67 : 2;
		byte rail_type : 6;
		byte road_type : 6;
		byte m8_1215 : 4;
	};

	struct DepotRail : public Depot {
		byte m1;
		byte m3;
		byte m4;
		byte direction : 2;
		byte m5_23 : 2;
		byte reserved : 1;
		byte m5_5 : 1;
		byte tile_type : 2;
		byte m6;
		byte m7;
		byte rail_type : 6;
		byte road_type : 6;
		byte m8_1215 : 4;
	};
	struct DepotShip : public Depot {
		byte owner : 5;
		byte wc : 2;
		byte is_docking : 1;
		byte m3;
		byte m4;
		byte part : 1;
		byte axis : 1;
		byte m5_23 : 2;
		byte type : 4;
		byte m6;
		byte m7;
		uint16 m8;
	};

	struct Object {
		uint16 index;
		byte owner : 5;
		byte wc : 2;
		byte m1_7 : 1;
		byte bits;
		byte m4;
		byte index2;
		byte m6;
		byte m7;
		uint16 m8;
	};

	struct Waypoint {
		uint16 station_id;
		byte m1;
		byte m3_03 : 4;
		byte m3_4 : 1;
		byte m3_57 : 3;
		byte tile_index;
		byte m5;
		byte m6_02 : 3;
		byte station_type : 3;
		byte m7;
		uint16 m8;
	};

	union {
		uint64 all;
		Raw raw;
		struct {
			struct {
				union {
					struct {
						byte zone : 2;
						byte above_x : 1;
						byte above_y : 1;
						byte type : 4;
					};
					struct {
						byte zone : 2;
						byte above : 2;
						byte type : 4;
					};
				};
				byte height;
			};
			union {
				Owned owned_;
				Animated animated_;
				WaterClass water_class_;
				Docking docking_;

				Clear clear_;
				Field field_;
				Water water_;
				Lock lock_;
				Tree tree_;
				Town town_;
				House house_;
				Industry industry_;
				Bridge bridge_;
				Road road_;
				Rail rail_;
				Station station_;
				DepotRoad depot_road_;
				DepotRail depot_rail_;
				DepotShip depot_ship_;
				Object object_;
				Waypoint waypoint_;
			};
		};
	};

	void init()
	{
		raw.m2 = 0;
		raw.m1 = 0;
		raw.m3 = 0;
		raw.m4 = 0;
		raw.m5 = 0;
		raw.m6 = 0;
		raw.m7 = 0;
		raw.m8 = 0;
	}

	Owned& owned()
	{
		assert(type != MP_VOID && type != MP_HOUSE && type != MP_INDUSTRY);

		return owned_;
	}

	Animated& animated()
	{
		assert(type == MP_HOUSE || type == MP_OBJECT || type == MP_INDUSTRY || type == MP_STATION);

		return animated_;
	}

	bool IsWaterClass() const { return type == MP_WATER || type == MP_STATION || type == MP_INDUSTRY || type == MP_OBJECT || type == MP_TREES; }
	WaterClass& water_class()
	{
		assert(IsWaterClass());

		return water_class_;
	}

	bool IsDocking() const { return type == MP_WATER || type == MP_RAILWAY || type == MP_STATION || type == MP_TUNNELBRIDGE; }
	Docking& docking()
	{
		assert(IsDocking());

		return docking_;
	}

	Clear& clear()
	{
		assert(type == MP_CLEAR);

		return clear_;
	}

	Field& field()
	{
		return field_;
	}

	Water& water()
	{
		assert(type == MP_WATER);

		return water_;
	}

	bool IsLock() { return water().GetWaterTileType() == WATER_TILE_LOCK; }
	Lock& lock()
	{
		assert(IsLock());

		return lock_;
	}

	Tree& tree()
	{
		assert(type == MP_TREES);

		return tree_;
	}

	Town& town()
	{
		assert(type == MP_HOUSE || (type == MP_ROAD && road_.tile_type != ROAD_TILE_DEPOT));

		return town_;
	}

	House& house()
	{
		assert(type == MP_HOUSE);

		return house_;
	}

	Industry& industry()
	{
		assert(type == MP_INDUSTRY);

		return industry_;
	}

	Bridge& bridge()
	{
		assert(type == MP_TUNNELBRIDGE);

		return bridge_;
	}

	Road& road()
	{
		//assert(IsTileType(t, MP_ROAD));
		assert((type == MP_ROAD || type == MP_STATION || type == MP_TUNNELBRIDGE));

		return road_;
	}

	Rail& rail()
	{
		//assert(IsTileType(t, MP_RAILWAY));
		assert((type == MP_RAILWAY || type == MP_STATION || type == MP_TUNNELBRIDGE));

		return rail_;
	}

	Station& station()
	{
		assert(type == MP_STATION);

		return station_;
	}

	Depot& depot()
	{
		assert((type == MP_ROAD && road_.tile_type == ROAD_TILE_DEPOT)
			|| (type == MP_RAILWAY && rail_.tile_type == RAIL_TILE_DEPOT)
			|| (type == MP_WATER && water_.type == WBL_TYPE_DEPOT));

		return depot_road_;
	}

	DepotRoad& depot_road()
	{
		//assert(type == MP_ROAD && road_.tile_type == ROAD_TILE_DEPOT);

		return depot_road_;
	}

	DepotRail& depot_rail()
	{
		assert(type == MP_RAILWAY && rail_.tile_type == RAIL_TILE_DEPOT);

		return depot_rail_;
	}

	DepotShip& depot_ship()
	{
		assert(type == MP_WATER && water_.type == WBL_TYPE_DEPOT);

		return depot_ship_;
	}

	Object& object()
	{
		return object_;
	}

	Waypoint& waypoint()
	{
		return waypoint_;
	}
};

static_assert(sizeof(Tile) == 16);


/**
 * An offset value between two tiles.
 *
 * This value is used for the difference between
 * two tiles. It can be added to a TileIndex to get
 * the resulting TileIndex of the start tile applied
 * with this saved difference.
 *
 * @see TileDiffXY(int, int)
 */
typedef int32 TileIndexDiff;

/**
 * A pair-construct of a TileIndexDiff.
 *
 * This can be used to save the difference between to
 * tiles as a pair of x and y value.
 */
struct TileIndexDiffC {
	int16 x;        ///< The x value of the coordinate
	int16 y;        ///< The y value of the coordinate
};

/** Minimal and maximal map width and height */
static const uint MIN_MAP_SIZE_BITS = 6;                      ///< Minimal size of map is equal to 2 ^ MIN_MAP_SIZE_BITS
static const uint MAX_MAP_SIZE_BITS = 12;                     ///< Maximal size of map is equal to 2 ^ MAX_MAP_SIZE_BITS
static const uint MIN_MAP_SIZE      = 1 << MIN_MAP_SIZE_BITS; ///< Minimal map size = 64
static const uint MAX_MAP_SIZE      = 1 << MAX_MAP_SIZE_BITS; ///< Maximal map size = 4096

/**
 * Approximation of the length of a straight track, relative to a diagonal
 * track (ie the size of a tile side).
 *
 * \#defined instead of const so it can
 * stay integer. (no runtime float operations) Is this needed?
 * Watch out! There are _no_ brackets around here, to prevent intermediate
 * rounding! Be careful when using this!
 * This value should be sqrt(2)/2 ~ 0.7071
 */
#define STRAIGHT_TRACK_LENGTH 7071/10000

/** Argument for CmdLevelLand describing what to do. */
enum LevelMode : byte {
	LM_LEVEL, ///< Level the land.
	LM_LOWER, ///< Lower the land.
	LM_RAISE, ///< Raise the land.
};

#endif /* MAP_TYPE_H */
