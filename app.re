module Colors = {
  let primary = `hex("333333");
  let selected = `hex("FFDBB3");
  let hover = `rgba((46, 60, 86, `percent(7.)));
};

let colorPrimary = [%cx {| color: $(Colors.primary) |}];
let backgroundSelected = [%cx {| background-color: $(Colors.selected) |}];

let textMd = [%cx
  {|
  font-family: Arial;
  font-size: 14px;
  font-weight: 400;
  line-height: 18px;
|}
];

let paddingBlockSm = [%cx {|
  padding-block: 4px;
|}];

let menu = [%cx {|
  padding-inline: 0px;
  overflow: auto;
  margin: 0;
|}];

let comboboxContainer = [%cx
  {|
  display: none;
  position: absolute;
  left: 0;
  top: 28px;
  box-shadow: 0px 3px 18px 0px #00000026;
  background-color: white;
  border: 1px solid rgba(0, 0, 0, 0.1);
  border-radius: 3px;
  max-width: max-content;
  max-height: 450px;
  flex-direction: column;
|}
];

let displayBlock = [%cx "display: block"];
let displayFlex = [%cx "display: flex"];

let menuItem = [%cx
  {|
  list-style: none;
  padding-block: 4px;
  padding-inline: 10px;

  &:hover {
    background-color: $(Colors.hover);
  }
|}
];

[@react.component]
let make = () => {
  let (countries, setCountries) = React.useState(_ => [||]);

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

  <main className="px-20">
    <div style={ReactDOM.Style.make(~height="1000px", ())} />
    <Combobox
      options=countries
      selectLabel={(country: CountriesApi.t) => country.label}
      onSelect={country => Js.log(country)}
      itemEqual={(a: CountriesApi.t, b: CountriesApi.t) => a.value == b.value}
      renderItem={(country: CountriesApi.t) => {
        let countryCode = country.value;
        <>
          <span className={j|fi fi-$(countryCode)|j} />
          {React.string(country.label)}
        </>;
      }}
      renderButton={_ => React.string("Click me")}
    />
    <div style={ReactDOM.Style.make(~height="1000px", ())} />
  </main>;
};
