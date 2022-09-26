/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file tile_map.cpp Global tile accessors. */

#include "stdafx.h"
#include "tile_map.h"

#include "safeguards.h"

/**
 * Get a tile's slope given the heigh of its four corners.
 * @param hnorth The height at the northern corner in the same unit as TileHeight.
 * @param hwest  The height at the western corner in the same unit as TileHeight.
 * @param heast  The height at the eastern corner in the same unit as TileHeight.
 * @param hsouth The height at the southern corner in the same unit as TileHeight.
 * @param[out] h The lowest height of the four corners.
 * @return The slope.
 */
static Slope GetTileSlopeGivenHeight(int hnorth, int hwest, int heast, int hsouth, int *h)
{
	/* Due to the fact that tiles must connect with each other without leaving gaps, the
	 * biggest difference in height between any corner and 'min' is between 0, 1, or 2.
	 *
	 * Also, there is at most 1 corner with height difference of 2.
	 */
	int hminnw = std::min(hnorth, hwest);
	int hmines = std::min(heast, hsouth);
	int hmin = std::min(hminnw, hmines);

	if (h != nullptr) *h = hmin;

	int hmaxnw = std::max(hnorth, hwest);
	int hmaxes = std::max(heast, hsouth);
	int hmax = std::max(hmaxnw, hmaxes);

	Slope r = SLOPE_FLAT;

	if (hnorth != hmin) r |= SLOPE_N;
	if (hwest  != hmin) r |= SLOPE_W;
	if (heast  != hmin) r |= SLOPE_E;
	if (hsouth != hmin) r |= SLOPE_S;

	if (hmax - hmin == 2) r |= SLOPE_STEEP;

	return r;
}

/**
 * Return the slope of a given tile inside the map.
 * @param tile Tile to compute slope of
 * @param h    If not \c nullptr, pointer to storage of z height
 * @return Slope of the tile, except for the HALFTILE part
 */
Slope GetTileSlope(TileIndex tile, int *h)
{
	uint x1 = TileX(tile);
	uint y1 = TileY(tile);
	uint x2 = std::min(x1 + 1, MapMaxX());
	uint y2 = std::min(y1 + 1, MapMaxY());

	int hnorth = TileHeight(tile);           // Height of the North corner.
	int hwest  = TileHeight(TileXY(x2, y1)); // Height of the West corner.
	int heast  = TileHeight(TileXY(x1, y2)); // Height of the East corner.
	int hsouth = TileHeight(TileXY(x2, y2)); // Height of the South corner.

	return GetTileSlopeGivenHeight(hnorth, hwest, heast, hsouth, h);
}

/**
 * Return the slope of a given tile, also for tiles outside the map (virtual "black" tiles).
 *
 * @param x X coordinate of the tile to compute slope of, may be outside the map.
 * @param y Y coordinate of the tile to compute slope of, may be outside the map.
 * @param h If not \c nullptr, pointer to storage of z height.
 * @return Slope of the tile, except for the HALFTILE part.
 */
Slope GetTilePixelSlopeOutsideMap(int x, int y, int *h)
{
	int hnorth = TileHeightOutsideMap(x,     y);     // N corner.
	int hwest  = TileHeightOutsideMap(x + 1, y);     // W corner.
	int heast  = TileHeightOutsideMap(x,     y + 1); // E corner.
	int hsouth = TileHeightOutsideMap(x + 1, y + 1); // S corner.

	Slope s = GetTileSlopeGivenHeight(hnorth, hwest, heast, hsouth, h);
	if (h != nullptr) *h *= TILE_HEIGHT;
	return s;
}

/**
 * Check if a given tile is flat
 * @param tile Tile to check
 * @param h If not \c nullptr, pointer to storage of z height (only if tile is flat)
 * @return Whether the tile is flat
 */
bool IsTileFlat(TileIndex tile, int *h)
{
	uint x1 = TileX(tile);
	uint y1 = TileY(tile);
	uint x2 = std::min(x1 + 1, MapMaxX());
	uint y2 = std::min(y1 + 1, MapMaxY());

	uint z = TileHeight(tile);
	if (TileHeight(TileXY(x2, y1)) != z) return false;
	if (TileHeight(TileXY(x1, y2)) != z) return false;
	if (TileHeight(TileXY(x2, y2)) != z) return false;

	if (h != nullptr) *h = z;
	return true;
}

/**
 * Get bottom height of the tile
 * @param tile Tile to compute height of
 * @return Minimum height of the tile
 */
int GetTileZ(TileIndex tile)
{
	uint x1 = TileX(tile);
	uint y1 = TileY(tile);
	uint x2 = std::min(x1 + 1, MapMaxX());
	uint y2 = std::min(y1 + 1, MapMaxY());

	return std::min({
		TileHeight(tile),           // N corner
		TileHeight(TileXY(x2, y1)), // W corner
		TileHeight(TileXY(x1, y2)), // E corner
		TileHeight(TileXY(x2, y2)), // S corner
	});
}

/**
 * Get top height of the tile inside the map.
 * @param t Tile to compute height of
 * @return Maximum height of the tile
 */
int GetTileMaxZ(TileIndex t)
{
	uint x1 = TileX(t);
	uint y1 = TileY(t);
	uint x2 = std::min(x1 + 1, MapMaxX());
	uint y2 = std::min(y1 + 1, MapMaxY());

	return std::max({
		TileHeight(t),              // N corner
		TileHeight(TileXY(x2, y1)), // W corner
		TileHeight(TileXY(x1, y2)), // E corner
		TileHeight(TileXY(x2, y2)), // S corner
	});
}













#include "station_map.h"


struct OwnedTile : public TileCore
{
	Owner owner; // 0, 5, owner; !!!
};

struct StationTile : public OwnedTile
{
	StationID sid; // m2
	uint16 m3m4;
	byte section;

	/*
	SB(_me[t].m6, 2, 1, 0);
	SB(_me[t].m6, 3, 3, st);
	_me[t].m7 = 0;
	_me[t].m8 = 0;
	*/
};

void MakeStation(TileIndex t, Owner o, StationID sid, StationType st, byte section, WaterClass wc)
{
	StationTile& st_ = (StationTile&)(_m[t]);
	SetTileType(t, MP_STATION);
	SetTileOwner(t, o);
	SetWaterClass(t, wc);
	SetDockingTile(t, false);
	_m[t].m2 = sid;
	_m[t].m3 = 0;
	_m[t].m4 = 0;
	_m[t].m5 = section;
	SB(_me[t].m6, 2, 1, 0);
	SB(_me[t].m6, 3, 3, st);
	_me[t].m7 = 0;
	_me[t].m8 = 0;
}
