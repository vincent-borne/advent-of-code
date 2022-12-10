const fs = require("fs").promises;

async function a() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  let score = 0;

  for (const item of input) {
    const leftStack = item.slice(0, item.length / 2).split("");
    const rightStack = item.slice(item.length / 2);
    const letter = leftStack.find((letter) => rightStack.includes(letter));
    score += getLetterValue(letter);
  }

  console.log(score);
}

async function b() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  let score = 0;

  for (let i = 0; i < input.length; i++) {
    const firstStack = input[i].split("");
    const letter = firstStack.find(
      (letter) => input[i + 1].includes(letter) && input[i + 2].includes(letter)
    );

    score += getLetterValue(letter);
    i += 2;
  }

  console.log(score);
}

function getLetterValue(letter) {
  let value = letter.charCodeAt(0) % 97;

  if (value > 26) {
    value -= 39;
  }

  return ++value;
}

async function main() {
  await a();
  await b();
}

main();
