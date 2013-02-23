devpkg
======

This is a utility created from Zed A. Shaw's 
_Learn C The Hard Way_ [book][]. _devpkg_ will download,
build and install a piece of software to your computer.
Probably.

[book]: http://c.learncodethehardway.net

### Commands ###

The available major commands are:
 - -S				Set up devpkg for it's first use.
 - -L				List the software already
					installed by devpkg.
 - -I url [options]	Download, build and install the
					software from that URL.
 - -B url [options]	Download and build the software
					from that URL.
 - -F url			Download the software from that
					URL.

The available options are:
 - -c string		Configure options.
 - -m string		Make options.
 - -i string		Install options.
 
