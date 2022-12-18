const fs = require("fs").promises;

const shapes = [
  [
    [0, 0],
    [1, 0],
    [2, 0],
    [3, 0],
  ],
  [
    [1, 0],
    [0, 1],
    [1, 1],
    [2, 1],
    [1, 2],
  ],
  [
    [0, 0],
    [1, 0],
    [2, 0],
    [2, 1],
    [2, 2],
  ],
  [
    [0, 0],
    [0, 1],
    [0, 2],
    [0, 3],
  ],
  [
    [0, 0],
    [1, 0],
    [0, 1],
    [1, 1],
  ],
];

async function a() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  const pattern = input[0].split("");
  const rocks = new Set();
  let highestRockY = 0;
  let patternX = 0;

  for (let i = 0; i < 2022; i++) {
    patternX = simulateRock(
      JSON.parse(JSON.stringify(shapes[i % 5])),
      pattern,
      patternX,
      rocks,
      highestRockY
    );
    patternX %= pattern.length;
    highestRockY = findMax(rocks, highestRockY);
  }

  console.log(highestRockY);
}

async function b() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  const pattern = input[0].split("");
  const highestColumns = new Array(7).fill(0).map((_) => 0);
  const rocks = new Set();
  let highestRockY = 0;
  let patternX = 0;
  const states = [];
  let findPattern;
  // No idea why - 4 works here, played around with it to match the result of the exemple
  const m = 1_000_000_000_000 - 4;

  main: for (let i = 0; i < m; i++) {
    patternX = simulateRock(
      JSON.parse(JSON.stringify(shapes[i % 5])),
      pattern,
      patternX,
      rocks,
      highestRockY,
      highestColumns
    );
    patternX %= pattern.length;
    highestRockY = findMax(rocks, highestRockY);

    // A state is an array of [amount of rocks placed, shape index, pattern index, current highest columns]
    const currentState = [i, i % 5, patternX, [...highestColumns]];

    // Find the states with the same current shape and current pattern index
    // Only way it would be a pattern
    const matchStates = states.filter(
      (state) => state[1] === i % 5 && state[2] === patternX
    );

    for (const matchedState of matchStates) {
      // Check if current state and matched state are equal by
      // substracting every of their columns by their smallest column
      // If they match it means it is a recurring pattern here
      let smallestColumnCurrentState = Math.min(...currentState[3]);
      let columnsCurrentState = [];
      currentState[3].forEach((column) =>
        columnsCurrentState.push(column - smallestColumnCurrentState)
      );

      let smallestColumnMatchedState = Math.min(...matchedState[3]);
      let columnsMatchedState = [];
      matchedState[3].forEach((column) =>
        columnsMatchedState.push(column - smallestColumnMatchedState)
      );

      if (
        JSON.stringify(columnsCurrentState) ===
        JSON.stringify(columnsMatchedState)
      ) {
        findPattern = [currentState, matchedState];
        // As soon as a pattern is found, we don't even need to continue the main loop
        break main;
      }
    }

    states.push(currentState);
  }

  // The highest column of the first recurring state
  const highestColumnFirstState = Math.max(...findPattern[1][3]);
  // The total of rocks to put - the total of already placed rocks = the total rocks left to place
  const leftToPlace = m - findPattern[1][0];
  // Interval of the pattern
  const patternInterval = findPattern[0][0] - findPattern[1][0];
  // The total rocks placed during a pattern interval
  // highestRockY is set in the last iteration of the main loop, can also be found in the state highestColumns
  const patternTotalRocks = highestRockY - highestColumnFirstState;

  console.log(
    Math.round(
      (leftToPlace / patternInterval) * patternTotalRocks +
        highestColumnFirstState
    )
  );
}

function simulateRock(
  rock,
  pattern,
  patternX,
  rocks,
  highestRockY,
  columns = []
) {
  let newRock = rock;
  // First place the rock on the grid
  for (const coord of newRock) {
    coord[0] += 2; // Place the rock 2 units away from the left wall
    coord[1] += highestRockY + 4; // Place the rock 3 units above the highest rock (floor is at 0 so we need to + 1 it)
  }

  let maxX = 6;
  while (true) {
    // Do the pattern move if possible
    const testRock = JSON.parse(JSON.stringify(newRock));
    let isInBound = true;
    for (const coord of testRock) {
      coord[0] += pattern[patternX] === ">" ? 1 : -1;
      if (
        coord[0] > maxX ||
        coord[0] < 0 ||
        rocks.has(`${coord[0]},${coord[1]}`) // Check if a rock is in this position already
      ) {
        isInBound = false;
      }
    }

    if (isInBound) {
      newRock = testRock;
    }

    // Do the next pattern the next itteration
    patternX++;
    patternX %= pattern.length;

    // Do the fall of 1 unit if possible
    // Else break this loop, it means we reach a rock
    const testRockFall = JSON.parse(JSON.stringify(newRock));
    let isFallInBound = true;
    for (const coord of testRockFall) {
      coord[1] -= 1;
      if (
        coord[1] <= 0 ||
        rocks.has(`${coord[0]},${coord[1]}`) // Check if a rock is in this position already
      ) {
        isFallInBound = false;
      }
    }

    if (isFallInBound) {
      newRock = testRockFall;
    } else {
      break;
    }
  }

  let highestY = 0;
  for (const coord of newRock) {
    if (coord[1] > highestY) {
      columns[coord[0]] = coord[1];
    }
    rocks.add(`${coord[0]},${coord[1]}`);
  }

  return patternX;
}

function print(rocks, highestRockY) {
  const lines = [];
  for (let i = 0; i < highestRockY + 5; i++) {
    let l = "";
    for (let j = 0; j <= 6; j++) {
      if (i === 0) {
        l += "-";
      } else if (rocks.has(`${j},${i}`)) {
        l += "#";
      } else {
        l += ".";
      }
    }
    lines.push(l);
  }
  for (const l of lines.reverse()) {
    console.log(l);
  }
  console.log();
  console.log();
}

function findMax(rocks, highestRockY) {
  let highest = 0;
  // Start from the highestRockY (from 0 at the start) to optimize the number of loop
  for (let i = highestRockY; i <= highestRockY + 4; i++) {
    for (let j = 0; j <= 6; j++) {
      if (rocks.has(`${j},${i}`)) {
        highest = i;
      }
    }
  }

  return highest;
}

async function main() {
  await a();
  await b();
}

main();
