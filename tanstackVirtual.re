[@deriving jsProperties]
type scrollToIndexOptions = {
  [@mel.optional]
  align: option(string),
  [@mel.optional]
  behaviour: option(string),
};

type virtualItem = {
  key: string,
  index: int,
  start: int,
  size: int,
  [@mel.as "end"]
  end_: int,
  lane: int,
};

module Virtualizer = {
  type t = {
    getTotalSize: unit => int,
    scrollToIndex: (. int, option(scrollToIndexOptions)) => unit,
    getVirtualItems: unit => array(virtualItem),
    measureElement: Js.nullable(Dom.element) => unit,
  };

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
  };

  [@mel.module "@tanstack/react-virtual"]
  external use: options => t = "useVirtualizer";
};
