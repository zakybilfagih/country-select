open Bindings.FloatingUI;

type t = {
  getReferenceProps: (~props: ReactDOM.domProps=?, unit) => ReactDOM.domProps,
  getFloatingProps: (~props: ReactDOM.domProps=?, unit) => ReactDOM.domProps,
};

let use = context => {
  let interaction =
    UseInteraction.use([|
      useRole(context, ()),
      useClick(context),
      useDismiss(context),
    |]);

  {
    getReferenceProps: UseInteraction.getReferenceProps(interaction),
    getFloatingProps: UseInteraction.getFloatingProps(interaction),
  };
};
