PapraGame Project
============

##About
<br/>
PapraGame is a game featuring ducks !

It's a two players versus snake-like. Two win, you have to be the first to get 5 duckies (you start with 2).
You can get duckies either by catch an egg, or by stealing one to your 	opponent (you can steal him/her a ducky by forcing him/her to punch you in the face or in your own duckies).

Several biomes are available (changes the graphics), and the map can be easily modified.

/!\ Under developement.
<br/>

###Map editing
<br>
You can edit the map opening the map file (in the folder `build/res/`) with your favorite text editor
It should be created according to the following (line by line)
+ Biome (sand, grass, space, snow, …)
+ Length of the map
+ Width of the map
+ Starting point of player 1 (x y)
    + On the same line, starting direction of player 1 (up, down, left, right)
+ Same for the playe 2
+ Map itself with characters :
    + ``o`` for the obstacle (rock, asteroïd, cactus, …)
    + ``.`` for the blank tile (grass, void space, sand, …)
    + ``|`` for the 'river' that goes from north to south
    + ``-`` for the east-west one
    + ``1`` for east-south (from the num-keypad)
    + ``3`` for west-south
    + ``7`` for east-north
    + ``9`` for west-north

##Authors
<br/>
+ Maxime PINARD - developer (maxime.pin@live.fr)
+ Lucas LAZARE - developer (lazarelucas@yahoo.fr)
+ Julien BARBIER - developer (julien.barbier@utbm.fr)

##Download
<br/>
You can get this game from the Time Winder Dev Team [github repo](https://github.com/TiWinDeTea/PapraGame).
<br/>

##Requirements
<br/>
+ Platform : Linux, Windows, Mac OSX (MAC OSX support is theorical)
+ SFML ≥ 2.2
+ A compiler that supports c++11
<br/>

##Copyright
<br/>
License : Mozilla Public License Version 2.0 

[Read the license file](LICENSE.md)

Disclaimer of Warranty

    Covered Software is provided under this License on an "as is"
    basis, without warranty of any kind, either expressed, implied, or
    statutory, including, without limitation, warranties that the
    Covered Software is free of defects, merchantable, fit for a
    particular purpose or non-infringing. The entire risk as to the
    quality and performance of the Covered Software is with You.
    Should any Covered Software prove defective in any respect, You
    (not any Contributor) assume the cost of any necessary servicing,
    repair, or correction. This disclaimer of warranty constitutes an
    essential part of this License. No use of any Covered Software is
    authorized under this License except under this disclaimer.

<br/>

##Compile

In order to compile PapraGame executable, go to the project's root with a terminal, and use the command

``$ make``

An executable named `PapraGame.elf` should be generated in the `res/` folder

You may delete the `obj/` folder if you like to.

##Contribute

You may contribute easily by trying the game and have fun ! That would make us very happy !
