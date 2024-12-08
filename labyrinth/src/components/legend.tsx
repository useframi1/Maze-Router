export default function Legend() {
    return (
        <div className="flex items-center justify-center gap-20">
            <div className="flex items-center gap-2">
                <span className="w-5 h-5 bg-blue-200 bg-m1-pattern"></span>
                <span>M1</span>
            </div>
            <div className="flex items-center gap-2">
                <span className="w-5 h-5 bg-m2-pattern bg-white"></span>
                <span>M2</span>
            </div>
            <div className="flex items-center gap-2">
                <span className="w-5 h-5 via-pattern bg-white"></span>
                <span>VIA12</span>
            </div>
            <div className="flex items-center gap-2">
                <span className="w-5 h-5 source-pattern bg-white"></span>
                <span>Source</span>
            </div>
            <div className="flex items-center gap-2">
                <span className="w-5 h-5 target-pattern bg-white"></span>
                <span>Target</span>
            </div>
        </div>
    );
}
