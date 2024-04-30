include Promise;

exception Invalid_json(string);

let fetchJson = url => {
  open Js.Promise.Let_syntax;
  let* res = Fetch.fetch(url);

  if (!Fetch.Response.ok(res)) {
    let status = Fetch.Response.status(res) |> string_of_int;
    let statusText = Fetch.Response.statusText(res);
    Js.Exn.raiseError(
      {j|"$status"|j} ++ (statusText == "" ? "" : {j|($statusText)|j}),
    );
  };

  Fetch.Response.json(res)
  |> Js.Promise.catch(err => {
       Js.Promise.reject(
         Invalid_json(
           Option.value(~default="", Js.Exn.message(Obj.magic(err))),
         ),
       )
     });
};
