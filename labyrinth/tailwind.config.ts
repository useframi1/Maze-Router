import type { Config } from "tailwindcss";

const config: Config = {
    content: [
        "./src/pages/**/*.{js,ts,jsx,tsx,mdx}",
        "./src/components/**/*.{js,ts,jsx,tsx,mdx}",
        "./src/app/**/*.{js,ts,jsx,tsx,mdx}",
    ],
    theme: {
        extend: {
            colors: {
                background: "var(--background)",
                foreground: "var(--foreground)",
            },
            backgroundImage: {
                "m1-pattern":
                    "repeating-linear-gradient(45deg, #0000ff 1px, #0000ff 4px, transparent 6px, transparent 7px)",
                "m2-pattern":
                    "repeating-linear-gradient(0deg, transparent 0px, transparent 5px, #FFA500 5px, #FFA500 10px), repeating-linear-gradient(90deg, transparent 0px, transparent 5px, #FFA500 5px, #FFA500 10px)",
            },
            backgroundSize: {
                "m2-pattern": "20px 15px",
            },
            backgroundPosition: {
                "m2-pattern": "0 0, 5px 5px",
            },
        },
    },
    plugins: [require("daisyui")],
    daisyui: {
        darkTheme: "light",
    },
};
export default config;
