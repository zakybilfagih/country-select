module Middleware = {
  type t;

  [@mel.module "@floating-ui/react"] external shift: unit => t = "shift";

  [@mel.module "@floating-ui/react"] external offset: int => t = "offset";

  [@deriving jsProperties]
  type flipOptions = {
    [@mel.optional]
    padding: option(int),
    [@mel.optional]
    fallbackStrategy: option(string),
    [@mel.optional]
    crossAxis: option(bool),
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
  placement: option(string),
  [@mel.optional]
  whileElementsMounted: option(whileElementsMountedFn),
  [@mel.optional] [@mel.as "open"]
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

type floatingContext;

type useFloatingReturn = {
  floatingStyles: ReactDOM.Style.t,
  refs: useFloatingRefs,
  context: floatingContext,
  isPositioned: bool,
};
[@mel.module "@floating-ui/react"]
external useFloating: useFloatingOptions => useFloatingReturn = "useFloating";

type elementProps;
[@deriving jsProperties]
type useRoleOptions = {
  [@mel.optional]
  enabled: option(bool),
  [@mel.optional]
  role: option(string),
};
[@mel.module "@floating-ui/react"]
external useRole:
  (floatingContext, ~props: useRoleOptions=?, unit) => elementProps =
  "useRole";

[@deriving jsProperties]
type useFocusOptions = {
  [@mel.optional]
  enabled: option(bool),
  [@mel.optional]
  visibleOnly: option(bool),
};
[@mel.module "@floating-ui/react"]
external useFocus:
  (floatingContext, ~props: useFocusOptions=?, unit) => elementProps =
  "useFocus";

[@mel.module "@floating-ui/react"]
external useDismiss: floatingContext => elementProps = "useDismiss";

[@mel.module "@floating-ui/react"]
external useClick: floatingContext => elementProps = "useClick";

[@deriving jsProperties]
type scrollItemIntoViewOptions = {
  [@mel.optional]
  block: option(string),
  [@mel.optional]
  inline: option(string),
  [@mel.optional]
  behaviour: option(string),
};

[@deriving jsProperties]
type useListNavigationOptions = {
  listRef: React.ref(array(Js.nullable(Dom.element))),
  activeIndex: Js.nullable(int),
  [@mel.optional]
  selectedIndex: option(Js.nullable(int)),
  [@mel.optional]
  onNavigate: option(Js.nullable(int) => unit),
  [@mel.optional] [@mel.as "virtual"]
  virtual_: option(bool),
  [@mel.optional]
  loop: option(bool),
  [@mel.optional]
  allowEscape: option(bool),
  [@mel.optional]
  focusItemOnOpen: option(bool),
  [@mel.optional]
  disabledIndices: option(array(int)),
  [@mel.optional]
  scrollItemIntoView: option(scrollItemIntoViewOptions),
};
[@mel.module "@floating-ui/react"]
external useListNavigation:
  (floatingContext, ~props: useListNavigationOptions=?, unit) => elementProps =
  "useListNavigation";

module UseInteraction = {
  type t;

  [@mel.send]
  external getReferenceProps:
    (t, ~props: ReactDOM.domProps=?, unit) => ReactDOM.domProps =
    "getReferenceProps";

  [@mel.send]
  external getFloatingProps:
    (t, ~props: ReactDOM.domProps=?, unit) => ReactDOM.domProps =
    "getFloatingProps";

  [@mel.send]
  external getItemProps:
    (t, ~props: ReactDOM.domProps=?, unit) => ReactDOM.domProps =
    "getItemProps";

  [@mel.module "@floating-ui/react"]
  external use: array(elementProps) => t = "useInteractions";
};

module FloatingPortal = {
  [@react.component] [@mel.module "@floating-ui/react"]
  external make: (~children: React.element) => React.element =
    "FloatingPortal";
};
module FloatingFocusManager = {
  [@react.component] [@mel.module "@floating-ui/react"]
  external make:
    (
      ~context: floatingContext,
      ~initialFocus: int=?,
      ~visuallyHiddenDismiss: bool=?,
      ~modal: bool=?,
      ~children: React.element
    ) =>
    React.element =
    "FloatingFocusManager";
};
