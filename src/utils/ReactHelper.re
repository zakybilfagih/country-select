module Spread = {
  [@react.component]
  let make = (~props, ~children) => {
    React.cloneElement(children, props);
  };
};

let useIsClient = () => {
  let (isClient, setIsClient) = React.useState(_ => false);

  React.useEffect0(() => {
    setIsClient(Fun.const(true));
    None;
  });

  isClient;
};

let useClientValue = (~fallback, f) => {
  let isClient = useIsClient();
  isClient ? f() : fallback;
};
