module Style = {
  let container = [%cx
    {|
    display: flex;
    gap: 5px;
    align-items: center;
    padding: 4px 10px;
    color: $(Colors.Light.textPrimary);
    background-color: $(Colors.Light.backgroundBox);
    border: 1px solid $(Colors.Light.borderControlAlpha);
    border-radius: 3px;
  |}
  ];

  let chevron = [%cx {|
    color: $(Colors.Light.textPrimary);
  |}];
};
[@react.component]
let make =
  React.forwardRef(
    (
      ~id=?,
      ~ariaLabel=?,
      ~className=?,
      ~ariaExpanded=?,
      ~ariaHaspopup=?,
      ~ariaControls=?,
      ~onPointerDown=?,
      ~onMouseDown=?,
      ~onClick=?,
      ~onKeyUp=?,
      ~onKeyDown=?,
      ~children,
      ref,
    ) => {
    <button
      ?id
      ?ariaLabel
      ?ariaExpanded
      ?ariaHaspopup
      ?ariaControls
      ?onPointerDown
      ?onMouseDown
      ?onClick
      ?onKeyUp
      ?onKeyDown
      type_="button"
      ref=?{ref |> Js.Nullable.toOption |> Option.map(ReactDOM.Ref.domRef)}
      className={Cn.make([|
        Style.container,
        StyleHelper.textMd,
        Option.value(className, ~default=""),
      |])}>
      children
      <Icon.Triangle className=Style.chevron />
    </button>
  });
