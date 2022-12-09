const fs = require("fs").promises;

async function a() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  let result = {};
  let hPos = { x: 1, y: 1 };
  let tPos = { x: 1, y: 1 };

  for (const item of input) {
    const split = item.split(" ");
    const direction = split[0];
    const times = Number(split[1]);

    for (let i = 0; i < times; i++) {
      result[`${tPos.x}_${tPos.y}`] = {};

      if (direction === "R") {
        hPos.x++;

        if (tPos.x + 1 === hPos.x - 1 && tPos.y === hPos.y) {
          tPos.x++;
        } else if (tPos.x + 1 === hPos.x - 1 && tPos.y + 1 === hPos.y) {
          tPos.x++;
          tPos.y++;
        } else if (tPos.x + 1 === hPos.x - 1 && tPos.y - 1 === hPos.y) {
          tPos.x++;
          tPos.y--;
        }
      } else if (direction === "L") {
        hPos.x--;

        if (tPos.x - 1 === hPos.x + 1 && tPos.y === hPos.y) {
          tPos.x--;
        } else if (tPos.x - 1 === hPos.x + 1 && tPos.y + 1 === hPos.y) {
          tPos.x--;
          tPos.y++;
        } else if (tPos.x - 1 === hPos.x + 1 && tPos.y - 1 === hPos.y) {
          tPos.x--;
          tPos.y--;
        }
      } else if (direction === "U") {
        hPos.y++;

        if (tPos.x + 1 === hPos.x && tPos.y + 1 === hPos.y - 1) {
          tPos.x++;
          tPos.y++;
        } else if (tPos.x - 1 === hPos.x && tPos.y + 1 === hPos.y - 1) {
          tPos.x--;
          tPos.y++;
        } else if (tPos.x === hPos.x && tPos.y + 1 === hPos.y - 1) {
          tPos.y++;
        }
      } else if (direction === "D") {
        hPos.y--;

        if (tPos.x + 1 === hPos.x && tPos.y - 1 === hPos.y + 1) {
          tPos.x++;
          tPos.y--;
        } else if (tPos.x - 1 === hPos.x && tPos.y - 1 === hPos.y + 1) {
          tPos.x--;
          tPos.y--;
        } else if (tPos.x === hPos.x && tPos.y - 1 === hPos.y + 1) {
          tPos.y--;
        }
      }
    }
  }

  console.log(Object.entries(result).length + 1);
}

async function b() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  let result = {};
  const rope = [];

  for (let i = 0; i < 10; i++) {
    rope.push({ x: 1, y: 1 });
  }

  for (const item of input) {
    const split = item.split(" ");
    const direction = split[0];
    let times = Number(split[1]);

    for (let i = 0; i < times; i++) {
      if (direction === "R") {
        rope[0].x++;
      } else if (direction === "L") {
        rope[0].x--;
      } else if (direction === "U") {
        rope[0].y++;
      } else if (direction === "D") {
        rope[0].y--;
      }

      result[`${rope[rope.length - 1].x}_${rope[rope.length - 1].y}`] = {};

      for (let i = 1; i < rope.length; i++) {
        if (
          Math.abs(rope[i - 1].x - rope[i].x) < 2 &&
          Math.abs(rope[i - 1].y - rope[i].y) < 2
        ) {
          continue;
        }

        if (rope[i - 1].y === rope[i].y) {
          if (rope[i - 1].x > rope[i].x) {
            rope[i].x++;
          } else {
            rope[i].x--;
          }
        } else if (rope[i - 1].x === rope[i].x) {
          if (rope[i - 1].y > rope[i].y) {
            rope[i].y++;
          } else {
            rope[i].y--;
          }
        } else {
          if (rope[i - 1].x > rope[i].x) {
            rope[i].x++;
          } else {
            rope[i].x--;
          }
          if (rope[i - 1].y > rope[i].y) {
            rope[i].y++;
          } else {
            rope[i].y--;
          }
        }
      }
    }
  }

  console.log(Object.entries(result).length + 1);
}

async function main() {
  await a();
  await b();
}

main();
