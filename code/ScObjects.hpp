/** \addtogroup Atic */
/*@{*/

#ifndef __SCENTITIES_HPP__
#define __SCENTITIES_HPP__

#include <Common/entity.hpp>
#include <Common/rectangle.hpp>
#include <Arcade/artist.hpp>
#include "Worlds.hpp"
#include "General.hpp"
#include "Defs.hpp"

/** Class to represent the chicken. */
class AticAtacChicken : public QGAMES::ScoreObject
{
	public:
	// Constructors
	AticAtacChicken ();

	virtual Entity* clone ()
							{ return (new AticAtacChicken ()); }

	/** To set the % of the chicken visible. */
	void setPercentage (int p);
	/** To know how % of the chicken has been consumed. */
	int percentage () const
							{ return (_percentage); }

	/** To initialize the chicken.
		Always 0% consumed. */
	virtual void initialize ();
	/** To draw the chicken.
		A part (below the percentage consumed will be shown as an eaten chicken,
		whilst the other part will be shown as a bot eatenb chicken. 
		@see parent. */
	virtual void drawOn (QGAMES::Screen* s, 
		const QGAMES::Position& p = QGAMES::Position::_noPoint);

	private:
	/** The percentrage of the chicken that has been consumed. */
	int _percentage;
};

/** Class to represent the frame around all the information about the game. */
class AticAtacScoreFrame : public QGAMES::ScoreObject
{
	public:
	// Constructors
	AticAtacScoreFrame ();

	virtual Entity* clone ()
							{ return (new AticAtacScoreFrame ()); }

	/** To change the color. */
	void setColor (int c);
	/** To know the color the frame is. */
	int color () const
							{ return (_color); }

	/** To draw the frame. 
		@see parent. */
	virtual void drawOn (QGAMES::Screen* s, 
		const QGAMES::Position& p = QGAMES::Position::_noPoint);

	private:
	/** The number of color the frame is. */
	int _color;
};

/** Class to represent the lives. */
class AticAtacLives : public QGAMES::ScoreObject
{
	public:
	// Constructors
	AticAtacLives ();

	virtual Entity* clone ()
							{ return (new AticAtacLives ()); }

	/** To set the number of lives. */
	void setLives (int l);
	/** To know the number of lives. */
	int lives () const
							{ return (_lives); }
	/** To set the aspect. */
	void setPlayerType (int t)
							{ _playerType = t; }
	/** To know the aspect. */
	int playerType () const
							{ return (_playerType); }

	/** To initialize the lives. */
	virtual void initialize ();
	/** To draw the lives.
		@see parent. */
	virtual void drawOn (QGAMES::Screen* s, 
		const QGAMES::Position& p = QGAMES::Position::_noPoint);

	private:
	/** The number of lives. */
	int _lives;
	/** The type of player. */
	int _playerType;
};

/** Class to represent the things the user has. */
class AticAtacThings : public QGAMES::ScoreObject
{
	public:
	// Constructors
	AticAtacThings ();

	virtual Entity* clone ()
							{ return (new AticAtacThings ()); }

	/** To set the things. */
	void setThings (const AticAtacWorld::ThingPositionsList& t)
							{ _things = t; }
	/** To know the things. */
	const AticAtacWorld::ThingPositionsList& things () const
							{ return (_things); }

	/** To know whether the main character could exit the game.
		This happens when the three parts of the main key exist
		and the are in the right order. */
	bool couldMainCharacterExit () const;

	/** To initialize the things. */
	virtual void initialize ();
	/** To draw the things.
		@see parent. */
	virtual void drawOn (QGAMES::Screen* s, 
		const QGAMES::Position& p = QGAMES::Position::_noPoint);

	private:
	/** The things. */
	AticAtacWorld::ThingPositionsList _things;
};

/** A class to represent the time counter. 
	The time core only makes the time to pass. */
class AticAtacTimeCounter : public QGAMES::ScoreObjectText
{
	public:
	// Constructors & Destructors
	/** Just to create the time counter. It only adds the right form. 
		This entity is not created so far from an entity builder. */
	AticAtacTimeCounter ();

	virtual Entity* clone ()
							{ return (new AticAtacTimeCounter ()); }

	/** To convert a time passed as parameter in seconds,
		into a text to be shown in the score object. */
	void setSeconds (int nS);

	protected:
	/** @see parent. */
	virtual int formToDrawLetters ()
							{ return (__LETTERSFORM); }
};

/** A class to represent the score counter. 
	This counter adds points any time a monster is destroyed, 
	any time something is gotten, and any time something is eaten. */
class AticAtacScoreCounter : public QGAMES::ScoreObjectNumber
{
	public:
	// Constructors & Destructors
	/** Just to create the score counter. It only adds the right form. 
		This entity is not created so far from an entity builder. */
	AticAtacScoreCounter ();

	virtual Entity* clone ()
							{ return (new AticAtacScoreCounter ()); }

	protected:
	/** @see parent. */
	virtual int formToDrawDigits ()
							{ return (__LETTERSFORM); }
};

#endif
  
// End of the file
/*@}*/