module Intl = {
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
};

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

    let info = [%cx
      {|
      flex-grow: 1;
      text-align: right;
      color: $(Styling.Colors.Light.textSecondary);
    |}
    ];
  };

  let formatter =
    Intl.NumberFormat.make(
      ~locales="en",
      ~options=Intl.NumberFormat.makeOptions(~notation=`compact, ()),
      (),
    );
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
      <span
        className={Utils.Cn.make([|Style.info, Styling.Typography.textXs|])}>
        {React.string(
           formatter |> Intl.NumberFormat.formatInt(Country.info(country)),
         )}
      </span>
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
            data => {
              Random.init(1337);
              Js.Array.map(
                ~f=
                  (country: CountriesAPI.t) => {
                    Country.make(
                      ~name=country.label,
                      ~code=country.value,
                      ~info=Random.int(300_000),
                    )
                  },
                data,
              );
            },
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
    additionalContentWidth={40 + 8}
    ?buttonClassName
    ?inputClassName
    ?inputContainerClassName
  />;
};
