/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file void_map.h Map accessors for void tiles. */

#ifndef VOID_MAP_H
#define VOID_MAP_H

#include "tile_map.h"

/**
 * Make a nice void tile ;)
 * @param t the tile to make void
 */
static inline void MakeVoid(TileIndex t)
{
	Tile& t_ = tile_map.init(t, MP_VOID);
}

#endif /* VOID_MAP_H */
