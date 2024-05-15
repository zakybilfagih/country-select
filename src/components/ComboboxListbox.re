module Style = {
  let listbox = [%cx
    {|
    width: 100%;
    position: relative;
    margin: 0px;
    padding: 0px;
    list-style: none;
  |}
  ];
};

[@react.component]
let make =
    (~children, ~id=?, ~role=?, ~style=?, ~onKeyDown=?, ~onPointerMove=?) => {
  <ul className=Style.listbox ?id ?role ?style ?onKeyDown ?onPointerMove>
    children
  </ul>;
};
