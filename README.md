PapraGame Project
============

##About
<br/>
PapraGame is a game featuring ducks !

It's a multiplayer versus snake-like (but it may be played in solo aswell). To win, you have to be the first to reach a certain number of duckies, depending on the map (you start with 2).
You can get duckies either by catching an egg, or by stealing one to your opponent(s) (you can steal him/her a ducky by forcing him/her to punch you in the duckies).

Several biomes are available (changes the graphics), and maps can be easily modified.

You may change your keys settings within the ``keys.conf`` file

/!\ Under developement. Latest version : v0.4 -- Crepitum Anatum
We are currently working make this game fully graphical, together with a Network part. (Graphics are ready, networking is in a debug stage)
<br/>

###Gameplay
<br/>
You can choose the keys for each player in the res/keys.conf file. Default keys are the arrows for player 1

Pause by pressing Escape (the game also pauses if it looses the focus).
When you hit an obstacle, you will be unvulnerable for 5 turns. Strange violet stuff that may appears in a map are warps. Going into a warp will teleport you to another random warp. Beware the walls !

###Maps
<br/>
You can add a map in the folder `./maps/`). Just create a file named ``<map_name>.map`` and edit it with text editor.
The file should be structured according to the following (line by line)
+ Biome (sand, grass, space, snow, magma, psychogwak)
+ The game 'slowness'
+ Number of eggs required to win (0 for infinity)
+ Length of the map
+ Width of the map
+ Map itself with characters :
    + ``o`` for the obstacle (rock, asteroid, cactus, …)
    + ``.`` for the blank tile (grass, void space, sand, …)
    + ``|`` for the 'river' that goes from north to south
    + ``-`` for the east-west one
    + ``1`` for east-south (from the num-keypad)
    + ``3`` for west-south
    + ``7`` for east-north
    + ``9`` for west-north
    + ``x`` for warps
+ Starting direction of player 1 (up, down, left, right)
    + On the same line, starting point of player 1 (x then y) [the origin is on the top left. x increases rightwards, y increases downwards]
+ Same for any other players
+ eof
(eof stands for End Of File)

##Authors
<br/>
+ Maxime PINARD - developer (maxime.pin@live.fr)
+ Lucas LAZARE - developer (lazarelucas@yahoo.fr)
+ Julien BARBIER - developer (a little), graphic designer (julien.barbier@utbm.fr)

##Download
<br/>
You can get the game sources from the Time Winder Dev Team [github repo](https://github.com/TiWinDeTea/PapraGame).

Binaries and sources of the v0.4 are available on [github](https://github.com/TiWinDeTea/PapraGame/releases/tag/v0.4) aswell
<br/>

##Compilation requirements
<br/>
+ Platform : Linux, Windows, Mac OSX (MAC OSX support is theorical)
+ SFML ≥ 2.2
+ A compiler that supports c++11
<br/>

##Copyright
<br/>
License : Mozilla Public License Version 2.0 

[Read the license file](LICENSE.md)

Extract : Disclaimer of Warranty

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

Note that you might need to configure the Makefile if you don't you g++, for instance

An executable named `PapraGame.elf` should be generated in the `build/` folder

##Contribute

You may contribute easily by trying the game and having fun ! That would make us very happy !

##Thanks

We would like to thanks the [SFML](http://www.sfml-dev.org/) dev team for their work, they're awesome
