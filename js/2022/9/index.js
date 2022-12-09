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
  const ropes = [];

  for (let i = 0; i < 10; i++) {
    ropes.push({ x: 1, y: 1 });
  }

  for (const item of input) {
    const split = item.split(" ");
    const direction = split[0];
    let times = Number(split[1]);

    for (let i = 0; i < times; i++) {
      if (direction === "R") {
        ropes[0].x++;
      } else if (direction === "L") {
        ropes[0].x--;
      } else if (direction === "U") {
        ropes[0].y++;
      } else if (direction === "D") {
        ropes[0].y--;
      }

      result[`${ropes[ropes.length - 1].x}_${ropes[ropes.length - 1].y}`] = {};

      for (let i = 1; i < ropes.length; i++) {
        if (
          Math.abs(ropes[i - 1].x - ropes[i].x) < 2 &&
          Math.abs(ropes[i - 1].y - ropes[i].y) < 2
        ) {
          continue;
        }

        if (ropes[i - 1].y === ropes[i].y) {
          if (ropes[i - 1].x > ropes[i].x) {
            ropes[i].x++;
          } else {
            ropes[i].x--;
          }
        } else if (ropes[i - 1].x === ropes[i].x) {
          if (ropes[i - 1].y > ropes[i].y) {
            ropes[i].y++;
          } else {
            ropes[i].y--;
          }
        } else {
          if (ropes[i - 1].x > ropes[i].x) {
            ropes[i].x++;
          } else {
            ropes[i].x--;
          }
          if (ropes[i - 1].y > ropes[i].y) {
            ropes[i].y++;
          } else {
            ropes[i].y--;
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
