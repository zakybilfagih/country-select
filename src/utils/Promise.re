module Let_syntax = {
  let ( let* ) = (e, f) => Js.Promise.then_(f, e);
  let (let+) = (x, f) =>
    Js.Promise.then_(t => Js.Promise.resolve(f(t)), x);
  let return = Js.Promise.resolve;
};
