C-8DE (Chip-8 Didactic Emulator)
================================

Version 0.0.1

2010 (cc) Xavier Belanche Alonso



Ubuntu
------

<code>
autoreconf -i && ./configure  && make
</code>


MacOSX
------

You need X devel utils and SDL and SDL_ttf development libraries it's also necessary to compile. Please, visit some of the next links to acquire more information about this.

The compile sequence is similar to Ubuntu, except for the configure step:

<code>
autoreconf -i && ./configure LDFLAGS=-L/opt/local/lib && make
</code>


Play !
------

`./yace` and you will see a nice Maze ;-)



