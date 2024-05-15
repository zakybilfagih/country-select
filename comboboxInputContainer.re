module Style = {
  let searchIconContainer = [%cx {|
    display: flex;
  |}];

  let container = [%cx
    {|
    display: flex;
    align-items: center;
    gap: 8px;
    padding-block: 4px;
    padding-inline: 10px;
    border-bottom: 1px solid $(Colors.Light.borderLineAlpha);
  |}
  ];
};

[@react.component]
let make = (~className=?, ~isLoading, ~children) => {
  <div
    className={Cn.make([|
      Style.container,
      Option.value(className, ~default=""),
    |])}>
    <div className=Style.searchIconContainer> <Icon.Search /> </div>
    children
    {isLoading ? <Icon.Loading /> : React.null}
  </div>;
};
