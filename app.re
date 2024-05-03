[%%mel.raw {|import "flag-icons/css/flag-icons.min.css"|}];
module Style = {
  let itemContainer = [%cx {|
    display: flex;
    gap: 8px;
  |}];

  let flag = [%cx {|
    width: 14px !important;
  |}];

  let buttonLabelContainer = [%cx
    {|
    display: flex;
    align-items: center;
    gap: 8px;
  |}
  ];
};

[@react.component]
let make = () => {
  let (countries, setCountries) = React.useState(_ => [||]);
  let (country, setCountry) = React.useState(_ => None);

  let fetchCountries = _ => {
    let _ = {
      open Promise.Let_syntax;
      let+ countries = CountriesApi.getAll();
      setCountries(_ => countries);
    };
    ();
  };

  React.useEffect0(() => {
    fetchCountries();
    None;
  });

  <main style={ReactDOM.Style.make(~padding="0 50px", ())}>
    <div style={ReactDOM.Style.make(~height="800px", ())} />
    <Combobox
      options=countries
      getItemLabel={(country: CountriesApi.t) => country.label}
      selected=country
      onSelect={country => setCountry(_ => country)}
      itemEqual={(a: CountriesApi.t, b: CountriesApi.t) => a.value == b.value}
      buttonAriaLabel="Choose country"
      noResultText="No country found."
      renderItem={(country: CountriesApi.t) => {
        let countryCode = country.value;
        <div className=Style.itemContainer>
          <span
            className={Cn.make([|{j|fi fi-$(countryCode)|j}, Style.flag|])}
          />
          {React.string(country.label)}
        </div>;
      }}
      renderButton={_ => {
        let label =
          Option.value(
            ~default="Country",
            Option.map((country: CountriesApi.t) => country.label, country),
          );
        let countryCode =
          Option.map((country: CountriesApi.t) => country.value, country);
        <div className=Style.buttonLabelContainer>
          {switch (countryCode) {
           | Some(countryCode) =>
             <span
               className={Cn.make([|{j|fi fi-$(countryCode)|j}, Style.flag|])}
             />
           | None => React.null
           }}
          {React.string(label)}
        </div>;
      }}
    />
    <div style={ReactDOM.Style.make(~height="1000px", ())} />
  </main>;
};
