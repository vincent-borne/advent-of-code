const fs = require("fs").promises;

async function a() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("");

  for (let i = 0; i < input.length; i++) {
    const arr = input.slice(i, i + 4);
    if (new Set(arr).size === arr.length) {
      return console.log(i + 4);
    }
  }
}

async function b() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("");

  for (let i = 0; i < input.length; i++) {
    const arr = input.slice(i, i + 14);
    if (new Set(arr).size === arr.length) {
      return console.log(i + 14);
    }
  }
}

async function main() {
  await a();
  await b();
}

main();
