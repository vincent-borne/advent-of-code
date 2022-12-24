const fs = require("fs").promises;

async function a() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  const blueprints = [];

  for (const line of input) {
    const split = line.split(".");
    const blueprint = [
      [Number(split[0].split(" ").at(-2)), 0, 0, 0],
      [Number(split[1].split(" ").at(-2)), 0, 0, 0],
      [
        Number(split[2].split(" ").at(-5)),
        Number(split[2].split(" ").at(-2)),
        0,
        0,
      ],
      [
        Number(split[3].split(" ").at(-5)),
        0,
        Number(split[3].split(" ").at(-2)),
        0,
      ],
    ];

    blueprints.push(blueprint);
  }

  let result = 0;
  let minutes = 24;

  for (const [index, blueprint] of blueprints.entries()) {
    console.log(`${index + 1}) Simulating...`);
    const geodes = simulateBlueprint(blueprint, minutes);
    console.log(`Find ${geodes} for this blueprint`);
    result += geodes * (index + 1);
  }

  console.log(result);
}

async function b() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  const blueprints = [];

  for (const [index, line] of input.entries()) {
    if (index > 2) break;

    const split = line.split(".");
    const blueprint = [
      [Number(split[0].split(" ").at(-2)), 0, 0, 0],
      [Number(split[1].split(" ").at(-2)), 0, 0, 0],
      [
        Number(split[2].split(" ").at(-5)),
        Number(split[2].split(" ").at(-2)),
        0,
        0,
      ],
      [
        Number(split[3].split(" ").at(-5)),
        0,
        Number(split[3].split(" ").at(-2)),
        0,
      ],
    ];

    blueprints.push(blueprint);
  }

  let minutes = 32;
  const totalGeodes = [];

  for (const [index, blueprint] of blueprints.entries()) {
    console.log(`${index + 1}) Simulating...`);
    const geodes = simulateBlueprint(blueprint, minutes);
    totalGeodes.push(geodes);
    console.log(`Find ${geodes} for this blueprint`);
  }

  console.log(totalGeodes.reduce((sum, value) => sum * value, 1));
}

function simulateBlueprint(blueprint, minutes) {
  const currentResources = [0, 0, 0, 0];
  const currentRobots = [1, 0, 0, 0];

  let queueCount = 1;
  let queueIndex = 0;
  let queue = [[[...currentResources], [...currentRobots], 1]];
  const visited = [];

  let max = 0;

  while (queueCount > queueIndex) {
    // Avoid js max memory limitation
    if (queueIndex > 1_000_000) {
      queue = queue.slice(queueIndex);
      queueCount -= queueIndex;
      queueIndex = 0;
    }

    const [currentResources, currentRobots, minute] = queue[queueIndex++];

    if (minute > minutes) {
      break;
    }

    const key = getHash(currentResources, currentRobots);
    if (!visited[minute]) visited[minute] = new Set();
    if (visited[minute].has(key)) continue;
    visited[minute].add(key);

    let canBuildRobots = canIBuild(currentResources, blueprint);

    for (const [index, robot] of currentRobots.entries()) {
      currentResources[index] += robot;
    }

    if (currentResources[3] >= max) {
      max = currentResources[3];
    } else {
      continue;
    }

    let hasBuiltGeodeRobot = false;

    for (const [index, canBuildRobot] of canBuildRobots.reverse().entries()) {
      if (canBuildRobot) {
        const copyResources = [...currentResources];
        const copyRobots = [...currentRobots];

        for (const [i] of copyResources.entries()) {
          copyResources[i] -= blueprint[3 - index][i];
        }

        ++copyRobots[3 - index];

        queue.push([[...copyResources], [...copyRobots], minute + 1]);
        queueCount++;

        if (index === 0) {
          hasBuiltGeodeRobot = true;
          break;
        }
      }
    }

    if (!hasBuiltGeodeRobot) {
      queue.push([[...currentResources], [...currentRobots], minute + 1]);
      queueCount++;
    }
  }

  return max;
}

function getHash(currentResources, currentRobots) {
  return `${JSON.stringify(currentResources)},${JSON.stringify(currentRobots)}`;
}

function canIBuild(currentResources, blueprint) {
  const canBuildRobots = [false, false, false, false];

  for (const [index, robot] of blueprint.entries()) {
    if (robot.every((resource, index) => resource <= currentResources[index])) {
      canBuildRobots[index] = true;
    }
  }

  return canBuildRobots;
}

async function main() {
  await a();
  await b();
}

main();
