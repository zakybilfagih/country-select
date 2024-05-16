module Style = {
  let item = [%cx
    {|
      position: absolute;
      top: 0;
      left: 0;
      width: 100%;
      cursor: default;
      padding: 4px 10px;
      color: $(Styling.Colors.Light.textPrimary);
    |}
  ];

  let active = [%cx
    {|
      background-color: $(Styling.Colors.Light.backgroundControlHover);
    |}
  ];

  let selected = [%cx
    {|
      background-color: $(Styling.Colors.Light.backgroundSelected);
    |}
  ];
};

[@react.component]
let make =
  React.forwardRef(
    (
      ~children,
      ~active,
      ~selected,
      ~index: int,
      ~id=?,
      ~className=?,
      ~onClick=?,
      ~onFocus=?,
      ~onMouseMove=?,
      ~onPointerLeave=?,
      ~role=?,
      ~style=?,
      ~ariaSetsize=?,
      ~ariaPosinset=?,
      ref,
    ) => {
    <Utils.ReactHelper.Spread props={"data-index": index}>
      <li
        ?id
        ?onClick
        ?onFocus
        ?onMouseMove
        ?onPointerLeave
        ?role
        ?style
        ?ariaSetsize
        ?ariaPosinset
        ariaSelected=selected
        ref=?{ref |> Js.Nullable.toOption |> Option.map(ReactDOM.Ref.domRef)}
        className={Utils.Cn.make([|
          Style.item,
          Styling.Typography.textMd,
          selected ? Style.selected : active ? Style.active : "",
          Option.value(className, ~default=""),
        |])}>
        children
      </li>
    </Utils.ReactHelper.Spread>
  });
