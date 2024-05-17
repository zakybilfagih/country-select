open Bindings;

[@react.component]
let make =
    (
      ~options: array('option),
      ~getOptionLabel: 'option => string,
      ~getOptionKey: 'option => 'key,
      ~optionKeyEqual: ('key, 'key) => bool=(==),
      ~selectedOption: option('key),
      ~onSelect: option('key) => unit,
      ~isLoading: bool=false,
      ~isError: bool=false,
      ~renderButtonLabel:
         (
           ~selectedOption: option('option),
           ~isLoading: bool,
           ~isError: bool
         ) =>
         React.element,
      ~renderOption: 'option => React.element,
      ~padding: int=25,
      ~maxHeight: int=400,
      ~additionalContentWidth: int=0,
      ~buttonAriaLabel: string="Choose item",
      ~noResultText: string="No item found.",
      ~buttonClassName: option(string)=?,
      ~inputClassName: option(string)=?,
      ~inputContainerClassName: option(string)=?,
      ~optionClassName: option(string)=?,
    ) => {
  let (_open, setOpen) = React.useState(_ => false);
  let (pointer, setPointer) = React.useState(_ => false);
  let (activeIndex, setActiveIndex) = React.useState(_ => None);

  let (computedMaxHeight, setComputedMaxHeight) =
    React.useState(_ => maxHeight);

  let buttonId = React.useId();

  if (!_open && pointer) {
    setPointer(_ => false);
  };

  let {refs, context, floatingStyles, isPositioned}: FloatingUI.useFloatingReturn =
    FloatingUI.(
      useFloating(
        useFloatingOptions(
          ~_open,
          ~placement=`bottomStart,
          ~whileElementsMounted=FloatingUI.autoUpdate,
          ~onOpenChange=
            value => {
              setOpen(_ => value);
              if (!value) {
                setActiveIndex(_ => None);
              };
            },
          ~middleware=
            Middleware.(
              [|
                offset(3),
                size(
                  ~options=
                    sizeOptions(
                      ~padding,
                      ~apply=
                        ({availableHeight, _}) => {
                          let maxHeight =
                            Js.Math.max_int(
                              150,
                              Js.Math.min_int(availableHeight, maxHeight),
                            );
                          ReactDOM.flushSync(() => {
                            setComputedMaxHeight(_ => maxHeight)
                          });
                        },
                      (),
                    ),
                  (),
                ),
                flip(
                  ~options=
                    flipOptions(
                      ~padding,
                      ~fallbackStrategy=`initialPlacement,
                      (),
                    ),
                  (),
                ),
              |]
            ),
          (),
        ),
      )
    );

  let {getReferenceProps, getFloatingProps}: Hooks.UseButtonInteraction.t =
    Hooks.UseButtonInteraction.use(context);

  let selectedOptionValue =
    Js.Array.find(
      ~f=
        option =>
          Option.fold(
            ~none=false,
            ~some=
              selectedOption =>
                optionKeyEqual(getOptionKey(option), selectedOption),
            selectedOption,
          ),
      options,
    );

  <>
    <Utils.ReactHelper.Spread props={getReferenceProps()}>
      <ComboboxButton
        id=buttonId
        ariaLabel=buttonAriaLabel
        className=?buttonClassName
        ref={ReactDOM.Ref.callbackDomRef(refs.setReference)}>
        {renderButtonLabel(
           ~selectedOption=selectedOptionValue,
           ~isLoading,
           ~isError,
         )}
      </ComboboxButton>
    </Utils.ReactHelper.Spread>
    {_open
       ? <ComboboxDialog
           refs
           context
           floatingStyles
           isPositioned
           getFloatingProps
           activeIndex
           setActiveIndex
           setOpen
           pointer
           setPointer
           computedMaxHeight
           buttonId
           options
           optionKeyEqual
           additionalContentWidth
           noResultText
           onSelect
           isLoading
           renderOption
           getOptionLabel
           getOptionKey
           selectedOption
           ?inputClassName
           ?inputContainerClassName
           ?optionClassName
         />
       : React.null}
  </>;
};
