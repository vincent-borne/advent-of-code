const fs = require("fs").promises;

async function a() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  const monkeys = [];

  for (let i = 0; i < input.length; i++) {
    monkeys[Number(input[i].split(" ")[1].substring(0, 1))] = {
      items: input[i + 1]
        .split(":")[1]
        .split(",")
        .map((a) => Number(a)),
      operation: input[i + 2].split(":")[1].split("=")[1].trim(),
      test: Number(input[i + 3].split(" ")[5]),
      ifTrue: Number(input[i + 4].split(" ")[9]),
      ifFalse: Number(input[i + 5].split(" ")[9]),
      inspected: 0,
    };

    i += 6;
  }

  for (let i = 0; i < 20; i++) {
    for (const monkey of monkeys) {
      if (monkey.items.length <= 0) continue;

      for (const item of monkey.items) {
        const operation = eval(monkey.operation.replaceAll("old", item));
        const worryLevel = Math.floor(operation / 3);

        if (worryLevel % monkey.test === 0) {
          monkeys[monkey.ifTrue].items.push(worryLevel);
        } else {
          monkeys[monkey.ifFalse].items.push(worryLevel);
        }

        ++monkey.inspected;
      }

      monkey.items = [];
    }
  }

  const inspected = monkeys.map((monkey) => monkey.inspected);
  console.log(
    inspected
      .sort((a, b) => b - a)
      .slice(0, 2)
      .reduce((sum, n) => sum * n, 1)
  );
}

async function b() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  const monkeys = [];

  for (let i = 0; i < input.length; i++) {
    monkeys[Number(input[i].split(" ")[1].substring(0, 1))] = {
      items: input[i + 1]
        .split(":")[1]
        .split(",")
        .map((a) => Number(a)),
      operation: input[i + 2].split(":")[1].split("=")[1].trim(),
      test: Number(input[i + 3].split(" ")[5]),
      ifTrue: Number(input[i + 4].split(" ")[9]),
      ifFalse: Number(input[i + 5].split(" ")[9]),
      inspected: 0,
    };

    i += 6;
  }

  const allDiv = monkeys
    .map((monkey) => monkey.test)
    .reduce((r, n) => r * n, 1);

  for (let i = 0; i < 10000; i++) {
    for (const monkey of monkeys) {
      if (monkey.items.length <= 0) continue;

      for (const item of monkey.items) {
        const operation = eval(monkey.operation.replaceAll("old", item));
        const worryLevel = operation % allDiv;

        if (operation % monkey.test === 0) {
          monkeys[monkey.ifTrue].items.push(worryLevel);
        } else {
          monkeys[monkey.ifFalse].items.push(worryLevel);
        }

        ++monkey.inspected;
      }

      monkey.items = [];
    }
  }

  const inspected = monkeys.map((monkey) => monkey.inspected);
  console.log(
    inspected
      .sort((a, b) => b - a)
      .slice(0, 2)
      .reduce((sum, n) => sum * n, 1)
  );
}

async function main() {
  await a();
  await b();
}

main();
