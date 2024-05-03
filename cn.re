let make = cns =>
  cns |> Js.Array.filter(~f=v => v != "") |> Js.Array.join(~sep=" ");
