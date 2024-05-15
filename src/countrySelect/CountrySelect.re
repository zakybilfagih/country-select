[%%mel.raw {|import "flag-icons/css/flag-icons.min.css"|}];

module Button = {
  module Style = {
    let container = [%cx
      {|
        display: flex;
        align-items: center;
        gap: 8px;
    |}
    ];

    let flag = [%cx {|
      width: 14px !important;
    |}];
  };

  [@react.component]
  let make = (~country) => {
    <div className=Style.container>
      {switch (country) {
       | Some(country) =>
         <>
           <span
             className={Utils.Cn.make([|
               Country.toFlagClassName(country),
               Style.flag,
             |])}
           />
           {React.string(Country.name(country))}
         </>
       | None => React.string("Country")
       }}
    </div>;
  };
};

module Item = {
  module Style = {
    let container = [%cx
      {|
      display: flex;
      align-items: center;
      gap: 8px;
    |}
    ];

    let flag = [%cx {|
      width: 14px !important;
    |}];
  };

  [@react.component]
  let make = (~country) => {
    <>
      <span
        className={Utils.Cn.make([|
          Country.toFlagClassName(country),
          Style.flag,
        |])}
      />
      {React.string(Country.name(country))}
    </>;
  };
};

let delayPromise = (delay, promise) => {
  promise
  |> Js.Promise.then_(promiseValue => {
       Js.Promise.make((~resolve, ~reject as _) =>
         Js.Global.setTimeout(~f=() => {resolve(. promiseValue)}, delay)
         |> ignore
       )
     });
};

[@react.component]
let make =
    (
      ~country,
      ~onChange,
      ~buttonClassName=?,
      ~inputClassName=?,
      ~inputContainerClassName=?,
      ~optionClassName=?,
      ~delay=None,
    ) => {
  let countriesQuery =
    ReactQuery.Query.(
      useQueryWithSelect(
        queryOptions(
          ~enabled=Option.is_some(delay),
          ~queryKey=ReactQuery.Utils.queryKey1("countries"),
          ~queryFn=
            _ => delayPromise(Option.get(delay), CountriesAPI.getAll()),
          ~select=
            Js.Array.map(~f=(country: CountriesAPI.t) =>
              Country.make(~name=country.label, ~code=country.value)
            ),
          ~staleTime=ReactQuery.Utils.time(`infinity),
          (),
        ),
      )
    );

  <Components.Combobox
    options={Option.value(countriesQuery.data, ~default=[||])}
    isLoading={countriesQuery.isLoading}
    selectedOption=country
    onSelect=onChange
    getOptionLabel=Country.name
    optionEqual=Country.equal
    buttonAriaLabel="Choose country"
    noResultText="No country found."
    renderButtonLabel={country => <Button country />}
    renderOption={country => <Item country />}
    optionClassName={Utils.Cn.make([|
      Item.Style.container,
      Option.value(optionClassName, ~default=""),
    |])}
    ?buttonClassName
    ?inputClassName
    ?inputContainerClassName
  />;
};
