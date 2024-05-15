open Bindings.FloatingUI;

type t = {
  getInputProps: (~props: ReactDOM.domProps=?, unit) => ReactDOM.domProps,
  getListFloatingProps:
    (~props: ReactDOM.domProps=?, unit) => ReactDOM.domProps,
  getItemProps: (~props: ReactDOM.domProps=?, unit) => ReactDOM.domProps,
};

let use = (~listRef, ~selectedIndex, ~activeIndex, ~onNavigate=?, context) => {
  let interaction =
    UseInteraction.use([|
      useListNavigation(
        context,
        ~props=
          useListNavigationOptions(
            ~listRef,
            ~selectedIndex=Js.Nullable.fromOption(selectedIndex),
            ~activeIndex=Js.Nullable.fromOption(activeIndex),
            ~virtual_=true,
            ~onNavigate?,
            ~focusItemOnOpen=true,
            ~disabledIndices=[||],
            ~loop=true,
            ~allowEscape=false,
            (),
          ),
        (),
      ),
    |]);

  {
    getInputProps: UseInteraction.getReferenceProps(interaction),
    getListFloatingProps: UseInteraction.getFloatingProps(interaction),
    getItemProps: UseInteraction.getItemProps(interaction),
  };
};
