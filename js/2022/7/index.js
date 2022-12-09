const fs = require("fs").promises;

// a
let score = 0;

async function a() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  let result = {};
  let path = [];

  for (const item of input) {
    if (item.includes("$")) {
      const split = item.split(" ");

      if (split[1] === "cd") {
        if (split[2] === "..") {
          path.pop();
        } else {
          const p = followPath(result, path);
          if (!p[split[2]]) p[split[2]] = { size: 0 };
          path.push(split[2]);
        }
      }
    } else {
      if (!item.includes("dir")) {
        const split = item.split(" ");
        const p = followPath(result, path);
        p.size += Number(split[0]);
      }
    }
  }

  calculateSize(result["/"]);
  console.log(score);
}

// b
const diskSpace = 70000000;
const updateSpace = 30000000;
let needSpace = updateSpace - (diskSpace - 41412830);
const toDelete = [];

async function b() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  let result = {};
  let path = [];

  for (const item of input) {
    if (item.includes("$")) {
      const split = item.split(" ");

      if (split[1] === "cd") {
        if (split[2] === "..") {
          path.pop();
        } else {
          const p = followPath(result, path);
          if (!p[split[2]]) p[split[2]] = { size: 0 };
          path.push(split[2]);
        }
      }
    } else {
      if (!item.includes("dir")) {
        const split = item.split(" ");
        const p = followPath(result, path);
        p.size += Number(split[0]);
      }
    }
  }

  calculateSize(result["/"]);
  console.log(toDelete.sort((a, b) => a - b)[0]);
}

function calculateSize(result) {
  for (const [key] of Object.entries(result)) {
    if (typeof result[key] === "object") {
      result.size += calculateSize(result[key]);
    }
  }

  if (result.size <= 100000) {
    score += result.size;
  }

  if (result.size >= needSpace) {
    toDelete.push(result.size);
  }

  return result.size;
}

function followPath(result, path) {
  let cur = result;

  for (const a of path) {
    cur = cur[a];
  }

  return cur;
}

async function main() {
  await a();
  await b();
}

main();
