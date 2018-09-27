---
title: JavaScript Game of Life
js_includes:
  - https://cdn.rawgit.com/owst/JS-Game-of-Life/2d78ba8a37d12ac7b2329a205622ccd85a1ee626/CA.js
  - https://cdnjs.cloudflare.com/ajax/libs/jqueryui/1.12.1/jquery-ui.min.js
css_includes:
  - https://cdnjs.cloudflare.com/ajax/libs/jqueryui/1.12.1/jquery-ui.min.css
---

<script>
/*
  A bit yucky, but Github has made it not possible to link to hosted js files
  so we can't stick this in an external file and into js_includes above. We
  also can't rely on jQuery being loaded here because it is only linked in the
  footer, so we use the load event to be sure everything has loaded.
*/
window.addEventListener("load", function() {
setTimeout(function() {
    var jCanvas = $('#world');
    var canvas = jCanvas.get(0);
    var ctx = canvas.getContext('2d');
    var pixelSize = 6;
    var width = canvas.width / pixelSize;
    var height = canvas.height / pixelSize;
    var startTimeout = 30;

    var ca = new CA(ctx, width, height, pixelSize, startTimeout);

    startStopHandler = function () {
        var button = $('#startStop');

        if (button.html() === 'Stop') {
            button.html('Start');

            clearTimeout(ca.timeout);
            ca.timeout = null;
        } else {
            button.html('Stop');

            ca.setupTimeout();
        }
    };

    randomise = function () {
        ca.randomData();
    };

    clearCtx = function () {
        ca.clear();
    };

    clearAndRandomise = function () {
        clearCtx();
        randomise();
    };

    jCanvas.mousedown(function (e) {
        var x = Math.round(e.offsetX / ca.pixelSize) * ca.pixelSize;
        var y = Math.round(e.offsetY / ca.pixelSize) * ca.pixelSize;

        if (e.which === 1) {
            ca.setPixelBlack(x, y);
        } else if (e.which === 3) {
            ca.clearPixel(x, y);
        }
    });

    jCanvas.contextmenu(function () {
        return false;
    });

    $('#slider').slider({
        value: startTimeout,
        min: 10,
        max: 500,
        step: 10,
        slide: function (event, ui) {
            $('#timeout').val(ui.value);
            ca.timeoutTime = ui.value;

            if (ca.timeout) {
                clearTimeout(ca.timeout);
                ca.setupTimeout();
            }
        }
    });

    $('#timeout').val(startTimeout);

    ca.initGliderGun(10, 40);
    ca.initGliderGun(60, 10);
}, 1000);
});
</script>

I thought it'd be interesting to code a JavaScript implementation of [John
Conway's Game of Life][gol] [cellular automaton][ca].  The game of life is
comprised of a set of cells (the world), where each cell can be either in the
"on" or "off" state (usually represented by black or white pixels,
respectively).

The game is split into discrete time steps. At each step, all of the cells in
the world are evaluated to calculate their liveness in the next step. Those
that are live will change to the "on" state, whilst those who have died are
changed to the "off" state.

The liveness of a cell is determined by its state, and the state of its 9
neighbours, using a simple set of rules. The number of live neighbours is
counted, and the cell's state is calculated thus:

1. A live cell with < 2 *dies*.
1. A live cell with 2 or 3 *lives*.
1. A live cell with > 3 *dies*.
1. A dead cell with 3 *lives*.

I've uploaded my implementation to [Github, here][github_link]. A demo is shown
below. The world is initialised to contain 2 [Gosper glider guns][glider],
which are designed such that they emit a certain pattern of cells with a fixed
period.

The start/stop button pauses the timesteps, whose length is determined by the
slider control. The clear button clears the world, whilst the randomise button
adds a random set of "on" cells to the world. Individual cells can be switched
on/off by clicking the left and right mouse buttons, respectively (pause the
game first, else the new cells will immediately die). I chose to implement my
world using a toroidal array, so both sides of the world "wrap around". This
means gliders can pass from one side to the other, rather than simply dying at
the edges.

<canvas style="display: block; margin:0 auto; border: 1px solid black;" id="world" width="600" height="420">
</canvas>
<div>
<button id="startStop" type="button"
    onclick="startStopHandler()">Start</button>
<button id="clear" type="button" onclick="clearCtx()">Clear</button>
<button id="randomise" type="button" onclick="randomise()">Randomise</button>
<button id="clearAndRandomise" type="button" onclick="clearAndRandomise()">Clear and Randomise</button>
<br/>
<label for="timeout">Timeout period (ms):</label>
<input type="text" id="timeout" disabled="true" style="width: 35px"/> <div id="slider" style="display: inline-block; width: 500px"></div>
</div>

[github_link]: https://github.com/owst/JS-Game-of-Life
[glider]: http://en.wikipedia.org/wiki/Gun_(cellular_automaton)
[gol]: http://en.wikipedia.org/wiki/Conway's_Game_of_Life
[ca]: http://en.wikipedia.org/wiki/Cellular_automaton

<link rel="stylesheet" type="text/css" href="http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.9/themes/smoothness/jquery-ui.css" />
