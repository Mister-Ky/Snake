# Snake
# my discord is - mister_ky (and kygechik)
# the program needs sfml 2.5.1
# sfml - https://www.sfml-dev.org/
# the program was written in C++ 17 and Microsoft Visual Studio 2022

the repository also has extlibs.rar archive, it contains the sfml version that was used in the project, unzip the archive by dragging the extlibs folder into the project directory and that's it

## The snake is complete

### list of startup commands:
* -modification (creates a .cmd file that will help you run the snake faster with personal settings)
* -size (cell size)
* -cols (x number of cells)
* -rows (number of cells y)
* -framerate (number of frames per second, preferably 10, so that it is not fast)
* -vsync (vertical synchronization)
* -multiplayer (multiplayer game version)
* -isolation (if true, the snake does not die from hitting the door)
* -revival (When you press the R key after death, you will be resurrected with progress reset)

### range of parameter values:
* modification (without value)
* size 10 - 25
* cols 15 - 30
* rows 15 - 30
* framerate 8 - 20
* vsync false - true
* multiplayer false - true
* isolation false - true
* revival false - true

### usage example:
* Snake.exe -size 25 -cols 30 -rows 30 -framerate 10 -vsync false -multiplayer false -isolation false -revival true
* the above command has the initial values of the game, that is, it specifies the parameters that the game uses basic

### when using -modification, the program will create a special file and close

### example -modification:
* Snake.exe -modification

### the order of the parameters does not matter

### system requirements:
* x32 (x86) or x64_x86-bit Windows (with Windows xp version 2, the service pack works, packages up to 2 I do not know if they are supported)
* Bit depth of colors with 8 bits seems to be possible
* the amount of RAM probably exceeds 128 MB (the program with basic settings takes about 15-20 MB)
* processor with a frequency of ~500 MHz or higher
* the video card is not needed in theory

(it depends on how you compile the program, but the above says about Snake.exe from a repository that has already been built)

Snake.exe he is in version 1.0 and code (main.cpp) in 1.1

## instead of Snake.exe you can also write just snake
## the application and the window have an icon

the above may be making a mistake somewhere
