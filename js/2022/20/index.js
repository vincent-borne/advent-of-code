const fs = require("fs").promises;

async function a() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  const indexes = [];
  const list = [];

  for (const [index, line] of input.entries()) {
    indexes.push({ value: Number(line), index });
    list.push(Number(line));
  }

  for (const item of indexes) {
    list.splice(item.index, 1);

    let newI = (item.index + item.value) % list.length;
    list.splice(newI, 0, item.value);

    if (newI < 0) {
      newI += list.length - 1;
    }

    if (newI > item.index) {
      indexes.forEach((a) => {
        if (a.value === item.value && a.index === item.index) return;

        if (a.index <= newI && a.index > item.index) {
          --a.index;
        }
      });
    } else if (newI < item.index) {
      indexes.forEach((a) => {
        if (a.value === item.value && a.index === item.index) return;

        if (a.index >= newI && a.index < item.index) {
          ++a.index;
        }
      });
    }

    item.index = newI;
  }

  const zeroIndex = list.findIndex((n) => n === 0);
  const result = [...list.slice(zeroIndex), ...list.slice(0, zeroIndex)];
  const r1 = 1000 % result.length;
  const r2 = 2000 % result.length;
  const r3 = 3000 % result.length;

  console.log(result[r1] + result[r2] + result[r3]);
}

async function b() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  const indexes = [];
  const list = [];

  for (const [index, line] of input.entries()) {
    const value = Number(line) * 811589153;
    indexes.push({ value: value, index });
    list.push(value);
  }

  for (let i = 0; i < 10; i++) {
    for (const item of indexes) {
      list.splice(item.index, 1);

      let newI = (item.index + item.value) % list.length;
      list.splice(newI, 0, item.value);

      if (newI < 0) {
        newI += list.length - 1;
      }

      if (newI > item.index) {
        indexes.forEach((a) => {
          if (a.value === item.value && a.index === item.index) return;

          if (a.index <= newI && a.index > item.index) {
            --a.index;
          }
        });
      } else if (newI < item.index) {
        indexes.forEach((a) => {
          if (a.value === item.value && a.index === item.index) return;

          if (a.index >= newI && a.index < item.index) {
            ++a.index;
          }
        });
      }

      item.index = newI;
    }
  }

  const zeroIndex = list.findIndex((n) => n === 0);
  const result = [...list.slice(zeroIndex), ...list.slice(0, zeroIndex)];
  const r1 = 1000 % result.length;
  const r2 = 2000 % result.length;
  const r3 = 3000 % result.length;

  console.log(result[r1] + result[r2] + result[r3]);
}

async function main() {
  await a();
  await b();
}

main();
