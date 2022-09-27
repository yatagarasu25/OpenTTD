/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file map_type.h Types related to maps. */

#ifndef MAP_TYPE_H
#define MAP_TYPE_H

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
		byte m5;
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
	};

	struct Town {
		uint16 id;
		byte m1;
		byte m3;
		byte m4;
		byte m5;
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
	};

	struct Bridge {
		uint16 m2;
		byte owner : 5;
		byte wc : 2;
		byte TUNNELBRIDGE : 1;
		byte m3_18;
		byte type : 6;
		byte m4_67 : 2;
		byte direction : 2;
		byte transport_type : 2;
		byte reserved : 1;
		byte m5_67 : 2;
		byte is_bridge : 1;
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
			};
			struct {
				byte axis : 1;
				byte m5_13 : 3;
				byte reserved : 1;
				byte barred : 1;
				byte m5_67 : 2;
			} crossing;
		};
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
		byte m1_7 : 1;
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
	};

	struct Station {
		uint16 id;
		byte owner : 5;
		byte wc : 2;
		byte m1_7 : 1;
		byte m3 : 4;
		byte random : 4;
		byte spec_index;
		byte gfx;
	};

	struct Depot {
		uint16 id;
		union {
			struct {
				byte m1;
				byte m3;
				byte type : 6;
				byte m4 : 2;
				byte direction : 2;
				byte m5_23 : 2;
				byte reserved : 1;
				byte m5_5 : 1;
				byte tile_type : 2;
			} road;
			struct {
				byte m1;
				byte m3;
				byte m4;
				byte direction : 2;
				byte m5_23 : 2;
				byte reserved : 1;
				byte m5_5 : 1;
				byte tile_type : 2;
			} rail;
			struct {
				byte m1;
				byte m3;
				byte m4;
				byte part : 1;
				byte axis : 1;
				byte m5_23 : 2;
				byte type : 4;
			} ship;
		};
	};

	struct Object {
		uint16 index;
		byte owner : 5;
		byte wc : 2;
		byte m1_7 : 1;
		byte bits;
		byte m4;
		byte index2;
	};

	struct Waypoint {
		uint16 station_id;
		byte m1;
		byte m3_03 : 4;
		byte m3_4 : 1;
		byte m3_57 : 3;
		byte tile_index;
		byte m5;
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
				Animated animated;

				Clear clear;
				Field field;
				Water water;
				Lock lock;
				Tree tree;
				Town town;
				House house;
				Industry industry;
				Bridge bridge;
				Road road;
				Rail rail;
				Station station;
				Depot depot;
				Object object;
				Waypoint waypoint;

				struct {
					uint16 _m2;          ///< Primarily used for indices to towns, industries and stations
					byte   _m1;          ///< Primarily used for ownership information
					byte   _m3;          ///< General purpose
					byte   _m4;          ///< General purpose
					byte   _m5;          ///< General purpose
					byte   m6;          ///< General purpose
					byte   m7;          ///< Primarily used for newgrf support
					uint16 m8;          ///< General purpose
				};
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
