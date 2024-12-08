import Grid from "@/components/grid";
import Legend from "@/components/legend";
import PopupMessageEmitter from "@/components/popup-message/popup-message-emitter";
import {
    fillGridData,
    getNets,
    readGridDimensions,
    readRoutes,
} from "@/lib/utils";

export default async function Home() {
    const routes = await readRoutes();
    const gridDimensions = await readGridDimensions();
    const grid = fillGridData(routes, gridDimensions.rows, gridDimensions.cols);
    const nets = getNets(routes);

    return (
        <>
            <PopupMessageEmitter routes={routes} />
            <div className="flex flex-col h-full gap-3">
                <div className="flex items-center justify-center flex-1 overflow-hidden">
                    <div className="h-full w-full overflow-auto flex justify-center items-center">
                        <Grid grid={grid} nets={nets} />
                    </div>
                </div>
                <div className="w-full">
                    <Legend />
                </div>
            </div>
        </>
    );
}
