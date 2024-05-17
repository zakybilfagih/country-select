let getSearchParam = param => {
  let search =
    Webapi.Dom.window
    |> Webapi.Dom.Window.location
    |> Webapi.Dom.Location.search;
  let searchParams = Webapi.Url.URLSearchParams.make(search);
  Webapi.Url.URLSearchParams.get(param, searchParams);
};

module Style = {
  let sidebar = (~hide) => {
    let translate = hide ? `percent(-100.) : `percent(0.);
    [%cx
     {|
        position: fixed;
        top: 0;
        width: 220px;
        height: 100vh;
        box-shadow: 0px 3px 18px 0px #00000026;
        background-color: $(Styling.Colors.Light.backgroundBox);
        transition: transform .3s cubic-bezier(.36,-0.01,0,.77);
        transform: translateX($(translate));
    |}];
  };

  let form = [%cx {|
        margin: 10px;
      |}];

  let inputContainer = [%cx "margin-bottom: 10px"];

  let main = (~hide) => {
    let padding = hide ? `px(36) : `px(256);
    [%cx
     {|
      transition: padding-left .3s cubic-bezier(.36,-0.01,0,.77);
      padding-left: $(padding);
      padding-right: 36px;
    |}];
  };
};

[@react.component]
let make = () => {
  let delay =
    Utils.ReactHelper.useClientValue(
      () =>
        getSearchParam("delay")
        |> Option.map(Js.Float.fromString)
        |> Option.map(delay =>
             Js.Float.isNaN(delay) ? 0 : int_of_float(delay)
           )
        |> Option.fold(~none=Some(0), ~some=Option.some(_)),
      ~fallback=None,
    );

  let (country, setCountry) = React.useState(() => None);
  let countryCode =
    Utils.ReactHelper.useClientValue(
      () => getSearchParam("countryCode"),
      ~fallback=None,
    );
  React.useEffect1(
    () => {
      setCountry(_ => countryCode);
      None;
    },
    [|countryCode|],
  );

  let selectContainer = React.useRef(Js.Nullable.null);

  React.useEffect1(
    () => {
      selectContainer.current
      |> Js.Nullable.toOption
      |> Option.iter(Webapi.Dom.Element.scrollIntoView);

      None;
    },
    [|selectContainer.current|],
  );

  let (hideSidebar, setHideSidebar) = React.useState(_ => false);

  <>
    <aside className={Style.sidebar(~hide=hideSidebar)}>
      <button
        type_="button"
        className=[%cx
          "position: absolute; top: 0; right: 0; transform: translateX(100%)"
        ]
        onClick={_ => setHideSidebar(hide => !hide)}>
        {React.string(hideSidebar ? "Show" : "Hide")}
      </button>
      <form className=Style.form>
        <div className=Style.inputContainer>
          <label htmlFor="delay">
            <p
              className={Utils.Cn.make([|
                [%cx "margin-bottom: 4px;"],
                Styling.Typography.textMd,
              |])}>
              {React.string("Fetch delay in milliseconds: ")}
            </p>
          </label>
          <input
            type_="number"
            id="delay"
            name="delay"
            defaultValue={Option.fold(
              ~none="",
              ~some=delay => {delay == 0 ? "" : string_of_int(delay)},
              delay,
            )}
            min="0"
            max="10000"
            className=[%cx "width: 100%;"]
          />
        </div>
        <div className=Style.inputContainer>
          <label htmlFor="countryCode">
            <p
              className={Utils.Cn.make([|
                [%cx "margin-bottom: 4px;"],
                Styling.Typography.textMd,
              |])}>
              {React.string("Initial country code")}
            </p>
          </label>
          <input
            type_="text"
            id="countryCode"
            name="countryCode"
            defaultValue={Option.value(~default="", countryCode)}
            min="0"
            max="10000"
            className=[%cx "width: 100%;"]
          />
        </div>
        <button type_="submit"> {React.string("Submit")} </button>
      </form>
    </aside>
    <main className={Style.main(~hide=hideSidebar)}>
      <div className=[%cx "height: 100vh;"] />
      <div
        className=[%cx
          "display: flex; margin-left: 30%; padding-block: 100px; flex-direction: column;"
        ]
        ref={ReactDOM.Ref.domRef(selectContainer)}>
        <CountrySelect
          country
          onChange={country => setCountry(_ => country)}
          delay
          buttonClassName={Utils.Cn.make([|[%cx "width: max-content;"]|])}
        />
        <p className={Utils.Cn.make([|Styling.Typography.textMd|])}>
          {React.string("Value: " ++ Option.value(~default="", country))}
        </p>
      </div>
      <div className=[%cx "height: 100vh;"] />
    </main>
  </>;
};
