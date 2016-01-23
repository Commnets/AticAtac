/** \addtogroup Atic */
/*@{*/

#ifndef __ORIGINALDATA_HPP__
#define __ORIGINALDATA_HPP__

#include "Defs.hpp"

class OriginalData
{
	public:
	OriginalData ();

	static const OriginalData _oData;

	public:
	static const int roomTypeOffset;
	static const int roomTypes [__NUMBEROFROOMTYPES__][6];
	static const int roomLinePointsOffSet;
	static const int roomsOffSet;
	static const int rooms [__NUMBEROFROOMS__][2];
	static const int roomPoints [2297];
	static const int backgroundElementsOffSet;
	static const int backgroundElements [__NUMBEROFOBJECTS__][8];
	static const int elements [__NUMBEROFTHINGS][5];
};

#endif
  
// End of the file
/*@}*/

