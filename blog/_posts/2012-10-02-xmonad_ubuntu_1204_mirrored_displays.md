---
title: Xmonad, Ubuntu 12.04 and Display Resizing/Mirroring
---
I've just spent a fair while with an infuriating screen resizing behaviour in
Ubuntu 12.04, so I thought I'd document it here.

For my work machine, I use Ubuntu with a dual-screen setup (using Nvidia's
TwinView), with a mixture of Gnome and Xmonad for my window manager/desktop
requirements. A handy key-binding that I use frequently is Super-p (that's the
"Windows" key followed by p) used to bring up a program-launch menu. 

After upgrading from Ubuntu 11.10 to 12.04 I was annoyed to find that using
this short-cut would cause the screen to flicker and the two displays to become
mirrored.

After spending a good amount of time recompiling Xmonad and checking various
NVidia/Xorg settings, to no avail, I discovered that this is an intentional
behaviour of Gnome (!).

[Apparently][1] this is due to a Microsoft recommendation that hardware
manufacturers map the various ways of controlling projector/external displays
onto a "simple" key-press of super-p! [This has led][2] gnome-settings-daemon
to implement super-p as a short-cut to [xrandr][3], hence my display
resizing/mirroring symptoms.

The solution is simple - prevent gnome-settings-daemon from making the
call to xrandr by disabling its plugin:

1. Run dconf-editor
1. Expand org.gnome.settings-daemon.plugins.xrandr
1. Uncheck 'active'

[1]: http://mjg59.livejournal.com/121851.html
[2]: https://bugs.launchpad.net/ubuntu/+source/linux/+bug/539477
[3]: http://xorg.freedesktop.org/wiki/Projects/XRandR
