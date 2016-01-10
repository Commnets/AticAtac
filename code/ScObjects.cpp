#include "ScObjects.hpp"
#include <Common/game.hpp>
#include <sstream>

// ---
AticAtacChicken::AticAtacChicken ()
		: QGAMES::ScoreObject (__CHICKENOBJECT),
		  _percentage (0)
{
	// This is not really needed...
	// Just to avoid an error during the standard initialization
	// It is double checked that there is a form at least...
	_forms.insert (QGAMES::Forms::value_type (__CHICKENFORM, 
		QGAMES::Game::game () -> form (__CHICKENFORM)));
}

// ---
void AticAtacChicken::setPercentage (int p)
{
	_percentage = p;
	if (_percentage > 100) _percentage = 100;
	if (_percentage < 0) _percentage = 0;
}

// ---
void AticAtacChicken::initialize ()
{
	QGAMES::ScoreObject::initialize ();
	_percentage = 0;
}

// ---
void AticAtacChicken::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::Form* f = QGAMES::Game::game () -> form (__CHICKENFORM);
	int posF = (int) (f -> frameHeight () * (_percentage / 100.0));
	int pixF = f -> frameHeight () - posF;
	s -> drawForm (position (), f, 0, 0, f -> frameWidth (), posF);
	s -> drawForm (position () + QGAMES::Vector (__BD 0, __BD posF, __BD 0), 
		f, f -> frameWidth (), posF, f -> frameWidth (), pixF);
}

// ---
AticAtacScoreFrame::AticAtacScoreFrame ()
		: QGAMES::ScoreObject (__SCOREFRAMEOBJECT),
		  _color (0)
{
	// This is not really needed...
	// Just to avoid an error during the standard initialization
	// It is double checked that there is a form at least...
	_forms.insert (QGAMES::Forms::value_type (__FRAMEFORM, 
		QGAMES::Game::game () -> form (__FRAMEFORM)));
	setCurrentForm (__FRAMEFORM);
	setCurrentAspect (__REDFRAME + _color);
}

// ---
void AticAtacScoreFrame::setColor (int c)
{ 
	_color = c; 
	if (_color > __CYANFRAME) 
		_color = 0; 
	setCurrentAspect (__REDFRAME + _color);
}

// ---
void AticAtacScoreFrame::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::ScoreObject::drawOn (s, p);

	QGAMES::Form* f = QGAMES::Game::game () -> form (__TIMESCOREFORM);
	f -> drawOn (s, 0, position () + QGAMES::Vector (__BD 40, __BD 140, __BD 0));
}

// ---
AticAtacLives::AticAtacLives ()
		: QGAMES::ScoreObject (__LIVESOBJECT),
		  _lives (0)
{
	// This is not really needed...
	// Just to avoid an error during the standard initialization
	// It is double checked that there is a form at least...
	_forms.insert (QGAMES::Forms::value_type (__KNIGHTFORM, 
		QGAMES::Game::game () -> form (__KNIGHTFORM)));
	_forms.insert (QGAMES::Forms::value_type (__WIZARDFORM, 
		QGAMES::Game::game () -> form (__WIZARDFORM)));
	_forms.insert (QGAMES::Forms::value_type (__SERVANTFORM, 
		QGAMES::Game::game () -> form (__SERVANTFORM)));
}

// ---
void AticAtacLives::setLives (int l)
{
	_lives = l;
	if (_lives > __MAXNUMBEROFLIVES) _lives = __MAXNUMBEROFLIVES;
	if (_lives < 0) _lives = 0;
}

// ---
void AticAtacLives::initialize ()
{
	QGAMES::ScoreObject::initialize ();
	_lives = 0;
	_playerType = 0;
}

// ---
void AticAtacLives::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::Form* f = QGAMES::Game::game () -> form (__KNIGHTFORM + _playerType);
	for (int i = 0; i < _lives; i++)
		s -> drawForm (position () + 
			QGAMES::Vector (__BD (i * f -> frameWidth ()), __BD 0, __BD 0), f,
				0, 0, f -> frameWidth (), f -> frameHeight ());
}

AticAtacThings::AticAtacThings ()
	: QGAMES::ScoreObject (__THINGSOBJECT),
	  _things ()
{
	// This is not really needed...
	// Just to avoid an error during the standard initialization
	// It is double checked that there is a form at least...
	_forms.insert (QGAMES::Forms::value_type (__FRAMEFORM, 
		QGAMES::Game::game () -> form (__FRAMEFORM)));
}

// ---
bool AticAtacThings::couldMainCharacterExit () const
{
	// When the game is in debug mode the user can always exit the game...
	// So it is not needed to have the three pars of the exit key!
	#ifndef NDEBUG
//	return (true);
	#endif

	// If there is no three things or more...
	// It is not possible the three parts of main key are!
	if (_things.size () < 3)
		return (false);

	bool result = true; int count = 0;
	for (AticAtacWorld::ThingPositions::const_iterator i = _things.begin (); 
			i != _things.end () && result && count < 3; i++, count++)
		if ((*i).first != (__EXITKEYID + count))
			result = false; // First time the piece is not in the right position, exiting is not possible!
	return (result);
}

// ---
void AticAtacThings::initialize ()
{
	QGAMES::ScoreObject::initialize ();
	_things = AticAtacWorld::ThingPositions ();
}

void AticAtacThings::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	int counterX = 0; int counterY = 0;
	for (AticAtacWorld::ThingPositions::const_iterator i = _things.begin (); 
			i != _things.end (); i++)
	{
		if ((*i).first > (int) General::_e._things.size ())
			continue; // Probably it is a key...we will see how to draw it...

		General::ThingDefinition tD = General::_e._things [(*i).first];
		General::FormDefinition fD = *tD._forms.begin ();
		QGAMES::Form* f = QGAMES::Game::game () -> form (fD._form);
		f -> drawOn (s, fD._initialFrame, position () + 
			QGAMES::Vector (__BD (counterX * f -> frameWidth ()), 
				__BD (counterY * f -> frameHeight ()), __BD 0));

		// Moves the cursor...
		if (++counterX >= 4) { counterY++; counterX = 0; }
	}
}

// ---
AticAtacTimeCounter::AticAtacTimeCounter ()
	: QGAMES::ScoreObjectText (__TIMECOUNTEROBJECT, "", QGAMES::Forms (), 0 /* the space */)
{
	_forms.insert (QGAMES::Forms::value_type (__LETTERSFORM, 
		QGAMES::Game::game () -> form (__LETTERSFORM)));
}

// ---
void AticAtacTimeCounter::setSeconds (int nS)
{
	int nM = (nS / 60);
	int nSS = (nS % 60);

	std::string cero (2, '0');
	std::stringstream nMT; nMT << nM; std::string nMTs = nMT.str ();
	std::stringstream nST; nST << nSS; std::string nSTs = nST.str ();

	std::string tx = cero.substr (0, 2 - nMTs.length ()) + nMTs + ":" +
		cero.substr (0, 2 - nSTs.length ()) + nSTs;
	setText (tx);
}

// ---
AticAtacScoreCounter::AticAtacScoreCounter ()
	: QGAMES::ScoreObjectNumber (__SCORECOUNTEROBJECT, 0, QGAMES::Forms (), 16 /* the 0 */)
{
	_forms.insert (QGAMES::Forms::value_type (__LETTERSFORM, 
		QGAMES::Game::game () -> form (__LETTERSFORM)));
}

