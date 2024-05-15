module Style = {
  let item = [%cx
    {|
      position: absolute;
      top: 0;
      left: 0;
      width: 100%;
      cursor: default;
      padding: 4px 10px;
      color: $(Colors.Light.textPrimary);
    |}
  ];

  let active = [%cx
    {|
      background-color: $(Colors.Light.backgroundControlHover);
    |}
  ];

  let selected = [%cx
    {|
      background-color: $(Colors.Light.backgroundSelected);
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
      className={Cn.make([|
        Style.item,
        StyleHelper.textMd,
        selected ? Style.selected : active ? Style.active : "",
        Option.value(className, ~default=""),
      |])}>
      children
    </li>
  });
