const fs = require("fs").promises;

class Graph {
  constructor(length) {
    this.length = length;
    this.points = new Map();
  }

  addPoint(name, value) {
    if (!this.points.has(name)) {
      this.points.set(name, { value: Number(value), neighbours: new Set() });
    } else {
      this.points.set(name, {
        value: Number(value),
        neighbours: this.points.get(name).neighbours,
      });
    }
  }

  addEdge(source, destination) {
    if (!this.points.has(source)) this.addPoint(source);
    if (!this.points.has(destination)) this.addPoint(destination);

    this.points.get(source).neighbours.add(destination);
    this.points.get(destination).neighbours.add(source);
  }

  print() {
    for (const name of this.points.keys()) {
      const point = this.points.get(name);

      console.log(
        `${name} (${point.value}) -> ${[...point.neighbours].join(" ")}`
      );
    }
  }

  BFS(s) {
    let visited = new Array(this.length);
    for (let i = 0; i < this.length; i++) visited[i] = false;

    let queue = [];

    visited[s] = true;
    queue.push(s);

    while (queue.length > 0) {
      s = queue[0];
      queue.shift();

      this.points.get(s).neighbours.forEach((neighbour) => {
        if (!visited[neighbour]) {
          visited[neighbour] = true;
          queue.push(neighbour);
        }
      });
    }
  }

  dijkstra(source) {
    const dist = {};
    const previous = {};
    for (const [vertex] of this.points) {
      dist[vertex] = Infinity;
      previous[vertex] = null;
    }
    dist[source] = 0;

    let queue = [...this.points.keys()];

    while (queue.length > 0) {
      const u = this.smallestDist(queue, dist);
      queue = queue.filter((v) => v !== u);

      for (const v of this.points.get(u).neighbours) {
        const alt = dist[u] + 1; // 1 = dist between u and v (always 1 in our case)
        if (alt < dist[v]) {
          dist[v] = alt;
          previous[v] = u;
        }
      }
    }

    const distances = [];
    for (const key in dist) {
      distances.push({ name: key, dist: dist[key] });
    }
    return [distances.sort((a, b) => b.dist - a.dist), previous];
  }

  smallestDist(queue, distances) {
    let min = { name: null, value: Infinity };

    for (const v of queue) {
      if (distances[v] <= min.value) {
        min.name = v;
        min.value = distances[v];
      }
    }

    return min.name;
  }
}

async function a() {
  let input = (await fs.readFile(`${__dirname}/test.txt`))
    .toString()
    .split("\n");

  const graph = {};

  for (const line of input) {
    const split = line.split(";");
    const point = {
      name: split[0].split(" ")[1],
      value: split[0].split("=")[1].split(";")[0],
      neighbours: [],
      visited: false,
    };

    if (split[1].includes("valves")) {
      for (const link of split[1].split("valves")[1].split(",")) {
        point.neighbours.push(link.trim());
      }
    } else {
      point.neighbours.push(split[1].split("valve")[1].trim());
    }

    graph[point.name] = point;
  }

  let queue = [graph["AA"]];

  while (queue.length > 0) {
    s = queue.shift();
    console.log(s);

    s.neighbours.forEach((neighbour) => {
      if (!queue[neighbour].visited) {
        queue[neighbour].visited = true;
        queue.push(queue[neighbour]);
      }
    });
  }
}

async function b() {}

async function main() {
  await a();
  await b();
}

main();
