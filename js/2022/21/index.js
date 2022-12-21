const fs = require("fs").promises;

async function a() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  const operations = {};

  for (const line of input) {
    const split = line.split(":");
    let operation = split[1].trim();

    if (Number.isInteger(Number(operation))) {
      operation = Number(operation);
    } else {
      operation = operation.split(" ");
    }

    operations[split[0]] = operation;
  }

  const root = getResultPart1(operations, operations.root);
  console.log(root);
}

function getResultPart1(operations, operation) {
  // If it's a number we return directly
  if (Number.isInteger(operation)) {
    return operation;
  }

  let left = operation[0];
  const operator = operation[1];
  let right = operation[2];

  // Left is not a number
  left = getResultPart1(operations, operations[left]);

  // Right is not a number
  right = getResultPart1(operations, operations[right]);

  // Both should be a number here
  if (operator === "+") {
    return left + right;
  } else if (operator === "-") {
    return left - right;
  } else if (operator === "*") {
    return left * right;
  } else if (operator === "/") {
    return left / right;
  }
}

async function b() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  const operations = {};

  for (const line of input) {
    const split = line.split(":");
    let operation = split[1].trim();

    if (Number.isInteger(Number(operation))) {
      operation = Number(operation);
    } else {
      operation = operation.split(" ");
    }

    operations[split[0]] = operation;
  }

  operations.root[1] = "=";
  operations.humn = "x";

  const equation = getResultPart2(operations, operations.root);
  console.log(equation);
}

function getResultPart2(operations, operation) {
  // If it's a number we return directly
  if (Number.isInteger(operation)) {
    return operation;
  } else if (!Array.isArray(operation)) {
    return operation;
  }

  let left = operation[0];
  const operator = operation[1];
  let right = operation[2];

  left = getResultPart2(operations, operations[left]);
  right = getResultPart2(operations, operations[right]);

  // If both are numbers
  if (Number.isInteger(left) && Number.isInteger(right)) {
    if (operator === "+") {
      return left + right;
    } else if (operator === "-") {
      return left - right;
    } else if (operator === "*") {
      return left * right;
    } else if (operator === "/") {
      return left / right;
    }
  } else {
    if (operator === "+") {
      return `(${left} + ${right})`;
    } else if (operator === "-") {
      return `(${left} - ${right})`;
    } else if (operator === "*") {
      return `(${left} * ${right})`;
    } else if (operator === "/") {
      return `(${left} / ${right})`;
    } else if (operator === "=") {
      return `${left} = ${right}`;
    }
  }
}

async function main() {
  await a();
  await b();
}

main();
