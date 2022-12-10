const fs = require("fs").promises;

async function a() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  let X = 1;
  let tick = 0;
  let signal = 0;

  for (const item of input) {
    const split = item.split(" ");
    tick++;
    signal = processSignal(X, tick, signal);

    if (split[0] === "addx") {
      tick++;
      signal = processSignal(X, tick, signal);

      const toAdd = Number(split[1]);
      if (!Number.isNaN(toAdd)) {
        X += toAdd;
      }
    }
  }

  console.log(signal);
}

function processSignal(X, tick, signal) {
  const toMultiply = [20, 60, 100, 140, 180, 220];

  if (toMultiply.includes(tick)) {
    signal += X * tick;
  }

  return signal;
}

async function b() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  let X = 1;
  let tick = 0;
  const sprite = {
    grid: [],
    x: 0,
    y: 0,
  };

  for (const item of input) {
    const split = item.split(" ");
    ++tick;
    renderSprite(sprite, spritePosition(X), tick);

    if (split[0] === "addx") {
      ++tick;
      renderSprite(sprite, spritePosition(X), tick);

      const toAdd = Number(split[1]);
      if (!Number.isNaN(toAdd)) {
        X += toAdd;
      }
    }
  }

  for (const row of sprite.grid) {
    let s = "";

    for (const col of row) {
      s += col;
    }

    console.log(s);
  }
}

function renderSprite(sprite, spritePosition, tick) {
  ++sprite.x;

  if (!sprite.grid[sprite.y]) {
    sprite.grid[sprite.y] = [];
  }

  sprite.grid[sprite.y].push(spritePosition[sprite.x]);

  if (tick % 40 === 0) {
    ++sprite.y;
    sprite.x = 0;
  }
}

function spritePosition(pos) {
  const arr = new Array(41).fill(0).map((_) => ".");
  arr[pos] = "#";
  arr[pos + 1] = "#";
  arr[pos + 2] = "#";

  return arr;
}

async function main() {
  await a();
  await b();
}

main();
