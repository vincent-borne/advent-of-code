const fs = require("fs").promises;

function moveHead(direction, head) {
  if (direction === "R") {
    head.x++;
  } else if (direction === "L") {
    head.x--;
  } else if (direction === "U") {
    head.y++;
  } else if (direction === "D") {
    head.y--;
  }
}

function follow(head, tail) {
  if (Math.abs(tail.x - head.x) < 2 && Math.abs(tail.y - head.y) < 2) {
    return;
  }

  const newX = (tail.x + head.x) / 2;
  const newY = (tail.y + head.y) / 2;

  const isFloat = (n) => {
    return n % 1 !== 0;
  };

  tail.x = isFloat(newX) ? head.x : newX;
  tail.y = isFloat(newY) ? head.y : newY;
}

async function a() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  const result = new Set();
  let head = { x: 1, y: 1 };
  let tail = { x: 1, y: 1 };

  for (const item of input) {
    const split = item.split(" ");
    const direction = split[0];
    const times = Number(split[1]);

    for (let i = 0; i < times; i++) {
      result.add(`${tail.x},${tail.y}`);
      moveHead(direction, head);
      follow(head, tail);
    }

    result.add(`${tail.x},${tail.y}`);
  }

  console.log(result.size);
}

async function b() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  const result = new Set();
  const ropeSize = 10;
  const rope = new Array(ropeSize).fill(0).map((_) => ({ x: 1, y: 1 }));
  const head = rope[0];
  const tail = rope[rope.length - 1];

  for (const item of input) {
    const split = item.split(" ");
    const direction = split[0];
    const times = Number(split[1]);

    for (let i = 0; i < times; i++) {
      moveHead(direction, head);

      for (let j = 0; j < rope.length; j++) {
        result.add(`${tail.x},${tail.y}`);
        if (j + 1 === rope.length) continue;
        follow(rope[j], rope[j + 1]);
      }
    }

    result.add(`${tail.x},${tail.y}`);
  }

  console.log(result.size);
}

async function main() {
  await a();
  await b();
}

main();
