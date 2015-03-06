## Overview
This repository contains all source code for the Olympic Steel Eagles during the 2014-2015 school year.

Two robot bases are contained in this repository:
	- Team 7701 / Olympic Steel Eagle I / Vulcan
	- Team 7702 / Olympic Steel Eagle II / Dios (Robot now deprecated)
	- Dummy Project - motor test bed

The Smart Motor Library source code is containined in this repository and implements additional functioanlity for motors utilizing IMEs and for basic acceleration profiles.

To get the pretty log:
<code>git log --graph --pretty=medium --date=local --abbrev-commit</code>

## Compilation Instructions

Before compiling any base, use <code>make clean</code> to clean the bin.

|	Base	|					Command					|							Comments 							|
|:---------:|:-----------------------------------------:|---------------------------------------------------------------|
|   dios	|     <code>make SUBDIRS=dios</code>		| Makes the Dios Project										|
|   dios	| <code>make SUBDIRS=dios upload</code>		| Makes and uploads Dios										|
|  vulcan	| 	 <code>make SUBDIRS=vulcan</code>		| Makes the vulcan project										|
|  vulcan 	| <code>make SUBDIRS=vulcan upload</code>	| Makes and uploads vulcan										|
| 	SML		| 			<code>make</code>				| *Navigate to */libsml* <br /> Makes and creates .a for libsml	|
|	LCD		|		    <code>make</code>				| *Navigate to */liblcd* <br /> Makes and creates .a for liblcd	|
|  dummy1	|	<code>make SUBDIRS=dummy</code>			| Makes the dummy project										|
|  dummy1	| <code>make SUBDIRS=dummy upload </code>	| Makes and uploads dummy1										|
|  dummy2	| 			<code>make</code>				| Makes the base contained in /src								|
|  dummy2	| 		<code>make upload</code>			| Makes and upload /src											|

## License
Copyright (c) 2014-2015 Elliot Berman and Robert Shrote

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## PROS License
*For compactness, main.h and API.h have been excluded from the printed documentation for compactness.
Below is a copy of the PROS License:*

Copyright (c) 2011-2014, Purdue University ACM SIG BOTS.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Purdue University ACM SIG BOTS nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL PURDUE UNIVERSITY ACM SIG BOTS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Purdue Robotics OS contains FreeRTOS (http://www.freertos.org) whose source code may be
obtained from http://sourceforge.net/projects/freertos/files/ or on request.