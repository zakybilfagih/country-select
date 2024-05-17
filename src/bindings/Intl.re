module NumberFormat = {
  type t;

  type makeOptions;
  [@mel.obj]
  external makeOptions:
    (
      ~notation: [ | `standard | `scientific | `engineering | `compact]=?,
      unit
    ) =>
    makeOptions;

  [@mel.scope "Intl"]
  external make: (~locales: string=?, ~options: makeOptions=?, unit) => t =
    "NumberFormat";

  [@mel.send.pipe: t] external format: float => string = "format";
  [@mel.send.pipe: t] external formatInt: int => string = "format";
};
