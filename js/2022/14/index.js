const fs = require("fs").promises;

// All possible sand movements
const directions = [
  [0, 1], // Down
  [-1, 1], // Down left
  [1, 1], // Down right
];

async function a() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  const map = [];
  const rocks = parse(input, map);
  const lowestRock = rocks.sort((a, b) => b[1] - a[1])[0];
  let sands = 0;

  while (true) {
    let previousSand;
    let newSand = [500, 0];

    do {
      previousSand = newSand;
      newSand = simulateSand(previousSand, map);
    } while (newSand[0] !== previousSand[0] || newSand[1] !== previousSand[1]);

    if (newSand[1] > lowestRock[1]) {
      break;
    }

    ++sands;

    if (!map[newSand[1]]) map[newSand[1]] = [];
    map[newSand[1]][newSand[0]] = "o";
  }

  console.log(sands);
}

async function b() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  const map = [];
  let sands = 0;
  const rocks = parse(input, map);
  const floorY = rocks.sort((a, b) => b[1] - a[1])[0][1] + 2;
  map[floorY] = [];

  for (let i = 0; i < 50000; i++) {
    map[floorY].push("#");
  }

  while (true) {
    let previousSand;
    let newSand = [500, 0];

    do {
      previousSand = newSand;
      newSand = simulateSand(previousSand, map);
    } while (newSand[0] !== previousSand[0] || newSand[1] !== previousSand[1]);

    ++sands;
    if (!map[newSand[1]]) map[newSand[1]] = [];
    map[newSand[1]][newSand[0]] = "o";

    if (newSand[0] === 500 && newSand[1] === 0) {
      break;
    }
  }

  console.log(sands);
}

function simulateSand(startAt, map) {
  for (const direction of directions) {
    const newX = startAt[0] + direction[0];
    let newY = startAt[1] + direction[1];

    if (map[newY]?.[newX]) {
      continue;
    }

    if (newY > 1000) {
      newY = 1000;
    }

    return [newX, newY];
  }

  return startAt;
}

function parse(input, map) {
  const rocks = new Set();

  for (const line of input) {
    const split = line.split("->");
    for (let i = 0; i < split.length - 1; i++) {
      const from = split[i].split(",").map((n) => Number(n));
      const to = split[i + 1].split(",").map((n) => Number(n));
      if (from[0] !== to[0]) {
        const max = Math.max(from[0], to[0]);
        const min = Math.min(from[0], to[0]);
        for (let j = min; j <= max; j++) {
          if (!map[from[1]]) map[from[1]] = [];
          map[from[1]][j] = "#";
          rocks.add([j, from[1]]);
        }
      } else {
        const max = Math.max(from[1], to[1]);
        const min = Math.min(from[1], to[1]);
        for (let j = min; j <= max; j++) {
          if (!map[j]) map[j] = [];
          map[j][from[0]] = "#";
          rocks.add([from[0], j]);
        }
      }
    }
  }

  return [...rocks];
}

function visualizeMap(map, intervalX, intervalY) {
  for (let j = intervalY[0]; j < intervalY[1] + 10; j++) {
    let line = "";

    for (let i = intervalX[0] - 10; i < intervalX[1] + 10; i++) {
      if (map[j]?.[i]) {
        line += map[j][i];
      } else {
        line += ".";
      }
    }

    console.log(line);
  }
}

async function main() {
  await a();
  await b();
}

main();
