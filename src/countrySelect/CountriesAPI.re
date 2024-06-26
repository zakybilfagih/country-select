open Ppx_deriving_json_runtime.Primitives;

[@deriving json]
type t = {
  label: string,
  value: string,
};

[@deriving json]
type countries = array(t);

let apiUrl = "https://gist.githubusercontent.com/rusty-key/659db3f4566df459bd59c8a53dc9f71f/raw/4127f9550ef063121c564025f6d27dceeb279623/counties.json";

let getAll = () => {
  open Utils.Promise.Let_syntax;
  let+ data = Utils.FetchJSON.fetch(apiUrl);
  countries_of_json(data)
};
