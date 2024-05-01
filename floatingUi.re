module Middleware = {
  type t;

  [@mel.module "@floating-ui/react"] external offset: int => t = "offset";

  [@deriving jsProperties]
  type flipOptions = {
    [@mel.optional]
    padding: option(int),
    fallbackStrategy: string,
  };

  [@mel.module "@floating-ui/react"] external flip: flipOptions => t = "flip";

  type elements = {
    reference: React.element,
    floating: Dom.htmlElement,
  };

  type coords = {
    x: int,
    y: int,
    width: int,
    height: int,
  };

  type rects = {
    reference: coords,
    floating: coords,
  };

  type sizeOptionsApplyParam = {
    availableHeight: int,
    placement: string,
    elements,
    rects,
  };

  type sizeOptionsApply = sizeOptionsApplyParam => unit;

  [@deriving jsProperties]
  type sizeOptions = {
    [@mel.optional]
    apply: option(sizeOptionsApply),
    [@mel.optional]
    padding: option(int),
  };
  [@mel.module "@floating-ui/react"] external size: sizeOptions => t = "size";
};

type whileElementsMountedFn;

[@mel.module "@floating-ui/react"]
external autoUpdate: whileElementsMountedFn = "autoUpdate";

[@deriving jsProperties]
type useFloatingOptions = {
  [@mel.optional]
  whileElementsMounted: option(whileElementsMountedFn),
  [@mel.optional]
  open_: option(bool),
  [@mel.optional]
  middleware: option(array(Middleware.t)),
  [@mel.optional]
  onOpenChange: option(bool => unit),
};

type useFloatingRefs = {
  setReference: Js.nullable(Dom.element) => unit,
  setFloating: Js.nullable(Dom.element) => unit,
};

type useFloatingContext;

type useFloatingReturn = {
  floatingStyles: ReactDOM.style,
  refs: useFloatingRefs,
  context: useFloatingContext,
};
[@mel.module "@floating-ui/react"]
external useFloating: useFloatingOptions => useFloatingReturn = "useFloating";
