# Catch the Ghost!
_A homework for CS318_

There's a ghost walking in your room late at night and making noises. Instead of being all
nervous about it, you manned up and picked up a ghost sensor. You'll teach that ghost a lesson
today. You can check a certain area in your room. The sensor will display some color indicating
how far the ghost is:

* RED: Ghost is very close.
* ORANGE: Ghost is not far away.
* GREEN: Ghost is very far away.

Also, that sensor is very old. You haven't used it after catching the last ghost Ulubama de
Pariso. So there might be some glitch with the colors. The sensor can display some wrong colors
a few times.

You have a ghost buster too. But it's battery level is very low and the charge it has, the 
buster can be used only once. So, be very sure before you use the buster in some area in your
room. Because after the buster is used, if you missed the ghost, the ghost will be very angry
that you tried to bust him (the ghost doesn't like the buster at all) and will raise havoc in
your room. Maybe he'll sleep in your bed even. You don't want that, do you?

There are certain time frame shifts. After each shift, the ghost will move to an adjacent place
or stay at the same place. But most times, the ghost will move to some cell with a manhattan
distance of 1. A few times, the ghost will move diagonally or stay at the same place.

Now now, soldier. You have only one mission. Catch it. Catch this ghost and let's teach him a
lesson today. I'll show you the probabilities and you move the time frames and check some 
positions in each time frame. Geronimo!

### Dependencies

* You'll need a Linux machine.
* `g++`, `python3`.
* [`pygame` module from `python`](https://www.pygame.org/wiki/GettingStarted).

### Run

Execute `run.sh`.

### Hacks

If you want a smaller grid, say 5x5, go over to `settings.py` and change `GRID_SZ = 9` to
`GRID_SZ = 5`.

### Notes

Hidden Markov Model is used to calculate the probabilities. 

If you want particle filtering, do the following:
* Go over to `def.h`, un-comment these 3 lines:
```cpp
// #ifndef PARTICLE
// #define PARTICLE
// #endif
```
* Go over to `particle.h` and change the value of `GC` to any value you seem fit:
```cpp
const static int GC = 5;	// particle count
```

### Author

Rafid Bin Mostofa <br/>
Dec 12, 2020.
