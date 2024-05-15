module Style = {
  let container = [%cx
    {|
    display: flex;
    gap: 5px;
    align-items: center;
    padding: 4px 10px;
    color: $(Styling.Colors.Light.textPrimary);
    background-color: $(Styling.Colors.Light.backgroundBox);
    border: 1px solid $(Styling.Colors.Light.borderControlAlpha);
    border-radius: 3px;
  |}
  ];

  let chevron = [%cx {|
    color: $(Styling.Colors.Light.textPrimary);
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
      className={Utils.Cn.make([|
        Style.container,
        Styling.Typography.textMd,
        Option.value(className, ~default=""),
      |])}>
      children
      <ComboboxIcon.Triangle className=Style.chevron />
    </button>
  });
