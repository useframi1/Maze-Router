import { GridData, Route, RouteStep, MetalLayerData } from "./types";
import { promises as fs } from "fs";

/**
 * Parses input data in the format:
 * net1 (1, 10, 20) (1, 11, 20) (1, 12, 20)
 * net2 (2, 100, 200) (2, 100, 201) (2, 100, 202)
 */
export const readFileAndParseData = async (): Promise<Route[]> => {
    // Read the file content
    const file = await fs.readFile(
        process.cwd() + "/src/app/input.txt",
        "utf8"
    );

    // Split the content into lines and process each line
    const lines = file.split("\n").filter((line) => line.trim() !== "");

    const routes: Route[] = [];

    for (const line of lines) {
        const [net, ...points] = line.split(/\s+/);

        const path: RouteStep[] = points.map((point) => {
            const match = point.match(/\((\d+),\s*(\d+),\s*(\d+)\)/);
            if (!match) throw new Error(`Invalid format: ${point}`);
            // eslint-disable-next-line @typescript-eslint/no-unused-vars
            const [_, layer, x, y] = match.map(Number);
            return { layer, x, y, direction: "horizontal", isVia: false }; // Default direction is 'horizontal'
        });

        // Infer directions for the path and mark opposite directions
        for (let i = 1; i < path.length; i++) {
            const prev = path[i - 1];
            const curr = path[i];

            if (curr.layer === prev.layer) {
                if (curr.x !== prev.x && curr.y === prev.y) {
                    // Horizontal direction: x changes, y remains the same
                    path[i].direction = "horizontal";
                } else if (curr.y !== prev.y && curr.x === prev.x) {
                    // Vertical direction: y changes, x remains the same
                    path[i].direction = "vertical";
                }

                // Mark opposite direction when there's a change from horizontal to vertical or vice versa
                if (i > 1) {
                    const prevPrev = path[i - 2];
                    const prevDirection = prevPrev.direction;
                    const currDirection = path[i].direction;
                    if (path[i - 2].layer === curr.layer) {
                        if (
                            (prevDirection === "horizontal" &&
                                currDirection === "vertical") ||
                            (prevDirection === "vertical" &&
                                currDirection === "horizontal")
                        ) {
                            path[i - 1].direction = "opposite";
                        }
                    }
                }
            } else {
                path[i].isVia = true;
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
            via: false,
            isSource: false,
            isTarget: false,
        }))
    );

    let counter = 0;

    for (const { net, path } of routes) {
        for (const { layer, x, y, direction, isVia } of path) {
            if (counter === 0) {
                grid[y][x].isSource = true;
            } else if (counter === path.length - 1) {
                grid[y][x].isTarget = true;
            }

            grid[y][x].net = net;
            if (layer === 1) {
                grid[y][x].M1 = direction;
            } else if (layer === 2) {
                grid[y][x].M2 = direction;
            }

            grid[y][x].via = isVia;
            counter++;
        }
        counter = 0;
    }

    return grid;
};
