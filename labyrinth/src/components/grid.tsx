"use client";

import { GridData } from "@/lib/types";
import Cell from "./cell";
import MultiSelectDropdown from "./multi-select-dropdown";
import { useState } from "react";

interface GridProps {
    grid: GridData;
    nets: string[];
}

export default function Grid({ grid, nets }: GridProps) {
    const [selectedNets, setSelectedNets] = useState<string[]>([]);

    return (
        <div className="flex gap-5 h-full w-full justify-center relative">
            <div className="overflow-auto content-center">
                <div
                    className="grid"
                    style={{
                        display: "grid",
                        gridTemplateColumns: `repeat(${grid[0].length}, 1fr)`, // Ensure columns are evenly spaced
                        gridAutoRows: "1fr", // Ensure rows are evenly spaced
                    }}
                >
                    {grid.map((row, rowIndex) =>
                        row.map((cell, colIndex) => {
                            return (
                                <Cell
                                    key={`${rowIndex}-${colIndex}`}
                                    cell={cell}
                                    selectedNets={selectedNets}
                                />
                            );
                        })
                    )}
                </div>
            </div>
            <div className="basis-1/4 flex justify-center fixed top-0 right-0 p-4">
                <MultiSelectDropdown
                    formFieldName="Nets"
                    options={nets}
                    onChange={setSelectedNets}
                />
            </div>
        </div>
    );
}
