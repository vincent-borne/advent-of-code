const fs = require("fs").promises;

async function a() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  const sensors = [];
  const sensorsSet = new Set();
  const beacons = new Set();

  for (const line of input) {
    const split = line.split("=");
    const sensor = {
      coord: [Number(split[1].split(",")[0]), Number(split[2].split(":")[0])],
    };
    const beaconX = Number(split[3].split(",")[0]);
    const beaconY = Number(split[4]);
    beacons.add(`${beaconX},${beaconY}`);
    sensorsSet.add(`${sensor.coord[0]},${sensor.coord[1]}`);
    sensor.d = distance(sensor.coord, [beaconX, beaconY]);
    sensors.push(sensor);
  }

  const result = new Set();
  const lookingFor = 2000000;
  for (const sensor of sensors) {
    let [x, y] = sensor.coord;
    for (let j = 0; j <= sensor.d; j++) {
      const newY = y + j;
      if (newY !== lookingFor) continue;

      for (let i = 1; i <= sensor.d; i++) {
        const newX = x + i - j;
        if (newX < x) continue;

        if (
          !beacons.has(`${newX},${newY}`) &&
          !sensorsSet.has(`${newX},${newY}`)
        ) {
          result.add(`${newX},${newY}`);
        }
      }
    }
    for (let j = sensor.d; j >= 0; j--) {
      const newY = y - j;
      if (newY !== lookingFor) continue;

      for (let i = 1; i <= sensor.d; i++) {
        const newX = x + i - j;
        if (newX < x) continue;
        if (
          !beacons.has(`${newX},${newY}`) &&
          !sensorsSet.has(`${newX},${newY}`)
        ) {
          result.add(`${newX},${newY}`);
        }
      }
    }
    for (let j = sensor.d; j >= 0; j--) {
      const newY = y - j;
      if (newY !== lookingFor) continue;
      for (let i = 1; i <= sensor.d; i++) {
        const newX = x - i + j;
        if (newX > x) continue;
        if (
          !beacons.has(`${newX},${newY}`) &&
          !sensorsSet.has(`${newX},${newY}`)
        ) {
          result.add(`${newX},${newY}`);
        }
      }
    }
    for (let j = 0; j <= sensor.d; j++) {
      const newY = y + j;
      if (newY !== lookingFor) continue;

      for (let i = 1; i <= sensor.d; i++) {
        const newX = x - i + j;
        if (newX > x) continue;
        if (
          !beacons.has(`${newX},${newY}`) &&
          !sensorsSet.has(`${newX},${newY}`)
        ) {
          result.add(`${newX},${newY}`);
        }
      }
    }
  }

  console.log(result.size);
}

async function b() {
  let input = (await fs.readFile(`${__dirname}/input.txt`))
    .toString()
    .split("\n");

  const devices = [];

  for (const line of input) {
    const split = line.split("=");
    const device = {
      sensor: [Number(split[1].split(",")[0]), Number(split[2].split(":")[0])],
      beacon: [Number(split[3].split(",")[0]), Number(split[4])],
    };

    device.d = distance(device.sensor, device.beacon);
    devices.push(device);
  }

  const max = 4000000;

  for (let y = 0; y < max; y++) {
    let intervals = [];

    // Create intervals
    for (const device of devices) {
      const d = distance(device.sensor, [device.sensor[0], y]);

      if (d <= device.d) {
        intervals.push([
          device.sensor[0] - (device.d - d),
          device.sensor[0] + (device.d - d),
        ]);
      }
    }

    // Sort the intervals
    intervals.sort((a, b) => a[0] - b[0]);

    // Merge the intervals
    for (let i = 0; i < intervals.length - 1; i++) {
      if (intervals[i][1] >= intervals[i + 1][0]) {
        intervals[i][1] = Math.max(intervals[i][1], intervals[i + 1][1]);
        intervals.splice(i + 1, 1);
        i--;
      }
    }

    const result = [];
    for (const interval of intervals) {
      if (interval[0] > max || interval[1] < 0) continue;
      result.push([Math.max(0, interval[0]), Math.min(interval[1], max)]);
    }

    if (result.length <= 1) continue;

    console.log((result[0][1] + 1) * 4e6 + y);
    break;
  }
}

function distance(a, b) {
  return Math.abs(a[0] - b[0]) + Math.abs(a[1] - b[1]);
}

async function main() {
  await a();
  await b();
}

main();
