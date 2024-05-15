module SinglePage = {
  type t;

  [@mel.module "vite-react-ssg/single-page"]
  external make: React.element => t = "ViteReactSSG";

  [@deriving jsProperties]
  type makeOptions = {
    [@mel.optional]
    getStyleCollector:
      option(
        unit =>
        Js.Promise.t({
          .
          "collect": React.element => React.element,
          "toString": unit => string,
        }),
      ),
  };

  [@mel.module "vite-react-ssg/single-page"]
  external makeWithOptions:
    (React.element, ~setupFn: unit => unit=?, ~options: makeOptions=?, unit) =>
    t =
    "ViteReactSSG";
};
module Head = {
  [@react.component] [@mel.module "vite-react-ssg/single-page"]
  external make: (~children: React.element) => React.element = "Head";
};
