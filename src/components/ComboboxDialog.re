open Bindings;

module FloatingContainer = [%styled.section
  {|
    display: flex;
    flex-direction: column;
    box-shadow: 0px 3px 18px 0px #00000026;
    background-color: $(Styling.Colors.Light.backgroundBox);
    border: 1px solid $(Styling.Colors.Light.borderFloatingAlpha);
    border-radius: 3px;
    overflow: hidden;
|}
];
module ScrollContainer = [%styled.div
  {|
    padding-block: 0px;
    overflow-y: auto;
    background-color: $(Styling.Colors.Light.backgroundBox);
|}
];
module Item = ComboboxItem;
module InputContainer = ComboboxInputContainer;
module Input = ComboboxInput;
module NoResult = ComboboxNoResult;
module Listbox = ComboboxListbox;

let filterOptions = (options, inputValue, getOptionLabel) => {
  options
  |> Js.Array.filter(~f=item => {
       item
       |> getOptionLabel
       |> Js.String.toLowerCase
       |> Js.String.startsWith(~prefix=Js.String.toLowerCase(inputValue))
     });
};

let getOptionsMaxWidth = (options, getOptionLabel, additionalContentWidth) => {
  options
  |> Js.Array.map(~f=getOptionLabel)
  |> Js.Array.map(~f=label =>
       Utils.TextWidth.get(~text=label, ~font="normal 14px Arial")
       + 42
       + additionalContentWidth
     )
  |> Js.Array.concat(~other=[|0|])  // handle empty array
  |> Js.Math.maxMany_int;
};

[@react.component]
let make =
    (
      ~context,
      ~isPositioned,
      ~floatingStyles,
      ~refs: FloatingUI.useFloatingRefs,
      ~getFloatingProps:
         (~props: ReactDOM.domProps=?, unit) => ReactDOM.domProps,
      ~activeIndex,
      ~setActiveIndex,
      ~selectedOption,
      ~computedMaxHeight,
      ~pointer,
      ~setPointer,
      ~setOpen,
      ~getOptionLabel,
      ~options,
      ~optionEqual,
      ~additionalContentWidth,
      ~noResultText,
      ~onSelect,
      ~renderOption,
      ~isLoading,
      ~buttonId,
      ~inputClassName=?,
      ~inputContainerClassName=?,
      ~optionClassName=?,
    ) => {
  let (inputValue, setInputValue) = React.useState(_ => "");

  let noResultId = React.useId();
  let listboxId = React.useId();

  let filteredOptions =
    React.useMemo2(
      () => filterOptions(options, inputValue, getOptionLabel),
      (inputValue, options),
    );

  let listRef =
    React.useRef(
      Array.make(Js.Array.length(filteredOptions), Js.Nullable.null),
    );

  React.useEffect1(
    () => {
      listRef.current =
        Array.make(Js.Array.length(filteredOptions), Js.Nullable.null);
      None;
    },
    [|filteredOptions|],
  );

  let selectedIndex =
    React.useMemo2(
      () =>
        Option.bind(selectedOption, selected =>
          switch (
            Js.Array.findIndex(
              ~f=item => {optionEqual(selected, item)},
              filteredOptions,
            )
          ) {
          | (-1) => None
          | index => Some(index)
          }
        ),
      (selectedOption, filteredOptions),
    );

  let {getInputProps, getListFloatingProps, getItemProps}: Hooks.UseComboboxInteraction.t =
    Hooks.UseComboboxInteraction.use(
      ~listRef,
      ~selectedIndex,
      ~activeIndex,
      ~onNavigate=
        activeIndex => setActiveIndex(_ => Js.Nullable.toOption(activeIndex)),
      context,
    );

  let scrollRef = React.useRef(Js.Nullable.null);

  let rowVirtualizer =
    TanstackVirtual.Virtualizer.use(
      TanstackVirtual.Virtualizer.options(
        ~count=Js.Array.length(filteredOptions),
        ~overscan=5,
        ~estimateSize=_ => 26,
        ~getScrollElement=() => scrollRef.current,
        ~paddingStart=4,
        ~paddingEnd=4,
        ~scrollPaddingStart=4,
        ~scrollPaddingEnd=4,
        (),
      ),
    );

  React.useLayoutEffect6(
    () => {
      if (isPositioned && !pointer) {
        if (Option.is_none(activeIndex)
            && Option.is_none(selectedIndex)
            && Js.Array.length(
                 TanstackVirtual.Virtualizer.getVirtualItems(rowVirtualizer),
               )
            > 0) {
          TanstackVirtual.Virtualizer.scrollToIndex(0, rowVirtualizer);
        };

        Option.iter(
          TanstackVirtual.Virtualizer.scrollToIndex(_, rowVirtualizer),
          activeIndex,
        );
      };
      None;
    },
    (rowVirtualizer, isPositioned, activeIndex, pointer, selectedIndex, refs),
  );

  React.useLayoutEffect3(
    () => {
      if (isPositioned && Option.is_some(selectedIndex)) {
        let selectedIndex = Option.get(selectedIndex);
        TanstackVirtual.Virtualizer.scrollToIndexWithOptions(
          selectedIndex,
          TanstackVirtual.Virtualizer.scrollOptions(~align=`start, ()),
          rowVirtualizer,
        );
        setActiveIndex(_ => Some(selectedIndex));
      };
      None;
    },
    (rowVirtualizer, isPositioned, selectedIndex),
  );

  let optionsMaxWidth =
    React.useMemo1(
      () =>
        getOptionsMaxWidth(
          filteredOptions,
          getOptionLabel,
          additionalContentWidth,
        ),
      [|filteredOptions|],
    );

  <FloatingUI.FloatingPortal>
    <FloatingUI.FloatingFocusManager context modal=false>
      <Utils.ReactHelper.Spread
        props={Js.Obj.assign(
          Obj.magic(getFloatingProps(~props=getListFloatingProps(), ())),
          {"aria-activedescendant": Js.undefined},
        )}>
        <FloatingContainer
          innerRef={ReactDOM.Ref.callbackDomRef(refs.setFloating)}
          ariaLabelledby=buttonId
          style={ReactDOM.Style.combine(
            floatingStyles,
            ReactDOM.Style.make(
              ~maxHeight=string_of_int(computedMaxHeight) ++ "px",
              (),
            ),
          )}>
          <InputContainer className=?inputContainerClassName isLoading>
            <Utils.ReactHelper.Spread
              props={getInputProps(
                ~props=
                  ReactDOM.domProps(
                    ~onChange=
                      event => {
                        let target = event |> React.Event.Form.target;
                        let value = target##value;
                        setInputValue(_ => value);
                        setActiveIndex(_ => None);
                      },
                    ~onKeyDown=
                      event => {
                        setPointer(_ => false);
                        if (React.Event.Keyboard.key(event) == "Enter"
                            && Option.is_some(activeIndex)) {
                          let activeIndex = Option.get(activeIndex);
                          onSelect(Some(filteredOptions[activeIndex]));
                          setActiveIndex(_ => None);
                          setOpen(_ => false);
                          setInputValue(_ => "");
                        };
                      },
                    (),
                  ),
                (),
              )}>
              <Input
                value=inputValue
                ariaControls={
                  Js.Array.length(filteredOptions) == 0
                    ? noResultId : listboxId
                }
                className=?inputClassName
              />
            </Utils.ReactHelper.Spread>
          </InputContainer>
          <ScrollContainer
            innerRef={ReactDOM.Ref.domRef(scrollRef)}
            style={ReactDOM.Style.make(
              ~maxHeight=string_of_int(computedMaxHeight) ++ "px",
              (),
            )}
            tabIndex=(-1)
            onKeyDown={_ => {setPointer(_ => false)}}>
            {Js.Array.length(filteredOptions) == 0
               ? <NoResult
                   id=noResultId
                   role="region"
                   ariaAtomic=true
                   ariaLive="assertive">
                   {React.string(noResultText)}
                 </NoResult>
               : <Listbox
                   id=listboxId
                   role="listbox"
                   style={ReactDOM.Style.make(
                     ~height=
                       string_of_int(
                         TanstackVirtual.Virtualizer.getTotalSize(
                           rowVirtualizer,
                         ),
                       )
                       ++ "px",
                     ~minWidth=string_of_int(optionsMaxWidth) ++ "px",
                     (),
                   )}
                   onPointerMove={_ => {setPointer(_ => true)}}>
                   {rowVirtualizer
                    |> TanstackVirtual.Virtualizer.getVirtualItems
                    |> Js.Array.map(~f=item => {
                         TanstackVirtual.Virtualizer.(
                           <Utils.ReactHelper.Spread
                             key={VirtualItem.key(item)}
                             props={getItemProps(
                               ~props=
                                 ReactDOM.domProps(
                                   ~onClick=
                                     _ => {
                                       onSelect(
                                         Some(
                                           filteredOptions[VirtualItem.index(
                                                             item,
                                                           )],
                                         ),
                                       );

                                       setOpen(_ => false);
                                       setActiveIndex(_ => None);
                                       setInputValue(_ => "");
                                     },
                                   (),
                                 ),
                               (),
                             )}>
                             <Item
                               id={listboxId ++ VirtualItem.key(item)}
                               ariaSetsize={Js.Array.length(filteredOptions)}
                               ariaPosinset={VirtualItem.index(item) + 1}
                               className=?optionClassName
                               ref={ReactDOM.Ref.callbackDomRef(
                                 Js.Array.unsafe_set(
                                   listRef.current,
                                   VirtualItem.index(item),
                                 ),
                               )}
                               style={ReactDOM.Style.make(
                                 ~transform=
                                   "translateY("
                                   ++ string_of_int(VirtualItem.start(item))
                                   ++ "px)",
                                 (),
                               )}
                               active={Option.value(
                                 activeIndex
                                 |> Option.map(
                                      Int.equal(VirtualItem.index(item)),
                                    ),
                                 ~default=false,
                               )}
                               selected={Option.value(
                                 selectedIndex
                                 |> Option.map(
                                      Int.equal(VirtualItem.index(item)),
                                    ),
                                 ~default=false,
                               )}>
                               {renderOption(
                                  filteredOptions[VirtualItem.index(item)],
                                )}
                             </Item>
                           </Utils.ReactHelper.Spread>
                         )
                       })
                    |> React.array}
                 </Listbox>}
          </ScrollContainer>
        </FloatingContainer>
      </Utils.ReactHelper.Spread>
    </FloatingUI.FloatingFocusManager>
  </FloatingUI.FloatingPortal>;
};
