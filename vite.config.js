import { defineConfig } from "vite";
import { nodeResolve } from "@rollup/plugin-node-resolve";

export default defineConfig({
  plugins: [nodeResolve()],
  build: {
    rollupOptions: {
      external: ["@emotion/server", "react-dom/server"],
    },
  },
  ssgOptions: {
    entry: "index.js",
    crittersOptions: {},
  },
});
