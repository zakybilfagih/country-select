module Style = {
  let input = [%cx
    {|
    border: none;
    padding: 8px;
    outline: none;
    padding: 4px 0;
    width: 100%;
    color: black;

    &::placeholder {
      color: $(Colors.Light.textTertiary);
    }
  |}
  ];
};

[@react.component]
let make =
    (
      ~value=?,
      ~className=?,
      ~onChange=?,
      ~onKeyDown=?,
      ~onClick=?,
      ~onFocus=?,
      ~onMouseDown=?,
      ~onPointerDown=?,
      ~ariaControls=?,
    ) => {
  <input
    ?onChange
    ?onKeyDown
    ?onClick
    ?onFocus
    ?onMouseDown
    ?onPointerDown
    ?value
    ?ariaControls
    className={Cn.make([|
      Style.input,
      StyleHelper.textMd,
      Option.value(className, ~default=""),
    |])}
    type_="text"
    placeholder="Search"
    ariaAutocomplete="list"
    ariaExpanded=true
    role="combobox"
  />;
};
