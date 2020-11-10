# Pengswim

-------------------------
PengSwim game!!
-------------------------

-------------------------
Introduction
-------------------------

PengSwim is a game of swimming and eating, a penguin's two favorite activities. The pbject of PengSwim is to eat as many fish as possible within the time limit. Once you've eaten all the fish on the level, you move to a new level with more fish to eat. What could be simpler?

-------------------------
Requirements
-------------------------

I've only tested the game on Linux, but it may be possible to install in on other systems, provided you fix the Makefile accordingly. I use SDL, SDL_mixer, and SDL_ttf, all of which have been ported to a variety of systems. You'll also need a TrueType font, preferrably Helvetica (see Installation). Other than that, it should compile without a problem. Let me know of any success stories on non-Linux platforms.

-------------------------
Download
-------------------------

It's all at sourceforge

-------------------------
Installation
-------------------------

There is a make file included with the tarball, but it leaves much to be desired. The default binary directory is /usr/games and the default data directory is /usr/share/pengswim. If you need to use different directories, particularly if you want to compile PengSwim to run in your home directory, you'll need to edit these values. Also, you'll need to change the path and file of the font if your Helvetica font is not installed in the stated directory or if you want to use a different font. In the future, I intend to use autoconf and automake to make this all much more sane. Until then, bear with me.

-------------------------
Play
-------------------------

Controls:
UP - Accelerate
DOWN - Brake
LEFT - Turn counter-clockwise
RIGHT - Turn clockwise
SPACE - Charge
Q or ESC - Quit

The controls may take a second to get used to, but it's basically like the old Asteroids game with a lot more viscosity. Note that when you jump out of the water, you have no control.

You have 30 seconds per level to eat all the fish on the screen. Once you eat all the fish, you have the option of going to a new level with two additional fish. This goes on for a maximum of 20 fish (I may raise that eventually). My current high score is 69 fish. N.B. The last fish is always the toughest.

-------------------------
What's New
-------------------------

__0.2.0__

* added ice walls and level maps
* improved fish graphics and motion--they now turn rotate like the penguin
* added really basic AI to the fish. It's the loosest definition of "intelligence"
* changed the physics to accomodate the new obstacles and prey
* fixed the install process using autoconf and automake
* reduced the total particles to ease the processor a bit

-------------------------
Roadmap
-------------------------

There's a ton of things I'd like to add.
Short Term:

* Better looking score/timer box
* Better text between leves/at end of game
* Power Ups! That's what those poorly animated birds are all about--my scheme is for them to carry power-ups in their talons. Tux would have to jump out of the water to grab them. Ideas:
        1. More Time
        2. Bonus Fish
        3. Dynamite! --when Tux hits this, an explosion freezes all the stunned fish where they are for a set amount of time
        4. Grease -- reduces the drag on Tux, so he swims fasters, slides farther
        5. Gown -- Tux's lady friend appears and helps him catch fish for a few seconds
        6. Bait -- Fish become drawn to Tux for a limited time
        7. Maluses -- If you've played LBreakout2, you know not all bonuses are good. Maybe bad things could appear, like Leopard Seals or Fish Repellant
        8. Mystery Power Up -- any of the above
        9. Others?
* Better physics, particularly with the way he jumps out of the water
* Code cleanup. This is my first game and my first SDL app, so a lot of test code and garbage are sitting in there
* Compressed images using SDL_image instead of using big 'ol BMP files
* High Score table
* Indicator bar for Charging

Long Term:

* Original music. The current score was a free .it file from modarchive.com. It's really, really great, but I haven't been able to get ahold of the artist, who is somewhere in the Ukraine. It's a free track anyway, but I'd be more comfortable with original music unless I get explicit permission to keep using it
* More sound effects. A friend suggested a burp after every five fish or so. Splashes would be good, too
* Particle splashes, bubbles on fish, power-up effects, other particle-driven doodads
* Main menu/options page. I may try to learn more about ParaGUI to do this, but I'd also like to keep the required libraries to a minimum
* Obstacles, walls, icebergs, other flotsam (floaters) and jetsam (sinkers)
* Enemies like leopard seals, sharks. Not sure about the southern range of killer whales, but maybe them, too.
* Network play? Not sure how this could work, but it might be pretty fun
* Animated penguin. My original plan had Tux flapping his fins, which would look pretty cool

Of course, I'd love to hear any suggestions you have, and I'd REALLY love any help from out there, especially if you've done this sort of thing before. I'm kind of making this up as I go along, so I could use an experienced hand, even if just to inspect my code. Artists, musicians, sound geeks, and any other talent are also appreciated and welcomed. Get in touch with me, if you are interested

Thanks for the user: http://sourceforge.net/users/mikejedw at sourceforge.net, author of the Pengswim game.

And many thanks for sourceforge for hosting Pengswim!! Look for more info at http://pengswim.sf.net.
