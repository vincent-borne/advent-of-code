const fs = require("fs").promises;

const directions = [
  [0, 1],
  [1, 0],
  [0, -1],
  [-1, 0],
];

async function a() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  let start;
  let end;
  const grid = [];

  for (const [x, row] of input.entries()) {
    grid.push([]);

    for (let [y, column] of row.split("").entries()) {
      if (column === "S") {
        column = "a";
        start = [x, y];
      } else if (column === "E") {
        column = "z";
        end = [x, y];
      }

      grid[grid.length - 1].push(getValue(column));
    }
  }

  console.log(search(grid, start, end));
}

function getHashKey(x, y) {
  return `${x},${y}`;
}

function getValue(letter) {
  return letter.charCodeAt(0) % 97;
}

async function b() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  const starts = [];
  let end;
  const grid = [];

  for (const [x, row] of input.entries()) {
    grid.push([]);

    for (let [y, column] of row.split("").entries()) {
      // Take all the "a" as starts, to try all of them as a start path
      if (["S", "a"].includes(column)) {
        column = "a";
        starts.push([x, y]);
      } else if (column === "E") {
        column = "z";
        end = [x, y];
      }

      grid[grid.length - 1].push(getValue(column));
    }
  }

  // Make a search on all the starts and get the smallest depth
  const result = [];
  for (const start of starts) {
    result.push(search(grid, start, end));
  }

  console.log(result.sort((a, b) => a - b)[0]);
}

function search(grid, start, end) {
  const visited = new Set();
  const stack = [{ position: start, depth: 0 }];

  while (stack.length > 0) {
    const item = stack.shift();
    const [x, y] = item.position;
    const depth = item.depth;

    const hashKey = getHashKey(x, y);
    if (visited.has(hashKey)) {
      continue;
    }

    // Found the end node
    if (x === end[0] && y === end[1]) {
      return depth;
    }

    for (const [dx, dy] of directions) {
      // Limitation of the grid
      if (
        x + dx < 0 ||
        x + dx >= grid.length ||
        y + dy < 0 ||
        y + dy >= grid[0].length
      ) {
        continue;
      }

      // Already visited or the next index is too high
      if (
        visited.has(getHashKey(x + dx, y + dy)) ||
        grid[x + dx][y + dy] > grid[x][y] + 1
      ) {
        continue;
      }

      // Go through the next possible node
      stack.push({ position: [x + dx, y + dy], depth: depth + 1 });
    }

    visited.add(hashKey);
  }
}

async function main() {
  await a();
  await b();
}

main();
