/*
    C-Dogs SDL
    A port of the legendary (and fun) action/arcade cdogs.
    Copyright (C) 1995 Ronny Wester
    Copyright (C) 2003 Jeremy Chin 
    Copyright (C) 2003-2007 Lucas Martin-King 

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    This file incorporates work covered by the following copyright and
    permission notice:

    Copyright (c) 2014, Cong Xu
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __PICS
#define __PICS

#include "blit.h"
#include "defs.h"
#include "pic_file.h"


#define PIC_UZIBULLET      42
#define PIC_COUNT1        398
#define PIC_COUNT2        182
#define PIC_MAX           (PIC_COUNT1 + PIC_COUNT2)
#define P2                PIC_COUNT1
#define PIC_LOGO          (P2 + 122)

#define FIREBALL_MAX    16
#define DEATH_MAX       9

#define WALL_SINGLE         0
#define WALL_LEFT           1
#define WALL_RIGHT          2
#define WALL_TOP            3
#define WALL_BOTTOM         4
#define WALL_TOPLEFT        5
#define WALL_TOPRIGHT       6
#define WALL_BOTTOMLEFT     7
#define WALL_BOTTOMRIGHT    8
#define WALL_LEFT_T         9
#define WALL_RIGHT_T       10
#define WALL_TOP_T         11
#define WALL_BOTTOM_T      12
#define WALL_VERTICAL      13
#define WALL_HORIZONTAL    14
#define WALL_CROSS         15
#define WALL_TYPES         16
const char *WallTypeStr(const int w);

typedef enum
{
	WALL_STYLE_STEEL,
	WALL_STYLE_BRICKS,
	WALL_STYLE_RED,
	WALL_STYLE_STEEL_N_WOOD,
	WALL_STYLE_STONE,
	WALL_STYLE_PLASTEEL,
	WALL_STYLE_GRANITE,
	WALL_STYLE_COUNT
} wall_style_e;
const char *WallStyleStr(const wall_style_e w);

#define FLOOR_SHADOW       0
#define FLOOR_NORMAL       1
#define FLOOR_1            2
#define FLOOR_2            3
#define FLOOR_TYPES        4
const char *FloorTypeStr(const int f);

#define ROOMFLOOR_SHADOW   0
#define ROOMFLOOR_NORMAL   1
#define ROOMFLOOR_TYPES    2
const char *RoomTypeStr(const int r);

typedef enum
{
	FLOOR_STYLE_GREEN,
	FLOOR_STYLE_PURPLE,
	FLOOR_STYLE_DIRT,
	FLOOR_STYLE_BLUE,
	FLOOR_STYLE_STRIPES,
	FLOOR_STYLE_SMALLSQUARES,
	FLOOR_STYLE_STONE,
	FLOOR_STYLE_WOOD,
	FLOOR_STYLE_WHITE,
	FLOOR_STYLE_GRID,
	FLOOR_STYLE_COUNT
} floor_style_e;
const char *FloorStyleStr(const floor_style_e f);

typedef enum
{
	ROOM_STYLE_RECESSED,
	ROOM_STYLE_TILE,
	ROOM_STYLE_DIRT,
	ROOM_STYLE_FLAT,
	ROOM_STYLE_STRIPED,
	ROOM_STYLE_SMALLSQUARES,
	ROOM_STYLE_STONE,
	ROOM_STYLE_WOOD,
	ROOM_STYLE_GRID,
	ROOM_STYLE_BIGGRID,
	ROOM_STYLE_CHECKER,
	ROOM_STYLE_COUNT
} RoomStyle;
const char *RoomStyleStr(const RoomStyle r);


struct Offset {
	int dx, dy;
};

typedef struct Offset OffsetTable[DIRECTION_COUNT];


extern TPalette origPalette;

extern const int cBodyPic[BODY_COUNT][DIRECTION_COUNT][STATE_COUNT];
extern const int cHeadPic[FACE_COUNT][DIRECTION_COUNT][STATE_COUNT+2];
extern const OffsetTable cBodyOffset[BODY_COUNT];
extern const OffsetTable cNeckOffset[BODY_COUNT];
extern const OffsetTable cGunHandOffset[BODY_COUNT];
extern const OffsetTable cHeadOffset[FACE_COUNT];
extern const TOffsetPic cGrenadePics[4];
extern const TOffsetPic cFlamePics[4];
extern const TOffsetPic cFireBallPics[FIREBALL_MAX];
#define BEAM_PIC_COUNT 2
extern const TOffsetPic cBeamPics[BEAM_PIC_COUNT][DIRECTION_COUNT];
extern const int cWallPics[WALL_STYLE_COUNT][WALL_TYPES];
extern const int cFloorPics[FLOOR_STYLE_COUNT][FLOOR_TYPES];
extern const int cRoomPics[ROOM_STYLE_COUNT][ROOMFLOOR_TYPES];
extern const struct Offset cWallOffset;
extern const TOffsetPic cDeathPics[DEATH_MAX];

#endif
