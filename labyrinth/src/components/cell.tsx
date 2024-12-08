import { CellData, Direction } from "@/lib/types";

interface CellProps {
    cell: CellData; // A single cell's data
    selectedNets: string[];
}

export default function Cell({ cell, selectedNets }: CellProps) {
    const baseStyle = "relative w-20 h-20 border border-gray-400";

    const setDirectionStyle = (direction: Direction) => {
        if (direction === "opposite") {
            return "inset-0";
        } else if (direction === "horizontal") {
            return "inset-y-3 inset-x-0";
        } else if (direction === "vertical") {
            return "inset-y-0 inset-x-3";
        }
    };

    return (
        <div className={baseStyle}>
            {selectedNets.length > 0 &&
                (cell.source === "" && cell.target === "" ? (
                    <>
                        {/* M1 Layer */}
                        {cell?.M1 && selectedNets.includes(cell.M1.net) && (
                            <div
                                className={`tooltip absolute ${setDirectionStyle(
                                    cell.M1.direction
                                )} bg-blue-200 bg-m1-pattern`}
                                data-tip={`M1: ${cell.M1.net}`}
                            ></div>
                        )}
                        {/* M2 Layer */}
                        {cell?.M2 && selectedNets.includes(cell.M2.net) && (
                            <div
                                className={`tooltip absolute ${setDirectionStyle(
                                    cell.M2.direction
                                )} bg-m2-pattern z-20`}
                                data-tip={`M2: ${cell.M2.net}`}
                            ></div>
                        )}
                        {/* Via */}
                        {cell.via && selectedNets.includes(cell.via) && (
                            <div
                                className="tooltip absolute inset-0 via-pattern z-10 opacity-70"
                                data-tip={`Via: ${cell.via}`}
                            ></div>
                        )}
                    </>
                ) : (
                    <>
                        {/* Source */}
                        {cell.source !== "" &&
                            selectedNets.includes(cell.source) && (
                                <div
                                    className="tooltip absolute inset-0 source-pattern justify-center items-center flex z-30"
                                    data-tip={`Source: ${cell.source}`}
                                >
                                    <span className="text-2xl font-extrabold">
                                        S
                                    </span>
                                </div>
                            )}
                        {/* Target */}
                        {cell.target !== "" &&
                            selectedNets.includes(cell.target) && (
                                <div
                                    className="tooltip absolute inset-0 target-pattern justify-center items-center flex z-30"
                                    data-tip={`Target: ${cell.target}`}
                                >
                                    <span className="text-2xl font-extrabold">
                                        T
                                    </span>
                                </div>
                            )}
                    </>
                ))}
        </div>
    );
}
