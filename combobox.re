open ReactHelper;
open Bindings;

[@react.component]
let make =
    (
      ~padding: int=25,
      ~renderButtonLabel: option('option) => React.element,
      ~maxHeight: int=400,
      ~buttonAriaLabel: string="Choose item",
      ~buttonClassName: option(string)=?,
      ~selectedOption: option('option),
      ~options: array('option),
      ~getOptionLabel: 'option => string,
      ~optionEqual: ('option, 'option) => bool,
      ~additionalContentWidth: int=0,
      ~noResultText: string="No item found.",
      ~onSelect: option('option) => unit,
      ~renderOption: 'option => React.element,
      ~isLoading: bool=false,
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

  let {refs, context, floatingStyles, isPositioned}: FloatingUi.useFloatingReturn =
    FloatingUi.(
      useFloating(
        useFloatingOptions(
          ~_open,
          ~placement=`bottomStart,
          ~whileElementsMounted=FloatingUi.autoUpdate,
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

  let {getReferenceProps, getFloatingProps}: UseButtonInteraction.t =
    UseButtonInteraction.use(context);

  <>
    <Spread props={getReferenceProps()}>
      <ComboboxButton
        id=buttonId
        ariaLabel=buttonAriaLabel
        className=?buttonClassName
        ref={ReactDOM.Ref.callbackDomRef(refs.setReference)}>
        {renderButtonLabel(selectedOption)}
      </ComboboxButton>
    </Spread>
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
           optionEqual
           additionalContentWidth
           noResultText
           onSelect
           isLoading
           renderOption
           getOptionLabel
           selectedOption
           ?inputClassName
           ?inputContainerClassName
           ?optionClassName
         />
       : React.null}
  </>;
};
