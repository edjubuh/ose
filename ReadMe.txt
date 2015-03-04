This repository contains all source code for the Olympic Steel Eagles during the 2014-2015 school year.

Two robot bases are contained in this repository:
	- Team 7701 / Olympic Steel Eagle I / Vulcan
	- Team 7702 / Olympic Steel Eagle II / Dios
	- Dummy Project - motor test bed

The Smart Motor Library source code is containined in this repository and implements additional functioanlity for motors utilizing IMEs and for basic acceleration profiles.

COMPILATION INSTRUCTIONS:

Before compiling any base, use make clean to clean the bin.

+---------------------------------------------------------------------------+
|	Base	|			Command			|				Comments 			|
|-----------|---------------------------|-----------------------------------|
|   dios	|     make SUBDIRS=dios     | Makes the Dios Project			|
|   dios	| make SUBDIRS=dios upload	| Makes and uploads Dios			|
|  vulcan	| 	 make SUBDIRS=vulcan	| Makes the vulcan project			|
|  vulcan 	| make SUBDIRS=vulcan upload| Makes and uploads vulcan			|
| 	SML		| 			make			| Navigate to */libsml				|
|			|							|   Makes and creates .a for libsml	|
|  dummy1	|	make SUBDIRS=dummy		| Makes the dummy project			|
|  dummy1	| make SUBDIRS=dummy upload | Makes and uploads dummy1			|
|  dummy2	| 			make			| Makes the base contained in /src	|
|  dummy2	| 		make upload			| Makes and upload /src				|
+-----------+---------------------------+-----------------------------------+


Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.  This file is offered as-is,
without any warranty.