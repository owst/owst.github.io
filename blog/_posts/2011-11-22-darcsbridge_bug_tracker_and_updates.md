---
title: Darcs-bridge bug tracker and updates.
published: false
---
I've recently been checking-in with my GSoC darcs-bridge work. I've re-worked
the horrible state-file format I originally used, and have been making various
other tweaks to the code. 

Unfortunately, I've come across a few bugs (thanks to Brent Yorgey and Jesper
Reenburg for reporting some initial problems they've hit), and have been trying
to iron them out. With that in mind, I've added a bug tracker "Topic" for
darcs-bridge on the darcs.net bug tracker: [Bug
tracker](http://bugs.darcs.net/issue?%40search_text=&title=&%40columns=title&topic=47&id=&%40columns=id&creation=&creator=&activity=&%40columns=activity&%40sort=activity&actor=&nosy=&priority=&%40group=priority&milestone=&status=-1%2C1%2C2%2C3%2C4%2C5%2C6%2C16%2C17&%40columns=status&resolvedin=&assignedto=&%40columns=assignedto&%40pagesize=50&%40startwith=0&%40queryname=&%40old-queryname=&%40action=search).

Hopefully, I'll be able to sort out the remaining bugs, and then move onto a
tidy-up and refactor of the existing code.
