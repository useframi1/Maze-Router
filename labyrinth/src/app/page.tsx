import Grid from "@/components/grid";
import Legend from "@/components/legend";
import { fillGridData, readFileAndParseData } from "@/lib/utils";

export default async function Home() {
    const routes = await readFileAndParseData();
    const grid = fillGridData(routes, 10, 10);

    return (
        <div className="flex flex-col h-full gap-3">
            <div className="flex items-center justify-center flex-1 overflow-hidden">
                <div className="h-full w-full overflow-auto flex justify-center items-center">
                    <Grid grid={grid} />
                </div>
            </div>
            <div className="w-full">
                <Legend />
            </div>
        </div>
    );
}
