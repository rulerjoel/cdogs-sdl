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

    Copyright (c) 2013, Cong Xu
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
#include "weapon.h"

#include <assert.h>

#include "config.h"
#include "objs.h"
#include "sounds.h"

GunDescription gGunDescriptions[GUN_COUNT];

#define RELOAD_DISTANCE_PLUS 300

const TOffsetPic cGunPics[GUNPIC_COUNT][DIRECTION_COUNT][GUNSTATE_COUNT] = {
	{
	 {{-2, -10, 86}, {-3, -8, 78}, {-3, -7, 78}},
	 {{-2, -10, 87}, {-2, -9, 79}, {-3, -8, 79}},
	 {{0, -12, 88}, {0, -5, 80}, {-1, -5, 80}},
	 {{-2, -9, 90}, {0, -2, 81}, {-1, -3, 81}},
	 {{-2, -9, 90}, {-1, -2, 82}, {-1, -3, 82}},
	 {{-6, -10, 91}, {-7, -4, 83}, {-6, -5, 83}},
	 {{-8, -11, 92}, {-12, -6, 84}, {-11, -6, 84}},
	 {{-6, -14, 93}, {-8, -12, 85}, {-7, -11, 85}}
	 },
	{
	 {{-1, -7, 142}, {-1, -7, 142}, {-1, -7, 142}},
	 {{-1, -7, 142}, {-1, -7, 142}, {-1, -7, 142}},
	 {{-2, -8, 143}, {-2, -8, 143}, {-2, -8, 143}},
	 {{-3, -5, 144}, {-3, -5, 144}, {-3, -5, 144}},
	 {{-3, -5, 144}, {-3, -5, 144}, {-3, -5, 144}},
	 {{-3, -5, 144}, {-3, -5, 144}, {-3, -5, 144}},
	 {{-8, -10, 145}, {-8, -10, 145}, {-8, -10, 145}},
	 {{-8, -10, 145}, {-8, -10, 145}, {-8, -10, 145}}
	 }
};

const OffsetTable cMuzzleOffset[GUNPIC_COUNT] = {
	{
	 {2, 0},
	 {7, 2},
	 {13, 2},
	 {7, 6},
	 {2, 6},
	 {2, 6},
	 {0, 2},
	 {2, 2}
	 }
};

// Initialise all the static weapon data
// TODO: load from data file
void WeaponInitialize(void)
{
	GunDescription *g;
	int i;

	// Load defaults
	for (i = 0; i < GUN_COUNT; i++)
	{
		g = &gGunDescriptions[i];
		g->pic = GUNPIC_BLASTER;
		g->ReloadLead = -1;
		g->ReloadSound = -1;
		g->SoundLockLength = 0;
		g->Recoil = 0;
		g->Spread.Count = 1;
		g->Spread.Width = 0;
		g->MuzzleHeight = BULLET_Z;
	}

	g = &gGunDescriptions[GUN_KNIFE];
	g->pic = GUNPIC_KNIFE;
	strcpy(g->name, "Knife");
	g->Cost = 0;
	g->Lock = 0;
	g->Sound = -1;
	g->SoundLockLength = 10;

	g = &gGunDescriptions[GUN_MG];
	strcpy(g->name, "Machine gun");
	g->Bullet = BULLET_MG;
	g->Cost = 1;
	g->Lock = 6;
	g->Sound = SND_MACHINEGUN;
	g->Recoil = 7.0 / 256 * 2 * PI;

	g = &gGunDescriptions[GUN_GRENADE];
	g->pic = -1;
	strcpy(g->name, "Grenades");
	g->Bullet = BULLET_GRENADE;
	g->Cost = 20;
	g->Lock = 30;
	g->Sound = SND_LAUNCH;

	g = &gGunDescriptions[GUN_FLAMER];
	strcpy(g->name, "Flamer");
	g->Bullet = BULLET_FLAME;
	g->Cost = 1;
	g->Lock = 6;
	g->Sound = SND_FLAMER;
	g->SoundLockLength = 36;

	g = &gGunDescriptions[GUN_SHOTGUN];
	strcpy(g->name, "Shotgun");
	g->Bullet = BULLET_SHOTGUN;
	g->Cost = 5;
	g->Lock = 50;
	g->ReloadLead = 10;
	g->Sound = SND_SHOTGUN;
	g->ReloadSound = SND_SHOTGUN_R;
	g->Spread.Count = 5;
	g->Spread.Width = 2 * PI / 32;

	g = &gGunDescriptions[GUN_POWERGUN];
	strcpy(g->name, "Powergun");
	g->Bullet = BULLET_LASER;
	g->Cost = 2;
	g->Lock = 20;
	g->Sound = SND_POWERGUN;

	g = &gGunDescriptions[GUN_FRAGGRENADE];
	g->pic = -1;
	strcpy(g->name, "Shrapnel bombs");
	g->Bullet = BULLET_SHRAPNELBOMB;
	g->Cost = 20;
	g->Lock = 30;
	g->Sound = SND_LAUNCH;

	g = &gGunDescriptions[GUN_MOLOTOV];
	g->pic = -1;
	strcpy(g->name, "Molotovs");
	g->Bullet = BULLET_MOLOTOV;
	g->Cost = 20;
	g->Lock = 30;
	g->Sound = SND_LAUNCH;

	g = &gGunDescriptions[GUN_SNIPER];
	strcpy(g->name, "Sniper rifle");
	g->Bullet = BULLET_SNIPER;
	g->Cost = 5;
	g->Lock = 100;
	g->ReloadLead = 15;
	g->Sound = SND_LASER;
	g->ReloadSound = SND_LASER_R;

	g = &gGunDescriptions[GUN_MINE];
	g->pic = -1;
	strcpy(g->name, "Prox. mine");
	g->Bullet = BULLET_PROXMINE;
	g->Cost = 10;
	g->Lock = 100;
	g->ReloadLead = 15;
	g->Sound = SND_HAHAHA;
	g->ReloadSound = SND_PACKAGE_R;
	g->MuzzleHeight = 0;

	g = &gGunDescriptions[GUN_DYNAMITE];
	g->pic = -1;
	strcpy(g->name, "Dynamite");
	g->Bullet = BULLET_DYNAMITE;
	g->Cost = 7;
	g->Lock = 100;
	g->ReloadLead = 15;
	g->Sound = SND_HAHAHA;
	g->ReloadSound = SND_PACKAGE_R;
	g->MuzzleHeight = 0;

	g = &gGunDescriptions[GUN_GASBOMB];
	g->pic = -1;
	strcpy(g->name, "Chemo bombs");
	g->Bullet = BULLET_GASBOMB;
	g->Cost = 7;
	g->Lock = 30;
	g->Sound = SND_LAUNCH;

	g = &gGunDescriptions[GUN_PETRIFY];
	strcpy(g->name, "Petrify gun");
	g->Bullet = BULLET_PETRIFIER;
	g->Cost = 10;
	g->Lock = 100;
	g->ReloadLead = 15;
	g->Sound = SND_LASER;
	g->ReloadSound = SND_LASER_R;

	g = &gGunDescriptions[GUN_BROWN];
	strcpy(g->name, "Browny gun");
	g->Bullet = BULLET_BROWN;
	g->Cost = 5;
	g->Lock = 30;
	g->Sound = SND_POWERGUN;

	g = &gGunDescriptions[GUN_CONFUSEBOMB];
	g->pic = -1;
	strcpy(g->name, "Confusion bombs");
	g->Bullet = BULLET_CONFUSEBOMB;
	g->Cost = 10;
	g->Lock = 30;
	g->Sound = SND_LAUNCH;

	g = &gGunDescriptions[GUN_GASGUN];
	strcpy(g->name, "Chemo gun");
	g->Cost = 1;
	g->Lock = 6;
	g->Sound = SND_FLAMER;
	g->SoundLockLength = 36;

	g = &gGunDescriptions[GUN_PULSERIFLE];
	strcpy(g->name, "Pulse Rifle");
	g->Bullet = BULLET_RAPID;
	g->Cost = 1;
	g->Lock = 4;
	g->Sound = SND_MINIGUN;
	g->Recoil = 15.0 / 256 * 2 * PI;

	g = &gGunDescriptions[GUN_HEATSEEKER];
	strcpy(g->name, "Heatseeker");
	g->Bullet = BULLET_HEATSEEKER;
	g->Cost = 7;
	g->Lock = 30;
	g->Sound = SND_LAUNCH;
}

Weapon WeaponCreate(gun_e gun)
{
	Weapon w;
	w.gun = gun;
	w.state = GUNSTATE_READY;
	w.lock = 0;
	w.soundLock = 0;
	w.stateCounter = -1;
	return w;
}

gunpic_e GunGetPic(gun_e gun)
{
	return gGunDescriptions[gun].pic;
}

const char *GunGetName(gun_e gun)
{
	return gGunDescriptions[gun].name;
}
gun_e StrGunName(const char *s)
{
	gun_e i;
	for (i = 0; i < GUN_COUNT; i++)
	{
		if (strcmp(s, GunGetName(i)) == 0)
		{
			return i;
		}
	}
	assert(0 && "cannot parse gun name");
	return GUN_KNIFE;
}

Vec2i GunGetMuzzleOffset(gun_e gun, direction_e dir, int isArmed)
{
	Vec2i position;
	gunpic_e g = GunGetPic(gun);
	position.x =
		cGunHandOffset[isArmed][dir].dx +
		cGunPics[g][dir][GUNSTATE_FIRING].dx +
		cMuzzleOffset[g][dir].dx;
	position.y =
		cGunHandOffset[isArmed][dir].dy +
		cGunPics[g][dir][GUNSTATE_FIRING].dy +
		cMuzzleOffset[g][dir].dy + BULLET_Z;
	return Vec2iScale(position, 256);
}

void WeaponSetState(Weapon *w, gunstate_e state);

void WeaponUpdate(Weapon *w, int ticks, Vec2i tilePosition)
{
	// Reload sound
	if (gConfig.Sound.Reloads &&
		w->lock > gGunDescriptions[w->gun].ReloadLead &&
		w->lock - ticks <= gGunDescriptions[w->gun].ReloadLead &&
		w->lock > 0 &&
		(int)gGunDescriptions[w->gun].ReloadSound != -1)
	{
		SoundPlayAtPlusDistance(
			&gSoundDevice,
			gGunDescriptions[w->gun].ReloadSound,
			tilePosition,
			RELOAD_DISTANCE_PLUS);
	}
	w->lock -= ticks;
	if (w->lock < 0)
	{
		w->lock = 0;
	}
	w->soundLock -= ticks;
	if (w->soundLock < 0)
	{
		w->soundLock = 0;
	}
	if (w->stateCounter >= 0)
	{
		w->stateCounter = MAX(0, w->stateCounter - ticks);
		if (w->stateCounter == 0)
		{
			switch (w->state)
			{
			case GUNSTATE_FIRING:
				WeaponSetState(w, GUNSTATE_RECOIL);
				break;
			case GUNSTATE_RECOIL:
				WeaponSetState(w, GUNSTATE_READY);
				break;
			default:
				assert(0);
				break;
			}
		}
	}
}

int GunGetCost(gun_e gun)
{
	return gGunDescriptions[gun].Cost;
}

int WeaponCanFire(Weapon *w)
{
	return w->lock <= 0;
}

void WeaponPlaySound(Weapon *w, Vec2i tilePosition)
{
	if (w->soundLock <= 0 && (int)gGunDescriptions[w->gun].Sound != -1)
	{
		SoundPlayAt(
			&gSoundDevice,
			gGunDescriptions[w->gun].Sound,
			tilePosition);
		w->soundLock = gGunDescriptions[w->gun].SoundLockLength;
	}
}

void WeaponFire(
	Weapon *w, direction_e d, Vec2i muzzlePosition, Vec2i tilePosition,
	int flags, int player)
{
	double radians = dir2radians[d];
	GunDescription *desc = &gGunDescriptions[w->gun];
	int spreadCount = desc->Spread.Count;
	double spreadStartAngle = 0;
	double spreadWidth = desc->Spread.Width;
	if (spreadCount > 1)
	{
		// Find the starting angle of the spread (clockwise)
		// Keep in mind the fencepost problem, i.e. spread of 3 means a
		// total spread angle of 2x width
		spreadStartAngle = -(spreadCount - 1) * spreadWidth / 2;
	}
	
	assert(WeaponCanFire(w));
	for (int i = 0; i < spreadCount; i++)
	{
		double spreadAngle = spreadStartAngle + i * spreadWidth;
		double recoil = 0;
		if (desc->Recoil > 0)
		{
			recoil =
				((double)rand() / RAND_MAX * desc->Recoil) - desc->Recoil / 2;
		}
		double finalAngle = radians + spreadAngle + recoil;
		switch (w->gun)
		{
		case GUN_KNIFE:
			// Do nothing
			break;

		case GUN_MG:			// fallthrough
		case GUN_SHOTGUN:		// fallthrough
		case GUN_BROWN:			// fallthrough
		case GUN_MINE:			// fallthrough
		case GUN_DYNAMITE:		// fallthrough
		case GUN_PULSERIFLE:	// fallthrough
		case GUN_HEATSEEKER:
			AddBullet(
				muzzlePosition, desc->MuzzleHeight, finalAngle,
				desc->Bullet, flags, player);
			break;

		case GUN_GRENADE:		// fallthrough
		case GUN_FRAGGRENADE:	// fallthrough
		case GUN_MOLOTOV:		// fallthrough
		case GUN_GASBOMB:		// fallthrough
		case GUN_CONFUSEBOMB:
			AddGrenade(
				muzzlePosition, desc->MuzzleHeight, finalAngle,
				desc->Bullet, flags, player);
			break;

		case GUN_FLAMER:	// fallthrough
		case GUN_PETRIFY:
			AddBulletBig(
				muzzlePosition, desc->MuzzleHeight, finalAngle,
				desc->Bullet, flags, player);
			break;

		case GUN_POWERGUN:	// fallthrough
		case GUN_SNIPER:
			AddBulletDirectional(
				muzzlePosition, desc->MuzzleHeight, d,
				desc->Bullet, flags, player);
			break;

		case GUN_GASGUN:
			AddGasCloud(
				muzzlePosition, desc->MuzzleHeight, finalAngle, 384, 35,
				flags, SPECIAL_POISON, player);
			break;

		default:
			// unknown gun?
			assert(0);
			break;
		}
	}

	w->lock = gGunDescriptions[w->gun].Lock;
	WeaponPlaySound(w, tilePosition);
	if (w->state != GUNSTATE_FIRING && w->state != GUNSTATE_RECOIL)
	{
		WeaponSetState(w, GUNSTATE_FIRING);
	}
}

void WeaponHoldFire(Weapon *w)
{
	WeaponSetState(w, GUNSTATE_READY);
}

void WeaponSetState(Weapon *w, gunstate_e state)
{
	w->state = state;
	switch (state)
	{
	case GUNSTATE_FIRING:
		w->stateCounter = 8;
		break;
	case GUNSTATE_RECOIL:
		// This is to make sure the gun stays recoiled as long as the gun is
		// "locked", i.e. cannot fire
		w->stateCounter = w->lock;
		break;
	default:
		w->stateCounter = -1;
		break;
	}
}

int GunIsStatic(gun_e gun)
{
	switch (gun)
	{
	case GUN_MINE:
	case GUN_DYNAMITE:
		return 1;
	default:
		return 0;
	}
}
int GunHasMuzzle(gun_e gun)
{
	return gGunDescriptions[gun].pic == GUNPIC_BLASTER;
}
int IsHighDPS(gun_e gun)
{
	switch (gun)
	{
	case GUN_GRENADE:
	case GUN_FRAGGRENADE:
	case GUN_MOLOTOV:
	case GUN_MINE:
	case GUN_DYNAMITE:
		return 1;
	default:
		return 0;
	}
}
int IsLongRange(gun_e gun)
{
	switch (gun)
	{
	case GUN_MG:
	case GUN_POWERGUN:
	case GUN_SNIPER:
	case GUN_PETRIFY:
	case GUN_BROWN:
		return 1;
	default:
		return 0;
	}
}
int IsShortRange(gun_e gun)
{
	switch (gun)
	{
	case GUN_KNIFE:
	case GUN_FLAMER:
	case GUN_MOLOTOV:
	case GUN_MINE:
	case GUN_DYNAMITE:
	case GUN_GASGUN:
		return 1;
	default:
		return 0;
	}
}