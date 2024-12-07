import { CellData, Direction, MetalLayerData } from "@/lib/types";

interface CellProps {
    cell: CellData; // A single cell's data
    renderData: boolean;
}

export default function Cell({ cell, renderData }: CellProps) {
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
            {renderData &&
                (!cell.isSource && !cell.isTarget ? (
                    <>
                        {/* M1 Layer */}
                        {cell?.M1 && (
                            <div
                                className={`tooltip absolute ${setDirectionStyle(
                                    cell.M1
                                )} bg-blue-200 bg-m1-pattern`}
                                data-tip={`M1: ${cell.net}`}
                            ></div>
                        )}
                        {/* M2 Layer */}
                        {cell?.M2 && (
                            <div
                                className={`tooltip absolute ${setDirectionStyle(
                                    cell.M2
                                )} bg-m2-pattern bg-white z-20`}
                                data-tip={`M2: ${cell.net}`}
                            ></div>
                        )}
                        {/* Via */}
                        {cell.via && (
                            <div
                                className="tooltip absolute inset-0 via-pattern z-10 opacity-70"
                                data-tip={`Via: ${cell.net}`}
                            ></div>
                        )}
                    </>
                ) : (
                    <>
                        {/* Source */}
                        {cell.isSource && (
                            <div
                                className="tooltip absolute inset-0 source-pattern justify-center items-center flex"
                                data-tip={`Source: ${cell.net}`}
                            >
                                <span className="text-2xl font-extrabold">
                                    S
                                </span>
                            </div>
                        )}
                        {/* Target */}
                        {cell.isTarget && (
                            <div
                                className="tooltip absolute inset-0 target-pattern justify-center items-center flex"
                                data-tip={`Target: ${cell.net}`}
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
