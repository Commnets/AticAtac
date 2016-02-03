#include "GameStates.hpp"
#include "Game.hpp"
#include "InputHandler.hpp"

// ---
AticAtacPresentationText::AticAtacPresentationText (const std::string& t)
		: QGAMES::ScoreObjectText (0, t, QGAMES::Forms (), 0)
{
	_forms.insert (QGAMES::Forms::value_type (__LETTERSFORM, 
		QGAMES::Game::game () -> form (__LETTERSFORM)));
	setText (t);
}

// ---
void AticAtacGameStateLoading::onEnter ()
{
	// The counter to control how long the 
	// screen is a loading mode...
	_counter = 0;
	// The background used during this state
	_background = _game -> form (__BACKGROUNDFORM);
	// The logo used during this state...
	_logo = _game -> form (__COMMNETSFORM);
	// The text used...
	_text = new AticAtacPresentationText (std::string ("COPYRIGHT 2015 Ignacio Cea Fornies"));
	_text -> setSpace (-2); // Letters are tight!!
	// Not fadding 
	_fadeOut = false;
	// The counter for alpha channel to the begging...
	_counterAlpha = 255;

	// There is no active world...so far
	(__AGM _game) -> setWorld (__MININT__);
	// There is no score objects...
	(__AGM _game) -> removeScoreObjects ();
}

// ---
void AticAtacGameStateLoading::updatePositions ()
{
	// Two seconds in this state...
	// Before fading the title out...
	if (_counter++ > (2 * _game -> framesPerSecond ()))
		_fadeOut = true;

	// If fadding out...
	if (_fadeOut)
	{
		// No more than 10 seconds...
		if (_counter++ > (10 * _game -> framesPerSecond ())) 
			_game -> setState (std::string (__GAMESTATEINITIALNAME));

		// ...or until dissapear...
		_counterAlpha = _counterAlpha - 2; 
		if (_counterAlpha < 0)
			_game -> setState (std::string (__GAMESTATEINITIALNAME));
	}
}

// ---
void AticAtacGameStateLoading::drawOn (QGAMES::Screen* s)
{
	// Display the background...a very scared one!
	_background -> drawOn (s, 0, QGAMES::Position::_cero, 100);
	// During this state the logo of Ceasoftware is displayed at the bootm
	QGAMES::Position lPos = QGAMES::Position 
		(__BD 10, __BD (__SCREENHEIGHT__ - _logo -> frameHeight () - 10), __BD 0); 
	_logo -> drawOn (s, 0, lPos, _counterAlpha);
	// Write down the text, next to the logo...
	_text -> setAlphaLevel (_counterAlpha);
	_text -> drawOn (s, lPos + QGAMES::Position (__BD (_logo -> frameWidth () + 10), 
		__BD ((_logo -> frameHeight () - _text -> height ()) / 2), __BD 0));
}

// ---
void AticAtacGameStateLoading::onExit ()
{
	delete _text;
}

// ---
void AticAtacGameStateInitial::onEnter ()
{
	// The counter to count every pixel movement...
	_counter = 0;
	// To count in which pixel the the instructios are...
	_position = 0;
	// The instructuctios...
	_instructions = _game -> form (__INSTRUCTIONSFORM);
	// The max pixel possible...
	_maxPosition = _instructions -> frameHeight ();

	// The background used during this state
	_background = _game -> form (__BACKGROUNDFORM);
	// ...and the logo used during this state...
	_logo = _game -> form (__COMMNETSFORM);

	// It doesn't want to exit...so far
	_wantToExit = false;

	// There is no active world...so far
	(__AGM _game) -> setWorld (__MININT__);
	// There is no score objects...
	(__AGM _game) -> removeScoreObjects ();

	// The sound whilst the instructions are in the screen...
	_soundPlaying = __TITLESOUND + (rand () % __MAXTITLESOUNDS);
	_game -> sound (_soundPlaying) -> play (__GENERICMUSICCHANNELSOUND);
}

// ---
void AticAtacGameStateInitial::updatePositions ()
{
	// The instructios moves 4 pixels every second...
	if (_counter++ > (_game -> framesPerSecond () / 20))
	{
		_counter = 0;
		if (_position++ > _maxPosition)
			_wantToExit = true;
	}

	// if the right key has been selected,
	// It is time to move to the next state...
	if (_wantToExit)
		_game -> setState (std::string (__GAMESTATESELECTNAME));
}

// ---
void AticAtacGameStateInitial::drawOn (QGAMES::Screen* s)
{
	// Draw the background and the logo...
	_background -> drawOn (s, 0, QGAMES::Position::_cero, 100); // A little bit fade - out!
	QGAMES::Position lPos = QGAMES::Position 
		(__BD 10, __BD (__SCREENHEIGHT__ - _logo -> frameHeight () - 10), __BD 0); 
	_logo -> drawOn (s, 0, lPos);

	// Now it draws a big box with paralel lines around the instructios...
	QGAMES::Position iPos (__BD (__SCREENWIDTH__ - _instructions -> frameWidth () - 80),
		__BD 10, __BD 0);
	int ml = __SCREENHEIGHT__ - 10 - 10;
	int l = _instructions -> frameHeight () - _position;
	if (l > ml) l = ml;
	s -> drawForm (iPos, _instructions, 0, _position, _instructions -> frameWidth (), l);
}

// ---
void AticAtacGameStateInitial::onExit ()
{
	// Stop the sound...
	_game -> sound (_soundPlaying) -> stop ();
}

// ---
AticAtacGameStateSelect::AticAtacGameStateSelect (QGAMES::Game* g)
	: QGAMES::GameState (__GAMESTATESELECT, g),
	  _logo (NULL),
	  _options (),
	  _optionLevels (),
	  _optionGraphics (),
	  _optionForms (),
	  _optionAspects (),
	  _currentOption (0),
	  _currentOptionLevel (0),
	  _counterMovement (0),
	  _positionOption (0),
	  _movingToOption (0),
	  _figure (0),
	  _typeOfControl (0),
	  _typePlayer (AticAtacCharacter::Type::__KNIGHT),
	  _blinkAttr (50),
	  _blinkDirection (2),
	  _counterNoActivity (0),
	  _wantToExit (false)
{
	// Nothing else to do...
}

// ---
void AticAtacGameStateSelect::nextOption ()
{
	_counterNoActivity = 0; // Something has been pressed...
	if (_movingToOption != 0)
		return;
	if (_currentOption < (__NUMBEROFOPTIONS - 1))
		_movingToOption = 1;
}

// ---
void AticAtacGameStateSelect::previousOption ()
{
	_counterNoActivity = 0; // Something has been pressed...
	if (_movingToOption != 0)
		return;
	if (_currentOption > 0)
		_movingToOption = -1;
}

// ---
void AticAtacGameStateSelect::optionSelected ()
{
	_counterNoActivity = 0; // Something has been pressed...
	if (_movingToOption != 0)
		return;
	if (_currentOption == 0)
		_typeOfControl = 0;
	if (_currentOption == 1)
		_typeOfControl = 1;
	if (_currentOption == 2)
		_typePlayer = AticAtacCharacter::Type::__KNIGHT;
	if (_currentOption == 3)
		_typePlayer = AticAtacCharacter::Type::__WIZARD;
	if (_currentOption == 4)
		_typePlayer = AticAtacCharacter::Type::__SERVANT;
	if (_currentOption == 6)
		_wantToExit = true;

	if (_currentOption == 5)
	{
		if (++_currentOptionLevel >= __MAXNUMBEROFLEVELS)
			_currentOptionLevel = 0;
		_options [5] = _optionLevels [_currentOptionLevel];
		delete _optionGraphics [5];
		_optionGraphics [5] = new AticAtacPresentationText (_options [5]);
		_optionGraphics [5] -> setSpace (0);
	}
}

// ---
void AticAtacGameStateSelect::optionAt (const QGAMES::Position& pos)
{
	_counterNoActivity = 0;
	QGAMES::Position oPos (__BD (__SCREENWIDTH__ - 410), __BD 40, __BD 0);
	for (int i = 0; i < __NUMBEROFOPTIONS; i++)
	{
		QGAMES::Position p1C = oPos + 
			(__BD (i * 60) * QGAMES::Vector (__BD 0, __BD 1, __BD 0));
		QGAMES::Position p2C = p1C + QGAMES::Vector (__BD 350, __BD 54, __BD 0);
		QGAMES::Rectangle rOpt (p1C, p2C);
		if (rOpt.hasIn (pos))
			_currentOption = i;
	}
}

// ---
void AticAtacGameStateSelect::onEnter ()
{
	// The background used during this state
	_background = _game -> form (__BACKGROUNDFORM);
	// ...and the logo used during this state...
	_logo = _game -> form (__COMMNETSFORM);

	// Define the options related with the level...
	_optionLevels.resize (__MAXNUMBEROFLEVELS);
	_optionLevels [0] = std::string ("Easy");
	_optionLevels [1] = std::string ("Normal");
	_optionLevels [2] = std::string ("Difficult");

	// Define the options...
	_options.resize (__NUMBEROFOPTIONS);
	_options [0] = std::string ("Keyboard");
	_options [1] = std::string ("Joystick");
	_options [2] = std::string ("Knight");
	_options [3] = std::string ("Wizard");
	_options [4] = std::string ("Servant");
	_options [5] = _optionLevels [_currentOptionLevel];
	_options [6] = std::string ("Start game");

	// ...and the equivalent graphics...
	_optionGraphics.resize (__NUMBEROFOPTIONS);
	for (int i = 0; i < __NUMBEROFOPTIONS; i++)
	{
		_optionGraphics [i] = new AticAtacPresentationText (_options [i]);
		_optionGraphics [i] -> setSpace (0);
	}

	// ..and also the graphics in the left...
	_optionForms.clear ();
	_optionForms.resize (__NUMBEROFOPTIONS);
	_optionForms [0] = _game -> form (__KEYBOARDFORM);
	_optionForms [1] = _game -> form (__JOYSTICKFORM);
	_optionForms [2] = _game -> form (__PLAYERFORM);
	_optionForms [3] = _game -> form (__PLAYERFORM);
	_optionForms [4] = _game -> form (__PLAYERFORM);
	_optionForms [5] = _game -> form (__LEVELFORM);
	_optionForms [6] = _game -> form (__GOFORM);

	// ...and thier respectively forms...
	_optionAspects.clear ();
	_optionAspects.resize (__NUMBEROFOPTIONS);
	_optionAspects [0] = _optionAspects [1] = _optionAspects [5] = _optionAspects [6];
	_optionAspects [2] = 0;
	_optionAspects [3] = 32;
	_optionAspects [4] = 64;

	// It doesn't want to exit...so far
	_wantToExit = false;
	// The counter movement is set to 0
	_counterMovement = 0;
	// No movement to none option...
	_movingToOption = 0;
	// No counter in place to execute the movement
	_positionOption = 0;
	// Type of control selected: Keyboard or Joystick?
	// First time it will be keyboard, but after first game who knows?
	_typeOfControl = ((InputHandler*) _game -> inputHandler ()) -> isJoystickActive () ? 1 : 0; 
	// Type of player selected
	_typePlayer = AticAtacCharacter::Type::__KNIGHT;
	// Blinking attribute
	_blinkAttr = 50;
	// The dircetion is up...
	_blinkDirection = 2;
	// The counter of no activity starts...
	_counterNoActivity = 0;

	// Sets up the score objects...
	(__AGM _game) -> addScoreObjects ();
	// Sets up the world...
	(__AGM _game) -> setWorld (__ATICATACWORLD);
	// Reset everything...
	((AticAtacGame*) _game) -> resetGame ();

	// But this state has nothing to show...
	(__AGM _game) -> setWorld (__MININT__);
	// ...even no score objects...
	(__AGM _game) -> removeScoreObjects ();
}

// ---
void AticAtacGameStateSelect::updatePositions ()
{
	// The things move...
	if (++_counterMovement > 6)
	{
		_counterMovement = 0;
		if (++_figure > 7)
			_figure = 0;
	}

	// Moves the rectangle around the option...
	if (_movingToOption != 0)
	{
		_positionOption += 4;
		if (_positionOption > 60)
		{
			_positionOption = 0;
			if (_movingToOption == 1)
				_currentOption++;
			else
				_currentOption--;
			_movingToOption = 0;
		}
	}

	// Blinks...
	_blinkAttr += _blinkDirection;
	if (_blinkAttr > 150) _blinkAttr = 150;
	if (_blinkAttr < 50) _blinkAttr = 50;
	if (_blinkAttr == 150 || _blinkAttr == 50)
		_blinkDirection *= -1;

	// if the right key has been selected,
	// It is time to move to the next state...
	if (_wantToExit)
		_game -> setState (std::string (__GAMESTATESTARTSNAME));

	// Counts the no activity...
	if (++_counterNoActivity > (10 * _game -> framesPerSecond ()))
		_game -> setState (std::string (__GAMESTATEDEMONAME));
}

// ---
void AticAtacGameStateSelect::drawOn (QGAMES::Screen* s)
{
	// Draw the background and the logo...
	_background -> drawOn (s, 0, QGAMES::Position::_cero, 100); // A little bit fade - out!
	QGAMES::Position lPos = QGAMES::Position 
		(__BD 10, __BD (__SCREENHEIGHT__ - _logo -> frameHeight () - 10), __BD 0); 
	_logo -> drawOn (s, 0, lPos);

	// The original position of the options..
	QGAMES::Position oPos (__BD (__SCREENWIDTH__ - 410), __BD 40, __BD 0);

	// Draws a full rectangle around the current options selected
	// The color of the rectangle is gray...
	QGAMES::Position p1A = oPos + 
		(__BD ((_typeOfControl == 0) ? 0 : 1) * 60) * QGAMES::Vector (__BD 0, __BD 1, __BD 0);
	QGAMES::Position p2A = p1A + QGAMES::Vector (__BD 350, __BD 54, __BD 0);
	s -> drawRectangle (QGAMES::Rectangle (p1A, p2A), QGAMES::Color (_blinkAttr, _blinkAttr, _blinkAttr), true);
	s -> drawRectangle (QGAMES::Rectangle (p1A, p2A), QGAMES::Color (255, 255, 255));
	QGAMES::Position p1B = oPos + 
		(__BD (((_typePlayer == AticAtacCharacter::Type::__KNIGHT) ? 2 
			: ((_typePlayer == AticAtacCharacter::Type::__WIZARD) ? 3 : 4)) * 60) 
				* QGAMES::Vector (__BD 0, __BD 1, __BD 0));
	QGAMES::Position p2B = p1B + QGAMES::Vector (__BD 350, __BD 54, __BD 0);
	s -> drawRectangle (QGAMES::Rectangle (p1B, p2B), QGAMES::Color (_blinkAttr, _blinkAttr, _blinkAttr), true);
	s -> drawRectangle (QGAMES::Rectangle (p1B, p2B), QGAMES::Color (255, 255, 255));

	// Draws the options...
	for (int i = 0; i < __NUMBEROFOPTIONS; i++)
	{
		QGAMES::Vector aPos (__BD 0, __BD (i * 60), __BD 0);
		_optionForms [i] -> drawOn (s, (i == 2 || i == 3 || i == 4) 
			? _optionAspects [i] + _figure : 0, oPos + aPos);
		_optionGraphics [i] -> 
			drawOn (s, oPos + aPos + QGAMES::Vector (__BD 60, __BD 10, __BD 0));
	}
	
	// Draws a while rectangle around the selected option...
	QGAMES::Position p1C = oPos + 
		(__BD (_currentOption * 60) * QGAMES::Vector (__BD 0, __BD 1, __BD 0)) + 
			(__BD _movingToOption * QGAMES::Vector (__BD 0, __BD _positionOption, __BD 0));
	QGAMES::Position p2C = p1C + QGAMES::Vector (__BD 350, __BD 54, __BD 0);
	s -> drawRectangle (QGAMES::Rectangle (p1C, p2C), QGAMES::Color (255,255,255), false);
}

// ---
void AticAtacGameStateSelect::onExit ()
{
	// Deletes all the reserved things...
	for (int i = 0; i < __NUMBEROFOPTIONS; i++)
		delete (_optionGraphics [i]);
	_options.clear ();
	_optionGraphics.clear ();
	_optionForms.clear ();

	// It is time to select the type of player...
	((AticAtacGame*) _game) -> setPlayerType (_typePlayer);
	// It is time to select the level...
	((AticAtacGame*) _game) -> setLevel (_currentOptionLevel);
	// Select the type of control: joystick or keyboard?
	((AticAtacGame*) _game) -> setJoystick ((_typeOfControl == 0) ? false : true);
}

// ---
void AticAtacGameStateDemo::onEnter ()
{
	// It doesn't want to exit...so far
	_wantToExit = false;
	// The counter to 0
	_counter = 0;

	// The background has to appear...
	(__AGM _game) -> setWorld (__ATICATACWORLD);
	// ...and the score objects to...
	(__AGM _game) -> addScoreObjects ();
	// The energy is now back...
	((AticAtacGame*) _game) -> setEnergyAvailable (0); 
	// Switch to invisible the main character...
	((AticAtacCharacter*) (__AGM _game) -> artist (__ENTITYPLAYER)) 
		-> setVisible (false);
	// Sets the initial room...
	((AticAtacGame*) _game) -> setRoomNumber (__FIRSTROOM);

	// The sound whilst the instructions are in the screen...
	_soundPlaying = __TITLESOUND + (rand () % __MAXTITLESOUNDS);
	_game -> sound (_soundPlaying) -> play (__GENERICMUSICCHANNELSOUND);
}

// ---
void AticAtacGameStateDemo::updatePositions ()
{
	// See if it's time to change the room visited...
	if (++_counter > (5 * _game -> framesPerSecond ())) // Every two seconds...
	{
		_counter = 0;
		int nR = rand () % __NUMBEROFROOMS__;
		if (nR == __FALLINGROOM) nR = 0; // The falling room is not shown...
		((AticAtacGame*) _game) -> setRoomNumber (nR);
	}

	// if the right key has been selected,
	// It is time to move back to the selection state...
	if (_wantToExit)
		_game -> setState (std::string (__GAMESTATESELECTNAME));
}

// ---
void AticAtacGameStateDemo::onExit ()
{
	// The background has to disappear...
	(__AGM _game) -> setWorld (__MININT__);
	// ...and the score objects too...
	(__AGM _game) -> removeScoreObjects ();

	// Stop the sound...
	_game -> sound (_soundPlaying) -> stop ();
}

// ---
void AticAtacGameStateArrive::onEnter ()
{
	// This is a simple counter to count 
	// how long the player is in this state...
	_counter = 0;
	// ...and which is the part of the player drawn...
	_percentage = 0;
	// A counter to mark when to grow
	_counterBeforeGrowing = 0;
	// To indicate whether the player can or not grow...
	_canGrow = false;

	// The player with the right form to grow...
	_player = _game -> form (__KNIGHTFORM + (int) (((AticAtacGame*) _game) -> playerType ()));

	// The background has to appear...
	(__AGM _game) -> setWorld (__ATICATACWORLD);
	// ...and the score objects to...
	(__AGM _game) -> addScoreObjects ();
	// The energy is now back...
	((AticAtacGame*) _game) -> setEnergyAvailable (0); 
	// Switch to invisible the main character...
	((AticAtacCharacter*) (__AGM _game) -> artist (__ENTITYPLAYER)) 
		-> setVisible (false); 

	// A simple sound to indicate that the player is growing up...
	_game -> sound (__PLAYERAPPEARS) -> play (__GENERICMUSICCHANNELSOUND);
}

// ---
void AticAtacGameStateArrive::updatePositions ()
{
	// Before growing there is time doing nothing...
	// Exactly three seconds...
	if (_counterBeforeGrowing++ > (2 * _game -> framesPerSecond ()))
		_canGrow = true;

	// ...which part of the player is visible...
	if (_canGrow)
	{
		_percentage++;
		if (_percentage > 100) 
			_percentage = 100;
	}
	
	// ...time limit?
	// Two seconds is the time the player takes to graw...
	if (_counter++ > (4 * _game -> framesPerSecond ()))
		_game ->setState (std::string (__GAMESTATEPLAYINGNAME));
}

// ---
void AticAtacGameStateArrive::drawOn (QGAMES::Screen* s)
{
	int pos = (int) (_player -> frameHeight () * (_percentage / 100.0));
	QGAMES::Position pP = ((AticAtacGame*) _game) -> centerOfCurrentRoom () - 
		QGAMES::Vector (__BD (_player -> frameWidth () / 2), 
			__BD (_player -> frameHeight () / 2), __BD 0);
	s -> drawForm (pP + QGAMES::Vector (__BD 0, __BD (_player -> frameHeight () - pos), __BD 0), 
			_player, 0, _player -> frameHeight () - pos, _player -> frameWidth (), pos);
}

// ---
void AticAtacGameStateArrive::onExit ()
{
	// Just in case...
	// It is suppossed the sound has finished at this point...
	_game -> sound (__PLAYERAPPEARS) -> stop ();
}

// ---
void AticAtacGameStatePlaying::onEnter ()
{
	// Sets the world to play...
	// Probably this call is not needed as the previous state did invoke it.
	// But it is used just to confirm...
	(__AGM _game) -> setWorld (__ATICATACWORLD);
	// There is finally score objects...
	// The same, probably this invocation is not needed...
	(__AGM _game) -> addScoreObjects ();
	// Re-set the number of the room...
	((AticAtacWorld*) (__AGM _game) -> activeWorld ()) 
		-> setRoomNumber (((AticAtacGame*) _game) -> roomNumber ());

	// The player is now visible...
	// Switch to invisible the main character...
	((AticAtacCharacter*) (__AGM _game) -> artist (__ENTITYPLAYER)) 
		-> setVisible (true);
	// ...and the aspect is the one defined at game level...
	((AticAtacGame*) _game) -> setPlayerType (((AticAtacGame*) _game) -> playerType ());
}

// ---
void AticAtacGameStatePlaying::drawOn (QGAMES::Screen* s)
{
	if (_game -> isGamePaused ())
	{
		QGAMES::Form* f = _game -> form (__PAUSEFORM);
		f -> drawOn (s, 0, ((AticAtacGame*) _game) -> centerOfCurrentRoom () - 
			QGAMES::Vector (__BD (f -> frameWidth () / 2), 
				__BD (f -> frameHeight () / 2), __BD 0), 255);
	}
}

// ---
void AticAtacGameStateFalling::onEnter ()
{
	// Starts to count...
	_counter = 0;

	// The room number where everything happens it the room number 150
	((AticAtacGame*) _game) -> setRoomNumber (__FALLINGROOM);
	// The player is not visible...
	((AticAtacCharacter*) (__AGM _game) -> artist (__ENTITYPLAYER)) 
		-> setVisible (false);

	// The sound...
	// A simple sound to indicate that the player is faling...
	_game -> sound (__PLAYERFALLING) -> play (__PLAYERFALLINGCHANNEL);
}

// ---
void AticAtacGameStateFalling::updatePositions ()
{
	// Just to count...
	if (_counter++ > (2 * _game -> framesPerSecond ()))
		_game -> setState (std::string (__GAMESTATEPLAYINGNAME));
}

// ---
void AticAtacGameStateFalling::onExit ()
{
	((AticAtacGame*) _game) -> setRoomNumber (_roomNumberTarget);
	((AticAtacCharacter*) (__AGM _game) -> artist (__ENTITYPLAYER)) -> 
		setPosition (_playerPosition);
}

// ---
void AticAtacGameStateDie::onEnter ()
{
	// A counter to count the number of seconds in this state...
	_counter = 0;

	// Plays the sound dieing...
	_game -> sound (__PLAYERDIES) -> play (__PLAYERDIEING);

	// The world still exists...
	// ..and the room is the same the player is in...
	// but it has to be re-set
	((AticAtacWorld*) (__AGM _game) -> activeWorld ()) 
		-> setRoomNumber (((AticAtacGame*) _game) -> roomNumber ());
	// Switch to invisible the main character...
	((AticAtacCharacter*) (__AGM _game) -> artist (__ENTITYPLAYER)) 
		-> setVisible (false);
}

// ---
void AticAtacGameStateDie::updatePositions ()
{
	// After 3 seconds, it is time to decide whether the game 
	// finishes or continues...
	if (_counter++ > (3 * _game -> framesPerSecond ()))
	{
		if (((AticAtacGame*) _game) -> numberOfLives () == 1) // It was the last live...
			((AticAtacGame*) _game) -> // ...so the game finishes!
				setState (std::string (__GAMESTATEENDNAME));
		else
		{
			// Only in real mode, the lives go down!
			#ifdef NDEBUG
			((AticAtacGame*) _game) -> 
				setNumberOfLives (((AticAtacGame*) _game) -> numberOfLives () - 1);
			#endif
			((AticAtacGame*) _game) -> // The game starts back again!
				setState (std::string (__GAMESTATESTARTSNAME));
		}
	}
}

// ---
void AticAtacGameStateDie::drawOn (QGAMES::Screen* s)
{
	// There is nothing to draw...so far...
}

// ---
void AticAtacGameStateDie::onExit ()
{
	// Definetively at this point, the sound has finished...
	// So this instruction is just in case
	_game -> sound (__PLAYERDIES) -> stop ();
}

// ---
void AticAtacGameStateEnd::onEnter ()
{
	// The player at this tate is invisible...
	// The a big label at the middle appear...
	// and the system wait until the user presse the enter key to start again...
	_label = _game -> form (__ENDFORM);

	// The counter and the direction for alpha...
	_alphaCounter = 150;
	_dirAlpha = -3;

	// Don't want to exit..
	_wantToExit = false;

	// The sound of the end of the game starts to play...
	_soundPlaying = __TITLESOUND + (rand () % __MAXTITLESOUNDS);
	_game -> sound (_soundPlaying) -> play (__GENERICMUSICCHANNELSOUND);
}

// ---
void AticAtacGameStateEnd::updatePositions ()
{
	_alphaCounter = _alphaCounter - _dirAlpha;
	if (_alphaCounter > 150) 
		{ _alphaCounter = 150; _dirAlpha = -_dirAlpha; }
	if (_alphaCounter < 50) 
		{ _alphaCounter = 50; _dirAlpha = -_dirAlpha; }

	// if the right key has been selected,
	// It is time to move to the initial state
	if (_wantToExit)
		_game -> setState (std::string (__GAMESTATESELECTNAME));
}

// ---
void AticAtacGameStateEnd::drawOn (QGAMES::Screen* s)
{
	_label -> drawOn (s, 0, ((AticAtacGame*) _game) -> centerOfCurrentRoom () - 
		QGAMES::Vector (__BD (_label -> frameWidth () / 2), 
			__BD (_label -> frameHeight () / 2), __BD 0), _alphaCounter);
}

// ---
void AticAtacGameStateEnd::onExit ()
{
	_game -> sound (_soundPlaying) -> stop ();
}

// ---
void AticAtacGameStateWin::onEnter ()
{
	// The player at this tate is invisible...
	// The a big label at the middle appear...
	// and the system wait until the user presse the enter key to start again...
	_label = _game -> form (__WINFORM);

	// The counter and the direction for alpha...
	_alphaCounter = 150;
	_dirAlpha = -3;

	// Don't want to exit...
	_wantToExit = false;

	// The sound of the end of the game starts to play...
	_soundPlaying = __TITLESOUND + (rand () % __MAXTITLESOUNDS);
	_game -> sound (_soundPlaying) -> play (__GENERICMUSICCHANNELSOUND);
}

// ---
void AticAtacGameStateWin::updatePositions ()
{
	_alphaCounter = _alphaCounter - _dirAlpha;
	if (_alphaCounter > 150) 
		{ _alphaCounter = 150; _dirAlpha = -_dirAlpha; }
	if (_alphaCounter < 50) 
		{ _alphaCounter = 50; _dirAlpha = -_dirAlpha; }

	// if the right key has been selected,
	// It is time to move to the initial state
	if (_wantToExit)
		_game -> setState (std::string (__GAMESTATESELECTNAME));
}

// ---
void AticAtacGameStateWin::drawOn (QGAMES::Screen* s)
{
	_label -> drawOn (s, 0, ((AticAtacGame*) _game) -> centerOfCurrentRoom () - 
		QGAMES::Vector (__BD (_label -> frameWidth () / 2), 
			__BD (_label -> frameHeight () / 2), __BD 0), _alphaCounter);
}

// ---
void AticAtacGameStateWin::onExit ()
{
	_game -> sound (_soundPlaying) -> stop ();
}

