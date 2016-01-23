#ifndef __GAMESTATES_HPP__
#define __GAMESTATES_HPP__

#include <Common/gamestate.hpp>
#include "Worlds.hpp"
#include "Defs.hpp"

/** A class to define text drawn
	during the presentation. */
class AticAtacPresentationText : public QGAMES::ScoreObjectText
{
	public:
	AticAtacPresentationText (const std::string& t = std::string (__NULL_STRING__));

	virtual Entity* clone ()
							{ return (new AticAtacPresentationText (_text)); }

	protected:
	/** @see parent. */
	virtual int formToDrawLetters ()
							{ return (__LETTERSFORM); }
};

/** When the game is loading. */
class AticAtacGameStateLoading : public QGAMES::GameState 
{
	public:
	AticAtacGameStateLoading (QGAMES::Game* g)
		: QGAMES::GameState (__GAMESTATELOADING, g),
		  _counter (0),
		  _background (NULL),
		  _logo (NULL),
		  _text (NULL),
		  _fadeOut (false),
		  _counterAlpha (255)
							{ }

	/** @see parent. */
	void onEnter ();
	void updatePositions ();
	void drawOn (QGAMES::Screen* s);
	void onExit ();

	private:
	/** To count how long the logo is in the screen... */
	int _counter; 
	/** The background to be shown. */
	QGAMES::Form* _background;
	/** The logo to be shown... */
	QGAMES::Form* _logo; 
	/** The text refering to the author. */
	AticAtacPresentationText* _text;
	/** The logo start to fade out after a couple of seconds.. */
	bool _fadeOut; 
	/** The level of fade out... */
	int _counterAlpha; 
};

/** When the game starts. */
class AticAtacGameStateInitial : public QGAMES::GameState 
{
	public:
	AticAtacGameStateInitial (QGAMES::Game* g)
		: QGAMES::GameState (__GAMESTATEINITIAL, g),
		  _counter (0),
		  _position (0),
		  _maxPosition (0),
		  _logo (NULL),
		  _instructions (NULL),
		  _wantToExit (false),
		  _soundPlaying (0)
							{ }

	void setWantToExit (bool e)
							{ _wantToExit = e; }

	/** @see parent. */
	void onEnter ();
	void updatePositions ();
	void drawOn (QGAMES::Screen* s);
	void onExit ();

	private:
	/** The counter between every movement of the title... */
	int _counter; 
	/** The position of the cursor within the title content 
		(between 0 and the height of the title) */
	int _position; 
	/** The max position...this is the height of the title sprite */
	int _maxPosition; 
	/** The background to be shown. */
	QGAMES::Form* _background;
	/** The logo form, shown in this state */
	QGAMES::Form* _logo; 
	/** The instructions moving in the screen */
	QGAMES::Form* _instructions; 

	// Implementation
	/** When the input handler detects an special key has been pressed
		and the game stands in this state this variable is set to true. */
	bool _wantToExit;
	/** The sound being playing is random. It is needed to keep it in order to stop
		it when the state ends. */
	int _soundPlaying;
};

/** When the user has to select the different options of the game. */
class AticAtacGameStateSelect : public QGAMES::GameState 
{
	public:
	AticAtacGameStateSelect (QGAMES::Game* g);

	void nextOption ();
	void previousOption ();
	bool isMovingToAnOption () const
							{ return (_movingToOption != 0); }
	void optionSelected ();
	void optionAt (const QGAMES::Position& pos);

	/** @see parent. */
	void onEnter ();
	void updatePositions ();
	void drawOn (QGAMES::Screen* s);
	void onExit ();

	private:
	/** The background to be shown. */
	QGAMES::Form* _background;
	/** The logo form, shown in this state */
	QGAMES::Form* _logo; 
	/** The options to be selected. */
	std::vector <std::string> _options;
	/** The levels potentially selected. */
	std::vector <std::string> _optionLevels;
	/** ...and the entites representing them. */
	std::vector <AticAtacPresentationText*> _optionGraphics;
	/** ...and the graphics in the left. */
	std::vector <QGAMES::Form*> _optionForms;
	/** ...and the aspects of the elements. */
	std::vector <int> _optionAspects;

	// Implementation
	/** The current option selected. */
	int _currentOption;
	/** The current option related with the level. */
	int _currentOptionLevel;
	/** Counter for the things moving... */
	int _counterMovement;
	/** The selection moves little by little.
		This parameters marks how much is it moving. */
	int _positionOption;
	/** To indicate the type of movement. */
	int _movingToOption;
	/** The figure that is being shown. */
	int _figure;
	/** The type of control selected: 0 keyboard, 1 joystick. */
	int _typeOfControl;
	/** The type of player selected. */
	AticAtacCharacter::Type _typePlayer;
	/** Blinking attribute. */
	int _blinkAttr;
	/** Direction of the blinking. */
	int _blinkDirection;
	/** Counting time with no activite.
		This is done to move the state toi a demo situation. */
	int _counterNoActivity;
	/** When the input handler detects an special key has been pressed
		and the game stands in this state this variable is set to true. */
	bool _wantToExit;
};

/** When no options are selected 
	the game set on this state to make a round the different rooms. */
class AticAtacGameStateDemo : public QGAMES::GameState
{
	public:
	AticAtacGameStateDemo (QGAMES::Game* g)
		: QGAMES::GameState (__GAMESTATEDEMO, g),
		  _wantToExit (false),
		  _soundPlaying (0),
		 _counter (0)
							{ }

	void setWantToExit (bool e)
							{ _wantToExit = e; }

	/** @see parent. */
	void onEnter ();
	void updatePositions ();
	void onExit ();

	// Implementation
	/** When the input handler detects an special key has been pressed
		and the game stands in this state this variable is set to true. */
	bool _wantToExit;
	/** The sound being playing is random. It is needed to keep it in order to stop
		it when the state ends. */
	int _soundPlaying;
	/** Counter to change the room visited. */
	int _counter;
};

/** When the player is arriving the castle. */
class AticAtacGameStateArrive : public QGAMES::GameState 
{
	public:
	AticAtacGameStateArrive (QGAMES::Game* g)
		: QGAMES::GameState (__GAMESTATESTARTS, g),
		  _counter (0),
		  _percentage (0),
		  _counterBeforeGrowing (0),
		  _canGrow (false),
		  _player (NULL)
							{ }

	/** @see parent. */
	void onEnter ();
	void updatePositions ();
	void drawOn (QGAMES::Screen* s);
	void onExit ();

	private:
	/** The time this state stands... */
	int _counter; 
	/** The percentage of the main player shown (from 0 to 100) */
	int _percentage; 
	/** The time between avery bit of increment... */
	int _counterBeforeGrowing; 
	/** Whether the player can or not grow (this happens at the third second) */
	bool _canGrow; 
	/** The form of the player... */
	QGAMES::Form* _player; 
};

/** When the player is moving into the castle. 
	When playing all actions are executed in the main loop within the game class. 
	The only thing needed is to set up the game. */
class AticAtacGameStatePlaying : public QGAMES::GameState 
{
	public:
	AticAtacGameStatePlaying (QGAMES::Game* g)
		: QGAMES::GameState (__GAMESTATEPLAYING, g)
							{ }

	/** @see parent. */
	void onEnter ();
	void drawOn (QGAMES::Screen* s);
};

/** When the player is falling in some trap. */
class AticAtacGameStateFalling : public QGAMES::GameState 
{
	public:
	AticAtacGameStateFalling (QGAMES::Game* g)
		: QGAMES::GameState (__GAMESTATEFALLING, g),
		  _counter (0),
		  _roomNumberTarget (0),
		  _playerPosition (QGAMES::Position::_cero)
							{ }

	/** To set the room number target. */
	void setRommNumberTarget (int nR)
							{ _roomNumberTarget = nR; }
	/** To set the player position. */
	void setPlayerPosition (const QGAMES::Position& p)
							{ _playerPosition = p; }

	/** @see parent. */
	void onEnter ();
	void updatePositions ();
	void onExit ();

	private:
	/** To count the number of seconds the player is in this state. */
	int _counter;

	// Implementation
	/** The room the player has to get after falling. */
	int _roomNumberTarget;
	/** The position of the player. */
	QGAMES::Position _playerPosition;
};

/** When the player dies. */
class AticAtacGameStateDie : public QGAMES::GameState 
{
	public:
	AticAtacGameStateDie (QGAMES::Game* g)
		: QGAMES::GameState (__GAMESTATEPLAYERDIES, g),
		  _counter (0)
							{ }

	/** @see parent. */
	void onEnter ();
	void updatePositions ();
	void drawOn (QGAMES::Screen* s);
	void onExit ();

	private:
	/** The time the game is in this situation 
		before moving back to the playing status */
	int _counter; 
};

/** The end. */
class AticAtacGameStateEnd : public QGAMES::GameState 
{
	public:
	AticAtacGameStateEnd (QGAMES::Game* g)
		: QGAMES::GameState (__GAMESTATEEND, g),
		  _alphaCounter (255),
		  _dirAlpha (-1),
		  _label (NULL),
		  _wantToExit (false),
		  _soundPlaying (0)
							{ }

	void setWantToExit (bool e)
							{ _wantToExit = e; }

	/** @see parent. */
	void onEnter ();
	void updatePositions ();
	void drawOn (QGAMES::Screen* s);
	void onExit ();

	private:
	/** Counter holding the alpha status of the game over label. */
	int _alphaCounter;
	/** Tp keep whether the alpha counter is going up or down. */
	int _dirAlpha;
	/** The label being shown with the game over text. */
	QGAMES::Form* _label;

	// Implementation
	/** When the input handler detects an special key has been pressed
		and the game stands in this state this variable is set to true. */
	bool _wantToExit;
	/** The sound being playing is random. It is needed to keep it in order to stop
		it when the state ends. */
	int _soundPlaying;
};

/** When the player wins the game. */
class AticAtacGameStateWin : public QGAMES::GameState 
{
	public:
	AticAtacGameStateWin (QGAMES::Game* g)
		: QGAMES::GameState (__GAMESTATEWIN, g),
		  _alphaCounter (255),
		  _dirAlpha (-1),
		  _label (NULL),
		  _wantToExit (false),
		  _soundPlaying (0)
							{ }

	void setWantToExit (bool e)
							{ _wantToExit = e; }

	/** @see parent. */
	void onEnter ();
	void updatePositions ();
	void drawOn (QGAMES::Screen* s);
	void onExit ();

	private:
	/** Counter holding the alpha status of the game over label. */
	int _alphaCounter;
	/** Tp keep whether the alpha counter is going up or down. */
	int _dirAlpha;
	/** The label being shown with the game over text. */
	QGAMES::Form* _label;

	// Implementation
	/** When the input handler detects an special key has been pressed
		and the game stands in this state this variable is set to true. */
	bool _wantToExit;
	/** The sound being playing is random. It is needed to keep it in order to stop
		it when the state ends. */
	int _soundPlaying;
};

#endif
