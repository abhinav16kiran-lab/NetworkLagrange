# NetworkLagrange

C++ optimizer that finds the ideal server placement by minimizing weighted latency across a set of cities — inspired by the Lagrange point analogy from orbital mechanics.

## How it works

Each city is a `Node` with a latitude, longitude, and traffic weight. The optimizer runs gradient descent over geographic coordinates, using the Haversine formula to compute real-world distances, converging on the `Server` location that minimizes total weighted latency.

## Files

| File | Description |
|------|-------------|
| `optimizer.hpp / .cpp` | Core optimizer — gradient descent + Haversine distance |
| `map_generator.hpp` | Generates GeoJSON output for visualization |
| `nodes.csv` | Input city data (lat, lon, weight) |
| `map_result.geojson` | Output — visualize in geojson.io |
| `main.cpp` | Entry point |

## Build & run

```bash
g++ -O2 -o optimizer main.cpp optimizer.cpp
./optimizer
```

Output is written to `map_result.geojson`. Drop it into [geojson.io](https://geojson.io) to see the result on a map.

## License

MIT
