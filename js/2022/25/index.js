const fs = require("fs").promises;

async function a() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  let result = 0;

  for (const line of input) {
    const split = line.split("").reverse();
    result += getValue(split);
  }

  console.log(result);
  const items = [1, 2, 0, "=", "-"];
}

function getValue(arr) {
  let result = 0;

  for (let [index, number] of arr.entries()) {
    if (number === "=") {
      number = -2;
    } else if (number === "-") {
      number = -1;
    } else {
      number = Number(number);
    }

    result += number * (index === 0 ? 1 : Math.pow(5, index));
  }

  return result;
}

// "You can do it the other direction, too. Say you have the SNAFU number 2=-01. That's 2 in the 625s place, = (double-minus) in the 125s place, - (minus) in the 25s place, 0 in the 5s place, and 1 in the 1s place. (2 times 625) plus (-2 times 125) plus (-1 times 25) plus (0 times 5) plus (1 times 1). That's 1250 plus -250 plus -25 plus 0 plus 1. 976!"

async function b() {}
async function main() {
  await a();
  await b();
}

main();
