export type Direction = "horizontal" | "vertical" | "opposite";

export interface GridDimensions {
    rows: number;
    cols: number;
}

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
    M1: MetalLayerData | undefined; // Metal Layer 1 (horizontal preferred)
    M2: MetalLayerData | undefined; // Metal Layer 2 (vertical preferred)
    via: string; // Nets with vias
    source: string;
    target: string;
}

export type GridData = CellData[][];
