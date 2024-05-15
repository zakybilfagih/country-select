module Style = {
  let noResult = [%cx
    {|
    margin: 0px;
    padding: 8px 10px;
    color: $(Styling.Colors.Light.textPrimary)
  |}
  ];
};

[@react.component]
let make = (~children, ~id=?, ~role=?, ~ariaAtomic=?, ~ariaLive=?) => {
  <p
    ?id
    ?role
    ?ariaAtomic
    ?ariaLive
    className={Utils.Cn.make([|Style.noResult, Styling.Typography.textMd|])}>
    children
  </p>;
};
