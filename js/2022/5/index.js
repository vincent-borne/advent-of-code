const fs = require("fs").promises;

async function a() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  const stacks = getStacks(input);

  for (const item of input) {
    if (!item.includes("move")) continue;

    const split = item.split(" ");
    const times = Number(split[1]);
    const from = stacks[Number(split[3]) - 1];
    const to = stacks[Number(split[5]) - 1];

    for (let i = 0; i < times; i++) {
      to.unshift(from[0]);
      from.shift();
    }
  }

  console.log(stacks.map((arr) => arr[0]).join(""));
}

async function b() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  const stacks = getStacks(input);

  for (const item of input) {
    if (!item.includes("move")) continue;

    const split = item.split(" ");
    const times = Number(split[1]);
    const from = stacks[Number(split[3]) - 1];
    const to = stacks[Number(split[5]) - 1];

    for (let i = 0; i < times; i++) {
      to.splice(i, 0, from[0]);
      from.shift();
    }
  }

  console.log(stacks.map((arr) => arr[0]).join(""));
}

function getStacks(input) {
  const stacks = [];
  let stackIndex = 0;

  for (const item of input) {
    const split = item.split("");
    if (split.length === 0) break;

    let nbOfSpaces = 0;

    for (let i = 0; i < split.length; i++) {
      if (split[i] === " ") ++nbOfSpaces;
      if (nbOfSpaces === 4) {
        nbOfSpaces = 0;
        if (!stacks[stackIndex]) stacks.push([]);
        ++stackIndex;
      }

      if (split[i] === "[") {
        nbOfSpaces = 0;
        if (!stacks[stackIndex]) stacks.push([]);
        stacks[stackIndex].push(split[i + 1]);
        ++stackIndex;
      }
    }

    stackIndex = 0;
  }

  return stacks;
}

async function main() {
  await a();
  await b();
}

main();
