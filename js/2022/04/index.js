const fs = require("fs").promises;

async function a() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  let score = 0;

  for (const item of input) {
    const split = item.split(",");
    const leftSide = split[0].split("-").map((x) => Number(x));
    const rightSide = split[1].split("-").map((x) => Number(x));

    if (
      (rightSide[0] >= leftSide[0] && rightSide[1] <= leftSide[1]) ||
      (rightSide[0] <= leftSide[0] && rightSide[1] >= leftSide[1])
    ) {
      ++score;
    }
  }

  console.log(score);
}

async function b() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  let score = 0;

  for (const item of input) {
    const split = item.split(",");
    const leftSide = split[0].split("-").map((x) => Number(x));
    const rightSide = split[1].split("-").map((x) => Number(x));

    if (rightSide[0] <= leftSide[1] && rightSide[1] >= leftSide[0]) {
      ++score;
    }
  }

  console.log(score);
}

async function main() {
  await a();
  await b();
}

main();
