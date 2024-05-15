module Style = {
  let noResult = [%cx
    {|
    margin: 0px;
    padding: 8px 10px;
    color: $(Colors.Light.textPrimary)
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
    className={Cn.make([|Style.noResult, StyleHelper.textMd|])}>
    children
  </p>;
};
