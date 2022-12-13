const fs = require("fs").promises;

async function a() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  let result = 0;
  let j = 1;

  for (let i = 0; i < input.length - 1; i++) {
    if (input[i] === "" || input[i + 1] === "") continue;

    if (recursiveIsOrdered(JSON.parse(input[i]), JSON.parse(input[i + 1]))) {
      result += j;
    }

    ++j;
  }

  console.log(result);
}

async function b() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  let pairs = [];

  for (let i = 0; i < input.length; i++) {
    if (input[i] !== "") {
      pairs.push(JSON.parse(input[i]));
    }
  }

  // Two new packets are at the end
  const packetStart = pairs[pairs.length - 2];
  const packetEnd = pairs[pairs.length - 1];

  // Sort each list using the recursive function of part 1
  pairs.sort((a, b) => (recursiveIsOrdered(a, b) ? -1 : 1));

  const key1 = pairs.findIndex((list) => list === packetStart) + 1;
  const key2 = pairs.findIndex((list) => list === packetEnd) + 1;
  console.log(key1 * key2);
}

function recursiveIsOrdered(left, right) {
  // If the left list runs out of items first, the inputs are in the right order. If the right list runs out of items first, the inputs are not in the right order. If the lists are the same length and no comparison makes a decision about the order, continue checking the next part of the input.
  if (left[0] == null && right[0] != null) {
    return true;
  } else if (left[0] != null && right[0] == null) {
    return false;
  }

  for (let i = 0; i < left.length; i++) {
    // If both values are integers, the lower integer should come first. If the left integer is lower than the right integer, the inputs are in the right order. If the left integer is higher than the right integer, the inputs are not in the right order. Otherwise, the inputs are the same integer; continue checking the next part of the input.
    if (Number.isInteger(left[i]) && Number.isInteger(right[i])) {
      if (left[i] !== right[i]) {
        return left[i] < right[i];
      }
    }
    // If both values are lists, compare the first value of each list, then the second value, and so on.
    else if (Array.isArray(left[i]) && Array.isArray(right[i])) {
      const result = recursiveIsOrdered(left[i], right[i]);
      if (result != null) return result;
    }
    // If exactly one value is an integer, convert the integer to a list which contains that integer as its only value, then retry the comparison. For example, if comparing [0,0,0] and 2, convert the right value to [2] (a list containing 2); the result is then found by instead comparing [0,0,0] and [2].
    else if (Array.isArray(left[i]) && !Array.isArray(right[i])) {
      const result = recursiveIsOrdered(left[i], [right[i]]);
      if (result != null) return result;
    } else if (!Array.isArray(left[i]) && Array.isArray(right[i])) {
      const result = recursiveIsOrdered([left[i]], right[i]);
      if (result != null) return result;
    }
  }

  // If the left list runs out of items first, the inputs are in the right order. If the right list runs out of items first, the inputs are not in the right order. If the lists are the same length and no comparison makes a decision about the order, continue checking the next part of the input.
  if (left.length < right.length) {
    return true;
  } else if (left.length > right.length) {
    return false;
  }
}

async function main() {
  await a();
  await b();
}

main();
