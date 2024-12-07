export type Direction = "horizontal" | "vertical" | "opposite";

export interface RouteStep {
    layer: number; // 1 for M1, 2 for M2
    x: number; // X coordinate
    y: number; // Y coordinate
    direction: Direction;
    isVia: boolean;
}

export interface Route {
    net: string;
    path: RouteStep[];
}

export interface MetalLayerData {
    net: string;
    direction: Direction;
}

export interface CellData {
    net: string | undefined;
    M1: Direction | undefined; // Metal Layer 1 (horizontal preferred)
    M2: Direction | undefined; // Metal Layer 2 (vertical preferred)
    via: boolean; // Nets with vias
    isSource: boolean;
    isTarget: boolean;
}

export type GridData = CellData[][];
