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

let cn = classNames => {
  CssJs.merge(classNames);
};

[@react.component]
let make = () => {
  let (countries, setCountries) = React.useState(_ => [||]);
  let (filter, setFilter) = React.useState(_ => "");
  let (openList, setOpenList) = React.useState(_ => false);

  let fetchCountries = _ => {
    let _ = {
      open Promise;
      open Js.Promise.Let_syntax;
      let+ countries = CountriesApi.getAll();
      setCountries(_ => countries);
    };
    ();
  };

  let filteredCountries =
    countries
    |> Js.Array.filter(~f=(country: CountriesApi.t) => {
         Js.String.indexOf(
           ~search=Js.String.toLowerCase(filter),
           Js.String.toLowerCase(country.label),
         )
         == 0
       });
  <>
    <button onClick=fetchCountries type_="button"> {React.string("Click me")} </button>
    <button type_="button" onClick={_ => setOpenList(v => !v)}>
      {React.string("Toggle")}
    </button>
    <div className={cn([|comboboxContainer, openList ? displayFlex : ""|])}>
      <input
        type_="string"
        value=filter
        onInput={e => setFilter(React.Event.Form.target(e)##value)}
      />
      <ul className={cn([|menu, paddingBlockSm|])}>
        {filteredCountries
         |> Js.Array.map(~f=(country: CountriesApi.t) => {
              <li
                key={country.value}
                className={cn([|textMd, menuItem, colorPrimary|])}>
                {React.string(country.label)}
              </li>
            })
         |> React.array}
      </ul>
    </div>
  </>;
};
