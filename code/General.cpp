#include "General.hpp"
#include "OriginalData.hpp"
#include "Defs.hpp"

// The generic object defining the game...
General General::_e = General ();

// ---
General::General ()
	: _roomTypes (),
	  _objectsPerRoom (),
	  _objects (),
	  _things (),
	  _rooms (),
	  _greenKeyRooms (), _yellowKeyRooms (), _redKeyRooms (), _cyanKeyRooms (),
	  _exitKeyRooms (),
	  _mummy (),
	  _devil (),
	  _dracula (),
	  _hunchback (),
	  _mushrooms (),
	  _appearing (),
	  _dieing (),
	  _appearingMonstersLeft (),
	  _appearingMonstersRight (),
	  _tombStone ()
{
	// Nothing to do...
}

// ---
void General::initialize ()
{
	// Read the definition if the rooms...
	readRoomDefinition ();
	// REad the objects...
	readRoomObjects ();
	// Create the rooms, assigning each object to its room...
	readRooms ();
	// Read the things every roomn has...
	readThings ();
	// Create special things, like keys of something...
	createSpecialTnings ();
	// Assign the potential location of the keys...
	assignKeys ();

	// ..and finally it creates the appearing monsters
	// The form they have when appearing...
	for (int i = 0; i < __NUMBEROFAPPEARINGASPECTS; i++)
	{
		int nF = __APPEARINGFNALASPECT - __APPEARINGINITIALASPECT + 1; 
		_appearing._forms.push_back 
			(form (i, __APPEARINGMONSTERSFORM, i * nF, ((i + 1) * nF) - 1, General::Orientation::__NONE,
				QGAMES::Position (__BD 0, __BD -61, __BD 0)));
	}

	// The form they have when dieing...
	for (int i = 0; i < __NUMBEROFDIEINGASPECTS; i++)
	{
		int nF = __DIEINGFINALASPECT - __DIEINGINITIALASPECT + 1; 
		_dieing._forms.push_back 
			(form (i, __DIEINGMONSTERDFORM, i * nF, ((i + 1) * nF) - 1, General::Orientation::__NONE, 
				QGAMES::Position (__BD 0, __BD -61, __BD 0)));
	}

	// ...and also when they move...
	for (int i = 0; i < __APPEARINGMONSTERNUMBERASPECTS; i++)
	{
		int nF = (__APPEARINGMONSTERFINALASPECT - __APPEARINGMONSTERINITIALASPECT + 1);
		_appearingMonstersLeft._forms.push_back 
			(form (0, __APPEARINGMONSTERSLEFTFORM, (i * nF) + __APPEARINGMONSTERINITIALASPECT, 
				(i * nF) + __APPEARINGMONSTERFINALASPECT,  General::Orientation::__NONE,
					QGAMES::Position::_cero));
		_appearingMonstersRight._forms.push_back 
			(form (0, __APPEARINGMONSTERSRIGHTFORM, (i * nF) + __APPEARINGMONSTERINITIALASPECT, 
				(i * nF) + __APPEARINGMONSTERFINALASPECT,  General::Orientation::__NONE,
					QGAMES::Position::_cero));
	}
}

// ---
QGAMES::Position General::convertPoint (const QGAMES::Position& p)
{
	QGAMES::Position result = p;
	result = 2.5 * p; 
	return (result);
}

// ---
QGAMES::Color General::convertColor (int c)
{
	QGAMES::Color red (255,0,0);
	QGAMES::Color magenta (255,0,255);
	QGAMES::Color green (0,255,0);
	QGAMES::Color cyan (0,255,255);
	QGAMES::Color yellow (255,255,0);
	QGAMES::Color white (255,255,255);

	std::vector <QGAMES::Color> color (6);
	color [0] = red; color [1] = magenta; color [2] = green;
	color [3] = cyan; color [4] = yellow; color [5] = white;

	// 0x00 is also white... 0x42 is the basic color...
	return ((c == 0x00) ? white : color [c - 0x42]); 
}

// ---
General::FormsDefinitionList General::convertForm (int f, int fg)
{
	General::FormsDefinitionList result;

	// Depending on the object the form will be one or other...
	switch (f)
	{
		case __ORIGINALCAVEDOOR:
		case __ORIGINALNORMALDOORFRAME:
		case __ORIGINALBIGDOORFRAME:
		case __ORIGINALREDLOCKEDNORMALDOOR:
		case __ORIGINALGREENLOCKEDNORMALDOOR:
		case __ORIGINALWHITELOCKEDNORMALDOOR:
		case __ORIGINALYELLOWLOCKEDNORMALDOOR:
		case __ORIGINALREDLOCKEDCAVEDOOR:
		case __ORIGINALGREENLOCKEDCAVEDOOR:
		case __ORIGINALWHITELOCKEDCAVEDOOR:
		case __ORIGINALYELLOWLOCKEDCAVEDOOR:
			{
				General::FormDefinition fD; // The frame...
				fD._form = 100; fD._initialFrame = 0; fD._finalFrame = 0;
				General::FormDefinition fDB; // The border...(to collision with and determinate the limit)
				fDB._form = 1001; fDB._initialFrame = 0; fDB._finalFrame = 0;
				General::FormDefinition fDD; // The door itself...
				fDD._form = 1002; fDD._initialFrame = 0; fDD._finalFrame = 0;

				if (f == __ORIGINALREDLOCKEDNORMALDOOR || f == __ORIGINALREDLOCKEDCAVEDOOR)
					{ fDD._initialFrame = 1; fDD._finalFrame = 1; }
				if (f == __ORIGINALGREENLOCKEDNORMALDOOR || f == __ORIGINALGREENLOCKEDCAVEDOOR)
					{ fDD._initialFrame = 3; fDD._finalFrame = 3; }
				if (f == __ORIGINALWHITELOCKEDNORMALDOOR || f == __ORIGINALWHITELOCKEDCAVEDOOR)
					{ fDD._initialFrame = 4; fDD._finalFrame = 4; }
				if (f == __ORIGINALYELLOWLOCKEDNORMALDOOR || f == __ORIGINALYELLOWLOCKEDCAVEDOOR)
					{ fDD._initialFrame = 5; fDD._finalFrame = 5; }

				switch (fg)
				{
					case 0x00: // At the top
						fDB._initialFrame = fDB._initialFrame + 2;
						fDB._finalFrame = fDB._finalFrame + 2;
						fD._adjust = QGAMES::Position (__BD 0, __BD -72);
						fDB._adjust = QGAMES::Position (__BD 0, __BD -72);
						fDD._adjust = QGAMES::Position (__BD 0, __BD -72);
						fD._orientation = General::Orientation::__UP;
						fDB._orientation = General::Orientation::__UP;
						fDD._orientation = General::Orientation::__UP;
						break;

					case 0x60: // At the right
						fD._initialFrame = fD._initialFrame + 2;
						fD._finalFrame = fD._finalFrame + 2;
						fDB._initialFrame = fDB._initialFrame + 1;
						fDB._finalFrame = fDB._finalFrame + 1;
						fDD._initialFrame = fDD._initialFrame + 18;
						fDD._finalFrame = fDD._finalFrame + 18;
						fD._adjust = QGAMES::Position (__BD -8, __BD -81);
						fDB._adjust = QGAMES::Position (__BD -8, __BD -81);
						fDD._adjust = QGAMES::Position (__BD -8, __BD -81);
						fD._orientation = General::Orientation::__RIGHT;
						fDB._orientation = General::Orientation::__RIGHT;
						fDD._orientation = General::Orientation::__RIGHT;
						break;

					case 0x80: // At the bottom
						fD._initialFrame = fD._initialFrame + 3;
						fD._finalFrame = fD._finalFrame + 3;
						fDB._initialFrame = fDB._initialFrame + 3;
						fDB._finalFrame = fDB._finalFrame + 3;
						fDD._initialFrame = fDD._initialFrame + 6;
						fDD._finalFrame = fDD._finalFrame + 6;
						fD._adjust = QGAMES::Position (__BD 0, __BD -65);
						fDB._adjust = QGAMES::Position (__BD 0, __BD -65);
						fDD._adjust = QGAMES::Position (__BD 0, __BD -65);
						fD._orientation = General::Orientation::__DOWN;
						fDB._orientation = General::Orientation::__DOWN;
						fDD._orientation = General::Orientation::__DOWN;
						break;

					case 0xE0: // At the left
						fD._initialFrame = fD._initialFrame + 1;
						fD._finalFrame = fD._finalFrame + 1;
						fDD._initialFrame = fDD._initialFrame + 12;
						fDD._finalFrame = fDD._finalFrame + 12;
						fD._adjust = QGAMES::Position (__BD -15, __BD -81);
						fDB._adjust = QGAMES::Position (__BD -15, __BD -81);
						fDD._adjust = QGAMES::Position (__BD -15, __BD -81);
						fD._orientation = General::Orientation::__LEFT;
						fDB._orientation = General::Orientation::__LEFT;
						fDD._orientation = General::Orientation::__LEFT;
						break;

					default:
						break;
				};

				result.push_back (fD);
				result.push_back (fDB);
				result.push_back (fDD);
			}

			break;

		case __ORIGINALTABLE:
			// The orientation of the table or rug doesn't depend on the flags
			result.push_back (form (0, 109, 0, 0, General::Orientation::__NONE, 
				QGAMES::Position (__BD 0, __BD -59, __BD 0)));
			break;

		case __ORIGINALDARKPICTURE:
		case __ORIGINALLIGHTPICTURE:
			{
				// Both are managed equal...
				General::FormDefinition fD;
				fD._form = 112; 
				// Just to test: fD._form = (f == __ORIGINALLIGHTPICTURE) ? 112 : 1112; 
				fD._initialFrame = (rand () % 2) * 4; // Random between two options...
				fD._finalFrame = fD._initialFrame;

				switch (fg)
				{
					case 0x00: // At the top
						if (f == __ORIGINALDARKPICTURE)	fD._adjust = QGAMES::Position (__BD 0, __BD -68);
						else fD._adjust = QGAMES::Position (__BD 0, __BD -54);
						fD._orientation = General::Orientation::__UP;
						break;

					case 0x60: // At the right
					case 0x61: // Sometimes...
					case 0x40: // Sometimes...
					case 0x41: // Sometimes...
						fD._initialFrame = fD._initialFrame + 1;
						fD._finalFrame = fD._finalFrame + 1;
						if (f == __ORIGINALDARKPICTURE)	fD._adjust = QGAMES::Position (__BD -5, __BD -78);
						else fD._adjust = QGAMES::Position (__BD -20, __BD -78);
						fD._orientation = General::Orientation::__RIGHT;
						break;

					case 0x80: // At the bottom
					case 0x81: // Sometimes...
						fD._initialFrame = fD._initialFrame + 2;
						fD._finalFrame = fD._finalFrame + 2;
						if (f == __ORIGINALDARKPICTURE)	fD._adjust = QGAMES::Position (__BD 0, __BD -60);
						else fD._adjust = QGAMES::Position (__BD 0, __BD -78);
						fD._orientation = General::Orientation::__DOWN;
						break;

					case 0xE0: // At the left
					case 0xE1: // Sometimes...
						fD._initialFrame = fD._initialFrame + 3;
						fD._finalFrame = fD._finalFrame + 3;
						if (f == __ORIGINALDARKPICTURE)	fD._adjust = QGAMES::Position (__BD -20, __BD -68);
						else fD._adjust = QGAMES::Position (__BD -20, __BD -54);
						fD._orientation = General::Orientation::__LEFT;
						break;

					default:
						break;
				};

				result.push_back (fD);
			}

			break;

		case __ORIGINALANTLERTROPHY:
		case __ORIGINALTROPHY:
			{ // Both are managed equal...
				General::FormDefinition fD;
				fD._form = 110; 
				// Just to test fD._form = (f == __ORIGINALANTLERTROPHY) ? 110 : 1110; 
				fD._initialFrame = (rand () % 2) * 4; // Random between two options...
				fD._finalFrame = fD._initialFrame;

				switch (fg)
				{
					case 0x00: // At the top
						if (f == __ORIGINALANTLERTROPHY) fD._adjust = QGAMES::Position (__BD -5, __BD -47);
						else fD._adjust = QGAMES::Position (__BD -5, __BD -47);
						fD._orientation = General::Orientation::__UP;
						break;

					case 0x60: // At the right
					case 0x40: // Sometimes...
						fD._initialFrame = fD._initialFrame + 1;
						fD._finalFrame = fD._finalFrame + 1;
						if (f == __ORIGINALANTLERTROPHY) fD._adjust = QGAMES::Position (__BD -5, __BD -67);
						else fD._adjust = QGAMES::Position (__BD -5, __BD -67);
						fD._orientation = General::Orientation::__RIGHT;
						break;

					case 0x80: // At the bottom
						fD._initialFrame = fD._initialFrame + 2;
						fD._finalFrame = fD._finalFrame + 2;
						if (f == __ORIGINALANTLERTROPHY) fD._adjust = QGAMES::Position (__BD -15, __BD -47);
						else fD._adjust = QGAMES::Position (__BD -5, __BD -47);
						fD._orientation = General::Orientation::__DOWN;
						break;

					case 0xE0: // At the left
					case 0x81: // Sometimes...
						fD._initialFrame = fD._initialFrame + 3;
						fD._finalFrame = fD._finalFrame + 3;
						if (f == __ORIGINALANTLERTROPHY) fD._adjust = QGAMES::Position (__BD -5, __BD -47);
						else fD._adjust = QGAMES::Position (__BD -5, __BD -47);
						fD._orientation = General::Orientation::__LEFT;
						break;

					default:
						break;
				};

				result.push_back (fD);
			}

			break;

		case __ORIGINALRUG:
			// The orientation of the table or rug doesn't depend on the flags
			result.push_back (form (0, 113, 3, 3, General::Orientation::__NONE,
				QGAMES::Position (__BD 0, __BD -124, __BD 0)));
			break;

		case __ORIGINALSKELETON:
			{ 
				General::FormDefinition fD;
				fD._form = 119;
				fD._initialFrame = fD._finalFrame = 0;

				switch (fg)
				{
					case 0x00: // At the top
						fD._adjust = QGAMES::Position (__BD 0, __BD -117);
						fD._orientation = General::Orientation::__UP;
						break;

					case 0x60: // At the right
					case 0x61: // Sometimes...
						fD._initialFrame = fD._initialFrame + 1;
						fD._finalFrame = fD._finalFrame + 1;
						fD._adjust = QGAMES::Position (__BD 0, __BD -117);
						fD._orientation = General::Orientation::__RIGHT;
						break;

					case 0x80: // At the bottom
					case 0x81: // Sometimes...
						fD._initialFrame = fD._initialFrame + 2;
						fD._finalFrame = fD._finalFrame + 2;
						fD._adjust = QGAMES::Position (__BD -15, __BD -100);
						fD._orientation = General::Orientation::__DOWN;
						break;

					case 0xE0: // At the left
					case 0xE1: // Sometimes...
						fD._initialFrame = fD._initialFrame + 3;
						fD._finalFrame = fD._finalFrame + 3;
						fD._adjust = QGAMES::Position (__BD -15, __BD -100);
						fD._orientation = General::Orientation::__LEFT;
						break;

					default:
						break;
				};

				result.push_back (fD);
			}

			break;

		case __ORIGINALKNIGHT:
			{ 
				General::FormDefinition fD;
				fD._form = 103;
				fD._initialFrame = fD._finalFrame = 0; 

				switch (fg)
				{
					case 0x00: // At the top
						fD._adjust = QGAMES::Position (__BD -24, __BD -80);
						fD._orientation = General::Orientation::__UP;
						break;

					case 0x60: // At the right
						fD._initialFrame = fD._initialFrame + 2;
						fD._finalFrame = fD._finalFrame + 2;
						fD._adjust = QGAMES::Position (__BD -8, __BD -64);
						fD._orientation = General::Orientation::__RIGHT;
						break;

					case 0x80: // At the bottom
						fD._initialFrame = fD._initialFrame + 3;
						fD._finalFrame = fD._finalFrame + 3;
						fD._adjust = QGAMES::Position (__BD -24, __BD -84);
						fD._orientation = General::Orientation::__DOWN;
						break;

					case 0xE0: // At the left
						fD._initialFrame = fD._initialFrame + 1;
						fD._finalFrame = fD._finalFrame + 1;
						fD._adjust = QGAMES::Position (__BD -8, __BD -64);
						fD._orientation = General::Orientation::__LEFT;
						break;

					default:
						break;
				};

				result.push_back (fD);
			}

			break;

		case __ORIGINALCLOCK:
			{
				General::FormDefinition fD; // The frame...
				fD._form = 108;
				fD._initialFrame = fD._finalFrame = 0;
				General::FormDefinition fDB; // The border...(to collision with and determinate the limit)
				fDB._form = 1081;
				fDB._initialFrame = 0; fDB._finalFrame = 0;
				General::FormDefinition fDD; // The door itself...
				fDD._form = 1082;
				fDD._initialFrame = 0; fDD._finalFrame = 0;

				switch (fg)
				{
					case 0x00: // At the top
					case 0x01: // Sometimes...
						fD._adjust = QGAMES::Position (__BD -10, __BD -90);
						fDB._adjust = QGAMES::Position (__BD -10, __BD -90);
						fDD._adjust = QGAMES::Position (__BD 25, __BD -29);
						fD._orientation = General::Orientation::__UP;
						fDB._orientation = General::Orientation::__UP;
						fDD._orientation = General::Orientation::__UP;
						break;

					case 0x60: // At the right
					case 0x61: // Sometimes...
					case 0x40: // Sometimes...
						fD._initialFrame = fD._initialFrame + 1;
						fD._finalFrame = fD._finalFrame + 1;
						fDB._initialFrame = fDB._initialFrame + 1;
						fDB._finalFrame = fDB._finalFrame + 1;
						fDD._initialFrame = fDD._initialFrame + 1;
						fDD._finalFrame = fDD._finalFrame + 1;
						fD._adjust = QGAMES::Position (__BD -10, __BD -90);
						fDB._adjust = QGAMES::Position (__BD -10, __BD -90);
						fDD._adjust = QGAMES::Position (__BD 0, __BD -55);
						fD._orientation = General::Orientation::__RIGHT;
						fDB._orientation = General::Orientation::__RIGHT;
						fDD._orientation = General::Orientation::__RIGHT;
						break;

					case 0x80: // At the bottom
					case 0x81: // Sometimes...
					case 0xA0: // Sometimes...
						fD._initialFrame = fD._initialFrame + 2;
						fD._finalFrame = fD._finalFrame + 2;
						fDB._initialFrame = fDB._initialFrame + 2;
						fDB._finalFrame = fDB._finalFrame + 2;
						fDD._initialFrame = fDD._initialFrame + 2;
						fDD._finalFrame = fDD._finalFrame + 2;
						fD._adjust = QGAMES::Position (__BD -10, __BD -90);
						fDB._adjust = QGAMES::Position (__BD -10, __BD -90);
						fDD._adjust = QGAMES::Position (__BD 26, __BD -80);
						fD._orientation = General::Orientation::__DOWN;
						fDB._orientation = General::Orientation::__DOWN;
						fDD._orientation = General::Orientation::__DOWN;
						break;

					case 0xE0: // At the left
					case 0xE1: // Sometimes...
						fD._initialFrame = fD._initialFrame + 3;
						fD._finalFrame = fD._finalFrame + 3;
						fDB._initialFrame = fDB._initialFrame + 3;
						fDB._finalFrame = fDB._finalFrame + 3;
						fDD._initialFrame = fDD._initialFrame + 3;
						fDD._finalFrame = fDD._finalFrame + 3;
						fD._adjust = QGAMES::Position (__BD -10, __BD -90);
						fDB._adjust = QGAMES::Position (__BD -10, __BD -90);
						fDD._adjust = QGAMES::Position (__BD 51, __BD -54);
						fD._orientation = General::Orientation::__LEFT;
						fDB._orientation = General::Orientation::__LEFT;
						fDD._orientation = General::Orientation::__LEFT;
						break;

					default:
						break;
				};

				result.push_back (fD);
				result.push_back (fDB);
				result.push_back (fDD);
			}

			break;

		case __ORIGINALBOOKCASE:
			{
				General::FormDefinition fD; // The frame...
				fD._form = 106;
				fD._initialFrame = fD._finalFrame = 0;
				General::FormDefinition fDB; // The border...(to collision with and determinate the limit)
				fDB._form = 1061;
				fDB._initialFrame = 0; fDB._finalFrame = 0;
				General::FormDefinition fDD; // The door itself...

				switch (fg)
				{
					case 0x00: // At the top
					case 0x01: // Sometimes...
						fD._adjust = QGAMES::Position (__BD 5, __BD -98);
						fDB._adjust = QGAMES::Position (__BD 5, __BD -98);
						fD._orientation = General::Orientation::__UP;
						fDB._orientation = General::Orientation::__UP;
						fDD._orientation = General::Orientation::__UP;
						break;

					case 0x60: // At the right
					case 0x61: // Sometimes...
					case 0x40: // Sometimes...
						fD._initialFrame = fD._initialFrame + 1;
						fD._finalFrame = fD._finalFrame + 1;
						fDB._initialFrame = fDB._initialFrame + 1;
						fDB._finalFrame = fDB._finalFrame + 1;
						fD._adjust = QGAMES::Position (__BD 3, __BD -110);
						fDB._adjust = QGAMES::Position (__BD 3, __BD -110);
						fD._orientation = General::Orientation::__RIGHT;
						fDB._orientation = General::Orientation::__RIGHT;
						fDD._orientation = General::Orientation::__RIGHT;
						break;

					case 0x80: // At the bottom
					case 0x81: // Sometimes...
					case 0xA0: // Sometimes...
						fD._initialFrame = fD._initialFrame + 2;
						fD._finalFrame = fD._finalFrame + 2;
						fDB._initialFrame = fDB._initialFrame + 2;
						fDB._finalFrame = fDB._finalFrame + 2;
						fD._adjust = QGAMES::Position (__BD 5, __BD -82);
						fDB._adjust = QGAMES::Position (__BD 5, __BD -82);
						fD._orientation = General::Orientation::__DOWN;
						fDB._orientation = General::Orientation::__DOWN;
						fDD._orientation = General::Orientation::__DOWN;
						break;

					case 0xE0: // At the left
					case 0xE1: // Sometimes...
						fD._initialFrame = fD._initialFrame + 3;
						fD._finalFrame = fD._finalFrame + 3;
						fDB._initialFrame = fDB._initialFrame + 3;
						fDB._finalFrame = fDB._finalFrame + 3;
						fD._adjust = QGAMES::Position (__BD -3, __BD -90);
						fDB._adjust = QGAMES::Position (__BD -3, __BD -90);
						fD._orientation = General::Orientation::__LEFT;
						fDB._orientation = General::Orientation::__LEFT;
						fDD._orientation = General::Orientation::__LEFT;
						break;

					default:
						break;
				};

				result.push_back (fD);
				result.push_back (fDB);
			}

			break;

		case __ORIGINALBARREL:
			{
				General::FormDefinition fD; // The frame...
				fD._form = 104;
				fD._initialFrame = fD._finalFrame = 0;
				General::FormDefinition fDB; // The border...(to collision with and determinate the limit)
				fDB._form = 1041;
				fDB._initialFrame = 0; fDB._finalFrame = 0;
				General::FormDefinition fDD; // The door itself...

				switch (fg)
				{
					case 0x00: // At the top
					case 0x01: // Sometimes...
						fD._adjust = QGAMES::Position (__BD -12, __BD -98);
						fDB._adjust = QGAMES::Position (__BD -12, __BD -98);
						fD._orientation = General::Orientation::__UP;
						fDB._orientation = General::Orientation::__UP;
						break;

					case 0x60: // At the right
					case 0x61: // Sometimes...
					case 0x40: // Sometimes...
						fD._initialFrame = fD._initialFrame + 1;
						fD._finalFrame = fD._finalFrame + 1;
						fDB._initialFrame = fDB._initialFrame + 1;
						fDB._finalFrame = fDB._finalFrame + 1;
						fD._adjust = QGAMES::Position (__BD -2, __BD -90);
						fDB._adjust = QGAMES::Position (__BD -2, __BD -90);
						fD._orientation = General::Orientation::__RIGHT;
						fDB._orientation = General::Orientation::__RIGHT;
						break;

					case 0x80: // At the bottom
					case 0x81: // Sometimes...
					case 0xA0: // Sometimes...
						fD._initialFrame = fD._initialFrame + 2;
						fD._finalFrame = fD._finalFrame + 2;
						fDB._initialFrame = fDB._initialFrame + 2;
						fDB._finalFrame = fDB._finalFrame + 2;
						fD._adjust = QGAMES::Position (__BD -12, __BD -82);
						fDB._adjust = QGAMES::Position (__BD -12, __BD -82);
						fD._orientation = General::Orientation::__DOWN;
						fDB._orientation = General::Orientation::__DOWN;
						break;

					case 0xE0: // At the left
					case 0xE1: // Sometimes...
						fD._initialFrame = fD._initialFrame + 3;
						fD._finalFrame = fD._finalFrame + 3;
						fDB._initialFrame = fDB._initialFrame + 3;
						fDB._finalFrame = fDB._finalFrame + 3;
						fD._adjust = QGAMES::Position (__BD -20, __BD -90);
						fDB._adjust = QGAMES::Position (__BD -20, __BD -90);
						fD._orientation = General::Orientation::__LEFT;
						fDB._orientation = General::Orientation::__LEFT;
						break;

					default:
						break;
				};

				result.push_back (fD);
				result.push_back (fDB);
			}

			break;

		case __ORIGINALSHUTTRAPDOOR:
		case __ORIGINALOPENTRAPEDOOR:
			{
				int a = rand () % 6; // Aspect is random among 6 options...
				result.push_back (form (0, 115, a, a, General::Orientation::__NONE, 
					QGAMES::Position (__BD 0, __BD -90, __BD 0)));
				result.push_back (form (0, 1151, 0, 0, General::Orientation::__NONE,
					QGAMES::Position (__BD 18, __BD -78)));
			}

			break;

		case __ORIGINALACGSHIELD:
		case __ORIGINALSHIELD:
			{ // Both are managed equal...
				General::FormDefinition fD;
				fD._form = 114;
				// Just to test fD._form = (f == __ORIGINALACGSHIELD) ? 114 : 1114; 
				fD._initialFrame = rand () % 4; // Random between four options...
				fD._finalFrame = fD._initialFrame;

				switch (fg)
				{
					case 0x00: // At the top
						if (f == __ORIGINALACGSHIELD) fD._adjust = QGAMES::Position (__BD -5, __BD -47);
						else fD._adjust = QGAMES::Position (__BD -5, __BD -47);
						fD._orientation = General::Orientation::__UP;
						break;

					case 0x60: // At the right
					case 0x40: // Sometimes...
						fD._initialFrame = fD._initialFrame + 4;
						fD._finalFrame = fD._finalFrame + 4;
						if (f == __ORIGINALACGSHIELD) fD._adjust = QGAMES::Position (__BD -5, __BD -47);
						else fD._adjust = QGAMES::Position (__BD -5, __BD -47);
						fD._orientation = General::Orientation::__RIGHT;
						break;

					case 0x80: // At the bottom
						fD._initialFrame = fD._initialFrame + 8;
						fD._finalFrame = fD._finalFrame + 8;
						if (f == __ORIGINALACGSHIELD) fD._adjust = QGAMES::Position (__BD -5, __BD -47);
						else fD._adjust = QGAMES::Position (__BD -5, __BD -47);
						fD._orientation = General::Orientation::__DOWN;
						break;

					case 0xE0: // At the left
						fD._initialFrame = fD._initialFrame + 12;
						fD._finalFrame = fD._finalFrame + 12;
						if (f == __ORIGINALACGSHIELD) fD._adjust = QGAMES::Position (__BD -5, __BD -47);
						else fD._adjust = QGAMES::Position (__BD -5, __BD -47);
						fD._orientation = General::Orientation::__LEFT;
						break;

					default:
						break;
				};

				result.push_back (fD);
			}

			break;

		case __ORIGINALACGEXITDOOR:
			result.push_back (form (0, (fg == 0xE0) ? 117 : 116, 0, 0,
				General::Orientation::__RIGHT,
				(fg == 0xE0) ? QGAMES::Position (__BD -4, __BD -175, __BD 0) 
					: QGAMES::Position (__BD -4, __BD -175, __BD 0)));
			result.push_back (form (0, (fg == 0xE0) ? 1171 : 1161, 0, 4, 
				General::Orientation::__RIGHT,
				(fg == 0xE0) ? QGAMES::Position (__BD 19, __BD -126, __BD 0) 
					: QGAMES::Position (__BD 19, __BD -129, __BD 0)));
			break;

		case __ORIGINALBARRELS:
			result.push_back (form (0, __BARRELSFORM, 
				(fg == 0x80) ? __BARRELSUPASPECT : __BARRELSDOWNASPECT, 
				(fg == 0x80) ? __BARRELSUPASPECT : __BARRELSDOWNASPECT,
				General::Orientation::__NONE,
				QGAMES::Position (__BD 0, __BD -74, __BD 0)));
			break;

		case __ORIGINALLEAF:
			result.push_back (form (0, __THINGSTOCATCHFORM, __LEAFASPECT, __LEAFASPECT,
				General::Orientation::__NONE,
				QGAMES::Position (__BD 0, __BD -50, __BD 0)));
			break;

		case __ORIGINALCROSS:
			result.push_back (form (0, __THINGSTOCATCHFORM, __CROSSASPECT, __CROSSASPECT, 
				General::Orientation::__NONE,
				QGAMES::Position (__BD 0, __BD -50, __BD 0)));
			break;

		case __ORIGINALWRENCH:
			result.push_back (form (0, __THINGSTOCATCHFORM, __WRENCHASPECT, __WRENCHASPECT, 
				General::Orientation::__NONE,
				QGAMES::Position (__BD 0, __BD -50, __BD 0)));
			break;

		case __ORIGINALBOTTLE:
			result.push_back (form (0, __THINGSTOCATCHFORM, __BOTTLEASPECT, __BOTTLEASPECT, 
				General::Orientation::__NONE,
				QGAMES::Position (__BD 0, __BD -50, __BD 0)));
			break;

		case __ORIGINALFACECOIN:
			result.push_back (form (0, __THINGSTOCATCHFORM, __FACECOINASPECT, __FACECOINASPECT, 
				General::Orientation::__NONE,
				QGAMES::Position (__BD 0, __BD -50, __BD 0)));
			break;

		case __ORIGINALWING:
			result.push_back (form (0, __THINGSTOCATCHFORM, __WINGASPECT, __WINGASPECT, 
				General::Orientation::__NONE,
				QGAMES::Position (__BD 0, __BD -50, __BD 0)));
			break;

		case __ORIGINALWHIP:
			result.push_back (form (0, __THINGSTOCATCHFORM, __WHIPASPECT, __WHIPASPECT, 
				General::Orientation::__NONE,
				QGAMES::Position (__BD 0, __BD -50, __BD 0)));
			break;

		case __ORIGINALCLAW:
			result.push_back (form (0, __THINGSTOCATCHFORM, __CLAWASPECT, __CLAWASPECT, 
				General::Orientation::__NONE,
				QGAMES::Position (__BD 0, __BD -50, __BD 0)));
			break;

		case __ORIGINALDIAMOND:
			result.push_back (form (0, __THINGSTOCATCHFORM, __DIAMONDASPECT, __DIAMONDASPECT, 
				General::Orientation::__NONE,
				QGAMES::Position (__BD 0, __BD -50, __BD 0)));
			break;

		case __ORIGINALBAGOFMONEY:
			result.push_back (form (0, __THINGSTOCATCHFORM, __BAGOFMONEYASPECT, __BAGOFMONEYASPECT, 
				General::Orientation::__NONE,
				QGAMES::Position (__BD 0, __BD -50, __BD 0)));
			break;

		case __ORIGINALSKULL:
			result.push_back (form (0, __THINGSTOCATCHFORM, __SKULLASPECT, __SKULLASPECT, 
				General::Orientation::__NONE,
				QGAMES::Position (__BD 0, __BD -50, __BD 0)));
			break;

		case __ORIGINALBOTE:
			for (int i = 0; i < __BOTENUMBERASPECTS; i++)
				result.push_back (form (0, __FOODFORM, __BOTEASPECT + i, __BOTEASPECT + i,
					General::Orientation::__NONE, QGAMES::Position (__BD 0, __BD -57, __BD 0)));
			break;

		case __ORIGINALCANDY:
			for (int i = 0; i < __CANDYNUMBERASPECTS; i++)
				result.push_back (form (0, __FOODFORM, __CANDYASPECT + i, __CANDYASPECT + i, 
					General::Orientation::__NONE, QGAMES::Position (__BD 0, __BD -57, __BD 0)));
			break;

		case __ORIGINALLEG:
			for (int i = 0; i < __LEGNUMBERASPECTS; i++)
				result.push_back (form (0, __FOODFORM, __LEGASPECT + i, __LEGASPECT + i, 
					General::Orientation::__NONE, QGAMES::Position (__BD 0, __BD -57, __BD 0)));
			break;

		case __ORIGINALLOLLIPOP:
			for (int i = 0; i < __POLIPOPNUMBERASPECTS; i++)
				result.push_back (form (0, __FOODFORM, __POLIPOPASPECT + i, __POLIPOPASPECT + i, 
					General::Orientation::__NONE, QGAMES::Position (__BD 0, __BD -57, __BD 0)));
			break;

		case __ORIGINALCONE:
			for (int i = 0; i < __CONENUMBERASPECTS; i++)
				result.push_back (form (0, __FOODFORM, __CONEASPECT + i, __CONEASPECT + i, 
					General::Orientation::__NONE, QGAMES::Position (__BD 0, __BD -57, __BD 0)));
			break;

		case __ORIGINALBOWL:
			for (int i = 0; i < __BOWLNUMBERASPECTS; i++)
				result.push_back (form (0, __FOODFORM, __BOWLASPECT + i, __BOWLASPECT + i, 
					General::Orientation::__NONE, QGAMES::Position (__BD 0, __BD -57, __BD 0)));
			break;

		case __ORIGINALAPPLE:
			for (int i = 0; i < __APPLENUMBERASPECTS; i++)
				result.push_back (form (0, __FOODFORM, __APPLEASPECT + i, __APPLEASPECT + i, 
					General::Orientation::__NONE, QGAMES::Position (__BD 0, __BD -57, __BD 0)));
			break;

		case __ORIGINALMILK:
			result.push_back (form (0, __FOODFORM, __MILKASPECT, __MILKASPECT, 
				General::Orientation::__NONE, QGAMES::Position (__BD 0, __BD -57, __BD 0)));
			break;

		case __ORIGINALMUSHROOM:
			for (int i = 0; i < __MUSHROOMNUMBERASPECTS; i++)
				result.push_back (form (0, __MUSHROOMSFORM, __MUSHROOMASPECT + i, __MUSHROOMASPECT + i, 
					General::Orientation::__NONE, QGAMES::Position (__BD 0, __BD -54, __BD 0)));
			break;

		case __ORIGINALMUMMY:
			result.push_back (form (0, __MONSTERFORM, __MUMMYINITALASPECT, __MUMMYFINALASPECT, 
				General::Orientation::__NONE, QGAMES::Position (__BD 0, __BD -66, __BD 0)));
			break;

		case __ORIGINALFRANK:
			result.push_back (form (0, __MONSTERFORM, __FRANKINITIALASPECT, __FRANKFINALASPECT, 
				General::Orientation::__NONE, QGAMES::Position (__BD 0, __BD -66, __BD 0)));
			break;

		case __ORIGINALDEVIL:
			result.push_back (form (0, __MONSTERFORM, __DEVILINITIALASPECT, __DEVILFINALASPECT, 
				General::Orientation::__NONE, QGAMES::Position (__BD 0, __BD -66, __BD 0)));
			break;

		case __ORIGINALDRACULA:
			result.push_back (form (0, __MONSTERFORM, __DRACULAINITIALASPECT, __DRACULAFINALASPECT, 
				General::Orientation::__NONE, QGAMES::Position (__BD 0, __BD -66, __BD 0)));
			break;

		case __ORIGINALHUNCH:
			result.push_back (form (0, __MONSTERFORM, __HUNCHINITIALASPECT, __HUNCHFINALASPECT, 
				General::Orientation::__NONE, QGAMES::Position (__BD 0, __BD -66, __BD 0)));
			break;

		default:
			break;
	}

	return (result);
}

// ---
void General::readRoomDefinition ()
{
	// First the definition in line and point of every room type...
	// AFter the loop there will be a defintion about how to draw avery room...
	for (int i = 0; i < __NUMBEROFROOMTYPES__; i++)
	{
		RoomPicture p;

		// The position where the definition of the points the room is made up starts...
		int pPos = ((OriginalData::roomTypes [i][3] * 256) + OriginalData::roomTypes [i][2]) 
			- OriginalData::roomLinePointsOffSet; 
		// The position where the definition of the lines the room is mada up starts...
		int lPos = ((OriginalData::roomTypes [i][5] * 256) + OriginalData::roomTypes [i][4]) 
			- OriginalData::roomLinePointsOffSet; 

		// Iterates over all line definitions...
		// Every line has to points, the beginning and the end
		// The definition of the x coord is where the cursor over the line definition points
		// The definition of the y coord is the next
		while (OriginalData::roomPoints [lPos] != 0xFF)
		{
			// The starting point for the line...
			int iXPos = OriginalData::roomPoints 
				[pPos + 2 * OriginalData::roomPoints [lPos]];
			int iYPos = OriginalData::roomPoints 
				[pPos + (2 * OriginalData::roomPoints [lPos]) + 1];

			// There could be many lines from that starting point.
			// The initial point doesn't change until a not valid point is reached
			while (OriginalData::roomPoints [++lPos] != 0xFF)
			{
				// A line...
				LineToDraw l; 
				// The end point...
				int eXPos = OriginalData::roomPoints 
					[pPos + 2 * OriginalData::roomPoints [lPos]];
				int eYPos = OriginalData::roomPoints 
					[pPos + (2 * OriginalData::roomPoints [lPos]) + 1];
				l._origin = convertPoint (QGAMES::Position (__BD iXPos, __BD iYPos, __BD 0));
				l._end = convertPoint (QGAMES::Position (__BD eXPos, __BD eYPos, __BD 0));
				p.push_back (l); 
				// ...that is inserted into the line definition...
			}

			lPos++;
		}

		// A new type of room is inserted...
		_roomTypes.insert (RoomTypes::value_type (i, p)); 
	}
}

// ---
void General::readRoomObjects ()
{
	// Now it is type to create the objects...
	// located at the different rooms
	_objects.clear ();
	_objects.resize (__NUMBEROFOBJECTS__);
	for (int i = 0; i < __NUMBEROFOBJECTS__; i++)
	{
		// Create the object...
		ObjectDefinition obj;
		obj._id = i;
		// The final form of the object will depend on the initial form, and the flags...
		obj._forms = convertForm (OriginalData::backgroundElements [i][0], 
			OriginalData::backgroundElements [i][5]);
		obj._inRoom = OriginalData::backgroundElements [i][1];
		obj._position = convertPoint (QGAMES::Position (__BD OriginalData::backgroundElements [i][3],
			__BD OriginalData::backgroundElements [i][4], __BD 0));
		obj._time = OriginalData::backgroundElements [i][6]; // 0 when it is nor a door...
		if (OriginalData::backgroundElements [i][0] != __ORIGINALNORMALDOORFRAME &&
			OriginalData::backgroundElements [i][0] != __ORIGINALCAVEDOOR &&
			OriginalData::backgroundElements [i][0] != __ORIGINALBIGDOORFRAME &&
			OriginalData::backgroundElements [i][0] != __ORIGINALSHUTTRAPDOOR &&
			OriginalData::backgroundElements [i][0] != __ORIGINALOPENTRAPEDOOR &&
			OriginalData::backgroundElements [i][0] != __ORIGINALCLOCK)
			obj._time = -1; // Not open or close automatically...

		// What it is...
		// ...¿A generic door?
		if (OriginalData::backgroundElements [i][0] == __ORIGINALNORMALDOORFRAME ||
			OriginalData::backgroundElements [i][0] == __ORIGINALCAVEDOOR ||
			OriginalData::backgroundElements [i][0] == __ORIGINALBIGDOORFRAME)
			obj._whatIs = General::WhatIs::__NORMALDOR; // It is a normal door...
		// Or an specific type of door?...
		// ...green?
		if (OriginalData::backgroundElements [i][0] == __ORIGINALGREENLOCKEDCAVEDOOR ||
			OriginalData::backgroundElements [i][0] == __ORIGINALGREENLOCKEDNORMALDOOR)
			obj._whatIs = General::WhatIs::__GREENDOOR;
		// ...yellow?
		if (OriginalData::backgroundElements [i][0] == __ORIGINALYELLOWLOCKEDCAVEDOOR ||
			OriginalData::backgroundElements [i][0] == __ORIGINALYELLOWLOCKEDNORMALDOOR)
			obj._whatIs = General::WhatIs::__YELLOWDOOR;
		// ...red?
		if (OriginalData::backgroundElements [i][0] == __ORIGINALREDLOCKEDCAVEDOOR ||
			OriginalData::backgroundElements [i][0] == __ORIGINALREDLOCKEDNORMALDOOR)
			obj._whatIs = General::WhatIs::__REDDOOR;
		// ...white?
		if (OriginalData::backgroundElements [i][0] == __ORIGINALWHITELOCKEDCAVEDOOR ||
			OriginalData::backgroundElements [i][0] == __ORIGINALWHITELOCKEDNORMALDOOR)
			obj._whatIs = General::WhatIs::__WHITEDOOR;
		// ...trap door?
		if (OriginalData::backgroundElements [i][0] == __ORIGINALSHUTTRAPDOOR ||
			OriginalData::backgroundElements [i][0] == __ORIGINALOPENTRAPEDOOR)
			obj._whatIs = General::WhatIs::__TRAPDOOR;
		// ...or a clock?
		if (OriginalData::backgroundElements [i][0] == __ORIGINALCLOCK)
			obj._whatIs = General::WhatIs::__CLOCKDOOR;
		// ...or it is a bookcase door?
		if (OriginalData::backgroundElements [i][0] == __ORIGINALBOOKCASE)
			obj._whatIs = General::WhatIs::__BOOKCASEDOOR;
		// ...or a barrel door?
		if (OriginalData::backgroundElements [i][0] == __ORIGINALBARREL)
			obj._whatIs = General::WhatIs::__BARRELDOOR;
		// ...or the exit door?
		if (OriginalData::backgroundElements [i][0] == __ORIGINALACGEXITDOOR)
			obj._whatIs = General::WhatIs::__EXITDOOR;
		// ..or a table?
		if (OriginalData::backgroundElements [i][0] == __ORIGINALTABLE)
			obj._whatIs = General::WhatIs::__TABLE;
		// ..or the barrels?
		if (OriginalData::backgroundElements [i][0] == __ORIGINALBARRELS)
			obj._whatIs = General::WhatIs::__BARRELS;
		// ..or the skeleton?
		if (OriginalData::backgroundElements [i][0] == __ORIGINALSKELETON)
			obj._whatIs = General::WhatIs::__SKELETON;

		// To know whether it is connected or not...
		// The object is connected to other when it is not a door
		// it is when the attribute number 3 is not 0
		// In this case the connected element depends whether the 
		// object is in an odd or even position!
		obj._isConnected = (OriginalData::backgroundElements [i][2] != 0 && 
			OriginalData::backgroundElements [i][0] != __ORIGINALRUG); // The rug is not connected...back
		obj._connectedToObj = obj._connectedToRoom = 0;
		if (obj._isConnected)
		{
			int oCon = (((i + 1) % 2) == 0) ? (i - 1) : (i + 1); 
			// Next in odd position, and previous is even position...
			obj._connectedToObj = oCon;
			obj._connectedToRoom = OriginalData::backgroundElements [oCon][1];
		}

		// The last adjustment...
		// The doors in the stairsway...
		// The original game had different sprites for door at trhe top of the stairs.
		// In this version we are using the same thing
		// So, it it needed to adjust.
		if (OriginalData::backgroundElements [i][0] == __ORIGINALBIGDOORFRAME)
		{
			int pD = OriginalData::backgroundElements [i][5];
			QGAMES::Vector adj = ( pD == 0x80) // Down...
				? QGAMES::Vector (__BD 18, __BD -21, __BD 0) 
				: ((pD == 0xE0) /* Left */ ? QGAMES::Vector (__BD 23, __BD -18, __BD 0) 
					: QGAMES::Vector::_cero);
			FormsDefinitionList fLs;
			for (int j = 0; j < (int) obj._forms.size (); j++)
			{
				FormDefinition fL = obj._forms [j];
				fL._adjust = fL._adjust + adj;
				fLs.push_back (fL);
			}

			obj._forms = fLs;
		}

		// When the object is a table (or similar), it blocks the
		// movement of the player in the room...
		if (obj._whatIs == General::WhatIs::__TABLE ||
			obj._whatIs == General::WhatIs::__BARRELS ||
			obj._whatIs == General::WhatIs::__SKELETON)
			obj._blockMovement = true;

		// ...and insert the object in the definition of objects...
		Objects objs;
		ObjectsPerRoom::iterator j = _objectsPerRoom.find (obj._inRoom);
		if (j != _objectsPerRoom.end ())
			objs = (*j).second;
		objs.push_back (obj);
		if (j != _objectsPerRoom.end ())
			(*j).second = objs;
		else
			_objectsPerRoom.insert (ObjectsPerRoom::value_type (obj._inRoom, objs));

		// Insert the object in the list of objects...
		_objects [i] = obj;
	}
}

// ---
void General::readRooms ()
{
	// Time to create the rooms...
	for (int i = 0; i < __NUMBEROFROOMS__; i++)
	{
		// The room is made up of color, type of room, and objects inside...
		RoomDefinition room;
		room._id = i;
		// The color has to be translated...
		room._color = convertColor (OriginalData::rooms [i][0]); 
		// The type of the room is the one defined in the structure...
		room._type = OriginalData::rooms [i][1];
		// The size of the room is in the definition, but some adjustments has been done...
		QGAMES::Position size = convertPoint (QGAMES::Position 
			(__BD OriginalData::roomTypes [room._type][0], 
				__BD OriginalData::roomTypes [room._type][1]));
		room._w = (int) size.posX () + 20; room._h = (int) size.posY () + 20;
		room._cx = 239; room._cy = 239;
		General::ObjectsPerRoom::const_iterator j = _objectsPerRoom.find (i);
		room._objects = (j != _objectsPerRoom.end ()) ? (*j).second : General::Objects ();
		_rooms.push_back (room); 
		// A room has been created...!!
	}
}

// ---
void General::readThings ()
{
	// Now it is time to assign the different elemnts in every room...
	_things.clear ();
	_things.resize (__NUMBEROFTHINGS);
	for (int i = 0; i < (__NUMBEROFTHINGS - 7) /* Three colored keys and three parts of the main key */; 
		i++)
	{
		ThingDefinition th;
		th._id = i;
		th._forms = convertForm (OriginalData::elements [i][0], 0); 
		// No attributes related with position is needed...
		th._position = 
			convertPoint (QGAMES::Position (__BD OriginalData::elements [i][2], 
				__BD OriginalData::elements [i][3]));
		th._inRoom = OriginalData::elements [i][1];
		switch (OriginalData::elements [i][4])
		{
			// It is something to be eaten...
			case 0x01:
				{
					th._whatIs = General::WhatIs::__FOOD;
					th._time = 20; // Seconds to re-appear when it is aeaten...
					_rooms [OriginalData::elements [i][1]]._thingsToEat.push_back (th);
				}

				break;

			// It is a monster that can not be killed
			case 0x02:
				{
					th._whatIs = General::WhatIs::__MUSHROOM;
					_mushrooms = th;
					_rooms [OriginalData::elements [i][1]]._monsters.push_back (th);
				}

				break;

			// It is something to kill the monsters, or to do something else in the game...
			case 0x03: 
				{
					th._whatIs =
						((OriginalData::elements [i][0] == __ORIGINALLEAF) ? General::WhatIs::__KILLMUMMY :
						((OriginalData::elements [i][0] == __ORIGINALCROSS) ? General::WhatIs::__KILLDRACULA : 
						((OriginalData::elements [i][0] == __ORIGINALWRENCH) ? General::WhatIs::__KILLFRANK :
						((OriginalData::elements [i][0] == __ORIGINALCLAW) ? General::WhatIs::__KILLHUNCH : 
							General::WhatIs::__AMULET)))); // The rest are amultes...
						// It is not possible to kill, distract or whatever the devil...
					_rooms [OriginalData::elements [i][1]]._thingsToCatch.push_back (th);
				}

				break;

			// It is a normal monster
			case 0x04:
				{
					th._whatIs =
						((OriginalData::elements [i][0] == __ORIGINALMUMMY) ? General::WhatIs::__MUMMY :
						((OriginalData::elements [i][0] == __ORIGINALFRANK) ? General::WhatIs::__FRANK :
						((OriginalData::elements [i][0] == __ORIGINALDEVIL) ? General::WhatIs::__DEVIL : 
						((OriginalData::elements [i][0] == __ORIGINALDRACULA) ? General::WhatIs::__DRACULA : 
						((OriginalData::elements [i][0] == __ORIGINALHUNCH) ? General::WhatIs::__HUNCH : 
							General::WhatIs::__GENERICTHING))))); // The rest are things...
					_rooms [OriginalData::elements [i][1]]._monsters.push_back (th);
					if (th._whatIs == General::WhatIs::__MUMMY)
						_mummy = th; // To be get it back later...
					else
					if (th._whatIs == General::WhatIs::__DEVIL)
						_devil = th;
					else
					if (th._whatIs == General::WhatIs::__DRACULA)
						_dracula = th;
					else
					if (th._whatIs == General::WhatIs::__HUNCH)
						_hunchback = th;
					else
					if (th._whatIs == General::WhatIs::__FRANK)
						_frankestein = th;
				}

				break;

			case 0x05:
				{
					th._whatIs =
						((OriginalData::elements [i][0] == __ORIGINALBOTTLE) ? General::WhatIs::__BOTTLE :
						((OriginalData::elements [i][0] == __ORIGINALFACECOIN) ? General::WhatIs::__FACECOIN : 
						((OriginalData::elements [i][0] == __ORIGINALWING) ? General::WhatIs::__WING :
						((OriginalData::elements [i][0] == __ORIGINALWHIP) ? General::WhatIs::__WHIP : 
						((OriginalData::elements [i][0] == __ORIGINALDIAMOND) ? General::WhatIs::__DIAMOND : 
						((OriginalData::elements [i][0] == __ORIGINALBAGOFMONEY) ? General::WhatIs::__BAGOFMONEY : 
						((OriginalData::elements [i][0] == __ORIGINALSKULL) ? General::WhatIs::__SKULL : 
							General::WhatIs::__GENERICTHING))))))); // The rest are things...
						// It is not possible to kill, distract or whatever the devil...
					_rooms [OriginalData::elements [i][1]]._thingsToCatch.push_back (th);
				}

				break;

			default:
				break;
		}

		// Insert the thing in the list of things...
		_things [i] = th;
	}
}

// ---
void General::createSpecialTnings ()
{
	// The green key...
	ThingDefinition gK;
	gK._id = __GREENKEYID;
	gK._forms.push_back 
		(form (0, __THINGSTOCATCHFORM, __GREENKEYASPECT, __GREENKEYASPECT, General::Orientation::__NONE,
			QGAMES::Position (__BD 0, __BD -50, __BD 0)));
	gK._whatIs = General::WhatIs::__GREENKEY;
	_things [__NUMBEROFTHINGS - 7] = gK;

	// The yellow one...
	ThingDefinition yK;
	yK._id = __YELLOWKEYID;
	yK._forms.push_back 
		(form (0, __THINGSTOCATCHFORM, __YELLOWKEYASPECT, __YELLOWKEYASPECT, General::Orientation::__NONE, 
			QGAMES::Position (__BD 0, __BD -50, __BD 0)));
	yK._whatIs = General::WhatIs::__YELLOWKEY;
	_things [__NUMBEROFTHINGS  - 6] = yK;

	// The red one...
	ThingDefinition rK;
	rK._id = __REDKEYID;
	rK._forms.push_back 
		(form (0, __THINGSTOCATCHFORM, __REDKEYASPECT, __REDKEYASPECT, General::Orientation::__NONE,
			QGAMES::Position (__BD 0, __BD -50, __BD 0)));
	rK._whatIs = General::WhatIs::__REDKEY;
	_things [__NUMBEROFTHINGS - 5] = rK;

	// The cyan (white one);
	ThingDefinition wK;
	wK._id = __CYANKEYID;
	wK._forms.push_back 
		(form (0, __THINGSTOCATCHFORM, __CYANKEYASPECT, __CYANKEYASPECT, General::Orientation::__NONE, 
			QGAMES::Position (__BD 0, __BD -50, __BD 0)));
	wK._whatIs = General::WhatIs::__WHITEKEY;
	_things [__NUMBEROFTHINGS - 4] = wK;

	// The main keys...
	for (int i = 0; i < 3; i++)
	{
		ThingDefinition mK;
		mK._id = __EXITKEYID + i;
		mK._forms.push_back 
			(form (i, __THINGSTOCATCHFORM, __EXITKEYASPECT + i, __EXITKEYASPECT + i, General::Orientation::__NONE, 
				QGAMES::Position (__BD 0, __BD -50, __BD 0)));
		mK._whatIs = (General::WhatIs ) ((int) General::WhatIs::__ACGKEY1 + i);
		_things [__NUMBEROFTHINGS - 3 + i] = mK;
	}

	// The tombstone...
	_tombStone._id = __TOMBSTONEOBJID;
	_tombStone._whatIs = General::WhatIs::__TOMBSTONE;
	_tombStone._forms.push_back 
		(form (0, __TOMBSTONEFORM, __TOMBSTONEASPECT, __TOMBSTONEASPECT));
	_tombStone._whatIs = General::WhatIs::__TOMBSTONE;
}

// ---
void General::assignKeys ()
{
	// The keys to open the doors...
	// First: Rooms where the yellow key can be...
	_yellowKeyRooms.resize (1);
	_yellowKeyRooms [0] = 102;

	// Second: Rooms where the green key can be...
	_greenKeyRooms.resize (8);
	_greenKeyRooms [0] = 5;
	_greenKeyRooms [1] = 6;
	_greenKeyRooms [2] = 7;
	_greenKeyRooms [3] = 109;
	_greenKeyRooms [4] = 37;
	_greenKeyRooms [5] = 36;
	_greenKeyRooms [6] = 35;
	_greenKeyRooms [7] = 34;

	// Third: Rooms where the red key can be...
	_redKeyRooms.resize (8);
	_redKeyRooms [0] = 23;
	_redKeyRooms [1] = 19;
	_redKeyRooms [2] = 9;
	_redKeyRooms [3] = 13;
	_redKeyRooms [4] = 137;
	_redKeyRooms [5] = 135;
	_redKeyRooms [6] = 128;
	_redKeyRooms [7] = 133;

	// Third: Rooms where the cyan (white) key can be...
	_cyanKeyRooms.resize (8);
	_cyanKeyRooms [0] = 83;
	_cyanKeyRooms [1] = 143;
	_cyanKeyRooms [2] = 65;
	_cyanKeyRooms [3] = 148;
	_cyanKeyRooms [4] = 51;
	_cyanKeyRooms [5] = 145;
	_cyanKeyRooms [6] = 57;
	_cyanKeyRooms [7] = 76;

	// The exit key is splitted into three pieces
	// ...and it can be in each of any group of three...
	_exitKeyRooms.resize (8);
	for (int i = 0; i < 8; i++) _exitKeyRooms [i].resize (3);
	_exitKeyRooms [0][0] = 129;	_exitKeyRooms [0][1] = 69;	_exitKeyRooms [0][2] = 124;
	_exitKeyRooms [1][0] = 133;	_exitKeyRooms [1][1] = 73;	_exitKeyRooms [1][2] = 43;
	_exitKeyRooms [2][0] = 106;	_exitKeyRooms [2][1] = 59;	_exitKeyRooms [2][2] = 124;
	_exitKeyRooms [3][0] = 105;	_exitKeyRooms [3][1] = 113;	_exitKeyRooms [3][2] = 43;
	_exitKeyRooms [4][0] = 103;	_exitKeyRooms [4][1] = 133;	_exitKeyRooms [4][2] = 124;
	_exitKeyRooms [5][0] = 104;	_exitKeyRooms [5][1] = 127;	_exitKeyRooms [5][2] = 43;
	_exitKeyRooms [6][0] = 77;	_exitKeyRooms [6][1] = 115;	_exitKeyRooms [6][2] = 124;
	_exitKeyRooms [7][0] = 23;	_exitKeyRooms [7][1] = 16;	_exitKeyRooms [7][2] = 43;
}

// ---
General::FormDefinition General::form (int id, int f, int iF, int fF, Orientation o,
		const QGAMES::Position& a)
{
	General::FormDefinition result;
	result._id = id;
	result._form = f;
	result._initialFrame = iF; result._finalFrame = fF;
	result._adjust = a;
	result._orientation = o;
	return (result);
}

