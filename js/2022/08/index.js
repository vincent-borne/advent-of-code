const fs = require("fs").promises;

async function a() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  const clean = [];

  for (let i = 0; i < input.length; i++) {
    const item = input[i].split("");
    clean.push([]);

    for (let j = 0; j < item.length; j++) {
      clean[i].push(Number(item[j]));
    }
  }

  let result = 0;

  for (let i = 1; i < clean.length - 1; i++) {
    const row = input[i];
    for (let j = 1; j < row.length - 1; j++) {
      const col = Number(row[j]);

      if (
        checkUpVisible(input, col, i, j) ||
        checkRightVisible(input, col, i, j) ||
        checkDownVisible(input, col, i, j) ||
        checkLeftVisible(input, col, i, j)
      ) {
        result++;
      }
    }
  }

  console.log(result + input.length * 2 + (input[0].length * 2 - 4));
}

function checkUpVisible(input, col, i, j) {
  for (let k = i - 1; k >= 0; k--) {
    if (col <= input[k][j]) {
      return false;
    }
  }

  return true;
}

function checkRightVisible(input, col, i, j) {
  for (let k = j + 1; k < input[i].length; k++) {
    if (col <= input[i][k]) {
      return false;
    }
  }

  return true;
}

function checkDownVisible(input, col, i, j) {
  for (let k = i + 1; k < input.length; k++) {
    if (col <= input[k][j]) {
      return false;
    }
  }

  return true;
}

function checkLeftVisible(input, col, i, j) {
  for (let k = j - 1; k >= 0; k--) {
    if (col <= input[i][k]) {
      return false;
    }
  }

  return true;
}

async function b() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  const clean = [];

  for (let i = 0; i < input.length; i++) {
    const item = input[i].split("");
    clean.push([]);

    for (let j = 0; j < item.length; j++) {
      clean[i].push(Number(item[j]));
    }
  }

  let result = 0;

  for (let i = 1; i < clean.length - 1; i++) {
    const row = input[i];
    for (let j = 1; j < row.length - 1; j++) {
      const col = Number(row[j]);

      const up = checkUpVisibleCount(input, col, i, j);
      const right = checkRightVisibleCount(input, col, i, j);
      const down = checkDownVisibleCount(input, col, i, j);
      const left = checkLeftVisibleCount(input, col, i, j);
      const t = up * right * down * left;

      if (t > result) {
        result = t;
      }
    }
  }
  console.log(result);
}

function checkUpVisibleCount(input, col, i, j) {
  let trees = 0;
  for (let k = i - 1; k >= 0; k--) {
    trees++;

    if (col <= input[k][j]) {
      break;
    }
  }

  return trees;
}

function checkRightVisibleCount(input, col, i, j) {
  let trees = 0;

  for (let k = j + 1; k < input[i].length; k++) {
    trees++;

    if (col <= input[i][k]) {
      break;
    }
  }

  return trees;
}

function checkDownVisibleCount(input, col, i, j) {
  let trees = 0;

  for (let k = i + 1; k < input.length; k++) {
    trees++;

    if (col <= input[k][j]) {
      break;
    }
  }

  return trees;
}

function checkLeftVisibleCount(input, col, i, j) {
  let trees = 0;

  for (let k = j - 1; k >= 0; k--) {
    trees++;

    if (col <= input[i][k]) {
      break;
    }
  }

  return trees;
}

async function main() {
  await a();
  await b();
}

main();
