import { defineConfig } from "vite";
import { nodeResolve } from "@rollup/plugin-node-resolve";

export default defineConfig({
  base: "/country-select/",
  plugins: [nodeResolve()],
  build: {
    rollupOptions: {
      external: ["@emotion/server", "react-dom/server"],
    },
  },
  ssgOptions: {
    entry: "./output/bin/index.js",
    crittersOptions: {},
  },
});
