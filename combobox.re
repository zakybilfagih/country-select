open ReactHelper;
open Bindings;
open StyleHelper;

module Item = {
  module Style = {
    let item = [%cx
      {|
      position: absolute;
      top: 0;
      left: 0;
      width: 100%;
      cursor: default;
      padding: 4px 10px;
      color: $(Colors.Light.textPrimary);
    |}
    ];

    let active = [%cx
      {|
      background-color: $(Colors.Light.backgroundControlHover);
    |}
    ];

    let selected = [%cx
      {|
      background-color: $(Colors.Light.backgroundSelected);
    |}
    ];
  };

  [@react.component]
  let make =
    React.forwardRef(
      (
        ~children,
        ~active,
        ~selected,
        ~id=?,
        ~className=?,
        ~onClick=?,
        ~onFocus=?,
        ~onMouseMove=?,
        ~onPointerLeave=?,
        ~role=?,
        ~style=?,
        ~ariaSetsize=?,
        ~ariaPosinset=?,
        ref,
      ) => {
      <li
        ?id
        ?onClick
        ?onFocus
        ?onMouseMove
        ?onPointerLeave
        ?role
        ?style
        ?ariaSetsize
        ?ariaPosinset
        ariaSelected=selected
        ref=?{ref |> Js.Nullable.toOption |> Option.map(ReactDOM.Ref.domRef)}
        className={Cn.make([|
          Style.item,
          textMd,
          selected ? Style.selected : active ? Style.active : "",
          Option.value(className, ~default=""),
        |])}>
        children
      </li>
    });
};

module Style = {
  let floating = [%cx
    {|
    display: flex;
    flex-direction: column;
    box-shadow: 0px 3px 18px 0px #00000026;
    background-color: $(Colors.Light.backgroundBox);
    border: 1px solid $(Colors.Light.borderFloatingAlpha);
    border-radius: 3px;
    overflow: hidden;
  |}
  ];

  let button = [%cx
    {|
    display: flex;
    gap: 5px;
    align-items: center;
    padding: 4px 10px;
    color: $(Colors.Light.textPrimary);
    background-color: $(Colors.Light.backgroundBox);
    border: 1px solid $(Colors.Light.borderControlAlpha);
    border-radius: 3px;
  |}
  ];

  let buttonTriangle = [%cx {|
    color: $(Colors.Light.textPrimary);
  |}];

  let searchIconContainer = [%cx {|
    display: flex;
  |}];

  let loadingIconContainer = [%cx
    {|
    display: flex;
    height: 14px;
    width: 14px;
  |}
  ];

  let input = [%cx
    {|
    border: none;
    padding: 8px;
    outline: none;
    padding: 4px 0;
    width: 100%;
    color: black;

    &::placeholder {
      color: $(Colors.Light.textTertiary);
    }
  |}
  ];

  let inputContainer = [%cx
    {|
    display: flex;
    align-items: center;
    gap: 8px;
    padding-block: 4px;
    padding-inline: 10px;
    border-bottom: 1px solid $(Colors.Light.borderLineAlpha);
  |}
  ];

  let scrollContainer = [%cx
    {|
    padding-block: 4px;
    overflow-y: auto;
    background-color: $(Colors.Light.backgroundBox);
  |}
  ];

  let listbox = [%cx
    {|
    width: 100%;
    position: relative;
    margin: 0px;
    padding: 0px;
    list-style: none;
  |}
  ];

  let noResult = [%cx
    {|
    margin: 0px;
    padding: 8px 10px;
    color: $(Colors.Light.textPrimary)
  |}
  ];
};

type buttonInteractions = {
  getReferenceProps: (~props: ReactDOM.domProps=?, unit) => ReactDOM.domProps,
  getFloatingProps: (~props: ReactDOM.domProps=?, unit) => ReactDOM.domProps,
};

let useButtonInteractions = (~context) => {
  let buttonInteractions =
    FloatingUi.UseInteraction.use([|
      FloatingUi.useRole(context, ()),
      FloatingUi.useClick(context),
      FloatingUi.useDismiss(context),
    |]);

  {
    getReferenceProps:
      FloatingUi.UseInteraction.getReferenceProps(buttonInteractions),
    getFloatingProps:
      FloatingUi.UseInteraction.getFloatingProps(buttonInteractions),
  };
};

type comboboxInteractions = {
  getInputProps: (~props: ReactDOM.domProps=?, unit) => ReactDOM.domProps,
  getListFloatingProps:
    (~props: ReactDOM.domProps=?, unit) => ReactDOM.domProps,
  getItemProps: (~props: ReactDOM.domProps=?, unit) => ReactDOM.domProps,
};

let useComboboxInteractions =
    (
      ~context,
      ~listRef,
      ~selectedIndex,
      ~activeIndex,
      ~setActiveIndex,
      ~open_,
    ) => {
  let comboboxInteractions =
    FloatingUi.UseInteraction.use([|
      FloatingUi.useListNavigation(
        context,
        ~props=
          FloatingUi.useListNavigationOptions(
            ~listRef,
            ~selectedIndex=Js.Nullable.fromOption(selectedIndex),
            ~activeIndex=Js.Nullable.fromOption(activeIndex),
            ~onNavigate=?{
              open_
                ? Some(
                    activeIndex =>
                      setActiveIndex(_ => Js.Nullable.toOption(activeIndex)),
                  )
                : None;
            },
            ~virtual_=true,
            (),
          ),
        (),
      ),
    |]);

  {
    getInputProps:
      FloatingUi.UseInteraction.getReferenceProps(comboboxInteractions),
    getListFloatingProps:
      FloatingUi.UseInteraction.getFloatingProps(comboboxInteractions),
    getItemProps:
      FloatingUi.UseInteraction.getItemProps(comboboxInteractions),
  };
};

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
       TextWidth.get(~text=label, ~font="normal 14px Arial")
       + 42
       + additionalContentWidth
     )
  |> Js.Array.concat(~other=[|0|])  // handle empty array
  |> Js.Math.maxMany_int;
};

[@react.component]
let make =
    (
      ~options: array('option),
      ~onSelect: option('option) => unit,
      ~selectedOption: option('option),
      ~getOptionLabel: 'option => string,
      ~optionEqual: ('option, 'option) => bool,
      ~renderOption: 'option => React.element,
      ~renderButtonLabel: option('option) => React.element,
      ~isLoading: bool=false,
      ~maxHeight: int=400,
      ~padding: int=25,
      ~additionalContentWidth: int=0,
      ~buttonAriaLabel: string="Choose item",
      ~noResultText: string="No item found.",
      ~buttonClassName: option(string)=?,
      ~inputClassName: option(string)=?,
      ~inputContainerClassName: option(string)=?,
      ~optionClassName: option(string)=?,
    ) => {
  let (open_, setOpen) = React.useState(_ => false);
  let (pointer, setPointer) = React.useState(_ => false);
  let (activeIndex, setActiveIndex) = React.useState(_ => None);

  let (computedMaxHeight, setComputedMaxHeight) =
    React.useState(_ => maxHeight);

  let (inputValue, setInputValue) = React.useState(_ => "");

  let noResultId = React.useId();
  let buttonId = React.useId();
  let listboxId = React.useId();

  if (!open_ && pointer) {
    setPointer(_ => false);
  };

  let listRef = React.useRef([||]);

  let {refs, floatingStyles, context, isPositioned}: FloatingUi.useFloatingReturn =
    FloatingUi.(
      useFloating(
        useFloatingOptions(
          ~open_,
          ~placement="bottom-start",
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
                ),
                flip(
                  flipOptions(
                    ~padding,
                    ~fallbackStrategy="initialPlacement",
                    (),
                  ),
                ),
              |]
            ),
          (),
        ),
      )
    );

  let filteredOptions =
    React.useMemo2(
      () => filterOptions(options, inputValue, getOptionLabel),
      (inputValue, options),
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

  let scrollRef = React.useRef(Js.Nullable.null);
  let rowVirtualizer =
    TanstackVirtual.Virtualizer.use(
      TanstackVirtual.Virtualizer.options(
        ~count=Js.Array.length(filteredOptions),
        ~overscan=5,
        ~estimateSize=_ => 26,
        ~getScrollElement=() => scrollRef.current,
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
          TanstackVirtual.Virtualizer.scrollToIndexOptions(
            ~align="start",
            (),
          ),
          rowVirtualizer,
        );
        setActiveIndex(_ => Some(selectedIndex));
      };
      None;
    },
    (rowVirtualizer, isPositioned, selectedIndex),
  );

  let {getReferenceProps, getFloatingProps} =
    useButtonInteractions(~context);
  let {getInputProps, getListFloatingProps, getItemProps} =
    useComboboxInteractions(
      ~context,
      ~listRef,
      ~selectedIndex,
      ~activeIndex,
      ~setActiveIndex,
      ~open_,
    );

  let floatingProps =
    Js.Obj.assign(
      Obj.magic(getFloatingProps(~props=getListFloatingProps(), ())),
      {"aria-activedescendant": Js.undefined},
    );

  let handleInputChange = (event: React.Event.Form.t) => {
    let target = event |> React.Event.Form.target;
    let value = target##value;
    setInputValue(_ => value);
    setActiveIndex(_ => None);
  };

  let handleInputKeyDown = (event: React.Event.Keyboard.t) =>
    if (React.Event.Keyboard.key(event) == "Enter"
        && Option.is_some(activeIndex)) {
      let activeIndex = Option.get(activeIndex);
      onSelect(Some(filteredOptions[activeIndex]));

      setActiveIndex(_ => None);
      setOpen(_ => false);
      setInputValue(_ => "");
    };

  <>
    <Spread props={getReferenceProps()}>
      <button
        type_="button"
        id=buttonId
        ref={ReactDOM.Ref.callbackDomRef(refs.setReference)}
        ariaLabel=buttonAriaLabel
        className={Cn.make([|
          Style.button,
          textMd,
          Option.value(buttonClassName, ~default=""),
        |])}>
        {renderButtonLabel(selectedOption)}
        <Icon.Triangle className=Style.buttonTriangle />
      </button>
    </Spread>
    {open_
       ? <FloatingUi.FloatingPortal>
           <FloatingUi.FloatingFocusManager context modal=false>
             <Spread props=floatingProps>
               <div
                 className=Style.floating
                 style={ReactDOM.Style.combine(
                   floatingStyles,
                   ReactDOM.Style.make(
                     ~maxHeight=string_of_int(computedMaxHeight) ++ "px",
                     (),
                   ),
                 )}
                 ref={ReactDOM.Ref.callbackDomRef(refs.setFloating)}
                 ariaLabelledby=buttonId>
                 <div
                   className={Cn.make([|
                     Style.inputContainer,
                     Option.value(inputContainerClassName, ~default=""),
                   |])}>
                   <div className=Style.searchIconContainer>
                     <Icon.Search />
                   </div>
                   <Spread
                     props={getInputProps(
                       ~props=
                         ReactDOM.domProps(
                           ~onChange=handleInputChange,
                           ~onKeyDown=handleInputKeyDown,
                           (),
                         ),
                       (),
                     )}>
                     <input
                       className={Cn.make([|
                         Style.input,
                         textMd,
                         Option.value(inputClassName, ~default=""),
                       |])}
                       value=inputValue
                       type_="text"
                       placeholder="Search"
                       ariaAutocomplete="list"
                       ariaExpanded=true
                       role="combobox"
                       ariaControls={
                         Js.Array.length(filteredOptions) == 0
                           ? noResultId : listboxId
                       }
                     />
                   </Spread>
                   {isLoading ? <Icon.Loading /> : React.null}
                 </div>
                 <div
                   ref={ReactDOM.Ref.domRef(scrollRef)}
                   className=Style.scrollContainer
                   style={ReactDOM.Style.make(
                     ~maxHeight=string_of_int(computedMaxHeight) ++ "px",
                     (),
                   )}
                   tabIndex=(-1)>
                   {Js.Array.length(filteredOptions) == 0
                      ? <p
                          id=noResultId
                          className={Cn.make([|Style.noResult, textMd|])}
                          role="region"
                          ariaAtomic=true
                          ariaLive="assertive">
                          {React.string(noResultText)}
                        </p>
                      : React.null}
                   <ul
                     id=listboxId
                     className=Style.listbox
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
                     onKeyDown={_ => {setPointer(_ => false)}}
                     onPointerMove={_ => {setPointer(_ => true)}}>
                     {rowVirtualizer
                      |> TanstackVirtual.Virtualizer.getVirtualItems
                      |> Js.Array.map(~f=virtualItem => {
                           open TanstackVirtual.Virtualizer;
                           let index = VirtualItem.index(virtualItem);
                           let item = filteredOptions[index];
                           let isActive =
                             Option.value(
                               Option.map(Int.equal(index), activeIndex),
                               ~default=false,
                             );
                           let isSelected =
                             Option.value(
                               Option.map(Int.equal(index), selectedIndex),
                               ~default=false,
                             );

                           <Spread
                             key={VirtualItem.key(virtualItem)}
                             props={getItemProps(
                               ~props=
                                 ReactDOM.domProps(
                                   ~onClick=
                                     _ => {
                                       onSelect(Some(item));

                                       setOpen(_ => false);
                                       setActiveIndex(_ => None);
                                       setInputValue(_ => "");
                                     },
                                   (),
                                 ),
                               (),
                             )}>
                             <Item
                               ref={ReactDOM.Ref.callbackDomRef(
                                 Js.Array.unsafe_set(listRef.current, index),
                               )}
                               id={listboxId ++ VirtualItem.key(virtualItem)}
                               active=isActive
                               selected=isSelected
                               style={ReactDOM.Style.make(
                                 ~height=
                                   string_of_int(
                                     VirtualItem.size(virtualItem),
                                   )
                                   ++ "px",
                                 ~transform=
                                   "translateY("
                                   ++ string_of_int(
                                        VirtualItem.start(virtualItem),
                                      )
                                   ++ "px)",
                                 (),
                               )}
                               ariaSetsize={Js.Array.length(filteredOptions)}
                               ariaPosinset={index + 1}
                               className=?optionClassName>
                               {renderOption(item)}
                             </Item>
                           </Spread>;
                         })
                      |> React.array}
                   </ul>
                 </div>
               </div>
             </Spread>
           </FloatingUi.FloatingFocusManager>
         </FloatingUi.FloatingPortal>
       : React.null}
  </>;
};
