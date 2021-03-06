## Objective
I've started this proyect as a UPC videogame master task, which challenge us to make a "replica" of an old racing game from a list of them. In my case, I've chossen Space-Harrier (and yes, it's considered as racing game).

## Game Info
Space Harrier (Japanese: スペースハリアー Hepburn: Supēsu Hariā) is an arcade video game developed and released by Sega Enterprises in December 1985. Originally conceived as a realistic military-themed game played in the third-person perspective and featuring a player-controlled fighter jet, technical and memory restrictions at the time resulted in Sega developer Yu Suzuki redesigning it to fit a fantasy setting centered around a jet-propelled human character. [Wikipedia](https://en.wikipedia.org/wiki/Space_Harrier)

[Github Wiki](https://github.com/Moaif/Space-Harrier-Game/wiki)

### How to play
On this game, you have to avoid/destroy all obstacles comming to you. If you destroy them, you earn points. However some obstacles are not destructible, forcing you to evade them. The objective of the game is adquiring the best score in order to enter into the Scoreboard. 

#### Controls
- A/W/S/D or arrows to move chararcter
- Space  to shoot
- Enter to pause game
- Esc to exit game

## Video

### Original
<iframe width="560" height="315" src="https://www.youtube.com/embed/Ia9l6qCxayg" frameborder="0" allowfullscreen></iframe>
### Mine
<iframe width="560" height="315" src="https://www.youtube.com/embed/DWs5byacVXc" frameborder="0" allowfullscreen></iframe>

## Features

### Game Features implemented

- All player actions
- Level 1
- Level 3
- Intro
- Menu
- Level enemies actions
- Level bosses
- Shadows
- Scroller 2 layers background
- Lateral and vertical moving floor
- Fast level change
- Full game UI
- Replayable without closing game

### Code Featured implemented

- Texture cache for all font based ones
- Font factory with knowledge of which file/line has a pointer to a font
- GameObject based Colliders
- Collision speed based
- Deep blit buffer to print ordered by z
- Prototipe object instantiation model
- Load from Json:
  - Levels
  - Scoreboard
- Write Scoreboard in Json
- Hierarchy based classes
- Deltatime usage

## Download

[Click Here](https://github.com/Moaif/Space-Harrier-Game/releases)
