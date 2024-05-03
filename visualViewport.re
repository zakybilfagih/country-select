type t;

[@mel.get] [@mel.return nullable]
external height: t => option(int) = "height";

[@mel.send.pipe: t]
external addResizeEventListener:
  ([@mel.as "resize"] _, Dom.event => unit) => unit =
  "addEventListener";

[@mel.send.pipe: t]
external removeResizeEventListner:
  ([@mel.as "resize"] _, Dom.event => unit) => unit =
  "removeEventListener";
