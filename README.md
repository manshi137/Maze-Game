# Phineas and Ferb in IITD
## To run game:
## Server: 
 Make a folder containing Finalserver.cpp file, makefile, lazy.ttf, pics folder and Sound folder on one device.
 Extract the makefile from "Makefile for server" folder.
## Client: 
 Make a folder containing Finalclient.cpp file, makefile, lazy.ttf, pics folder and Sound folder on other device. 
 Extract the makefile from "Makefile for client" folder.

## Setup
 SDL2 libraries including SDL2, SDL2_mixer , SDL2_ttf, SDL2_image should be installed in both devices.
 Using Homebrew:
```
brew install sdl2
brew install sdl2_image
brew install sdl2_mixer
brew install sdl2_ttf
```

## On both devices

Open the folder containing all above mentioned files in terminal.

### Compile: Type make and press enter.
```
make
```
### Run: Type ./game and press enter.
```
        ./game
```
Game window will be displayed.


