C-8DE (Chip-8 Didactic Emulator)
================================

Version 0.0.1

2010 (cc) Xavier Belanche Alonso



GNU/Linux
---------

<code>
autoreconf -i && ./configure  && make
</code>


MacOSX
------

You need X devel utils and SDL and SDL_ttf development libraries it's also necessary to compile. Please, visit some of the next links to acquire more information about this.

I have a syntax error involving PKG_CHECK_MODULES. What's the fix?

If you get a syntax error involving PKG_CHECK_MODULES, it is likely that aclocal can't find the pkg.m4 file, which defines this macro. This is common on OS X with Fink, as the pkg-config package puts this file in /sw/share/aclocal, while the standard OS X aclocal program is looking in /usr/share/aclocal. Unfortunately, there is no reliable search path mechanism for aclocal, so the best fix is just to copy (or symlink) /sw/share/aclocal/pkg.m4 to /usr/share/aclocal. This FAQ taken from the Autonomy Lab's P/S Wiki.
 
sudo ln -s /opt/local/share/aclocal/pkg.m4 /usr/share/aclocal/

The compile sequence is similar to Ubuntu, except for the configure step:

<code>
autoreconf -i && ./configure LDFLAGS=-L/opt/local/lib && make
</code>


Play !
------

`./yace` and you will see a nice Maze ;-)



