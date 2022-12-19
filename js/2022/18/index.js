const fs = require("fs").promises;

const directions = [
  [1, 0, 0],
  [-1, 0, 0],
  [0, 1, 0],
  [0, -1, 0],
  [0, 0, 1],
  [0, 0, -1],
];

async function a() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  const cubes = [];

  for (const line of input) {
    const cube = line.split(",").map((l) => Number(l));
    cubes.push(cube);
  }

  let hiddenFaces = 0;

  for (const cube of cubes) {
    for (const direction of directions) {
      const newPos = [
        cube[0] + direction[0],
        cube[1] + direction[1],
        cube[2] + direction[2],
      ];

      if (
        cubes.find(
          (cube) =>
            cube[0] === newPos[0] &&
            cube[1] === newPos[1] &&
            cube[2] === newPos[2]
        )
      ) {
        ++hiddenFaces;
      }
    }
  }

  console.log(cubes.length * 6 - hiddenFaces);
}

async function b() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  const cubes = [];
  let min = [Infinity, Infinity, Infinity];
  let max = [0, 0, 0];

  for (const line of input) {
    const [x, y, z] = line.split(",").map((l) => Number(l));
    cubes.push([x, y, z]); // Start with the 6 faces

    // Find boundaries of all the existing cubes
    if (x > max[0]) max[0] = x;
    if (y > max[1]) max[1] = y;
    if (z > max[2]) max[2] = z;
    if (x < min[0]) min[0] = x;
    if (y < min[1]) min[1] = y;
    if (z < min[2]) min[2] = z;
  }
  // Expand search area by 1
  min = min.map((n) => n - 1);
  max = max.map((n) => n + 1);

  const queue = [min];
  const visited = new Set();
  let surfaces = 0;

  while (queue.length > 0) {
    const [x, y, z] = queue.shift();

    if (visited.has(`${x},${y},${z}`)) {
      continue;
    }

    for (const direction of directions) {
      const newPos = [x + direction[0], y + direction[1], z + direction[2]];

      if (
        newPos[0] > max[0] ||
        newPos[0] < min[0] ||
        newPos[1] > max[1] ||
        newPos[1] < min[1] ||
        newPos[2] > max[2] ||
        newPos[2] < min[2]
      ) {
        continue;
      }

      if (
        cubes.find(
          (cube) =>
            cube[0] === newPos[0] &&
            cube[1] === newPos[1] &&
            cube[2] === newPos[2]
        )
      ) {
        ++surfaces;
      } else {
        queue.push(newPos);
      }
    }

    visited.add(`${x},${y},${z}`);
  }

  console.log(surfaces);
}

async function main() {
  await a();
  await b();
}

main();
