const fs = require("fs").promises;

const win = [-1, 2];
const draw = 0;

async function a() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  let score = 0;

  for (const item of input) {
    const split = item.split(" ");
    const enemyChoice = (split[0].charCodeAt(0) % 65) + 1;
    const myChoice = (split[1].charCodeAt(0) % 88) + 1;
    score += myChoice;

    const cmp = enemyChoice - myChoice;
    if (cmp === draw) {
      score += 3;
    } else if (win.includes(cmp)) {
      score += 6;
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
    const split = item.split(" ");
    const enemyChoice = (split[0].charCodeAt(0) % 65) + 1;
    let myChoice = (split[1].charCodeAt(0) % 88) + 1;

    if (myChoice === 1) {
      while ([draw, ...win].includes(enemyChoice - myChoice)) {
        myChoice++;
      }
    } else if (myChoice === 2) {
      myChoice = enemyChoice;
    } else {
      while (!win.includes(enemyChoice - myChoice)) {
        myChoice--;
      }
    }

    score += myChoice;

    const cmp = enemyChoice - myChoice;
    if (cmp === draw) {
      score += 3;
    } else if (win.includes(cmp)) {
      score += 6;
    }
  }

  console.log(score);
}

async function main() {
  await a();
  await b();
}

main();
