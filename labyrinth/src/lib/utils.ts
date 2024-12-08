import { GridData, GridDimensions, Route, RouteStep } from "./types";
import { promises as fs } from "fs";

/**
 * Parses input data in the format:
 * net1 (1, 10, 20) (1, 11, 20) (1, 12, 20)
 * net2 (2, 100, 200) (2, 100, 201) (2, 100, 202)
 */
export const readRoutes = async (): Promise<Route[]> => {
    // Read the file content
    const file = await fs.readFile(
        process.cwd() + "/src/app/input.txt",
        "utf8"
    );

    // Split the content into lines and process each line
    const lines = file.split("\n").filter((line) => line.trim() !== "");

    const routes: Route[] = [];

    let prevNet = "";

    for (const line of lines) {
        const [net, ...points] = line.split(/\s+/);

        const path: RouteStep[] = points.map((point) => {
            const match = point.match(/\((\d+),\s*(\d+),\s*(\d+)\)/);
            if (!match) throw new Error(`Invalid format: ${point}`);
            // eslint-disable-next-line @typescript-eslint/no-unused-vars
            const [_, layer, x, y] = match.map(Number);
            return { layer, x, y, direction: "horizontal", isVia: false }; // Default direction is 'horizontal'
        });

        if (net === prevNet) {
            let prevRouteIndex = routes.length - 1;
            let found = false;
            while (
                prevRouteIndex >= 0 &&
                routes[prevRouteIndex].net === net &&
                !found
            ) {
                for (const step of routes[prevRouteIndex].path) {
                    if (step.x === path[0].x && step.y === path[0].y) {
                        if (step.layer !== path[0].layer) {
                            path[0].isVia = true;
                        } else {
                            if (path.length > 1) {
                                if (
                                    step.x === path[1].x &&
                                    step.y !== path[1].y &&
                                    step.direction === "horizontal"
                                ) {
                                    path[0].direction = "opposite";
                                } else if (
                                    step.x !== path[1].x &&
                                    step.y === path[1].y &&
                                    step.direction === "vertical"
                                ) {
                                    path[0].direction = "opposite";
                                } else {
                                    path[0].direction = step.direction;
                                }
                            }
                            path[0].isVia = step.isVia;
                        }

                        found = true;
                        break;
                    }
                }
                prevRouteIndex--;
            }
        }
        for (let i = 1; i < path.length - 1; i++) {
            const prev = path[i - 1];
            const curr = path[i];
            const next = path[i + 1];

            if (curr.layer === prev.layer) {
                if (
                    curr.x !== prev.x &&
                    (curr.x !== next.x || curr.layer !== next.layer)
                ) {
                    path[i].direction = "horizontal";
                } else if (
                    curr.y !== prev.y &&
                    (curr.y !== next.y || curr.layer !== next.layer)
                ) {
                    path[i].direction = "vertical";
                } else if (curr.x === prev.x && curr.y === next.y) {
                    path[i].direction = "opposite";
                } else if (curr.x === next.x && curr.y === prev.y) {
                    path[i].direction = "opposite";
                }
            } else {
                path[i].isVia = true;
                path[i - 1].isVia = true;
                if (i + 1 < path.length) {
                    const next = path[i + 1];

                    if (next.layer === curr.layer) {
                        if (next.x !== curr.x && next.y === curr.y) {
                            // Horizontal direction: x changes, y remains the same
                            path[i].direction = "horizontal";
                        } else if (next.y !== curr.y && next.x === curr.x) {
                            // Vertical direction: y changes, x remains the same
                            path[i].direction = "vertical";
                        }
                    }
                }
            }
        }

        prevNet = net;
        routes.push({ net, path });
    }

    return routes;
};

export const fillGridData = (
    routes: Route[],
    rows: number,
    cols: number
): GridData => {
    const grid: GridData = Array.from({ length: rows }, () =>
        Array.from({ length: cols }, () => ({
            net: undefined,
            M1: undefined,
            M2: undefined,
            via: "",
            source: "",
            target: "",
        }))
    );

    let counter = 0;
    let prevNet = "";

    for (const { net, path } of routes) {
        for (const { layer, x, y, direction, isVia } of path) {
            if (counter === 0 && net !== prevNet) {
                grid[y][x].source = net;
            } else if (counter === path.length - 1) {
                grid[y][x].target = net;
            }

            if (layer === 0) {
                grid[y][x].M1 = { net, direction };
            } else if (layer === 1) {
                grid[y][x].M2 = { net, direction };
            }

            grid[y][x].via = isVia ? net : "";
            counter++;
        }
        counter = 0;
        prevNet = net;
    }

    return grid;
};

export const getNets = (routes: Route[]) => {
    const nets = new Set<string>();

    for (const route of routes) {
        nets.add(route.net);
    }

    return Array.from(nets);
};

export const readGridDimensions = async (): Promise<GridDimensions> => {
    const file = await fs.readFile(
        process.cwd() + "/src/app/grid_dimensions.txt",
        "utf8"
    );
    const lines = file.split("\n");
    const [cols, rows] = lines[0].split(" ").map(Number);
    return { rows, cols };
};
