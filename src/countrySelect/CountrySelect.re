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
  let make = (~country, ~isLoading, ~isError) => {
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
       | None =>
         switch (isLoading, isError) {
         | (true, _) => React.string("Loading...")
         | (_, _) => React.string("Country")
         }
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
    Bindings.Intl.NumberFormat.make(
      ~locales="en",
      ~options=Bindings.Intl.NumberFormat.makeOptions(~notation=`compact, ()),
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
           formatter
           |> Bindings.Intl.NumberFormat.formatInt(Country.info(country)),
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
    getOptionLabel=Country.name
    getOptionKey=Country.code
    selectedOption=country
    onSelect=onChange
    isLoading={countriesQuery.isLoading}
    isError={countriesQuery.isError}
    renderButtonLabel={(~selectedOption, ~isLoading, ~isError) =>
      <Button country=selectedOption isLoading isError />
    }
    renderOption={country => <Item country />}
    additionalContentWidth={40 + 8}
    buttonAriaLabel="Choose country"
    noResultText="No country found."
    optionClassName={Utils.Cn.make([|
      Item.Style.container,
      Option.value(optionClassName, ~default=""),
    |])}
    ?buttonClassName
    ?inputClassName
    ?inputContainerClassName
  />;
};
