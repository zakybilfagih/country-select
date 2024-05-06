type ssgCreateRoot;

[@mel.module "vite-react-ssg/single-page"]
external makeSinglePage: React.element => ssgCreateRoot = "ViteReactSSG";

[@deriving jsProperties]
type makeSinglePageOptions = {
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
external makeSinglePageWithOptions:
  (
    React.element,
    ~setupFn: unit => unit,
    ~options: makeSinglePageOptions=?,
    unit
  ) =>
  ssgCreateRoot =
  "ViteReactSSG";

module Head = {
  [@react.component] [@mel.module "vite-react-ssg/single-page"]
  external make: (~children: React.element) => React.element = "Head";
};
