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
struct Tile : public TileCore {
	union {
		struct {
			uint16 m2;          ///< Primarily used for indices to towns, industries and stations
			union
			{
				byte   m1;          ///< Primarily used for ownership information
				struct {
					byte owner : 5;
					byte wc : 2;
					byte TUNNELBRIDGE : 1;
				};
				struct {
					byte owner : 5;
					byte wc : 2;
					byte is_docking : 1;
				} water;
			};
			byte   m3;          ///< General purpose
			byte   m4;          ///< General purpose
			byte   m5;          ///< General purpose
		};
		struct {
			uint16 industry_id;
			byte   m1;
			byte   field_type : 4;
			byte   is_snow : 1;
		} tile;
		struct {
			byte counter : 4;
			byte density : 2;
			byte ground : 3;
			byte b : 3;
			byte type;
		} tree;
		struct {
			uint16 id;
		} town;
		struct {
			uint16 town_id;
			byte random;
			byte triggers : 5;
			byte m3 : 1;
			byte clean_house_flag : 1;
			byte is_completed : 1;
		} house;
		struct {
			uint16 id;
			byte construction_stage : 2;
			byte construction_counter : 2;
			byte wc2 : 1;
			byte wc : 2;
			byte is_completed : 1;
			byte bits;
		} industry;
		struct {
			uint16 m2;
			byte owner : 5;
			byte m1 : 3;
			byte tram_bits : 4;
			byte tram_owner : 4;
		} road;
		struct {
			struct {
				byte signal_ul : 3;
				byte signal_ul_variant : 1;
				byte signal_lr : 3;
				byte signal_lr_variant : 1;
				byte bits : 3;
				byte m2_11 : 1;
				byte m2_12 : 4;
			};
			byte m1;
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
		} track;
		struct {
			uint16 id;
			byte m1;
			byte m3 : 4;
			byte random : 4;
			byte m4;
			byte section;
		} station;
		struct {
			uint16 id;
			union {
				struct {
					byte m1;
					byte m3;
					byte m4;
					byte m5;
				} road;
				struct {
					byte m1;
					byte m3;
					byte m4;
					byte m5;
				} rail;
				struct {
					byte m1;
					byte m3;
					byte m4;
					byte m5;
				} ship;
			};
		} depot;
		struct {
			uint16 index;
			byte m1;
			byte bits;
		} object;
		struct {
			uint16 station_id;
			byte m1;
			byte m3 : 4;
			byte m3_4 : 1;
			byte m3_5 : 3;
			byte tile_index;
		} waypoint;
	};

	void init()
	{
		m2 = 0;
		m1 = 0;
		m3 = 0;
		m4 = 0;
		m5 = 0;
	}
};

static_assert(sizeof(Tile) == 8);

/**
 * Data that is stored per tile. Also used Tile for this.
 * Look at docs/landscape.html for the exact meaning of the members.
 */
struct TileExtended {
	byte m6;   ///< General purpose
	byte m7;   ///< Primarily used for newgrf support
	uint16 m8; ///< General purpose
};

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
