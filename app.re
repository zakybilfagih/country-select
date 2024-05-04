let getSearchParam = param => {
  let search =
    Webapi.Dom.window
    |> Webapi.Dom.Window.location
    |> Webapi.Dom.Location.search;
  let searchParams = Webapi.Url.URLSearchParams.make(search);
  Webapi.Url.URLSearchParams.get(param, searchParams);
};

module Style = {
  let sidebar = [%cx
    {|
        position: fixed;
        top: 0;
        width: 220px;
        height: 100vh;
        box-shadow: 0px 3px 18px 0px #00000026;
        background-color: $(Colors.Light.backgroundBox);
    |}
  ];

  let form = [%cx {|
        margin: 10px;
      |}];

  let inputContainer = [%cx "margin-bottom: 10px"];

  let main = [%cx
    {|
      padding-left: 256px;
      padding-right: 36px;
    |}
  ];
};

[@react.component]
let make = () => {
  let (country, setCountry) = React.useState(() => None);

  let delay =
    Option.value(
      getSearchParam("delay")
      |> Option.map(Js.Float.fromString)
      |> Option.map(delay => Js.Float.isNaN(delay) ? 0 : int_of_float(delay)),
      ~default=0,
    );

  let selectContainer = React.useRef(Js.Nullable.null);

  React.useEffect1(
    () => {
      let scrollToSelect = () => {
        selectContainer.current
        |> Js.Nullable.toOption
        |> Option.iter(Webapi.Dom.Element.scrollIntoView);
      };

      let handle = Js.Global.setTimeout(~f=scrollToSelect, 100);
      Some(() => {Js.Global.clearTimeout(handle)});
    },
    [|selectContainer|],
  );

  <>
    <aside className=Style.sidebar>
      <form className=Style.form>
        <div className=Style.inputContainer>
          <label htmlFor="delay">
            <p
              className={Cn.make([|
                [%cx "margin-bottom: 4px;"],
                StyleHelper.textMd,
              |])}>
              {React.string("Fetch delay in milliseconds: ")}
            </p>
          </label>
          <input
            type_="number"
            id="delay"
            name="delay"
            defaultValue={delay == 0 ? "" : string_of_int(delay)}
            min="0"
            max="10000"
            className=[%cx "width: 100%;"]
          />
        </div>
        <button type_="submit"> {React.string("Submit")} </button>
      </form>
    </aside>
    <main className=Style.main>
      <div className=[%cx "height: 100vh;"] />
      <div
        className=[%cx "display: flex; margin-left: 30%; padding-block: 100px;"]
        ref={ReactDOM.Ref.domRef(selectContainer)}>
        <CountrySelect
          country
          onChange={country => setCountry(_ => country)}
          delay
        />
      </div>
      <div className=[%cx "height: 100vh;"] />
    </main>
  </>;
};
