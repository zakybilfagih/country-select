module Virtualizer = {
  type scrollOptions;
  [@mel.obj]
  external scrollOptions:
    (
      ~align: [ | `start | `center | [@mel.as "end"] `end_ | `auto]=?,
      ~behaviour: [ | `auto | `scroll]=?,
      unit
    ) =>
    scrollOptions;

  module VirtualItem = {
    [@deriving accessors]
    type t = {
      key: string,
      index: int,
      start: int,
      size: int,
      [@mel.as "end"]
      end_: int,
      lane: int,
    };
  };

  type t = {measureElement: Js.nullable(Dom.element) => unit};

  [@mel.send] external getTotalSize: t => int = "getTotalSize";
  [@mel.send]
  external getVirtualItems: t => array(VirtualItem.t) = "getVirtualItems";
  [@mel.send.pipe: t] external scrollToIndex: int => unit = "scrollToIndex";
  [@mel.send.pipe: t]
  external scrollToIndexWithOptions: (int, scrollOptions) => unit =
    "scrollToIndex";

  [@deriving accessors]
  type range = {
    startIndex: int,
    endIndex: int,
  };
  [@mel.get] [@mel.return nullable]
  external range: t => option(range) = "range";

  [@deriving jsProperties]
  type options = {
    count: int,
    estimateSize: int => int,
    [@mel.optional]
    overscan: option(int),
    [@mel.optional]
    getScrollElement: option(unit => Js.nullable(Dom.element)),
    [@mel.optional]
    paddingStart: option(int),
    [@mel.optional]
    paddingEnd: option(int),
    [@mel.optional]
    scrollPaddingStart: option(int),
    [@mel.optional]
    scrollPaddingEnd: option(int),
  };

  [@mel.module "@tanstack/react-virtual"]
  external use: options => t = "useVirtualizer";
};
