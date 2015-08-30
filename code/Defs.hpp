#ifndef __DEFS__
#define __DEFS__

#include <SDL.h>

// Definition files...
#define __ENTITIESFILE__					"entities.xml"
#define __MOVEMENTSFILE__					"movements.xml"
#define __FORMSFILE__						"forms.xml"
#define __OBJECTSFILE__						"objects.xml"
#define __SOUNDSFILE__						"sounds.xml"
#define __WORLDSFILE__						"worlds.xml"
#define __MAPSFILE__						"maps.xml"

// Main parameters of the screen game...
#define __GAMESNAME__						"ICF AticAtac Game"
#define __SCREENXPOS__						50  // Where the room starts in the x axis
#define __SCREENYPOS__						50  // Where the room starts in the y axis
#define __SCREENWIDTH__						690 // (256 * 2.5) + 50
#define __SCREENHEIGHT__					480 // (192 * 2.5)
#define __GRAYCOLOR							QGAMES::Color (50,50,50) // To surround things...
#define __MAXNUMBEROFLIVES					3
#define __MAXNUMBEROFLEVELS					3
#define __NUMBEROFOPTIONS					7
#define __INITIALNUMBEROFLIVES				3
#define __MAXNUMBEROFCARRYTHINGS			3
#define __MAXNUMBEROFSIMULTANEOUSSHOOTS		2
#define __SPEEDLOSSINGLIVE					1 // Per second...

// Main definitions of the game
#define __NUMBEROFROOMS__					151
#define __NUMBEROFROOMTYPES__				13
#define __NUMBEROFOBJECTS__					548
#define __NUMBEROFTHINGS					119 // 112 + 4 + 3 (Colored keys and part of the main key)
#define __FIRSTROOM							0
#define __FALLINGROOM						150 // This is drawn in a different way...(see Map related classes)

// The definitions of the main game objects...in the original game
#define __ORIGINALCAVEDOOR					1  // 0x01
#define __ORIGINALNORMALDOORFRAME			2  // 0x02
#define __ORIGINALBIGDOORFRAME				3  // 0x03
#define __ORIGINALREDLOCKEDNORMALDOOR		8  // 0x08
#define __ORIGINALGREENLOCKEDNORMALDOOR		9  // 0x09
#define __ORIGINALWHITELOCKEDNORMALDOOR		10 // 0x0A
#define __ORIGINALYELLOWLOCKEDNORMALDOOR	11 // 0x0B
#define __ORIGINALREDLOCKEDCAVEDOOR			12 // 0x0C
#define __ORIGINALGREENLOCKEDCAVEDOOR		13 // 0x0D
#define __ORIGINALWHITELOCKEDCAVEDOOR		14 // 0x0E
#define __ORIGINALYELLOWLOCKEDCAVEDOOR		15 // 0x0F
#define __ORIGINALCLOCK						16 // 0x10
#define __ORIGINALDARKPICTURE				17 // 0x11
#define __ORIGINALTABLE						18 // 0x12
#define __ORIGINALFULLCHICKEN				19 // 0x13
#define __ORIGINALEMPTYCHICKEN				20 // 0x14
#define __ORIGINALANTLERTROPHY				21 // 0x15
#define __ORIGINALTROPHY					22 // 0x16
#define __ORIGINALBOOKCASE					23 // 0x17
#define __ORIGINALSHUTTRAPDOOR				24 // 0x18
#define __ORIGINALOPENTRAPEDOOR				25 // 0x19
#define __ORIGINALBARREL					26 // 0x1A
#define __ORIGINALRUG						27 // 0x1B
#define __ORIGINALACGSHIELD					28 // 0x1C
#define __ORIGINALSHIELD					29 // 0x1D
#define __ORIGINALKNIGHT					30 // 0x1E
#define __ORIGINALSHUTNORMALDOOR			32 // 0x20
#define __ORIGINALOPENNORMALDOOR			33 // 0x21
#define __ORIGINALSHUTCAVEDOOR				34 // 0x22
#define __ORIGINALOPENCAVEDOOR				35 // 0x23
#define __ORIGINALACGEXITDOOR				36 // 0x24
#define __ORIGINALLIGHTPICTURE				37 // 0x25
#define __ORIGINALSKELETON					38 // 0x26
#define __ORIGINALBARRELS					39 // 0x27

// The definition of the things to eat, catch, kill,...
#define __ORIGINALLEAF						161 // 0xA1
#define __ORIGINALCROSS						162 // 0xA2
#define __ORIGINALWRENCH					163 // 0xA3
#define __ORIGINALBOTTLE					164 // 0xA4
#define __ORIGINALFACECOIN					165 // 0xA5
#define __ORIGINALWING						166 // 0xA6
#define __ORIGINALWHIP						167 // 0xA7
#define __ORIGINALCLAW						168 // 0xA8
#define __ORIGINALDIAMOND					169 // 0xA9
#define __ORIGINALBAGOFMONEY				170 // 0xAA
#define __ORIGINALSKULL						171 // 0xAB
#define __ORIGINALBOTE						172 // 0xAC
#define __ORIGINALCANDY						173 // 0xAD
#define __ORIGINALLEG						174 // 0xAE
#define __ORIGINALLOLLIPOP					175 // 0xAF
#define __ORIGINALCONE						176 // 0xB0
#define __ORIGINALBOWL						177 // 0xB1
#define __ORIGINALAPPLE						178 // 0xB2
#define __ORIGINALMILK						179 // 0xB3
#define __ORIGINALMUSHROOM					180 // 0xB4
#define __ORIGINALMUMMY						181 // 0xB5
#define __ORIGINALFRANK						182 // 0xB6
#define __ORIGINALDEVIL						183 // 0xB7
#define __ORIGINALDRACULA					184 // 0xB8
#define __ORIGINALHUNCH						185 // 0xB9

// The aspects of the different things...
#define __THINGSTOCATCHFORM					301
#define __LEAFASPECT						18
#define __CROSSASPECT						16
#define __WRENCHASPECT						17
#define __BOTTLEASPECT						11
#define __FACECOINASPECT					12
#define __WINGASPECT						13
#define __WHIPASPECT						8
#define __DIAMONDASPECT						14
#define __BAGOFMONEYASPECT					9
#define __SKULLASPECT						15
#define __CLAWASPECT						10
#define __GREENKEYASPECT					2
#define __REDKEYASPECT						0
#define __YELLOWKEYASPECT					4
#define __CYANKEYASPECT						3
#define __EXITKEYASPECT						5
#define __CHARACTERFORM						600
#define __CHICKENFORM						107
#define __EMPTYCHICKEN						0
#define __FULLCHICKEN						1
#define __FRAMEFORM							501
#define __REDFRAME							0
#define __VIOLETFRAME						1
#define __GREENFRAME						2
#define __CYANFRAME							3
#define __COMMNETSFORM						505
#define __INSTRUCTIONSFORM					500
#define __TITLEFORM							504
#define __ENDFORM							506
#define __PAUSEFORM							513
#define __TIMESCOREFORM						507
#define __PLAYERFORM						600
#define __KNIGHTFORM						604
#define __WIZARDFORM						605
#define	__SERVANTFORM						606
#define __TOMBSTONEFORM						111
#define __TOMBSTONEASPECT					0
#define __MONSTERFORM						403
#define __MUMMYINITALASPECT					0
#define __MUMMYFINALASPECT					5
#define __DRACULAINITIALASPECT				6
#define __DRACULAFINALASPECT				11
#define __HUNCHINITIALASPECT				12
#define __HUNCHFINALASPECT					17
#define __DEVILINITIALASPECT				18
#define __DEVILFINALASPECT					23
#define __FRANKINITIALASPECT				24
#define __FRANKFINALASPECT					29
#define __MUSHROOMSFORM						404
#define __MUSHROOMASPECT					0
#define __MUSHROOMNUMBERASPECTS				3
#define __FOODFORM							300
#define __MILKASPECT						28
#define __APPLEASPECT						17
#define __APPLENUMBERASPECTS				3
#define __BOWLASPECT						10
#define __BOWLNUMBERASPECTS					5
#define __CONEASPECT						25
#define __CONENUMBERASPECTS					3
#define __POLIPOPASPECT						5
#define __POLIPOPNUMBERASPECTS				5
#define __LEGASPECT							15
#define __LEGNUMBERASPECTS					2
#define __CANDYASPECT						0
#define __CANDYNUMBERASPECTS				5
#define __BOTEASPECT						20
#define __BOTENUMBERASPECTS					3
#define __BARRELSFORM						105
#define __BARRELSUPASPECT					0
#define __BARRELSDOWNASPECT					1
#define __APPEARINGMONSTERSFORM				400
#define __APPEARINGINITIALASPECT			0
#define __APPEARINGFNALASPECT				5
#define __NUMBEROFAPPEARINGASPECTS			7
#define __DIEINGMONSTERDFORM				401
#define __DIEINGINITIALASPECT				0
#define __DIEINGFINALASPECT					11
#define __NUMBEROFDIEINGASPECTS				1
#define __APPEARINGMONSTERSLEFTFORM			401
#define __APPEARINGMONSTERSRIGHTFORM		402
#define __APPEARINGMONSTERNUMBERASPECTS		11
#define __APPEARINGMONSTERINITIALASPECT		12
#define __APPEARINGMONSTERFINALASPECT		17
#define __WEAPON1FORM						601
#define __WEAPON2FORM						602
#define __WEAPON3FORM						603
#define __LETTERSFORM						508
#define __BACKGROUNDFORM					509
#define __KEYBOARDFORM						510
#define __JOYSTICKFORM						511
#define __GOFORM							512
#define __LEVELFORM							514
#define __WINFORM							515

// The number of the special things...
#define __GREENKEYID						(__NUMBEROFTHINGS - 7)
#define __YELLOWKEYID						(__NUMBEROFTHINGS - 6)
#define __REDKEYID							(__NUMBEROFTHINGS - 5)
#define __CYANKEYID							(__NUMBEROFTHINGS - 4)
#define __EXITKEYID							(__NUMBEROFTHINGS - 3) // The first one...
#define __LEAFID							0
#define __CROSSID							1
#define __WRENCHID							2
#define __BOTLLEMILKID						3
#define __FACECOINID						4
#define __WINGID							5
#define __WHIPID							6
#define __CLAWID							7
#define __DIAMONDID							8
#define __BAGOFMONEYID						9
#define __SKULLID							10
#define __TOMBSTONEOBJID					30000

// The behaviours of the monsters...
#define __NUMBEROFMONSTERBAHAVIOURS			7
#define __APPEARINGBEHAVIOUR				0
#define __MUMMYBEHAVIOUR					1
#define __DEVILBEHAVIOUR					2
#define __DRACULABEHAVIOUR					3
#define __HUNCHBACKBEHAVIOUR				4
#define __FRANKESTEINBEHAVIOUR				5
#define __MUSHROOMBEHAVIOUR					6

// Different sounds...
#define __ENTERINGTHESCENESOUND				111
#define __MONSTERAPPEARINGBASESOUND			121
#define __OPENDOORSOUND						107
#define __CLOSEDOORSOUND					109
#define __PLAYERWALKING						101
#define __PLAYERHITSOUND					117
#define __PLAYEREATSFOOD					116
#define __PLAYERDIES						104
#define __PLAYERAPPEARS						128
#define __TITLESOUND						130
#define __MAXTITLESOUNDS					7 // Number of different sound for th title...
#define __FIRESOUND							115
#define __WEAPONKILLSOMETHING				101
#define __PLAYERFALLING						113

// The different channels
#define __GENERICMUSICCHANNELSOUND			1
#define __WALKINGCHANNELSOUND				2
#define __HITTINGCHANNELSOUND				3
#define __MONSTERAPPEARINGCHANNELSOUND		4
#define __EATINGCHANNELSOUND				2
#define __CATCHCHANNELSOUND					2
#define __PLAYERDIEING						2
#define __SHOOTCHANNLESOUND					5
#define __KILLCHANNELSOUND					6
#define __PLAYERFALLINGCHANNEL				5
#define __OPENCLOSEDOORCHANNEL				7

// The entities are of the game:
// The basic entity located in the background of the game...
#define __ENTITYBACKGROUNDBASE				100
#define __NUMBERMAXENTITIESBK				20
#define __ENTITYMONSTERBASE					200
#define __NUMBERMAXENTITIESMONSTER			10
#define __ENTITYEATENBASE					300
#define __NUMBERMAXENTITIESEAT				5
#define __ENTITYCATCHBASE					400
#define __NUMBERMAXENTITIESCATCH			10
#define __ENTITYPLAYER						500
#define __NUMBERMAXENTITIESPLAYER			3
#define __MAXNUMBEROFMONSTERSAPPEARING		5
#define __SCOREFRAMEOBJECT					800
#define __CHICKENOBJECT						801
#define __LIVESOBJECT						802
#define __THINGSOBJECT						803
#define __TIMECOUNTEROBJECT					804
#define __SCORECOUNTEROBJECT				805

// The movements of the game:
// The basic movement of the monsters in the room...
#define __MOVEMENTMONSTERINROOMBASE			10
// The movement of the main character
#define __MOVEMENTCHARACTER					100
// The movement of the weapon
#define __MOVEMENTWEAPON					101

// The events that the game produces...
#define __MONSTERAPPEARED					100
#define __MONSTERDIED						101
#define __PLAYERENTERINGDOOR				102
#define __CHANGECOLORFRAME					103
#define __SHOOTFINISH						104

// The ids of the buoys...
#define __TOCHANGEROOMBUOYID				100
#define __TOFFAILBUOYID						101
#define __TOFINISHBUOYID					102

// The game states:
// The game state when the thing is loading
#define __GAMESTATELOADING					0
#define __GAMESTATELOADINGNAME				"0"
// The game state when the player starts the game
#define __GAMESTATEINITIAL					1
#define __GAMESTATEINITIALNAME				"1"
// The game state to select the different possibilities of the game
#define __GAMESTATESELECT					2
#define __GAMESTATESELECTNAME				"2"
// The game state when the players arrives the castle
#define __GAMESTATESTARTS					3
#define __GAMESTATESTARTSNAME				"3"
// The game state when the players is playing
#define __GAMESTATEPLAYING					4
#define __GAMESTATEPLAYINGNAME				"4"
// The game state when the players is falling
#define __GAMESTATEFALLING					5
#define __GAMESTATEFALLINGNAME				"5"
// The game state when the players dies
#define __GAMESTATEPLAYERDIES				6
#define __GAMESTATEPLAYERDIESNAME			"6"
// The game state when no more lives left
#define __GAMESTATEEND						7
#define __GAMESTATEENDNAME					"7"
// The game is in a demo mode
#define __GAMESTATEDEMO						8
#define __GAMESTATEDEMONAME					"8"
// The player wins the game
#define __GAMESTATEWIN						9
#define __GAMESTATEWINNAME					"9"

// The worlds of the game:
// Basic world (there is only one)
#define __ATICATACWORLD						1

// The scenes of the game
// Basic scene (there is only one)
#define __ATICATACSCENE						11

// The maps of the scenes:
// THe basic map 
// There is only one that changes according with the room the player is in
#define __ATICATACMAP						111

#endif