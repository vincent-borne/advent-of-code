const fs = require("fs").promises;

async function a() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  let bestScore = 0;
  let currentScore = 0;

  for (const item of input) {
    if (item === "") {
      if (currentScore > bestScore) {
        bestScore = currentScore;
      }

      currentScore = 0;
    } else {
      currentScore += Number(item);
    }
  }

  console.log(bestScore);
}

async function b() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  let scores = [];
  let currentScore = 0;

  for (const item of input) {
    if (item === "") {
      scores.push(currentScore);
      currentScore = 0;
    } else {
      currentScore += Number(item);
    }
  }

  console.log(
    scores
      .sort((a, b) => b - a)
      .slice(0, 3)
      .reduce((sum, a) => sum + a, 0)
  );
}

async function main() {
  await a();
  await b();
}

main();
