[@deriving accessors]
type t = {
  name: string,
  code: string,
  info: int,
};

let make = (~name, ~code, ~info) => {name, code, info};

let equal = (t1, t2) => String.equal(t1.code, t2.code);

let toFlagClassName = ({code, _}) => {j|fi fi-$(code)|j};
