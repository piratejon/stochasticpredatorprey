/*jslint browser:true */
/*global guid*/

var model = (function () {
    "use strict";

    var G, logtome, Color;

    Color = Object.freeze({'bare': '#22ff22', 'rabbit': '#2222ff', 'fox': '#ff2222'});

    function log(s) {
        G.logtome.value += s + '\n';
        G.logtome.scrollTop = G.logtome.scrollHeight;
    }

    function Square(type, age) {
        this.type = type === undefined ? 'bare' : type;
        this.age = age === undefined ? 0 : age;

        this.swapWith = function (n) {
            var tmp_age, tmp_type;

            tmp_type = n.type;
            tmp_age = n.age;

            n.type = this.type;
            n.age = this.age;

            this.type = tmp_type;
            this.age = tmp_age;
        };

        this.dropTheBabyOnEm = function (s) {
            s.type = this.type;
            s.age = 0;
        };

        this.die = function () {
            this.type = "bare";
            this.age = 0;
        };
    }

    function randomIntRange(lower_bound, upper_bound) {
        return Math.floor((Math.random() * (upper_bound - lower_bound)) + lower_bound);
    }

    function plot_point(pt) {
        G.ctx.beginPath();
        G.ctx.arc(pt.x, pt.y, 3, 2 * Math.PI, false);
        G.ctx.fillStyle = pt.color;
        G.ctx.fill();
    }

    function create_canvas(width, height) {
        var canvas = document.getElementById('map');
        if (canvas !== null) {
            canvas.parentNode.removeChild(canvas);
        }
        canvas = document.createElement('canvas');
        canvas.setAttribute('id', 'map');
        canvas.setAttribute('width', width);
        canvas.setAttribute('height', height);
        return canvas;
    }

    function clear_board() {
        var x, y;
        for (x = 0; x < G.width; x += 1) {
            for (y = 0; y < G.height; y += 1) {
                G.grid[x][y] = new Square(); // default is bare, age zero
            }
        }
    }

    function sprinkle() {
        var i, x, y, squares;

        squares = G.width * G.height;

        for (i = squares * G.percent_initial_rabbits; i > 0; i -= 1) {
            while (true) {
                x = randomIntRange(0, G.width);
                y = randomIntRange(0, G.height);
                if (G.grid[x][y].type === 'bare') {
                    G.grid[x][y].type = 'rabbit';
                    G.grid[x][y].age = 0;
                    break;
                }
            }
        }

        for (i = squares * G.percent_initial_foxes; i > 0; i -= 1) {
            while (true) {
                x = randomIntRange(0, G.width);
                y = randomIntRange(0, G.height);
                if (G.grid[x][y].type === 'bare') {
                    G.grid[x][y].type = 'fox';
                    G.grid[x][y].age = 0;
                    break;
                }
            }
        }
    }

    function render() {
        var x, y, xs, ys;
        for (x = 0, xs = 0; x < G.width; x += 1, xs += G.scale) {
            for (y = 0, ys = 0; y < G.height; y += 1, ys += G.scale) {
                G.ctx.fillStyle = Color[G.grid[x][y].type];
                G.ctx.fillRect(xs, ys, G.scale, G.scale);
            }
        }
    }

    function choose_among(somelist) {
        return somelist[randomIntRange(0, somelist.length)];
    }

    function select_a_random_neighbor(x, y) {
      /***
        0 1 2
        3 * 4
        5 6 7
        ***/
        var neighbor;

        if (x === 0) {
            if (y === 0) {
                neighbor =  choose_among([G.grid[x + 1][y], G.grid[x][y + 1], G.grid[x + 1][y + 1]]);
            } else if (y + 1 === G.height) {
                neighbor =  choose_among([G.grid[x][y - 1], G.grid[x + 1][y - 1], G.grid[x + 1][y]]);
            } else {
                neighbor =  choose_among([G.grid[x][y - 1], G.grid[x + 1][y - 1], G.grid[x + 1][y], G.grid[x][y + 1], G.grid[x + 1][y + 1]]);
            }
        } else if (x + 1 === G.width) {
            if (y === 0) {
                neighbor =  choose_among([G.grid[x - 1][y], G.grid[x - 1][y + 1], G.grid[x][y + 1]]);
            } else if (y + 1 === G.height) {
                neighbor =  choose_among([G.grid[x - 1][y - 1], G.grid[x - 1][y], G.grid[x][y - 1]]);
            } else {
                neighbor =  choose_among([G.grid[x - 1][y - 1], G.grid[x][y - 1], G.grid[x - 1][y], G.grid[x - 1][y + 1], G.grid[x][y + 1]]);
            }
        } else {
            if (y === 0) {
                neighbor =  choose_among([G.grid[x - 1][y], G.grid[x + 1][y], G.grid[x - 1][y + 1], G.grid[x][y + 1], G.grid[x + 1][y + 1]]);
            } else if (y + 1 === G.height) {
                neighbor =  choose_among([G.grid[x - 1][y - 1], G.grid[x][y - 1], G.grid[x + 1][y - 1], G.grid[x - 1][y], G.grid[x + 1][y]]);
            } else {
                neighbor =  choose_among([G.grid[x - 1][y - 1], G.grid[x][y - 1], G.grid[x + 1][y - 1], G.grid[x - 1][y], G.grid[x + 1][y], G.grid[x - 1][y + 1], G.grid[x][y + 1], G.grid[x + 1][y + 1]]);
            }
        }

        return neighbor;
    }

    function probabilistic_event_happens(p_event) {
        return p_event > Math.random();
    }

    function a_neighborly_interaction(a, n) {
        if (a.type === 'fox') {
            if (n.type === 'rabbit') {
                if (probabilistic_event_happens(G.p_fox_breed)) {
                    a.dropTheBabyOnEm(n);
                }
            } else {
                if (probabilistic_event_happens(G.p_fox_die)) {
                    a.die();
                } else if (n.type === 'bare') {
                    a.swapWith(n);
                }
            }
        } else if (a.type === 'rabbit') {
            if (n.type === 'bare') {
                if (probabilistic_event_happens(G.p_rabbit_breed)) {
                    a.dropTheBabyOnEm(n);
                } else {
                    a.swapWith(n);
                }
            }
        } else {
            if (n.type !== 'bare') {
                a.swapWith(n);
            }
        }
    }

    function update_counters() {
        var x, y, rabbits, foxes;

        rabbits = foxes = 0;

        for (x = 0; x < G.width; x += 1) {
            for (y = 0; y < G.height; y += 1) {
                if (G.grid[x][y].type === 'rabbit') {
                    rabbits += 1;
                } else if (G.grid[x][y].type === 'fox') {
                    foxes += 1;
                }
            }
        }

        G.rabbit_counter.innerHTML = rabbits;
        G.fox_counter.innerHTML = foxes;
        G.iteration_counter.innerHTML = G.iteration;
        G.logtome.value += "Iteration: " + G.iteration + "; Rabbits: " + rabbits + "; Foxes: " + foxes + "\n";
    }

    function pause() {
        if (G.timer) {
            clearInterval(G.timer);
        }
        G.timer = null;
    }

    function advance() {
        var x, y;

        G.iteration += 1;

        for (x = 0; x < G.width; x += 1) {
            for (y = 0; y < G.height; y += 1) {
                a_neighborly_interaction(G.grid[x][y], select_a_random_neighbor(x, y));
            }
        }

        if (G.render.checked === true) {
            render();
        }

        update_counters();
    }

    function start() {
        pause();
        if (G.render.checked) {
            G.timer = setInterval(advance, 0);
        } else {
            G.timer = setInterval(advance, G.animate_delay);
        }
    }

    function init(x_dimension, y_dimension, scale) {
        var x, seed;

        seed = document.getElementById('seed').value;

        if (seed === '') {
            seed = document.getElementById('seed').value = new Date().getTime();
        }

        Math.seedrandom(seed.toString());

        G = {};
        G.logtome = document.getElementById('logtome');
        G.width = x_dimension;
        G.height = y_dimension;
        G.scale = scale;
        G.canvas = create_canvas(G.width * G.scale, G.height * G.scale);
        G.ctx = G.canvas.getContext('2d');
        document.getElementById('canvas_container').appendChild(G.canvas);
        G.percent_initial_rabbits = document.getElementById('percent_initial_rabbits').value;
        G.percent_initial_foxes = document.getElementById('percent_initial_foxes').value;
        G.p_rabbit_breed = document.getElementById('p_rabbit_breed').value;
        G.p_fox_breed = document.getElementById('p_fox_breed').value;
        G.p_fox_die = document.getElementById('p_fox_die').value;
        G.animate_delay = document.getElementById('animate_interval').value;
        G.render = document.getElementById('render');
        G.rabbit_counter = document.getElementById('rabbit_counter');
        G.fox_counter = document.getElementById('fox_counter');
        G.iteration_counter = document.getElementById('iteration_counter');
        G.iteration = 0;

        G.grid = [];
        for (x = 0; x < G.width; x += 1) {
            G.grid[x] = [];
        }

        G.logtome.value = "Parameters: " + seed + "," + G.width + "," + G.height + "," + G.percent_initial_rabbits + "," + G.percent_initial_foxes + "," + G.p_rabbit_breed + "," + G.p_fox_breed + "," + G.p_fox_die + "\n";

        clear_board();
        sprinkle();
        render();
        update_counters();
    }

    function reset(x_dimension, y_dimension, scale) {
        pause();
        init(x_dimension, y_dimension, scale);
    }

    function downloadlog(mimetype) {
        var uri = "data:" + mimetype + ";filename=modellog.txt," + encodeURIComponent(G.logtome.value);
        window.open(uri, 'modellog.txt');
    }

    return { 'init': init, 'start': start, 'pause': pause, 'reset': reset, 'advance': advance, 'downloadlog': downloadlog };
}());
